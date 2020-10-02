#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_render_bs.hpp>
#include <qppcad/ws_item/geom_view/geom_view_render_dlines.hpp>
#include <qppcad/ws_item/geom_view/geom_view_render_xlines.hpp>
#include <qppcad/ws_item/geom_view/geom_view_render_billboards.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_measurement_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_labels_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_lattice_planes_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_sel_groups_subsys.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view.hpp>
#include <qppcad/core/json_helpers.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/core/timing.hpp>

#include <random>
#include <clocale>

using namespace qpp;
using namespace qpp::cad;

geom_view_t::geom_view_t(): ws_item_t() {

  /*hs begin*/
  begin_recording(hs_doc_rec_type_e::hs_doc_rec_init);

  add_hs_child(&m_shading_specular_power);
  add_hs_child(&m_atom_scale_factor);
  add_hs_child(&m_bond_scale_factor);
  add_hs_child(&m_cell_color);
  add_hs_child(&m_render_style);
  add_hs_child(&m_subcells_range);
  add_hs_child(&m_color_mode);
  add_hs_child(&m_role);
  add_hs_child(&m_draw_img_atoms);
  add_hs_child(&m_draw_img_bonds);
  add_hs_child(&m_draw_specular);
  add_hs_child(&m_draw_bonds);
  add_hs_child(&m_draw_atoms);
  add_hs_child(&m_bt_show_disabled_record);
  add_hs_child(&m_draw_cell);
  add_hs_child(&m_draw_subcells);
  add_hs_child(&m_draw_subcells);
  add_hs_child(&m_cell_vectors_ratio);
  add_hs_child(&m_cell_vector_offset);
  add_hs_child(&m_cell_vector_color);
  add_hs_child(&m_subcell_color);
  add_hs_child(&m_sel_vis);
  add_hs_child(&m_sel_vis_affect_bonds);

  m_shading_specular_power.set_value(12.0f);
  m_atom_scale_factor.set_value(0.3f);
  m_bond_scale_factor.set_value(0.09f);
  m_cell_color.set_value({0.1f, 0.1f, 0.1f});

  m_render_style.set_value(geom_view_render_style_e::ball_and_stick);

  m_subcells_range.set_value({1,1,1});
  m_color_mode.set_value(geom_view_color_e::color_from_ptable);
  m_role.set_value(geom_view_role_e::r_generic);
  m_draw_img_atoms.set_value(true);
  m_draw_img_bonds.set_value(true);
  m_draw_specular.set_value(true);
  m_draw_bonds.set_value(true);
  m_draw_atoms.set_value(true);
  m_bt_show_disabled_record.set_value(true);
  m_draw_cell.set_value(true);
  m_draw_subcells.set_value(false);
  m_draw_subcells.set_value(false);

  m_cell_vectors_ratio.set_value(0.35f);
  m_cell_vector_offset.set_value({0, 0, 0});
  m_cell_vector_color.set_value({1.0f, 1.0f, 0.0f});

  m_subcell_color.set_value({0.1f, 0.1f, 0.1f});
  m_sel_vis.set_value(false);
  m_sel_vis_affect_bonds.set_value(false);

  /*hs end*/

  set_default_flags(ws_item_flags_default
                    | ws_item_flags_support_tr
                    | ws_item_flags_support_sel
                    | ws_item_flags_support_cnt_edit
                    | ws_item_flags_support_render_bb
                    | ws_item_flags_toolbar_extension
                    | ws_item_flags_support_actions
                    | ws_item_flags_support_delete
                    | ws_item_flags_support_clone
                    | ws_item_flags_support_moveto
                    | ws_item_flags_support_rendering
                    | ws_item_flags_support_view_voting
                    | ws_item_flags_cam_target_view);

  m_geom = std::make_shared<xgeometry<float, periodic_cell<float> > >(3, "rg1");

  /* 0 atom
   * 1 X
   * 2 Y
   * 3 Z
   * 4 charge
   * 5 number
   * 6 hide
   * 7 sel
   * 8 ccr - red component of custom color
   * 9 ccg - green component of custom color
   * 10 ccb - blue component of custom color
   */

  m_geom->set_format(

      {"atom", //0
          "x",//1
          "y",//2
          "z",//3
          "charge",//4
          "number", //5
          "hide",//6
          "lshow",//7
          "r",//8
          "g",//9
          "b",//10
          "ltext",//11
          "ov",//12
          "atom_r", // 13
          "tag" //14
      },

      {basic_types::type_string, //0
       basic_types::type_real, //1
       basic_types::type_real, //2
       basic_types::type_real, //3
       basic_types::type_real, //4
       basic_types::type_int, //5
       basic_types::type_bool, //6
       basic_types::type_bool, //7
       basic_types::type_real, //8
       basic_types::type_real, //9
       basic_types::type_real, //10
       basic_types::type_string, //11
       basic_types::type_bool, // 12
       basic_types::type_real, // 13
       basic_types::type_string //14
      } );

  m_geom->set_DIM(0);
  //m_geom->cell.DIM = 0;
  m_geom->auto_update_types = true;
  end_recording();

  m_ext_obs = std::make_unique<extents_observer_t<float>>(*m_geom);
  m_tws_tr  = std::make_unique<tws_tree_t<float>>(*m_geom);
  m_tws_tr->do_action(act_unlock);

  m_anim = std::make_shared<geom_view_anim_subsys_t>(*this);
  m_measure = std::make_shared<geom_view_msr_subsys_t>(*this);
  m_labels = std::make_shared<geom_view_labels_subsys_t>(*this);
  m_lat_planes = std::make_shared<geom_view_lat_planes_subsys_t>(*this);
  m_selg = std::make_shared<geom_view_sel_groups_subsys_t>(*this);

  m_xgeom_proxy.set_xgeom(m_geom.get());

  add_hs_child(&m_xgeom_proxy);
  add_hs_child(m_anim.get());
  add_hs_child(m_measure.get());
  add_hs_child(m_labels.get());
  add_hs_child(m_lat_planes.get());
  add_hs_child(m_selg.get());

}

void geom_view_t::vote_for_view_vectors(vector3<float> &out_look_pos, vector3<float> &out_look_at){
  if (m_geom->nat() > 1) {
    out_look_at += (m_ext_obs->aabb.max + m_ext_obs->aabb.min) / 2.0;
    vector3<float> bb_size = m_ext_obs->aabb.max - m_ext_obs->aabb.min;
    float size = bb_size.norm();
    float g_sz_mod = 2.0f;
    out_look_pos += g_sz_mod * m_ext_obs->aabb.max.normalized() * clamp<float>(size, 10.0, 60.0);
  } else {
    out_look_pos += vector3<float>(0.0, 0.0, -5.0);
  }

}

void geom_view_t::target_view(cam_tv_e target_view_src,
                              vector3<float> &look_from,
                              vector3<float> &look_to,
                              vector3<float> &look_up,
                              bool &need_to_update_camera) {

  auto new_target_view = target_view_src;

  if (new_target_view == cam_tv_e::tv_auto) {
    if (m_geom->get_DIM() != 0) {
      new_target_view = cam_tv_e::tv_a;
    } else if (m_ext_obs->aabb.center().isApprox(vector3<float>{0}, 0.001f)) {
      new_target_view = cam_tv_e::tv_x;
    } else {
      new_target_view = cam_tv_e::tv_cc;
    }
  }

  switch (new_target_view) {

  case cam_tv_e::tv_x : {
    float axis_size = std::max(2.0f, m_ext_obs->aabb.max[0] - m_ext_obs->aabb.min[0]);
    look_from = m_pos.get_value() - 2.0f*vector3<float>(axis_size, 0.0, 0.0);
    look_to = m_pos.get_value();
    look_up = {0.0 , 0.0 , 1.0};
    need_to_update_camera = true;
    break;
  }

  case cam_tv_e::tv_y : {
    float axis_size = std::max(2.0f, m_ext_obs->aabb.max[1] - m_ext_obs->aabb.min[1]);
    look_from = m_pos.get_value() - 2.0f*vector3<float>(0.0, axis_size, 0.0);
    look_to = m_pos.get_value();
    look_up = {0.0, 0.0, 1.0};
    need_to_update_camera = true;
    break;
  }

  case cam_tv_e::tv_z : {
    float axis_size = std::max(2.0f, m_ext_obs->aabb.max[2] - m_ext_obs->aabb.min[2]);
    look_from = m_pos.get_value() - 2.0f*vector3<float>(0.0, 0.0, axis_size);
    look_to = m_pos.get_value();
    look_up = {0.0, 1.0, 0.0};
    need_to_update_camera = true;
    break;
  }

  case cam_tv_e::tv_a : {
    vector3<float> center = 0.5*(m_geom->cell.v[0] + m_geom->cell.v[1] + m_geom->cell.v[2]);
    look_from = m_pos.get_value() + center - 2.0f * m_geom->cell.v[0];
    look_to = m_pos.get_value()  + center;
    look_up = {0.0 , 0.0 , 1.0};
    need_to_update_camera = true;
    break;
  }

  case cam_tv_e::tv_b : {
    vector3<float> center = 0.5*(m_geom->cell.v[0] + m_geom->cell.v[1] + m_geom->cell.v[2]);
    look_from = m_pos.get_value() + center - 2.0f * m_geom->cell.v[1];
    look_to = m_pos.get_value()  + center;
    look_up = {0.0, 0.0, 1.0};
    need_to_update_camera = true;
    break;
  }

  case cam_tv_e::tv_c : {
    vector3<float> center = 0.5*(m_geom->cell.v[0] + m_geom->cell.v[1] + m_geom->cell.v[2]);
    look_from = m_pos.get_value() + center - 2.0f * m_geom->cell.v[2];
    look_to = m_pos.get_value()  + center;
    look_up = {0.0, 1.0, 0.0};
    need_to_update_camera = true;
    break;
  }

  case cam_tv_e::tv_cc : {
    float axis_size = std::max(2.0f, m_ext_obs->aabb.max[0] - m_ext_obs->aabb.min[0]);
    look_from = m_pos.get_value()
                + axis_size * (m_ext_obs->aabb.max - m_ext_obs->aabb.min).normalized();
    look_to = m_pos.get_value() + (m_ext_obs->aabb.max + m_ext_obs->aabb.min) * 0.5;
    look_up = {0.0 , 0.0 , 1.0};
    need_to_update_camera = true;
    break;
  }

  default : {
    break;
  }

  }

}

void geom_view_t::geometry_changed() {
  if (m_ext_obs)
    m_aabb = m_ext_obs->aabb;
}

void geom_view_t::render() {

  ws_item_t::render();

  if (!m_geom)
    return;

  app_state_t* astate = app_state_t::get_inst();
  vector3<float> _pos = m_pos.get_value();
  index all_null = index::D(m_geom->get_DIM()).all(0);

  if (astate->dp) {

    if (astate->m_debug_show_tws_tree) {
      astate->dp->begin_render_aabb();
      m_tws_tr->apply_visitor([astate, _pos](tws_node_t<float> *in_node, int deep_level) {
        astate->dp->render_aabb(clr_maroon,
                                in_node->m_bb.min+_pos,
                                in_node->m_bb.max+_pos);
      });
      astate->dp->end_render_aabb();
    }

    if (!m_is_visible.get_value())
      return;

    if (m_geom->get_DIM() == 3 && m_is_visible.get_value() && m_draw_cell.get_value()) {
      astate->dp->begin_render_line();

      if (m_draw_subcells.get_value()) {
        auto subcells_range = m_subcells_range.get_value();
        vector3<float> sc_a = m_geom->cell.v[0] / subcells_range[0];
        vector3<float> sc_b = m_geom->cell.v[1] / subcells_range[1];
        vector3<float> sc_c = m_geom->cell.v[2] / subcells_range[2];

        for (int i_a = 0; i_a < subcells_range[0]; i_a++)
          for (int i_b = 0; i_b < subcells_range[1]; i_b++)
            for (int i_c = 0; i_c < subcells_range[2]; i_c++) {
              vector3<float> new_pos = m_pos.get_value()
                                       + sc_a * i_a + sc_b * i_b + sc_c * i_c ;
              astate->dp->render_cell_3d(m_subcell_color.get_value(),
                                         sc_a, sc_b, sc_c, new_pos);
            }

      }

      vector3<float> cell_clr = m_cell_color.get_value();

      if (m_selected) {
        if(m_parent_ws->get_edit_type() == ws_edit_type_e::edit_item)
          cell_clr = clr_red;
        if(m_parent_ws->get_edit_type() == ws_edit_type_e::edit_content)
          cell_clr = clr_maroon;
      }

      astate->dp->render_cell_3d(cell_clr, m_geom->cell.v[0], m_geom->cell.v[1],
                                 m_geom->cell.v[2], m_pos.get_value());

      astate->dp->end_render_line();

    }

    if (m_geom->get_DIM() == 3 && m_draw_cell_vectors.get_value()) {

      astate->dp->begin_render_general_mesh();

      for (size_t i = 0; i < m_geom->get_DIM(); i++) {
        vector3<float> cell_v = m_geom->cell.v[i] * m_cell_vectors_ratio.get_value();
        astate->dp->render_arrow(
            m_pos.get_value() + m_cell_vector_offset.get_value(),
            m_pos.get_value() + m_cell_vector_offset.get_value() + cell_v,
            m_cell_vector_color.get_value(),
            0.1f, 0.17f, 0.25f, false);
      }

      astate->dp->end_render_general_mesh();

    }

    switch (m_render_style.get_value()) {

    case geom_view_render_style_e::ball_and_stick :
      geom_view_render_bs::render(*this);
      break;

    case geom_view_render_style_e::suprematic_atoms :
      geom_view_render_bs::render_suprematic(*this);
      break;

    case geom_view_render_style_e::dynamic_lines:
      geom_view_render_dlines::render(*this);
      break;

    case geom_view_render_style_e::xatom_lines:
      geom_view_render_xlines::render(*this);
      break;

    case geom_view_render_style_e::billboards:
      geom_view_render_billboards::render(*this);
      break;

    case geom_view_render_style_e::buffered_billboards: {
      if (!m_bs) {
        m_bs = std::make_unique<geom_view_render_buffered_billboards_t>(*this);
        m_bs->init();
      } else {
        m_bs->render();
      }
      //geom_view_render_billboards::render(*this);
      break;
    }

      //        default:
      //          break;

    }
    // atom render start

  }

  //render measurement

  m_lat_planes->render();

}

void geom_view_t::render_overlay(QPainter &painter) {
  m_measure->render_overlay(painter);
  m_labels->render_overlay(painter);
}

void geom_view_t::rebond() {
  app_state_t *astate = app_state_t::get_inst();
  m_tws_tr->m_bonding_table.init_default(m_geom.get());
  m_tws_tr->do_action(act_rebuild_ntable);
  if (is_selected())
    astate->astate_evd->cur_ws_selected_item_changed();
}

void geom_view_t::set_cell_within_eps(float value) {
  begin_structure_change();
  m_tws_tr->m_cell_within_eps = value;
  end_structure_change();
}

bool geom_view_t::mouse_click(ray_t<float> *click_ray) {

  app_state_t* astate = app_state_t::get_inst();
  m_need_to_update_overview = true;

  if (click_ray && m_geom && m_parent_ws) {
    std::vector<tws_query_data_t<float, size_t> > res;
    ray_t<float> local_geom_ray;
    local_geom_ray.start = click_ray->start - m_pos.get_value();
    local_geom_ray.dir = click_ray->dir;
    if (m_draw_img_atoms.get_value()) {
      m_tws_tr->query_ray<query_ray_add_all<float>>(local_geom_ray,
                                                    res,
                                                    m_atom_type_to_hide,
                                                    m_atom_scale_factor.get_value(),
                                                    m_sel_vis.get_value(),
                                                    xgeom_sel_vis_hide);
    } else {
      m_tws_tr->query_ray<query_ray_add_ignore_img<float>>(local_geom_ray,
                                                           res,
                                                           m_atom_type_to_hide,
                                                           m_atom_scale_factor.get_value(),
                                                           m_sel_vis.get_value(),
                                                           xgeom_sel_vis_hide);
    }
    recalc_gizmo_barycenter();
    if (!res.empty()) {
      std::sort(res.begin(), res.end(), &tws_query_data_sort_by_dist<float>);
      if (m_parent_ws->get_edit_type() == ws_edit_type_e::edit_content && m_selected) {
        if (res[0].m_idx == index::D(m_geom->get_DIM()).all(0)) {
          begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
          m_geom->toggle_selected(res[0].m_atm);
          end_recording();
        }
      }
      recalc_gizmo_barycenter();
      m_parent_ws->m_gizmo->update_gizmo(0.01f);
      timer_t timer_sel_nempty;
      astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
      astate->tlog("gv::mouse_click::atoms_selected spent {} sec.", timer_sel_nempty.elapsed());
      return true;
    } else {
      if (m_parent_ws->get_edit_type() == ws_edit_type_e::edit_content && m_selected ) {
        sel_atoms(false);
      }
    }

  }

  timer_t timer_sel_empty;
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
  astate->tlog("gv::mouse_click::atoms_nonselected spent {} sec.", timer_sel_empty.elapsed());
  return false;

}

void geom_view_t::mouse_double_click(ray_t<float> *ray) {
  app_state_t* astate = app_state_t::get_inst();
  if (is_selected()
      && m_parent_ws
      && m_parent_ws->get_edit_type() == ws_edit_type_e::edit_item) {
    m_parent_ws->set_edit_type(ws_edit_type_e::edit_content);
    astate->astate_evd->obj_insp_tab_open_requested(3);
  }
}

void geom_view_t::sel_atoms(bool all) {
  app_state_t* astate = app_state_t::get_inst();
  if (!m_geom)
    return;
  std::set<int> selected_atoms;
  for (int i = 0; i < m_geom->num_aselected(); i++)
    selected_atoms.insert((*m_geom->nth_aselected(i)).m_atm);
  begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  if (all) {
    for (int i = 0; i < m_geom->nat(); i++)
      if (selected_atoms.find(i) == end(selected_atoms))
        m_geom->select(i, true);
  } else { // need to unselect all, iterates over selecting
    for (auto &sel_rec : selected_atoms)
      m_geom->select(sel_rec, false);
  }
  end_recording();
  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
  astate->make_viewport_dirty();
}

void geom_view_t::sel_atom(int atom_id) {
  if (!m_geom)
    return;
  sel_atom(atom_id, index::D(m_geom->get_DIM()).all(0));
}

void geom_view_t::sel_atom(int atom_id, index atom_idx) {
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
  m_need_to_update_overview = true;
  if (!m_geom)
    return;
  if (atom_id >= 0 && atom_id < m_geom->nat()) {
    m_geom->select(atom_id);
    if (m_atom_ord_sel.size() >= max_sel_in_deque) {
      m_atom_ord_sel.resize(max_sel_in_deque);
      m_atom_ord_sel.pop_front();
    }
    m_atom_ord_sel.push_back(atom_index_set_key(atom_id, atom_idx));
    recalc_gizmo_barycenter();
    m_parent_ws->m_gizmo->update_gizmo(0.01f);
    if (m_geom->nat() < 1000) {
      astate->wlog(">> SEL: {}{:<8} {:>16.5f} {:>16.5f} {:>16.5f} {}",
                   m_geom->atom_name(atom_id),
                   atom_id,
                   m_geom->pos(atom_id)[0],
                   m_geom->pos(atom_id)[1],
                   m_geom->pos(atom_id)[2],
                   atom_idx == index::D(m_geom->get_DIM()).all(0) ? "" : "*");
    }
    astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
    return;
  }
  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
  return;
}

void geom_view_t::sel_visible() {
  begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (size_t i = 0; i < m_geom->nat(); i++)
    if (!m_geom->xfield<bool>(xgeom_sel_vis_hide, i))
      sel_atom(i);
  end_recording();
}

void geom_view_t::unsel_atom(int atom_id) {
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
  m_need_to_update_overview = true;
  if (!m_geom)
    return;
  if (atom_id >= 0 && atom_id < m_geom->nat()) {
    for (iterator idx(index::D(m_geom->get_DIM()).all(-1), index::D(m_geom->get_DIM()).all(1));
         !idx.end(); idx++ ) {
      auto key = atom_index_set_key(atom_id, idx);
      m_geom->select(atom_id, false);
      auto it_ordered = std::find(m_atom_ord_sel.begin(), m_atom_ord_sel.end(), key);
      if (it_ordered != m_atom_ord_sel.end())
        m_atom_ord_sel.erase(it_ordered);
    }
    recalc_gizmo_barycenter();
    m_parent_ws->m_gizmo->update_gizmo(0.01f);
    astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
    return;
  }
  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
  return;
}

void geom_view_t::unsel_atom(int atom_id, index atom_idx) {
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
  m_need_to_update_overview = true;
  if (!m_geom)
    return;
  if (atom_id >= 0 && atom_id < m_geom->nat()) {
    auto key = atom_index_set_key(atom_id, atom_idx);
    m_geom->select(atom_id, false);
    auto it_ordered = std::find(m_atom_ord_sel.begin(), m_atom_ord_sel.end(), key);
    if (it_ordered != m_atom_ord_sel.end())
      m_atom_ord_sel.erase(it_ordered);
    recalc_gizmo_barycenter();
    m_parent_ws->m_gizmo->update_gizmo(0.01f);
    return;
  }
  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  return;
}

void geom_view_t::sel_by_type(const int item_type_to_select) {
  app_state_t* astate = app_state_t::get_inst();
  if (!m_geom)
    return;
  begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (auto i = 0; i < m_geom->nat(); i++)
    if (m_geom->type_table(i) == item_type_to_select)
      sel_atom(i);
  end_recording();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
}

void geom_view_t::unsel_by_type(const int item_type_to_unselect) {
  app_state_t* astate = app_state_t::get_inst();
  if (!m_geom)
    return;
  begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (auto i = 0; i < m_geom->nat(); i++)
    if (m_geom->type_table(i) == item_type_to_unselect)
      unsel_atom(i);
  end_recording();
  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
}

void geom_view_t::inv_sel_atoms() {
  app_state_t* astate = app_state_t::get_inst();
  if (!m_geom)
    return;
  std::set<int> sel_atm;
  index zero = index::D(m_geom->get_DIM()).all(0);
  begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (int i = 0; i < m_geom->nat(); i++)
    m_geom->toggle_selected(i);
  end_recording();
  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
}

void geom_view_t::sel_by_box(vector3<float> start_pos, vector3<float> end_pos) {
  /**
   * brute force solution
  */
  begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (size_t i = 0; i < m_geom->nat(); i++) {
    aabb_3d_t<float> aabb{start_pos, end_pos};
    auto is_inside_aabb = aabb.test_point(m_geom->pos(i));
    if (is_inside_aabb)
      sel_atom(i);
  }
  end_recording();
}

void geom_view_t::sq_sel_by_box(const float box_scale = 1.1) {
  if (m_geom->num_aselected() != 2)
    return;
  auto idx1 = m_geom->nth_aselected(0);
  auto idx2 = m_geom->nth_aselected(1);
  auto pos1 = m_geom->pos(idx1->m_atm, idx1->m_idx);
  auto pos2 = m_geom->pos(idx2->m_atm, idx2->m_idx);
  vector3<float> center = (pos1 + pos2) * 0.5f;
  vector3<float> len{0};
  for (size_t i = 0; i < 3; i++)
    len[i] = std::max(std::max(pos1[i], pos2[i]) - std::min(pos1[i], pos2[i]), 0.1f);
  auto pos1_r = center - len * 0.5f * box_scale;
  auto pos2_r = center + len * 0.5f * box_scale;
  sel_by_box(pos1_r, pos2_r);
}

void geom_view_t::ins_atom(const int atom_type, const vector3<float> &pos) {
  if (!m_geom)
    return;
  m_anim->m_force_non_animable = true;
  m_geom->add(m_geom->atom_of_type(atom_type), pos);
}

void geom_view_t::ins_atom(const std::string &atom_name, const vector3<float> &pos) {
  if (!m_geom)
    return;
  m_anim->m_force_non_animable = true;
  m_geom->add(atom_name, pos);
}

void geom_view_t::upd_atom(const int at_id, const vector3<float> &pos) {
  if (!m_geom)
    return;
  m_anim->m_force_non_animable = true;
  m_geom->change_pos(at_id, pos);
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
}

void geom_view_t::upd_atom(const int at_id, const std::string &at_name) {
  if (!m_geom)
    return;
  m_anim->m_force_non_animable = true;
  m_geom->change(at_id, at_name, m_geom->pos(at_id));
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
  if (!m_geom)
    return;
}

void geom_view_t::upd_atom(const int at_id, const std::string &at_name, const vector3<float> &pos) {
  if (!m_geom)
    return;
  m_anim->m_force_non_animable = true;
  m_geom->change(at_id, at_name, pos);
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
}

void geom_view_t::transform_atom(const int at_id, const matrix3<float> &tm) {
  if (!m_geom)
    return;
  vector3<float> pos = m_geom->coord(at_id);
  vector3<float> new_pos = tm * pos;
  m_geom->coord(at_id) = new_pos;
}

void geom_view_t::transform_atom(const int at_id, const matrix4<float> &tm) {
  if (!m_geom)
    return;
  vector4<float> p_aff(m_geom->pos(at_id)[0], m_geom->pos(at_id)[1], m_geom->pos(at_id)[2], 1.0f);
  vector4<float> new_p = tm * p_aff;
  vector3<float> new_pos3(new_p[0], new_p[1], new_p[2]);
  m_geom->change_pos(at_id, new_pos3);
}

void geom_view_t::swap_atoms(const size_t at1, const size_t at2, bool swap_names) {
  app_state_t *astate = app_state_t::get_inst();
  std::string atom1_name = swap_names ? m_geom->atom_name(at1) : m_geom->atom_name(at2);
  std::string atom2_name = swap_names ? m_geom->atom_name(at2) : m_geom->atom_name(at1);
  vector3<float> atom1_pos = m_geom->pos(at1);
  vector3<float> atom2_pos = m_geom->pos(at2);
  m_geom->change(at1, atom1_name, atom2_pos);
  m_geom->change(at2, atom2_name, atom1_pos);
  astate->make_viewport_dirty();
}

void geom_view_t::sv_modify_selected(bool state) {

  app_state_t *astate = app_state_t::get_inst();

  for (auto i = 0; i < m_geom->num_selected(); i++) {
    auto rec = m_geom->nth_aselected(i);
    if (rec && (*rec).m_idx.is_zero())
      m_geom->xfield<bool>(xgeom_sel_vis_hide, (*rec).m_atm) = state;
  }

  if (!m_sel_vis.get_value()) {
    m_sel_vis.set_value(true);
    m_sel_vis_affect_bonds.set_value(true);
  }

  astate->make_viewport_dirty();

}

void geom_view_t::sv_hide_invert_selected() {

  app_state_t *astate = app_state_t::get_inst();

  std::unordered_set<size_t> cap_idx;

  //for (auto &elem : m_atom_idx_sel) cap_idx.insert(elem.m_atm);

  for (auto i = 0; i < m_geom->num_selected(); i++) {
    auto rec = m_geom->nth_aselected(i);
    if (rec) cap_idx.insert((*rec).m_atm);
  }

  for (size_t i = 0; i < m_geom->nat(); i++)
    if (cap_idx.find(i) == cap_idx.end())
      m_geom->xfield<bool>(xgeom_sel_vis_hide, i) = true;

  if (!m_sel_vis.get_value()) {
    m_sel_vis.set_value(true);
    m_sel_vis_affect_bonds.set_value(true);
  }

  astate->make_viewport_dirty();

}

void geom_view_t::xbool_invert_selected(size_t field_id) {

  for (auto i = 0; i < m_geom->num_selected(); i++) {
    auto rec = m_geom->nth_aselected(i);
    if (rec && (*rec).m_idx.is_zero())
      m_geom->xfield<bool>(field_id, (*rec).m_atm) = !m_geom->xfield<bool>(field_id, (*rec).m_atm);
  }

}

void geom_view_t::copy_from_xgeom(xgeometry<float, periodic_cell<float> > &xgeom_inst) {
  if (!m_geom)
    return;
  xgeom_inst.clone(*m_geom, true);
}

void geom_view_t::copy_to_xgeom(xgeometry<float, periodic_cell<float> > &xgeom_inst,
                                bool copy_selected, bool copy_cell) {

  if (!m_geom)
    return;

  xgeom_inst.clear();
  if (copy_cell) {
    xgeom_inst.set_DIM(m_geom->get_DIM());
    for (int i = 0; i < 3; i++)
      if (m_geom->get_DIM() > i) xgeom_inst.cell.v[i] = m_geom->cell.v[i];
  }

  if (copy_selected) {
    for (auto i = 0; i < m_geom->num_selected(); i++) {
      auto rec = m_geom->nth_aselected(i);
      if (rec && (*rec).m_idx.is_zero())
        xgeom_inst.add(m_geom->atom((*rec).m_atm), m_geom->pos((*rec).m_atm, (*rec).m_idx));
    }

  } else {
    for (int i = 0; i < m_geom->nat(); i++)
      xgeom_inst.add(m_geom->atom(i), m_geom->pos(i));
  }

}

void geom_view_t::copy_cell(geom_view_t &src, bool rebuild_tree) {

  app_state_t *astate = app_state_t::get_inst();

  if (rebuild_tree) {
    m_tws_tr->do_action(act_lock | act_clear_all);
    m_ext_obs->first_data = true;
  }

  m_geom->set_DIM(src.m_geom->get_DIM());
  for (int i = 0; i < 3; i++)
    if (m_geom->get_DIM() > i) m_geom->cell.v[i] = src.m_geom->cell.v[i];

  if (rebuild_tree) {
    m_tws_tr->do_action(act_unlock | act_rebuild_tree);
    m_tws_tr->do_action(act_rebuild_ntable);
  }

  if (is_selected() && m_parent_ws)
    astate->astate_evd->cur_ws_selected_item_changed();

}

void geom_view_t::set_cell(std::optional<vector3<float> > a,
                           std::optional<vector3<float> > b,
                           std::optional<vector3<float> > c) {

  if (m_geom->get_DIM() == 1 && a) {
    begin_structure_change();
    m_geom->cell.v[0] = *a;
    end_structure_change();
    return;
  }

  if (m_geom->get_DIM() == 2 && a && b) {
    begin_structure_change();
    m_geom->cell.v[0] = *a;
    m_geom->cell.v[1] = *b;
    end_structure_change();
    return;
  }

  if (m_geom->get_DIM() == 3 && a && b && c ) {
    begin_structure_change();
    m_geom->cell.v[0] = *a;
    m_geom->cell.v[1] = *b;
    m_geom->cell.v[2] = *c;
    end_structure_change();
    return;
  }

}

std::shared_ptr<ws_item_t> geom_view_t::clone_on_the_spot() {

  app_state_t *astate = app_state_t::get_inst();

  auto cloned = astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(geom_view_t::get_type_static());
  if (!cloned)
    return nullptr;
  cloned->m_name.set_value(fmt::format("{}_clone", m_name.get_value()));

  auto c_gv = cloned->cast_as<geom_view_t>();
  if (!c_gv)
    return nullptr;

  c_gv->m_tws_tr->do_action(act_lock | act_clear_all);

  copy_to_xgeom(*(c_gv->m_geom));

  c_gv->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
  c_gv->m_tws_tr->do_action(act_rebuild_ntable);

  if (m_parent_ws) {
    m_parent_ws->add_item_to_ws(cloned);
  }

  return cloned;

}

void geom_view_t::load_color_from_static_anim() {

  if (!m_geom) return;

  if (m_anim->m_anim_data.size() > 0) {

    int static_anim = -1;
    for (size_t i = 0; i < m_anim->m_anim_data.size(); i++)
      if (m_anim->m_anim_data[i].m_anim_type == geom_anim_e::anim_static) static_anim = i;

    if (static_anim >= 0 && m_anim->m_anim_data[static_anim].frames.size() > 0 &&
        m_anim->m_anim_data[static_anim].frames[0].atom_color.size() == m_geom->nat()) {

      for (int i = 0; i < m_geom->nat(); i++) {

        m_geom->xfield<float>(xgeom_ccr, i) =
            m_anim->m_anim_data[static_anim].frames[0].atom_color[i][0];
        m_geom->xfield<float>(xgeom_ccg, i) =
            m_anim->m_anim_data[static_anim].frames[0].atom_color[i][1];
        m_geom->xfield<float>(xgeom_ccb, i) =
            m_anim->m_anim_data[static_anim].frames[0].atom_color[i][2];

      }

    }

  }

}

vector3<float> geom_view_t::get_xcolor(const size_t atm) {
  if (atm >= m_geom->nat()) {
    throw std::out_of_range("invalid atom id");
  }
  return vector3<float>{m_geom->xfield<float>(xgeom_ccr, atm),
                        m_geom->xfield<float>(xgeom_ccg, atm),
                        m_geom->xfield<float>(xgeom_ccb, atm)};
}

void geom_view_t::set_xcolorv(const size_t atm, const vector3<float> color) {
  if (atm >= m_geom->nat()) {
    throw std::out_of_range("invalid atom id");
  }
  m_geom->xfield<float>(xgeom_ccr, atm) = color[0];
  m_geom->xfield<float>(xgeom_ccg, atm) = color[1];
  m_geom->xfield<float>(xgeom_ccb, atm) = color[2];
}

void geom_view_t::set_xcolorf(const size_t atm, const float c_r, const float c_g, const float c_b) {
  vector3<float> packed_color{c_r, c_g, c_b};
  set_xcolorv(atm, packed_color);
}

std::tuple<float, float> geom_view_t::get_min_max_xfield(const size_t xfield_id) {

  if (m_geom->field_type(xfield_id) != basic_types::type_real) {
    throw std::invalid_argument(fmt::format("wrong xfield type with id {} ", xfield_id));
  }

  if (m_geom->nat() == 0) {
    throw std::invalid_argument("m_geom->nat() == 0");
  }

  float min_v = m_geom->xfield<float>(xfield_id, 0);
  float max_v = m_geom->xfield<float>(xfield_id, 0);

  for (size_t i = 0; i < m_geom->nat(); i++) {
    float val = m_geom->xfield<float>(xfield_id, i);
    if (min_v > val) min_v = val;
    if (max_v < val) max_v = val;
  }

  return {min_v, max_v};

}

void geom_view_t::sel_atom_ngbs(const int at_id) {
  if (!m_geom)
    return;
  for (int i = 0; i < m_tws_tr->n(at_id); i++)
    if (m_tws_tr->table_idx(at_id, i) == index::D(m_geom->get_DIM()).all(0))
      sel_atom(m_tws_tr->table_atm(at_id, i));
}

void geom_view_t::sel_selected_atoms_ngbs() {
  if (!m_geom)
    return;
  std::set<int> stored_sel;
  for (auto i = 0; i < m_geom->num_selected(); i++) {
    auto rec = m_geom->nth_aselected(i);
    if (rec && (*rec).m_idx.is_zero())
      stored_sel.insert((*rec).m_atm);
  }
  for (auto &rec : stored_sel)
    sel_atom_ngbs(rec);
}

void geom_view_t::update_inter_atomic_dist(float new_dist,
                                           const int at1,
                                           const int at2,
                                           const index id1,
                                           const index id2,
                                           pair_dist_mode_e mode) {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_geom)
    return;
  if (at1 >= m_geom->nat() || at2 >= m_geom->nat())
    return;

  vector3<float> r_btw = (m_geom->pos(at1, id1) + m_geom->pos(at2, id2))*0.5f;
  vector3<float> dir_f = (m_geom->pos(at1, id1) - r_btw).normalized();
  vector3<float> dir_s = (m_geom->pos(at2, id2) - r_btw).normalized();

  switch (mode) {
  case pair_dist_mode_e::transform_both : {
    m_geom->change_pos(at1, r_btw + dir_f * new_dist * 0.5f);
    m_geom->change_pos(at2, r_btw + dir_s * new_dist * 0.5f);
    break;
  }
  case pair_dist_mode_e::fix_first : {
    m_geom->change_pos(at1, r_btw + dir_f * new_dist * 0.5f );
    break;
  }
  case pair_dist_mode_e::fix_second : {
    m_geom->change_pos(at2, r_btw + dir_s * new_dist * 0.5f);
    break;
  }
  }

  astate->make_viewport_dirty();

}

void geom_view_t::update_inter_atomic_dist_ex(float new_dist,
                                              const int at1, const int at2,
                                              pair_dist_mode_e mode) {
  if (!m_geom)
    return;
  m_xgeom_proxy.begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  update_inter_atomic_dist(new_dist, at1, at2, index::D(m_geom->get_DIM()).all(0),
                           index::D(m_geom->get_DIM()).all(0), mode);
  m_xgeom_proxy.end_recording();
}

void geom_view_t::translate_selected(const vector3<float> &t_vec) {

  if (!m_geom)
    return;
  if (m_geom->no_aselected())
    return;

  m_xgeom_proxy.begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);

  for (auto i = 0; i < m_geom->num_selected(); i++) {
    auto rec = m_geom->nth_aselected(i);
    if (rec && (*rec).m_idx.is_zero())
      upd_atom((*rec).m_atm, m_geom->pos((*rec).m_atm) + t_vec);
  }

  m_xgeom_proxy.end_recording();

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_selected_atoms_list_selected_content_changed();

}

void geom_view_t::delete_selected_atoms() {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_geom)
    return;

  if (!m_geom->no_aselected())
    m_anim->m_force_non_animable = true;

  std::vector<int> all_atom_num;
  all_atom_num.reserve(m_geom->num_aselected());
  std::vector<atom_index_set_key> atoms_to_unselect;

  for (auto i = 0; i < m_geom->num_selected(); i++) {
    auto rec = m_geom->nth_aselected(i);
    all_atom_num.push_back((*rec).m_atm);
    m_measure->notify_atom_has_been_deleted((*rec).m_atm);
    atoms_to_unselect.push_back((*rec));
  }

  auto uniq_atoms_last = std::unique(all_atom_num.begin(), all_atom_num.end());
  all_atom_num.erase(uniq_atoms_last, all_atom_num.end());

  //sort by ancending order
  std::sort(all_atom_num.begin(), all_atom_num.end());

  for (auto &elem : atoms_to_unselect)
    m_geom->iselect(elem.m_atm, elem.m_idx, false);

  begin_structure_change();

  for (uint16_t delta = 0; delta < all_atom_num.size(); delta++)
    m_geom->erase(all_atom_num[delta] - delta);

  end_structure_change();

  astate->astate_evd->cur_ws_selected_atoms_list_selected_content_changed();

}

void geom_view_t::delete_atoms(std::set<int> &to_delete) {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_geom)
    return;

  if (!to_delete.empty())
    m_anim->m_force_non_animable = true;

  std::vector<int> all_atom_num;
  all_atom_num.reserve(to_delete.size());

  //get unique selected atoms
  for(auto &elem : to_delete) {
    all_atom_num.push_back(elem);
    m_measure->notify_atom_has_been_deleted(elem);
  }

  auto uniq_atoms_last = std::unique(all_atom_num.begin(), all_atom_num.end());
  all_atom_num.erase(uniq_atoms_last, all_atom_num.end());

  //sort by ancending order
  std::sort(all_atom_num.begin(), all_atom_num.end());

  //m_atom_idx_sel.clear();
  for (auto &elem : all_atom_num)
    m_geom->select(elem, false);

  begin_structure_change();

  for (uint16_t delta = 0; delta < all_atom_num.size(); delta++)
    m_geom->erase(all_atom_num[delta] - delta);

  end_structure_change();

  astate->astate_evd->cur_ws_selected_atoms_list_selected_content_changed();

}

void geom_view_t::begin_structure_change() {
  m_tws_tr->do_action(act_lock | act_clear_all);
  m_ext_obs->first_data = true;
  m_need_to_update_overview = true;
}

void geom_view_t::end_structure_change() {
  geometry_changed();
  m_tws_tr->do_action(act_unlock | act_rebuild_tree);
  m_tws_tr->do_action(act_rebuild_ntable);
  m_need_to_update_overview = true;
}

void geom_view_t::refine_from_frac_coord() {
  begin_structure_change();
  for (size_t i = 0; i < m_geom->nat(); i++)
    m_geom->coord(i) = m_geom->cell.frac2cart(m_geom->pos(i));
  end_structure_change();
}

std::string geom_view_t::compose_type_descr() {
  return m_geom ? fmt::format("geom. view, D = [{}d]", m_geom->get_DIM()) : "geom. view(empty)";
}

std::string geom_view_t::compose_overview() {

  app_state_t *astate = app_state_t::get_inst();

  if (m_geom->num_aselected() < astate->m_gv_overview_max_atoms && !m_geom->no_aselected()) {

    std::map<size_t, size_t> lkp_sel_types;
    bool succes{true};
    size_t tot_sel_types{0};

    for (auto q = 0; q < m_geom->num_selected(); q++) {
      auto rec = m_geom->nth_aselected(q);
      if (!rec) continue;
      auto i = *rec;
      auto it = lkp_sel_types.find(m_geom->type_of_atom(i.m_atm));
      if (it == lkp_sel_types.end()) {
        tot_sel_types++;
        if (tot_sel_types > astate->m_gv_overview_max_sel_types) {
          succes = false;
          break;
        }
        lkp_sel_types[m_geom->type_of_atom(i.m_atm)]++;
      } else {
        it->second++;
      }
    }

    if (succes) {
      std::string selection;
      size_t elemc{0};
      for (auto const &elem : lkp_sel_types) {
        elemc++;
        selection += fmt::format("{}:{}{}", m_geom->atom_of_type(elem.first),
                                 elem.second,
                                 elemc == lkp_sel_types.size() ? "" : ", ");
      }

      return fmt::format("[Selected : [{}]]", selection);

    }

  }

  return fmt::format("[Selected : {}]", m_geom->num_aselected());

}

void geom_view_t::update (float delta_time) {

  app_state_t* astate = app_state_t::get_inst();

  ws_item_t::update(delta_time);
  m_anim->update(delta_time);

  if (m_tws_tr->m_tree_is_dirty) {
    m_tws_tr->m_tree_is_dirty = false;
  }

  if (m_tws_tr->m_atoms_existence_is_broken) {
    m_tws_tr->m_atoms_existence_is_broken = false;
    if (m_selected) astate->astate_evd->cur_ws_selected_item_changed();
  }

  if (m_need_to_update_overview && m_selected && m_parent_ws) {
    m_parent_ws->update_overview(compose_overview());
    m_need_to_update_overview = false;
  }

}

float geom_view_t::get_bb_prescaller() {
  if (m_geom && m_geom->get_DIM() == 3)
    return 1.5f;
  return 1.1f;
}

bool geom_view_t::is_bb_visible() {
  return m_geom && m_geom->get_DIM() == 0;
}

uint32_t geom_view_t::get_num_cnt_selected() {
  return this->m_geom->num_aselected();
}

size_t geom_view_t::get_content_count() {
  return m_geom ? m_geom->nat() : 0;
}

void geom_view_t::on_begin_content_gizmo_translate() {

  std::cout << "geom_view_t::on_begin_content_gizmo_translate()" << std::endl;
  m_xgeom_proxy.set_ignore_changes(true);

  if (!m_geom)
    return;
  std::vector<hist_doc_xgeom_proxy_t<float, periodic_cell<float>>::acts_t> tmp_acts;

  for (auto i = 0; i < m_geom->num_selected(); i++) {
      auto rec = m_geom->nth_aselected(i);
      if (!rec) continue;
      auto it = *rec;
      change_atom_pos_event_t<float> change_atom_pos_ev;
      change_atom_pos_ev.m_atom_idx = it.m_atm;
      change_atom_pos_ev.m_before_apos = m_geom->pos(it.m_atm);
      tmp_acts.push_back(std::move(change_atom_pos_ev));

  }

  m_xgeom_proxy.set_tmp_acts(std::move(tmp_acts));

}

void geom_view_t::apply_intermediate_translate_content(const vector3<float> &pos) {

  app_state_t* astate = app_state_t::get_inst();
  if (!m_geom)
    return;

  bool someone_from_atoms_were_translated = false;

  for (auto i = 0; i < m_geom->num_selected(); i++) {
    auto rec = m_geom->nth_aselected(i);
    if (!rec) continue;
    auto it = *rec;
    vector3<float> acc_pos = m_geom->coord(it.m_atm) + pos;
    m_geom->change_pos(it.m_atm, acc_pos);
    someone_from_atoms_were_translated = true;
  }

  if (someone_from_atoms_were_translated) {
    recalc_gizmo_barycenter();
    astate->astate_evd->cur_ws_selected_atoms_list_selected_content_changed();
  }

}

void geom_view_t::on_end_content_gizmo_translate() {

  m_xgeom_proxy.set_ignore_changes(true);
  m_xgeom_proxy.make_tmp_epoch_permanent();

  std::cout << "geom_view_t::on_end_content_gizmo_translate()" << std::endl;

}

void geom_view_t::recalc_gizmo_barycenter() {
  //barycenter in local frame
  m_gizmo_barycenter = vector3<float>::Zero();

  if (!m_geom->no_aselected() || m_geom->nat() == 0) {

    for (auto i = 0; i < m_geom->num_aselected(); i++) {
      auto rec = m_geom->nth_aselected(i);
      if (!rec) continue;
      auto it = *rec;
      m_gizmo_barycenter += m_geom->pos(it.m_atm, it.m_idx);
    }

    m_gizmo_barycenter /= m_geom->num_aselected();

  }
  else m_gizmo_barycenter = m_aabb.min;

}

const vector3<float> geom_view_t::get_gizmo_content_barycenter() {
  return m_gizmo_barycenter;
}

void geom_view_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

}

void geom_view_t::shift(const vector3<float> shift) {

  if (!m_geom)
    return;

  m_tws_tr->do_action(act_lock);

  for (int i = 0; i < m_geom->nat(); i++)
    m_geom->coord(i) = shift + m_geom->pos(i) ;

  m_ext_obs->aabb.min = shift + m_ext_obs->aabb.min;
  m_ext_obs->aabb.max = shift + m_ext_obs->aabb.max;
  m_tws_tr->apply_shift(shift);

  m_tws_tr->do_action(act_unlock);
  geometry_changed();

}

void geom_view_t::save_to_json(json &data) {

  ws_item_t::save_to_json(data);

  data[JSON_GV_DIM] = m_geom->get_DIM();
  data[JSON_GV_SHOW_IMG_ATOMS] = m_draw_img_atoms.get_value();
  data[JSON_GV_SHOW_IMG_BONDS] = m_draw_img_bonds.get_value();
  data[JSON_GV_SHOW_BONDS] = m_draw_bonds.get_value();
  data[JSON_GV_SHOW_ATOMS] = m_draw_atoms.get_value();
  data[JSON_GV_BT_SHOW_DSBL] = m_bt_show_disabled_record.get_value();
  data[JSON_GV_ATOM_SCALE] = m_atom_scale_factor.get_value();
  data[JSON_GV_BOND_SCALE] = m_bond_scale_factor.get_value();
  data[JSON_GV_RENDER_TYPE] = m_render_style.get_value();
  data[JSON_GV_DRAW_SPECULAR] = m_draw_specular.get_value();
  data[JSON_GV_SPECULAR] = m_shading_specular_power.get_value();

  data[JSON_GV_LABELS_TYPE] = m_labels->m_style.get_value();
  data[JSON_GV_LABELS_SIZE] = m_labels->m_lbl_font_size.get_value();
  data[JSON_GV_LABELS_DRAW_OUTLINE] = m_labels->m_render_outlines.get_value();
  data[JSON_GV_LABELS_INPLACE] = m_labels->m_render_inplace_hud.get_value();
  data[JSON_GV_LABELS_SEL_VIS] = m_labels->m_selective_lbl.get_value();
  data[JSON_GV_LABELS_SCREEN_SPC_SCALE] = m_labels->m_screen_scale.get_value();
  data[JSON_GV_LABELS_INPLACE_OFFSET_X] = m_labels->m_inplace_offset.get_value()[0];
  data[JSON_GV_LABELS_INPLACE_OFFSET_Y] = m_labels->m_inplace_offset.get_value()[1];
  data[JSON_GV_LABELS_OUTLINE_SZ] = m_labels->m_outline_size.get_value();
  data[JSON_GV_ROLE] = m_role.get_value();

  //cell
  data[JSON_GV_PERIODIC_DRAW_CELL] = m_draw_cell.get_value();
  json_io::hs_save_vec3(JSON_GV_PERIODIC_CELL_COLOR, m_cell_color, data);

  //cell vectors
  data[JSON_GV_PERIODIC_DRAW_VECTORS] = m_draw_cell_vectors.get_value();
  data[JSON_GV_PERIODIC_VECTORS_RATIO] = m_cell_vectors_ratio.get_value();
  json_io::hs_save_vec3(JSON_GV_PERIODIC_VECTORS_COLOR, m_cell_vector_color, data);
  json_io::hs_save_vec3(JSON_GV_PERIODIC_VECTORS_OFFSET, m_cell_vector_offset, data);

  data[JSON_GV_BONDING_TABLE] = json::array({});

  data[JSON_GV_SEL_VIS] = m_sel_vis.get_value();
  data[JSON_GV_SEL_VIS_AFFECT_BONDS] = m_sel_vis_affect_bonds.get_value();

  if (m_geom->get_DIM() == 3) {
    data[JSON_GV_PERIODIC_DRAW_SUBCELLS] = m_draw_subcells.get_value();
    json_io::hs_save_vec3(JSON_GV_PERIODIC_SUBCELLS_RANGE, m_subcells_range, data);
  }

  for (auto &record: m_tws_tr->m_bonding_table.m_dist) {
    json bt_rec = json::array({});
    bt_rec.push_back(m_geom->atom_of_type(record.first.m_a));
    bt_rec.push_back(m_geom->atom_of_type(record.first.m_b));
    bt_rec.push_back(record.second.m_bonding_dist);
    bt_rec.push_back(record.second.m_enabled);
    data[JSON_GV_BONDING_TABLE].push_back(bt_rec);
  }

  if (m_geom->get_DIM() > 0) {
    json cell = json::array({});
    for (uint8_t i = 0; i < m_geom->get_DIM(); i++) {
      json cell_data = json::array({});
      for (uint8_t q = 0; q < 3; q++) cell_data.push_back(m_geom->cell.v[i][q]);
      cell.push_back(cell_data);
    }
    data[JSON_GV_CELL] = cell;
  }

  json xfield_names = json::array({});
  for (size_t i = 0; i < m_geom->nfields(); i++)
    xfield_names.push_back(m_geom->field_name(i));

  json xfield_types = json::array({});
  for (size_t i = 0; i < m_geom->nfields(); i++)
    switch (m_geom->field_type(i)) {
    case basic_types::type_bool : {
      xfield_types.push_back("b");
      break;
    }
    case basic_types::type_int : {
      xfield_types.push_back("i");
      break;
    }
    case basic_types::type_real : {
      xfield_types.push_back("r");
      break;
    }
    case basic_types::type_double : {
      xfield_types.push_back("d");
      break;
    }
    case basic_types::type_float : {
      xfield_types.push_back("f");
      break;
    }
    case basic_types::type_string : {
      xfield_types.push_back("s");
      break;
    }
    }

  data[JSON_GV_XFIELD_NAMES] = xfield_names;
  data[JSON_GV_XFIELD_TYPES] = xfield_types;

  data[JSON_GV_ATOMS] = json::array({});
  for (auto q = 0; q < m_geom->nat(); q++) {
    json atom = json::array({});
    for (int i = 0; i < m_geom->nfields(); i++)
      switch (m_geom->field_type(i)) {
      case basic_types::type_bool : {
        atom.push_back(m_geom->xfield<bool>(i, q));
        break;
      }
      case basic_types::type_int : {
        atom.push_back(m_geom->xfield<int>(i, q));
        break;
      }
      case basic_types::type_real : {
        atom.push_back(m_geom->xfield<float>(i, q));
        break;
      }
      case basic_types::type_double : {
        atom.push_back(m_geom->xfield<double>(i, q));
        break;
      }
      case basic_types::type_float : {
        atom.push_back(m_geom->xfield<bool>(i, q));
        break;
      }
      case basic_types::type_string : {
        atom.push_back(m_geom->xfield<std::string>(i, q));
        break;
      }
      }
    data[JSON_GV_ATOMS].push_back(atom);
  }

  if (!m_type_color_override.empty()) {

    data[JSON_GV_TYPE_COLOR_OVERRIDE] = json::array({});

    for (auto &elem : m_type_color_override) {
      json color_override_rec = json::array({});
      color_override_rec.push_back(elem.first);
      color_override_rec.push_back(elem.second[0]);
      color_override_rec.push_back(elem.second[1]);
      color_override_rec.push_back(elem.second[2]);
      data[JSON_GV_TYPE_COLOR_OVERRIDE].push_back(color_override_rec);
    }
  }

  if (m_anim->animable()) {

    json animations = json::array({});

    for (auto &anim : m_anim->m_anim_data) {

      json animation = json::object();
      animation[JSON_GV_ANIMATION_NAME] = anim.m_anim_name;
      animation[JSON_GV_ANIMATION_TYPE] = anim.m_anim_type;
      json frames = json::array();

      for (auto &frame : anim.frames) {

        json frame_chunk = json::array({});
        for (auto &frame_coord : frame.atom_pos) {
          json frame_chunk_coord = json::array({});
          frame_chunk_coord.push_back(frame_coord[0]);
          frame_chunk_coord.push_back(frame_coord[1]);
          frame_chunk_coord.push_back(frame_coord[2]);
          frame_chunk.push_back(frame_chunk_coord);
        }
        frames.push_back(frame_chunk);
      }

      animation[JSON_GV_ANIMATION_FRAMES] = frames;
      animations.push_back(animation);
    }

    data[JSON_GV_ANIMATIONS] = animations;
  }

  m_measure->save_to_json(data);

}

void geom_view_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);

  if (auto val_itr = data.find(JSON_GV_DIM); val_itr != data.end()) {
    m_geom->set_DIM(val_itr.value());
    //m_geom->cell.DIM = m_geom->DIM;
  }

  json_io::hs_load_var(JSON_GV_ATOM_SCALE, m_atom_scale_factor, data);
  json_io::hs_load_var(JSON_GV_BOND_SCALE, m_bond_scale_factor, data);
  json_io::hs_load_var(JSON_GV_SHOW_IMG_ATOMS, m_draw_img_atoms, data);
  json_io::hs_load_var(JSON_GV_RENDER_TYPE, m_render_style, data);
  json_io::hs_load_var(JSON_GV_DRAW_SPECULAR, m_draw_specular, data);
  json_io::hs_load_var(JSON_GV_SPECULAR, m_shading_specular_power, data);
  json_io::hs_load_var(JSON_GV_LABELS_TYPE, m_labels->m_style, data);
  json_io::hs_load_var(JSON_GV_LABELS_SIZE, m_labels->m_lbl_font_size, data);
  json_io::hs_load_var(JSON_GV_LABELS_DRAW_OUTLINE, m_labels->m_render_outlines, data);
  json_io::hs_load_var(JSON_GV_LABELS_INPLACE, m_labels->m_render_inplace_hud, data);
  json_io::hs_load_var(JSON_GV_LABELS_SEL_VIS, m_labels->m_selective_lbl, data);
  json_io::hs_load_var(JSON_GV_LABELS_SCREEN_SPC_SCALE, m_labels->m_screen_scale, data);
  json_io::hs_load_var(JSON_GV_LABELS_OUTLINE_SZ, m_labels->m_outline_size, data);

  //  json_helper::load_var(JSON_GV_LABELS_INPLACE_OFFSET_X,
  //                        m_labels->m_inplace_offset[0],
  //                        data);
  //  json_helper::load_var(JSON_GV_LABELS_INPLACE_OFFSET_Y,
  //                        m_labels->m_inplace_offset[1],
  //                        data);

  json_io::hs_load_var(JSON_GV_SHOW_IMG_BONDS, m_draw_img_bonds, data);
  json_io::hs_load_var(JSON_GV_SHOW_BONDS, m_draw_bonds, data);
  json_io::hs_load_var(JSON_GV_SHOW_ATOMS, m_draw_atoms, data);
  json_io::hs_load_var(JSON_GV_SHOW_IMG_ATOMS, m_draw_img_atoms, data);
  json_io::hs_load_var(JSON_GV_BT_SHOW_DSBL, m_bt_show_disabled_record, data);
  json_io::hs_load_var(JSON_GV_PERIODIC_DRAW_SUBCELLS, m_draw_subcells, data);
  json_io::hs_load_var(JSON_GV_SEL_VIS, m_sel_vis, data);
  json_io::hs_load_var(JSON_GV_SEL_VIS_AFFECT_BONDS, m_sel_vis_affect_bonds, data);
  json_io::hs_load_var(JSON_GV_ROLE, m_role, data);

  //cell
  json_io::hs_load_var(JSON_GV_PERIODIC_DRAW_CELL, m_draw_cell, data);
  json_io::hs_load_vec3(JSON_GV_PERIODIC_CELL_COLOR, m_cell_color, data);

  //cell vectors
  json_io::hs_load_var(JSON_GV_PERIODIC_DRAW_VECTORS, m_draw_cell_vectors, data);
  json_io::hs_load_var(JSON_GV_PERIODIC_VECTORS_RATIO, m_cell_vectors_ratio, data);
  json_io::hs_load_vec3(JSON_GV_PERIODIC_VECTORS_OFFSET, m_cell_vector_offset, data);
  json_io::hs_load_vec3(JSON_GV_PERIODIC_VECTORS_COLOR, m_cell_vector_color, data);

  if (data.find(JSON_GV_PERIODIC_SUBCELLS_RANGE) != data.end()) {
    int sc_a = data[JSON_GV_PERIODIC_SUBCELLS_RANGE][0].get<int>();
    int sc_b = data[JSON_GV_PERIODIC_SUBCELLS_RANGE][1].get<int>();
    int sc_c = data[JSON_GV_PERIODIC_SUBCELLS_RANGE][2].get<int>();
    m_subcells_range.set_value(vector3<int>(sc_a, sc_b, sc_c));
  }

  if (auto val_itr = data.find(JSON_GV_TYPE_COLOR_OVERRIDE); val_itr != data.end()) {

    m_type_color_override.clear();

    for (const auto &rec : val_itr.value()) {

      size_t type_idx = rec[0].get<size_t>();
      float tco_r = rec[1].get<float>();
      float tco_g = rec[2].get<float>();
      float tco_b = rec[3].get<float>();
      m_type_color_override.insert({type_idx, vector3<float>{tco_r, tco_g, tco_b}});

    }

  }

  m_tws_tr->do_action(act_lock | act_clear_all);

  m_ext_obs->first_data = true;

  if (m_geom->get_DIM() > 0) {

    if (auto val_itr = data.find(JSON_GV_CELL); val_itr != data.end()) {

      for (uint8_t i = 0; i < m_geom->get_DIM(); i++) {
        vector3<float> cellv(val_itr.value()[i][0].get<float>(),
                             val_itr.value()[i][1].get<float>(),
                             val_itr.value()[i][2].get<float>());
        m_geom->cell.v[i] = cellv;
      }
    } else {
      m_geom->set_DIM(0);
      //("Cannot load cell data for geom with DIM>0");
    }

  }

  if (auto val_itr = data.find(JSON_GV_XFIELD_NAMES); val_itr != data.end()) {

    std::vector<STRING_EX> fn;
    std::vector<basic_types> ft;

    auto &data_xf_names = val_itr.value();
    std::transform(data_xf_names.begin(), data_xf_names.end(), std::back_inserter(fn),
                   [](auto &_elem)->STRING_EX{return _elem.template get<STRING_EX>();});

    if (auto val_itr2 = data.find(JSON_GV_XFIELD_TYPES); val_itr2 != data.end())
      for (auto &elem : val_itr2.value()) {
        STRING_EX fv = elem.get<STRING_EX>();
        if (fv == "b") ft.push_back(basic_types::type_bool);
        if (fv == "i") ft.push_back(basic_types::type_int);
        if (fv == "r") ft.push_back(basic_types::type_real);
        if (fv == "d") ft.push_back(basic_types::type_double);
        if (fv == "f") ft.push_back(basic_types::type_float);
        if (fv == "s") ft.push_back(basic_types::type_string);

      } else throw std::runtime_error("Invalid xfield types");

    m_geom->set_format(fn, ft);

  }

  if (auto val_itr = data.find(JSON_GV_ATOMS); val_itr != data.end())
    for (const auto &atom : val_itr.value()) {

      m_geom->add(atom[0].get<std::string>(),
                  vector3<float>(atom[1].get<float>(), atom[2].get<float>(), atom[3].get<float>()));

      size_t atom_id = m_geom->nat() - 1;

      for (size_t i = 4; i < atom.size(); i++) {

        switch (m_geom->field_type(i)) {

        case basic_types::type_bool : {
          m_geom->xfield<bool>(i, atom_id) = atom[i].get<bool>();
          break;
        }
        case basic_types::type_int : {
          m_geom->xfield<int>(i, atom_id) = atom[i].get<int>();
          break;
        }
        case basic_types::type_real : {
          m_geom->xfield<float>(i, atom_id) = atom[i].get<float>();
          break;
        }
        case basic_types::type_double : {
          m_geom->xfield<double>(i, atom_id) = atom[i].get<double>();
          break;
        }
        case basic_types::type_float : {
          m_geom->xfield<float>(i, atom_id) = atom[i].get<float>();
          break;
        }
        case basic_types::type_string : {
          m_geom->xfield<std::string>(i, atom_id) = atom[i].get<std::string>();
          break;
        }

        }

      }

    }

  if (auto val_itr = data.find(JSON_GV_BONDING_TABLE); val_itr != data.end()) {

    for (auto &elem : val_itr.value()) {

      int type1 = m_geom->type_of_atom(elem[0].get<std::string>());
      int type2 = m_geom->type_of_atom(elem[1].get<std::string>());
      float dist = elem[2].get<float>();
      bool br_enabled = elem[3].get<bool>();
      m_tws_tr->m_bonding_table.m_dist[sym_key<size_t>(type1, type2)].m_bonding_dist = dist;
      m_tws_tr->m_bonding_table.m_dist[sym_key<size_t>(type1, type2)].m_enabled = br_enabled;

    }

    m_tws_tr->m_bonding_table.update_pair_max_dist_all();

  }

  if (auto val_itr = data.find(JSON_GV_ANIMATIONS); val_itr != data.end()) {

    bool static_anim_found{false};

    for (auto &anim : val_itr.value()) {

      geom_anim_record_t<float> tmp_anim_rec;

      if (auto val_itr2 = anim.find(JSON_GV_ANIMATION_NAME); val_itr2 != anim.end())
        tmp_anim_rec.m_anim_name = val_itr2.value();
      else
        tmp_anim_rec.m_anim_name = "generic1";

      if (auto val_itr2 = anim.find(JSON_GV_ANIMATION_TYPE); val_itr2 != anim.end())
        tmp_anim_rec.m_anim_type = val_itr2.value();

      if (auto val_itr2 = anim.find(JSON_GV_ANIMATION_FRAMES); val_itr2 != anim.end()) {
        tmp_anim_rec.frames.reserve(val_itr2.value().size());

        for (auto &frame : val_itr2.value()) {

          tmp_anim_rec.frames.resize(tmp_anim_rec.frames.size() + 1);
          size_t nf_id = tmp_anim_rec.frames.size() - 1;

          tmp_anim_rec.frames[nf_id].atom_pos.reserve(frame.size());

          for (auto &frame_data : frame) {

            vector3<float> frame_coord;
            frame_coord[0] = frame_data[0].get<float>();
            frame_coord[1] = frame_data[1].get<float>();
            frame_coord[2] = frame_data[2].get<float>();
            //std::cout << frame_coord.to_string_vec() << std::endl;
            tmp_anim_rec.frames[nf_id].atom_pos.push_back(frame_coord);
          }

        }

      }

      if (tmp_anim_rec.m_anim_type == geom_anim_e::anim_static) static_anim_found = true;
      m_anim->m_anim_data.push_back(std::move(tmp_anim_rec));

    }

    if (!static_anim_found) {

      geom_anim_record_t<float> tmp_anim_static;
      tmp_anim_static.m_anim_name = "static";
      tmp_anim_static.m_anim_type = geom_anim_e::anim_static;
      tmp_anim_static.frames.resize(1);
      for (auto i = 0; i < m_geom->nat(); i++)
        tmp_anim_static.frames[0].atom_pos.push_back(m_geom->pos(i));
      m_anim->m_anim_data.insert(m_anim->m_anim_data.begin(), std::move(tmp_anim_static));

    }

  }

  //reading measurement subsystem
  m_measure->load_from_json(data);

  geometry_changed();
  m_tws_tr->do_action(act_unlock | act_build_all);

}

bool geom_view_t::can_be_written_to_json() {
  return true;
}

py::list geom_view_t::get_sel_atoms(int index_offset) {

  py::list ret;
  std::set<size_t> sels;
  for (auto q = 0; q < m_geom->num_selected(); q++) {
    auto rec = m_geom->nth_aselected(q);
    if (!rec) continue;
    auto i = *rec;
    sels.insert(i.m_atm);
  }

  for (size_t i = 0; i < m_geom->nat(); i++)
    if (sels.find(i) != sels.end()) ret.append(i+index_offset);

  return ret;

}

py::list geom_view_t::get_unsel_atoms(int index_offset) {

  py::list ret;
  std::set<size_t> sels;
  for (auto q = 0; q < m_geom->num_selected(); q++) {
    auto rec = m_geom->nth_aselected(q);
    if (!rec) continue;
    auto i = *rec;
    sels.insert(i.m_atm);
  }

  for (size_t i = 0; i < m_geom->nat(); i++)
    if (sels.find(i) == sels.end()) ret.append(i+index_offset);

  return ret;

}

void geom_view_t::copy_settings(geom_view_t *src) {

  if (!src)
    return;

  m_draw_atoms             = src->m_draw_atoms;
  m_draw_bonds             = src->m_draw_bonds;
  m_draw_img_atoms         = src->m_draw_img_atoms;
  m_draw_img_bonds         = src->m_draw_img_bonds;
  m_render_style           = src->m_render_style;
  m_sel_vis                = src->m_sel_vis;
  m_sel_vis_affect_bonds   = src->m_sel_vis_affect_bonds;
  m_atom_scale_factor      = src->m_atom_scale_factor;
  m_bond_scale_factor      = src->m_bond_scale_factor;

  m_labels->m_style         = src->m_labels->m_style;
  m_labels->m_screen_scale  = src->m_labels->m_screen_scale;
  m_labels->m_selective_lbl = src->m_labels->m_selective_lbl;

  m_show_bb                 = src->m_show_bb;

  m_type_color_override     = src->m_type_color_override;
  //m_atom_idx_sel            = src->m_atom_idx_sel;
  m_atom_ord_sel            = src->m_atom_ord_sel;
  m_atom_type_to_hide       = src->m_atom_type_to_hide;
  m_atom_type_to_hide_bond  = src->m_atom_type_to_hide_bond;

}

void geom_view_t::copy_xgeom_aux(geom_view_t *src) {

  if (!src || src->m_geom->nat() != m_geom->nat())
    return;

  for (size_t i = 0; i < m_geom->nat(); i++) {
    m_geom->xfield<bool>(xgeom_sel_vis_hide,i) = src->m_geom->xfield<bool>(xgeom_sel_vis_hide,i);
    m_geom->xfield<bool>(xgeom_override,i)     = src->m_geom->xfield<bool>(xgeom_override,i);
    m_geom->xfield<bool>(xgeom_label_show,i)   = src->m_geom->xfield<bool>(xgeom_label_show,i);
    m_geom->xfield<float>(xgeom_atom_r, i)     = src->m_geom->xfield<float>(xgeom_atom_r, i);
    m_geom->xfield<float>(xgeom_ccr, i)        = src->m_geom->xfield<float>(xgeom_ccr, i);
    m_geom->xfield<float>(xgeom_ccg, i)        = src->m_geom->xfield<float>(xgeom_ccg, i);
    m_geom->xfield<float>(xgeom_ccb, i)        = src->m_geom->xfield<float>(xgeom_ccb, i);
    m_geom->xfield<std::string>(xgeom_label_text, i) =
        src->m_geom->xfield<std::string>(xgeom_label_text, i);
  }

}

void geom_view_t::copy_measurements(geom_view_t *src) {

  if (!src || src->m_geom->nat() != m_geom->nat())
    return;

  for (size_t i = 0; i < src->m_measure->m_dist_recs.get_hs_children_count(); i++) {
    auto msr = src->m_measure->m_dist_recs.get_hs_child_as_array(i);
    auto copy_dist = std::make_shared<msr_bond_rec_t<uint32_t>>(
      msr->m_at1, msr->m_at2, msr->m_idx1, msr->m_idx2);
    m_measure->m_dist_recs.add_hs_child_as_array(msr);
  }

  for (size_t i = 0; i < src->m_measure->m_angle_recs.get_hs_children_count(); i++) {
    auto msr = src->m_measure->m_angle_recs.get_hs_child_as_array(i);
    auto copy_dist = std::make_shared<msr_angle_rec_t<uint32_t>>(
        msr->m_at1, msr->m_at2, msr->m_at3, msr->m_idx1, msr->m_idx2, msr->m_idx3);
    m_measure->m_angle_recs.add_hs_child_as_array(msr);
  }

}

std::string geom_view_t::print_epoch_info(int epoch) {
  return m_xgeom_proxy.print_epoch_info(epoch);
}

hs_result_e geom_view_t::on_epoch_changed(hs_doc_base_t::epoch_t prev_epoch) {
  if (is_selected()) recalc_gizmo_barycenter();
  return hs_result_e::hs_success;
}
