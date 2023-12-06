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
#include <abb_librws/v1_0/rws_interface.h>
#include <abb_librws/v1_0/rw/rapid.h>
#include <abb_librws/v1_0/rw/panel.h>
#include <abb_librws/v1_0/rw/io.h>
#include <abb_librws/rws_rapid.h>
#include <abb_librws/parsing.h>
#include <abb_librws/rws.h>

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <stdexcept>


namespace
{
static const char EXCEPTION_GET_CFG[]{"Failed to get configuration instances"};
static const char EXCEPTION_PARSE_CFG[]{"Failed to parse configuration instances"};
}

namespace abb :: rws :: v1_0
{

typedef SystemConstants::RAPID RAPID;


/***********************************************************************************************************************
 * Class definitions: RWSInterface
 */

 /************************************************************
 * Primary methods
 */

RWSInterface::RWSInterface(RWSClient& client)
: rws_client_ {client}
{
}

StaticInfo RWSInterface::collectStaticInfo()
{
  StaticInfo static_info;

  static_info.rapid_tasks = getRAPIDTasks();
  static_info.system_info = getSystemInfo();

  return static_info;
}

std::vector<cfg::moc::Arm> RWSInterface::getCFGArms()
{
  std::vector<cfg::moc::Arm> result;

  RWSResult rws_result = rws_client_.getConfigurationInstances(Identifiers::MOC, Identifiers::ARM);

  std::vector<Poco::XML::Node*> instances;
  instances = xmlFindNodes(rws_result, XMLAttributes::CLASS_CFG_DT_INSTANCE_LI);

  for(size_t i = 0; i < instances.size(); ++i)
  {
    std::vector<Poco::XML::Node*> attributes = xmlFindNodes(instances[i], XMLAttributes::CLASS_CFG_IA_T_LI);

    cfg::moc::Arm arm;

    for(size_t j = 0; j < attributes.size(); ++j)
    {
      Poco::XML::Node* attribute = attributes[j];
      if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, Identifiers::NAME))
      {
        arm.name = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(arm.name.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "lower_joint_bound"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> arm.lower_joint_bound;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "upper_joint_bound"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> arm.upper_joint_bound;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
    }

    result.push_back(arm);
  }

  return result;
}

std::vector<cfg::moc::Joint> RWSInterface::getCFGJoints()
{
  std::vector<cfg::moc::Joint> result;

  RWSResult rws_result = rws_client_.getConfigurationInstances("MOC", "JOINT");

  std::vector<Poco::XML::Node*> instances;
  instances = xmlFindNodes(rws_result, XMLAttributes::CLASS_CFG_DT_INSTANCE_LI);

  for(size_t i = 0; i < instances.size(); ++i)
  {
    std::vector<Poco::XML::Node*> attributes = xmlFindNodes(instances[i], XMLAttributes::CLASS_CFG_IA_T_LI);

    cfg::moc::Joint joint;

    for(size_t j = 0; j < attributes.size(); ++j)
    {
      Poco::XML::Node* attribute = attributes[j];
      if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, Identifiers::NAME))
      {
        joint.name = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(joint.name.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "logical_axis"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> joint.logical_axis;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "kinematic_axis_number"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> joint.kinematic_axis_number;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_arm"))
      {
        joint.use_arm = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(joint.use_arm.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_transmission"))
      {
        joint.use_transmission = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(joint.use_transmission.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
    }

    result.push_back(joint);
  }

  return result;
}

std::vector<cfg::moc::MechanicalUnit> RWSInterface::getCFGMechanicalUnits()
{
  std::vector<cfg::moc::MechanicalUnit> result;

  RWSResult rws_result = rws_client_.getConfigurationInstances(Identifiers::MOC, Identifiers::MECHANICAL_UNIT);

  std::vector<Poco::XML::Node*> instances;
  instances = xmlFindNodes(rws_result, XMLAttributes::CLASS_CFG_DT_INSTANCE_LI);

  for(size_t i = 0; i < instances.size(); ++i)
  {
    std::vector<Poco::XML::Node*> attributes = xmlFindNodes(instances[i], XMLAttributes::CLASS_CFG_IA_T_LI);

    cfg::moc::MechanicalUnit mechanical_unit;

    for(size_t j = 0; j < attributes.size(); ++j)
    {
      Poco::XML::Node* attribute = attributes[j];
      if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, Identifiers::NAME))
      {
        mechanical_unit.name = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(mechanical_unit.name.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_robot"))
      {
        // Note: The 'use_robot' attribute can be empty, since not all units have a robot (i.e. skip validation).
        mechanical_unit.use_robot = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_single_0") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_single_1") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_single_2") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_single_3") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_single_4") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_single_5"))
      {
        // Note: The 'use_single_N' attribute can be empty, since not all units have singles (i.e. skip validation).
        mechanical_unit.use_singles.push_back(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
      }
    }

    result.push_back(mechanical_unit);
  }

  return result;
}

std::vector<cfg::sys::MechanicalUnitGroup> RWSInterface::getCFGMechanicalUnitGroups()
{
  std::vector<cfg::sys::MechanicalUnitGroup> result;

  RWSResult rws_result = rws_client_.getConfigurationInstances(Identifiers::SYS, Identifiers::MECHANICAL_UNIT_GROUP);

  std::vector<Poco::XML::Node*> instances;
  instances = xmlFindNodes(rws_result, XMLAttributes::CLASS_CFG_DT_INSTANCE_LI);

  for(size_t i = 0; i < instances.size(); ++i)
  {
    std::vector<Poco::XML::Node*> attributes = xmlFindNodes(instances[i], XMLAttributes::CLASS_CFG_IA_T_LI);

    cfg::sys::MechanicalUnitGroup mechanical_unit_group;

    for(size_t j = 0; j < attributes.size(); ++j)
    {
      Poco::XML::Node* attribute = attributes[j];
      if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "Name"))
      {
        mechanical_unit_group.name = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(mechanical_unit_group.name.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "Robot"))
      {
        // Note: The 'Robot' attribute can be empty, since not all groups have a robot (i.e. skip validation).
        mechanical_unit_group.robot = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "MechanicalUnit_1") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "MechanicalUnit_2") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "MechanicalUnit_3") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "MechanicalUnit_4") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "MechanicalUnit_5") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "MechanicalUnit_6"))
      {
        // Note: The 'MechanicalUnit_N' attribute can be empty, since not all groups have units (i.e. skip validation).
        mechanical_unit_group.mechanical_units.push_back(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
      }
    }

    result.push_back(mechanical_unit_group);
  }

  return result;
}

std::vector<cfg::sys::PresentOption> RWSInterface::getCFGPresentOptions()
{
  std::vector<cfg::sys::PresentOption> result;

  RWSResult rws_result = rws_client_.getConfigurationInstances(Identifiers::SYS, Identifiers::PRESENT_OPTIONS);

  std::vector<Poco::XML::Node*> instances;
  instances = xmlFindNodes(rws_result, XMLAttributes::CLASS_CFG_DT_INSTANCE_LI);

  for(size_t i = 0; i < instances.size(); ++i)
  {
    std::vector<Poco::XML::Node*> attributes = xmlFindNodes(instances[i], XMLAttributes::CLASS_CFG_IA_T_LI);

    cfg::sys::PresentOption present_option;

    for(size_t j = 0; j < attributes.size(); ++j)
    {
      Poco::XML::Node* attribute = attributes[j];
      if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "name"))
      {
        present_option.name = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(present_option.name.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "desc"))
      {
        present_option.description = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(present_option.description.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
    }

    result.push_back(present_option);
  }

  return result;
}

std::vector<cfg::moc::Robot> RWSInterface::getCFGRobots()
{
  std::vector<cfg::moc::Robot> result;

  RWSResult rws_result = rws_client_.getConfigurationInstances(Identifiers::MOC, Identifiers::ROBOT);

  std::vector<Poco::XML::Node*> instances;
  instances = xmlFindNodes(rws_result, XMLAttributes::CLASS_CFG_DT_INSTANCE_LI);

  for(size_t i = 0; i < instances.size(); ++i)
  {
    std::vector<Poco::XML::Node*> attributes = xmlFindNodes(instances[i], XMLAttributes::CLASS_CFG_IA_T_LI);

    cfg::moc::Robot robot;

    for(size_t j = 0; j < attributes.size(); ++j)
    {
      Poco::XML::Node* attribute = attributes[j];
      if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, Identifiers::NAME))
      {
        robot.name = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(robot.name.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_robot_type"))
      {
        robot.use_robot_type = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(robot.use_robot_type.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_joint_0") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_joint_1") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_joint_2") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_joint_3") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_joint_4") ||
              xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_joint_5"))
      {
        // Note: The 'use_joint_N' attribute can be empty, since not all robots have 6 joints (i.e. skip validation).
        robot.use_joints.push_back(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_pos_x"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> robot.base_frame.pos.x.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
        robot.base_frame.pos.x.value *= 1e3;
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_pos_y"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> robot.base_frame.pos.y.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
        robot.base_frame.pos.y.value *= 1e3;
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_pos_z"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> robot.base_frame.pos.z.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
        robot.base_frame.pos.z.value *= 1e3;
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_orient_u0"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> robot.base_frame.rot.q1.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_orient_u1"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> robot.base_frame.rot.q2.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_orient_u2"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> robot.base_frame.rot.q3.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_orient_u3"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> robot.base_frame.rot.q4.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_coordinated"))
      {
        // Note: The 'base_frame_coordinated' attribute can be empty (i.e. skip validation).
        //       It is only used if this robot's base frame is moved by another robot or single.
        robot.base_frame_moved_by = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
      }
    }

     result.push_back(robot);
  }

  return result;
}

std::vector<cfg::moc::Single> RWSInterface::getCFGSingles()
{
  std::vector<cfg::moc::Single> result;

  RWSResult rws_result = rws_client_.getConfigurationInstances(Identifiers::MOC, Identifiers::SINGLE);

  std::vector<Poco::XML::Node*> instances;
  instances = xmlFindNodes(rws_result, XMLAttributes::CLASS_CFG_DT_INSTANCE_LI);

  for(size_t i = 0; i < instances.size(); ++i)
  {
    std::vector<Poco::XML::Node*> attributes = xmlFindNodes(instances[i], XMLAttributes::CLASS_CFG_IA_T_LI);

    cfg::moc::Single single;

    for(size_t j = 0; j < attributes.size(); ++j)
    {
      Poco::XML::Node* attribute = attributes[j];
      if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, Identifiers::NAME))
      {
        single.name = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(single.name.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_single_type"))
      {
        single.use_single_type = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(single.use_single_type.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "use_joint"))
      {
        single.use_joint = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(single.use_joint.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_pos_x"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> single.base_frame.pos.x.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
        single.base_frame.pos.x.value *= 1e3;
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_pos_y"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> single.base_frame.pos.y.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
        single.base_frame.pos.y.value *= 1e3;
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_pos_z"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> single.base_frame.pos.z.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
        single.base_frame.pos.z.value *= 1e3;
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_orient_u0"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> single.base_frame.rot.q1.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_orient_u1"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> single.base_frame.rot.q2.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_orient_u2"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> single.base_frame.rot.q3.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_orient_u3"))
      {
        std::stringstream ss(xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE));
        ss >> single.base_frame.rot.q4.value;
        if(ss.fail()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
      }
      else if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "base_frame_coordinated"))
      {
        // Note: The 'base_frame_coordinated' attribute can be empty (i.e. skip validation).
        //       It is only used if this single's base frame is moved by another robot or single.
        single.base_frame_coordinated = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
      }
    }

    result.push_back(single);
  }

  return result;
}

std::vector<cfg::moc::Transmission> RWSInterface::getCFGTransmission()
{
  std::vector<cfg::moc::Transmission> result;

  RWSResult rws_result = rws_client_.getConfigurationInstances("MOC", "TRANSMISSION");

  std::vector<Poco::XML::Node*> instances;
  instances = xmlFindNodes(rws_result, XMLAttributes::CLASS_CFG_DT_INSTANCE_LI);

  for(size_t i = 0; i < instances.size(); ++i)
  {
    std::vector<Poco::XML::Node*> attributes = xmlFindNodes(instances[i], XMLAttributes::CLASS_CFG_IA_T_LI);

    cfg::moc::Transmission transmission;

    transmission.name = xmlNodeGetAttributeValue(instances[i], Identifiers::TITLE);
    if(transmission.name.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);

    for(size_t j = 0; j < attributes.size(); ++j)
    {
      Poco::XML::Node* attribute = attributes[j];
      if(xmlNodeHasAttribute(attribute, Identifiers::TITLE, "rotating_move"))
      {
        std::string temp = xmlFindTextContent(attribute, XMLAttributes::CLASS_VALUE);
        if(temp.empty()) throw std::runtime_error(EXCEPTION_PARSE_CFG);
        transmission.rotating_move = (temp == "true");
      }
    }

    result.push_back(transmission);
  }

  return result;
}

std::vector<RobotWareOptionInfo> RWSInterface::getPresentRobotWareOptions()
{
  std::vector<RobotWareOptionInfo> result;

  RWSResult rws_result = rws_client_.getConfigurationInstances(Identifiers::SYS,
                                                                          Identifiers::PRESENT_OPTIONS);

  std::vector<Poco::XML::Node*> node_list = xmlFindNodes(rws_result, XMLAttributes::CLASS_CFG_IA_T_LI);

  for (size_t i = 0; i < node_list.size(); i+=2)
  {
    if (i + 1 < node_list.size())
    {
      result.push_back(RobotWareOptionInfo(xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_VALUE),
                                           xmlFindTextContent(node_list.at(i+1), XMLAttributes::CLASS_VALUE)));
    }
  }

  return result;
}


std::string RWSInterface::getIOSignal(const std::string& iosignal)
{
  return rw::io::getIOSignal(rws_client_, iosignal);
}


MechanicalUnitStaticInfo RWSInterface::getMechanicalUnitStaticInfo(const std::string& mechunit)
{
  RWSResult rws_result = rws_client_.getMechanicalUnitStaticInfo(mechunit);

  MechanicalUnitStaticInfo static_info;
  static_info.task_name = xmlFindTextContent(rws_result, XMLAttribute("class", "task-name"));
  static_info.is_integrated_unit = xmlFindTextContent(rws_result,
                                                      XMLAttribute("class", "is-integrated-unit"));
  static_info.has_integrated_unit = xmlFindTextContent(rws_result,
                                                        XMLAttribute("class", "has-integrated-unit"));
  std::string type = xmlFindTextContent(rws_result, XMLAttribute("class", "type"));

  // Assume mechanical unit type is undefined, update based on contents of 'type'.
  static_info.type = MechanicalUnitType::UNDEFINED;

  if(type == "None")
  {
    static_info.type = MechanicalUnitType::NONE;
  }
  else if(type == "TCPRobot")
  {
    static_info.type = MechanicalUnitType::TCP_ROBOT;
  }
  else if(type == "Robot")
  {
    static_info.type = MechanicalUnitType::ROBOT;
  }
  else if(type == "Single")
  {
    static_info.type = MechanicalUnitType::SINGLE;
  }

  std::stringstream axes(xmlFindTextContent(rws_result, XMLAttribute("class", "axes")));
  axes >> static_info.axes;

  std::stringstream axes_total(xmlFindTextContent(rws_result, XMLAttribute("class", "axes-total")));
  axes_total >> static_info.axes_total;

  // Basic verification.
  if(static_info.task_name.empty() ||
      static_info.is_integrated_unit.empty() ||
      static_info.has_integrated_unit.empty() ||
      static_info.type == MechanicalUnitType::UNDEFINED ||
      axes.fail() || axes_total.fail())
  {
    throw std::logic_error("RWSInterface::getMechanicalUnitStaticInfo(): inconsistent data");
  }

  return static_info;
}

MechanicalUnitDynamicInfo RWSInterface::getMechanicalUnitDynamicInfo(const std::string& mechunit)
{
  bool result = false;

  RWSResult rws_result = rws_client_.getMechanicalUnitDynamicInfo(mechunit);

  MechanicalUnitDynamicInfo dynamic_info;
  dynamic_info.tool_name = xmlFindTextContent(rws_result, XMLAttribute("class", "tool-name"));
  dynamic_info.wobj_name = xmlFindTextContent(rws_result, XMLAttribute("class", "wobj-name"));
  dynamic_info.payload_name = xmlFindTextContent(rws_result,
                                                  XMLAttribute("class", "payload-name"));
  dynamic_info.total_payload_name = xmlFindTextContent(rws_result,
                                                        XMLAttribute("class", "total-payload-name"));
  dynamic_info.status = xmlFindTextContent(rws_result, XMLAttribute("class", "status"));
  dynamic_info.jog_mode = xmlFindTextContent(rws_result, XMLAttribute("class", "jog-mode"));
  std::string mode = xmlFindTextContent(rws_result, XMLAttribute("class", "mode"));
  std::string coord_system = xmlFindTextContent(rws_result, XMLAttribute("class", "coord-system"));

  // Assume mechanical unit mode is unknown, update based on contents of 'mode'.
  dynamic_info.mode = MechanicalUnitMode::UNKNOWN_MODE;

  if(mode == "Activated")
  {
    dynamic_info.mode = MechanicalUnitMode::ACTIVATED;
  }
  else if(mode == "Deactivated")
  {
    dynamic_info.mode = MechanicalUnitMode::DEACTIVATED;
  }

  // Assume mechanical unit coordinate system is world, update based on contents of 'coord_system'.
  dynamic_info.coord_system = Coordinate::WORLD;

  if(coord_system == "Base")
  {
    dynamic_info.coord_system = Coordinate::BASE;
  }
  else if(coord_system == "Tool")
  {
    dynamic_info.coord_system = Coordinate::TOOL;
  }
  else if(coord_system == "Wobj")
  {
    dynamic_info.coord_system = Coordinate::WOBJ;
  }

  // Basic verification.
  if(dynamic_info.tool_name.empty() ||
      dynamic_info.wobj_name.empty() ||
      dynamic_info.payload_name.empty() ||
      dynamic_info.total_payload_name.empty() ||
      dynamic_info.status.empty() ||
      dynamic_info.jog_mode.empty() ||
      dynamic_info.mode == MechanicalUnitMode::UNKNOWN_MODE)
  {
    throw std::logic_error("RWSInterface::getMechanicalUnitDynamicInfo: inconsistent data");
  }

  return dynamic_info;
}

JointTarget RWSInterface::getMechanicalUnitJointTarget(const std::string& mechunit)
{
  RWSResult rws_result = rws_client_.getMechanicalUnitJointTarget(mechunit);
  std::stringstream ss;

  ss << "[["
      << xmlFindTextContent(rws_result, XMLAttribute("class", "rax_1")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "rax_2")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "rax_3")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "rax_4")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "rax_5")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "rax_6")) << "], ["
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_a")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_b")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_c")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_d")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_e")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_f")) << "]]";

  JointTarget jointtarget;
  jointtarget.parseString(ss.str());

  return jointtarget;
}

RobTarget RWSInterface::getMechanicalUnitRobTarget(const std::string& mechunit,
                                              Coordinate coordinate,
                                              const std::string& tool,
                                              const std::string& wobj)
{
  RWSResult rws_result = rws_client_.getMechanicalUnitRobTarget(mechunit, coordinate, tool, wobj);

  std::stringstream ss;

  ss << "[["
      << xmlFindTextContent(rws_result, XMLAttribute("class", "x")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "y")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "z")) << "], ["
      << xmlFindTextContent(rws_result, XMLAttribute("class", "q1")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "q2")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "q3")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "q4")) << "], ["
      << xmlFindTextContent(rws_result, XMLAttribute("class", "cf1")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "cf4")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "cf6")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "cfx")) << "], ["
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_a")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_b")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_c")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_d")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_e")) << ","
      << xmlFindTextContent(rws_result, XMLAttribute("class", "eax_f")) << "]]";

  RobTarget robtarget;
  robtarget.parseString(ss.str());

  return robtarget;
}

void RWSInterface::setRAPIDSymbolData(const std::string& task,
                                      const std::string& module,
                                      const std::string& name,
                                      const std::string& data)
{
  rw::rapid::setRAPIDSymbolData(rws_client_, RAPIDResource(task, module, name), data);
}


void RWSInterface::setRAPIDSymbolData(RAPIDResource const& resource, const RAPIDSymbolDataAbstract& data)
{
  rw::rapid::setRAPIDSymbolData(rws_client_, resource, data);
}


void RWSInterface::startRAPIDExecution()
{
  rw::rapid::startRAPIDExecution(rws_client_);
}

void RWSInterface::stopRAPIDExecution(StopMode stopmode, UseTsp usetsp)
{
  rw::rapid::stopRAPIDExecution(rws_client_, stopmode, usetsp);
}

void RWSInterface::resetRAPIDProgramPointer()
{
  rw::rapid::resetRAPIDProgramPointer(rws_client_);
}

void RWSInterface::setMotorsOn()
{
  rw::panel::setControllerState(rws_client_, rw::ControllerState::motorOn);
}

void RWSInterface::setMotorsOff()
{
  rw::panel::setControllerState(rws_client_, rw::ControllerState::motorOff);
}

void RWSInterface::setSpeedRatio(unsigned int ratio)
{
  rw::panel::setSpeedRatio(rws_client_, ratio);
}

std::vector<rw::RAPIDModuleInfo> RWSInterface::getRAPIDModulesInfo(const std::string& task)
{
  return rw::rapid::getRAPIDModulesInfo(rws_client_, task);
}

std::vector<rw::RAPIDTaskInfo> RWSInterface::getRAPIDTasks()
{
  return rw::rapid::getRAPIDTasks(rws_client_);
}

unsigned int RWSInterface::getSpeedRatio()
{
  return rw::panel::getSpeedRatio(rws_client_);
}

SystemInfo RWSInterface::getSystemInfo()
{
  SystemInfo result;

  RWSResult rws_result = rws_client_.getRobotWareSystem();

  std::vector<Poco::XML::Node*> node_list = xmlFindNodes(rws_result, XMLAttributes::CLASS_SYS_SYSTEM_LI);
  for (size_t i = 0; i < node_list.size(); ++i)
  {
    result.system_name = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_NAME);
    result.robot_ware_version = xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_RW_VERSION_NAME);
  }

  node_list = xmlFindNodes(rws_result, XMLAttributes::CLASS_SYS_OPTION_LI);
  for (size_t i = 0; i < node_list.size(); ++i)
  {
    result.system_options.push_back(xmlFindTextContent(node_list.at(i), XMLAttributes::CLASS_OPTION));
  }

  result.system_type = xmlFindTextContent(rws_client_.getContollerService(),
                                          XMLAttributes::CLASS_CTRL_TYPE);

  return result;
}

bool RWSInterface::isAutoMode()
{
  return rw::panel::getOperationMode(rws_client_) == rw::OperationMode::automatic;
}

bool RWSInterface::isMotorsOn()
{
  return rw::panel::getControllerState(rws_client_) == rw::ControllerState::motorOn;
}

bool RWSInterface::isRAPIDRunning()
{
  return rw::rapid::getRAPIDExecution(rws_client_).ctrlexecstate == rw::RAPIDExecutionState::running;
}

void RWSInterface::setIOSignal(const std::string& iosignal, const std::string& value)
{
  rw::io::setIOSignal(rws_client_, iosignal, value);
}

std::string RWSInterface::getRAPIDSymbolData(const std::string& task,
                                             const std::string& module,
                                             const std::string& name)
{
  return rw::rapid::getRAPIDSymbolData(rws_client_, RAPIDResource(task, module, name));
}


void RWSInterface::getRAPIDSymbolData(RAPIDResource const& resource, RAPIDSymbolDataAbstract& data)
{
  rw::rapid::getRAPIDSymbolData(rws_client_, resource, data);
}


void RWSInterface::loadModuleIntoTask(const std::string& task, const FileResource& resource, const bool replace)
{
  rw::rapid::loadModuleIntoTask(rws_client_, task, resource, replace);
}

void RWSInterface::unloadModuleFromTask(const std::string& task, const FileResource& resource)
{
  rw::rapid::unloadModuleFromTask(rws_client_, task, resource);
}

void RWSInterface::saveConfigDomain(FileResource const& resource, abb::rws::CFGDomain const& domain)
{
    std::stringstream uri;
    uri << Resources::RW_CFG << "/" << domain << "?action=saveas";

    std::string content =
        "filepath=" + Services::FILESERVICE + "/" + resource.directory + "/" + resource.filename;

    POCOResult result = rws_client_.httpPost(uri.str(), content, {Poco::Net::HTTPResponse::HTTP_NO_CONTENT});
}


std::string RWSInterface::getFile(const FileResource& resource)
{
  return rws_client_.getFile(resource);
}

void RWSInterface::uploadFile(const FileResource& resource, const std::string& file_content)
{
  rws_client_.uploadFile(resource, file_content);
}

void RWSInterface::deleteFile(const FileResource& resource)
{
  rws_client_.deleteFile(resource);
}

std::unique_ptr<subscription::SubscriptionGroup> RWSInterface::openSubscription (const SubscriptionResources& resources)
{
  return std::make_unique<subscription::SubscriptionGroup>(rws_client_, resources);
}

void RWSInterface::registerLocalUser(const std::string& username,
                                     const std::string& application,
                                     const std::string& location)
{
  rws_client_.registerLocalUser(username, application, location);
}

void RWSInterface::registerRemoteUser(const std::string& username,
                                      const std::string& application,
                                      const std::string& location)
{
  rws_client_.registerRemoteUser(username, application, location);
}


void RWSInterface::setDigitalSignal(std::string const& signal_name, bool value)
{
  rw::io::setDigitalSignal(rws_client_, signal_name, value);
}


void RWSInterface::setAnalogSignal(std::string const& signal_name, float value)
{
  rw::io::setAnalogSignal(rws_client_, signal_name, value);
}


void RWSInterface::setGroupSignal(std::string const& signal_name, std::uint32_t value)
{
  rw::io::setGroupSignal(rws_client_, signal_name, value);
}


bool RWSInterface::getDigitalSignal(std::string const& signal_name)
{
  return rw::io::getDigitalSignal(rws_client_, signal_name);
}


float RWSInterface::getAnalogSignal(std::string const& signal_name)
{
  return rw::io::getAnalogSignal(rws_client_, signal_name);
}


std::uint32_t RWSInterface::getGroupSignal(std::string const& signal_name)
{
  return rw::io::getGroupSignal(rws_client_, signal_name);
}


rw::io::IOSignalInfo RWSInterface::getIOSignals()
{
  return rw::io::getIOSignals(rws_client_);
}


void RWSInterface::requestMastership()
{
  rws_client_.httpPost("/rw/mastership?action=request");
}


void RWSInterface::requestMastership(MastershipDomain domain)
{
  std::stringstream uri;
  uri << "/rw/mastership/" << domain << "?action=request";

  rws_client_.httpPost(uri.str());
}


void RWSInterface::releaseMastership()
{
  rws_client_.httpPost("/rw/mastership?action=release");
}


void RWSInterface::releaseMastership(MastershipDomain domain)
{
  std::stringstream uri;
  uri << "/rw/mastership/" << domain << "?action=release";

  rws_client_.httpPost(uri.str());
}


void RWSInterface::activateAllTasks()
{
  rws_client_.httpPost("/rw/rapid/tasks?action=activate");
}


void RWSInterface::deactivateAllTasks()
{
  rws_client_.httpPost("/rw/rapid/tasks?action=deactivate");
}


/************************************************************
 * Auxiliary methods
 */

bool RWSInterface::compareSingleContent(const RWSResult& rws_result,
                                           const XMLAttribute& attribute,
                                           const std::string& compare_string)
{
  return xmlFindTextContent(rws_result, attribute) == compare_string;
}

}
