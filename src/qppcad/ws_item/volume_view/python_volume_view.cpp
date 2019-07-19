#include <qppcad/ws_item/volume_view/python_volume_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_volume_view_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<volume_view_t, std::shared_ptr<volume_view_t> >
  py_volume_view_t(module, "volume_view_t", ws_item_base);
  py_volume_view_t.def("cut_sph", &volume_view_t::volume_cut_sph);

}
