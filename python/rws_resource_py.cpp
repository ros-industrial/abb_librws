#include <pybind11/pybind11.h>

#include <abb_librws/rws_resource.h>

using namespace pybind11::literals;
namespace py = pybind11;

namespace abb {
namespace rws {

void init_resource(py::module &m) {

    py::class_<FileResource>(m, "FileResource")
        .def(py::init<const std::string &, const std::string &>(), "filename"_a, "directory"_a = SystemConstants::RWS::Identifiers::HOME_DIRECTORY)
        .def_readwrite("filename", &FileResource::filename)
        .def_readwrite("directory", &FileResource::directory)
    ;

}
} // end namespace rws
} // end namespace abb
