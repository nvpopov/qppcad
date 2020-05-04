#include <qppcad/ws_item/volume_view/python_volume_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view_tools.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_volume_view_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<ws_volume_record_t, std::shared_ptr<ws_volume_record_t> >
  py_ws_volume_record_t(module, "ws_volume_record_t");
  py_ws_volume_record_t
      .def_readonly("state_id", &ws_volume_record_t::m_state_id)
      .def_readonly("spin_subspace", &ws_volume_record_t::m_spin_subspace)

      .def_property("ready_to_render",
                    [](ws_volume_record_t &src)
                    {return src.m_ready_to_render;},
                    [](ws_volume_record_t &src, const bool value)
                    {src.m_ready_to_render = value; if(src.m_owner) src.m_owner->update_oi();})

      .def_property("need_to_regenerate",
                    [](ws_volume_record_t &src)
                    {return src.m_need_to_regenerate;},
                    [](ws_volume_record_t &src, const bool value)
                    {src.m_need_to_regenerate = value; if(src.m_owner) src.m_owner->update_oi();})

      .def_property("transparent_volume",
                    [](ws_volume_record_t &src)
                    {return src.m_transparent_volume.get_value();},
                    [](ws_volume_record_t &src, const bool value)
                    {src.m_transparent_volume.set_cvalue(value);
                    if(src.m_owner) src.m_owner->update_oi();})

      .def_property("render_permanent",
                    [](ws_volume_record_t &src)
                    {return src.m_render_permanent.get_value();},
                    [](ws_volume_record_t &src, const bool value)
                    {src.m_render_permanent.set_cvalue(value);
                    if(src.m_owner) src.m_owner->update_oi();})

      .def_property("color_pos",
                    [](ws_volume_record_t &src)
                    {return src.m_color_pos.get_value();},
                    [](ws_volume_record_t &src, const vector3<float> value)
                    {src.m_color_pos.set_cvalue(value); if(src.m_owner) src.m_owner->update_oi();})

      .def_property("color_neg",
                    [](ws_volume_record_t &src)
                    {return src.m_color_neg.get_value();},
                    [](ws_volume_record_t &src, const vector3<float> value)
                    {src.m_color_neg.set_cvalue(value); if(src.m_owner) src.m_owner->update_oi();})

      .def_property("color_vol",
                    [](ws_volume_record_t &src)
                    {return src.m_color_vol.get_value();},
                    [](ws_volume_record_t &src, const vector3<float> value)
                    {src.m_color_vol.set_cvalue(value); if(src.m_owner) src.m_owner->update_oi();});


  py::class_<volume_view_t, std::shared_ptr<volume_view_t> >
  py_volume_view_t(module, "volume_view_t", ws_item_base);
  py_volume_view_t
      .def("clone_volume", &volume_view_t::clone_volume)
      .def("__len__", [](volume_view_t &vv) {return vv.m_volumes.size();})
      .def("__getitem__", [](volume_view_t &vv, size_t i) {
                            if (i >= vv.m_volumes.size()) throw py::index_error();
                            return vv.m_volumes[i];
                          }, py::return_value_policy::reference_internal);

  auto vvt_module = module.def_submodule("vvt", "volume_view_t tools");
  vvt_module.def("sum_volumes", &volume_view_tools_t::sum_volumes,
                 py::arg("src"), py::arg("vol_idx"), py::arg("prefactor"),
                 py::arg("dst"), py::arg("dst_id") = std::nullopt)
            .def("cut_sph", &volume_view_tools_t::volume_cut_sph,
                 py::arg("volume_id"), py::arg("dst"), py::arg("sph_cnt"),
                 py::arg("sph_rad"), py::arg("cut_inner") = false)
            .def("cut_fnc", &volume_view_tools_t::volume_cut_fnc,
                 py::arg("dst"), py::arg("volume_id"), py::arg("cut_fnc"));

}
