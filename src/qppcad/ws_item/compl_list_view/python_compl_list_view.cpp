#include <qppcad/ws_item/compl_list_view/python_compl_list_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_compl_list_view_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<compl_list_view_t, std::shared_ptr<compl_list_view_t> >
  py_compl_list_view_t(module, "compl_list_view_t", ws_item_base);
  py_compl_list_view_t.def_readonly("cll", &compl_list_view_t::m_compl_list);

}
