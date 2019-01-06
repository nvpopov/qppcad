#include <qppcad/ws_atoms_list.hpp>
//#include <qppcad/ws_atoms_list_context_menu.hpp>
#include <qppcad/ws_atoms_list_render_bs.hpp>
#include <qppcad/ws_atoms_list_render_dlines.hpp>
#include <qppcad/ws_atoms_list_render_xlines.hpp>
#include <qppcad/ws_atoms_list_render_billboards.hpp>
//#include <qppcad/ws_atoms_list_cell_helper.hpp>
#include <qppcad/ws_comp_chem_data.hpp>
#include <qppcad/app_state.hpp>

//new ccd io modules
#include <io/ccd_firefly.hpp>
#include <io/ccd_xyz.hpp>
#include <io/ccd_cp2k.hpp>

//deprecated direct io modules
#include <io/geomio.hpp>
#include <io/vasp_io.hpp>
#include <io/cp2k.hpp>
#include <io/xyz_multiframe.hpp>

#include <clocale>

using namespace qpp;
using namespace qpp::cad;

ws_atoms_list_t::ws_atoms_list_t(): ws_item_t () {

  set_default_flags(ws_item_flags_default | ws_item_flags_support_translation |
                    ws_item_flags_support_selection | ws_item_flags_support_content_editing |
                    ws_item_flags_support_rendering_bb | ws_item_flags_toolbar_extension |
                    ws_item_flags_support_actions | ws_item_flags_support_delete |
                    ws_item_flags_support_clone | ws_item_flags_support_moveto |
                    ws_item_flags_support_rendering);

  m_geom = std::make_unique<xgeometry<float, periodic_cell<float> > >(3,"rg1");

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
   "ccr",//8
   "ccg",//9
   "ccb",//10
   "ltext"//11
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
   type_string //11
        });

  m_geom->DIM = 0;
  m_geom->cell.DIM = 0;
  m_geom->auto_update_types = true;

  m_ext_obs = std::make_unique<extents_observer_t<float> >(*m_geom);
  m_tws_tr  = std::make_unique<tws_tree_t<float> >(*m_geom);
  m_tws_tr->do_action(act_unlock);

  m_anim = std::make_unique<ws_atoms_list_anim_subsys_t>(*this);
  m_measure = std::make_unique<ws_atoms_list_measurement_subsys_t>(*this);
  m_labels = std::make_unique<ws_atoms_list_labels_subsys_t>(*this);
  m_lat_planes = std::make_unique<ws_atoms_list_lat_planes_subsys_t>(*this);

}

void ws_atoms_list_t::vote_for_view_vectors (vector3<float> &out_look_pos,
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

void ws_atoms_list_t::target_view(const cam_target_view_t _target_view,
                                  vector3<float> &look_from,
                                  vector3<float> &look_to,
                                  vector3<float> &look_up,
                                  bool &need_to_update_camera) {
  switch (_target_view) {

    case cam_target_view_t::tv_x_axis : {
        float axis_size = std::max(2.0f, m_ext_obs->aabb.max[0] - m_ext_obs->aabb.min[0]);
        look_from = m_pos + 2.0f*vector3<float>(axis_size, 0.0, 0.0);
        look_to = m_pos;
        look_up = {0.0 , 0.0 , 1.0};
        need_to_update_camera = true;
        break;
      }

    case cam_target_view_t::tv_y_axis : {
        float axis_size = std::max(2.0f, m_ext_obs->aabb.max[1] - m_ext_obs->aabb.min[1]);
        look_from = m_pos + 2.0f*vector3<float>(0.0, axis_size, 0.0);
        look_to = m_pos;
        look_up = {0.0, 0.0, 1.0};
        need_to_update_camera = true;
        break;
      }

    case cam_target_view_t::tv_z_axis : {
        float axis_size = std::max(2.0f, m_ext_obs->aabb.max[2] - m_ext_obs->aabb.min[2]);
        look_from = m_pos + 2.0f*vector3<float>(0.0, 0.0, axis_size);
        look_to = m_pos;
        look_up = {0.0, 1.0, 0.0};
        need_to_update_camera = true;
        break;
      }

    case cam_target_view_t::tv_a_axis : {
        vector3<float> center = 0.5*(m_geom->cell.v[0] + m_geom->cell.v[1] + m_geom->cell.v[2]);
        look_from = m_pos + center - 2.0f * m_geom->cell.v[0];
        look_to = m_pos  + center;
        look_up = {0.0 , 0.0 , 1.0};
        need_to_update_camera = true;
        break;
      }

    case cam_target_view_t::tv_b_axis : {
        vector3<float> center = 0.5*(m_geom->cell.v[0] + m_geom->cell.v[1] + m_geom->cell.v[2]);
        look_from = m_pos + center - 2.0f * m_geom->cell.v[1];
        look_to = m_pos  + center;
        look_up = {0.0, 0.0, 1.0};
        need_to_update_camera = true;
        break;
      }

    case cam_target_view_t::tv_c_axis : {
        vector3<float> center = 0.5*(m_geom->cell.v[0] + m_geom->cell.v[1] + m_geom->cell.v[2]);
        look_from = m_pos + center - 2.0f * m_geom->cell.v[2];
        look_to = m_pos  + center;
        look_up = {0.0, 1.0, 0.0};
        need_to_update_camera = true;
        break;
      }

    default : {
        break;
      }

    }
}

void ws_atoms_list_t::geometry_changed () {
  if (m_ext_obs)
    m_aabb = m_ext_obs->aabb;
}

void ws_atoms_list_t::render () {

  ws_item_t::render();

  app_state_t* astate = app_state_t::get_inst();
  vector3<float> _pos = m_pos;
  index all_null = index::D(m_geom->DIM).all(0);

  if (astate->dp){

      if (astate->m_debug_show_tws_tree) {
          astate->dp->begin_render_aabb();
          m_tws_tr->apply_visitor( [astate, _pos](tws_node_t<float> *in_node,
                                   int deep_level){
              astate->dp->render_aabb(clr_maroon,
                                      in_node->m_bb.min+_pos, in_node->m_bb.max+_pos);});
          astate->dp->end_render_aabb();
        }

      if (m_geom->DIM == 3 && m_is_visible && m_draw_cell) {
          astate->dp->begin_render_line();
          vector3<float> cell_clr = m_cell_color;
          if (m_selected){
              if(m_parent_ws->m_edit_type == ws_edit_t::edit_item)
                cell_clr = clr_red;
              if(m_parent_ws->m_edit_type == ws_edit_t::edit_content)
                cell_clr = clr_maroon;
            }

          astate->dp->render_cell_3d(
                cell_clr, m_geom->cell.v[0], m_geom->cell.v[1], m_geom->cell.v[2], m_pos);

          if ( m_draw_subcells) {
              vector3<float> sc_a = m_geom->cell.v[0] / m_subcells_range[0];
              vector3<float> sc_b = m_geom->cell.v[1] / m_subcells_range[1];
              vector3<float> sc_c = m_geom->cell.v[2] / m_subcells_range[2];
              for (int i_a = 0; i_a < m_subcells_range[0]; i_a++)
                for (int i_b = 0; i_b < m_subcells_range[1]; i_b++)
                  for (int i_c = 0; i_c < m_subcells_range[2]; i_c++) {
                      vector3<float> new_pos = m_pos + sc_a * i_a + sc_b * i_b + sc_c * i_c ;
                      astate->dp->render_cell_3d(cell_clr, sc_a, sc_b, sc_c, new_pos);
                    }
            }

          astate->dp->end_render_line();
        }



      if (!m_is_visible) return;

      switch (m_cur_render_type) {

        case ws_atoms_list_render_t::ball_and_stick :
          ws_atoms_list_render_bs::render(*this);
          break;

        case ws_atoms_list_render_t::dynamic_lines:
          ws_atoms_list_render_dlines::render(*this);
          break;

        case ws_atoms_list_render_t::xatom_lines:
          ws_atoms_list_render_xlines::render(*this);
          break;

        case ws_atoms_list_render_t::billboards:
          ws_atoms_list_render_billboards::render(*this);
          break;

        case ws_atoms_list_render_t::buffered_billboards: {
            if (!m_bs) {
                m_bs = std::make_unique<ws_atoms_list_render_buffered_billboards_t>(*this);
                m_bs->init();
              } else {
                m_bs->render();
              }
            //ws_atoms_list_render_billboards::render(*this);
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

void ws_atoms_list_t::render_overlay(QPainter &painter) {
  m_measure->render_overlay(painter);
  m_labels->render_overlay(painter);
}

bool ws_atoms_list_t::mouse_click (ray_t<float> *click_ray) {

  app_state_t* astate = app_state_t::get_inst();

  if (click_ray) {

      std::vector<tws_query_data_t<float, uint32_t> > res;

      ray_t<float> local_geom_ray;
      local_geom_ray.start = click_ray->start - m_pos;
      local_geom_ray.dir = click_ray->dir;

      if (m_draw_img_atoms)
        m_tws_tr->query_ray<query_ray_add_all<float> >(local_geom_ray,
                                                       res,
                                                       m_atom_scale_factor,
                                                       m_sel_vis,
                                                       xgeom_sel_vis);
      else
        m_tws_tr->query_ray<query_ray_add_ignore_img<float> >(local_geom_ray,
                                                              res,
                                                              m_atom_scale_factor,
                                                              m_sel_vis,
                                                              xgeom_sel_vis);
      recalc_gizmo_barycenter();

      if (!res.empty()) {

          std::sort(res.begin(), res.end(), &tws_query_data_sort_by_dist<float>);

          if (m_parent_ws->m_edit_type == ws_edit_t::edit_content && m_selected ) {
              atom_index_set_key iskey(int(res[0].m_atm), res[0].m_idx);
              auto atom_sel_it = m_atom_idx_sel.find(iskey);
              if (atom_sel_it == m_atom_idx_sel.end()) select_atom(res[0].m_atm, res[0].m_idx);
              else unselect_atom(res[0].m_atm, res[0].m_idx);

            }

          recalc_gizmo_barycenter();
          m_parent_ws->m_gizmo->update_gizmo(0.01f);
          astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
          return true;

        } else {

          //TODO: need refractoring
          if (m_parent_ws->m_edit_type == ws_edit_t::edit_content && m_selected ) {
              select_atoms(false);
            }

        }
    }

  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
  return false;

}

void ws_atoms_list_t::select_atoms (bool all) {

  app_state_t* astate = app_state_t::get_inst();

  if (all) {
      m_atom_idx_sel.clear();
      for (auto i = 0; i < m_geom->nat(); i++) select_atom(i);
    } else {
      m_atom_idx_sel.clear();
      m_atom_ord_sel.clear();
    }

  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
  astate->make_viewport_dirty();


}

void ws_atoms_list_t::select_atom (int atom_id) {
  select_atom(atom_id, index::D(m_geom->DIM).all(0));
}

void ws_atoms_list_t::select_atom(int atom_id, index atom_idx) {

  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();

  if (atom_id >= 0 && atom_id < m_geom->nat()) {
      m_atom_idx_sel.insert(atom_index_set_key(atom_id, atom_idx));
      if (m_atom_ord_sel.size() >= max_sel_in_deque) {
          m_atom_ord_sel.resize(max_sel_in_deque);
          m_atom_ord_sel.pop_front();
        }
      m_atom_ord_sel.push_back(atom_index_set_key(atom_id, atom_idx));
      //astate->make_viewport_dirty();
      recalc_gizmo_barycenter();
      m_parent_ws->m_gizmo->update_gizmo(0.01f);

      for (int i = 0; i < m_atom_ord_sel.size(); i++)
        astate->log(fmt::format("{} {} {}", i, m_atom_ord_sel[i].m_atm, m_atom_ord_sel[i].m_idx));

      return;
    }

  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);

  for (int i = 0; i < m_atom_ord_sel.size(); i++)
    astate->log(fmt::format("{} {} {}", i, m_atom_ord_sel[i].m_atm, m_atom_ord_sel[i].m_idx));

  return;

}

void ws_atoms_list_t::unselect_atom(int atom_id) {

  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();

  if (atom_id >= 0 && atom_id < m_geom->nat()) {

      for (iterator idx(index::D(m_geom->DIM).all(-1), index::D(m_geom->DIM).all(1));
           !idx.end(); idx++ ) {
          auto key = atom_index_set_key(atom_id, idx);

          auto i2 = std::find(m_atom_idx_sel.begin(), m_atom_idx_sel.end(), key);
          if (i2 != m_atom_idx_sel.end()) m_atom_idx_sel.erase(i2);

          auto it_ordered = std::find(m_atom_ord_sel.begin(), m_atom_ord_sel.end(), key);
          if (it_ordered != m_atom_ord_sel.end()) m_atom_ord_sel.erase(it_ordered);
        }

      //astate->make_viewport_dirty();
      recalc_gizmo_barycenter();
      m_parent_ws->m_gizmo->update_gizmo(0.01f);

      for (int i = 0; i < m_atom_ord_sel.size(); i++)
        astate->log(fmt::format("{} {} {}", i, m_atom_ord_sel[i].m_atm, m_atom_ord_sel[i].m_idx));

      return;
    }

  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);

  for (int i = 0; i < m_atom_ord_sel.size(); i++)
    astate->log(fmt::format("{} {} {}", i, m_atom_ord_sel[i].m_atm, m_atom_ord_sel[i].m_idx));

  return;
}

void ws_atoms_list_t::unselect_atom(int atom_id, index atom_idx) {

  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();

  if (atom_id >= 0 && atom_id < m_geom->nat()) {
      auto key = atom_index_set_key(atom_id, atom_idx);
      auto i2 = std::find(m_atom_idx_sel.begin(), m_atom_idx_sel.end(), key);
      if (i2 != m_atom_idx_sel.end()) m_atom_idx_sel.erase(i2);

      auto it_ordered = std::find(m_atom_ord_sel.begin(), m_atom_ord_sel.end(), key);
      if (it_ordered != m_atom_ord_sel.end()) m_atom_ord_sel.erase(it_ordered);

      recalc_gizmo_barycenter();
      m_parent_ws->m_gizmo->update_gizmo(0.01f);

      for (int i = 0; i < m_atom_ord_sel.size(); i++)
        astate->log(fmt::format("{} {} {}", i, m_atom_ord_sel[i].m_atm, m_atom_ord_sel[i].m_idx));

      return;
    }

  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);

  for (int i = 0; i < m_atom_ord_sel.size(); i++)
    astate->log(fmt::format("{} {} {}", i, m_atom_ord_sel[i].m_atm, m_atom_ord_sel[i].m_idx));

  return;

}

void ws_atoms_list_t::select_by_type (const int item_type_to_select) {

  app_state_t* astate = app_state_t::get_inst();

  for (auto i = 0; i < m_geom->nat(); i++)
    if (m_geom->type_table(i) == item_type_to_select) select_atom(i);

  //recalc_gizmo_barycenter();

  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();

}

void ws_atoms_list_t::unselect_by_type(const int item_type_to_unselect) {

  app_state_t* astate = app_state_t::get_inst();

  for (auto i = 0; i < m_geom->nat(); i++)

    if (m_geom->type_table(i) == item_type_to_unselect) unselect_atom(i);

  recalc_gizmo_barycenter();

  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();

}

void ws_atoms_list_t::invert_selected_atoms () {

  app_state_t* astate = app_state_t::get_inst();

  std::set<int> sel_atm;
  index zero = index::D(m_geom->DIM).all(0);

  for (auto &rec : m_atom_idx_sel)
    if (rec.m_idx == zero) sel_atm.insert(rec.m_atm);

  m_atom_idx_sel.clear();

  for (int i = 0 ; i < m_geom->nat(); i++)
    if (sel_atm.find(i) == sel_atm.end()) select_atom(i);

  recalc_gizmo_barycenter();
  m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();

}

void ws_atoms_list_t::insert_atom (const int atom_type, const vector3<float> &pos) {
  m_anim->m_force_non_animable = true;
  m_geom->add(m_geom->atom_of_type(atom_type), pos);
}

void ws_atoms_list_t::insert_atom (const string &atom_name, const vector3<float> &pos) {
  m_anim->m_force_non_animable = true;
  m_geom->add(atom_name, pos);
}

void ws_atoms_list_t::update_atom (const int at_id, const vector3<float> &pos) {
  m_anim->m_force_non_animable = true;
  m_geom->change_pos(at_id, pos);
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
}

void ws_atoms_list_t::update_atom (const int at_id, const string &at_name) {
  m_anim->m_force_non_animable = true;
  m_geom->change(at_id, at_name, m_geom->pos(at_id));
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
}

void ws_atoms_list_t::update_atom(const int at_id,
                                  const std::string &at_name,
                                  const vector3<float> &pos){
  m_anim->m_force_non_animable = true;
  m_geom->change(at_id, at_name, pos);
  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();
}

void ws_atoms_list_t::transform_atom(const int at_id,
                                     const matrix3<float> &tm) {
  vector3<float> pos = m_geom->coord(at_id);
  vector3<float> new_pos = tm * pos;
  m_geom->coord(at_id) = new_pos;
}

void ws_atoms_list_t::transform_atom(const int at_id,
                                     const matrix4<float> &tm) {

  vector4<float> p_aff(
        m_geom->coord(at_id)[0],
      m_geom->coord(at_id)[1],
      m_geom->coord(at_id)[2],
      1.0f);

  vector4<float> new_p = tm * p_aff;
  vector3<float> new_pos3(new_p[0], new_p[1], new_p[2]);

  m_geom->change_pos(at_id, new_pos3);

}

void ws_atoms_list_t::copy_from_xgeometry(xgeometry<float, periodic_cell<float> > &xgeom_inst) {
  for (int i = 0; i < xgeom_inst.nat(); i++) {
      m_geom->add(xgeom_inst.atom(i), xgeom_inst.pos(i));
      m_geom->xfield<float>(xgeom_charge, i) = xgeom_inst.xfield<float>(xgeom_charge, i);
    }
}

void ws_atoms_list_t::copy_to_xgeometry(xgeometry<float, periodic_cell<float> > &xgeom_inst) {

  xgeom_inst.clear();
  xgeom_inst.DIM = m_geom->DIM;
  xgeom_inst.cell.DIM = m_geom->cell.DIM;

  for (int i = 0; i < 3; i++)
    if (m_geom->DIM > i) xgeom_inst.cell.v[i] = m_geom->cell.v[i];

  for (int i = 0; i < m_geom->nat(); i++)
    xgeom_inst.add(m_geom->atom(i), m_geom->pos(i));
}

void ws_atoms_list_t::load_color_from_static_anim() {

  if (m_anim->m_anim_data.size()>0) {
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

void ws_atoms_list_t::select_atom_ngbs(const int at_id) {
  for (int i = 0; i < m_tws_tr->n(at_id); i++)
    if (m_tws_tr->table_idx(at_id, i) == index::D(m_geom->DIM).all(0))
      select_atom(m_tws_tr->table_atm(at_id, i));
}

void ws_atoms_list_t::select_selected_atoms_ngbs() {
  std::set<int> stored_sel;
  for (auto &rec : m_atom_idx_sel)
    if (rec.m_idx == index::D(m_geom->DIM).all(0)) stored_sel.insert(rec.m_atm);

  for (auto &rec : stored_sel) select_atom_ngbs(rec);
}

void ws_atoms_list_t::update_inter_atomic_dist(float new_dist,
                                               const int at1,
                                               const int at2,
                                               const index id1,
                                               const index id2,
                                               pair_dist_mode mode) {

  app_state_t* astate = app_state_t::get_inst();

  vector3<float> r_btw = (m_geom->pos(at1, id1) + m_geom->pos(at2, id2))*0.5f;
  vector3<float> dir_f = (m_geom->pos(at1, id1) - r_btw).normalized();
  vector3<float> dir_s = (m_geom->pos(at2, id2) - r_btw).normalized();

  switch (mode) {

    case pair_dist_mode::transform_both : {
        m_geom->change_pos(at1, r_btw + dir_f * new_dist * 0.5f);
        m_geom->change_pos(at2, r_btw + dir_s * new_dist * 0.5f);
        break;
      }

    case pair_dist_mode::fix_first : {
        m_geom->change_pos(at1, r_btw + dir_f * new_dist * 0.5f );
        break;
      }

    case pair_dist_mode::fix_second : {
        m_geom->change_pos(at2, r_btw + dir_s * new_dist * 0.5f);
        break;
      }

    }


  astate->make_viewport_dirty();
}

void ws_atoms_list_t::update_inter_atomic_dist(float new_dist,
                                               const int at1,
                                               const int at2,
                                               pair_dist_mode mode) {
  update_inter_atomic_dist(new_dist, at1, at2,
                           index::D(m_geom->DIM).all(0), index::D(m_geom->DIM).all(0), mode);
}

void ws_atoms_list_t::translate_selected (const vector3<float> &t_vec) {
  for (auto &elem : m_atom_idx_sel)
    if (elem.m_idx == index::D(m_geom->DIM).all(0))
      update_atom(elem.m_atm, m_geom->pos(elem.m_atm) + t_vec);
  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_selected_atoms_list_selected_content_changed();
}

void ws_atoms_list_t::delete_selected_atoms () {

  app_state_t* astate = app_state_t::get_inst();

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

void ws_atoms_list_t::make_super_cell (const int a_steps,
                                       const int b_steps,
                                       const int c_steps) {
  if (!m_parent_ws) return;
  if (m_geom->DIM != 3) return;

  std::shared_ptr<ws_atoms_list_t> sc_al = std::make_shared<ws_atoms_list_t>();
  sc_al->m_geom->DIM = 3;
  sc_al->m_geom->cell.DIM = 3;

  //sc_al->set_parent_workspace(parent_ws);
  sc_al->m_tws_tr->do_action(act_lock | act_clear_all);

  sc_al->m_geom->cell.v[0] = m_geom->cell.v[0] * (a_steps);
  sc_al->m_geom->cell.v[1] = m_geom->cell.v[1] * (b_steps);
  sc_al->m_geom->cell.v[2] = m_geom->cell.v[2] * (c_steps);

  if (m_role == ws_atoms_list_role_t::role_uc) {
      sc_al->m_draw_img_atoms = false;
      sc_al->m_draw_img_bonds = false;
    }

  for (auto i = 0; i < m_geom->nat(); i++)
    for (iterator idx_it(index::D(m_geom->DIM).all(0), index({a_steps-1, b_steps-1, c_steps-1}));
         !idx_it.end(); idx_it++ ) {
        vector3<float> new_atom_pos = m_geom->pos(i, idx_it);
        sc_al->m_geom->add(m_geom->atom(i), new_atom_pos);
        if (m_role == ws_atoms_list_role_t::role_uc)
          sc_al->m_geom->xfield<float>(xgeom_charge, sc_al->m_geom->nat()-1) =
              m_geom->xfield<float>(xgeom_charge, i);
      }

  sc_al->m_pos = m_pos + m_geom->cell.v[0] * 1.4f;

  sc_al->m_name = m_name + fmt::format("_sc_{}_{}_{}", a_steps, b_steps, c_steps);

  m_parent_ws->add_item_to_ws(sc_al);

  sc_al->m_tws_tr->do_action(act_unlock | act_rebuild_all);
  sc_al->geometry_changed();

  //perform purification
  if (m_role == ws_atoms_list_role_t::role_uc) {

      sc_al->m_tws_tr->do_action(act_lock);
      //intermediage xgeom
      xgeometry<float, periodic_cell<float> > g(3);
      g.set_format({"charge"},{type_real});
      g.DIM = 3;
      g.cell.DIM = 3;
      g.cell.v[0] = sc_al->m_geom->cell.v[0];
      g.cell.v[1] = sc_al->m_geom->cell.v[1];
      g.cell.v[2] = sc_al->m_geom->cell.v[2];
      // tws_tree_t<float, periodic_cell<float> > sum_tree(g);
      const float equality_dist = 0.01f;
      for (int i = 0; i < sc_al->m_geom->nat(); i++) {
          std::vector<tws_node_content_t<float> > res;
          sc_al->m_tws_tr->query_sphere(equality_dist, sc_al->m_geom->pos(i), res);
          float accum_chg = 0;

          bool need_to_add{true};
          for (auto &elem : res)
            if (elem.m_idx == index::D(sc_al->m_geom->DIM).all(0)) {
                accum_chg +=  sc_al->m_geom->xfield<float>(xgeom_charge, elem.m_atm);
                if (i > elem.m_atm) need_to_add = false;
              }

          if (need_to_add) {
              g.add(sc_al->m_geom->atom(i), sc_al->m_geom->pos(i));
              g.xfield<float>(xgeom_charge, g.nat()-1) = accum_chg;
            }
        }

      sc_al->m_geom->clear();
      sc_al->copy_from_xgeometry(g);
      sc_al->m_tws_tr->do_action(act_unlock | act_rebuild_all);

    }

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_changed();

}

void ws_atoms_list_t::apply_axial_scale (const float scale_a,
                                         const float scale_b,
                                         const float scale_c) {
  if (m_geom->DIM != 3) return;
  m_tws_tr->do_action(act_lock | act_clear_all);

  periodic_cell<float> new_cell(3);
  new_cell.v[0] = m_geom->cell.v[0] * scale_a;
  new_cell.v[1] = m_geom->cell.v[1] * scale_b;
  new_cell.v[2] = m_geom->cell.v[2] * scale_c;

  for (auto i = 0; i < m_geom->nat(); i++) {
      vector3<float> frac_in_old_cell = m_geom->cell.cart2frac(m_geom->pos(i));
      m_geom->change_pos(i, new_cell.frac2cart(frac_in_old_cell));
    }

  m_geom->cell.v[0] = new_cell.v[0];
  m_geom->cell.v[1] = new_cell.v[1];
  m_geom->cell.v[2] = new_cell.v[2];

  m_tws_tr->do_action(act_unlock | act_rebuild_all);

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_selected_atoms_list_cell_changed();

}

void ws_atoms_list_t::move_selected_atoms_to_home (bool ignore_selection) {

  for (int i = 0; i < m_geom->nat(); i++)
    if (m_atom_idx_sel.find(atom_index_set_key(i, index::D(m_geom->DIM).all(0))) !=
        m_atom_idx_sel.end() || ignore_selection) {
        vector3<float> pos = m_geom->pos(i);
        m_geom->change_pos(i, m_geom->cell.reduce(pos));
      }

}

std::string ws_atoms_list_t::compose_item_name () {
  return fmt::format("atom list, DIM = [{}d]", m_geom->DIM);
}

void ws_atoms_list_t::update (float delta_time) {
  ws_item_t::update(delta_time);
  m_anim->update(delta_time);
}

float ws_atoms_list_t::get_bb_prescaller () {
  if (m_geom->DIM == 3) return 1.5f;
  return 1.1f;
}

bool ws_atoms_list_t::is_bb_visible() {
  return m_geom->DIM == 0;
}

uint32_t ws_atoms_list_t::get_amount_of_selected_content () {
  return this->m_atom_idx_sel.size();
}

size_t ws_atoms_list_t::get_content_count () {
  return (m_geom->nat());
}

void ws_atoms_list_t::on_begin_content_gizmo_translate () {
  //c_app::log(fmt::format("Start of translating node [{}] content", m_name));
  //m_tws_tr->do_action(act_lock);
}

void ws_atoms_list_t::apply_intermediate_translate_content (const vector3<float> &pos) {

  app_state_t* astate = app_state_t::get_inst();

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

void ws_atoms_list_t::on_end_content_gizmo_translate () {
  //c_app::log(fmt::format("End of translating node [{}] content", m_name));
  //m_tws_tr->do_action(act_unlock);
}

void ws_atoms_list_t::recalc_gizmo_barycenter () {
  //barycenter in local frame
  m_gizmo_barycenter = vector3<float>::Zero();

  if (!m_atom_idx_sel.empty() || m_geom->nat() == 0) {
      for (const auto& atm_idx : m_atom_idx_sel)
        m_gizmo_barycenter += m_geom->pos(atm_idx.m_atm, atm_idx.m_idx);
      m_gizmo_barycenter /= m_atom_idx_sel.size();
    }
  else m_gizmo_barycenter = m_aabb.min;

}

const vector3<float> ws_atoms_list_t::get_gizmo_content_barycenter() {
  return m_gizmo_barycenter;
}

void ws_atoms_list_t::shift(const vector3<float> shift) {
  m_tws_tr->do_action(act_lock);

  for (int i = 0; i < m_geom->nat(); i++)
    m_geom->coord(i) = shift + m_geom->pos(i) ;

  m_ext_obs->aabb.min = shift + m_ext_obs->aabb.min;
  m_ext_obs->aabb.max = shift + m_ext_obs->aabb.max;
  m_tws_tr->apply_shift(shift);

  m_tws_tr->do_action(act_unlock);
  geometry_changed();
}

void ws_atoms_list_t::load_from_file(qc_file_fmt file_format, std::string file_name,
                                     bool auto_center) {

  app_state_t* astate = app_state_t::get_inst();

  using elapsed_duration = std::chrono::duration<float, std::ratio<1> >;

  std::setlocale(LC_ALL, "C");

  astate->log(fmt::format("Loading geometry from file {} to ws_atom_list in workspace {}",
                          file_name, m_parent_ws->m_ws_name));

  std::ifstream qc_data(file_name);
  if (!qc_data) {
      astate->log(fmt::format("Error in loading from file {}", file_name));
      return;
    }

  //clean geom and tws-tree
  m_tws_tr->do_action(act_lock | act_clear_all);
  m_ext_obs->first_data = true;

  m_name = extract_base_name(file_name);

  auto start_timer = std::chrono::steady_clock::now();
  bool need_to_compile_from_ccd = false;
  bool need_to_extract_ccd = false;
  bool need_to_compile_ccd = false;
  comp_chem_program_data_t<float> cc_inst;

  switch (file_format) {

    case qc_file_fmt::standart_xyz : {
        m_geom->DIM = 0;
        need_to_compile_from_ccd = true;
        read_ccd_from_xyz_file(qc_data, cc_inst);
        break;
      }

    case qc_file_fmt::qpp_uc : {
        astate->log(fmt::format("qpp_uc before m_geom.nat() {}", m_geom->nat()));
        m_geom->DIM = 3;
        m_geom->cell.DIM = 3;
        read_xyzq(qc_data, *m_geom, true);
        m_draw_img_atoms = false;
        m_draw_img_bonds = false;
        m_role = ws_atoms_list_role_t::role_uc;
        astate->log(fmt::format("qpp_uc after m_geom.nat() {}", m_geom->nat()));
        break;
      }

    case qc_file_fmt::vasp_poscar : {
        m_geom->DIM = 3;
        m_geom->cell.DIM = 3;
        read_vasp_poscar(qc_data, *(m_geom));
        break;
      }

    case qc_file_fmt::vasp_outcar_md : {
        m_geom->DIM = 3;
        m_geom->cell.DIM = 3;
        need_to_extract_ccd = true;
        read_vasp_outcar_md_with_frames(qc_data, *(m_geom), m_anim->m_anim_data);
        break;
      }

    case qc_file_fmt::firefly_output : {
        m_geom->DIM = 0;
        need_to_extract_ccd = true;
        read_ccd_from_firefly_output(qc_data, cc_inst);
        need_to_compile_from_ccd = true;
        need_to_compile_ccd = true;
        break;
      }

    case qc_file_fmt::cp2k_output : {
        need_to_extract_ccd = true;
        need_to_compile_from_ccd = true;
        need_to_compile_ccd = true;
        read_ccd_from_cp2k_output(qc_data, cc_inst);
        m_geom->DIM = cc_inst.m_DIM;
        m_geom->cell.DIM = m_geom->DIM;
        break;
      }

    default: {
        astate->log("File format not implemented");
      }
    }

  qc_data.close();

  if (need_to_compile_ccd) {
      bool succes_comp_ccd = compile_ccd(cc_inst, ccd_cf_default_flags |
                                         ccd_cf_remove_empty_geom_steps);
      astate->log(fmt::format("Is ccd compilation succes? {}", succes_comp_ccd));
    }

  if (need_to_compile_from_ccd) {
      bool succes_comp_geom = compile_geometry(cc_inst, *(m_geom.get()));
      bool succes_comp_static_anim = compile_static_animation(cc_inst, m_anim->m_anim_data);
      bool succes_anims = compile_animation(cc_inst, m_anim->m_anim_data);

      astate->log(fmt::format("Is geometry compilation succes? {}",
                              succes_comp_geom && succes_comp_static_anim));
      if (m_anim->get_total_anims() > 1 && succes_anims)
        astate->log("Animations have been added to geom");
    }

  if (need_to_extract_ccd) {
      std::shared_ptr<ws_comp_chem_data_t> extracted_ccd = std::make_shared<ws_comp_chem_data_t>();
      extracted_ccd->m_name = m_name+"_ccd";
      extracted_ccd->m_ccd =
          std::make_unique<comp_chem_program_data_t<float> >(std::move(cc_inst));
      extracted_ccd->m_connected_items.push_back(shared_from_this());
      extracted_ccd->m_connected_items_stride.push_back(0);
      m_parent_ws->add_item_to_ws(extracted_ccd);
    }

  auto end_timer = std::chrono::steady_clock::now();
  auto diff_timer = end_timer - start_timer;

  astate->log(fmt::format("Reading file {} took {} sec.", file_name,
                          elapsed_duration(diff_timer).count()) );

  //TODO: move autocentering to ccd compilation
  if (auto_center) {
      vector3<float> center(0.0, 0.0, 0.0);
      for (int i = 0; i < m_geom->nat(); i++)
        center += m_geom->pos(i);
      center *= (1.0f / m_geom->nat());
      for (int i = 0; i < m_geom->nat(); i++)
        m_geom->coord(i) = -center + m_geom->pos(i) ;

      m_ext_obs->aabb.min = -center + m_ext_obs->aabb.min;
      m_ext_obs->aabb.max = -center + m_ext_obs->aabb.max;

      for (auto &anim : m_anim->m_anim_data)
        for (auto &anim_frame : anim.frames)
          for (auto &anim_frame_rec : anim_frame.atom_pos)
            anim_frame_rec -= center;
    }

  auto start_timer_build_tree = std::chrono::steady_clock::now();
  m_tws_tr->do_action(act_unlock | act_rebuild_tree);
  auto end_timer_build_tree = std::chrono::steady_clock::now();
  auto diff_timer_build_tree = end_timer_build_tree - start_timer_build_tree;

  astate->log(fmt::format("Building tws-tree for file {} took {} sec.", file_name,
                          elapsed_duration(diff_timer_build_tree).count()));

  if (m_geom->nat() > 30000) {
      m_cur_render_type = ws_atoms_list_render_t::billboards;
    } else {

    }

  auto start_timer_build_ntable = std::chrono::steady_clock::now();
  m_tws_tr->do_action(act_rebuild_ntable);
  auto end_timer_build_ntable  = std::chrono::steady_clock::now();
  auto diff_timer_build_ntable  = end_timer_build_ntable  - start_timer_build_ntable ;
  astate->log(fmt::format("Building ntable for file {} took {} sec.", file_name,
                          elapsed_duration(diff_timer_build_ntable).count()));
  geometry_changed();

  if (m_parent_ws) {
      m_parent_ws->set_best_view();
      m_parent_ws->ws_changed();
    }

}

void ws_atoms_list_t::save_to_file (qc_file_fmt file_format, std::string file_name) {

  //std::setlocale(LC_ALL, "C");

  app_state_t* astate = app_state_t::get_inst();
  astate->log(fmt::format("Saving geometry[{}] to file {} from workspace {}",
                          m_name, file_name, m_parent_ws->m_ws_name));

  std::ofstream output(file_name);
  bool wrong_dimension{false};

  if (output) {
      switch (file_format) {
        case qc_file_fmt::standart_xyz: {
            if (m_geom->DIM == 0) write_xyz(output, *m_geom);
            else wrong_dimension = true;
            break;
          };
        case qc_file_fmt::vasp_poscar: {
            if (m_geom->DIM == 3) write_vasp_poscar(output, *m_geom);
            else wrong_dimension = true;
            break;
          };
        case qc_file_fmt::cp2k_coord_cell_section : {
            if (m_geom->DIM == 3) write_cp2k_coord_section(output, *m_geom);
            else wrong_dimension = true;
            break;
          }
        case qc_file_fmt::qpp_uc : {
            if (m_geom->DIM == 3) write_xyzq(output, *m_geom);
            else wrong_dimension = true;
            break;
          }
        default : {
            astate->log(fmt::format("Saving geomtery -> format[{}] not supported", file_format));
            break;
          }
        }
    }

  if (wrong_dimension) astate->log("Export selected geometry : wrong dimension!");
}

string ws_atoms_list_t::get_ws_item_class_name () {
  return "ws_atoms_list";
}

void ws_atoms_list_t::write_to_json (json &data) {

  ws_item_t::write_to_json(data);

  data[JSON_DIM] = m_geom->DIM;
  data[JSON_SHOW_IMG_ATOMS] = m_draw_img_atoms;
  data[JSON_SHOW_IMG_BONDS] = m_draw_img_bonds;
  data[JSON_SHOW_BONDS] = m_draw_bonds;
  data[JSON_SHOW_ATOMS] = m_draw_atoms;
  data[JSON_BT_SHOW_DSBL] = m_bt_show_disabled_record;
  data[JSON_ATOM_SCALE] = m_atom_scale_factor;
  data[JSON_BOND_SCALE] = m_bond_scale_factor;
  data[JSON_ATOMS_LIST_RENDER_TYPE] = m_cur_render_type;
  data[JSON_ATOMS_LIST_DRAW_SPECULAR] = m_draw_specular;
  data[JSON_ATOMS_LIST_SPECULAR] = m_shading_specular_power;

  data[JSON_ATOMS_LIST_LABELS_TYPE] = m_labels->m_style;

  data[JSON_CELL_COLOR] = json::array({m_cell_color[0], m_cell_color[1], m_cell_color[2]});
  data[JSON_BONDING_TABLE] = json::array({});

  data[JSON_ATOMS_LIST_SEL_VIS] = m_sel_vis;
  data[JSON_ATOMS_LIST_SEL_VIS_AFFECT_BONDS] = m_sel_vis_affect_bonds;

  if (m_geom->DIM == 3) {
      data[JSON_ATOMS_LIST_DRAW_SUBCELLS] = m_draw_subcells;
      data[JSON_ATOMS_LIST_SUBCELLS_RANGE] =
          json::array({m_subcells_range[0], m_subcells_range[1], m_subcells_range[2]});
    }

  for (auto &record: m_tws_tr->m_bonding_table.m_dist) {
      json bt_rec = json::array({});
      bt_rec.push_back(m_geom->atom_of_type(record.first.m_a));
      bt_rec.push_back(m_geom->atom_of_type(record.first.m_b));
      bt_rec.push_back(record.second.m_bonding_dist);
      bt_rec.push_back(record.second.m_enabled);
      data[JSON_BONDING_TABLE].push_back(bt_rec);
    }

  if (m_geom->DIM > 0) {
      json cell = json::array({});
      for (uint8_t i = 0; i < m_geom->DIM; i++) {
          json cell_data = json::array({});
          for (uint8_t q = 0; q < 3; q++) cell_data.push_back(m_geom->cell.v[i][q]);
          cell.push_back(cell_data);
        }
      data[JSON_CELL] = cell;
    }

  data[JSON_ATOMS] = json::array({});
  for (auto i = 0; i < m_geom->nat(); i++){
      json atom = json::array({});
      atom.push_back(m_geom->atom(i));
      atom.push_back(m_geom->pos(i)[0]);
      atom.push_back(m_geom->pos(i)[1]);
      atom.push_back(m_geom->pos(i)[2]);
      atom.push_back(m_geom->xfield<bool>(xgeom_sel_vis, i));
      atom.push_back(m_geom->xfield<bool>(xgeom_label_state, i));
      atom.push_back(m_geom->xfield<std::string>(xgeom_label_text, i));
      data[JSON_ATOMS].push_back(atom);
    }

  if (m_anim->animable()) {

      json animations = json::array({});

      for (auto &anim : m_anim->m_anim_data)
        if (anim.m_anim_type != geom_anim_t::anim_static) {

            json animation = json::object();
            animation[JSON_ATOMS_LIST_ANIMATION_NAME] = anim.m_anim_name;
            animation[JSON_ATOMS_LIST_ANIMATION_TYPE] = anim.m_anim_type;
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

            animation[JSON_ATOMS_LIST_ANIMATION_FRAMES] = frames;
            animations.push_back(animation);
          }

      data[JSON_ATOMS_LIST_ANIMATIONS] = animations;
    }

}

void ws_atoms_list_t::read_from_json (json &data) {

  ws_item_t::read_from_json(data);

  if (data.find(JSON_DIM) != data.end()) {
      m_geom->DIM = data[JSON_DIM];
      m_geom->cell.DIM = m_geom->DIM;
    }

  if (data.find(JSON_ATOM_SCALE) != data.end())
    m_atom_scale_factor = data[JSON_ATOM_SCALE].get<float>();

  if (data.find(JSON_BOND_SCALE) != data.end())
    m_bond_scale_factor = data[JSON_BOND_SCALE].get<float>();

  if (data.find(JSON_SHOW_IMG_ATOMS) != data.end())
    m_draw_img_atoms = data[JSON_SHOW_IMG_ATOMS];

  if (data.find(JSON_ATOMS_LIST_RENDER_TYPE) != data.end())
    m_cur_render_type = data[JSON_ATOMS_LIST_RENDER_TYPE];

  if (data.find(JSON_ATOMS_LIST_DRAW_SPECULAR) != data.end())
    m_draw_specular = data[JSON_ATOMS_LIST_DRAW_SPECULAR];

  if (data.find(JSON_ATOMS_LIST_SPECULAR) != data.end())
    m_shading_specular_power = data[JSON_ATOMS_LIST_SPECULAR].get<float>();

  if (data.find(JSON_ATOMS_LIST_LABELS_TYPE) != data.end())
    m_labels->m_style = data[JSON_ATOMS_LIST_LABELS_TYPE];

  if (data.find(JSON_SHOW_IMG_BONDS) != data.end())
    m_draw_img_bonds = data[JSON_SHOW_IMG_BONDS];

  if (data.find(JSON_SHOW_BONDS) != data.end())
    m_draw_atoms = data[JSON_SHOW_BONDS];

  if (data.find(JSON_SHOW_ATOMS) != data.end())
    m_draw_bonds = data[JSON_SHOW_ATOMS];

  if (data.find(JSON_BT_SHOW_DSBL) != data.end())
    m_bt_show_disabled_record = data[JSON_BT_SHOW_DSBL];

  if (data.find(JSON_ATOMS_LIST_DRAW_SUBCELLS) != data.end())
    m_draw_subcells = data[JSON_ATOMS_LIST_DRAW_SUBCELLS];

  if (data.find(JSON_ATOMS_LIST_SEL_VIS) != data.end())
    m_sel_vis = data[JSON_ATOMS_LIST_SEL_VIS];

  if (data.find(JSON_ATOMS_LIST_SEL_VIS_AFFECT_BONDS) != data.end())
    m_sel_vis_affect_bonds = data[JSON_ATOMS_LIST_SEL_VIS_AFFECT_BONDS];

  if (data.find(JSON_ATOMS_LIST_SUBCELLS_RANGE) != data.end()) {
      int sc_a = data[JSON_ATOMS_LIST_SUBCELLS_RANGE][0].get<int>();
      int sc_b = data[JSON_ATOMS_LIST_SUBCELLS_RANGE][1].get<int>();
      int sc_c = data[JSON_ATOMS_LIST_SUBCELLS_RANGE][2].get<int>();
      m_subcells_range = vector3<int>(sc_a, sc_b, sc_c);
    }

  m_tws_tr->do_action(act_lock | act_clear_all);

  m_ext_obs->first_data = true;

  if (m_geom->DIM>0) {
      if (data.find(JSON_CELL) != data.end()) {
          for (uint8_t i = 0; i < m_geom->DIM; i++) {
              vector3<float> cellv(data[JSON_CELL][i][0].get<float>(),
                  data[JSON_CELL][i][1].get<float>(),
                  data[JSON_CELL][i][2].get<float>());
              m_geom->cell.v[i] = cellv;
            }
        } else {
          m_geom->DIM = 0;
          //("Cannot load cell data for geom with DIM>0");
        }
    }

  if (data.find(JSON_ATOMS) != data.end())
    for (const auto &atom : data[JSON_ATOMS]) {
        m_geom->add(atom[0].get<string>(),
            vector3<float>(atom[1].get<float>(), atom[2].get<float>(), atom[3].get<float>()));

        if (atom.size() > 4) {
            m_geom->xfield<bool>(xgeom_sel_vis, m_geom->nat()-1) = atom[4].get<bool>();
            if (atom.size() > 6) {
                m_geom->xfield<bool>(xgeom_label_state, m_geom->nat()-1) = atom[5].get<bool>();
                m_geom->xfield<std::string>(xgeom_label_text, m_geom->nat()-1) =
                    atom[6].get<std::string>();
              }
          }
      }

  if (data.find(JSON_BONDING_TABLE) != data.end()) {
      for (auto &elem : data[JSON_BONDING_TABLE]) {
          int type1 = m_geom->type_of_atom(elem[0].get<string>());
          int type2 = m_geom->type_of_atom(elem[1].get<string>());
          float dist = elem[2].get<float>();
          bool br_enabled = elem[3].get<bool>();
          m_tws_tr->m_bonding_table.m_dist[sym_key<uint32_t>(type1, type2)].m_bonding_dist = dist;
          m_tws_tr->m_bonding_table.m_dist[sym_key<uint32_t>(type1, type2)].m_enabled = br_enabled;
        }
      m_tws_tr->m_bonding_table.update_pair_max_dist_all();
    }

  if (data.find(JSON_ATOMS_LIST_ANIMATIONS) != data.end()) {
      bool static_anim_found{false};
      for (auto &anim : data[JSON_ATOMS_LIST_ANIMATIONS]) {

          geom_anim_record_t<float> tmp_anim_rec;

          if (anim.find(JSON_ATOMS_LIST_ANIMATION_NAME) != anim.end())
            tmp_anim_rec.m_anim_name = anim[JSON_ATOMS_LIST_ANIMATION_NAME];
          else tmp_anim_rec.m_anim_name = "generic1";

          if (anim.find(JSON_ATOMS_LIST_ANIMATION_TYPE) != anim.end())
            tmp_anim_rec.m_anim_type = anim[JSON_ATOMS_LIST_ANIMATION_TYPE];

          if (anim.find(JSON_ATOMS_LIST_ANIMATION_FRAMES) != anim.end()) {
              tmp_anim_rec.frames.reserve(anim[JSON_ATOMS_LIST_ANIMATION_FRAMES].size());

              for (auto &frame : anim[JSON_ATOMS_LIST_ANIMATION_FRAMES]) {
                  tmp_anim_rec.frames.resize(tmp_anim_rec.frames.size() + 1);
                  size_t nf_id = tmp_anim_rec.frames.size() - 1;

                  tmp_anim_rec.frames[nf_id].atom_pos.reserve(frame.size());

                  for (auto &frame_data : frame) {
                      vector3<float> frame_coord;
                      frame_coord[0] = frame_data[0].get<float>();
                      frame_coord[1] = frame_data[1].get<float>();
                      frame_coord[2] = frame_data[2].get<float>();
                      std::cout << frame_coord.to_string_vec() << std::endl;
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

  geometry_changed();
  m_tws_tr->do_action(act_unlock | act_build_all);

}

bool ws_atoms_list_t::can_be_written_to_json() {
  return true;
}



