#include <qppcad/python/python_simple_query.hpp>
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

  m.def("get_wsm", &simple_query::get_ws_mgr, py::return_value_policy::reference)

   .def("set_font_size", &simple_query::set_font_size,
    R"str(
    Sets the font size in the console. The value is saved between sessions.
    Parameters
    ----------
    arg0: int - new font size in pt
    )str")

   .def("get_build_info", &simple_query::get_build_info)
   .def("qopen", &simple_query::open_file,
        py::arg("file_name"), py::arg("to_current") = false)
   .def("qopen", &simple_query::open_file_query,
        py::arg("file_name"), py::arg("file_format") = "", py::arg("to_current") = false)
   .def("mode", &simple_query::edit_mode)
   .def("mode_itm", [](){simple_query::edit_mode(0);})
   .def("mode_cnt", [](){simple_query::edit_mode(1);})
   .def("quit", &simple_query::quit)

   .def("bg", &simple_query::set_ws_bg,
    R"str(
    Sets the background color for the current workspace.
    Parameters
    ----------
    arg0: pq.vector3f - rgb color as 3d vector
    )str")

   .def("bg", [](float r, float g, float b) {simple_query::set_ws_bg(vector3<float>(r,g,b));},
    R"str(
    Sets the background color for the current workspace.
    Parameters
    ----------
    arg0: float - red component
    arg1: float - green component
    arg2: float - blue component
    )str" )

   .def("gp", &simple_query::gizmo_pos,
        R"str(
        Returns the current position of the gizmo-object
        )str" )

   .def("cs", &simple_query::get_current_selected,
        R"str(
        Returns the current selected ws_item
        )str")

   .def("xgeom_dfn", &simple_query::get_xgeom_dfn)
   .def("xgeom_dft", &simple_query::get_xgeom_dft)
   .def("cw", &simple_query::cur_ws)
   .def("labels", &simple_query::py_labels)
   .def("rebond", &simple_query::rebond)
   .def("t", &simple_query::translate_selected)
   .def("t", &simple_query::translate_selected_v3)
   .def("sc", &simple_query::make_super_cell)
   .def("set_chg", &simple_query::set_charge)
   .def("set_sel_color", &simple_query::set_sel_color)
   .def("set_sel_color", &simple_query::set_sel_color_vec);

  //****************************** tools module begin ******************************
  py::module tools = m.def_submodule("tools", "Generic tools");

  py::class_<color_map_t, std::shared_ptr<color_map_t>> py_color_map_t(tools, "color_map_t");
  py_color_map_t.def(py::init<>())
                .def("get_color", &color_map_t::get_color)
                .def("push_color", &color_map_t::push_color);

  tools.def("to_clipboard", &simple_query::to_clipboard)
       .def("set_msr_digits", &simple_query::set_msr_digits)
       .def("get_tool_groups", &simple_query::get_tool_groups)
       .def("get_tool_names", &simple_query::get_tool_names)
       .def("get_connected_items", &simple_query::get_connected_items)
       .def("get_followers", &simple_query::get_followers)
       .def("get_leader", &simple_query::get_leader)
       .def("exec_tool", &simple_query::exec_tool)

       .def("get_type_name", &simple_query::get_type_name)
       .def("get_type_hash", &simple_query::get_type_hash)
       .def("is_instance_of", &simple_query::is_instance_of_by_hash)
       .def("is_instance_of", &simple_query::is_instance_of_by_type_name)

       .def("get_point_sym_g", &simple_query::get_point_sym_group, py::arg("tolerance") = 0.1f)
       .def("make_point_sym_g", &simple_query::make_psg_view, py::arg("tolerance") = 0.1f)

       .def("make_traj_highlighter", &simple_query::make_traj_highlight,
            py::arg("atom_id"), py::arg("anim_id") = 1)

       .def("make_cube", &simple_query::make_cube_p)
       .def("embed_cube", &simple_query::embed_cube)
       .def("make_arrow", &simple_query::make_arrow_p,
            py::arg("from"), py::arg("to"), py::arg("name")="")

       .def("convert_spatial", &simple_query::convert_selected_units);
  //******************************** tools module end *******************************

  py::module sel = m.def_submodule("sel", "Selection routines");

  sel.def("ws",  &simple_query::select_ws,
          R"str(
          Selects nth workspace
          )str" )

     .def("it",  &simple_query::select_itm,
          R"str(
          Selects nth object from the current workspace
          :param arg0: an workspace index that needs to be selected
          )str")

     .def("c", &simple_query::sel_cnt,
          R"str(
          Only for geom_view_t. Selects an atom under a certain index.
          :param arg0: int an atom index that needs to be selected
          )str")

     .def("c", &simple_query::sel_cnt_list,
          R"str(
          Only for geom_view_t. Selects several atoms.
          :param arg0: list the list of atoms to be selected
          )str")

     .def("c", &simple_query::sel_cnt_type,
          R"str(
          Only for geom_view_t. Selects several atoms by type.
          :param arg0: str the type of atoms that need to be selected
          )str")

     .def("c", &simple_query::sel_cnt_all,
          R"str(
          Only for geom_view_t. Selects all atoms.
          )str")

     .def("c_p", &simple_query::sel_cnt_parity,
          R"str(
          Adds to the currently selected atoms those that are additional to them by
          inversion symmetry relative to the origin of coordinates
          )str")

     .def("uc", &simple_query::unsel_cnt_all,
          R"str(
          Only for geom_view_t. Unselects all atoms.
          )str")

     .def("uc", &simple_query::unsel_cnt,
          R"str(
          Only for geom_view_t. Unselects an atom under a certain index.
          :param arg0: int an atom index that needs to be unselected
          )str")

     .def("uc", &simple_query::unsel_cnt_list,
          R"str(
          Only for geom_view_t. Unselects several atoms.
          :param arg0: list the list of atoms to be unselected
          )str")

     .def("uc", &simple_query::unsel_cnt_type,
          R"str(
          Only for geom_view_t. Unselects several atoms by type.
          :param arg0: str the type of atoms that need to be unselected
          )str")

     .def("get", &simple_query::get_sel,
          R"str(
          Get a list of selected atoms.
          )str")

      .def("vis", &simple_query::sel_vis,
           R"str(
           Select visible atoms
           )str")

     .def("sph", &simple_query::sel_cnt_sphere)
     .def("gsp", &simple_query::sel_cnt_gsph)
     .def("inv", &simple_query::sel_invert)
     .def("hemisph", &simple_query::sel_hemisphere)
     .def("f", &simple_query::sel_cnt_fn)

     .def("gx",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return x > bound;});})
     .def("lx",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return x < bound;});})
     .def("gy",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return y > bound;});})
     .def("ly",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return y < bound;});})
     .def("gz",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return z > bound;});})
     .def("lz",
          [](float bound){simple_query::sel_cnt_fn(
            [&bound](float x, float y, float z){return z < bound;});});

  py::module pt = m.def_submodule("pt", "Periodic table manipulations");

  pt.def("c", &simple_query::ptable_set_color_by_number,
         py::arg("num"), py::arg("r"), py::arg("g"), py::arg("b"),
         R"str(
         Set color(r, g, b) for element with number <num> as 3 floats.
         )str")

    .def("c", &simple_query::ptable_set_color_by_number_as_vector,
         py::arg("num"), py::arg("color"),
         R"str(
         Set color(r, g, b) for element with number <num> as vector.
         )str")

    .def("c", &simple_query::ptable_set_color_by_name,
         py::arg("name"), py::arg("r"), py::arg("g"), py::arg("b"),
         R"str(
         Set color(r, g, b) for element with name as 3 floats.
         )str")

    .def("c", &simple_query::ptable_set_color_by_name_as_vector,
         py::arg("name"), py::arg("color"),
         R"str(
         Set color(r, g, b) for element with name as vector.
         )str")

    .def("c", &simple_query::ptable_get_color_by_number, py::arg("number"),
         R"str(
         Get color(r, g, b) for element with number as vector.
         )str")
    .def("c", &simple_query::ptable_get_color_by_name, py::arg("name"),
         R"str(
         Get color(r, g, b) for element with name as vector.
         )str")

    .def("r", &simple_query::ptable_set_r_by_name, py::arg("name"), py::arg("r"))
    .def("r", &simple_query::ptable_set_r_by_number, py::arg("number"), py::arg("r"))

    .def("r", &simple_query::ptable_get_r_by_name, py::arg("name"))
    .def("r", &simple_query::ptable_get_r_by_number, py::arg("number"))

    .def("cr", &simple_query::ptable_set_cov_r_by_name, py::arg("name"), py::arg("cov_r"))
    .def("cr", &simple_query::ptable_set_cov_r_by_number, py::arg("number"), py::arg("cov_r"))

    .def("cr", &simple_query::ptable_get_cov_r_by_name, py::arg("name"))
    .def("cr", &simple_query::ptable_get_cov_r_by_number, py::arg("number"));

  py::module cam = m.def_submodule("cam", "Camera manipulation");
  cam.def("t",   &simple_query::camera_move)
     .def("ry",  &simple_query::camera_rotate_yaw)
     .def("rp",  &simple_query::camera_rotate_pitch)
     .def("zoom", &simple_query::camera_zoom)
     .def("zoom", &simple_query::camera_get_zoom)
     .def("proj", &simple_query::camera_mode)
     .def("copy", &simple_query::copy_camera_from_ws);

}
