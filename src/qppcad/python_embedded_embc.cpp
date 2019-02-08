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
  m.def("dipole_moment", &embedded_cluster_tools::calc_dipole_moment);
  m.def("set_qm_r", &embedded_cluster_tools::set_qm_cluster_r_cur);
  m.def("move_qm_to_mm", &embedded_cluster_tools::move_sel_from_qm_to_cls_cur);
  m.def("gen_molcas_embc_sp", &embedded_cluster_tools::generate_molcas_embc_sp_input);
  m.def("gen_orca_embc_sp", &embedded_cluster_tools::generate_orca_embc_sp_input);
}
