#include <qppcad/ws_item/psg_view/python_psg_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_psg_view_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<psg_view_t, std::shared_ptr<psg_view_t> >
  py_point_sym_group_t(module, "psg_view_t", ws_item_base);
  py_point_sym_group_t.def_readwrite("ag", &psg_view_t::m_ag, py::return_value_policy::reference);
  py_point_sym_group_t.def_readwrite("pg_axes", &psg_view_t::m_pg_axes);
  py_point_sym_group_t.def("update_view", &psg_view_t::update_view);

}
