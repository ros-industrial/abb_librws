#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <abb_librws/rws_state_machine_interface.h>

using namespace pybind11::literals;
namespace py = pybind11;

namespace abb {
namespace rws {

void init_rapid(py::module &);
void init_cfg(py::module &);
void init_resource(py::module &);
void init_client(py::module &);
void init_interface(py::module &);

using Services = std::decay_t<std::result_of_t<decltype(&RWSStateMachineInterface::services)(RWSStateMachineInterface)>>;
using SG = std::decay_t<std::result_of_t<decltype(&Services::sg)(Services)>>;
using Main = std::decay_t<std::result_of_t<decltype(&Services::main)(Services)>>;
using RAPID = std::decay_t<std::result_of_t<decltype(&Services::rapid)(Services)>>;

PYBIND11_MODULE(_abb_librws, m) {
    init_rapid(m);
    init_cfg(m);
    init_resource(m);
    init_client(m);
    init_interface(m);

    py::class_<RWSStateMachineInterface, RWSInterface> sm_iface(m, "RWSStateMachineInterface");

    py::enum_<RWSStateMachineInterface::States>(m, "States")
        .value("IDLE", RWSStateMachineInterface::States::STATE_IDLE)
        .value("INITIALIZE", RWSStateMachineInterface::States::STATE_INITIALIZE)
        .value("RUN_RAPID_ROUTINE", RWSStateMachineInterface::States::STATE_RUN_RAPID_ROUTINE)
        .value("RUN_EGM_ROUTINE", RWSStateMachineInterface::States::STATE_RUN_EGM_ROUTINE)
        .value("UNKNOWN", RWSStateMachineInterface::States::STATE_UNKNOWN)
        .export_values()
    ;

    sm_iface.def(py::init<const std::string &>(), "ip_address"_a = "192.168.125.1")
        .def("services", &RWSStateMachineInterface::services)
    ;

    py::class_<Services>(m, "Services")
        .def("sg", &Services::sg)
        .def("main", &Services::main)
        .def("rapid", &Services::rapid)
    ;

    py::class_<SG>(m, "SmartGripper")
        .def("dual_calibrate", &SG::dualCalibrate)
        .def("dual_initialize", &SG::dualInitialize)
        .def("dual_get_settings", 
            [](SG& self) {
                RWSStateMachineInterface::SGSettings* lsgs = new RWSStateMachineInterface::SGSettings();
                RWSStateMachineInterface::SGSettings* rsgs = new RWSStateMachineInterface::SGSettings();
                self.dualGetSettings(lsgs, rsgs);
                return std::make_tuple(lsgs, rsgs);
            }
        )
        .def("dual_grip_in", &SG::dualGripIn)
        .def("dual_grip_out", &SG::dualGripOut)
        .def("dual_move_to", &SG::dualMoveTo)
        .def("dual_set_settings", &SG::dualSetSettings)
        .def("left_calibrate", &SG::leftCalibrate)
        .def("left_initialize", &SG::leftInitialize)
        .def("left_get_settings", 
            [](SG& self) {
                RWSStateMachineInterface::SGSettings* lsgs = new RWSStateMachineInterface::SGSettings();
                self.leftGetSettings(lsgs);
                return lsgs;
            }
        )
        .def("left_grip_in", &SG::leftGripIn)
        .def("left_grip_out", &SG::leftGripOut)
        .def("left_move_to", &SG::leftMoveTo)
        .def("left_set_settings", &SG::leftSetSettings)
        .def("right_calibrate", &SG::rightCalibrate)
        .def("right_initialize", &SG::rightInitialize)
        .def("right_get_settings", 
            [](SG& self) {
                RWSStateMachineInterface::SGSettings* rsgs = new RWSStateMachineInterface::SGSettings();
                self.rightGetSettings(rsgs);
                return rsgs;
            }
        )
        .def("right_grip_in", &SG::rightGripIn)
        .def("right_grip_out", &SG::rightGripOut)
        .def("right_move_to", &SG::rightMoveTo)
        .def("right_set_settings", &SG::rightSetSettings)
    ;

    py::class_<Main>(m, "Main")
        .def("get_state", &Main::getCurrentState)
        .def("is_idle", &Main::isStateIdle)
        .def("is_stationary", &Main::isStationary)
    ;

    py::class_<RAPID>(m, "Rapid")
        .def("run_module_load", &RAPID::runModuleLoad)
        .def("run_module_unload", &RAPID::runModuleUnload)
        .def("run_move_abs_j", &RAPID::runMoveAbsJ)
        .def("run_move_j", &RAPID::runMoveJ)
        .def("run_call_by_var", &RAPID::runCallByVar)
        .def("run_move_to_calibration_position", &RAPID::runMoveToCalibrationPosition)
        .def("set_move_speed", &RAPID::setMoveSpeed)
        .def("set_routine", &RAPID::setRoutineName)
        .def("run_routine", &RAPID::signalRunRAPIDRoutine)
    ;

    py::class_<RWSStateMachineInterface::SGSettings, RAPIDRecord>(m, "SGSettings")
        .def(py::init<>())
        .def(py::init(
            [](RAPIDNum& max_speed, RAPIDNum& hold_force, RAPIDNum& physical_limit) {
                RWSStateMachineInterface::SGSettings* sg = new RWSStateMachineInterface::SGSettings();
                sg->max_speed = max_speed;
                sg->hold_force = hold_force;
                sg->physical_limit = physical_limit;
                return sg;
            }),
            "max_speed"_a, "hold_force"_a, "physical_limit"_a
        )
        .def(py::init(
            [](py::array_t<double> arr) {
                auto arr_u = arr.unchecked<1>();
                if (arr_u.size() != 3) {
                    throw std::length_error("Input array is not of length 3!");
                }
                RWSStateMachineInterface::SGSettings* sg = new RWSStateMachineInterface::SGSettings();
                sg->max_speed = arr_u(0);
                sg->hold_force = arr_u(1);
                sg->physical_limit = arr_u(2);
                return sg;
            }),
            "sg_settings"_a
        )
        .def_readwrite("max_speed", &RWSStateMachineInterface::SGSettings::max_speed)
        .def_readwrite("hold_force", &RWSStateMachineInterface::SGSettings::hold_force)
        .def_readwrite("physical_limit", &RWSStateMachineInterface::SGSettings::physical_limit)
    ;

}
} // end namespace rws
} // end namespace abb
