#include <qppcad/ws_item/pdos_view/python_pdos_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_pdos_view_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<pdos_view_t, std::shared_ptr<pdos_view_t> >
  py_pdos_view_t(module, "pdos_view_t", ws_item_base);
  py_pdos_view_t.def("load", &pdos_view_t::py_load_from_list);
  py_pdos_view_t.def("load_dir", &pdos_view_t::py_load_from_dir);
  py_pdos_view_t.def("rebuild", &pdos_view_t::rebuild_plots);
  py_pdos_view_t.def("scale_chn", &pdos_view_t::scale_channel);
  py_pdos_view_t.def("e_fermi", &pdos_view_t::py_get_efermi_for_channel);

}
