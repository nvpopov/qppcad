#include <qppcad/ws_item/geom_view/python_geom_view.hpp>
#include <qppcad/ws_item/geom_view/displ_proj.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools_colorize.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_geom_view_reg_helper_t::reg(
    py::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  /* geom_view_render_style_e pyexport */
  py::enum_<geom_view_render_style_e>(module, "geom_view_render_style_t", py::arithmetic(), "")
          .value("ball_and_stick", ball_and_stick, "ball_and_stick")
          .value("dynamic_lines", dynamic_lines, "dynamic_lines")
          .value("xatom_lines", xatom_lines, "xatom_lines")
          .value("billboards", billboards, "billboards")
          .value("buffered_billboards", buffered_billboards, "buffered_billboards")
          .export_values();

  /* geom_labels_style_e pyexport */
  py::enum_<geom_labels_style_e>(module, "geom_view_labels_style_t", py::arithmetic(), "")
          .value("show_none", show_none, "show_none")
          .value("show_id", show_id, "show_id")
          .value("show_type", show_type, "show_type")
          .value("show_id_type", show_id_type, "show_id_type")
          .value("show_charge", show_charge, "show_charge")
          .value("show_custom", show_custom, "show_custom")
          .export_values();

  /* geom_anim_t pyexport */
  py::enum_<geom_anim_t>(module, "geom_anim_t", py::arithmetic(), "")
          .value("anim_static", anim_static, "anim_static")
          .value("anim_generic", anim_generic, "anim_generic")
          .value("anim_geo_opt", anim_geo_opt, "anim_geo_opt")
          .value("anim_md", anim_md, "anim_md")
          .value("anim_vib", anim_vib, "anim_vib")
          .export_values();

  /* py_geom_view_anim pyexport */
  py::class_<geom_view_anim_subsys_t, std::shared_ptr<geom_view_anim_subsys_t> >
  py_gv_anim_t(module, "geom_view_anim_subsys_t");
  py_gv_anim_t.def("nanim", &geom_view_anim_subsys_t::get_total_anims)
              .def("animable", &geom_view_anim_subsys_t::animable)
              .def("next_anim", &geom_view_anim_subsys_t::next_anim)
              .def("prev_anim", &geom_view_anim_subsys_t::prev_anim)
              .def("to_begin", &geom_view_anim_subsys_t::update_current_frame_to_begin)
              .def("to_end", &geom_view_anim_subsys_t::update_current_frame_to_end)
              .def("to_frame", &geom_view_anim_subsys_t::update_current_frame_to)
              .def("make_animable", &geom_view_anim_subsys_t::make_animable)
              .def("make_anim", &geom_view_anim_subsys_t::make_anim)
              .def("get_anim_type", &geom_view_anim_subsys_t::get_anim_type_by_idx)
              .def("get_cur_anim_type", &geom_view_anim_subsys_t::get_cur_anim_type)
              .def("make_interpolated_anim", &geom_view_anim_subsys_t::make_interpolated_anim)
              .def("commit_apos", &geom_view_anim_subsys_t::commit_atom_pos, py::arg("atom_id"),
                   py::arg("lerp_from_start") = false, py::arg("propagate_to_the_end") = false)
              .def("is_cell_animable", &geom_view_anim_subsys_t::is_cell_animable)
              .def("get_cell_vectors", &geom_view_anim_subsys_t::get_cell_vectors)
              .def("get_atom_pos", &geom_view_anim_subsys_t::get_atom_pos);

  /* geom_view_labels_subsys_t pyexport */
  py::class_<geom_view_labels_subsys_t, std::shared_ptr<geom_view_labels_subsys_t> >
  py_gv_lbl_t(module, "geom_view_labels_subsys_t");
  py_gv_lbl_t.def_property("inplace_hud",
                           [](geom_view_labels_subsys_t &src)
                           {return src.m_render_inplace_hud;},
                           [](geom_view_labels_subsys_t &src, const bool value)
                           {src.m_render_inplace_hud = value;
                            src.p_owner->update_oi();
                            app_state_t::get_inst()->make_viewport_dirty();})
             .def_property("sel_vis",
                           [](geom_view_labels_subsys_t &src)
                           {return src.m_selective_lbl;},
                           [](geom_view_labels_subsys_t &src, const bool value)
                           {src.m_selective_lbl = value;
                            src.p_owner->update_oi();
                            app_state_t::get_inst()->make_viewport_dirty();})
             .def_property("screen_scale",
                           [](geom_view_labels_subsys_t &src)
                           {return src.m_screen_scale;},
                           [](geom_view_labels_subsys_t &src, const bool value)
                           {src.m_screen_scale = value;
                            src.p_owner->update_oi();
                            app_state_t::get_inst()->make_viewport_dirty();})
             .def_property("render_outlines",
                           [](geom_view_labels_subsys_t &src)
                           {return src.m_render_outlines;},
                           [](geom_view_labels_subsys_t &src, const bool value)
                           {src.m_render_outlines = value;
                            src.p_owner->update_oi();
                            app_state_t::get_inst()->make_viewport_dirty();})
             .def_property("style",
                           [](geom_view_labels_subsys_t &src)
                           {return src.m_style;},
                           [](geom_view_labels_subsys_t &src,
                           const geom_labels_style_e value)
                           {src.m_style = value;
                            src.p_owner->update_oi();
                            app_state_t::get_inst()->make_viewport_dirty();})
             .def_property("font_size",
                           [](geom_view_labels_subsys_t &src)
                           {return src.m_lbl_font_size;},
                           [](geom_view_labels_subsys_t &src,
                           const int value)
                           {src.m_lbl_font_size = value; src.p_owner->update_oi();
                            app_state_t::get_inst()->make_viewport_dirty();})
             .def("lbl_by_ngb_cnt", &geom_view_labels_subsys_t::labelize_sel_by_neighbours_count)
             .def("lbl_by_geom_fctr", &geom_view_labels_subsys_t::labelize_sel_by_dist_factor);

  /* atom_index_set_key pyexport */
  py::class_<atom_index_set_key, std::shared_ptr<atom_index_set_key> >
  py_atom_index_set_key(module, "atom_index_set_key_t");
  py_atom_index_set_key.def_readwrite("atm", &atom_index_set_key::m_atm)
                       .def_readwrite("idx", &atom_index_set_key::m_idx)
                       .def(py::init<const int, const qpp::index>())
                       .def("__str__",
                            [](atom_index_set_key &src) {
                              return fmt::format("atm={}, idx={}", src.m_atm, src.m_idx);
                            });

  /*  geom_view_sel_group_t pyexport */
  py::class_<geom_view_sel_group_t, std::shared_ptr<geom_view_sel_group_t> >
  py_gv_sg_t(module, "geom_view_sel_group_t");
  py_gv_sg_t.def_readwrite("name", &geom_view_sel_group_t::m_name)
            .def(py::init<std::string&>())
            .def("append", &geom_view_sel_group_t::append)
            .def("remove", &geom_view_sel_group_t::remove)
            .def("__len__", [](geom_view_sel_group_t &selg) {return selg.m_data.size();})
            .def("__getitem__", [](geom_view_sel_group_t &selg, size_t i) {
                                  if (i >= selg.m_data.size()) throw py::index_error();
                                  return selg.m_data[i];
                                }, py::return_value_policy::reference_internal);

  /*  geom_view_sel_groups_subsys_t pyexport */
  py::class_<geom_view_sel_groups_subsys_t, std::shared_ptr<geom_view_sel_groups_subsys_t> >
  py_gv_sg_subsys_t(module, "geom_view_sel_groups_subsys_t");
  py_gv_sg_subsys_t.def("make_from_selected", &geom_view_sel_groups_subsys_t::make_from_selected)
                   .def("__len__", [](geom_view_sel_groups_subsys_t &sels) {
                                     return sels.m_sel_grps.size();
                                   });

  /* geom_view_t pyexport */
  py::class_<geom_view_t, std::shared_ptr<geom_view_t> >
  py_gv_t(module, "geom_view_t", ws_item_base);
  py_gv_t.def_readwrite("geom", &geom_view_t::m_geom)
         .def_property("atom_scale",
                       [](geom_view_t &src)
                       {return src.m_atom_scale_factor;},
                       [](geom_view_t &src, const float value)
                       {src.m_atom_scale_factor = value; src.update_oi();})
         .def_property("bond_scale",
                       [](geom_view_t &src)
                       {return src.m_bond_scale_factor;},
                       [](geom_view_t &src, const float value)
                       {src.m_bond_scale_factor = value; src.update_oi();})
         .def_property("draw_atoms",
                       [](geom_view_t &src)
                       {return src.m_draw_atoms;},
                       [](geom_view_t &src, const bool value)
                       {src.m_draw_atoms = value; src.update_oi();})
         .def_property("draw_img_atoms",
                       [](geom_view_t &src)
                       {return src.m_draw_img_atoms;},
                       [](geom_view_t &src, const bool value)
                       {src.m_draw_img_atoms = value; src.update_oi();})
         .def_property("draw_bonds",
                       [](geom_view_t &src)
                       {return src.m_draw_bonds;},
                       [](geom_view_t &src, const bool value)
                       {src.m_draw_bonds = value; src.update_oi();})
         .def_property("draw_img_bonds",
                       [](geom_view_t &src)
                       {return src.m_draw_img_bonds;},
                       [](geom_view_t &src, const bool value)
                       {src.m_draw_img_bonds = value; src.update_oi();})
         .def_property("render_style",
                       [](geom_view_t &src)
                       {return src.m_render_style;},
                       [](geom_view_t &src, const geom_view_render_style_e value)
                       {src.m_render_style = value; src.update_oi();})
         .def_property("sel_vis",
                       [](geom_view_t &src)
                       {return src.m_sel_vis;},
                       [](geom_view_t &src, const bool value)
                       {src.m_sel_vis = value; src.update_oi();})
         .def_property("sel_vis_affect_bonds",
                       [](geom_view_t &src)
                       {return src.m_sel_vis_affect_bonds;},
                       [](geom_view_t &src, const bool value)
                       {src.m_sel_vis_affect_bonds = value; src.update_oi();})
         .def_property("draw_cell",
                       [](geom_view_t &src)
                       {return src.m_draw_cell;},
                       [](geom_view_t &src, const bool value)
                       {src.m_draw_cell = value; src.update_oi();})
         .def_property("cell_color",
                       [](geom_view_t &src)
                       {return src.m_cell_color;},
                       [](geom_view_t &src, const vector3<float> value)
                       {src.m_cell_color = value; src.update_oi();})
         .def_property("draw_subcells",
                       [](geom_view_t &src)
                       {return src.m_draw_subcells;},
                       [](geom_view_t &src, const bool value)
                       {src.m_draw_subcells = value; src.update_oi();})
         .def_property("subcells_range",
                       [](geom_view_t &src)
                       {return src.m_subcells_range;},
                       [](geom_view_t &src, const vector3<int> value)
                       {src.m_subcells_range = value; src.update_oi();})
         .def("ts", &geom_view_t::translate_selected)
         .def("get_xcolor", &geom_view_t::get_xcolor)
         .def("set_xcolor", &geom_view_t::set_xcolorv)
         .def("set_xcolor", &geom_view_t::set_xcolorf)
         .def("min_max_xfield", &geom_view_t::get_min_max_xfield)
         .def("colorize_by_xfield", &geom_view_t::colorize_by_xfield)
         .def("colorize_by_category", &geom_view_t::colorize_by_category)
         .def("copy_cell", &geom_view_t::copy_cell, py::arg("src"),
              py::arg("rebuild_tws_tree") = true)
         .def("copy_settings", &geom_view_t::py_copy_settings)
         .def("copy_xgeom_aux", &geom_view_t::py_copy_xgeom_aux)
         .def("get_sel_atoms", &geom_view_t::get_sel_atoms, py::arg("index_offset") = 0)
         .def("get_unsel_atoms", &geom_view_t::get_unsel_atoms, py::arg("index_offset") = 0)
         .def("sel_by_box", &geom_view_t::sel_by_box)
         .def("sel_by_box", &geom_view_t::sq_sel_by_box, py::arg("box_scale") = 1.1)
         .def("clone_on_the_spot", &geom_view_t::clone_on_the_spot)
         .def("refine_from_frac_coord", &geom_view_t::refine_from_frac_coord)
         .def("rebond", &geom_view_t::rebond)
         .def("bb_ext", [](geom_view_t &src){return src.m_ext_obs->aabb;})
         .def("begin_structure_change", &geom_view_t::begin_structure_change)
         .def("end_structure_change", &geom_view_t::end_structure_change)
         .def_readonly("anim", &geom_view_t::m_anim)
         .def_readonly("lbl", &geom_view_t::m_labels)
         .def_readonly("sg", &geom_view_t::m_selg);

  /*
   * displ_proj_set_t pyexport
   */
  py_displ_proj_reg_helper_t::reg(module);

  auto gvt = module.def_submodule("gvt", "geom_view_t tools");
  gvt.def("shake_atoms", &geom_view_tools_t::py_shake_atoms,
          py::arg("gv"), py::arg("atoms_to_shake"), py::arg("magn"))
     .def("purify_boundary_atoms", &geom_view_tools_t::purify_boundary_atoms,
          py::arg("dst"), py::arg("src"))

     .def("dipole_moment", &geom_view_tools_t::dipole_moment, py::arg("gv"))
     .def("total_charge", &geom_view_tools_t::total_charge, py::arg("gv"))
     .def("get_charge_partition", &geom_view_tools_t::py_get_charge_partition, py::arg("gv"))

     .def("pertrub_via_normal_mode", &geom_view_tools_t::pertrub_via_normal_mode,
          py::arg("gv"), py::arg("disp"))
     .def("pretty_print_selected_atoms", &geom_view_tools_t::pretty_print_selected_atoms,
          py::arg("gv"), py::arg("new_frame") = vector3<float>{0})
     .def("flip_atom_in_cell", &geom_view_tools_t::flip_atom_in_cell,
                      py::arg("gv"), py::arg("at_id"), py::arg("dim_id"),
                      py::arg("flip_magn") = 1.0f, py::arg("rebuild_tree") = false)
     .def("flip_sel_atoms_in_cell", &geom_view_tools_t::flip_sel_atoms_in_cell,
          py::arg("gv"), py::arg("dim_id"), py::arg("flip_magn") = 1)
     .def("align_atoms_to_point", &geom_view_tools_t::align_atoms_to_point,
          py::arg("gv"), py::arg("fpoint"))

     .def("get_atoms_cn", &geom_view_tools_t::get_atoms_cn, py::arg("gv"))
     .def("get_atoms_sublattices", &geom_view_tools_t::get_atoms_sublattices,
                      py::arg("gv"), py::arg("score_eps") = 0.1f)
     .def("clamp_atoms_to_cell", &geom_view_tools_t::clamp_atoms_to_cell,
                      py::arg("gv"), py::arg("ignore_selection") = true)
     .def("center_cell_on", &geom_view_tools_t::center_cell_on,
                      py::arg("gv"), py::arg("new_cnt"), py::arg("clamp_atoms") = true)

      .def("tr_align_geoms", &geom_view_tools_t::tr_align_geoms,
           py::arg("what_gv"), py::arg("to_gv"), py::arg("start_offset"),
           py::arg("axis_steps") = vector3<float>{0.1}, py::arg("total_steps") = 200)

      .def("change_cell_keep_atoms", &geom_view_tools_t::change_cell_keep_atoms,
           py::arg("gv"), py::arg("new_a"), py::arg("new_b"), py::arg("new_c"))
      .def("compose_gv_from_images", &geom_view_tools_t::compose_gv_from_images, py::arg("gvs"))

      .def("generate_scell", &geom_view_tools_t::generate_supercell,
           py::arg("src"), py::arg("dst"), py::arg("index"), py::arg("role") = std::nullopt)
      .def("generate_ncells", &geom_view_tools_t::generate_ncells, py::arg("gv"),
           py::arg("s_a"), py::arg("e_a"),
           py::arg("s_b"), py::arg("e_b"),
           py::arg("s_c"), py::arg("e_c"))

      .def("set_charge_for_type", &geom_view_tools_t::set_charge_for_type,
           py::arg("gv"), py::arg("atype"), py::arg("chg"))
      .def("purify_atom_names_from_numbers", &geom_view_tools_t::purify_atom_names_from_numbers)
      .def("cut_selected_as_new_gv", &geom_view_tools_t::cut_selected_as_new_gv,
           py::arg("gv"), py::arg("cut_selected") = true);

  auto clr = gvt.def_submodule("clr", "geom_view_t tools - colorize");
  clr.def("color_by_dist", &geom_view_colorizer_helper::py_colorize_by_distance,
          py::arg("min_dist"), py::arg("min_dist_color"),
          py::arg("over_dist_color"))
     .def("color_by_dist_pairs", &geom_view_colorizer_helper::py_colorize_by_distance_with_pairs,
          py::arg("min_dist"), py::arg("min_dist_color"), py::arg("over_dist_color"),
          py::arg("atom_type1"), py::arg("atom_type2"));

}
