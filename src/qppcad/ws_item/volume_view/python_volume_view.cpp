#include <qppcad/ws_item/volume_view/python_volume_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view_tools.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_volume_view_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<volume_view_t, std::shared_ptr<volume_view_t> >
  py_volume_view_t(module, "volume_view_t", ws_item_base);
  py_volume_view_t.def("clone_volume", &volume_view_t::clone_volume);

  auto vvt_module = module.def_submodule("vvt", "volume_view_t tools");
  vvt_module.def("sum_volumes", &volume_view_tools_t::sum_volumes,
                 py::arg("src1"), py::arg("vol1_idx"), py::arg("prefactor1"),
                 py::arg("src2"), py::arg("vol2_idx"), py::arg("prefactor2"),
                 py::arg("dst"), py::arg("dst_id") = std::nullopt)
            .def("cut_sph", &volume_view_tools_t::volume_cut_sph,
                 py::arg("volume_id"), py::arg("dst"), py::arg("sph_cnt"),
                 py::arg("sph_rad"), py::arg("cut_inner") = false)
            .def("cut_fnc", &volume_view_tools_t::volume_cut_fnc,
                 py::arg("dst"), py::arg("volume_id"), py::arg("cut_fnc"));

}
