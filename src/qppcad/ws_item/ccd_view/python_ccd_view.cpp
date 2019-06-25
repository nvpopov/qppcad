#include <qppcad/ws_item/ccd_view/python_ccd_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_ccd_view_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::enum_<comp_chem_program_e>(module, "comp_chem_program_e", py::arithmetic(), "")
          .value("pr_unknown", pr_unknown, "pr_unknown")
          .value("pr_vasp", pr_vasp, "pr_vasp")
          .value("pr_firefly", pr_firefly, "pr_firefly")
          .value("pr_pc_gamess", pr_pc_gamess, "pr_pc_gamess")
          .value("pr_cp2k", pr_cp2k, "pr_cp2k")
          .value("pr_orca", pr_orca, "pr_orca")
          .value("pr_molcas", pr_molcas, "pr_molcas")
          .value("pr_molden", pr_molden, "pr_molden")
          .export_values();

  py::class_<comp_chem_program_data_t<float>, std::shared_ptr<comp_chem_program_data_t<float> > >
  py_comp_chem_program_data_t(module, "comp_chem_program_data_t");

  py::class_<ccd_view_t, std::shared_ptr<ccd_view_t> >
  py_ccd_view_t(module, "ccd_view_t", ws_item_base);
  py_ccd_view_t.def_readonly("ccd", &ccd_view_t::m_ccd);

}
