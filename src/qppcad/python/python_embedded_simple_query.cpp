#include <qppcad/python/python_simple_query.hpp>
#include <qppcad/ws_item/geom_view/geom_view_colorizer.hpp>
#include <qppcad/ui/color_map.hpp>
#include <pybind11/functional.h>

using namespace qpp;
using namespace qpp::cad;

PYBIND11_EMBEDDED_MODULE(sq, m) {

  py::enum_<spatial_units_e>(m, "spatial_units", py::arithmetic(), "Spatial(Ang or Bohr)")
          .value("ang", spatial_units_ang, "Docstring for spatial_units_ang")
          .value("bohr", spatial_units_bohr, "Docstring for spatial_units_bohr")
          .export_values();

  m.attr("def_isovalue_dens") = py::float_(qpp::def_isovalue_dens);
  m.attr("def_isovalue_mo") = py::float_(qpp::def_isovalue_mo);

  m.def("get_wsm", &simple_query::get_ws_mgr, py::return_value_policy::reference);

  m.def("set_font_size", &simple_query::set_font_size,
    R"str(
    Sets the font size in the console. The value is saved between sessions.
    Parameters
    ----------
    arg0: int - new font size in pt
    )str");

  m.def("get_build_info", &simple_query::get_build_info);
  m.def("qopen", &simple_query::open_file,
        py::arg("file_name"), py::arg("to_current") = false);
  m.def("qopen", &simple_query::open_file_query,
        py::arg("file_name"), py::arg("file_format") = "", py::arg("to_current") = false);
  m.def("mode", &simple_query::edit_mode);

  m.def("bg", &simple_query::set_ws_bg,
    R"str(
    Sets the background color for the current workspace.
    Parameters
    ----------
    arg0: pq.vector3f - rgb color as 3d vector
    )str");

  m.def("bg", [](float r, float g, float b) {simple_query::set_ws_bg(vector3<float>(r,g,b));},
    R"str(
    Sets the background color for the current workspace.
    Parameters
    ----------
    arg0: float - red component
    arg1: float - green component
    arg2: float - blue component
    )str" );

  m.def("gp", &simple_query::gizmo_pos,
        R"str(
        Returns the current position of the gizmo-object
        )str" );

  m.def("cs", &simple_query::get_current_selected,
        R"str(
        Returns the current selected ws_item
        )str");

  m.def("xgeom_dfn", &simple_query::get_xgeom_dfn);
  m.def("xgeom_dft", &simple_query::get_xgeom_dft);
  m.def("cw", &simple_query::cur_ws);

  //****************************** tools module begin ******************************
  py::module tools = m.def_submodule("tools", "Generic tools");

  tools.def("to_clipboard", &simple_query::to_clipboard);
  tools.def("set_msr_digits", &simple_query::set_msr_digits);
  tools.def("get_tool_groups", &simple_query::get_tool_groups);
  tools.def("get_tool_names", &simple_query::get_tool_names);
  tools.def("get_connected_items", &simple_query::get_connected_items);
  tools.def("get_followers", &simple_query::get_followers);
  tools.def("get_leader", &simple_query::get_leader);
  tools.def("exec_tool", &simple_query::exec_tool);

  py::class_<color_map_t, std::shared_ptr<color_map_t>> py_color_map_t(tools, "color_map_t");
  py_color_map_t.def(py::init<>());
  py_color_map_t.def("get_color", &color_map_t::get_color);
  py_color_map_t.def("push_color", &color_map_t::push_color);

  tools.def("rebond", &simple_query::rebond);
  tools.def("t", &simple_query::translate_selected);
  tools.def("sc", &simple_query::make_super_cell);
  tools.def("set_chg", &simple_query::set_charge);
  tools.def("set_sel_color", &simple_query::set_sel_color);
  tools.def("set_sel_color", &simple_query::set_sel_color_vec);

  tools.def("color_by_dist", &geom_view_colorizer_helper::py_colorize_by_distance);
  tools.def("color_by_dist_pairs",
            &geom_view_colorizer_helper::py_colorize_by_distance_with_pairs);
  tools.def("get_type_name", &simple_query::get_type_name);
  tools.def("get_type_hash", &simple_query::get_type_hash);
  tools.def("is_instance_of", &simple_query::is_instance_of_by_hash);
  tools.def("is_instance_of", &simple_query::is_instance_of_by_type_name);

  tools.def("get_point_sym_g", &simple_query::get_point_sym_group, py::arg("tolerance") = 0.1f);
  tools.def("make_point_sym_g", &simple_query::make_psg_view, py::arg("tolerance") = 0.1f);

  tools.def("make_traj_highlighter", &simple_query::make_traj_highlight,
            py::arg("atom_id"), py::arg("anim_id") = 1);
  tools.def("compose_gb_from_images", &simple_query::compose_gv_from_images);

  tools.def("make_cube", &simple_query::make_cube_p);
  tools.def("embed_cube", &simple_query::embed_cube);
  tools.def("make_arrow", &simple_query::make_arrow_p,
            py::arg("from"), py::arg("to"), py::arg("name")="");

  tools.def("convert_spatial", &simple_query::convert_selected_units);
  //******************************** tools module end *******************************

  py::module sel = m.def_submodule("sel", "Selection routines");

  sel.def("ws",  &simple_query::select_ws,
          R"str(
          Selects nth workspace
          )str" );

  sel.def("it",  &simple_query::select_itm,
          R"str(
          Selects nth object from the current workspace
          :param arg0: an workspace index that needs to be selected
          )str");

  sel.def("c", &simple_query::sel_cnt,
          R"str(
          Only for geom_view_t. Selects an atom under a certain index.
          :param arg0: int an atom index that needs to be selected
          )str");

  sel.def("c", &simple_query::sel_cnt_list,
          R"str(
          Only for geom_view_t. Selects several atoms.
          :param arg0: list the list of atoms to be selected
          )str");

  sel.def("c", &simple_query::sel_cnt_type,
          R"str(
          Only for geom_view_t. Selects several atoms by type.
          :param arg0: str the type of atoms that need to be selected
          )str");

  sel.def("c", &simple_query::sel_cnt_all,
          R"str(
          Only for geom_view_t. Selects all atoms.
          )str");

  sel.def("c_p", &simple_query::sel_cnt_parity,
          R"str(
          Adds to the currently selected atoms those that are additional to them by
          inversion symmetry relative to the origin of coordinates
          )str");

  sel.def("uc", &simple_query::unsel_cnt_all,
          R"str(
          Only for geom_view_t. Unselects all atoms.
          )str");

  sel.def("uc", &simple_query::unsel_cnt,
          R"str(
          Only for geom_view_t. Unselects an atom under a certain index.
          :param arg0: int an atom index that needs to be unselected
          )str");

  sel.def("uc", &simple_query::unsel_cnt_list,
          R"str(
          Only for geom_view_t. Unselects several atoms.
          :param arg0: list the list of atoms to be unselected
          )str");

  sel.def("uc", &simple_query::unsel_cnt_type,
          R"str(
          Only for geom_view_t. Unselects several atoms by type.
          :param arg0: str the type of atoms that need to be unselected
          )str");

  sel.def("get", &simple_query::get_sel,
          R"str(
          Get a list of selected atoms.
          )str");

  sel.def("sph", &simple_query::sel_cnt_sphere);
  sel.def("gsp", &simple_query::sel_cnt_gsph);
  sel.def("inv", &simple_query::sel_invert);
  sel.def("hemisph", &simple_query::sel_hemisphere);
  sel.def("f", &simple_query::sel_cnt_fn);

  sel.def("gx",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return x > bound;});});
  sel.def("lx",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return x < bound;});});
  sel.def("gy",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return y > bound;});});
  sel.def("ly",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return y < bound;});});
  sel.def("gz",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return z > bound;});});
  sel.def("lz",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return z < bound;});});

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
  cam.def("zoom", &simple_query::camera_get_zoom);
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
