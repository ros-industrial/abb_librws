/***********************************************************************************************************************
 *
 * Copyright (c) 2015, ABB Schweiz AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that
 * the following conditions are met:
 *
 *    * Redistributions of source code must retain the
 *      above copyright notice, this list of conditions
 *      and the following disclaimer.
 *    * Redistributions in binary form must reproduce the
 *      above copyright notice, this list of conditions
 *      and the following disclaimer in the documentation
 *      and/or other materials provided with the
 *      distribution.
 *    * Neither the name of ABB nor the names of its
 *      contributors may be used to endorse or promote
 *      products derived from this software without
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***********************************************************************************************************************
 */

#include <sstream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/NetException.h>
#include <Poco/StreamCopier.h>

#include <abb_librws/rws_poco_client.h>
#include <abb_librws/rws_error.h>

#include <boost/log/trivial.hpp>


using namespace Poco;
using namespace Poco::Net;

namespace abb
{
namespace rws
{
/***********************************************************************************************************************
 * Class definitions: POCOClient
 */

POCOClient::POCOClient(
  Poco::Net::HTTPClientSession& session,
  const std::string& username,
  const std::string& password)
: http_client_session_ {session}
, http_credentials_ {username, password}
{
  http_client_session_.setKeepAlive(true);
}

POCOClient::~POCOClient()
{
}

/************************************************************
 * Primary methods
 */

POCOResult POCOClient::httpGet(const std::string& uri)
{
  return makeHTTPRequest(HTTPRequest::HTTP_GET, uri);
}

POCOResult POCOClient::httpPost(const std::string& uri, const std::string& content, const std::string& content_type)
{
  return makeHTTPRequest(HTTPRequest::HTTP_POST, uri, content, content_type);
}

POCOResult POCOClient::httpPut(const std::string& uri, const std::string& content, const std::string& content_type)
{
  return makeHTTPRequest(HTTPRequest::HTTP_PUT, uri, content, content_type);
}

POCOResult POCOClient::httpDelete(const std::string& uri)
{
  return makeHTTPRequest(HTTPRequest::HTTP_DELETE, uri);
}

POCOResult POCOClient::makeHTTPRequest(const std::string& method,
                                                   const std::string& uri,
                                                   const std::string& content,
                                                   const std::string& content_type)
{
  // The response and the request.
  HTTPResponse response;
  std::string response_content;

  HTTPRequest request(method, uri, HTTPRequest::HTTP_1_1);
  request.add("accept", "application/xhtml+xml;v=2.0");
  request.setCookies(cookies_);
  request.setContentLength(content.length());

  if (!content_type.empty())
  {
    request.setContentType(content_type);
  } else if (method == HTTPRequest::HTTP_POST || !content.empty())
  {
    request.setContentType("application/x-www-form-urlencoded");
  }

  // Attempt the communication.
  try
  {

    BOOST_LOG_TRIVIAL(debug) << "Tyring to " << method << " from uri=" << uri << " request_content=" << content << "\n";
    sendAndReceive(request, response, content, response_content);

    BOOST_LOG_TRIVIAL(debug) << "Got status=" << response.getStatus() << " when " << method
                             << "ing from uri=" << uri << " response_content=" << response_content << "\n";
    // Check if the server has sent an update for the cookies.
    std::vector<HTTPCookie> temp_cookies;
    response.getCookies(temp_cookies);
    for (size_t i = 0; i < temp_cookies.size(); ++i)
    {
      if (cookies_.find(temp_cookies[i].getName()) != cookies_.end())
      {
        cookies_.set(temp_cookies[i].getName(), temp_cookies[i].getValue());
      }
      else
      {
        cookies_.add(temp_cookies[i].getName(), temp_cookies[i].getValue());
      }
    }

    // Check if there was a server error, if so, make another attempt with a clean sheet.
    if (response.getStatus() >= HTTPResponse::HTTP_INTERNAL_SERVER_ERROR)
    {
      http_client_session_.reset();
      request.erase(HTTPRequest::COOKIE);
      sendAndReceive(request, response, content, response_content);
    }

    // Check if the request was unauthorized, if so add credentials.
    if (response.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED)
    {
      BOOST_LOG_TRIVIAL(debug) << "Tyring to SECURELY " << method << " from uri=" << uri << "\n";
      authenticate(request, response, content, response_content);
    }


    return POCOResult {response.getStatus(), response.getReason(), response, response_content};
  }
  catch (CommunicationError const&)
  {
    cookies_.clear();
    http_client_session_.reset();

    throw;
  }
}


Poco::Net::WebSocket POCOClient::webSocketConnect(const std::string& uri, const std::string& protocol, Poco::Net::HTTPClientSession&& session)
{
  // The response and the request.
  HTTPResponse response;
  HTTPRequest request(HTTPRequest::HTTP_GET, uri, HTTPRequest::HTTP_1_1);
  request.set("Sec-WebSocket-Protocol", protocol);
  request.setCookies(cookies_);

  // Attempt the communication.
  try
  {
    Poco::Net::WebSocket websocket {session, request, response};

    if (response.getStatus() != HTTPResponse::HTTP_SWITCHING_PROTOCOLS)
      BOOST_THROW_EXCEPTION(
        ProtocolError {"webSocketConnect() failed"}
          << HttpStatusErrorInfo {response.getStatus()}
          << HttpReasonErrorInfo {response.getReason()}
          << HttpMethodErrorInfo {request.getMethod()}
          << UriErrorInfo {uri}
      );

    return websocket;
  }
  catch (Poco::Exception const& e)
  {
    BOOST_THROW_EXCEPTION(
      CommunicationError {"webSocketConnect() failed: " + e.displayText()}
        << HttpStatusErrorInfo {response.getStatus()}
        << HttpReasonErrorInfo {response.getReason()}
        << HttpMethodErrorInfo {request.getMethod()}
        << UriErrorInfo {uri}
        << boost::errinfo_nested_exception {boost::current_exception()}
    );
  }
}


/************************************************************
 * Auxiliary methods
 */

void POCOClient::sendAndReceive(HTTPRequest& request,
                                HTTPResponse& response,
                                const std::string& request_content,
                                std::string& response_content)
{
  HTTPInfo log_entry;

  // Add request info to the log entry.
  log_entry.addHTTPRequestInfo(request, request_content);

  // Contact the server.

  try
  {
    std::ostream& request_content_stream = http_client_session_.sendRequest(request);
    request_content_stream << request_content;
  }
  catch (Poco::Exception const& e)
  {
    BOOST_THROW_EXCEPTION(
      CommunicationError {"HTTP send error: " + e.displayText()}
        << HttpMethodErrorInfo {request.getMethod()}
        << UriErrorInfo {request.getURI()}
        << HttpRequestContentErrorInfo {request_content}
        << boost::errinfo_nested_exception {boost::current_exception()}
    );
  }

  try
  {
    std::istream& response_content_stream = http_client_session_.receiveResponse(response);

    response_content.clear();
    StreamCopier::copyToString(response_content_stream, response_content);
  }
  catch (Poco::Exception const& e)
  {
    BOOST_THROW_EXCEPTION(
      CommunicationError {"HTTP receive error: " + e.displayText()}
        << HttpMethodErrorInfo {request.getMethod()}
        << UriErrorInfo {request.getURI()}
        << HttpRequestContentErrorInfo {request_content}
        << boost::errinfo_nested_exception {boost::current_exception()}
    );
  }

  // Add response info to the log entry.
  log_entry.addHTTPResponseInfo(response, response_content);

  // Add entry to the log
  if (log_.size() >= LOG_SIZE)
  {
    log_.pop_back();
  }

  log_.push_front(log_entry);
}


void POCOClient::authenticate(HTTPRequest& request,
                              HTTPResponse& response,
                              const std::string& request_content,
                              std::string& response_content)
{
  // Remove any old cookies.
  cookies_.clear();

  BOOST_LOG_TRIVIAL(debug) << "Authenticating to " << request.getMethod() << " from uri=" << request.getURI() << "\n";
  // Authenticate with the provided credentials.
  http_credentials_.authenticate(request, response);
  BOOST_LOG_TRIVIAL(debug) << "Got status=" << response.getStatus() << " when authenticating to"
                           << request.getMethod() << " to uri=" <<  request.getURI() << "\n";

  // Contact the server, and extract and store the received cookies.

  BOOST_LOG_TRIVIAL(debug) << "After authentication, trying to " << response.getStatus() << " when authenticating to"
                           << request.getMethod() << " to uri=" <<  request.getURI() << "\n";
  sendAndReceive(request, response, request_content, response_content);
  BOOST_LOG_TRIVIAL(debug) << "After authentication got status=" << response.getStatus() << " when "
                           << request.getMethod() << "ing to uri=" <<  request.getURI() << "\n";
  std::vector<HTTPCookie> temp_cookies;
  response.getCookies(temp_cookies);

  for (size_t i = 0; i < temp_cookies.size(); ++i)
  {
    extractAndStoreCookie(temp_cookies[i].toString());
  }
}

void POCOClient::extractAndStoreCookie(const std::string& cookie_string)
{
  // Find the positions of the cookie delimiters.
  size_t position_1 = cookie_string.find_first_of("=");
  size_t position_2 = cookie_string.find_first_of(";");

  // Extract and store the cookie (if the delimiters was found).
  if (position_1 != std::string::npos && position_2 != std::string::npos)
  {
    std::string result = cookie_string.substr(0, position_1++);
    std::string result2 = cookie_string.substr(position_1, position_2 - position_1);

    cookies_.add(result, result2);
  }
}


std::string POCOClient::getLogText(bool verbose) const
{
  if (log_.size() == 0)
  {
    return "";
  }

  std::stringstream ss;

  for (size_t i = 0; i < log_.size(); ++i)
  {
    std::stringstream temp;
    temp << i + 1 << ". ";
    ss << temp.str() << log_[i].toString(verbose, temp.str().size()) << std::endl;
  }

  return ss.str();
}


std::string POCOClient::getLogTextLatestEvent(bool verbose) const
{
  return (log_.size() == 0 ? "" : log_[0].toString(verbose, 0));
}


void POCOClient::HTTPInfo::addHTTPRequestInfo(const Poco::Net::HTTPRequest& request,
                                                const std::string& request_content)
{
  this->request = {request.getMethod(), request.getURI(), request_content};
}


void POCOClient::HTTPInfo::addHTTPResponseInfo(const Poco::Net::HTTPResponse& response,
                                                const std::string& response_content)
{
  std::string header_info;

  for (HTTPResponse::ConstIterator i = response.begin(); i != response.end(); ++i)
  {
    header_info += i->first + "=" + i->second + "\n";
  }

  this->response = HTTPInfo::ResponseInfo {response.getStatus(), header_info, response_content};
}


std::string POCOClient::HTTPInfo::toString(bool verbose, size_t indent) const
{
  std::stringstream ss;

  std::string seperator = (indent == 0 ? " | " : "\n" + std::string(indent, ' '));

  if (request)
  {
    ss << seperator << "HTTP Request: " << request->method << " " << request->uri;
  }

  if (response)
  {
    ss << seperator << "HTTP Response: " << response->status << " - "
      << HTTPResponse::getReasonForStatus(response->status);

    if (verbose)
    {
      ss << seperator << "HTTP Response Content: " << response->content;
    }
  }

  return ss.str();
}

} // end namespace rws
} // end namespace abb
