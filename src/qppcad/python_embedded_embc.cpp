#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

//#include <qppcad/python_simple_query.hpp>
#include <qppcad/embedded_cluster_tools.hpp>

using namespace qpp;
using namespace qpp::cad;

PYBIND11_EMBEDDED_MODULE(embc, m) {

  py::module embq = m.def_submodule("embc", "Embedded cluster tools");
  embq.def("gen_sph", &embedded_cluster_tools::gen_spherical_cluster);
    //embq.def("ws",  &simple_query::select_ws);

}
