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

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/NetException.h"
#include "Poco/StreamCopier.h"

#include "abb_librws/rws_poco_client.h"

using namespace Poco;
using namespace Poco::Net;

namespace abb
{
namespace rws
{
/***********************************************************************************************************************
 * Class definitions: POCOClient
 */

/************************************************************
 * Primary methods
 */

POCOResult POCOClient::httpGet(const std::string& uri)
{
  return makeHTTPRequest(HTTPRequest::HTTP_GET, uri);
}

POCOResult POCOClient::httpPost(const std::string& uri, const std::string& content)
{
  return makeHTTPRequest(HTTPRequest::HTTP_POST, uri, content);
}

POCOResult POCOClient::httpPut(const std::string& uri, const std::string& content)
{
  return makeHTTPRequest(HTTPRequest::HTTP_PUT, uri, content);
}

POCOResult POCOClient::httpDelete(const std::string& uri)
{
  return makeHTTPRequest(HTTPRequest::HTTP_DELETE, uri);
}

POCOResult POCOClient::makeHTTPRequest(const std::string& method,
                                                   const std::string& uri,
                                                   const std::string& content)
{
  // Lock the object's mutex. It is released when the method goes out of scope.
  ScopedLock<Mutex> lock(http_mutex_);

  // Result of the communication.
  POCOResult result;

  // The response and the request.
  HTTPResponse response;
  HTTPRequest request(method, uri, HTTPRequest::HTTP_1_1);
  request.setCookies(cookies_);
  request.setContentLength(content.length());
  if (method == HTTPRequest::HTTP_POST || !content.empty())
  {
    request.setContentType("application/x-www-form-urlencoded");
  }

  // Attempt the communication.
  try
  {
    sendAndReceive(result, request, response, content);

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
      sendAndReceive(result, request, response, content);
    }

    // Check if the request was unauthorized, if so add credentials.
    if (response.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED)
    {
      authenticate(result, request, response, content);
    }

    result.status = POCOResult::OK;
  }
  catch (Poco::Exception const&)
  {
    cookies_.clear();
    http_client_session_.reset();

    throw;
  }

  return result;
}


Poco::Net::WebSocket POCOClient::webSocketConnect(const std::string& uri,
                                                    const std::string& protocol)
{
  // Lock the object's mutex. It is released when the method goes out of scope.
  ScopedLock<Mutex> lock(http_mutex_);

  // The response and the request.
  HTTPResponse response;
  HTTPRequest request(HTTPRequest::HTTP_GET, uri, HTTPRequest::HTTP_1_1);
  request.set("Sec-WebSocket-Protocol", protocol);
  request.setCookies(cookies_);

  // Attempt the communication.
  try
  {
    Poco::Net::WebSocket websocket {http_client_session_, request, response};
    if (response.getStatus() != HTTPResponse::HTTP_SWITCHING_PROTOCOLS)
      throw std::runtime_error("webSocketConnect() failed: HTTP response " + std::to_string(response.getStatus()));

    return websocket;
  }
  catch (std::exception const& e)
  {
    // Should we really reset the session if creating the WebSocket failed?
    http_client_session_.reset();
    throw;
  }
}


/************************************************************
 * Auxiliary methods
 */

void POCOClient::sendAndReceive(POCOResult& result,
                                HTTPRequest& request,
                                HTTPResponse& response,
                                const std::string& request_content)
{
  // Add request info to the result.
  result.addHTTPRequestInfo(request, request_content);

  // Contact the server.
  std::string response_content;
  http_client_session_.sendRequest(request) << request_content;
  StreamCopier::copyToString(http_client_session_.receiveResponse(response), response_content);

  // Add response info to the result.
  result.addHTTPResponseInfo(response, response_content);
}

void POCOClient::authenticate(POCOResult& result,
                              HTTPRequest& request,
                              HTTPResponse& response,
                              const std::string& request_content)
{
  // Remove any old cookies.
  cookies_.clear();

  // Authenticate with the provided credentials.
  http_credentials_.authenticate(request, response);

  // Contact the server, and extract and store the received cookies.
  sendAndReceive(result, request, response, request_content);
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

} // end namespace rws
} // end namespace abb
