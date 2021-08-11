#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <abb_librws/rws_interface.h>

using namespace pybind11::literals;
namespace py = pybind11;

namespace abb {
namespace rws {

void init_interface(py::module &m){
    py::class_<RWSInterface> iface(m, "RWSInterface");

    py::enum_<RAPIDTaskExecutionState>(m, "RAPIDTaskExecutionState")
        .value("UNKNOWN", RAPIDTaskExecutionState::UNKNOWN)
        .value("READY", RAPIDTaskExecutionState::READY)
        .value("STOPPED", RAPIDTaskExecutionState::STOPPED)
        .value("STARTED", RAPIDTaskExecutionState::STARTED)
        .value("UNINITIALIZED", RAPIDTaskExecutionState::UNINITIALIZED)
        .export_values()
    ;

    py::enum_<MechanicalUnitType>(m, "MechanicalUnitType")
        .value("NONE", MechanicalUnitType::NONE)
        .value("TCP_ROBOT", MechanicalUnitType::TCP_ROBOT)
        .value("ROBOT", MechanicalUnitType::ROBOT)
        .value("SINGLE", MechanicalUnitType::SINGLE)
        .value("UNDEFINED", MechanicalUnitType::UNDEFINED)
        .export_values()
    ;

    py::enum_<MechanicalUnitMode>(m, "MechanicalUnitMode")
        .value("UNKNOWN_MODE", MechanicalUnitMode::UNKNOWN_MODE)
        .value("ACTIVATED", MechanicalUnitMode::ACTIVATED)
        .value("DEACTIVATED", MechanicalUnitMode::DEACTIVATED)
        .export_values()
    ;

    py::class_<MechanicalUnitStaticInfo>(m, "MechanicalUnitStaticInfo")
        .def_readonly("type", &MechanicalUnitStaticInfo::type)
        .def_readonly("task_name", &MechanicalUnitStaticInfo::task_name)
        .def_readonly("axes", &MechanicalUnitStaticInfo::axes)
        .def_readonly("axes_total", &MechanicalUnitStaticInfo::axes_total)
        .def_readonly("is_integrated_unit", &MechanicalUnitStaticInfo::is_integrated_unit)
        .def_readonly("has_integrated_unit", &MechanicalUnitStaticInfo::has_integrated_unit)
    ;

    py::class_<MechanicalUnitDynamicInfo>(m, "MechanicalUnitDynamicInfo")
        .def_readonly("tool_name", &MechanicalUnitDynamicInfo::tool_name)
        .def_readonly("wobj_name", &MechanicalUnitDynamicInfo::wobj_name)
        .def_readonly("payload_name", &MechanicalUnitDynamicInfo::payload_name)
        .def_readonly("total_payload_name", &MechanicalUnitDynamicInfo::total_payload_name)
        .def_readonly("status", &MechanicalUnitDynamicInfo::status)
        .def_readonly("mode", &MechanicalUnitDynamicInfo::mode)
        .def_readonly("jog_mode", &MechanicalUnitDynamicInfo::jog_mode)
    ;

    py::class_<SystemInfo>(m, "SystemInfo")
        .def_readonly("robot_ware_version", &SystemInfo::robot_ware_version)
        .def_readonly("system_name", &SystemInfo::system_name)
        .def_readonly("system_type", &SystemInfo::system_type)
        .def_readonly("system_options", &SystemInfo::system_options)
    ;

    py::class_<RAPIDModuleInfo>(m, "RAPIDModuleInfo")
        .def_readonly("name", &RAPIDModuleInfo::name)
        .def_readonly("type", &RAPIDModuleInfo::type)
    ;

    py::class_<RAPIDTaskInfo>(m, "RAPIDTaskInfo")
        .def_readonly("name", &RAPIDTaskInfo::name)
        .def_readonly("motion_task", &RAPIDTaskInfo::is_motion_task)
        .def_readonly("active", &RAPIDTaskInfo::is_active)
        .def_readonly("execution_state", &RAPIDTaskInfo::execution_state)
    ;

    py::class_<StaticInfo>(m, "StaticInfo")
        .def_readonly("rapid_tasks", &StaticInfo::rapid_tasks)
        .def_readonly("system_info", &StaticInfo::system_info)
    ;

    py::class_<RuntimeInfo>(m, "RuntimeInfo")
        .def_readonly("auto_mode", &RuntimeInfo::auto_mode)
        .def_readonly("motors_on", &RuntimeInfo::motors_on)
        .def_readonly("rapid_running", &RuntimeInfo::rapid_running)
        .def_readonly("rws_connected", &RuntimeInfo::rws_connected)
    ;

    iface.def(py::init<const std::string &>(), "ip_address"_a = "192.168.125.1")
        .def_property_readonly("runtime_info", &RWSInterface::collectRuntimeInfo)
        .def_property_readonly("static_info", &RWSInterface::collectStaticInfo)
        .def("arms", &RWSInterface::getCFGArms)
        .def("joints", &RWSInterface::getCFGJoints)
        .def("mechanical_units", &RWSInterface::getCFGMechanicalUnits)
        .def("mechanical_unit_groups", &RWSInterface::getCFGMechanicalUnitGroups)
        .def("present_options", &RWSInterface::getCFGPresentOptions)
        .def("robots", &RWSInterface::getCFGRobots)
        .def("singles", &RWSInterface::getCFGSingles)
        .def("transmission", &RWSInterface::getCFGTransmission)
        .def("get_digital_signal", &RWSInterface::getDigitalSignal)
        .def("get_analog_signal", &RWSInterface::getAnalogSignal)
        .def("mechanical_unit_static_info", &RWSInterface::getMechanicalUnitStaticInfo)
        .def("mechanical_unit_dynamic_info", &RWSInterface::getMechanicalUnitDynamicInfo)
        .def("mechanical_unit_joint_target", &RWSInterface::getMechanicalUnitJointTarget)
        .def("mechanical_unit_rob_target", &RWSInterface::getMechanicalUnitRobTarget, "mechunit"_a, "coordinate"_a = Coordinate::ACTIVE, "tool"_a = "tool0", "wobj"_a = "wobj0")
        .def("get_rapid_symbol_data", py::overload_cast<const std::string&, const std::string&, const std::string&>(&RWSInterface::getRAPIDSymbolData), "task"_a, "module"_a, "name"_a)
        .def("rapid_modules_info", &RWSInterface::getRAPIDModulesInfo)
        .def_property_readonly("rapid_tasks", &RWSInterface::getRAPIDTasks)
        .def("get_speed_ratio", &RWSInterface::getSpeedRatio)
        .def_property_readonly("system_info", &RWSInterface::getSystemInfo)
        .def_property_readonly("auto_mode", &RWSInterface::isAutoMode)
        .def_property_readonly("motors_on", &RWSInterface::isMotorsOn)
        .def_property_readonly("rapid_running", &RWSInterface::isRAPIDRunning)
        .def("set_digital_signal", &RWSInterface::setDigitalSignal)
        .def("set_analog_signal", &RWSInterface::setAnalogSignal)
        .def("set_rapid_symbol_data", py::overload_cast<const std::string&, const std::string&, const std::string&, const std::string&>(&RWSInterface::setRAPIDSymbolData), "task"_a, "module"_a, "name"_a, "data"_a)
        .def("start_rapid", &RWSInterface::startRAPIDExecution)
        .def("stop_rapid", &RWSInterface::stopRAPIDExecution)
        .def("reset_program_pointer", &RWSInterface::resetRAPIDProgramPointer)
        .def("set_motors_on", &RWSInterface::setMotorsOn)
        .def("set_motors_off", &RWSInterface::setMotorsOff)
        .def("set_speed_ratio", &RWSInterface::setSpeedRatio)
        .def("get_file", &RWSInterface::getFile)
        .def("upload_file", &RWSInterface::uploadFile)
        .def("delete_file", &RWSInterface::deleteFile)
        .def("log_text", &RWSInterface::getLogText, "verbose"_a = false)
        .def("log_text_latest", &RWSInterface::getLogTextLatestEvent, "verbose"_a = false)
    ;

}
} // end namespace rws
} // end namespace abb
