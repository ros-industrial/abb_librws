#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <abb_librws/rws_common.h>

namespace py = pybind11;
using namespace pybind11::literals;

namespace abb {
namespace rws {

void init_common(py::module &m){


    py::enum_<TriBool::Values>(m, "Values")
        .value("UNKNOWN", TriBool::Values::UNKNOWN_VALUE)
        .value("TRUE", TriBool::Values::TRUE_VALUE)
        .value("FALSE", TriBool::Values::FALSE_VALUE)
        .export_values()
    ;

    py::class_<TriBool>(m, "TriBool")
        .def(py::init<>())
        .def(py::init<const TriBool::Values>(), "value"_a)
        .def(py::init<const bool>(), "value"_a)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("is_unknown", &TriBool::isUnknown)
        .def("is_true", &TriBool::isTrue)
        .def("is_false", &TriBool::isFalse)
        .def("__repr__", &TriBool::toString)
    ;
}
} // end namespace rws
} // end namespace abb
