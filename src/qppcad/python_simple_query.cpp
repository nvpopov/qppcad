#include <qppcad/python_simple_query.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/geom_view/geom_view.hpp>
#include <qppcad/ws_volume_data/ws_volume_data.hpp>
#include <qppcad/psg_view/psg_view.hpp>
#include <qppcad/ws_traj_highlight/ws_traj_highlight.hpp>
#include <qppcad/ws_cube_primitive/ws_cube_primitive.hpp>
#include <qppcad/ws_arrow_primitive/ws_arrow_primitive.hpp>
#include <data/ptable.hpp>
#include <qppcad/tools/supercell/supercell.hpp>
#include <symm/point_groups.hpp>

using namespace qpp;
using namespace qpp::cad;

namespace py = pybind11;

 std::shared_ptr<workspace_manager_t> simple_query::get_ws_mgr() {

  app_state_t *astate = app_state_t::get_inst();
  return astate->ws_manager;

}

void simple_query::open_file(std::string file_name,
                             bool to_current) {

  app_state_t *astate = app_state_t::get_inst();

  astate->ws_manager->load_from_file_autodeduce(file_name, "", !to_current);
  astate->make_viewport_dirty();
  astate->astate_evd->python_console_focus_requested();

}

void simple_query::open_file_query(std::string file_name,
                                   std::string file_format,
                                   bool to_current) {

  app_state_t *astate = app_state_t::get_inst();

  astate->ws_manager->load_from_file_autodeduce(file_name, file_format, !to_current);
  astate->make_viewport_dirty();
  astate->astate_evd->python_console_focus_requested();

}

void simple_query::make_super_cell(const int sc_a, const int sc_b, const int sc_c) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  supercell_tool_t sct;
  sct.make_super_cell(al.get(), sc_a, sc_b, sc_c);
  cur_ws->set_selected_item(cur_ws->m_ws_items.size()-1);
  astate->astate_evd->python_console_focus_requested();
  astate->make_viewport_dirty();

}

void simple_query::select_ws(int ws_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  astate->ws_manager->set_cur_id(ws_idx);
  astate->astate_evd->cur_ws_changed();
  astate->make_viewport_dirty();

}

void simple_query::select_itm(int itm_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;
  cur_ws->set_selected_item(itm_idx);

  astate->make_viewport_dirty();

  astate->astate_evd->python_console_focus_requested();

}

void simple_query::sel_cnt(int cnt_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  al->select_atom(cnt_idx);

  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_parity() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  index zero = index::D(al->m_geom->DIM).all(0);
  for (auto &elem : al->m_atom_idx_sel)
    if (elem.m_idx == zero) {
        std::array<int, 2> parity_d{-1,1};
        for (auto &p_x : parity_d)
          for (auto &p_y : parity_d)
            for (auto &p_z : parity_d) {
                vector3<float> new_pos{0};
                new_pos[0] = al->m_geom->coord(elem.m_atm)[0] * p_x;
                new_pos[1] = al->m_geom->coord(elem.m_atm)[1] * p_y;
                new_pos[2] = al->m_geom->coord(elem.m_atm)[2] * p_z;
                std::vector<tws_node_content_t<float> > res;
                const float eps_dist = 0.01;
                al->m_tws_tr->query_sphere(eps_dist, new_pos, res);
                for (auto &res_elem : res)
                  if (res_elem.m_idx == zero) al->select_atom(res_elem.m_atm);
              }
      }

  astate->make_viewport_dirty();

}

void simple_query::sel_invert() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  al->invert_selected_atoms();

  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_all() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  al->select_atoms(true);

  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_list(pybind11::list sel_list) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  for (auto itm : sel_list)
    if (py::isinstance<py::int_>(itm)) al->select_atom(py::cast<int>(itm));

  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_type(pybind11::str sel_type) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  std::string type_name = py::cast<std::string>(sel_type);
  int type_id = al->m_geom->type_of_atom(type_name);
  if (type_id != -1) al->select_by_type(type_id);

  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_sphere(vector3<float> sph_center, float sph_rad) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  std::vector<tws_node_content_t<float> > cnt;
  al->m_tws_tr->query_sphere(sph_rad, sph_center, cnt);

  for (auto &item : cnt)
    if (item.m_idx == index::D(al->m_geom->DIM).all(0))
      al->select_atom(item.m_atm);

  astate->make_viewport_dirty();

}

void simple_query::sel_hemisphere(int coord_idx, bool positive) {

  app_state_t *astate = app_state_t::get_inst();

  if (coord_idx < 0 || coord_idx > 2) return;

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  for (int i = 0; i < al->m_geom->nat(); i++)
    if (positive == al->m_geom->pos(i)[coord_idx] > -0.01)
      al->select_atom(i);

  astate->make_viewport_dirty();

}

void simple_query::edit_mode(int mode) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  if (mode == 0) cur_ws->set_edit_type(ws_edit_t::edit_item);
  else cur_ws->set_edit_type(ws_edit_t::edit_content);

  astate->make_viewport_dirty();

}

void simple_query::unsel_cnt_all() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  al->select_atoms(true);
  al->invert_selected_atoms();

  astate->make_viewport_dirty();

}

void simple_query::unsel_cnt(int cnt_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  al->unselect_atom(cnt_idx);

  astate->make_viewport_dirty();

}

void simple_query::unsel_cnt_list(pybind11::list sel_list) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  for (auto itm : sel_list)
    if (py::isinstance<py::int_>(itm)) al->unselect_atom(py::cast<int>(itm));

  astate->make_viewport_dirty();

}

void simple_query::unsel_cnt_type(pybind11::str sel_type) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  std::string type_name = py::cast<std::string>(sel_type);
  int type_id = al->m_geom->type_of_atom(type_name);
  if (type_id != -1) al->unselect_by_type(type_id);

  astate->make_viewport_dirty();

}

pybind11::list simple_query::get_sel() {

  app_state_t *astate = app_state_t::get_inst();
  py::list res;

  if (!astate->ws_manager->has_wss()) return py::none();

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return py::none();

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return py::none();

  for (auto &elem : al->m_atom_idx_sel)
    if (elem.m_idx == index::D(al->m_geom->DIM).all(0)) res.append(elem.m_atm);


  return res;
}

py::str simple_query::get_type_name() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return py::none();

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return py::none();

  auto cur_it = cur_ws->get_selected();
  if (cur_it) return cur_it->get_type_name();

  return "unknown";

}

py::int_ simple_query::get_type_hash() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return py::none();

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return py::none();

  auto cur_it = cur_ws->get_selected();

  if (cur_it) return cur_it->get_type();

  return py::none();

}

pybind11::bool_ simple_query::is_instance_of_by_hash(size_t _type_hash) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return false;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return false;

  auto cur_it = cur_ws->get_selected();

  if (cur_it) return cur_it->is_instance_of(_type_hash);

  return false;

}

pybind11::bool_ simple_query::is_instance_of_by_type_name(std::string _type_name) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return false;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return false;

  auto cur_it = cur_ws->get_selected();

  if (cur_it) return cur_it->get_type_name() == _type_name;

  return false;
}


vector3<float> simple_query::gizmo_pos() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return vector3<float>::Zero();

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return vector3<float>::Zero();

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return vector3<float>::Zero();

  if (al) return al->get_gizmo_content_barycenter();

  return vector3<float>::Zero();
}

std::tuple<std::string, vector3<float> >  simple_query::get_point_sym_group(float tolerance) {

  app_state_t *astate = app_state_t::get_inst();

  auto ret_empty = [](){return std::make_tuple("C1", vector3<float>{0});};

  if (!astate->ws_manager->has_wss()) return ret_empty();

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return ret_empty();

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return ret_empty();

  if (al && al->m_geom->DIM == 0) {
      array_group<matrix3<float> > G;
      vector3<float> new_center;
      find_point_symm(G, *(al->m_geom), new_center, tolerance);
      return std::make_tuple(G.name, new_center);
    }

  return ret_empty();

}

void simple_query::make_psg_view(float tolerance) {

  app_state_t *astate = app_state_t::get_inst();

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  auto cur_ws = astate->ws_manager->get_cur_ws();

  if (al && al->m_parent_ws && cur_ws) {

      if (cur_ws->m_edit_type == ws_edit_t::edit_item && al->m_geom->DIM == 0) {
          auto ws_pg = astate->ws_manager->m_bhv_mgr->fabric_by_type(
                         psg_view_t::get_type_static());
          auto ws_pg_c = ws_pg->cast_as<psg_view_t>();

          if (ws_pg_c) {
              ws_pg_c->m_connected_items.push_back(al->m_parent_ws->get_selected_sp());
              ws_pg_c->gen_from_geom(*al->m_geom, tolerance, true);
              al->m_parent_ws->add_item_to_ws(ws_pg);
              ws_pg_c->m_name =
                  fmt::format("point_sym_grp{}", al->m_parent_ws->m_ws_items.size());
            }

        } else if (cur_ws->m_edit_type == ws_edit_t::edit_content &&
                   !al->m_atom_idx_sel.empty()) {
          auto ws_pg_partial = astate->ws_manager->m_bhv_mgr->fabric_by_type(
                                 psg_view_t::get_type_static());
          auto ws_pg_partial_c = ws_pg_partial->cast_as<psg_view_t>();
          if (ws_pg_partial_c) {
              xgeometry<float, periodic_cell<float> > buffer{0};
              al->copy_to_xgeometry(buffer, true, false);
              ws_pg_partial_c->m_connected_items.push_back(al->m_parent_ws->get_selected_sp());
              ws_pg_partial_c->gen_from_geom(buffer, tolerance, true);
              al->m_parent_ws->add_item_to_ws(ws_pg_partial);
              ws_pg_partial_c->m_name =
                  fmt::format("point_sym_grp_part{}", al->m_parent_ws->m_ws_items.size());
            }
        }

    }

  astate->astate_evd->cur_ws_changed();

}

void simple_query::make_traj_highlight(size_t atom_id, size_t anim_id) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  auto traj_hl =
      astate->ws_manager->m_bhv_mgr->fabric_by_type(ws_traj_highlight_t::get_type_static());

  if (!traj_hl) return;

  traj_hl->m_name = fmt::format("traj_highlighter_{}", cur_ws->m_ws_items.size());

  auto traj_hl_casted = traj_hl->cast_as<ws_traj_highlight_t>();
  if (!traj_hl_casted) return;

  traj_hl_casted->m_anim_id = anim_id;
  traj_hl_casted->m_atm_id = atom_id;

  al->add_follower(traj_hl);
  cur_ws->add_item_to_ws(traj_hl);

  astate->make_viewport_dirty();

}

void simple_query::rebond() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  al->m_tws_tr->m_bonding_table.init_default(al->m_geom.get());
  al->m_tws_tr->do_action(act_rebuild_ntable);
  astate->astate_evd->cur_ws_selected_item_changed();

}

void simple_query::translate_selected(float tx, float ty, float tz) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  if (cur_ws->m_edit_type == ws_edit_t::edit_item) al->translate(vector3<float>(tx, ty, tz));
  else al->translate_selected(vector3<float>(tx, ty, tz));

  astate->make_viewport_dirty();

}

void simple_query::set_charge(float charge) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  if (al && cur_ws->m_edit_type == ws_edit_t::edit_content) {
      index zero = index::D(al->m_geom->DIM).all(0);
      for (auto &elem : al->m_atom_idx_sel)
        if (elem.m_idx == zero)
          al->m_geom->xfield<float>(xgeom_charge, elem.m_atm) = charge;
    }

  astate->make_viewport_dirty();

}

void simple_query::set_ws_bg(vector3<float> bg) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  cur_ws->m_background_color = bg;

}

void simple_query::add_atoms_list_0d(std::string name) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto new_al = std::make_shared<geom_view_t>();
  new_al->m_name = name;
  cur_ws->add_item_to_ws(new_al);

}

void simple_query::add_atoms_list_3d(std::string name,
                                     vector3<float> a,
                                     vector3<float> b,
                                     vector3<float> c) {
  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto new_al = std::make_shared<geom_view_t>();
  new_al->m_name = name;
  new_al->m_geom->DIM = 3;
  new_al->m_geom->cell.DIM = 3;
  new_al->m_geom->cell.v[0] = a;
  new_al->m_geom->cell.v[1] = b;
  new_al->m_geom->cell.v[2] = c;
  cur_ws->add_item_to_ws(new_al);

}

void simple_query::make_cube_p(std::string name,
                              vector3<float> pos,
                              float size_a,
                              float size_b,
                              float size_c) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto new_cube = std::make_shared<ws_cube_primitive_t>();
  new_cube->m_name = name;
  new_cube->m_pos = pos;
  new_cube->m_scale = {size_a, size_b, size_c};
  cur_ws->add_item_to_ws(new_cube);

}

void simple_query::make_arrow_p(std::string name,
                                vector3<float> from,
                                vector3<float> to) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto new_arr = std::make_shared<ws_arrow_primitive_t>();
  new_arr->m_name = name;
  new_arr->m_pos = from;
  new_arr->m_arrow_to = to;
  cur_ws->add_item_to_ws(new_arr);

}

void simple_query::convert_selected_units(spatial_units new_unit) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_manager->has_wss()) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto al = astate->ws_manager->get_sel_itm_sp_as<geom_view_t>();
  if (!al) return;

  float mod = new_unit == spatial_units_ang ? qpp::bohr_to_angs : qpp::ang_to_bohr;

  al->m_tws_tr->do_action(act_lock | act_clear_all);
  al->m_ext_obs->first_data = true;
  //transform cell
  for (int i = 0; i < al->m_geom->DIM; i++) al->m_geom->cell.v[i] *= mod;
  //transform content
  for (int i = 0; i < al->m_geom->nat(); i++) al->m_geom->coord(i) *= mod;
  al->geometry_changed();
  al->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
  al->m_tws_tr->do_action(act_rebuild_ntable);
}

float simple_query::get_isolevel() {

  app_state_t *astate = app_state_t::get_inst();

//  if (astate->ws_manager->has_wss()) {

//      auto cur_ws = astate->ws_manager->get_cur_ws();

//      if (cur_ws) {

//          auto cur_it_vol = dynamic_cast<ws_volume_data_t*>(cur_ws->get_selected());

//          if (cur_it_vol) {
//              return cur_it_vol->m_isolevel;
//            }
//        }

//      astate->make_viewport_dirty();

//    }

  return 0.0f;
}

void simple_query::set_isolevel(float new_isolevel) {

  app_state_t *astate = app_state_t::get_inst();

//  if (astate->ws_manager->has_wss()) {

//      auto cur_ws = astate->ws_manager->get_cur_ws();

//      if (cur_ws) {

//          auto cur_it_vol = dynamic_cast<ws_volume_data_t*>(cur_ws->get_selected());

//          if (cur_it_vol) {
//              cur_it_vol->update_isolevel(new_isolevel);
//            }
//        }

//      astate->make_viewport_dirty();

//    }
}

void simple_query::set_sel_color_vec(vector3<float> color) {
  set_sel_color(color[0], color[1], color[2]);
}

void simple_query::set_sel_color(float r, float g, float b) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {

          auto cur_it_al = dynamic_cast<geom_view_t*>(cur_ws->get_selected());

          if (cur_it_al)
            for (auto &elem : cur_it_al->m_atom_idx_sel) {
                cur_it_al->m_geom->xfield<float>(xgeom_ccr, elem.m_atm) = r;
                cur_it_al->m_geom->xfield<float>(xgeom_ccg, elem.m_atm) = g;
                cur_it_al->m_geom->xfield<float>(xgeom_ccb, elem.m_atm) = b;

                if (cur_it_al->m_anim->get_total_anims() > 0 &&
                    !cur_it_al->m_anim->m_anim_data[0].frames.empty() &&
                    cur_it_al->m_anim->m_anim_data[0].frames[0].atom_color.size() ==
                    cur_it_al->m_geom->nat()) {
                    cur_it_al->m_anim->m_anim_data[0].frames[0].atom_color[elem.m_atm][0] = r;
                    cur_it_al->m_anim->m_anim_data[0].frames[0].atom_color[elem.m_atm][1] = g;
                    cur_it_al->m_anim->m_anim_data[0].frames[0].atom_color[elem.m_atm][2] = b;
                  }

              }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::ptable_set_color_by_number(int num, float r, float g, float b) {

  app_state_t *astate = app_state_t::get_inst();

  if (num > 0 && num <100) {
      ptable_atom_record &rec = ptable::get_inst()->arecs[num-1];
      rec.m_color_jmol = vector3<float>(r, g, b);
      rec.m_redefined = true;
      astate->astate_evd->cur_ws_selected_item_changed();
      astate->astate_evd->python_console_focus_requested();
    }
}

void simple_query::ptable_set_color_by_name(std::string name, float r, float g, float b) {
  auto ap_idx = ptable::number_by_symbol(name);
  if (ap_idx) ptable_set_color_by_number(*ap_idx, r, g, b);
}

void simple_query::ptable_set_radius_by_number(int num, float r) {
  if (num > 0 && num <100) {
      ptable_atom_record &rec = ptable::get_inst()->arecs[num-1];
      rec.m_radius = r;
      rec.m_redefined = true;
    }
}

void simple_query::ptable_set_radius_by_name(std::string name, float r) {
  auto ap_idx = ptable::number_by_symbol(name);
  if (ap_idx) ptable_set_radius_by_number(*ap_idx, r);
}

vector3<float> simple_query::ptable_get_color_by_number(int num) {
  if (num > 0 && num <100) return ptable::get_inst()->arecs[num-1].m_color_jmol;
  else return vector3<float>::Zero();
}

vector3<float> simple_query::ptable_get_color_by_name(std::string name) {
  auto ap_idx = ptable::number_by_symbol(name);
  if (ap_idx) return ptable_get_color_by_number(*ap_idx);
  else return vector3<float>::Zero();
}

float simple_query::ptable_get_radius_by_number(int num) {
  if (num > 0 && num <100) return ptable::get_inst()->arecs[num-1].m_radius;
  else return 0.0f;
}

float simple_query::ptable_get_radius_by_name(std::string name) {
  auto ap_idx = ptable::number_by_symbol(name);
  if (ap_idx) return ptable_get_radius_by_number(*ap_idx);
  else return 0.0f;
}

void simple_query::camera_move(vector3<float> axis, float magnitude) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      astate->camera->translate_camera(axis * magnitude);
      astate->camera->update_camera();
    }
}

void simple_query::camera_rotate_yaw(float magnitude) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      astate->camera->rotate_camera_orbit_yaw(magnitude);
      astate->camera->update_camera();
    }

}

void simple_query::camera_rotate_pitch(float magnitude) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      astate->camera->rotate_camera_orbit_pitch(magnitude);
      astate->camera->update_camera();
    }

}

void simple_query::camera_zoom(float magnitude) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      astate->camera->update_camera_zoom(magnitude);
      astate->camera->update_camera();
    }

}

void simple_query::camera_mode(int mode) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      if (mode == 0) astate->camera->set_projection(cam_proj_t::proj_persp);
      else astate->camera->set_projection(cam_proj_t::proj_ortho);
      astate->camera->update_camera();
    }
}

void simple_query::copy_camera_from_ws(int ws_id) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->camera) {
      auto ws_ref = astate->ws_manager->get_ws(ws_id);
      if (ws_ref) astate->camera->copy_from_camera(*(ws_ref->m_camera));
    }
}

pybind11::list simple_query::sv_get() {

  py::list ret;

  app_state_t *astate = app_state_t::get_inst();
  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<geom_view_t*>(cur_ws->get_selected());
          if (_al) {
              for (int i = 0; i < _al->m_geom->nat(); i++)
                ret.append(_al->m_geom->xfield<bool>(xgeom_sel_vis, i));
            }
        }
      astate->make_viewport_dirty();
    }
  return ret;
}

void simple_query::sv_edit(int at, bool status) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<geom_view_t*>(cur_ws->get_selected());
          if (_al) {
              _al->m_geom->xfield<bool>(xgeom_sel_vis, at) = status;
            }
        }
      astate->make_viewport_dirty();
    }
}

void simple_query::sv_edit_list(pybind11::list at_list, bool status) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<geom_view_t*>(cur_ws->get_selected());
          if (_al) {
              for (auto elem : at_list)
                if (py::isinstance<py::int_>(elem))
                  _al->m_geom->xfield<bool>(xgeom_sel_vis, elem.cast<py::int_>()) =
                      status;
            }
        }
      astate->make_viewport_dirty();
    }
}

void simple_query::sv_edit_all(bool status) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<geom_view_t*>(cur_ws->get_selected());
          if (_al)
            for (int i = 0; i < _al->m_geom->nat(); i++)
              _al->m_geom->xfield<bool>(xgeom_sel_vis,i) = status;
        }
      astate->make_viewport_dirty();
    }
}

pybind11::list simple_query::cl_get() {

  py::list ret;

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<geom_view_t*>(cur_ws->get_selected());
          if (_al) {
              for (int i = 0; i < _al->m_geom->nat(); i++)
                ret.append(_al->m_geom->xfield<bool>(xgeom_label_state, i));
            }
        }
      astate->make_viewport_dirty();
    }
  return ret;

}

void simple_query::set_cl_state(int at, bool status) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<geom_view_t*>(cur_ws->get_selected());
          if (_al) {
              _al->m_geom->xfield<bool>(xgeom_label_state, at) = status;
            }
        }
      astate->make_viewport_dirty();
    }
}

void simple_query::set_cl_text(int at, std::string text) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<geom_view_t*>(cur_ws->get_selected());
          if (_al) {
              _al->m_geom->xfield<std::string>(xgeom_label_text, at) = text;
            }
        }
      astate->make_viewport_dirty();
    }
}
