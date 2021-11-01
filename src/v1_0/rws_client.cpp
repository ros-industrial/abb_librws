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

#include <sstream>
#include <stdexcept>

#include <iostream>


namespace
{
static const char EXCEPTION_CREATE_STRING[]{"Failed to create string"};
}

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
  catch (std::exception const& e)
  {
    // Catch all exceptions in dtor.
    std::cerr << "Exception in RWSClient::~RWSClient(): " << e.what() << std::endl;
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

RWSResult RWSClient::getIOSignals()
{
  std::string const & uri = Resources::RW_IOSYSTEM_SIGNALS;
  return parseContent(httpGet(uri));
}

RWSResult RWSClient::getIOSignal(const std::string& iosignal)
{
  try
  {
    std::string uri = generateIOSignalPath(iosignal);
    return parseContent(httpGet(uri));
  }
  catch (boost::exception& e)
  {
    e << IoSignalErrorInfo {iosignal};
    throw;
  }
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

RWSResult RWSClient::getSpeedRatio()
{
  std::string uri = "/rw/panel/speedratio";
  return parseContent(httpGet(uri));
}

RWSResult RWSClient::getPanelControllerState()
{
  std::string uri = Resources::RW_PANEL_CTRLSTATE;
  return parseContent(httpGet(uri));
}

RWSResult RWSClient::getPanelOperationMode()
{
  std::string uri = Resources::RW_PANEL_OPMODE;
  return parseContent(httpGet(uri));
}

void RWSClient::setIOSignal(const std::string& iosignal, const std::string& value)
{
  try
  {
    std::string uri = generateIOSignalPath(iosignal) + "?" + Queries::ACTION_SET;
    std::string content = Identifiers::LVALUE + "=" + value;

    httpPost(uri, content);
  }
  catch (boost::exception& e)
  {
    e << IoSignalErrorInfo {iosignal};
    throw;
  }
}

void RWSClient::setMotorsOn()
{
  std::string uri = Resources::RW_PANEL_CTRLSTATE + "?" + Queries::ACTION_SETCTRLSTATE;
  std::string content = "ctrl-state=motoron";

  httpPost(uri, content);
}

void RWSClient::setMotorsOff()
{
  std::string uri = Resources::RW_PANEL_CTRLSTATE + "?" + Queries::ACTION_SETCTRLSTATE;
  std::string content = "ctrl-state=motoroff";

  httpPost(uri, content);
}

void RWSClient::setSpeedRatio(unsigned int ratio)
{
  if(ratio > 100) throw std::out_of_range("Speed ratio argument out of range (should be 0 <= ratio <= 100)");

  std::stringstream ss;
  ss << ratio;
  if(ss.fail()) throw std::runtime_error(EXCEPTION_CREATE_STRING);

  std::string uri = "/rw/panel/speedratio?action=setspeedratio";
  std::string content = "speed-ratio=" + ss.str();

  httpPost(uri, content);
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
  httpGet(uri);
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

std::string RWSClient::generateIOSignalPath(const std::string& iosignal)
{
  return Resources::RW_IOSYSTEM_SIGNALS + "/" + iosignal;
}

std::string RWSClient::generateMechanicalUnitPath(const std::string& mechunit)
{
  return Resources::RW_MOTIONSYSTEM_MECHUNITS + "/" + mechunit;
}

std::string RWSClient::generateFilePath(const FileResource& resource)
{
  return Services::FILESERVICE + "/" + resource.directory + "/" + resource.filename;
}


POCOResult RWSClient::httpGet(const std::string& uri)
{
  POCOResult const result = http_client_.httpGet(uri);

  if (result.httpStatus() != HTTPResponse::HTTP_OK)
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


POCOResult RWSClient::httpPut(const std::string& uri, const std::string& content)
{
  POCOResult const result = http_client_.httpPut(uri, content);
  if (result.httpStatus() != HTTPResponse::HTTP_OK && result.httpStatus() != HTTPResponse::HTTP_CREATED)
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


POCOResult RWSClient::httpDelete(const std::string& uri)
{
  POCOResult const result = http_client_.httpDelete(uri);
  if (result.httpStatus() != HTTPResponse::HTTP_OK && result.httpStatus() != HTTPResponse::HTTP_NO_CONTENT)
    BOOST_THROW_EXCEPTION(ProtocolError {"HTTP response status not accepted"}
      << HttpMethodErrorInfo {"DELETE"}
      << HttpStatusErrorInfo {result.httpStatus()}
      << HttpResponseContentErrorInfo {result.content()}
      << UriErrorInfo {uri}
      << HttpReasonErrorInfo {result.reason()}
    );

  return result;
}


std::string RWSClient::openSubscription(std::vector<std::pair<std::string, SubscriptionPriority>> const& resources)
{
  // Generate content for a subscription HTTP post request.
  std::stringstream subscription_content;
  for (std::size_t i = 0; i < resources.size(); ++i)
  {
    subscription_content << "resources=" << i
                          << "&"
                          << i << "=" << resources[i].first
                          << "&"
                          << i << "-p=" << static_cast<int>(resources[i].second)
                          << (i < resources.size() - 1 ? "&" : "");
  }

  // Make a subscription request.
  POCOResult const poco_result = http_client_.httpPost(Services::SUBSCRIPTION, subscription_content.str());

  if (poco_result.httpStatus() != HTTPResponse::HTTP_CREATED)
    BOOST_THROW_EXCEPTION(
      ProtocolError {"Unable to create Subscription"}
      << HttpStatusErrorInfo {poco_result.httpStatus()}
      << HttpReasonErrorInfo {poco_result.reason()}
      << HttpMethodErrorInfo {HTTPRequest::HTTP_POST}
      << HttpRequestContentErrorInfo {subscription_content.str()}
      << HttpResponseContentErrorInfo {poco_result.content()}
      << HttpResponseErrorInfo {poco_result}
      << UriErrorInfo {Services::SUBSCRIPTION}
    );

  std::string subscription_group_id;

  // Find "Location" header attribute
  auto const h = std::find_if(
    poco_result.headerInfo().begin(), poco_result.headerInfo().end(),
    [] (auto const& p) { return p.first == "Location"; });

  if (h != poco_result.headerInfo().end())
  {
    std::string const poll = "/poll/";
    auto const start_postion = h->second.find(poll);

    if (start_postion != std::string::npos)
      subscription_group_id = h->second.substr(start_postion + poll.size());
  }

  if (subscription_group_id.empty())
    BOOST_THROW_EXCEPTION(ProtocolError {"Cannot get subscription group from HTTP response"});

  return subscription_group_id;
}


void RWSClient::closeSubscription(std::string const& subscription_group_id)
{
  // Unsubscribe from events
  std::string const uri = Services::SUBSCRIPTION + "/" + subscription_group_id;
  httpDelete(uri);
}


Poco::Net::WebSocket RWSClient::receiveSubscription(std::string const& subscription_group_id)
{
  return http_client_.webSocketConnect("/poll/" + subscription_group_id, "robapi2_subscription",
    Poco::Net::HTTPClientSession {connectionOptions_.ip_address, connectionOptions_.port});
}


std::string RWSClient::getResourceURI(IOSignalResource const& io_signal) const
{
  std::string resource_uri = Resources::RW_IOSYSTEM_SIGNALS;
  resource_uri += "/";
  resource_uri += io_signal.name;
  resource_uri += ";";
  resource_uri += Identifiers::STATE;
  return resource_uri;
}


std::string RWSClient::getResourceURI(RAPIDResource const& resource) const
{
  std::string resource_uri = Resources::RW_RAPID_SYMBOL_DATA_RAPID;
  resource_uri += "/";
  resource_uri += resource.task;
  resource_uri += "/";
  resource_uri += resource.module;
  resource_uri += "/";
  resource_uri += resource.name;
  resource_uri += ";";
  resource_uri += Identifiers::VALUE;
  return resource_uri;
}


std::string RWSClient::getResourceURI(RAPIDExecutionStateResource const&) const
{
  return "/rw/rapid/execution;ctrlexecstate";
}


void RWSClient::processEvent(Poco::AutoPtr<Poco::XML::Document> doc, SubscriptionCallback& callback) const
{
  // IMPORTANT: don't use AutoPtr<XML::Node> here! Otherwise you will get memory corruption.
  Poco::XML::Node const * li_node = doc->getNodeByPath("html/body/div/ul/li");
  if (!li_node)
    BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS event message: can't find XML path html/body/div/ul/li"});

  auto const * a_node = li_node->getNodeByPath("a");
  if (!a_node)
    BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS event message: can't find XML path html/body/div/ul/li/a"});

  std::string uri;
  uri = xmlNodeGetAttributeValue(a_node, "href");

  std::string const class_attribute_value = xmlNodeGetAttributeValue(li_node, "class");

  if (class_attribute_value == "ios-signalstate-ev")
  {
    IOSignalStateEvent event;
    std::string const prefix = "/rw/iosystem/signals/";

    if (uri.find(prefix) != 0)
      BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS event message: invalid resource URI"} << UriErrorInfo {uri});

    event.signal = uri.substr(prefix.length(), uri.find(";") - prefix.length());
    event.value = xmlFindTextContent(li_node, XMLAttribute {"class", "lvalue"});
    callback.processEvent(event);
  }
  else if (class_attribute_value == "rap-ctrlexecstate-ev")
  {
    RAPIDExecutionStateEvent event;
    event.state = rw::makeRAPIDExecutionState(xmlFindTextContent(li_node, XMLAttribute {"class", "ctrlexecstate"}));
    callback.processEvent(event);
  }
  else
    BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS event message: unrecognized class " + class_attribute_value});
}
}
