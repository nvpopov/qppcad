#include <qppcad/python/python_simple_query.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>
#include <qppcad/ws_item/traj_hl/traj_hl.hpp>
#include <qppcad/ws_item/cube_primitive/cube_primitive.hpp>
#include <qppcad/ws_item/arrow_primitive/arrow_primitive.hpp>
#include <data/ptable.hpp>
#include <qppcad/tools/supercell/supercell.hpp>
#include <symm/point_groups.hpp>

using namespace qpp;
using namespace qpp::cad;

namespace py = pybind11;

std::shared_ptr<workspace_manager_t> simple_query::get_ws_mgr() {

  app_state_t *astate = app_state_t::get_inst();
  return astate->ws_mgr;

}

void simple_query::set_font_size(int new_font_size) {
  app_state_t *astate = app_state_t::get_inst();
  astate->m_console_font_size = new_font_size;
  astate->astate_evd->python_console_font_size_updated();
}

std::tuple<std::string, std::string> simple_query::get_build_info() {
  return {build_info_helper::get_git_version(),
          build_info_helper::get_build_date()};
}

void simple_query::open_file(std::string file_name,
                             bool to_current) {

  app_state_t *astate = app_state_t::get_inst();

  astate->ws_mgr->load_from_file_autodeduce(file_name, "", !to_current);
  astate->make_viewport_dirty();
  astate->astate_evd->python_console_focus_requested();

}

void simple_query::open_file_query(std::string file_name,
                                   std::string file_format,
                                   bool to_current) {

  app_state_t *astate = app_state_t::get_inst();

  astate->ws_mgr->load_from_file_autodeduce(file_name, file_format, !to_current);
  astate->make_viewport_dirty();
  astate->astate_evd->python_console_focus_requested();

}

void simple_query::make_super_cell(const int sc_a, const int sc_b, const int sc_c) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  supercell_tool_t sct;
  sct.make_super_cell(al, sc_a, sc_b, sc_c);
  cur_ws->set_selected_item(cur_ws->m_ws_items.size()-1);
  astate->astate_evd->python_console_focus_requested();
  astate->make_viewport_dirty();

}

void simple_query::select_ws(int ws_idx) {

  app_state_t *astate = app_state_t::get_inst();
  if (!astate->ws_mgr->has_wss()) return;

  astate->ws_mgr->set_cur_id(opt<size_t>(ws_idx));
  astate->astate_evd->cur_ws_changed();
  astate->make_viewport_dirty();

}

void simple_query::select_itm(int itm_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_mgr->has_wss()) return;

  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (!cur_ws) return;
  cur_ws->set_selected_item(itm_idx);

  astate->make_viewport_dirty();

  astate->astate_evd->python_console_focus_requested();

}

void simple_query::sel_cnt(int cnt_idx) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);
  al->select_atom(cnt_idx);
  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_fn(std::function<bool (float, float, float)> cfn) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  for (size_t i = 0; i < al->m_geom->nat(); i++)
    if (cfn(al->m_geom->pos(i)[0], al->m_geom->pos(i)[1], al->m_geom->pos(i)[2]))
     al->select_atom(i);

  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_parity() {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

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
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);
  al->invert_selected_atoms();
  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_all() {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);
  al->select_atoms(true);
  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_list(pybind11::list sel_list) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  for (auto itm : sel_list)
    if (py::isinstance<py::int_>(itm)) al->select_atom(py::cast<int>(itm));

  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_type(pybind11::str sel_type) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  std::string type_name = py::cast<std::string>(sel_type);
  int type_id = al->m_geom->type_of_atom(type_name);
  if (type_id != -1) al->select_by_type(type_id);

  astate->make_viewport_dirty();

}

void simple_query::sel_cnt_sphere(vector3<float> sph_center, float sph_rad) {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

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

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  for (int i = 0; i < al->m_geom->nat(); i++)
    if (positive == al->m_geom->pos(i)[coord_idx] > -0.01)
      al->select_atom(i);

  astate->make_viewport_dirty();

}

void simple_query::edit_mode(int mode) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_mgr->has_wss()) return;

  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (!cur_ws) return;

  if (mode == 0) cur_ws->set_edit_type(ws_edit_e::edit_item);
  else cur_ws->set_edit_type(ws_edit_e::edit_content);

  astate->make_viewport_dirty();

}

void simple_query::unsel_cnt_all() {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);
  al->select_atoms(true);
  al->invert_selected_atoms();
  astate->make_viewport_dirty();

}

void simple_query::unsel_cnt(int cnt_idx) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);
  al->unselect_atom(cnt_idx);
  astate->make_viewport_dirty();

}

void simple_query::unsel_cnt_list(pybind11::list sel_list) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  for (auto itm : sel_list)
    if (py::isinstance<py::int_>(itm)) al->unselect_atom(py::cast<int>(itm));

  astate->make_viewport_dirty();

}

void simple_query::unsel_cnt_type(pybind11::str sel_type) {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  std::string type_name = py::cast<std::string>(sel_type);
  int type_id = al->m_geom->type_of_atom(type_name);
  if (type_id != -1) al->unselect_by_type(type_id);

  astate->make_viewport_dirty();

}

pybind11::list simple_query::get_sel() {

  app_state_t *astate = app_state_t::get_inst();
  py::list res;

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  if (!al) return py::none();

  for (auto &elem : al->m_atom_idx_sel)
    if (elem.m_idx == index::D(al->m_geom->DIM).all(0)) res.append(elem.m_atm);

  return res;

}

pybind11::list simple_query::get_tool_groups() {

  py::list ret;

  app_state_t *astate = app_state_t::get_inst();
  for (auto &tg : astate->ws_mgr->m_bhv_mgr->m_tools_groups)
    ret.append(py::str(tg.second.m_full_name));

  return ret;

}

pybind11::list simple_query::get_tool_names() {

  py::list ret;

  app_state_t *astate = app_state_t::get_inst();
  for (auto &tg : astate->ws_mgr->m_bhv_mgr->m_tools_info)
    ret.append(py::str(tg.second.m_full_name));

  return ret;

}

void simple_query::exec_tool(std::shared_ptr<ws_item_t> ws_item, std::string tool_name) {

  app_state_t *astate = app_state_t::get_inst();
  astate->ws_mgr->m_bhv_mgr->exec_tool_by_name(tool_name, ws_item.get());
  astate->make_viewport_dirty();

}

py::str simple_query::get_type_name() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_mgr->has_wss()) return py::none();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  if (!cur_ws) return py::none();
  if (cur_it) return cur_it->get_type_name();

  return "unknown";

}

py::int_ simple_query::get_type_hash() {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_mgr->has_wss()) return py::none();

  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (!cur_ws) return py::none();

  auto cur_it = cur_ws->get_selected();
  if (cur_it) return cur_it->get_type();

  return py::none();

}

py::list simple_query::get_connected_items(std::shared_ptr<ws_item_t> ws_item) {

  py::list retl;

  if (ws_item)
    for (auto &elem : ws_item->m_connected_items)
      if (elem) retl.append(elem->m_name);

  return retl;

}

py::list simple_query::get_followers(std::shared_ptr<ws_item_t> ws_item) {

  py::list retl;

  if (ws_item)
    for (auto &elem : ws_item->m_followers)
      if (elem) retl.append(elem->m_name);

  return retl;

}

py::str simple_query::get_leader(std::shared_ptr<ws_item_t> ws_item) {

  if (ws_item->m_leader) return ws_item->m_leader->m_name;
  else return py::none();

}

pybind11::bool_ simple_query::is_instance_of_by_hash(size_t _type_hash) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_mgr->has_wss()) return false;

  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (!cur_ws) return false;

  auto cur_it = cur_ws->get_selected();
  if (cur_it) return cur_it->is_instance_of(_type_hash);

  return false;

}

pybind11::bool_ simple_query::is_instance_of_by_type_name(std::string _type_name) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_mgr->has_wss()) return false;
  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (!cur_ws) return false;
  auto cur_it = cur_ws->get_selected();
  if (cur_it) return cur_it->get_type_name() == _type_name;
  return false;

}


vector3<float> simple_query::gizmo_pos() {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  if (!cur_ws) return vector3<float>::Zero();
  if (!al) return vector3<float>::Zero();
  if (al) return al->get_gizmo_content_barycenter();
  return vector3<float>::Zero();

}

std::tuple<std::string, vector3<float> >  simple_query::get_point_sym_group(float tolerance) {

  app_state_t *astate = app_state_t::get_inst();

  auto ret_empty = [](){return std::make_tuple("C1", vector3<float>{0});};
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  if (!astate->ws_mgr->has_wss()) return ret_empty();
  if (!cur_ws) return ret_empty();
  if (!al) return ret_empty();

  if (al && al->m_geom->DIM == 0) {
      array_group<matrix3<float> > G;
      vector3<float> new_center;
      find_point_symm(G, *(al->m_geom), new_center, tolerance);
      return std::make_tuple(G.name, new_center);
    }

  return ret_empty();

}

void simple_query::embed_cube() {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  if (!al) {
      throw std::invalid_argument("embed_cube : select geom_view_t");
      return;
    }

  if (!al->m_parent_ws || !cur_ws) {
      throw std::invalid_argument("embed_cube : invalid context");
      return;
    }

  if (cur_ws->m_edit_type != ws_edit_e::edit_content) {
      throw std::invalid_argument("embed_cube : edit_type != edit_content");
      return;
    }

  if (al->m_atom_idx_sel.size() != 2) {
      throw std::invalid_argument("embed_cube : sel.size() != 2 ");
      return;
    }

  auto it1 = al->m_atom_idx_sel.begin();
  auto it2 = ++(al->m_atom_idx_sel.begin());

  vector3<float> p1 = al->m_geom->pos(it1->m_atm, it1->m_idx) + al->m_pos;
  vector3<float> p2 = al->m_geom->pos(it2->m_atm, it2->m_idx) + al->m_pos;

  vector3<float> center = (p1 + p2) * 0.5f;
  vector3<float> len{0};

  for (size_t i = 0; i < 3; i++) len[i] = std::max(
                                            std::max(p1[i], p2[i]) - std::min(p1[i], p2[i]),
                                            0.1f);

  auto new_cube = std::make_shared<cube_primitive_t>();
  new_cube->m_name = fmt::format("cube{}", cur_ws->m_ws_items.size());
  new_cube->m_pos = center;
  new_cube->m_scale = len;
  cur_ws->add_item_to_ws(new_cube);

}

void simple_query::make_psg_view(float tolerance) {

  app_state_t *astate = app_state_t::get_inst();

  auto cur_ws = astate->ws_mgr->get_cur_ws();

  if (!cur_ws) {
      return;
    }

  auto cur_it = cur_ws->get_selected();
  if (!cur_it) {
      return;
    }

  auto al = cur_it->cast_as<geom_view_t>();
  if (!al) {
      return;
    }

  if (al && al->m_parent_ws && cur_ws) {

      if (cur_ws->m_edit_type == ws_edit_e::edit_item && al->m_geom->DIM == 0) {
          auto ws_pg = astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(
                         psg_view_t::get_type_static());
          auto ws_pg_c = ws_pg->cast_as<psg_view_t>();

          if (ws_pg_c) {
              al->add_follower(ws_pg);
              ws_pg_c->set_bounded_to_leader(true);
              ws_pg_c->gen_from_geom(*al->m_geom, tolerance, true);
              al->m_parent_ws->add_item_to_ws(ws_pg);
              ws_pg_c->m_name =
                  fmt::format("point_sym_grp{}", al->m_parent_ws->m_ws_items.size());
            }

        } else if (cur_ws->m_edit_type == ws_edit_e::edit_content &&
                   !al->m_atom_idx_sel.empty()) {
          auto ws_pg_partial = astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(
                                 psg_view_t::get_type_static());
          auto ws_pg_partial_c = ws_pg_partial->cast_as<psg_view_t>();
          if (ws_pg_partial_c) {
              al->add_follower(ws_pg_partial);
              ws_pg_partial_c->set_bounded_to_leader(true);
              xgeometry<float, periodic_cell<float> > buffer{0};
              al->copy_to_xgeometry(buffer, true, false);
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

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  auto traj_hl = astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(traj_hl_t::get_type_static());
  if (!traj_hl) return;

  traj_hl->m_name = fmt::format("traj_highlighter_{}", cur_ws->m_ws_items.size());

  auto traj_hl_casted = traj_hl->cast_as<traj_hl_t>();
  if (!traj_hl_casted) return;

  traj_hl_casted->m_anim_id = anim_id;
  traj_hl_casted->m_atm_id = atom_id;

  al->add_follower(traj_hl);
  cur_ws->add_item_to_ws(traj_hl);

  astate->make_viewport_dirty();

}

//TODO: need to update
std::vector<std::string> simple_query::get_xgeom_dfn() {

  return {
      "atom", //0
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
      "ltext",//11
      "override",//12
      "atom_r", // 13
      "external" //14
    };

}

std::vector<basic_types> simple_query::get_xgeom_dft() {

  return  {
          type_string, //0
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
          type_real //14
    };
}

void simple_query::rebond() {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  al->m_tws_tr->m_bonding_table.init_default(al->m_geom.get());
  al->m_tws_tr->do_action(act_rebuild_ntable);
  astate->astate_evd->cur_ws_selected_item_changed();

}

void simple_query::translate_selected(float tx, float ty, float tz) {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  if (cur_ws->m_edit_type == ws_edit_e::edit_item) al->translate(vector3<float>(tx, ty, tz));
  else al->translate_selected(vector3<float>(tx, ty, tz));

  astate->make_viewport_dirty();

}

void simple_query::set_charge(float charge) {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  if (al && cur_ws->m_edit_type == ws_edit_e::edit_content) {
      index zero = index::D(al->m_geom->DIM).all(0);
      for (auto &elem : al->m_atom_idx_sel)
        if (elem.m_idx == zero)
          al->m_geom->xfield<float>(xgeom_charge, elem.m_atm) = charge;
    }

  astate->make_viewport_dirty();

}

void simple_query::set_ws_bg(vector3<float> bg) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_mgr->has_wss()) return;

  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (!cur_ws) return;

  cur_ws->m_background_color = bg;

}

void simple_query::add_atoms_list_0d(std::string name) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_mgr->has_wss()) return;

  auto cur_ws = astate->ws_mgr->get_cur_ws();
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

  if (!astate->ws_mgr->has_wss()) return;

  auto cur_ws = astate->ws_mgr->get_cur_ws();
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

  if (!astate->ws_mgr->has_wss()) return;

  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (!cur_ws) return;

  auto new_cube = std::make_shared<cube_primitive_t>();
  new_cube->m_name = name;
  new_cube->m_pos = pos;
  new_cube->m_scale = {size_a, size_b, size_c};
  cur_ws->add_item_to_ws(new_cube);

}

void simple_query::make_arrow_p(std::string name,
                                vector3<float> from,
                                vector3<float> to) {

  app_state_t *astate = app_state_t::get_inst();

  if (!astate->ws_mgr->has_wss()) return;

  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (!cur_ws) return;

  auto new_arr = std::make_shared<arrow_primitive_t>();
  new_arr->m_name = name;
  new_arr->m_pos = from;
  new_arr->m_arrow_to = to;
  cur_ws->add_item_to_ws(new_arr);

}

void simple_query::convert_selected_units(spatial_units_e new_unit) {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

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

  return 0.0f;
}

void simple_query::set_isolevel(float new_isolevel) {

  app_state_t *astate = app_state_t::get_inst();

}

void simple_query::set_sel_color_vec(vector3<float> color) {
  set_sel_color(color[0], color[1], color[2]);
}

void simple_query::set_sel_color(float r, float g, float b) {

  app_state_t *astate = app_state_t::get_inst();

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  for (auto &elem : al->m_atom_idx_sel) {
  al->m_geom->xfield<float>(xgeom_ccr, elem.m_atm) = r;
  al->m_geom->xfield<float>(xgeom_ccg, elem.m_atm) = g;
  al->m_geom->xfield<float>(xgeom_ccb, elem.m_atm) = b;

  if (al->m_anim->get_total_anims() > 0 &&
      !al->m_anim->m_anim_data[0].frames.empty() &&
      al->m_anim->m_anim_data[0].frames[0].atom_color.size() ==
      al->m_geom->nat()) {
      al->m_anim->m_anim_data[0].frames[0].atom_color[elem.m_atm][0] = r;
      al->m_anim->m_anim_data[0].frames[0].atom_color[elem.m_atm][1] = g;
      al->m_anim->m_anim_data[0].frames[0].atom_color[elem.m_atm][2] = b;
    }

  }

  astate->make_viewport_dirty();

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

float simple_query::camera_get_zoom() {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera && astate->camera->m_cur_proj == cam_proj_t::proj_ortho)
    return astate->camera->m_ortho_scale;
  return 0.0f;

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
      auto ws_ref = astate->ws_mgr->get_ws(ws_id);
      if (ws_ref) astate->camera->copy_from_camera(*(ws_ref->m_camera));
    }

}

pybind11::list simple_query::sv_get() {

  py::list ret;
  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  for (int i = 0; i < al->m_geom->nat(); i++)
    ret.append(al->m_geom->xfield<bool>(xgeom_sel_vis, i));

  astate->make_viewport_dirty();
  return ret;

}

void simple_query::sv_edit(int at, bool status) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);
  al->m_geom->xfield<bool>(xgeom_sel_vis, at) = status;
  astate->make_viewport_dirty();

}

void simple_query::sv_edit_list(pybind11::list at_list, bool status) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  for (auto elem : at_list)
    if (py::isinstance<py::int_>(elem))
      al->m_geom->xfield<bool>(xgeom_sel_vis, elem.cast<py::int_>()) = status;

  astate->make_viewport_dirty();

}

void simple_query::sv_edit_all(bool status) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  for (int i = 0; i < al->m_geom->nat(); i++) al->m_geom->xfield<bool>(xgeom_sel_vis,i) = status;

  astate->make_viewport_dirty();

}

pybind11::list simple_query::cl_get() {

  py::list ret;
  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);

  for (int i = 0; i < al->m_geom->nat(); i++)
    ret.append(al->m_geom->xfield<bool>(xgeom_label_show, i));

  astate->make_viewport_dirty();
  return ret;

}

void simple_query::set_cl_state(int at, bool status) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);
  al->m_geom->xfield<bool>(xgeom_label_show, at) = status;
  astate->make_viewport_dirty();

}

void simple_query::set_cl_text(int at, std::string text) {

  app_state_t *astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);
  al->m_geom->xfield<std::string>(xgeom_label_text, at) = text;
  astate->make_viewport_dirty();

}
