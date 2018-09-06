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

#include <algorithm>
#include <sstream>

#include "abb_librws/rws_interface.h"
#include "abb_librws/rws_rapid.h"

namespace abb
{
namespace rws
{

typedef SystemConstants::ContollerStates ContollerStates;
typedef SystemConstants::RAPID RAPID;
typedef SystemConstants::RWS::XMLAttributes XMLAttributes;

/***********************************************************************************************************************
 * Class definitions: RWSInterface
 */

 /************************************************************
 * Primary methods
 */

RWSInterface::RuntimeInfo RWSInterface::collectRuntimeInfo()
{
  RuntimeInfo runtime_info;

  runtime_info.auto_mode     = isAutoMode();
  runtime_info.motor_on      = isMotorOn();
  runtime_info.rapid_running = isRAPIDRunning();
  runtime_info.rws_connected = (runtime_info.auto_mode != TriBool::UNKNOWN_VALUE &&
                                runtime_info.motor_on != TriBool::UNKNOWN_VALUE &&
                                runtime_info.rapid_running != TriBool::UNKNOWN_VALUE);

  return runtime_info;
}

RWSInterface::StaticInfo RWSInterface::collectStaticInfo()
{
  StaticInfo static_info;

  static_info.rapid_tasks = getRAPIDTasks();
  static_info.system_info = getSystemInfo();

  return static_info;
}

std::string RWSInterface::getIOSignal(const std::string iosignal)
{
  std::string result;

  RWSClient::RWSResult rws_result = rws_client_.getIOSignal(iosignal);

  if (rws_result.success)
  {
    result = xmlFindTextContent(rws_result.p_xml_document, XMLAttributes::CLASS_LVALUE);
  }

  return result;
}

bool RWSInterface::getMechanicalUnitJointTarget(const std::string mechunit, JointTarget* p_jointtarget)
{
  bool result = false;

  if (p_jointtarget)
  {
    RWSClient::RWSResult rws_result = rws_client_.getMechanicalUnitJointTarget(mechunit);
    result = rws_result.success;

    if (result)
    {
      std::stringstream ss;
      
      ss << "[["
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "rax_1")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "rax_2")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "rax_3")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "rax_4")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "rax_5")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "rax_6")) << "], ["
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_a")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_b")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_c")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_d")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_e")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_f")) << "]]";

      p_jointtarget->parseRWSValueString(ss.str());
    }
  }

  return result;
}

bool RWSInterface::getMechanicalUnitRobTarget(const std::string mechunit, RobTarget* p_robtarget)
{
  bool result = false;

  if (p_robtarget)
  {
    RWSClient::RWSResult rws_result = rws_client_.getMechanicalUnitRobTarget(mechunit);
    result = rws_result.success;

    if (result)
    {
      std::stringstream ss;
      
      ss << "[["
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "x")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "y")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "z")) << "], ["
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "q1")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "q2")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "q3")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "q4")) << "], ["
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "cf1")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "cf4")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "cf6")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "cfx")) << "], ["
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_a")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_b")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_c")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_d")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_e")) << ","
         << xmlFindTextContent(rws_result.p_xml_document, XMLAttribute("class", "eax_f")) << "]]";

      p_robtarget->parseRWSValueString(ss.str());
    }
  }

  return result;
}

bool RWSInterface::setRAPIDSymbolData(const std::string task,
                                      const std::string module,
                                      const std::string name,
                                      RAPIDSymbolDataAbstract& data)
{
  return rws_client_.setRAPIDSymbolData(RWSClient::RAPIDResource(task, module, name), data).success;
}

bool RWSInterface::setRAPIDSymbolData(const std::string task,
                                      const RWSClient::RAPIDSymbolResource symbol,
                                      RAPIDSymbolDataAbstract& data)
{
  return rws_client_.setRAPIDSymbolData(RWSClient::RAPIDResource(task, symbol), data).success;
}

bool RWSInterface::startRAPIDExecution()
{
  return rws_client_.startRAPIDExecution().success;
}
  
bool RWSInterface::stopRAPIDExecution()
{
  return rws_client_.stopRAPIDExecution().success;
}
  
bool RWSInterface::resetRAPIDProgramPointer()
{
  return rws_client_.resetRAPIDProgramPointer().success;
}

bool RWSInterface::setMotorsOn()
{
  return rws_client_.setMotorsOn().success;
}

bool RWSInterface::setMotorsOff()
{
  return rws_client_.setMotorsOff().success;
}

std::vector<RWSInterface::RAPIDTask> RWSInterface::getRAPIDTasks()
{
  std::vector<RAPIDTask> result;

  RWSClient::RWSResult rws_result = rws_client_.getRAPIDTasks();
  std::vector<Poco::XML::Node*> node_list = xmlFindNodes(rws_result.p_xml_document, XMLAttributes::CLASS_RAP_TASK_LI);

  for (size_t i = 0; i < node_list.size(); ++i)
  {
    std::string name = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_NAME);
    bool is_motion_task = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_MOTIONTASK) == RAPID::RAPID_TRUE;

    result.push_back(RAPIDTask(name, is_motion_task));
  }

  return result;
}

RWSInterface::SystemInfo RWSInterface::getSystemInfo()
{
  SystemInfo result;

  RWSClient::RWSResult rws_result = rws_client_.getRobotWareSystem();
  std::vector<Poco::XML::Node*> node_list = xmlFindNodes(rws_result.p_xml_document, XMLAttributes::CLASS_SYS_SYSTEM_LI);

  for (size_t i = 0; i < node_list.size(); ++i)
  {
    result.system_name = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_NAME);
    result.robot_ware_version = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_RW_VERSION_NAME);
  }

  return result;
}

TriBool RWSInterface::isAutoMode()
{
  return compareSingleContent(rws_client_.getPanelOperationMode(),
                              XMLAttributes::CLASS_OPMODE,
                              ContollerStates::PANEL_OPERATION_MODE_AUTO);
}

TriBool RWSInterface::isMotorOn()
{
  return compareSingleContent(rws_client_.getPanelControllerState(),
                              XMLAttributes::CLASS_CTRLSTATE,
                              ContollerStates::CONTROLLER_MOTOR_ON);
}

TriBool RWSInterface::isRAPIDRunning()
{
  return compareSingleContent(rws_client_.getRAPIDExecution(),
                              XMLAttributes::CLASS_CTRLEXECSTATE,
                              ContollerStates::RAPID_EXECUTION_RUNNING);
}

bool RWSInterface::setIOSignal(const std::string iosignal, const std::string value)
{
  return rws_client_.setIOSignal(iosignal, value).success;
}

bool RWSInterface::getRAPIDSymbolData(const std::string task,
                                      const std::string module,
                                      const std::string name,
                                      RAPIDSymbolDataAbstract* p_data)
{
  return rws_client_.getRAPIDSymbolData(RWSClient::RAPIDResource(task, module, name), p_data).success;
}

bool RWSInterface::getRAPIDSymbolData(const std::string task,
                                      const RWSClient::RAPIDSymbolResource symbol,
                                      RAPIDSymbolDataAbstract* p_data)
{
  return rws_client_.getRAPIDSymbolData(RWSClient::RAPIDResource(task, symbol), p_data).success;
}

bool RWSInterface::getFile(const RWSClient::FileResource resource, std::string* p_file_content)
{
  return rws_client_.getFile(resource, p_file_content).success;
}

bool RWSInterface::uploadFile(const RWSClient::FileResource resource, const std::string file_content)
{
  return rws_client_.uploadFile(resource, file_content).success;
}

bool RWSInterface::deleteFile(const RWSClient::FileResource resource)
{
  return rws_client_.deleteFile(resource).success;
}

bool RWSInterface::startSubscription (RWSClient::SubscriptionResources resources)
{
  return rws_client_.startSubscription(resources).success;
}

bool RWSInterface::waitForSubscriptionEvent()
{
  RWSClient::RWSResult rws_result = rws_client_.waitForSubscriptionEvent();

  return (rws_result.success && !rws_result.p_xml_document.isNull());
}

bool RWSInterface::waitForSubscriptionEvent(Poco::AutoPtr<Poco::XML::Document>* p_xml_document)
{
  bool result = false;

  if (p_xml_document)
  {
    RWSClient::RWSResult rws_result = rws_client_.waitForSubscriptionEvent();

    if (rws_result.success && !rws_result.p_xml_document.isNull())
    {
      *p_xml_document = rws_result.p_xml_document;
      result = true;
    }
  }

  return result;
}

bool RWSInterface::endSubscription()
{
  return rws_client_.endSubscription().success;
}

bool RWSInterface::registerLocalUser(std::string username,
                                     std::string application,
                                     std::string location)
{
  return rws_client_.registerLocalUser(username, application, location).success;
}

bool RWSInterface::registerRemoteUser(std::string username,
                                      std::string application,
                                      std::string location)
{
  return rws_client_.registerRemoteUser(username, application, location).success;
}

std::string RWSInterface::getLogText(const bool verbose)
{
  return rws_client_.getLogText(verbose);
}

/************************************************************
 * Auxiliary methods
 */

TriBool RWSInterface::compareSingleContent(const RWSClient::RWSResult& rws_result,
                                           const XMLAttribute& attribute,
                                           const std::string& compare_string)
{
  TriBool result;

  if (rws_result.success)
  {
    std::string temp = xmlFindTextContent(rws_result.p_xml_document, attribute);
    result = (temp == compare_string);
  }

  return result;
}

} // end namespace rws
} // end namespace abb