#include <qppcad/ws_item/ccd_view/python_ccd_view.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view_tools.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_ccd_view_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<ccd_view_t, std::shared_ptr<ccd_view_t> >
  py_ccd_view_t(module, "ccd_view_t", ws_item_base);
  py_ccd_view_t.def_readonly("ccd", &ccd_view_t::m_ccd);
  py_ccd_view_t.def("query_vib_by_atoms", &ccd_view_t::py_query_vib_by_atoms);

  auto cvt_module = module.def_submodule("cvt", "ccd_view_t tools");
  cvt_module.def("get_states_for_tddft_root", &ccd_view_tools_t::get_states_for_tddft_root,
                 py::arg("ccd"), py::arg("root_id"), py::arg("min_amplitude") = 0.05)
            .def("get_amplitudes_for_tddft_root", &ccd_view_tools_t::get_amplitudes_for_tddft_root,
                 py::arg("ccd"), py::arg("root_id"), py::arg("states"));

}
