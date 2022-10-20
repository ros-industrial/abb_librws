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

#include <abb_librws/v1_0/rws_client.h>
#include <abb_librws/v1_0/rws.h>
#include <abb_librws/rws_error.h>
#include <abb_librws/parsing.h>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/DOM/NodeList.h>

#include <sstream>
#include <stdexcept>

#include <iostream>


namespace abb :: rws :: v1_0
{
using namespace Poco::Net;


/***********************************************************************************************************************
 * Class definitions: RWSClient
 */

/************************************************************
 * Primary methods
 */

RWSClient::RWSClient(ConnectionOptions const& connection_options)
: connectionOptions_ {connection_options}
, session_ {connectionOptions_.ip_address, connectionOptions_.port}
, http_client_ {session_, connectionOptions_.username, connectionOptions_.password}
{
  session_.setTimeout(
    connectionOptions_.connection_timeout.count(),
    connectionOptions_.send_timeout.count(),
    connectionOptions_.receive_timeout.count()
  );

  // Make a request to the server to check connection and initiate authentification.
  getRobotWareSystem();
}


RWSClient::~RWSClient()
{
  try
  {
    logout();
  }

  catch (std::exception const& ex)
  {
    // Catch all exceptions in dtor.
    std::string info = boost::diagnostic_information(ex);
    std::clog << "Exception in RWSClient::~RWSClient(): " << info;
  }
}


RWSResult RWSClient::getContollerService()
{
  std::string uri = Services::CTRL;
  return parseContent(httpGet(uri));
}

RWSResult RWSClient::getConfigurationInstances(const std::string& topic, const std::string& type)
{
  std::string uri = generateConfigurationPath(topic, type) + Resources::INSTANCES;
  return parseContent(httpGet(uri));
}


RWSResult RWSClient::getMechanicalUnitStaticInfo(const std::string& mechunit)
{
  std::string uri = generateMechanicalUnitPath(mechunit) + "?resource=static";
  return parseContent(httpGet(uri));
}

RWSResult RWSClient::getMechanicalUnitDynamicInfo(const std::string& mechunit)
{
  std::string uri = generateMechanicalUnitPath(mechunit) + "?resource=dynamic";
  return parseContent(httpGet(uri));
}

RWSResult RWSClient::getMechanicalUnitJointTarget(const std::string& mechunit)
{
  std::string uri = generateMechanicalUnitPath(mechunit) + Resources::JOINTTARGET;
  return parseContent(httpGet(uri));
}

RWSResult RWSClient::getMechanicalUnitRobTarget(const std::string& mechunit,
                                                           Coordinate coordinate,
                                                           const std::string& tool,
                                                           const std::string& wobj)
{
  std::string uri = generateMechanicalUnitPath(mechunit) + Resources::ROBTARGET;

  std::string args = "";
  if (!tool.empty())
  {
    args += "&tool=" + tool;
  }
  if (!wobj.empty())
  {
    args += "&wobj=" + wobj;
  }

  const std::string coordinate_arg = "?coordinate=";
  switch (coordinate)
  {
    case Coordinate::BASE:
      uri += coordinate_arg + SystemConstants::General::COORDINATE_BASE + args;
    break;
    case Coordinate::WORLD:
      uri += coordinate_arg + SystemConstants::General::COORDINATE_WORLD + args;
    break;
    case Coordinate::TOOL:
      uri += coordinate_arg + SystemConstants::General::COORDINATE_TOOL + args;
    break;
    case Coordinate::WOBJ:
      uri += coordinate_arg + SystemConstants::General::COORDINATE_WOBJ + args;
    break;
    default:
      // If the "ACTIVE" enumeration is passed in (or any other non-identified value),
      // do not add any arguments to this command
    break;
  }

  return parseContent(httpGet(uri));
}

RWSResult RWSClient::getRobotWareSystem()
{
  std::string uri = Resources::RW_SYSTEM;
  return parseContent(httpGet(uri));
}

std::string RWSClient::getFile(const FileResource& resource)
{
  std::string uri = generateFilePath(resource);
  return httpGet(uri).content();
}

void RWSClient::uploadFile(const FileResource& resource, const std::string& file_content)
{
  std::string uri = generateFilePath(resource);
  std::string content = file_content;

  httpPut(uri, content);
}

void RWSClient::deleteFile(const FileResource& resource)
{
  std::string uri = generateFilePath(resource);

  httpDelete(uri);
}


void RWSClient::logout()
{
  std::string uri = Resources::LOGOUT;
  //ABB controller sometimes gives HTTP_OK in response, although according to the documentation it should be HTTP_NO_CONTENT
  httpGet(uri, {Poco::Net::HTTPResponse::HTTP_OK, Poco::Net::HTTPResponse::HTTP_NO_CONTENT});
}


void RWSClient::registerLocalUser(const std::string& username,
                                                  const std::string& application,
                                                  const std::string& location)
{
  std::string uri = Services::USERS;
  std::string content = "username=" + username +
             "&application=" + application +
             "&location=" + location +
             "&ulocale=" + SystemConstants::General::LOCAL;

  httpPost(uri, content);
}

void RWSClient::registerRemoteUser(const std::string& username,
                                                   const std::string& application,
                                                   const std::string& location)
{
  std::string uri = Services::USERS;
  std::string content = "username=" + username +
                        "&application=" + application +
                        "&location=" + location +
                        "&ulocale=" + SystemConstants::General::REMOTE;

  httpPost(uri, content);
}

/************************************************************
 * Auxiliary methods
 */

RWSResult RWSClient::parseContent(const POCOResult& poco_result)
{
  return parser_.parseString(poco_result.content());
}


std::string RWSClient::generateConfigurationPath(const std::string& topic, const std::string& type)
{
  return Resources::RW_CFG + "/" + topic + "/" + type;
}


std::string RWSClient::generateMechanicalUnitPath(const std::string& mechunit)
{
  return Resources::RW_MOTIONSYSTEM_MECHUNITS + "/" + mechunit;
}

std::string RWSClient::generateFilePath(const FileResource& resource)
{
  return Services::FILESERVICE + "/" + resource.directory + "/" + resource.filename;
}


POCOResult RWSClient::httpGet(const std::string& uri,
  std::set<Poco::Net::HTTPResponse::HTTPStatus> const& accepted_status)
{
  POCOResult const result = http_client_.httpGet(uri);

  if (accepted_status.find(result.httpStatus()) == accepted_status.end())
    BOOST_THROW_EXCEPTION(ProtocolError {"HTTP response status not accepted"}
      << HttpMethodErrorInfo {"GET"}
      << UriErrorInfo {uri}
      << HttpStatusErrorInfo {result.httpStatus()}
      << HttpResponseContentErrorInfo {result.content()}
      << HttpReasonErrorInfo {result.reason()}
    );

  return result;
}


POCOResult RWSClient::httpPost(const std::string& uri, const std::string& content,
  std::set<Poco::Net::HTTPResponse::HTTPStatus> const& accepted_status)
{
  POCOResult const result = http_client_.httpPost(uri, content);

  if (accepted_status.find(result.httpStatus()) == accepted_status.end())
    BOOST_THROW_EXCEPTION(ProtocolError {"HTTP response status not accepted"}
      << HttpMethodErrorInfo {"POST"}
      << UriErrorInfo {uri}
      << HttpStatusErrorInfo {result.httpStatus()}
      << HttpResponseContentErrorInfo {result.content()}
      << HttpRequestContentErrorInfo {content}
      << HttpReasonErrorInfo {result.reason()}
    );

  return result;
}


POCOResult RWSClient::httpPut(const std::string& uri, const std::string& content,
  std::set<Poco::Net::HTTPResponse::HTTPStatus> const& accepted_status)
{
  POCOResult const result = http_client_.httpPut(uri, content);
  if (accepted_status.find(result.httpStatus()) == accepted_status.end())
    BOOST_THROW_EXCEPTION(ProtocolError {"HTTP response status not accepted"}
      << HttpMethodErrorInfo {"PUT"}
      << UriErrorInfo {uri}
      << HttpStatusErrorInfo {result.httpStatus()}
      << HttpResponseContentErrorInfo {result.content()}
      << HttpRequestContentErrorInfo {content}
      << HttpReasonErrorInfo {result.reason()}
    );

  return result;
}


POCOResult RWSClient::httpDelete(const std::string& uri,
  std::set<Poco::Net::HTTPResponse::HTTPStatus> const& accepted_status)
{
  POCOResult const result = http_client_.httpDelete(uri);
  if (accepted_status.find(result.httpStatus()) == accepted_status.end())
    BOOST_THROW_EXCEPTION(ProtocolError {"HTTP response status not accepted"}
      << HttpMethodErrorInfo {"DELETE"}
      << HttpStatusErrorInfo {result.httpStatus()}
      << HttpResponseContentErrorInfo {result.content()}
      << UriErrorInfo {uri}
      << HttpReasonErrorInfo {result.reason()}
    );

  return result;
}


Poco::Net::WebSocket RWSClient::receiveSubscription(std::string const& subscription_group_id)
{
  return http_client_.webSocketConnect("/poll/" + subscription_group_id, "robapi2_subscription",
    Poco::Net::HTTPClientSession {connectionOptions_.ip_address, connectionOptions_.port});
}
}
