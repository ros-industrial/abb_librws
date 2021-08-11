#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <abb_librws/rws_cfg.h>

using namespace pybind11::literals;
namespace py = pybind11;

namespace abb {
namespace rws {

void init_cfg(py::module &m) {

    py::class_<cfg::moc::Arm>(m, "Arm")
        .def_readonly("name", &cfg::moc::Arm::name)
        .def_readonly("lower_joint_bound", &cfg::moc::Arm::lower_joint_bound)
        .def_readonly("upper_joint_bound", &cfg::moc::Arm::upper_joint_bound)
    ;

    py::class_<cfg::moc::Joint>(m, "Joint")
        .def_readonly("name", &cfg::moc::Joint::name)
        .def_readonly("logical_axis", &cfg::moc::Joint::logical_axis)
        .def_readonly("kinematic_axis_number", &cfg::moc::Joint::kinematic_axis_number)
        .def_readonly("arm", &cfg::moc::Joint::use_arm)
        .def_readonly("transmission", &cfg::moc::Joint::use_transmission)
    ;
    
    py::class_<cfg::moc::MechanicalUnit>(m, "MechanicalUnit")
        .def_readonly("name", &cfg::moc::MechanicalUnit::name)
        .def_readonly("robot", &cfg::moc::MechanicalUnit::use_robot)
        .def_readonly("singles", &cfg::moc::MechanicalUnit::use_singles)
    ;

    py::class_<cfg::moc::Robot>(m, "Robot")
        .def_readonly("name", &cfg::moc::Robot::name)
        .def_readonly("type", &cfg::moc::Robot::use_robot_type)
        .def_readonly("joints", &cfg::moc::Robot::use_joints)
        .def_readonly("base_frame", &cfg::moc::Robot::base_frame)
        .def_readonly("base_frame_moved_by", &cfg::moc::Robot::base_frame_moved_by)
    ;

    py::class_<cfg::moc::Single>(m, "Single")
        .def_readonly("name", &cfg::moc::Single::name)
        .def_readonly("type", &cfg::moc::Single::use_single_type)
        .def_readonly("joint", &cfg::moc::Single::use_joint)
        .def_readonly("base_frame", &cfg::moc::Single::base_frame)
        .def_readonly("base_frame_coordinated", &cfg::moc::Single::base_frame_coordinated)
    ;

    py::class_<cfg::moc::Transmission>(m, "Transmission")
        .def_readonly("name", &cfg::moc::Transmission::name)
        .def_property_readonly("type", 
            [](cfg::moc::Transmission &self) {
                return self.rotating_move ? "rotating" : "linear";
            }
        )
    ;

    py::class_<cfg::sys::MechanicalUnitGroup>(m, "MechanicalUnitGroup")
        .def_readonly("name", &cfg::sys::MechanicalUnitGroup::name)
        .def_readonly("robot", &cfg::sys::MechanicalUnitGroup::robot)
        .def_readonly("mechanical_units", &cfg::sys::MechanicalUnitGroup::mechanical_units)
    ;

    py::class_<cfg::sys::PresentOption>(m, "PresentOption")
        .def_readonly("name", &cfg::sys::PresentOption::name)
        .def_readonly("description", &cfg::sys::PresentOption::description)
    ;

}
} // end namespace rws
} // end namespace abb
