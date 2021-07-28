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

#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeList.h"

#include "abb_librws/rws_common.h"

namespace abb
{
namespace rws
{
/***********************************************************************************************************************
 * Function definitions
 */

std::vector<Poco::XML::Node*> xmlFindNodes(Poco::XML::Node* p_root, const XMLAttribute& attribute)
{
  std::vector<Poco::XML::Node*> result;

  if(p_root)
  {
    bool found = false;

    Poco::XML::NodeIterator node_iterator(p_root, Poco::XML::NodeFilter::SHOW_ELEMENT);
    Poco::XML::Node* p_node = node_iterator.nextNode();

    while (p_node)
    {
      if (xmlNodeHasAttribute(p_node, attribute))
      {
        result.push_back(p_node);
      }

      p_node = node_iterator.nextNode();
    }
  }

  return result;
}

std::vector<Poco::XML::Node*> xmlFindNodes(Poco::AutoPtr<Poco::XML::Document> p_xml_document,
                                           const XMLAttribute& attribute)
{
  std::vector<Poco::XML::Node*> result;

  if (!p_xml_document.isNull())
  {
    Poco::XML::NodeIterator node_iterator(p_xml_document, Poco::XML::NodeFilter::SHOW_ELEMENT);
    Poco::XML::Node* p_node = node_iterator.nextNode();

    while (p_node)
    {
      if (xmlNodeHasAttribute(p_node, attribute))
      {
        result.push_back(p_node);
      }

      p_node = node_iterator.nextNode();
    }
  }

  return result;
}

std::string xmlFindTextContent(Poco::AutoPtr<Poco::XML::Document> p_xml_document, const XMLAttribute& attribute)
{
  std::string result;

  if (!p_xml_document.isNull())
  {
    Poco::XML::NodeIterator node_iterator(p_xml_document, Poco::XML::NodeFilter::SHOW_ELEMENT);
    result = xmlFindTextContent(node_iterator.nextNode(), attribute);
  }

  return result;
}

std::string xmlFindTextContent(const Poco::XML::Node* p_node, const XMLAttribute& attribute)
{
  std::string result;

  if (p_node)
  {
    if (p_node->nodeType() == Poco::XML::Node::TEXT_NODE && xmlNodeHasAttribute(p_node->parentNode(), attribute))
    {
      result = p_node->nodeValue();
    }
    else
    {
      bool found = false;
      if (p_node->hasChildNodes())
      {
        Poco::AutoPtr<Poco::XML::NodeList> p_children(p_node->childNodes());

        for (unsigned long i = 0; i < p_children->length() && !found; i++)
        {
          Poco::XML::Node* p_child = p_children->item(i);

          if (p_child->nodeType() == Poco::XML::Node::TEXT_NODE &&
              xmlNodeHasAttribute(p_child->parentNode(), attribute))
          {
            found = true;
            result = p_child->nodeValue();
          }
          else
          {
            result = xmlFindTextContent(p_child, attribute);
            found = !result.empty();
          }
        }
      }
    }
  }

  return result;
}

std::string xmlNodeGetAttributeValue(const Poco::XML::Node* p_node, const std::string& name)
{
  std::string result;

  if (p_node && p_node->hasAttributes() && !name.empty())
  {
    Poco::AutoPtr<Poco::XML::NamedNodeMap> p_attributes(p_node->attributes());

    for (unsigned long i = 0; i < p_attributes->length(); ++i)
    {
      Poco::XML::Node* p_attribute = p_attributes->item(i);

      if (p_attribute->nodeName() == name)
      {
        return p_attribute->nodeValue();
      }
    }
  }

  return result;
}

bool xmlNodeHasAttribute(const Poco::XML::Node* p_node, const XMLAttribute& attribute)
{
  bool found = attribute.name.empty() && attribute.value.empty();

  if(!found && p_node && p_node->hasAttributes())
  {
    Poco::AutoPtr<Poco::XML::NamedNodeMap> p_attributes(p_node->attributes());

    for (unsigned long i = 0; i < p_attributes->length() && !found; i++)
    {
      Poco::XML::Node* p_attribute = p_attributes->item(i);

      if (p_attribute->nodeName() == attribute.name && p_attribute->nodeValue() == attribute.value)
      {
        found = true;
      }
    }
  }

  return found;
}

bool xmlNodeHasAttribute(const Poco::XML::Node* p_node, const std::string& name, const std::string& value)
{
  return xmlNodeHasAttribute(p_node, XMLAttribute(name, value));
}


std::string findSubstringContent(const std::string& whole_string,
                                             const std::string& substring_start,
                                             const std::string& substring_end)
{
  std::string result;
  size_t start_postion = whole_string.find(substring_start);

  if (start_postion != std::string::npos)
  {
    start_postion += substring_start.size();
    size_t end_postion = whole_string.find_first_of(substring_end, start_postion);

    if (end_postion != std::string::npos)
    {
      result = whole_string.substr(start_postion, end_postion - start_postion);
    }
  }

  std::string quot = "&quot;";
  size_t quot_position = 0;
  do
  {
    quot_position = result.find(quot);
    if (quot_position != std::string::npos)
    {
      result.replace(quot_position, quot.size(), "");
    }
  } while (quot_position != std::string::npos);

  return result;
}


/***********************************************************************************************************************
 * Struct definitions: SystemConstants
 */

const std::string SystemConstants::ContollerStates::CONTROLLER_MOTOR_ON       = "motoron";
const std::string SystemConstants::ContollerStates::CONTROLLER_MOTOR_OFF      = "motoroff";
const std::string SystemConstants::ContollerStates::PANEL_OPERATION_MODE_AUTO = "AUTO";
const std::string SystemConstants::ContollerStates::RAPID_EXECUTION_RUNNING   = "running";

const std::string SystemConstants::General::EXTERNAL_APPLICATION   = "ExternalApplication";
const std::string SystemConstants::General::EXTERNAL_LOCATION      = "ExternalLocation";
const unsigned short SystemConstants::General::DEFAULT_PORT_NUMBER = 80;
const std::string SystemConstants::General::DEFAULT_PASSWORD       = "robotics";
const std::string SystemConstants::General::DEFAULT_USERNAME       = "Default User";
const std::string SystemConstants::General::LOCAL                  = "local";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_1  = "ROB_1";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_2  = "ROB_2";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_3  = "ROB_3";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_4  = "ROB_4";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_L  = "ROB_L";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_R  = "ROB_R";
const std::string SystemConstants::General::REMOTE                 = "remote";
const std::string SystemConstants::General::COORDINATE_BASE        = "Base";
const std::string SystemConstants::General::COORDINATE_WORLD       = "Word";
const std::string SystemConstants::General::COORDINATE_TOOL        = "Tool";
const std::string SystemConstants::General::COORDINATE_WOBJ        = "Wobj";

const std::string SystemConstants::IOSignals::HAND_ACTUAL_POSITION_L   = "hand_ActualPosition_L";
const std::string SystemConstants::IOSignals::HAND_ACTUAL_POSITION_R   = "hand_ActualPosition_R";
const std::string SystemConstants::IOSignals::HAND_ACTUAL_SPEED_L      = "hand_ActualSpeed_L";
const std::string SystemConstants::IOSignals::HAND_ACTUAL_SPEED_R      = "hand_ActualSpeed_R";
const std::string SystemConstants::IOSignals::HAND_STATUS_CALIBRATED_L = "hand_StatusCalibrated_L";
const std::string SystemConstants::IOSignals::HAND_STATUS_CALIBRATED_R = "hand_StatusCalibrated_R";
const std::string SystemConstants::IOSignals::HIGH                     = "1";
const std::string SystemConstants::IOSignals::LOW                      = "0";

const std::string SystemConstants::RAPID::RAPID_FALSE = "FALSE";
const std::string SystemConstants::RAPID::RAPID_TRUE  = "TRUE";
const std::string SystemConstants::RAPID::TASK_ROB_1  = "T_ROB1";
const std::string SystemConstants::RAPID::TASK_ROB_2  = "T_ROB2";
const std::string SystemConstants::RAPID::TASK_ROB_3  = "T_ROB3";
const std::string SystemConstants::RAPID::TASK_ROB_4  = "T_ROB4";
const std::string SystemConstants::RAPID::TASK_ROB_L  = "T_ROB_L";
const std::string SystemConstants::RAPID::TASK_ROB_R  = "T_ROB_R";
const std::string SystemConstants::RAPID::TYPE_BOOL   = "bool";
const std::string SystemConstants::RAPID::TYPE_DNUM   = "dnum";
const std::string SystemConstants::RAPID::TYPE_NUM    = "num";
const std::string SystemConstants::RAPID::TYPE_STRING = "string";

} // end namespace rws
} // end namespace abb
