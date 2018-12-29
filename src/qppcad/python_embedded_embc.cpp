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

  m.def("gen_sph", &embedded_cluster_tools::gen_spherical_cluster_cur);
  m.def("gen_sph_qm", &embedded_cluster_tools::gen_spherical_cluster_cur_qm);
    //embq.def("ws",  &simple_query::select_ws);

}
