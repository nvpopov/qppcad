#include <qppcad/ws_item/ccd_view/python_ccd_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_ccd_view_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<ccd_view_t, std::shared_ptr<ccd_view_t> >
  py_ccd_view_t(module, "ccd_view_t", ws_item_base);
  py_ccd_view_t.def_readonly("ccd", &ccd_view_t::m_ccd);
  py_ccd_view_t.def("query_vib_by_atoms", &ccd_view_t::py_query_vib_by_atoms);

}
