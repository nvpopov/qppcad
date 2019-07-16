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
#include <qppcad/json_helpers.hpp>
#include <qppcad/app_state.hpp>

#include <random>
#include <clocale>

using namespace qpp;
using namespace qpp::cad;

geom_view_t::geom_view_t(): ws_item_t () {

  set_default_flags(ws_item_flags_default |
                    ws_item_flags_support_tr |
                    ws_item_flags_support_sel |
                    ws_item_flags_support_cnt_edit |
                    ws_item_flags_support_render_bb |
                    ws_item_flags_toolbar_extension |
                    ws_item_flags_support_actions |
                    ws_item_flags_support_delete |
                    ws_item_flags_support_clone |
                    ws_item_flags_support_moveto |
                    ws_item_flags_support_rendering |
                    ws_item_flags_support_view_voting |
                    ws_item_flags_cam_target_view);

  m_geom = std::make_shared<xgeometry<float, periodic_cell<float> > >(3,"rg1");

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

  {type_string, //0
   type_real, //1
   type_real, //2
   type_real, //3
   type_real, //4
   type_int, //5
   type_bool, //6
   type_bool, //7
   type_real, //8
   type_real, //9
   type_real, //10
   type_string, //11
   type_bool, // 12
   type_real, // 13
   type_string //14
        });

  m_geom->DIM = 0;
  m_geom->cell.DIM = 0;
  m_geom->auto_update_types = true;

  m_ext_obs = std::make_unique<extents_observer_t<float> >(*m_geom);
  m_tws_tr  = std::make_unique<tws_tree_t<float> >(*m_geom);
  m_tws_tr->do_action(act_unlock);

  m_anim = std::make_shared<geom_view_anim_subsys_t>(*this);
  m_measure = std::make_shared<geom_view_msr_subsys_t>(*this);
  m_labels = std::make_shared<geom_view_labels_subsys_t>(*this);
  m_lat_planes = std::make_shared<geom_view_lat_planes_subsys_t>(*this);
  m_selg = std::make_shared<geom_view_sel_groups_subsys_t>(*this);

}

void geom_view_t::vote_for_view_vectors (vector3<float> &out_look_pos,
                                         vector3<float> &out_look_at) {
  if (m_geom->nat() > 1) {
      out_look_at += (m_ext_obs->aabb.max + m_ext_obs->aabb.min) / 2.0;
      vector3<float> bb_size = m_ext_obs->aabb.max - m_ext_obs->aabb.min;
      float size = bb_size.norm();
      float g_sz_mod = 2.0f;
      out_look_pos +=
          g_sz_mod * m_ext_obs->aabb.max.normalized() * clamp<float>(size, 10.0, 60.0);
    } else out_look_pos += vector3<float>(0.0, 0.0, -5.0);

}

void geom_view_t::target_view(cam_tv_e target_view_src,
                              vector3<float> &look_from,
                              vector3<float> &look_to,
                              vector3<float> &look_up,
                              bool &need_to_update_camera) {

  auto new_target_view = target_view_src;

  if (new_target_view == cam_tv_e::tv_auto) {
      if (m_geom->DIM != 0) new_target_view = cam_tv_e::tv_a;
      else if (m_ext_obs->aabb.center().isApprox(vector3<float>{0}, 0.001f))
        new_target_view = cam_tv_e::tv_x;
      else
        new_target_view = cam_tv_e::tv_cc;
    }

  switch (new_target_view) {

    case cam_tv_e::tv_x : {
        float axis_size = std::max(2.0f, m_ext_obs->aabb.max[0] - m_ext_obs->aabb.min[0]);
        look_from = m_pos - 2.0f*vector3<float>(axis_size, 0.0, 0.0);
        look_to = m_pos;
        look_up = {0.0 , 0.0 , 1.0};
        need_to_update_camera = true;
        break;
      }

    case cam_tv_e::tv_y : {
        float axis_size = std::max(2.0f, m_ext_obs->aabb.max[1] - m_ext_obs->aabb.min[1]);
        look_from = m_pos - 2.0f*vector3<float>(0.0, axis_size, 0.0);
        look_to = m_pos;
        look_up = {0.0, 0.0, 1.0};
        need_to_update_camera = true;
        break;
      }

    case cam_tv_e::tv_z : {
        float axis_size = std::max(2.0f, m_ext_obs->aabb.max[2] - m_ext_obs->aabb.min[2]);
        look_from = m_pos - 2.0f*vector3<float>(0.0, 0.0, axis_size);
        look_to = m_pos;
        look_up = {0.0, 1.0, 0.0};
        need_to_update_camera = true;
        break;
      }

    case cam_tv_e::tv_a : {
        vector3<float> center = 0.5*(m_geom->cell.v[0] + m_geom->cell.v[1] + m_geom->cell.v[2]);
        look_from = m_pos + center - 2.0f * m_geom->cell.v[0];
        look_to = m_pos  + center;
        look_up = {0.0 , 0.0 , 1.0};
        need_to_update_camera = true;
        break;
      }

    case cam_tv_e::tv_b : {
        vector3<float> center = 0.5*(m_geom->cell.v[0] + m_geom->cell.v[1] + m_geom->cell.v[2]);
        look_from = m_pos + center - 2.0f * m_geom->cell.v[1];
        look_to = m_pos  + center;
        look_up = {0.0, 0.0, 1.0};
        need_to_update_camera = true;
        break;
      }

    case cam_tv_e::tv_c : {
        vector3<float> center = 0.5*(m_geom->cell.v[0] + m_geom->cell.v[1] + m_geom->cell.v[2]);
        look_from = m_pos + center - 2.0f * m_geom->cell.v[2];
        look_to = m_pos  + center;
        look_up = {0.0, 1.0, 0.0};
        need_to_update_camera = true;
        break;
      }

    case cam_tv_e::tv_cc : {
        float axis_size = std::max(2.0f, m_ext_obs->aabb.max[0] - m_ext_obs->aabb.min[0]);
        look_from = m_pos - axis_size*(m_ext_obs->aabb.max - m_ext_obs->aabb.min).normalized();
        look_to = m_pos + (m_ext_obs->aabb.max + m_ext_obs->aabb.min) * 0.5;
        look_up = {0.0 , 0.0 , 1.0};
        need_to_update_camera = true;
        break;
      }

    default : {
        break;
      }

    }

}

void geom_view_t::geometry_changed () {
  if (m_ext_obs)
    m_aabb = m_ext_obs->aabb;
}

void geom_view_t::render () {

  ws_item_t::render();

  if (!m_geom) return;

  app_state_t* astate = app_state_t::get_inst();
  vector3<float> _pos = m_pos;
  index all_null = index::D(m_geom->DIM).all(0);

  if (astate->dp) {

      if (astate->m_debug_show_tws_tree) {
          astate->dp->begin_render_aabb();
          m_tws_tr->apply_visitor( [astate, _pos](tws_node_t<float> *in_node,
                                   int deep_level){
              astate->dp->render_aabb(clr_maroon,
                                      in_node->m_bb.min+_pos, in_node->m_bb.max+_pos);});
          astate->dp->end_render_aabb();
        }

      if (!m_is_visible) return;

      if (m_geom->DIM == 3 && m_is_visible && m_draw_cell) {
          astate->dp->begin_render_line();

          if (m_draw_subcells) {
              vector3<float> sc_a = m_geom->cell.v[0] / m_subcells_range[0];
              vector3<float> sc_b = m_geom->cell.v[1] / m_subcells_range[1];
              vector3<float> sc_c = m_geom->cell.v[2] / m_subcells_range[2];
              for (int i_a = 0; i_a < m_subcells_range[0]; i_a++)
                for (int i_b = 0; i_b < m_subcells_range[1]; i_b++)
                  for (int i_c = 0; i_c < m_subcells_range[2]; i_c++) {
                      vector3<float> new_pos = m_pos + sc_a * i_a + sc_b * i_b + sc_c * i_c ;
                      astate->dp->render_cell_3d(m_subcell_color, sc_a, sc_b, sc_c, new_pos);
                    }
            }

          vector3<float> cell_clr = m_cell_color;
          if (m_selected) {
              if(m_parent_ws->m_edit_type == ws_edit_e::edit_item) cell_clr = clr_red;
              if(m_parent_ws->m_edit_type == ws_edit_e::edit_content) cell_clr = clr_maroon;
            }

          astate->dp->render_cell_3d(
                cell_clr, m_geom->cell.v[0], m_geom->cell.v[1], m_geom->cell.v[2], m_pos);

          astate->dp->end_render_line();
        }

      if (m_geom->DIM == 3 && m_draw_cell_vectors) {
          astate->dp->begin_render_general_mesh();
          for (size_t i = 0; i < m_geom->DIM; i++) {
              vector3<float> cell_v = m_geom->cell.v[i] * m_cell_vectors_ratio;
              astate->dp->render_arrow(m_pos + m_cell_vector_offset,
                                       m_pos + m_cell_vector_offset + cell_v,
                                       m_cell_vector_color,
                                       0.1f, 0.17f, 0.25f, false);
            }
          astate->dp->end_render_general_mesh();
        }

      switch (m_render_style) {

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

  if (is_selected()) astate->astate_evd->cur_ws_selected_item_changed();

}

bool geom_view_t::mouse_click (ray_t<float> *click_ray) {

  app_state_t* astate = app_state_t::get_inst();

  if (click_ray && m_geom && m_parent_ws) {

      std::vector<tws_query_data_t<float, size_t> > res;

      ray_t<float> local_geom_ray;
      local_geom_ray.start = click_ray->start - m_pos;
      local_geom_ray.dir = click_ray->dir;

      if (m_draw_img_atoms)
        m_tws_tr->query_ray<query_ray_add_all<float> >(local_geom_ray,
                                                       res,
                                                       m_atom_type_to_hide,
                                                       m_atom_scale_factor,
                                                       m_sel_vis,
                                                       xgeom_sel_vis);
      else
        m_tws_tr->query_ray<query_ray_add_ignore_img<float> >(local_geom_ray,
                                                              res,
                                                              m_atom_type_to_hide,
                                                              m_atom_scale_factor,
                                                              m_sel_vis,
                                                              xgeom_sel_vis);
      recalc_gizmo_barycenter();

      if (!res.empty()) {

          std::sort(res.begin(), res.end(), &tws_query_data_sort_by_dist<float>);

          if (m_parent_ws->m_edit_type == ws_edit_e::edit_content && m_selected ) {

              atom_index_set_key iskey(int(res[0].m_atm), res[0].m_idx);
              auto atom_sel_it = m_atom_idx_sel.find(iskey);
              if (atom_sel_it == m_atom_idx_sel.end())
                sel_atom(res[0].m_atm, res[0].m_idx);
              else
                unsel_atom(res[0].m_atm, res[0].m_idx);

            }

          recalc_gizmo_barycenter();
          m_parent_ws->m_gizmo->update_gizmo(0.01f);
          astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
          return true;

        } else {

          if (m_parent_ws->m_edit_type == ws_edit_e::edit_content && m_selected ) {
              sel_atoms(false);
            }

        }

    }

  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
  return false;

}

void geom_view_t::mouse_double_click(ray_t<float> *ray) {

  app_state_t* astate = app_state_t::get_inst();

  if (is_selected() && m_parent_ws && m_parent_ws->m_edit_type == ws_edit_e::edit_item) {
      m_parent_ws->set_edit_type(ws_edit_e::edit_content);
      astate->astate_evd->obj_insp_tab_open_requested(3);
    }

}

void geom_view_t::sel_atoms (bool all) {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_geom) return;

  if (all) {
      m_atom_idx_sel.clear();
      for (auto i = 0; i < m_geom->nat(); i++) sel_atom(i);
    } else {
      m_atom_idx_sel.clear();
      m_atom_ord_sel.clear();
    }

  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
  astate->make_viewport_dirty();

}

void geom_view_t::sel_atom (int atom_id) {

  if (!m_geom) return;
  sel_atom(atom_id, index::D(m_geom->DIM).all(0));

}

void geom_view_t::toggle_atom_sel(int atom_id) {

  auto it_0 = m_atom_idx_sel.find({atom_id, index::D(m_geom->DIM).all(0)});
  if (it_0 != m_atom_idx_sel.end()) {
      unsel_atom(atom_id);
    } else {
      sel_atom(atom_id);
    }

}

void geom_view_t::sel_atom(int atom_id, index atom_idx) {

  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();

  if (!m_geom) return;

  if (atom_id >= 0 && atom_id < m_geom->nat()) {
      m_atom_idx_sel.insert(atom_index_set_key(atom_id, atom_idx));
      if (m_atom_ord_sel.size() >= max_sel_in_deque) {
          m_atom_ord_sel.resize(max_sel_in_deque);
          m_atom_ord_sel.pop_front();
        }
      m_atom_ord_sel.push_back(atom_index_set_key(atom_id, atom_idx));

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

void geom_view_t::unsel_atom(int atom_id) {

  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();

  if (!m_geom) return;

  if (atom_id >= 0 && atom_id < m_geom->nat()) {

      for (iterator idx(index::D(m_geom->DIM).all(-1), index::D(m_geom->DIM).all(1));
           !idx.end(); idx++ ) {
          auto key = atom_index_set_key(atom_id, idx);

          auto i2 = std::find(m_atom_idx_sel.begin(), m_atom_idx_sel.end(), key);
          if (i2 != m_atom_idx_sel.end()) m_atom_idx_sel.erase(i2);

          auto it_ordered = std::find(m_atom_ord_sel.begin(), m_atom_ord_sel.end(), key);
          if (it_ordered != m_atom_ord_sel.end()) m_atom_ord_sel.erase(it_ordered);
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
  if (!m_geom) return;

  if (atom_id >= 0 && atom_id < m_geom->nat()) {
      auto key = atom_index_set_key(atom_id, atom_idx);
      auto i2 = std::find(m_atom_idx_sel.begin(), m_atom_idx_sel.end(), key);
      if (i2 != m_atom_idx_sel.end()) m_atom_idx_sel.erase(i2);

      auto it_ordered = std::find(m_atom_ord_sel.begin(), m_atom_ord_sel.end(), key);
      if (it_ordered != m_atom_ord_sel.end()) m_atom_ord_sel.erase(it_ordered);

      recalc_gizmo_barycenter();
      m_parent_ws->m_gizmo->update_gizmo(0.01f);

      //      for (int i = 0; i < m_atom_ord_sel.size(); i++)
      //        astate->log(fmt::format("{} {} {}", i, m_atom_ord_sel[i].m_atm, m_atom_ord_sel[i].m_idx));

      return;
    }

  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);

  //  for (int i = 0; i < m_atom_ord_sel.size(); i++)
  //    astate->log(fmt::format("{} {} {}", i, m_atom_ord_sel[i].m_atm, m_atom_ord_sel[i].m_idx));

  return;

}

void geom_view_t::sel_by_type (const int item_type_to_select) {

  app_state_t* astate = app_state_t::get_inst();
  if (!m_geom) return;

  for (auto i = 0; i < m_geom->nat(); i++)
    if (m_geom->type_table(i) == item_type_to_select) sel_atom(i);

  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();

}

void geom_view_t::unsel_by_type(const int item_type_to_unselect) {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_geom) return;

  for (auto i = 0; i < m_geom->nat(); i++)

    if (m_geom->type_table(i) == item_type_to_unselect) unsel_atom(i);

  recalc_gizmo_barycenter();

  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();

}

void geom_view_t::inv_sel_atoms () {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_geom) return;

  std::set<int> sel_atm;
  index zero = index::D(m_geom->DIM).all(0);

  for (auto &rec : m_atom_idx_sel)
    if (rec.m_idx == zero) sel_atm.insert(rec.m_atm);

  m_atom_idx_sel.clear();

  for (int i = 0 ; i < m_geom->nat(); i++)
    if (sel_atm.find(i) == sel_atm.end()) sel_atom(i);

  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();

}

void geom_view_t::ins_atom (const int atom_type, const vector3<float> &pos) {

  if (!m_geom) return;

  m_anim->m_force_non_animable = true;
  m_geom->add(m_geom->atom_of_type(atom_type), pos);

}

void geom_view_t::ins_atom (const std::string &atom_name, const vector3<float> &pos) {

  if (!m_geom) return;
  m_anim->m_force_non_animable = true;
  m_geom->add(atom_name, pos);

}

void geom_view_t::upd_atom (const int at_id, const vector3<float> &pos) {

  if (!m_geom) return;
  m_anim->m_force_non_animable = true;
  m_geom->change_pos(at_id, pos);
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();

}

void geom_view_t::upd_atom (const int at_id, const std::string &at_name) {

  if (!m_geom) return;
  m_anim->m_force_non_animable = true;
  m_geom->change(at_id, at_name, m_geom->pos(at_id));
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
  if (!m_geom) return;

}

void geom_view_t::upd_atom(const int at_id,
                           const std::string &at_name,
                           const vector3<float> &pos) {

  if (!m_geom) return;
  m_anim->m_force_non_animable = true;
  m_geom->change(at_id, at_name, pos);
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();

}

void geom_view_t::transform_atom(const int at_id,
                                 const matrix3<float> &tm) {

  if (!m_geom) return;
  vector3<float> pos = m_geom->coord(at_id);
  vector3<float> new_pos = tm * pos;
  m_geom->coord(at_id) = new_pos;

}

void geom_view_t::transform_atom(const int at_id,
                                 const matrix4<float> &tm) {

  if (!m_geom) return;
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

void geom_view_t::flip_atom_in_cell(size_t at_id,
                                    size_t dim_id,
                                    float flip_magn,
                                    bool rebuild_tree) {

  app_state_t *astate = app_state_t::get_inst();

  if (at_id >= m_geom->nat() || dim_id > m_geom->DIM) return;

  if (rebuild_tree) begin_structure_change();

  auto as_frac = m_geom->cell.cart2frac(m_geom->pos(at_id));
  as_frac[dim_id] = as_frac[dim_id] + flip_magn;
  auto as_cart = m_geom->cell.frac2cart(as_frac);
  m_geom->coord(at_id) = as_cart;

  if (rebuild_tree) end_structure_change();

  //astate->tlog("@DEBUG: flip_atom_in_cell");

  astate->make_viewport_dirty();

}

void geom_view_t::flip_sel_atoms_in_cell(size_t dim_id, float flip_magn) {

  begin_structure_change();

  index zero = index::D(m_geom->DIM).all(0);

  for (auto &sel : m_atom_idx_sel)
    if (sel.m_idx == zero)
      flip_atom_in_cell(sel.m_atm, dim_id, flip_magn, false);

  end_structure_change();

}

void geom_view_t::align_atoms_to_point(vector3<float> fpoint) {

  begin_structure_change();

  for (size_t i = 0; i < m_geom->nat(); i++) {

      float min_dist = 100.0f;
      auto goal_vector = m_geom->pos(i);
      for (iterator idx(index::D(m_geom->DIM).all(-1),
                        index::D(m_geom->DIM).all(1)); !idx.end(); idx++ ) {
          auto t_pos_cf = m_geom->cell.transform(m_geom->pos(i), idx);
          auto dist = (fpoint - t_pos_cf).norm();
          if (dist < min_dist && m_geom->cell.within(t_pos_cf)) {
              min_dist = dist;
              //min_dist_index = i;
              goal_vector = t_pos_cf;
            }
        }

      m_geom->coord(i) = goal_vector;

    }

  end_structure_change();

}

void geom_view_t::sv_modify_selected(bool state) {

  app_state_t *astate = app_state_t::get_inst();

  for (auto &rec : m_atom_idx_sel)
    if (rec.m_idx == index::D(m_geom->DIM).all(0))
      m_geom->xfield<bool>(xgeom_sel_vis, rec.m_atm) = state;

  if (!m_sel_vis) {
      m_sel_vis = true;
      m_sel_vis_affect_bonds = true;
    }

  astate->make_viewport_dirty();

}

void geom_view_t::sv_hide_invert_selected() {

  app_state_t *astate = app_state_t::get_inst();

  std::unordered_set<size_t> cap_idx;

  for (auto &elem : m_atom_idx_sel) cap_idx.insert(elem.m_atm);

  for (size_t i = 0; i < m_geom->nat(); i++)
    if (cap_idx.find(i) == cap_idx.end())
      m_geom->xfield<bool>(xgeom_sel_vis, i) = true;

  if (!m_sel_vis) {
      m_sel_vis = true;
      m_sel_vis_affect_bonds = true;
    }

  astate->make_viewport_dirty();

}

void geom_view_t::xbool_invert_selected(size_t field_id) {

  index zero = index::D(m_geom->DIM).all(0);
  for (auto &elem : m_atom_idx_sel)
    if (elem.m_idx == zero)
      m_geom->xfield<bool>(field_id, elem.m_atm) = !m_geom->xfield<bool>(field_id, elem.m_atm);

}

void geom_view_t::copy_from_xgeom(xgeometry<float, periodic_cell<float> > &xgeom_inst) {

  if (!m_geom) return;
  for (int i = 0; i < xgeom_inst.nat(); i++) {
      m_geom->add(xgeom_inst.atom(i), xgeom_inst.pos(i));
      m_geom->xfield<float>(xgeom_charge, i) = xgeom_inst.xfield<float>(xgeom_charge, i);
    }

}

void geom_view_t::copy_to_xgeom(xgeometry<float, periodic_cell<float> > &xgeom_inst,
                                bool copy_selected,
                                bool copy_cell) {
  if (!m_geom) return;

  xgeom_inst.clear();

  if (copy_cell) {
      xgeom_inst.DIM = m_geom->DIM;
      xgeom_inst.cell.DIM = m_geom->cell.DIM;

      for (int i = 0; i < 3; i++)
        if (m_geom->DIM > i) xgeom_inst.cell.v[i] = m_geom->cell.v[i];
    }

  if (copy_selected) {
      for (auto &sel : m_atom_idx_sel)
        xgeom_inst.add(m_geom->atom(sel.m_atm), m_geom->pos(sel.m_atm, sel.m_idx));
    } else {
      for (int i = 0; i < m_geom->nat(); i++)
        xgeom_inst.add(m_geom->atom(i), m_geom->pos(i));
    }

}

void geom_view_t::copy_cell(geom_view_t &src, bool rebuild_tws_tree) {

  app_state_t *astate = app_state_t::get_inst();

  if (rebuild_tws_tree) {
      m_tws_tr->do_action(act_lock | act_clear_all);
      m_ext_obs->first_data = true;
    }

  m_geom->DIM = src.m_geom->DIM;
  m_geom->cell.DIM = src.m_geom->cell.DIM;

  for (int i = 0; i < 3; i++)
    if (m_geom->DIM > i) m_geom->cell.v[i] = src.m_geom->cell.v[i];

  if (rebuild_tws_tree) {
      m_tws_tr->do_action(act_unlock | act_rebuild_tree);
      m_tws_tr->do_action(act_rebuild_ntable);
    }

  if (is_selected()) astate->astate_evd->cur_ws_selected_item_changed();

}

std::shared_ptr<ws_item_t> geom_view_t::clone_on_the_spot() {

  app_state_t *astate = app_state_t::get_inst();

  auto cloned = astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(geom_view_t::get_type_static());
  if (!cloned) return nullptr;
  cloned->m_name = fmt::format("{}_clone", m_name);

  auto c_gv = cloned->cast_as<geom_view_t>();
  if (!c_gv) return nullptr;

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
        if (m_anim->m_anim_data[i].m_anim_type == geom_anim_t::anim_static) static_anim = i;

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

void geom_view_t::set_xcolorf(const size_t atm,
                              const float _r,
                              const float _g,
                              const float _b) {

  vector3<float> packed_color{_r, _g, _b};
  set_xcolorv(atm, packed_color);

}

void geom_view_t::colorize_by_xfield(const vector3<float> color_low,
                                     const vector3<float> color_high,
                                     const size_t xfield_id) {

  auto field_min_max = get_min_max_xfield(xfield_id);

//  bool has_static_anim{false};
  opt<size_t> static_anim_id;

  for (size_t i = 0; i < m_geom->nat(); i++) {

      float el_val = std::get<1>(field_min_max) - m_geom->xfield<float>(xfield_id, i);
      float len = std::get<1>(field_min_max) - std::get<0>(field_min_max);
      vector3<float> color_interp = color_low + (color_high - color_low) * (1 - el_val / len);

      m_geom->xfield<float>(xgeom_ccr, i) = color_interp[0];
      m_geom->xfield<float>(xgeom_ccg, i) = color_interp[1];
      m_geom->xfield<float>(xgeom_ccb, i) = color_interp[2];

      //TODO: propagate colors to first static anim

    }

}

std::tuple<float, float> geom_view_t::get_min_max_xfield(const size_t xfield_id) {

  if (m_geom->field_type(xfield_id) != type_real) {
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

  if (!m_geom) return;

  for (int i = 0; i < m_tws_tr->n(at_id); i++)
    if (m_tws_tr->table_idx(at_id, i) == index::D(m_geom->DIM).all(0))
      sel_atom(m_tws_tr->table_atm(at_id, i));

}

void geom_view_t::sel_selected_atoms_ngbs() {

  if (!m_geom) return;

  std::set<int> stored_sel;
  for (auto &rec : m_atom_idx_sel)
    if (rec.m_idx == index::D(m_geom->DIM).all(0)) stored_sel.insert(rec.m_atm);

  for (auto &rec : stored_sel) sel_atom_ngbs(rec);

}

void geom_view_t::update_inter_atomic_dist(float new_dist,
                                           const int at1,
                                           const int at2,
                                           const index id1,
                                           const index id2,
                                           pair_dist_mode_e mode) {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_geom) return;

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

void geom_view_t::update_inter_atomic_dist(float new_dist,
                                           const int at1,
                                           const int at2,
                                           pair_dist_mode_e mode) {

  if (!m_geom) return;

  update_inter_atomic_dist(new_dist, at1, at2,
                           index::D(m_geom->DIM).all(0), index::D(m_geom->DIM).all(0), mode);

}

void geom_view_t::translate_selected (const vector3<float> &t_vec) {

  if (!m_geom) return;

  for (auto &elem : m_atom_idx_sel)
    if (elem.m_idx == index::D(m_geom->DIM).all(0))
      upd_atom(elem.m_atm, m_geom->pos(elem.m_atm) + t_vec);
  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_selected_atoms_list_selected_content_changed();

}

void geom_view_t::delete_selected_atoms () {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_geom) return;

  if (!m_atom_idx_sel.empty()) m_anim->m_force_non_animable = true;

  std::vector<int> all_atom_num;
  all_atom_num.reserve(m_atom_idx_sel.size());

  //get unique selected atoms
  for(auto &elem : m_atom_idx_sel) {
      all_atom_num.push_back(elem.m_atm);
      m_measure->notify_atom_has_been_deleted(elem.m_atm);
    }

  auto uniq_atoms_last = std::unique(all_atom_num.begin(), all_atom_num.end());
  all_atom_num.erase(uniq_atoms_last, all_atom_num.end());

  //sort by ancending order
  std::sort(all_atom_num.begin(), all_atom_num.end());

  m_atom_idx_sel.clear();

  for (uint16_t delta = 0; delta < all_atom_num.size(); delta++) {
      if (delta == 0 && all_atom_num.size() > 1)
        m_tws_tr->do_action(act_lock);
      if ((delta == all_atom_num.size() - 1) && all_atom_num.size() > 1)
        m_tws_tr->do_action(act_unlock);
      m_geom->erase(all_atom_num[delta] - delta);
    }

  astate->astate_evd->cur_ws_selected_atoms_list_selected_content_changed();

}

void geom_view_t::delete_atoms(std::set<int> &to_delete) {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_geom) return;

  if (!to_delete.empty()) m_anim->m_force_non_animable = true;

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

  m_atom_idx_sel.clear();

  for (uint16_t delta = 0; delta < all_atom_num.size(); delta++) {
      if (delta == 0 && all_atom_num.size() > 1)
        m_tws_tr->do_action(act_lock);
      if ((delta == all_atom_num.size() - 1) && all_atom_num.size() > 1)
        m_tws_tr->do_action(act_unlock);
      m_geom->erase(all_atom_num[delta] - delta);
    }

  astate->astate_evd->cur_ws_selected_atoms_list_selected_content_changed();

}

void geom_view_t::begin_structure_change() {

  m_tws_tr->do_action(act_lock | act_clear_all);
  m_ext_obs->first_data = true;

}

void geom_view_t::end_structure_change() {

  geometry_changed();
  m_tws_tr->do_action(act_unlock | act_rebuild_tree);
  m_tws_tr->do_action(act_rebuild_ntable);

}

void geom_view_t::refine_from_frac_coord() {

  begin_structure_change();

  for (size_t i = 0; i < m_geom->nat(); i++)
    m_geom->coord(i) = m_geom->cell.frac2cart(m_geom->pos(i));

  end_structure_change();

}

std::string geom_view_t::compose_type_descr () {
  return m_geom ? fmt::format("geom. view, DIM = [{}d]", m_geom->DIM) : "geom. view(empty)";
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

}

float geom_view_t::get_bb_prescaller () {
  if (m_geom && m_geom->DIM == 3) return 1.5f;
  return 1.1f;
}

bool geom_view_t::is_bb_visible() {
  return m_geom && m_geom->DIM == 0;
}

uint32_t geom_view_t::get_num_cnt_selected () {
  return this->m_atom_idx_sel.size();
}

size_t geom_view_t::get_content_count () {
  return m_geom ? m_geom->nat() : 0;
}

void geom_view_t::on_begin_content_gizmo_translate () {
  //c_app::log(fmt::format("Start of translating node [{}] content", m_name));
  //m_tws_tr->do_action(act_lock);
}

void geom_view_t::apply_intermediate_translate_content (const vector3<float> &pos) {

  app_state_t* astate = app_state_t::get_inst();
  if (!m_geom) return;

  bool someone_from_atoms_were_translated = false;
  for (auto &it : m_atom_idx_sel) {
      vector3<float> acc_pos = m_geom->coord(it.m_atm) + pos;
      m_geom->change_pos(it.m_atm, acc_pos);
      someone_from_atoms_were_translated = true;
    }
  if (someone_from_atoms_were_translated) {
      recalc_gizmo_barycenter();
      astate->astate_evd->cur_ws_selected_atoms_list_selected_content_changed();
    }

}

void geom_view_t::on_end_content_gizmo_translate () {
  //c_app::log(fmt::format("End of translating node [{}] content", m_name));
  //m_tws_tr->do_action(act_unlock);
}

void geom_view_t::recalc_gizmo_barycenter () {
  //barycenter in local frame
  m_gizmo_barycenter = vector3<float>::Zero();

  if (!m_atom_idx_sel.empty() || m_geom->nat() == 0) {
      for (const auto& atm_idx : m_atom_idx_sel)
        m_gizmo_barycenter += m_geom->pos(atm_idx.m_atm, atm_idx.m_idx);
      m_gizmo_barycenter /= m_atom_idx_sel.size();
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

  if (!m_geom) return;

  m_tws_tr->do_action(act_lock);

  for (int i = 0; i < m_geom->nat(); i++)
    m_geom->coord(i) = shift + m_geom->pos(i) ;

  m_ext_obs->aabb.min = shift + m_ext_obs->aabb.min;
  m_ext_obs->aabb.max = shift + m_ext_obs->aabb.max;
  m_tws_tr->apply_shift(shift);

  m_tws_tr->do_action(act_unlock);
  geometry_changed();

}

void geom_view_t::save_to_json (json &data) {

  ws_item_t::save_to_json(data);

  data[JSON_GEOM_VIEW_DIM] = m_geom->DIM;
  data[JSON_GEOM_VIEW_SHOW_IMG_ATOMS] = m_draw_img_atoms;
  data[JSON_GEOM_VIEW_SHOW_IMG_BONDS] = m_draw_img_bonds;
  data[JSON_GEOM_VIEW_SHOW_BONDS] = m_draw_bonds;
  data[JSON_GEOM_VIEW_SHOW_ATOMS] = m_draw_atoms;
  data[JSON_GEOM_VIEW_BT_SHOW_DSBL] = m_bt_show_disabled_record;
  data[JSON_GEOM_VIEW_ATOM_SCALE] = m_atom_scale_factor;
  data[JSON_GEOM_VIEW_BOND_SCALE] = m_bond_scale_factor;
  data[JSON_GEOM_VIEW_RENDER_TYPE] = m_render_style;
  data[JSON_GEOM_VIEW_DRAW_SPECULAR] = m_draw_specular;
  data[JSON_GEOM_VIEW_SPECULAR] = m_shading_specular_power;

  data[JSON_GEOM_VIEW_LABELS_TYPE] = m_labels->m_style;
  data[JSON_GEOM_VIEW_LABELS_SIZE] = m_labels->m_lbl_font_size;
  data[JSON_GEOM_VIEW_LABELS_DRAW_OUTLINE] = m_labels->m_render_outlines;
  data[JSON_GEOM_VIEW_LABELS_INPLACE] = m_labels->m_render_inplace_hud;
  data[JSON_GEOM_VIEW_LABELS_SEL_VIS] = m_labels->m_selective_lbl;
  data[JSON_GEOM_VIEW_LABELS_SCREEN_SPC_SCALE] = m_labels->m_screen_scale;
  data[JSON_GEOM_VIEW_LABELS_INPLACE_OFFSET_X] = m_labels->m_inplace_offset[0];
  data[JSON_GEOM_VIEW_LABELS_INPLACE_OFFSET_Y] = m_labels->m_inplace_offset[1];
  data[JSON_GEOM_VIEW_LABELS_OUTLINE_SZ] = m_labels->m_outline_size;
  data[JSON_GEOM_VIEW_ROLE] = m_role;

  //cell
  data[JSON_GEOM_VIEW_PERIODIC_DRAW_CELL] = m_draw_cell;
  json_helper::save_vec3(JSON_GEOM_VIEW_PERIODIC_CELL_COLOR, m_cell_color, data);

  //cell vectors
  data[JSON_GEOM_VIEW_PERIODIC_DRAW_VECTORS] = m_draw_cell_vectors;
  data[JSON_GEOM_VIEW_PERIODIC_VECTORS_RATIO] = m_cell_vectors_ratio;
  json_helper::save_vec3(JSON_GEOM_VIEW_PERIODIC_VECTORS_COLOR, m_cell_vector_color, data);
  json_helper::save_vec3(JSON_GEOM_VIEW_PERIODIC_VECTORS_OFFSET, m_cell_vector_offset, data);

  data[JSON_GEOM_VIEW_BONDING_TABLE] = json::array({});

  data[JSON_GEOM_VIEW_SEL_VIS] = m_sel_vis;
  data[JSON_GEOM_VIEW_SEL_VIS_AFFECT_BONDS] = m_sel_vis_affect_bonds;

  if (m_geom->DIM == 3) {
      data[JSON_GEOM_VIEW_PERIODIC_DRAW_SUBCELLS] = m_draw_subcells;
      data[JSON_GEOM_VIEW_PERIODIC_SUBCELLS_RANGE] =
          json::array({m_subcells_range[0], m_subcells_range[1], m_subcells_range[2]});
    }

  for (auto &record: m_tws_tr->m_bonding_table.m_dist) {
      json bt_rec = json::array({});
      bt_rec.push_back(m_geom->atom_of_type(record.first.m_a));
      bt_rec.push_back(m_geom->atom_of_type(record.first.m_b));
      bt_rec.push_back(record.second.m_bonding_dist);
      bt_rec.push_back(record.second.m_enabled);
      data[JSON_GEOM_VIEW_BONDING_TABLE].push_back(bt_rec);
    }

  if (m_geom->DIM > 0) {
      json cell = json::array({});
      for (uint8_t i = 0; i < m_geom->DIM; i++) {
          json cell_data = json::array({});
          for (uint8_t q = 0; q < 3; q++) cell_data.push_back(m_geom->cell.v[i][q]);
          cell.push_back(cell_data);
        }
      data[JSON_GEOM_VIEW_CELL] = cell;
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

  data[JSON_GEOM_VIEW_XFIELD_NAMES] = xfield_names;
  data[JSON_GEOM_VIEW_XFIELD_TYPES] = xfield_types;

  data[JSON_GEOM_VIEW_ATOMS] = json::array({});
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
      data[JSON_GEOM_VIEW_ATOMS].push_back(atom);
    }

  if (!m_type_color_override.empty()) {

      data[JSON_GEOM_VIEW_TYPE_COLOR_OVERRIDE] = json::array({});

      for (auto &elem : m_type_color_override) {
          json color_override_rec = json::array({});
          color_override_rec.push_back(elem.first);
          color_override_rec.push_back(elem.second[0]);
          color_override_rec.push_back(elem.second[1]);
          color_override_rec.push_back(elem.second[2]);
          data[JSON_GEOM_VIEW_TYPE_COLOR_OVERRIDE].push_back(color_override_rec);
        }
    }

  if (m_anim->animable()) {

      json animations = json::array({});

      for (auto &anim : m_anim->m_anim_data) {

          json animation = json::object();
          animation[JSON_GEOM_VIEW_ANIMATION_NAME] = anim.m_anim_name;
          animation[JSON_GEOM_VIEW_ANIMATION_TYPE] = anim.m_anim_type;
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

          animation[JSON_GEOM_VIEW_ANIMATION_FRAMES] = frames;
          animations.push_back(animation);
        }

      data[JSON_GEOM_VIEW_ANIMATIONS] = animations;
    }

  m_measure->save_to_json(data);

}

void geom_view_t::load_from_json (json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);

  if (data.find(JSON_GEOM_VIEW_DIM) != data.end()) {
      m_geom->DIM = data[JSON_GEOM_VIEW_DIM];
      m_geom->cell.DIM = m_geom->DIM;
    }

  json_helper::load_var(JSON_GEOM_VIEW_ATOM_SCALE, m_atom_scale_factor, data);
  json_helper::load_var(JSON_GEOM_VIEW_BOND_SCALE, m_bond_scale_factor, data);
  json_helper::load_var(JSON_GEOM_VIEW_SHOW_IMG_ATOMS, m_draw_img_atoms, data);
  json_helper::load_var(JSON_GEOM_VIEW_RENDER_TYPE, m_render_style, data);
  json_helper::load_var(JSON_GEOM_VIEW_DRAW_SPECULAR, m_draw_specular, data);
  json_helper::load_var(JSON_GEOM_VIEW_SPECULAR, m_shading_specular_power, data);
  json_helper::load_var(JSON_GEOM_VIEW_LABELS_TYPE, m_labels->m_style, data);
  json_helper::load_var(JSON_GEOM_VIEW_LABELS_SIZE, m_labels->m_lbl_font_size, data);
  json_helper::load_var(JSON_GEOM_VIEW_LABELS_DRAW_OUTLINE, m_labels->m_render_outlines, data);
  json_helper::load_var(JSON_GEOM_VIEW_LABELS_INPLACE, m_labels->m_render_inplace_hud, data);
  json_helper::load_var(JSON_GEOM_VIEW_LABELS_SEL_VIS, m_labels->m_selective_lbl, data);
  json_helper::load_var(JSON_GEOM_VIEW_LABELS_SCREEN_SPC_SCALE, m_labels->m_screen_scale, data);
  json_helper::load_var(JSON_GEOM_VIEW_LABELS_OUTLINE_SZ, m_labels->m_outline_size, data);

  json_helper::load_var(JSON_GEOM_VIEW_LABELS_INPLACE_OFFSET_X,
                        m_labels->m_inplace_offset[0],
                        data);
  json_helper::load_var(JSON_GEOM_VIEW_LABELS_INPLACE_OFFSET_Y,
                        m_labels->m_inplace_offset[1],
                        data);

  json_helper::load_var(JSON_GEOM_VIEW_SHOW_IMG_BONDS, m_draw_img_bonds, data);
  json_helper::load_var(JSON_GEOM_VIEW_SHOW_BONDS, m_draw_bonds, data);
  json_helper::load_var(JSON_GEOM_VIEW_SHOW_ATOMS, m_draw_atoms, data);
  json_helper::load_var(JSON_GEOM_VIEW_SHOW_IMG_ATOMS, m_draw_img_atoms, data);
  json_helper::load_var(JSON_GEOM_VIEW_BT_SHOW_DSBL, m_bt_show_disabled_record, data);
  json_helper::load_var(JSON_GEOM_VIEW_PERIODIC_DRAW_SUBCELLS, m_draw_subcells, data);
  json_helper::load_var(JSON_GEOM_VIEW_SEL_VIS, m_sel_vis, data);
  json_helper::load_var(JSON_GEOM_VIEW_SEL_VIS_AFFECT_BONDS, m_sel_vis_affect_bonds, data);
  json_helper::load_var(JSON_GEOM_VIEW_ROLE, m_role, data);

  //cell
  json_helper::load_var(JSON_GEOM_VIEW_PERIODIC_DRAW_CELL, m_draw_cell, data);
  json_helper::load_vec3(JSON_GEOM_VIEW_PERIODIC_CELL_COLOR, m_cell_color, data);

  //cell vectors
  json_helper::load_var(JSON_GEOM_VIEW_PERIODIC_DRAW_VECTORS, m_draw_cell_vectors, data);
  json_helper::load_var(JSON_GEOM_VIEW_PERIODIC_VECTORS_RATIO, m_cell_vectors_ratio, data);
  json_helper::load_vec3(JSON_GEOM_VIEW_PERIODIC_VECTORS_OFFSET, m_cell_vector_offset, data);
  json_helper::load_vec3(JSON_GEOM_VIEW_PERIODIC_VECTORS_COLOR, m_cell_vector_color, data);

  if (data.find(JSON_GEOM_VIEW_PERIODIC_SUBCELLS_RANGE) != data.end()) {
      int sc_a = data[JSON_GEOM_VIEW_PERIODIC_SUBCELLS_RANGE][0].get<int>();
      int sc_b = data[JSON_GEOM_VIEW_PERIODIC_SUBCELLS_RANGE][1].get<int>();
      int sc_c = data[JSON_GEOM_VIEW_PERIODIC_SUBCELLS_RANGE][2].get<int>();
      m_subcells_range = vector3<int>(sc_a, sc_b, sc_c);
    }

  if (data.find(JSON_GEOM_VIEW_TYPE_COLOR_OVERRIDE) != data.end()) {
      m_type_color_override.clear();
      for (const auto &rec : data[JSON_GEOM_VIEW_TYPE_COLOR_OVERRIDE]) {
          size_t type_idx = rec[0].get<size_t>();
          float tco_r = rec[1].get<float>();
          float tco_g = rec[2].get<float>();
          float tco_b = rec[3].get<float>();
          m_type_color_override.insert({type_idx, vector3<float>{tco_r, tco_g, tco_b}});
        }
    }

  m_tws_tr->do_action(act_lock | act_clear_all);

  m_ext_obs->first_data = true;

  if (m_geom->DIM>0) {
      if (data.find(JSON_GEOM_VIEW_CELL) != data.end()) {
          for (uint8_t i = 0; i < m_geom->DIM; i++) {
              vector3<float> cellv(data[JSON_GEOM_VIEW_CELL][i][0].get<float>(),
                  data[JSON_GEOM_VIEW_CELL][i][1].get<float>(),
                  data[JSON_GEOM_VIEW_CELL][i][2].get<float>());
              m_geom->cell.v[i] = cellv;
            }
        } else {
          m_geom->DIM = 0;
          //("Cannot load cell data for geom with DIM>0");
        }
    }

  if (data.find(JSON_GEOM_VIEW_XFIELD_NAMES) != data.end()) {

      std::vector<STRING_EX> fn;
      std::vector<basic_types> ft;

      auto &data_xf_names = data[JSON_GEOM_VIEW_XFIELD_NAMES];
      std::transform(data_xf_names.begin(), data_xf_names.end(), std::back_inserter(fn),
                     [](auto &_elem)->STRING_EX{return _elem.template get<STRING_EX>();});

      if (data.find(JSON_GEOM_VIEW_XFIELD_TYPES) != data.end())
        for (auto &elem : data[JSON_GEOM_VIEW_XFIELD_TYPES]) {
            STRING_EX fv = elem.get<STRING_EX>();
            if (fv == "b") ft.push_back(type_bool);
            if (fv == "i") ft.push_back(type_int);
            if (fv == "r") ft.push_back(type_real);
            if (fv == "d") ft.push_back(type_double);
            if (fv == "f") ft.push_back(type_float);
            if (fv == "s") ft.push_back(type_string);

          } else throw std::runtime_error("Invalid xfield types");

      m_geom->set_format(fn, ft);

    }

  if (data.find(JSON_GEOM_VIEW_ATOMS) != data.end())
    for (const auto &atom : data[JSON_GEOM_VIEW_ATOMS]) {

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

  if (data.find(JSON_GEOM_VIEW_BONDING_TABLE) != data.end()) {
      for (auto &elem : data[JSON_GEOM_VIEW_BONDING_TABLE]) {
          int type1 = m_geom->type_of_atom(elem[0].get<std::string>());
          int type2 = m_geom->type_of_atom(elem[1].get<std::string>());
          float dist = elem[2].get<float>();
          bool br_enabled = elem[3].get<bool>();
          m_tws_tr->m_bonding_table.m_dist[sym_key<size_t>(type1, type2)].m_bonding_dist = dist;
          m_tws_tr->m_bonding_table.m_dist[sym_key<size_t>(type1, type2)].m_enabled = br_enabled;
        }
      m_tws_tr->m_bonding_table.update_pair_max_dist_all();
    }

  if (data.find(JSON_GEOM_VIEW_ANIMATIONS) != data.end()) {
      bool static_anim_found{false};
      for (auto &anim : data[JSON_GEOM_VIEW_ANIMATIONS]) {

          geom_anim_record_t<float> tmp_anim_rec;

          if (anim.find(JSON_GEOM_VIEW_ANIMATION_NAME) != anim.end())
            tmp_anim_rec.m_anim_name = anim[JSON_GEOM_VIEW_ANIMATION_NAME];
          else tmp_anim_rec.m_anim_name = "generic1";

          if (anim.find(JSON_GEOM_VIEW_ANIMATION_TYPE) != anim.end())
            tmp_anim_rec.m_anim_type = anim[JSON_GEOM_VIEW_ANIMATION_TYPE];

          if (anim.find(JSON_GEOM_VIEW_ANIMATION_FRAMES) != anim.end()) {
              tmp_anim_rec.frames.reserve(anim[JSON_GEOM_VIEW_ANIMATION_FRAMES].size());

              for (auto &frame : anim[JSON_GEOM_VIEW_ANIMATION_FRAMES]) {
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
          if (tmp_anim_rec.m_anim_type == geom_anim_t::anim_static) static_anim_found = true;
          m_anim->m_anim_data.push_back(std::move(tmp_anim_rec));
        }

      if (!static_anim_found) {
          geom_anim_record_t<float> tmp_anim_static;
          tmp_anim_static.m_anim_name = "static";
          tmp_anim_static.m_anim_type = geom_anim_t::anim_static;
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

void geom_view_t::shake_atoms(std::set<size_t> atoms_to_shake, float magn) {

  app_state_t* astate = app_state_t::get_inst();

  static std::default_random_engine e;
  static std::uniform_real_distribution<> dis(0, magn);

  for (auto &atom : atoms_to_shake)
    if (atom < m_geom->nat()) {

        vector3<float> new_pos = m_geom->coord(atom) +
                                 vector3<float>(magn / 2) -
                                 vector3<float>(dis(e), dis(e), dis(e));

        m_geom->coord(atom) = new_pos;

      }

  astate->make_viewport_dirty();

}

void geom_view_t::py_shake_atoms(pybind11::list atoms_to_shake, float magn) {

  std::set<size_t> atoms;
  for (auto elem : atoms_to_shake)
    if (py::isinstance<py::int_>(elem)) atoms.insert(py::cast<int>(elem));

  shake_atoms(atoms, magn);

}

void geom_view_t::purify_boundary_atoms(geom_view_t *src) {

  if (!src || src->m_geom->nat() != m_geom->nat()) return;

  begin_structure_change();

  for (size_t i = 0; i < m_geom->nat(); i++) {

      float min_dist = 100.0f;
      auto goal_vector = m_geom->pos(i);
      auto pos_in_src = src->m_geom->pos(i);
      for (iterator idx(index::D(m_geom->DIM).all(-1),
                        index::D(m_geom->DIM).all(1)); !idx.end(); idx++ ) {
          auto t_pos_cf = m_geom->cell.transform(m_geom->pos(i), idx);
          auto dist = (pos_in_src - t_pos_cf).norm();
          if (dist < min_dist) {
              min_dist = dist;
              //min_dist_index = i;
              goal_vector = t_pos_cf;
            }
        }

      m_geom->coord(i) = goal_vector;

    }

  end_structure_change();

}

vector3<float> geom_view_t::dipole_moment() {

  vector3<float> accum_dm{0,0,0};
  for (int i = 0; i < m_geom->nat(); i++)
    accum_dm += m_geom->pos(i) * m_geom->xfield<float>(xgeom_charge, i);
  return accum_dm;

}

float geom_view_t::total_charge() {

  float accum_charge{0};
  for (int i = 0; i < m_geom->nat(); i++)
    accum_charge += m_geom->xfield<float>(xgeom_charge, i);
  return accum_charge;

}

pybind11::list geom_view_t::py_get_sel_pos_in_frame(vector3<float> t_frame) {

  return py::none();

}

py::list geom_view_t::get_sel_atoms(int index_offset) {

  py::list ret;

  std::set<size_t> sels;

  for (auto &sel : m_atom_idx_sel) sels.insert(sel.m_atm);

  for (size_t i = 0; i < m_geom->nat(); i++)
    if (sels.find(i) != sels.end()) ret.append(i+index_offset);

  return ret;

}

py::list geom_view_t::get_unsel_atoms(int index_offset) {

  py::list ret;

  std::set<size_t> sels;

  for (auto &sel : m_atom_idx_sel) sels.insert(sel.m_atm);

  for (size_t i = 0; i < m_geom->nat(); i++)
    if (sels.find(i) == sels.end()) ret.append(i+index_offset);

  return ret;

}

void geom_view_t::py_copy_settings(geom_view_t *src) {

  if (!src) return;

  m_draw_atoms             = src->m_draw_atoms;
  m_draw_bonds             = src->m_draw_bonds;
  m_draw_img_atoms         = src->m_draw_img_atoms;
  m_draw_img_bonds         = src->m_draw_img_bonds;
  m_render_style           = src->m_render_style;
  m_sel_vis                = src->m_sel_vis;
  m_sel_vis_affect_bonds   = src->m_sel_vis_affect_bonds;
  m_atom_scale_factor      = src->m_atom_scale_factor;
  m_bond_scale_factor      = src->m_bond_scale_factor;

  m_labels->m_style                  = src->m_labels->m_style;
  m_labels->m_screen_scale           = src->m_labels->m_screen_scale;
  m_labels->m_selective_lbl = src->m_labels->m_selective_lbl;

}

void geom_view_t::py_copy_xgeom_aux(geom_view_t *src) {

  if (!src || src->m_geom->nat() != m_geom->nat()) return;

  for (size_t i = 0; i < m_geom->nat(); i++) {

      m_geom->xfield<bool>(xgeom_sel_vis,i) = src->m_geom->xfield<bool>(xgeom_sel_vis,i);
      m_geom->xfield<bool>(xgeom_override,i) = src->m_geom->xfield<bool>(xgeom_override,i);
      m_geom->xfield<bool>(xgeom_label_show,i) = src->m_geom->xfield<bool>(xgeom_label_show,i);

      m_geom->xfield<std::string>(xgeom_label_text, i) =
          src->m_geom->xfield<std::string>(xgeom_label_text, i);

    }

}

std::vector<scalar_partition_per_type_t<>> geom_view_t::get_charge_partition() {

  std::vector<scalar_partition_per_type_t<>> retcp;

  const auto ch_eps = 0.0001f;

  for (size_t i = 0; i < m_geom->nat(); i++) {

      bool rec_founded{false};

      for (size_t q = 0; q < retcp.size(); q++)
        if (retcp[q].atype == m_geom->type(i) &&
            (std::fabs(retcp[q].value - m_geom->xfield<float>(xgeom_charge, i)) < ch_eps)) {
            rec_founded = true;
            retcp[q].count++;
          }

      if (!rec_founded) {
          scalar_partition_per_type_t<> tmp_rec;
          tmp_rec.atype = m_geom->type(i);
          tmp_rec.value = m_geom->xfield<float>(xgeom_charge, i);
          tmp_rec.count = 1;
          retcp.push_back(std::move(tmp_rec));
        }

    }

  return retcp;

}

py::list geom_view_t::py_get_charge_partition() {

  py::list ret_list;

  auto charge_part = get_charge_partition();

  for (auto &ch_rec : charge_part) {

      py::list ch_rec_list;
      ch_rec_list.append(ch_rec.atype);
      ch_rec_list.append(ch_rec.value);
      ch_rec_list.append(ch_rec.count);

      ret_list.append(ch_rec_list);

    }

  return ret_list;

}

void geom_view_t::pertrub_via_normal_mode(generic_array_t<vector3<float>, float> &disp) {

  if (m_geom->nat() != disp.size()) return;

  for (size_t i = 0; i < m_geom->nat(); i++)
    m_geom->coord(i) = m_geom->coord(i) + disp[i];

}

std::string geom_view_t::pretty_print_selected_atoms(vector3<float> new_frame) {

  std::string ret;

  bool first = true;
  for (auto &rec : m_atom_idx_sel) {

      vector3<float> pos_i = m_geom->pos(rec.m_atm) - new_frame;

      geom_labels_style_e cur_style =
          m_labels->m_style == geom_labels_style_e::show_none ?
            geom_labels_style_e::show_id_type :
            m_labels->m_style;

      ret +=
          fmt::format(first ? "{} {:8.8f} {:8.8f} {:8.8f}" : "\n{} {:8.8f} {:8.8f} {:8.8f}",
                      geom_view_labels_subsys_t::label_gen_fn(this, cur_style, rec.m_atm),
                      pos_i[0],
                      pos_i[1],
                      pos_i[2]
          );

      first = false;

    }

  return ret;

}



