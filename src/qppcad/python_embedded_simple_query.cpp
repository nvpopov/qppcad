#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/python_simple_query.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list_colorizer.hpp>

using namespace qpp;
using namespace qpp::cad;

PYBIND11_EMBEDDED_MODULE(sq, m) {

  py::enum_<spatial_units>(m, "spatial_units", py::arithmetic(), "Spatial(Ang or Bohr)")
          .value("ang", spatial_units_ang, "Docstring for spatial_units_ang")
          .value("bohr", spatial_units_bohr, "Docstring for spatial_units_bohr")
          .export_values();

  m.attr("def_isovalue_dens") = py::float_(qpp::def_isovalue_dens);
  m.attr("def_isovalue_mo") = py::float_(qpp::def_isovalue_mo);

  m.def("get_wsm", &simple_query::get_ws_mgr, py::return_value_policy::reference);

  m.def("qopen", &simple_query::open_file,
        py::arg("file_name"), py::arg("to_current") = false);
  m.def("qopen", &simple_query::open_file_query,
        py::arg("file_name"), py::arg("file_format") = "", py::arg("to_current") = false);

  m.def("mode", &simple_query::edit_mode);

  m.def("bg", &simple_query::set_ws_bg);
  m.def("bg", [](float r, float g, float b) {simple_query::set_ws_bg(vector3<float>(r,g,b));} );
  m.def("gpos", &simple_query::gizmo_pos);

  py::module tools = m.def_submodule("tools", "Generic tools");
  tools.def("get_isolevel", &simple_query::get_isolevel);
  tools.def("set_isolevel", &simple_query::set_isolevel);

  tools.def("add_al_0d", &simple_query::add_atoms_list_0d);
  tools.def("add_al_3d", &simple_query::add_atoms_list_3d);
  tools.def("rebond", &simple_query::rebond);
  tools.def("t", &simple_query::translate_selected);
  tools.def("sc", &simple_query::make_super_cell);
  tools.def("set_chg", &simple_query::set_charge);
  tools.def("set_sel_color", &simple_query::set_sel_color);
  tools.def("set_sel_color", &simple_query::set_sel_color_vec);

  tools.def("color_by_dist", &ws_atoms_list_colorizer_helper::py_colorize_by_distance);
  tools.def("color_by_dist_pairs",
            &ws_atoms_list_colorizer_helper::py_colorize_by_distance_with_pairs);
  tools.def("get_type_name", &simple_query::get_type_name);
  tools.def("get_type_hash", &simple_query::get_type_hash);
  tools.def("is_instance_of", &simple_query::is_instance_of_by_hash);
  tools.def("is_instance_of", &simple_query::is_instance_of_by_type_name);

  tools.def("get_point_sym_g", &simple_query::get_point_sym_group, py::arg("tolerance") = 0.1f);
  tools.def("make_point_sym_g", &simple_query::make_ws_point_sym_group,
            py::arg("tolerance") = 0.1f);

  tools.def("make_traj_highlighter", &simple_query::make_traj_highlight,  py::arg("atom_id"),
            py::arg("anim_id") = 1);

  tools.def("make_cube", &simple_query::make_cube_p);
  tools.def("make_arrow", &simple_query::make_arrow_p);

  tools.def("convert_spatial", &simple_query::convert_selected_units);

  py::module sel = m.def_submodule("sel", "Selection routines");

  sel.def("ws",  &simple_query::select_ws);
  sel.def("it",  &simple_query::select_itm);
  sel.def("c", &simple_query::sel_cnt);
  sel.def("c", &simple_query::sel_cnt_list);
  sel.def("c", &simple_query::sel_cnt_type);
  sel.def("c", &simple_query::sel_cnt_all);
  sel.def("c_p", &simple_query::sel_cnt_parity);
  sel.def("uc", &simple_query::unsel_cnt_all);
  sel.def("uc", &simple_query::unsel_cnt);
  sel.def("uc", &simple_query::unsel_cnt_list);
  sel.def("uc", &simple_query::unsel_cnt_type);
  sel.def("get", &simple_query::get_sel);
  sel.def("sph", &simple_query::sel_cnt_sphere);
  sel.def("inv", &simple_query::sel_invert);
  sel.def("hemisph", &simple_query::sel_hemisphere);

  py::module pt = m.def_submodule("pt", "Periodic table manipulations");
  pt.def("c", &simple_query::ptable_set_color_by_number);
  pt.def("c", &simple_query::ptable_set_color_by_name);
  pt.def("r", &simple_query::ptable_set_radius_by_name);
  pt.def("r", &simple_query::ptable_set_radius_by_number);
  pt.def("c", &simple_query::ptable_get_color_by_number);
  pt.def("c", &simple_query::ptable_get_color_by_name);
  pt.def("r", &simple_query::ptable_get_radius_by_name);
  pt.def("r", &simple_query::ptable_get_radius_by_number);

  py::module cam = m.def_submodule("cam", "Camera manipulation");
  cam.def("t", &simple_query::camera_move);
  cam.def("ry", &simple_query::camera_rotate_yaw);
  cam.def("rp", &simple_query::camera_rotate_pitch);
  cam.def("zoom", &simple_query::camera_zoom);
  cam.def("proj", &simple_query::camera_mode);
  cam.def("copy", &simple_query::copy_camera_from_ws);

  py::module sv = m.def_submodule("sv", "Selective visibility manipulation");
  sv.def("state", &simple_query::sv_get);
  sv.def("set", &simple_query::sv_edit);
  sv.def("set", &simple_query::sv_edit_list);
  sv.def("set", &simple_query::sv_edit_all);

  py::module cl = m.def_submodule("cl", "Custom labels");
  cl.def("state", &simple_query::cl_get);
  cl.def("set_st", &simple_query::set_cl_state);
  cl.def("set_t", &simple_query::set_cl_text);
  //cam.def("tu", )
}
