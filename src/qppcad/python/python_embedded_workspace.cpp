#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/ws_item/ws_item_trampoline.hpp>
#include <qppcad/workspace.hpp>
#include <symm/index.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_labels_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_sel_groups_subsys.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view.hpp>
#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/ws_item/pdos_view/pdos_view.hpp>
#include <io/ccd_programs.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

namespace py = pybind11;

std::shared_ptr<ws_item_t> construct_from_geom(
    workspace_t &ws,
    std::shared_ptr<xgeometry<float, periodic_cell<float> > > geom,
    std::string name) {

    auto new_item = ws.m_owner->m_bhv_mgr->fbr_ws_item_by_type(geom_view_t::get_type_static());
    if (!new_item) return nullptr;

    auto as_gv = new_item->cast_as<geom_view_t>();
    if(!as_gv) return nullptr;

    if (as_gv->m_geom) {
        as_gv->m_geom->remove_observer(*as_gv->m_ext_obs);
        as_gv->m_geom->remove_observer(*as_gv->m_tws_tr);
      }

    as_gv->m_geom = geom;
    as_gv->m_ext_obs->geom = as_gv->m_geom.get();
    as_gv->m_tws_tr->geom = as_gv->m_geom.get();

    as_gv->m_tws_tr->do_action(act_lock | act_clear_all);
    as_gv->m_geom->add_observer(*as_gv->m_ext_obs);
    as_gv->m_geom->add_observer(*as_gv->m_tws_tr);
    as_gv->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
    as_gv->m_tws_tr->do_action(act_rebuild_ntable);
    as_gv->m_name = name;
    ws.add_item_to_ws(new_item);

    return new_item;

}

std::shared_ptr<ws_item_t> construct_from_array_group(
    workspace_t &ws,
    std::shared_ptr<array_group<matrix3<float> > > ag,
    std::string name) {

    auto new_item = ws.m_owner->m_bhv_mgr->fbr_ws_item_by_type(psg_view_t::get_type_static());
    if (!new_item) return nullptr;

    auto as_psg = new_item->cast_as<psg_view_t>();
    if(!as_psg) return nullptr;

    as_psg->m_ag = ag;
    as_psg->m_name = name;
    ws.add_item_to_ws(new_item);
    as_psg->update_view();

    return new_item;

}

void upd_oi(ws_item_t *_item) {

  if (_item && _item->m_selected)
    app_state_t::get_inst()->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();

}

void mvd() {

  app_state_t *astate = app_state_t::get_inst();
  astate->make_viewport_dirty();

}

PYBIND11_EMBEDDED_MODULE(cad, m) {

  py::class_<workspace_manager_t,  std::shared_ptr<workspace_manager_t> >(m, "workspace_manager_t")
      .def("__len__", [](const workspace_manager_t &wsm){ return wsm.m_ws.size();})
      .def("__getitem__", [](const workspace_manager_t &wsm, size_t i) {
        if (i >= wsm.m_ws.size()) throw py::index_error();
        return wsm.m_ws[i];
      }, py::return_value_policy::reference_internal)
      .def("__getitem__", [](workspace_manager_t &wsm, std::string ws_name) {
        auto retv = wsm.get_by_name(ws_name);
        if (!retv) throw py::key_error();
        return retv;
       }, py::return_value_policy::reference_internal)
      .def("has_wss", &workspace_manager_t::has_wss)
      .def("set_cur", &workspace_manager_t::set_cur_id);

  py::class_<workspace_t,  std::shared_ptr<workspace_t> >(m, "workspace_t")
      .def_readwrite("name", &workspace_t::m_ws_name)
      .def("__len__", [](workspace_t &ws) {return ws.m_ws_items.size();})
      .def("__getitem__", [](workspace_t &ws, size_t i) {
        if (i >= ws.m_ws_items.size()) throw py::index_error();
        return ws.m_ws_items[i];
      }, py::return_value_policy::reference_internal, py::keep_alive<0,2>())
      .def("__getitem__", [](workspace_t &ws, std::string item_name) {
        auto retv = ws.get_by_name(item_name);
        if (!retv) throw py::key_error();
        return retv;
       }, py::return_value_policy::reference_internal, py::keep_alive<0,2>())
      .def("construct", &workspace_t::py_construct_item)
      .def("construct", &construct_from_geom)
      .def("construct", &construct_from_array_group)
      .def_property("bg",
                    [](workspace_t &src)
                    {return src.m_background_color;},
                    [](workspace_t &src, const vector3<float> value)
                    {src.m_background_color = value; mvd();})
      .def("__repr__", &workspace_t::py_get_repr)
      .def("__str__", &workspace_t::py_get_repr);

  py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> py_ws_item_t(m, "ws_item_t");
  py_ws_item_t
          .def_readwrite("name", &ws_item_t::m_name)
          .def("get_cnt_count", &ws_item_t::get_content_count)
          .def("get_parent_ws", [](ws_item_t &wsi){return wsi.m_parent_ws;})
         // .def_readwrite("m_pos", &ws_item_t::get_pos, &ws_item_t::set_pos)
          .def_property("is_visible",
                        [](ws_item_t &src)
                        {return src.m_is_visible;},
                        [](ws_item_t &src, const bool value)
                        {src.m_is_visible = value; upd_oi(&src);})
          .def_property("show_bb",
                        [](ws_item_t &src)
                        {return src.m_show_bb;},
                        [](ws_item_t &src, const bool value)
                        {src.m_show_bb = value; upd_oi(&src);})
          .def_property("pos",
                        [](ws_item_t &src)
                        {return src.m_pos;},
                        [](ws_item_t &src, const vector3<float> value)
                        {src.m_pos = value; upd_oi(&src);})
          .def("__repr__", &ws_item_t::py_get_repr)
          .def("__str__", &ws_item_t::py_get_repr)
          .def_readwrite("offset", &ws_item_t::m_leader_offset);

  py::enum_<geom_view_render_style_e>(m, "geom_view_render_style_t", py::arithmetic(), "")
          .value("ball_and_stick", ball_and_stick, "ball_and_stick")
          .value("dynamic_lines", dynamic_lines, "dynamic_lines")
          .value("xatom_lines", xatom_lines, "xatom_lines")
          .value("billboards", billboards, "billboards")
          .value("buffered_billboards", buffered_billboards, "buffered_billboards")
          .export_values();

  py::enum_<geom_labels_style_e>(m, "geom_view_labels_style_t", py::arithmetic(), "")
          .value("show_none", show_none, "show_none")
          .value("show_id", show_id, "show_id")
          .value("show_type", show_type, "show_type")
          .value("show_id_type", show_id_type, "show_id_type")
          .value("show_charge", show_charge, "show_charge")
          .value("show_custom", show_custom, "show_custom")
          .export_values();

  py::enum_<geom_anim_t>(m, "geom_anim_t", py::arithmetic(), "")
          .value("anim_static", anim_static, "anim_static")
          .value("anim_generic", anim_generic, "anim_generic")
          .value("anim_geo_opt", anim_geo_opt, "anim_geo_opt")
          .value("anim_md", anim_md, "anim_md")
          .value("anim_vib", anim_vib, "anim_vib")
          .export_values();

  py::enum_<comp_chem_program_e>(m, "comp_chem_program_e", py::arithmetic(), "")
          .value("pr_unknown", pr_unknown, "pr_unknown")
          .value("pr_vasp", pr_vasp, "pr_vasp")
          .value("pr_firefly", pr_firefly, "pr_firefly")
          .value("pr_pc_gamess", pr_pc_gamess, "pr_pc_gamess")
          .value("pr_cp2k", pr_cp2k, "pr_cp2k")
          .value("pr_orca", pr_orca, "pr_orca")
          .value("pr_molcas", pr_molcas, "pr_molcas")
          .value("pr_molden", pr_molden, "pr_molden")
          .export_values();

  py::class_<geom_view_anim_subsys_t, std::shared_ptr<geom_view_anim_subsys_t> >
  py_geom_view_anim(m, "geom_view_anim_subsys_t");
  py_geom_view_anim.def("nanim", &geom_view_anim_subsys_t::get_total_anims);
  py_geom_view_anim.def("animable", &geom_view_anim_subsys_t::animable);
  py_geom_view_anim.def("next_anim", &geom_view_anim_subsys_t::next_anim);
  py_geom_view_anim.def("prev_anim", &geom_view_anim_subsys_t::prev_anim);
  py_geom_view_anim.def("to_begin", &geom_view_anim_subsys_t::update_current_frame_to_begin);
  py_geom_view_anim.def("to_end", &geom_view_anim_subsys_t::update_current_frame_to_end);
  py_geom_view_anim.def("to_frame", &geom_view_anim_subsys_t::update_current_frame_to);
  py_geom_view_anim.def("make_animable", &geom_view_anim_subsys_t::make_animable);
  py_geom_view_anim.def("make_anim", &geom_view_anim_subsys_t::make_anim);
  py_geom_view_anim.def("get_anim_type", &geom_view_anim_subsys_t::get_anim_type_by_idx);
  py_geom_view_anim.def("get_cur_anim_type", &geom_view_anim_subsys_t::get_cur_anim_type);
  py_geom_view_anim.def("make_interpolated_anim",
                        &geom_view_anim_subsys_t::make_interpolated_anim);
  py_geom_view_anim.def("commit_apos", &geom_view_anim_subsys_t::commit_atom_pos,
                        py::arg("atom_id"),
                        py::arg("lerp_from_start") = false,
                        py::arg("propagate_to_the_end") = false);
  py_geom_view_anim.def("is_cell_animable", &geom_view_anim_subsys_t::is_cell_animable);
  py_geom_view_anim.def("get_cell_vectors", &geom_view_anim_subsys_t::get_cell_vectors);
  py_geom_view_anim.def("get_atom_pos", &geom_view_anim_subsys_t::get_atom_pos);

  py::class_<atom_index_set_key, std::shared_ptr<atom_index_set_key> >
  py_atom_index_set_key(m, "atom_index_set_key_t");
  py_atom_index_set_key.def_readwrite("atm", &atom_index_set_key::m_atm);
  py_atom_index_set_key.def_readwrite("idx", &atom_index_set_key::m_idx);
  py_atom_index_set_key.def(py::init<const int, const qpp::index>());
  py_atom_index_set_key.def("__str__",
                            [](atom_index_set_key &src) {
                              return fmt::format("atm={}, idx={}", src.m_atm, src.m_idx);
                            });

  py::class_<geom_view_sel_group_t, std::shared_ptr<geom_view_sel_group_t> >
  py_geom_view_sel_group(m, "geom_view_sel_group_t");
  py_geom_view_sel_group.def_readwrite("name", &geom_view_sel_group_t::m_name);
  py_geom_view_sel_group.def(py::init<std::string&>());
  py_geom_view_sel_group.def("append", &geom_view_sel_group_t::append);
  py_geom_view_sel_group.def("remove", &geom_view_sel_group_t::remove);
  py_geom_view_sel_group.def("__len__", [](geom_view_sel_group_t &selg) {
    return selg.m_data.size();
  });
  py_geom_view_sel_group.def("__getitem__", [](geom_view_sel_group_t &selg, size_t i) {
    if (i >= selg.m_data.size()) throw py::index_error();
    return selg.m_data[i];
  }, py::return_value_policy::reference_internal);

  py::class_<geom_view_sel_groups_subsys_t, std::shared_ptr<geom_view_sel_groups_subsys_t> >
  pygeom_view_sel_groups_subsys(m, "geom_view_sel_groups_subsys_t");
  pygeom_view_sel_groups_subsys.def("make_from_selected",
                                    &geom_view_sel_groups_subsys_t::make_from_selected);
  pygeom_view_sel_groups_subsys.def("__len__", [](geom_view_sel_groups_subsys_t &sels) {
    return sels.m_sel_grps.size();
  });

  py::class_<geom_view_t, std::shared_ptr<geom_view_t> >
  py_atoms_list_t(m, "geom_view_t", py_ws_item_t);
  py_atoms_list_t.def_readwrite("geom", &geom_view_t::m_geom);
  py_atoms_list_t.def_property("atom_scale",
                               [](geom_view_t &src)
                               {return src.m_atom_scale_factor;},
                               [](geom_view_t &src, const float value)
                               {src.m_atom_scale_factor = value; upd_oi(&src);});
  py_atoms_list_t.def_property("bond_scale",
                               [](geom_view_t &src)
                               {return src.m_bond_scale_factor;},
                               [](geom_view_t &src, const float value)
                               {src.m_bond_scale_factor = value; upd_oi(&src);});
  py_atoms_list_t.def_property("draw_atoms",
                               [](geom_view_t &src)
                               {return src.m_draw_atoms;},
                               [](geom_view_t &src, const bool value)
                               {src.m_draw_atoms = value; upd_oi(&src);});
  py_atoms_list_t.def_property("draw_img_atoms",
                               [](geom_view_t &src)
                               {return src.m_draw_img_atoms;},
                               [](geom_view_t &src, const bool value)
                               {src.m_draw_img_atoms = value; upd_oi(&src);});
  py_atoms_list_t.def_property("draw_bonds",
                               [](geom_view_t &src)
                               {return src.m_draw_bonds;},
                               [](geom_view_t &src, const bool value)
                               {src.m_draw_bonds = value; upd_oi(&src);});
  py_atoms_list_t.def_property("draw_img_bonds",
                               [](geom_view_t &src)
                               {return src.m_draw_img_bonds;},
                               [](geom_view_t &src, const bool value)
                               {src.m_draw_img_bonds = value; upd_oi(&src);});
  py_atoms_list_t.def_property("render_style",
                               [](geom_view_t &src)
                               {return src.m_render_style;},
                               [](geom_view_t &src, const geom_view_render_style_e value)
                               {src.m_render_style = value; upd_oi(&src);});
  py_atoms_list_t.def_property("labels_style",
                               [](geom_view_t &src)
                               {return src.m_labels->m_style;},
                               [](geom_view_t &src, const geom_labels_style_e value)
                               {src.m_labels->m_style = value; upd_oi(&src);});
  py_atoms_list_t.def_property("sel_vis",
                               [](geom_view_t &src)
                               {return src.m_sel_vis;},
                               [](geom_view_t &src, const bool value)
                               {src.m_sel_vis = value; upd_oi(&src);});
  py_atoms_list_t.def_property("sel_vis_affect_bonds",
                               [](geom_view_t &src)
                               {return src.m_sel_vis_affect_bonds;},
                               [](geom_view_t &src, const bool value)
                               {src.m_sel_vis_affect_bonds = value; upd_oi(&src);});
  py_atoms_list_t.def_property("draw_cell",
                               [](geom_view_t &src)
                               {return src.m_draw_cell;},
                               [](geom_view_t &src, const bool value)
                               {src.m_draw_cell = value; upd_oi(&src);});
  py_atoms_list_t.def_property("cell_color",
                               [](geom_view_t &src)
                               {return src.m_cell_color;},
                               [](geom_view_t &src, const vector3<float> value)
                               {src.m_cell_color = value; upd_oi(&src);});
  py_atoms_list_t.def_property("draw_subcells",
                               [](geom_view_t &src)
                               {return src.m_draw_subcells;},
                               [](geom_view_t &src, const bool value)
                               {src.m_draw_subcells = value; upd_oi(&src);});
  py_atoms_list_t.def_property("subcells_range",
                               [](geom_view_t &src)
                               {return src.m_subcells_range;},
                               [](geom_view_t &src, const vector3<int> value)
                               {src.m_subcells_range = value; upd_oi(&src);});
  py_atoms_list_t.def("get_xcolor", &geom_view_t::get_xcolor);
  py_atoms_list_t.def("set_xcolor", &geom_view_t::set_xcolorv);
  py_atoms_list_t.def("set_xcolor", &geom_view_t::set_xcolorf);
  py_atoms_list_t.def("min_max_xfield", &geom_view_t::get_min_max_xfield);
  py_atoms_list_t.def("colorize_xfield", &geom_view_t::colorize_by_xfield);
  py_atoms_list_t.def("copy_cell", &geom_view_t::copy_cell, py::arg("src"),
                      py::arg("rebuild_tws_tree") = true);
  py_atoms_list_t.def("clone_on_the_spot", &geom_view_t::clone_on_the_spot);
  py_atoms_list_t.def("refine_from_frac_coord", &geom_view_t::refine_from_frac_coord);

  py_atoms_list_t.def("rebond", &geom_view_t::rebond);
  py_atoms_list_t.def_readonly("anim", &geom_view_t::m_anim);
  py_atoms_list_t.def_readonly("sg", &geom_view_t::m_selg);

  py::class_<psg_view_t, std::shared_ptr<psg_view_t> >
  py_point_sym_group_t(m, "psg_view_t", py_ws_item_t);
  py_point_sym_group_t.def_readwrite("ag", &psg_view_t::m_ag, py::return_value_policy::reference);
  py_point_sym_group_t.def_readwrite("pg_axes", &psg_view_t::m_pg_axes);
  py_point_sym_group_t.def("update_view", &psg_view_t::update_view);

  py::class_<volume_view_t, std::shared_ptr<volume_view_t> >
  py_volume_view_t(m, "volume_view_t", py_ws_item_t);

  py::class_<node_book_t, std::shared_ptr<node_book_t> >
  py_node_book_t(m, "node_book_t", py_ws_item_t);

  py_node_book_t.def("num_nds", [](node_book_t &src){return src.m_scene->m_nodes.size();});
  py_node_book_t.def("num_sck", [](node_book_t &src){return src.m_scene->m_sockets.size();});
  py_node_book_t.def("num_con", [](node_book_t &src){return src.m_scene->m_connections.size();});
  py_node_book_t.def("execute", &node_book_t::execute);

  py::class_<pdos_view_t, std::shared_ptr<pdos_view_t> >
  py_pdos_view_t(m, "pdos_view_t", py_ws_item_t);
  py_pdos_view_t.def("load", &pdos_view_t::py_load_from_list);
  py_pdos_view_t.def("load_dir", &pdos_view_t::py_load_from_dir);
  py_pdos_view_t.def("rebuild", &pdos_view_t::rebuild_plots);
  py_pdos_view_t.def("scale_chn", &pdos_view_t::scale_channel);
  py_pdos_view_t.def("e_fermi", &pdos_view_t::py_get_efermi_for_channel);

}
