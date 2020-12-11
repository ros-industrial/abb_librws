/***********************************************************************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2020, Unibap AB (publ)
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
 *
 * Authors: Batu Akan, Aris Synodinos
 *
 */


#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "abb_librws/rws_interface.h"
#include "abb_librws/rws_rapid.h"

#include <Poco/DOM/DOMWriter.h>

#include <vector>

using namespace abb::rws;


class PyRAPIDRecord : public RAPIDRecord
{
public:
  using RAPIDRecord::RAPIDRecord;

  void addComponents(RAPIDSymbolDataAbstract* data)
  {
    components_.push_back(data);
  }
};


class RWSInterfacePy : public RWSInterface
{
    using RWSInterface::RWSInterface;
    public:
    std::string waitForSubscriptionEventAsString()
    {
        RWSClient::RWSResult rws_result = rws_client_.waitForSubscriptionEvent();
        if (rws_result.success && !rws_result.p_xml_document.isNull())
        {
            std::ostringstream ostr;
            Poco::XML::DOMWriter().writeNode(ostr, rws_result.p_xml_document);
            return ostr.str() ;
        }
        return "";
    }
};

//thin wrappers for functions with default values
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getMechanicalUnitRobTarget_overloads, RWSInterface::getMechanicalUnitRobTarget, 2, 5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(registerLocalUser_overloads, RWSInterface::registerLocalUser, 0, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(registerRemoteUser_overloads, RWSInterface::registerRemoteUser, 0, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getLogText_overloads, RWSInterface::getLogText, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getLogTextLatestEvent_overloads, RWSInterface::getLogTextLatestEvent, 0, 1)

BOOST_PYTHON_MODULE(abb_librws)
{

    using namespace boost::python;

    class_<std::vector<std::string> >("string_vector")
        .def(vector_indexing_suite<std::vector<std::string> >())
    ;

    class_<std::vector<RWSInterface::RAPIDTaskInfo> >("RAPIDTaskInfo_vector")
        .def(vector_indexing_suite<std::vector<RWSInterface::RAPIDTaskInfo> >())
    ;

    class_<std::vector<cfg::moc::Arm> >("Arm_vector")
        .def(vector_indexing_suite<std::vector<cfg::moc::Arm> >())
    ;

    class_<std::vector<cfg::moc::Joint> >("Joint_vector")
        .def(vector_indexing_suite<std::vector<cfg::moc::Joint> >())
    ;

    class_<std::vector<cfg::moc::MechanicalUnit> >("MechanicalUnit_vector")
        .def(vector_indexing_suite<std::vector<cfg::moc::MechanicalUnit> >())
    ;

    class_<std::vector<cfg::sys::MechanicalUnitGroup> >("MechanicalUnitGroup_vector")
        .def(vector_indexing_suite<std::vector<cfg::sys::MechanicalUnitGroup> >())
    ;

    class_<std::vector<cfg::sys::PresentOption> >("PresentOption_vector")
        .def(vector_indexing_suite<std::vector<cfg::sys::PresentOption> >())
    ;

    class_<std::vector<cfg::moc::Robot> >("Robot_vector")
        .def(vector_indexing_suite<std::vector<cfg::moc::Robot> >())
    ;

    class_<std::vector<cfg::moc::Single> >("Single_vector")
        .def(vector_indexing_suite<std::vector<cfg::moc::Single> >())
    ;

    class_<std::vector<cfg::moc::Transmission> >("Transmission_vector")
        .def(vector_indexing_suite<std::vector<cfg::moc::Transmission> >())
    ;

    class_<std::vector<RWSInterface::RAPIDModuleInfo> >("RAPIDModuleInfo_vector")
        .def(vector_indexing_suite<std::vector<RWSInterface::RAPIDModuleInfo> >())
    ;


    enum_<TriBool::Values>("Motion")
        .value("UNKNOWN_VALUE", TriBool::UNKNOWN_VALUE)
        .value("TRUE_VALUE", TriBool::TRUE_VALUE)
        .value("FALSE_VALUE", TriBool::FALSE_VALUE)
    ;

    class_<TriBool>("TriBool")
        .def(init<TriBool::Values>())
        .def(init<bool>())
        .def("isUnknown", &TriBool::isUnknown)
        .def("isTrue", &TriBool::isTrue)
        .def("isFalse", &TriBool::isFalse)
        .def("toString", &TriBool::toString)
        .def("__str__", &TriBool::toString)
        //TODO: Operator overrides
    ;

    /*rws_config*/
    class_<cfg::moc::Arm, boost::noncopyable>("Arm", no_init)
        .def_readwrite("name", &cfg::moc::Arm::name)
        .def_readwrite("lower_joint_bound", &cfg::moc::Arm::lower_joint_bound)
        .def_readwrite("upper_joint_bound", &cfg::moc::Arm::upper_joint_bound)
    ;

    class_<cfg::moc::Joint, boost::noncopyable>("Joint", no_init)
        .def_readwrite("name", &cfg::moc::Joint::name)
        .def_readwrite("logical_axis", &cfg::moc::Joint::logical_axis)
        .def_readwrite("kinematic_axis_number", &cfg::moc::Joint::kinematic_axis_number)
        .def_readwrite("use_arm", &cfg::moc::Joint::use_arm)
        .def_readwrite("use_transmission", &cfg::moc::Joint::use_transmission)
    ;

    class_<cfg::moc::MechanicalUnit, boost::noncopyable>("MechanicalUnit", no_init)
        .def_readwrite("name", &cfg::moc::MechanicalUnit::name)
        .def_readwrite("use_robot", &cfg::moc::MechanicalUnit::use_robot)
        .def_readwrite("use_singles", &cfg::moc::MechanicalUnit::use_singles)
    ;

    class_<cfg::moc::Robot, boost::noncopyable>("Robot", no_init)
        .def_readwrite("name", &cfg::moc::Robot::name)
        .def_readwrite("use_robot_type", &cfg::moc::Robot::use_robot_type)
        .def_readwrite("use_joints", &cfg::moc::Robot::use_joints)
        .def_readwrite("base_frame", &cfg::moc::Robot::base_frame)
        .def_readwrite("base_frame_moved_by", &cfg::moc::Robot::base_frame_moved_by)
    ;

    class_<cfg::moc::Single, boost::noncopyable>("Single", no_init)
        .def_readwrite("name", &cfg::moc::Single::name)
        .def_readwrite("use_single_type", &cfg::moc::Single::use_single_type)
        .def_readwrite("use_joint", &cfg::moc::Single::use_joint)
        .def_readwrite("base_frame", &cfg::moc::Single::base_frame)
        .def_readwrite("base_frame_coordinated", &cfg::moc::Single::base_frame_coordinated)
    ;

    class_<cfg::moc::Transmission, boost::noncopyable>("Transmission", no_init)
        .def_readwrite("name", &cfg::moc::Transmission::name)
        .def_readwrite("rotating_move", &cfg::moc::Transmission::rotating_move)
    ;

    class_<cfg::sys::MechanicalUnitGroup, boost::noncopyable>("MechanicalUnitGroup", no_init)
        .def_readwrite("name", &cfg::sys::MechanicalUnitGroup::name)
        .def_readwrite("robot", &cfg::sys::MechanicalUnitGroup::robot)
        .def_readwrite("mechanical_units", &cfg::sys::MechanicalUnitGroup::mechanical_units)
    ;

    class_<cfg::sys::PresentOption, boost::noncopyable>("MechanicalUnitGroup", no_init)
        .def_readwrite("name", &cfg::sys::PresentOption::name)
        .def_readwrite("description", &cfg::sys::PresentOption::description)
    ;

    /*rws_client*/
    class_<RWSClient::RAPIDSymbolResource, boost::noncopyable>("RAPIDSymbolResource", init<const std::string&, const std::string&>())
        .def_readwrite("module", &RWSClient::RAPIDSymbolResource::module)
        .def_readwrite("name", &RWSClient::RAPIDSymbolResource::name)
    ;


    class_<RWSClient::RAPIDResource, boost::noncopyable>("RAPIDResource", init<const std::string&, const std::string&, const std::string&>())
        .def(init<const std::string&, const RWSClient::RAPIDSymbolResource&>())
        .def_readwrite("task", &RWSClient::RAPIDResource::task)
        .def_readwrite("module", &RWSClient::RAPIDResource::module)
        .def_readwrite("name", &RWSClient::RAPIDResource::name)
    ;

    class_<RWSClient::FileResource, boost::noncopyable>("FileResource", init<const std::string&>())
        .def(init<const std::string&, const std::string&>())
        .def_readwrite("filename", &RWSClient::FileResource::filename)
        .def_readwrite("directory", &RWSClient::FileResource::directory)
    ;

    enum_<RWSClient::SubscriptionResources::Priority>("Priority")
        .value("LOW", RWSClient::SubscriptionResources::LOW)
        .value("MEDIUM", RWSClient::SubscriptionResources::MEDIUM)
        .value("HIGH", RWSClient::SubscriptionResources::HIGH)
    ;

    class_<RWSClient::SubscriptionResources::SubscriptionResource, boost::noncopyable>("SubscriptionResource", init<const std::string&, const RWSClient::SubscriptionResources::Priority>())
        .def_readwrite("resource_uri", &RWSClient::SubscriptionResources::SubscriptionResource::resource_uri)
        .def_readwrite("priority", &RWSClient::SubscriptionResources::SubscriptionResource::priority)
    ;

    class_<RWSClient::SubscriptionResources>("SubscriptionResources")
        .def("add", &RWSClient::SubscriptionResources::add)
        .def("addIOSignal", &RWSClient::SubscriptionResources::addIOSignal)
        .def("addRAPIDPersistantVariable", &RWSClient::SubscriptionResources::addRAPIDPersistantVariable)
        //.def("getResources", &RWSClient::SubscriptionResources::getResources)
    ;

    enum_<RWSClient::Coordinate>("Coordinate")
        .value("BASE", RWSClient::BASE)
        .value("WORLD", RWSClient::WORLD)
        .value("TOOL", RWSClient::TOOL)
        .value("WOBJ", RWSClient::WOBJ)
        .value("ACTIVE", RWSClient::ACTIVE)
    ;


    /*rws_rapid*/

    class_<RAPIDSymbolDataAbstract, boost::noncopyable>("RAPIDSymbolDataAbstract", no_init)
        .def("getType", &RAPIDSymbolDataAbstract::getType)
        .def("parseString", &RAPIDSymbolDataAbstract::parseString)
        .def("constructString", &RAPIDSymbolDataAbstract::constructString)
        .def("__str__", &RAPIDSymbolDataAbstract::constructString)
    ;

    class_<RAPIDBool, bases<RAPIDSymbolDataAbstract> >("RAPIDBool", init<>())
        .def(init<const bool>())
    ;

    implicitly_convertible<bool, RAPIDBool>();

    class_<RAPIDNum, bases<RAPIDSymbolDataAbstract> >("RAPIDNum", init<>())
        .def(init<const float>())
    ;

    implicitly_convertible<float,RAPIDNum>();

    class_<RAPIDDnum, bases<RAPIDSymbolDataAbstract> >("RAPIDDnum", init<>())
        .def(init<const double>())
    ;

    implicitly_convertible<float,RAPIDDnum>();

    class_<RAPIDString, bases<RAPIDSymbolDataAbstract> >("RAPIDString", init<>())
        .def(init<const std::string&>())
    ;

    implicitly_convertible<std::string,RAPIDString>();


    class_<RAPIDRecord, bases<RAPIDSymbolDataAbstract> >("RAPIDRecord", init<const std::string&>())

    ;

    class_<PyRAPIDRecord, bases<RAPIDRecord> >("PyRAPIDRecord", init<const std::string&>())
        .def<void (PyRAPIDRecord::*)(RAPIDSymbolDataAbstract*)>("addComponents", &PyRAPIDRecord::addComponents)

    ;

    class_<RobJoint, bases<RAPIDRecord> >("RobJoint", init<>())
        .def_readwrite("rax_1", &RobJoint::rax_1)
        .def_readwrite("rax_2", &RobJoint::rax_2)
        .def_readwrite("rax_3", &RobJoint::rax_3)
        .def_readwrite("rax_4", &RobJoint::rax_4)
        .def_readwrite("rax_5", &RobJoint::rax_5)
        .def_readwrite("rax_6", &RobJoint::rax_6)
    ;

    class_<ExtJoint, bases<RAPIDRecord> >("ExtJoint", init<>())
        .def_readwrite("eax_a", &ExtJoint::eax_a)
        .def_readwrite("eax_b", &ExtJoint::eax_b)
        .def_readwrite("eax_c", &ExtJoint::eax_c)
        .def_readwrite("eax_d", &ExtJoint::eax_d)
        .def_readwrite("eax_e", &ExtJoint::eax_e)
        .def_readwrite("eax_f", &ExtJoint::eax_f)
    ;

    class_<JointTarget, bases<RAPIDRecord> >("JointTarget", init<>())
        .def(init<const JointTarget&>())
        .def_readwrite("robax", &JointTarget::robax)
        .def_readwrite("extax", &JointTarget::extax)
    ;

    class_<Pos, bases<RAPIDRecord> >("Pos", init<>())
        .def_readwrite("x", &Pos::x)
        .def_readwrite("y", &Pos::y)
        .def_readwrite("z", &Pos::z)
    ;

    class_<Orient, bases<RAPIDRecord> >("Orient", init<>())
        .def_readwrite("q1", &Orient::q1)
        .def_readwrite("q2", &Orient::q2)
        .def_readwrite("q3", &Orient::q3)
        .def_readwrite("q4", &Orient::q4)
    ;

    class_<Pose, bases<RAPIDRecord> >("Pose", init<>())
        .def(init<const Pose&>())
        .def_readwrite("pos", &Pose::pos)
        .def_readwrite("rot", &Pose::rot)
    ;

    class_<ConfData, bases<RAPIDRecord> >("ConfData", init<>())
        .def_readwrite("q1", &ConfData::cf1)
        .def_readwrite("q2", &ConfData::cf4)
        .def_readwrite("q3", &ConfData::cf6)
        .def_readwrite("q4", &ConfData::cfx)
    ;

    class_<RobTarget, bases<RAPIDRecord> >("RobTarget", init<>())
        .def(init<const RobTarget&>())
        .def_readwrite("pos", &RobTarget::pos)
        .def_readwrite("orient", &RobTarget::orient)
        .def_readwrite("robconf", &RobTarget::robconf)
        .def_readwrite("extax", &RobTarget::extax)
    ;

    class_<LoadData, bases<RAPIDRecord> >("LoadData", init<>())
        .def(init<const LoadData&>())
        .def_readwrite("mass", &LoadData::mass)
        .def_readwrite("cog", &LoadData::cog)
        .def_readwrite("aom", &LoadData::aom)
        .def_readwrite("ix", &LoadData::ix)
        .def_readwrite("iy", &LoadData::iy)
        .def_readwrite("iz", &LoadData::iz)
    ;

    class_<ToolData, bases<RAPIDRecord> >("ToolData", init<>())
        .def(init<const ToolData&>())
        .def_readwrite("robhold", &ToolData::robhold)
        .def_readwrite("tframe", &ToolData::tframe)
        .def_readwrite("tload", &ToolData::tload)
    ;

    class_<WObjData, bases<RAPIDRecord> >("WObjData", init<>())
        .def(init<const WObjData&>())
        .def_readwrite("robhold", &WObjData::robhold)
        .def_readwrite("ufprog", &WObjData::ufprog)
        .def_readwrite("ufmec", &WObjData::ufmec)
        .def_readwrite("uframe", &WObjData::uframe)
        .def_readwrite("oframe", &WObjData::oframe)
    ;

    class_<SpeedData, bases<RAPIDRecord> >("SpeedData", init<>())
        .def_readwrite("v_tcp", &SpeedData::v_tcp)
        .def_readwrite("v_ori", &SpeedData::v_ori)
        .def_readwrite("v_leax", &SpeedData::v_leax)
        .def_readwrite("v_reax", &SpeedData::v_reax)
    ;

    /* RWSInterface */
    enum_<RWSInterface::RAPIDTaskExecutionState>("RAPIDTaskExecutionState")
        .value("UNKNOWN", RWSInterface::UNKNOWN)
        .value("READY", RWSInterface::READY)
        .value("STOPPED", RWSInterface::STOPPED)
        .value("STARTED", RWSInterface::STARTED)
        .value("UNINITIALIZED", RWSInterface::UNINITIALIZED)
    ;

    enum_<RWSInterface::MechanicalUnitType>("MechanicalUnitType")
        .value("NONE", RWSInterface::NONE)
        .value("TCP_ROBOT", RWSInterface::TCP_ROBOT)
        .value("ROBOT", RWSInterface::ROBOT)
        .value("SINGLE", RWSInterface::SINGLE)
        .value("UNDEFINED", RWSInterface::UNDEFINED)
    ;

    enum_<RWSInterface::MechanicalUnitMode>("MechanicalUnitMode")
        .value("UNKNOWN_MODE", RWSInterface::UNKNOWN_MODE)
        .value("ACTIVATED", RWSInterface::ACTIVATED)
        .value("DEACTIVATED", RWSInterface::DEACTIVATED)
    ;

    class_<RWSInterface::MechanicalUnitStaticInfo>("MechanicalUnitStaticInfo")
        .def_readwrite("type", &RWSInterface::MechanicalUnitStaticInfo::type)
        .def_readwrite("task_name", &RWSInterface::MechanicalUnitStaticInfo::task_name)
        .def_readwrite("axes", &RWSInterface::MechanicalUnitStaticInfo::axes)
        .def_readwrite("axes_total", &RWSInterface::MechanicalUnitStaticInfo::axes_total)
        .def_readwrite("is_integrated_unit", &RWSInterface::MechanicalUnitStaticInfo::is_integrated_unit)
        .def_readwrite("has_integrated_unit", &RWSInterface::MechanicalUnitStaticInfo::has_integrated_unit)
    ;

    class_<RWSInterface::MechanicalUnitDynamicInfo>("MechanicalUnitDynamicInfo")
        .def_readwrite("tool_name", &RWSInterface::MechanicalUnitDynamicInfo::tool_name)
        .def_readwrite("wobj_name", &RWSInterface::MechanicalUnitDynamicInfo::wobj_name)
        .def_readwrite("payload_name", &RWSInterface::MechanicalUnitDynamicInfo::payload_name)
        .def_readwrite("total_payload_name", &RWSInterface::MechanicalUnitDynamicInfo::total_payload_name)
        .def_readwrite("status", &RWSInterface::MechanicalUnitDynamicInfo::status)
        .def_readwrite("mode", &RWSInterface::MechanicalUnitDynamicInfo::mode)
        .def_readwrite("jog_mode", &RWSInterface::MechanicalUnitDynamicInfo::jog_mode)
        .def_readwrite("coord_system", &RWSInterface::MechanicalUnitDynamicInfo::coord_system)
    ;

    class_<RWSInterface::SystemInfo>("SystemInfo")
        .def_readwrite("robot_ware_version", &RWSInterface::SystemInfo::robot_ware_version)
        .def_readwrite("system_name", &RWSInterface::SystemInfo::system_name)
        .def_readwrite("system_type", &RWSInterface::SystemInfo::system_type)
        .def_readwrite("system_options", &RWSInterface::SystemInfo::system_options)
    ;

    class_<RWSInterface::RobotWareOptionInfo>("RobotWareOptionInfo", init<std::string&, std::string&>())
        .def_readwrite("name", &RWSInterface::RobotWareOptionInfo::name)
        .def_readwrite("description", &RWSInterface::RobotWareOptionInfo::description)
    ;

    class_<RWSInterface::RAPIDModuleInfo>("RAPIDModuleInfo", init<std::string&, std::string&>())
        .def_readwrite("name", &RWSInterface::RAPIDModuleInfo::name)
        .def_readwrite("type", &RWSInterface::RAPIDModuleInfo::type)
    ;

    class_<RWSInterface::RAPIDTaskInfo>("RAPIDTaskInfo", init<const std::string&, const bool, const bool, const RWSInterface::RAPIDTaskExecutionState>())
        .def_readwrite("name", &RWSInterface::RAPIDTaskInfo::name)
        .def_readwrite("is_motion_task", &RWSInterface::RAPIDTaskInfo::is_motion_task)
        .def_readwrite("is_active", &RWSInterface::RAPIDTaskInfo::is_active)
        .def_readwrite("execution_state", &RWSInterface::RAPIDTaskInfo::execution_state)
    ;

    class_<RWSInterface::StaticInfo>("StaticInfo")
        .def_readwrite("rapid_tasks", &RWSInterface::StaticInfo::rapid_tasks)
        .def_readwrite("system_info", &RWSInterface::StaticInfo::system_info)
    ;

    class_<RWSInterface::RuntimeInfo>("RuntimeInfo")
        .def_readwrite("auto_mode", &RWSInterface::RuntimeInfo::auto_mode)
        .def_readwrite("motors_on", &RWSInterface::RuntimeInfo::motors_on)
        .def_readwrite("rapid_running", &RWSInterface::RuntimeInfo::rapid_running)
        .def_readwrite("rws_connected", &RWSInterface::RuntimeInfo::rws_connected)
    ;

    class_<RWSInterfacePy, boost::noncopyable>("RWSInterface", init<const std::string&>())
        .def(init<std::string&, std::string&, std::string&>())
        .def(init<std::string&, unsigned short>())
        .def(init<std::string&, unsigned short, std::string&, std::string&>())

        .def("collectRuntimeInfo", &RWSInterfacePy::collectRuntimeInfo)
        .def("collectStaticInfo", &RWSInterfacePy::collectStaticInfo)
        .def("getCFGArms", &RWSInterfacePy::getCFGArms)
        .def("getCFGJoints", &RWSInterfacePy::getCFGJoints)
        .def("getCFGMechanicalUnits", &RWSInterfacePy::getCFGMechanicalUnits)
        .def("getCFGMechanicalUnitGroups", &RWSInterfacePy::getCFGMechanicalUnitGroups)
        .def("getCFGPresentOptions", &RWSInterfacePy::getCFGPresentOptions)
        .def("getCFGRobots", &RWSInterfacePy::getCFGRobots)
        .def("getCFGSingles", &RWSInterfacePy::getCFGSingles)
        .def("getCFGTransmission", &RWSInterfacePy::getCFGTransmission)
        //Deprecated
        //.def("getPresentRobotWareOptions", &RWSInterface::getPresentRobotWareOptions)
        .def("getIOSignal", &RWSInterfacePy::getIOSignal)
        .def("getMechanicalUnitStaticInfo", &RWSInterfacePy::getMechanicalUnitStaticInfo)
        .def("getMechanicalUnitDynamicInfo", &RWSInterfacePy::getMechanicalUnitDynamicInfo)
        .def("getMechanicalUnitJointTarget", &RWSInterfacePy::getMechanicalUnitJointTarget)

        .def<bool (RWSInterfacePy::*)(const std::string&, RobTarget*, const RWSClient::Coordinate&,
                                      const std::string&, const std::string&)>("getMechanicalUnitRobTarget",
                                      &RWSInterfacePy::getMechanicalUnitRobTarget, getMechanicalUnitRobTarget_overloads(
                                      (arg("mechunit"), arg("p_robtarget"), arg("coordinate") = RWSClient::ACTIVE,
                                      arg("tool") = "", arg("wobj") = "")))


        .def<std::string (RWSInterfacePy::*)(const std::string&, const std::string&, const std::string&)>("getRAPIDSymbolData", &RWSInterfacePy::getRAPIDSymbolData)
        .def<bool (RWSInterfacePy::*)(const std::string&, const std::string&, const std::string&, RAPIDSymbolDataAbstract*)>("getRAPIDSymbolData", &RWSInterfacePy::getRAPIDSymbolData)
        .def<bool (RWSInterfacePy::*)(const std::string&, const RWSClient::RAPIDSymbolResource&, RAPIDSymbolDataAbstract*)>("getRAPIDSymbolData", &RWSInterfacePy::getRAPIDSymbolData)

        .def("getRAPIDModulesInfo", &RWSInterfacePy::getRAPIDModulesInfo)
        .def("getRAPIDTasks", &RWSInterfacePy::getRAPIDTasks)
        .def("getSpeedRatio", &RWSInterfacePy::getSpeedRatio)
        .def("getSystemInfo", &RWSInterfacePy::getSystemInfo)
        .def("isAutoMode", &RWSInterfacePy::isAutoMode)
        .def("isMotorsOn", &RWSInterfacePy::isMotorsOn)
        .def("isRAPIDRunning", &RWSInterfacePy::isRAPIDRunning)
        .def("setIOSignal", &RWSInterfacePy::setIOSignal)

        .def<bool (RWSInterfacePy::*)(const std::string&, const std::string&, const std::string&, const std::string&)>("setRAPIDSymbolData", &RWSInterfacePy::setRAPIDSymbolData)
        .def<bool (RWSInterfacePy::*)(const std::string&, const std::string&, const std::string&, const RAPIDSymbolDataAbstract&)>("setRAPIDSymbolData", &RWSInterfacePy::setRAPIDSymbolData)
        .def<bool (RWSInterfacePy::*)(const std::string&, const RWSClient::RAPIDSymbolResource&, const RAPIDSymbolDataAbstract&)>("setRAPIDSymbolData", &RWSInterfacePy::setRAPIDSymbolData)

        .def("startRAPIDExecution", &RWSInterfacePy::startRAPIDExecution)
        .def("stopRAPIDExecution", &RWSInterfacePy::stopRAPIDExecution)
        .def("resetRAPIDProgramPointer", &RWSInterfacePy::resetRAPIDProgramPointer)
        .def("setMotorsOn", &RWSInterfacePy::setMotorsOn)
        .def("setMotorsOff", &RWSInterfacePy::setMotorsOff)
        .def("setSpeedRatio", &RWSInterfacePy::setSpeedRatio)
        .def("getFile", &RWSInterfacePy::getFile)
        .def("uploadFile", &RWSInterfacePy::uploadFile)
        .def("deleteFile", &RWSInterfacePy::deleteFile)
        .def("startSubscription", &RWSInterfacePy::startSubscription)
        .def<bool (RWSInterfacePy::*)()>("waitForSubscriptionEvent", &RWSInterfacePy::waitForSubscriptionEvent)
        //.def<bool (RWSInterface::*)(Poco::AutoPtr<Poco::XML::Document>*)>("waitForSubscriptionEvent", &RWSInterface::waitForSubscriptionEvent)
        .def("waitForSubscriptionEventAsString", &RWSInterfacePy::waitForSubscriptionEventAsString)
        .def("endSubscription", &RWSInterfacePy::endSubscription)
        .def("forceCloseSubscription", &RWSInterfacePy::forceCloseSubscription)
        .def<bool (RWSInterfacePy::*)(const std::string&, const std::string&, const std::string&)>("registerLocalUser", &RWSInterfacePy::registerLocalUser,
                                      registerLocalUser_overloads((arg("username") = SystemConstants::General::DEFAULT_USERNAME,
                                                                   arg("application") = SystemConstants::General::EXTERNAL_APPLICATION,
                                                                   arg("location") = SystemConstants::General::EXTERNAL_LOCATION)))
        .def<bool (RWSInterfacePy::*)(const std::string&, const std::string&, const std::string&)>("registerRemoteUser", &RWSInterfacePy::registerRemoteUser,
                                      registerRemoteUser_overloads((arg("username") = SystemConstants::General::DEFAULT_USERNAME,
                                                                    arg("application") = SystemConstants::General::EXTERNAL_APPLICATION,
                                                                    arg("location") = SystemConstants::General::EXTERNAL_LOCATION)))
        .def<std::string (RWSInterfacePy::*)(const bool)>("getLogText", &RWSInterfacePy::getLogText, getLogText_overloads((arg("verbose") = false)))
        .def<std::string (RWSInterfacePy::*)(const bool)>("getLogTextLatestEvent", &RWSInterfacePy::getLogTextLatestEvent, getLogTextLatestEvent_overloads((arg("verbose") = false)))
        .def("setHTTPTimeout", &RWSInterfacePy::setHTTPTimeout)
    ;
}
