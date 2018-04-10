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

#include "Poco/SAX/InputSource.h"

#include "abb_librws/rws_client.h"

namespace abb
{
namespace rws
{
using namespace Poco::Net;

typedef SystemConstants::RWS::Identifiers   Identifiers;
typedef SystemConstants::RWS::Queries       Queries;
typedef SystemConstants::RWS::Resources     Resources;
typedef SystemConstants::RWS::Services      Services;
typedef SystemConstants::RWS::XMLAttributes XMLAttributes;

/***********************************************************************************************************************
 * Class definitions: RWSClient::SubscriptionResources
 */

/************************************************************
 * Primary methods
 */

void RWSClient::SubscriptionResources::addIOSignal(const std::string iosignal, const Priority priority)
{
  std::string resource_uri = Resources::RW_IOSYSTEM_SIGNALS;
  resource_uri += "/";
  resource_uri += iosignal;
  resource_uri += ";";
  resource_uri += Identifiers::STATE;

  add(resource_uri, priority);
}

void RWSClient::SubscriptionResources::addRAPIDPersistantVariable(const RAPIDResource resource, const Priority priority)
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

  add(resource_uri, priority);
}

void RWSClient::SubscriptionResources::add(const std::string resource_uri, const Priority priority)
{
  resources_.push_back(SubscriptionResource(resource_uri, priority));
}




/***********************************************************************************************************************
 * Class definitions: RWSClient
 */
  
/************************************************************
 * Primary methods
 */

RWSClient::RWSResult RWSClient::getIOSignal(const std::string iosignal)
{
  uri_ = generateIOSignalPath(iosignal);

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getMechanicalUnitJointTarget(const std::string mechunit)
{
  uri_ = generateMechanicalUnitPath(mechunit) + Resources::JOINTTARGET;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getMechanicalUnitRobTarget(const std::string mechunit)
{
  uri_ = generateMechanicalUnitPath(mechunit) + Resources::ROBTARGET;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getRAPIDExecution()
{
  uri_ = Resources::RW_RAPID_EXECUTION;
  
  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getRAPIDTasks()
{
  uri_ = Resources::RW_RAPID_TASKS;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getRobotWareSystem()
{
  uri_ = Resources::RW_SYSTEM;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getPanelControllerState()
{
  uri_ = Resources::RW_PANEL_CTRLSTATE;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getPanelOperationMode()
{
  uri_ = Resources::RW_PANEL_OPMODE;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getRAPIDSymbolData(const RAPIDResource resource)
{
  uri_ = generateRAPIDDataPath(resource);

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getRAPIDSymbolData(const RAPIDResource resource, RAPIDSymbolDataAbstract* p_data)
{
  RWSResult result;
  std::string data_type;

  if (p_data)
  {
    RWSResult temp_result = getRAPIDSymbolProperties(resource);
    
    if (temp_result.success)
    {
      data_type = xmlFindTextContent(temp_result.p_xml_document, XMLAttributes::CLASS_DATTYP);

      if (p_data->getType().compare(data_type) == 0)
      {
        result = getRAPIDSymbolData(resource);

        if (result.success)
        {
          std::string value = xmlFindTextContent(result.p_xml_document, XMLAttributes::CLASS_VALUE);

          if (!value.empty())
          {
            p_data->parseRWSValueString(value);
          }
          else
          {
            result.success = false;
          }
        }
      }
    }
  }

  return result;
}

RWSClient::RWSResult RWSClient::getRAPIDSymbolProperties(const RAPIDResource resource)
{
  uri_ = generateRAPIDPropertiesPath(resource);

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::setIOSignal(const std::string iosignal, const std::string value)
{
  uri_ = generateIOSignalPath(iosignal) + "?" + Queries::ACTION_SET;
  content_ = Identifiers::LVALUE + "=" + value;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_NO_CONTENT);

  return evaluatePOCOResult(httpPost(uri_, content_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::setRAPIDSymbolData(const RAPIDResource resource, const std::string data)
{
  uri_ = generateRAPIDDataPath(resource) + "?" + Queries::ACTION_SET;
  content_ = Identifiers::VALUE + "=" + data;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_NO_CONTENT);

  return evaluatePOCOResult(httpPost(uri_, content_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::setRAPIDSymbolData(const RAPIDResource resource, RAPIDSymbolDataAbstract& data)
{
  return setRAPIDSymbolData(resource, data.constructRWSValueString());
}

RWSClient::RWSResult RWSClient::startRAPIDExecution()
{
  uri_ = Resources::RW_RAPID_EXECUTION + "?" + Queries::ACTION_START;
  content_ = "regain=continue&execmode=continue&cycle=forever&condition=none&stopatbp=disabled&alltaskbytsp=false";

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_NO_CONTENT);

  return evaluatePOCOResult(httpPost(uri_, content_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::stopRAPIDExecution()
{
  uri_ = Resources::RW_RAPID_EXECUTION + "?" + Queries::ACTION_STOP;
  content_ = "stopmode=stop";

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_NO_CONTENT);

  return evaluatePOCOResult(httpPost(uri_, content_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::resetRAPIDProgramPointer()
{
  uri_ = Resources::RW_RAPID_EXECUTION + "?" + Queries::ACTION_RESETPP;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_NO_CONTENT);

  return evaluatePOCOResult(httpPost(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::setMotorsOn()
{
  uri_ = Resources::RW_PANEL_CTRLSTATE + "?" + Queries::ACTION_SETCTRLSTATE;
  content_ = "ctrl-state=motoron";

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_NO_CONTENT);

  return evaluatePOCOResult(httpPost(uri_, content_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::setMotorsOff()
{
  uri_ = Resources::RW_PANEL_CTRLSTATE + "?" + Queries::ACTION_SETCTRLSTATE;
  content_ = "ctrl-state=motoroff";

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_NO_CONTENT);

  return evaluatePOCOResult(httpPost(uri_, content_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::getFile(const FileResource resource, std::string* p_file_content)
{
  RWSResult rws_result;
  POCOClient::POCOResult poco_result;

  if (p_file_content)
  {
    uri_ = generateFilePath(resource);
    poco_result = httpGet(uri_);

    evaluation_conditions_.reset();
    evaluation_conditions_.parse_message_into_xml = false;
    evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

    rws_result = evaluatePOCOResult(poco_result, evaluation_conditions_);

    if (rws_result.success)
    {
      *p_file_content = poco_result.poco_info.http.response.content;
    }
  }

  return rws_result;
}

RWSClient::RWSResult RWSClient::uploadFile(const FileResource resource, const std::string file_content)
{
  uri_ = generateFilePath(resource);
  content_ = file_content;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_CREATED);

  return evaluatePOCOResult(httpPut(uri_, content_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::deleteFile(const FileResource resource)
{
  uri_ = generateFilePath(resource);

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpDelete(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::startSubscription(SubscriptionResources resources)
{
  RWSResult result;

  if (!webSocketExist())
  {
    std::vector<SubscriptionResources::SubscriptionResource> temp = resources.getResources();

    // Generate content for a subscription HTTP post request.
    subscription_content_.str(std::string());
    for (int i = 0; i < temp.size(); ++i)
    {
      subscription_content_ << "resources=" << i
                            << "&"
                            << i << "=" << temp.at(i).resource_uri
                            << "&"
                            << i << "-p=" << temp.at(i).priority
                            << (i < temp.size() - 1 ? "&" : "");
    }

    // Make a subscription request.
    evaluation_conditions_.reset();
    evaluation_conditions_.parse_message_into_xml = false;
    evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_CREATED);
    POCOClient::POCOResult poco_result = httpPost(Services::SUBSCRIPTION, subscription_content_.str());
    result = evaluatePOCOResult(poco_result, evaluation_conditions_);

    if (result.success)
    {
      std::string poll = "/poll/";
      subscription_group_id_ = findSubstringContent(poco_result.poco_info.http.response.header_info, poll, "\n");
      poll += subscription_group_id_;

      // Create a WebSocket for receiving subscription events.
      evaluation_conditions_.reset();
      evaluation_conditions_.parse_message_into_xml = false;
      evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_SWITCHING_PROTOCOLS);
      result = evaluatePOCOResult(webSocketConnect(poll, "robapi2_subscription"), evaluation_conditions_);

      if(!result.success)
      {
        subscription_group_id_.clear();
      }
    }
  }

  return result;
}

RWSClient::RWSResult RWSClient::waitForSubscriptionEvent()
{
  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(webSocketRecieveFrame(), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::endSubscription()
{
  RWSResult result;

  if (webSocketExist())
  {
    if (!subscription_group_id_.empty())
    {
      uri_ = Services::SUBSCRIPTION + "/" + subscription_group_id_;

      evaluation_conditions_.reset();
      evaluation_conditions_.parse_message_into_xml = false;
      evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

      result = evaluatePOCOResult(httpDelete(uri_), evaluation_conditions_);
    }
  }

  return result;
}

RWSClient::RWSResult RWSClient::logout()
{
  uri_ = Resources::LOGOUT;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = true;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

  return evaluatePOCOResult(httpGet(uri_), evaluation_conditions_);
}

RWSClient::RWSResult RWSClient::registerLocalUser(std::string username,
                                                  std::string application,
                                                  std::string location)
{
  uri_ = Services::USERS;
  content_ = "username=" + username +
             "&application=" + application +
             "&location=" + location +
             "&ulocale=" + SystemConstants::General::LOCAL;
  
  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_CREATED);

  RWSResult result = evaluatePOCOResult(httpPost(uri_, content_), evaluation_conditions_);
  
  return result;
}

RWSClient::RWSResult RWSClient::registerRemoteUser(std::string username,
                                                   std::string application,
                                                   std::string location)
{
  uri_ = Services::USERS;
  content_ = "username=" + username +
             "&application=" + application +
             "&location=" + location +
             "&ulocale=" + SystemConstants::General::REMOTE;

  evaluation_conditions_.reset();
  evaluation_conditions_.parse_message_into_xml = false;
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);
  evaluation_conditions_.accepted_outcomes.push_back(HTTPResponse::HTTP_CREATED);

  RWSResult result = evaluatePOCOResult(httpPost(uri_, content_), evaluation_conditions_);

  return result;
}

/************************************************************
 * Auxiliary methods
 */

RWSClient::RWSResult RWSClient::evaluatePOCOResult(const POCOResult& poco_result,
                                                   const EvaluationConditions& conditions)
{
  RWSResult result;

  checkAcceptedOutcomes(&result, poco_result, conditions);

  if (result.success && conditions.parse_message_into_xml)
  {
    parseMessage(&result, poco_result);
  }

  if (log_.size() >= LOG_SIZE)
  {
    log_.pop_back();
  }
  log_.push_front(poco_result);

  return result;
}

void RWSClient::checkAcceptedOutcomes(RWSResult* result,
                                      const POCOResult& poco_result,
                                      const EvaluationConditions& conditions)
{
  if (result)
  {
    if (poco_result.status == POCOResult::OK && poco_result.exception_message.empty())
    {
      for (size_t i = 0; i < conditions.accepted_outcomes.size() && !result->success; ++i)
      {
        result->success = (poco_result.poco_info.http.response.status == conditions.accepted_outcomes.at(i));
      }
    }
  }
}

void RWSClient::parseMessage(RWSResult* result, const POCOResult& poco_result)
{
  if (result)
  {
    std::stringstream ss;

    if (!poco_result.poco_info.http.response.content.empty())
    {
      ss << poco_result.poco_info.http.response.content;
    }
    else if (!poco_result.poco_info.websocket.frame_content.empty())
    {
      ss << poco_result.poco_info.websocket.frame_content;
    }
    else
    {
      // XML parsing: Missing message
      result->success = false;
    }

    if (result->success)
    {
      try
      {
        Poco::XML::InputSource input_source(ss);
        result->p_xml_document = xml_parser_.parse(&input_source);
      }
      catch (...)
      {
        // XML parsing: Failed
        result->success = false;
      }
    }
  }
}

std::string RWSClient::getLogText(const bool verbose)
{
  std::stringstream ss;

  if (log_.size() == 0)
  {
    ss << "Log is empty.";
  }

  for (size_t i = 0; i < log_.size(); ++i)
  {
    std::stringstream temp;
    temp << i + 1 << ". ";
    ss << temp.str() << log_[i].toString(verbose, temp.str().size()) << std::endl;
  }

  return ss.str();
}

std::string RWSClient::generateIOSignalPath(const std::string& iosignal)
{
  return Resources::RW_IOSYSTEM_SIGNALS + "/" + iosignal;
}

std::string RWSClient::generateMechanicalUnitPath(const std::string& mechunit)
{
  return Resources::RW_MOTIONSYSTEM_MECHUNITS + "/" + mechunit;
}

std::string RWSClient::generateRAPIDDataPath(const RAPIDResource& resource)
{
  return Resources::RW_RAPID_SYMBOL_DATA_RAPID + "/" + resource.task + "/" + resource.module + "/" + resource.name;
}

std::string RWSClient::generateRAPIDPropertiesPath(const RAPIDResource& resource)
{
  return Resources::RW_RAPID_SYMBOL_PROPERTIES_RAPID + "/" + resource.task + "/" + resource.module + "/"+ resource.name;
}

std::string RWSClient::generateFilePath(const FileResource& resource)
{
  return Services::FILESERVICE + "/" + resource.directory + "/" + resource.filename;
}

} // end namespace rws
} // end namespace abb