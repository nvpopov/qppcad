#include <qppcad/ws_item/ws_vector3/python_ws_vector3.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_ws_vector3_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<ws_vector3_t, std::shared_ptr<ws_vector3_t> >
  py_ws_vector3_t(module, "ws_vector3_t", ws_item_base);

}
