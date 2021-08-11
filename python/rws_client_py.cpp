#include <pybind11/pybind11.h>

#include <abb_librws/rws_client.h>

namespace py = pybind11;

namespace abb {
namespace rws {

void init_client(py::module &m){

    py::enum_<Coordinate>(m, "Coordinate")
        .value("BASE", Coordinate::BASE)
        .value("WORLD", Coordinate::WORLD)
        .value("TOOL", Coordinate::TOOL)
        .value("WOBJ", Coordinate::WOBJ)
        .value("ACTIVE", Coordinate::ACTIVE)
        .export_values()
    ;
}
} // end namespace rws
} // end namespace abb