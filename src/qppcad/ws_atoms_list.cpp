#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/ws_atoms_list_context_menu.hpp>
#include <qppcad/ws_atoms_list_obj_insp.hpp>
#include <qppcad/app.hpp>
#include <io/geomio.hpp>
#include <io/vasp_io.hpp>
#include <io/xyz_multiframe.hpp>
#include <clocale>

using namespace qpp;
using namespace qpp::cad;

ws_atoms_list_t::ws_atoms_list_t():ws_item_t () {

  m_geom = make_unique<xgeometry<float, periodic_cell<float> > >(3,"rg1");

  m_geom->set_format({"atom", "number", "charge", "x", "y", "z", "show", "sel",
                      "cc", "ccr", "ccg", "ccb"},

  {type_string, type_int, type_real, type_real, type_real, type_real,
   type_bool, type_bool, type_bool, type_real, type_real, type_real});

  m_geom->DIM = 0;
  m_geom->cell.DIM = 0;

  m_ext_obs = make_unique<extents_observer_t<float> >(*m_geom);
  m_tws_tr  = make_unique<tws_tree_t<float> >(*m_geom);
  m_tws_tr->do_action(act_unlock);

  //parent->add_item_to_workspace(this->shared_from_this());

}

void ws_atoms_list_t::vote_for_view_vectors (vector3<float> &out_look_pos,
                                             vector3<float> &out_look_at) {
  if (m_geom->nat() > 2) {
      out_look_at += (m_ext_obs->aabb.max + m_ext_obs->aabb.min) / 2.0;
      vector3<float> bb_size = m_ext_obs->aabb.max - m_ext_obs->aabb.min;
      float size = bb_size.norm();

      out_look_pos  +=
          2.35f * m_ext_obs->aabb.max.normalized() * clamp<float>(size, 10.0, 100.0);
    }
  else out_look_pos += vector3<float>(0.0, 0.0, -5.0);

}

void ws_atoms_list_t::geometry_changed () {
  m_aabb = m_ext_obs->aabb;
}

void ws_atoms_list_t::render () {
  ws_item_t::render();
  //we need it for lambda fn

  app_state_t* astate = &(c_app::get_state());
  vector3<float> _pos = m_pos;
  index all_null = index::D(m_geom->DIM).all(0);
  if (app_state_c->dp != nullptr){

      if (astate->debug_show_tws_tree) {
          astate->dp->begin_render_aabb();
          m_tws_tr->apply_visitor( [astate, _pos](tws_node_t<float> *in_node, int deep_level){
              astate->dp->render_aabb(clr_maroon, in_node->m_bb.min+_pos, in_node->m_bb.max+_pos);});
          astate->dp->end_render_aabb();
        }

      if (m_geom->DIM == 3 && m_is_visible && m_draw_cell) {
          astate->dp->begin_render_line();
          vector3<float> cell_clr = m_cell_color;
          if (m_selected){
              if(parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM)  cell_clr = clr_red;
              if(parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT) cell_clr = clr_maroon;
            }

          app_state_c->dp->render_cell_3d(
                cell_clr, m_geom->cell.v[0], m_geom->cell.v[1], m_geom->cell.v[2], m_pos, 2.1f);
          astate->dp->end_render_line();
        }

      if (!m_is_visible) return;

      // atom render start
      astate->dp->begin_atom_render();

      // draw {0,..} atoms
      for (uint32_t i = 0; i < m_geom->nat(); i++)
        if (m_show_atoms &&
            m_atom_type_to_hide.find(m_geom->type_table(i)) == m_atom_type_to_hide.end())
          render_atom(i, index::D(m_geom->DIM).all(0));

      // draw imaginary atoms that appear due to periodic
      if (m_geom->DIM > 0 && m_show_atoms && m_show_imaginary_atoms)
        for (const auto &at_img : m_tws_tr->m_img_atoms)
          if (m_atom_type_to_hide.find(m_geom->type_table(at_img.m_atm)) ==
              m_atom_type_to_hide.end())
            render_atom(at_img.m_atm, at_img.m_idx);

      astate->dp->end_atom_render();
      // atom render end

      // bond render
      astate->dp->begin_render_bond();

      if (m_show_bonds)
        for (uint16_t i = 0; i < m_geom->nat(); i++)
          for (uint16_t j = 0; j < m_tws_tr->n(i); j++) {

              uint16_t id1 = i;
              uint16_t id2 = m_tws_tr->table_atm(i,j);
              index idx2 = m_tws_tr->table_idx(i,j);

              if (idx2 == all_null || m_show_imaginary_bonds)
                render_bond(id1, all_null, id2, idx2);

              if (idx2 != all_null && m_show_imaginary_bonds)
                render_bond(id2, idx2, id1, index::D(m_geom->DIM).all(0));
            }

      if (m_geom->DIM > 0 && m_show_imaginary_bonds && m_show_bonds)
        for (const auto &img_atom : m_tws_tr->m_img_atoms)
          for (const auto &img_bond : img_atom.m_img_bonds) {

              uint16_t id1 = img_atom.m_atm;
              uint16_t id2 = img_bond.m_atm;

              index idx1 = img_atom.m_idx;
              index idx2 = img_bond.m_idx;

              render_bond(id1, idx1, id2, idx2);
            }

      astate->dp->end_render_bond();

    }

  //render measurement
  if (m_selected && m_draw_line_in_dist_measurement && m_atom_idx_sel.size() == 2) {
      auto fa = m_atom_idx_sel.cbegin();
      auto la  = ++(m_atom_idx_sel.cbegin());
      astate->dp->begin_render_line();
      astate->dp->render_line(clr_white, m_geom->pos(fa->m_atm, fa->m_idx),
                              m_geom->pos(la->m_atm, la->m_idx), 6.0f);
      astate->dp->end_render_line();
    }
}

void ws_atoms_list_t::render_atom (const uint32_t at_num, const index &at_index) {

  auto ap_idx = ptable::number_by_symbol(m_geom->atom(at_num));
  float dr_rad = 0.4f;
  vector3<float> color(0.0, 0.0, 1.0);

  if (ap_idx) {
      dr_rad = ptable::get_inst()->arecs[*ap_idx-1].aRadius * m_atom_scale_factor;
      color = ptable::get_inst()->arecs[*ap_idx-1].aColorJmol;
    }

  if (parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT) {
      if (m_atom_idx_sel.find(atom_index_set_key(at_num, at_index)) != m_atom_idx_sel.end()
          && m_selected)
        color = vector3<float>(0.43f, 0.55f, 0.12f);
    }

  app_state_c->dp->render_atom(color, m_geom->pos(at_num, at_index) + m_pos, dr_rad);
}

void ws_atoms_list_t::render_bond (const uint32_t atNum1, const index &atIndex1,
                                   const uint32_t atNum2, const index &atIndex2) {
  auto ap_idx = ptable::number_by_symbol(m_geom->atom(atNum1));
  vector3<float> bcolor(0.0, 0.0, 1.0);
  if(ap_idx){bcolor = ptable::get_inst()->arecs[*ap_idx-1].aColorJmol;}
  app_state_c->dp->render_bond(bcolor, m_geom->pos(atNum1, atIndex1) + m_pos,
                               m_geom->pos(atNum2, atIndex2) + m_pos,
                               m_bond_scale_factor);
}

void ws_atoms_list_t::render_ui () {
  ws_item_t::render_ui();
  ws_atoms_list_obj_insp_helper::render_ui(this);
}

void ws_atoms_list_t::td_context_menu_edit_item () {
  ws_item_t::td_context_menu_edit_item();
}

void ws_atoms_list_t::td_context_menu_edit_content () {
  ws_item_t::td_context_menu_edit_content();
  ws_atoms_list_context_menu_helper::render_content_edit_menu(this);
}

bool ws_atoms_list_t::mouse_click (ray_t<float> *click_ray) {

  if (click_ray){

      vector<tws_query_data_t<float> > res;
      //we need to translate ray in world frame to local geometry frame
      ray_t<float> local_geom_ray;
      local_geom_ray.start = click_ray->start - m_pos;
      local_geom_ray.dir = click_ray->dir;
      m_tws_tr->query_ray<query_ray_add_all<float> >(&local_geom_ray, res, m_atom_scale_factor);

      recalc_gizmo_barycenter();

      if (!res.empty()) {
          std::sort(res.begin(), res.end(), tws_query_data_sort_by_dist<float>);
          if (parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT && m_selected ) {
              atom_index_set_key iskey(res[0].m_atm, res[0].m_idx);
              auto atom_sel_it = m_atom_idx_sel.find(iskey);
              if (atom_sel_it == m_atom_idx_sel.end()) {
                  m_atom_idx_sel.insert(iskey);
                  m_atom_sel.insert(res[0].m_atm);
                } else {
                  m_atom_idx_sel.erase(atom_sel_it);
                  auto it = m_atom_sel.find(res[0].m_atm);
                  if (it != m_atom_sel.end()) m_atom_sel.erase(m_atom_sel.find(res[0].m_atm));
                }
            };

          recalc_gizmo_barycenter();
          return true;
        } else {
          //TODO: need refractoring
          if (parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT && m_selected ) {
              m_atom_sel.clear();
              m_atom_idx_sel.clear();
            }
        }
    }
  return false;
}

void ws_atoms_list_t::select_atoms (bool all) {
  if (all) {
      m_atom_sel.clear();
      m_atom_idx_sel.clear();
      for (auto i = 0; i < m_geom->nat(); i++) {
          m_atom_sel.insert(i);
          m_atom_idx_sel.insert(atom_index_set_key(i, index::D(m_geom->DIM).all(0)));
        }
    } else {
      m_atom_sel.clear();
      m_atom_idx_sel.clear();
    }
}

void ws_atoms_list_t::invert_selected_atoms () {
  m_atom_idx_sel.clear();
  for (auto i = 0; i < m_geom->nat(); i++){
      auto it = m_atom_sel.find(i);

      if (it != m_atom_sel.end()) {
          m_atom_sel.erase(it);
          for (iterator idx(index::D(m_geom->DIM).all(-1), index::D(m_geom->DIM).all(1));
               !idx.end(); idx++ ) {
              auto i2 = m_atom_idx_sel.find(atom_index_set_key(i, idx));
              if (i2 != m_atom_idx_sel.end()) m_atom_idx_sel.erase(i2);
            }
        } else {
          m_atom_sel.insert(i);
          m_atom_idx_sel.insert(atom_index_set_key(i, index::D(m_geom->DIM).all(0)));
        }
    }
}

void ws_atoms_list_t::insert_atom(const int atom_type, const vector3<float> &pos){
  m_force_non_animable = true;
  m_geom->add(m_geom->atom_of_type(atom_type), pos);
}

void ws_atoms_list_t::insert_atom(const string &atom_name, const vector3<float> &pos){
  m_force_non_animable = true;
  m_geom->add(atom_name, pos);
}

void ws_atoms_list_t::update_atom(const int at_id, const vector3<float> &pos){
  m_force_non_animable = true;
  m_geom->change_pos(at_id, pos);
}

void ws_atoms_list_t::update_atom(const int at_id, const string &at_name){
  m_force_non_animable = true;
  m_geom->change(at_id, at_name, m_geom->pos(at_id));
}

void ws_atoms_list_t::delete_selected_atoms () {

  if (!m_atom_idx_sel.empty() || !m_atom_sel.empty()) m_force_non_animable = true;

  vector<int> all_atom_num;
  all_atom_num.reserve(m_atom_idx_sel.size());

  //get unique selected atoms
  for(auto &elem : m_atom_idx_sel) all_atom_num.push_back(elem.m_atm);
  auto uniq_atoms_last = std::unique(all_atom_num.begin(), all_atom_num.end());
  all_atom_num.erase(uniq_atoms_last, all_atom_num.end());

  //sort by ancending order
  std::sort(all_atom_num.begin(), all_atom_num.end());

  m_atom_idx_sel.clear();
  m_atom_sel.clear();

  for (uint16_t delta = 0; delta < all_atom_num.size(); delta++) {
      if (delta == 0 && all_atom_num.size() > 1)
        m_tws_tr->do_action(act_lock);
      if ((delta == all_atom_num.size() - 1) && all_atom_num.size() > 1)
        m_tws_tr->do_action(act_unlock);
      m_geom->erase(all_atom_num[delta] - delta);
    }
}

bool ws_atoms_list_t::animable(){
  if (m_force_non_animable) return false;
  if (m_anim.empty()) return false;
  for (auto &anim : m_anim)
    if (anim.frame_data.empty()) return false;
  return true;
}

void ws_atoms_list_t::update_geom_to_anim(const int anim_id,
                                          const float current_frame){
  float start_frame = int(current_frame);
  float end_frame   = std::ceil(current_frame);
  float frame_delta = 1 - (current_frame - start_frame);
  int start_frame_n = int(start_frame);
  int end_frame_n   = int(end_frame);

  //TODO: throw
  if (anim_id > m_anim.size()) return;

  if (!m_rebuild_bonds_in_anim) m_tws_tr->do_action(act_lock);
  else if (m_geom->DIM > 0) m_tws_tr->do_action(act_lock_img);

  for (auto i = 0; i < m_geom->nat(); i++){

      if (m_anim[anim_id].frame_data[start_frame_n].size() != m_geom->nat()){
          m_force_non_animable = true;
          return;
        }
      //std::cout << m_anim[anim_id].frame_data[start_frame_n][i].to_string_vec() <<"\n" << std::endl ;
      vector3<float> new_pos = m_anim[anim_id].frame_data[start_frame_n][i] * (frame_delta) +
                               m_anim[anim_id].frame_data[end_frame_n][i] * (1-frame_delta);
      m_geom->change_pos(i, new_pos);
    }

  if (!m_rebuild_bonds_in_anim) m_tws_tr->do_action(act_unlock);
  else if (m_geom->DIM > 0) m_tws_tr->do_action(act_unlock_img);
}

bool ws_atoms_list_t::support_translation () { return true; }

bool ws_atoms_list_t::support_rotation () { return false; }

bool ws_atoms_list_t::support_scaling () { return  false; }

bool ws_atoms_list_t::support_content_editing () { return true; }

bool ws_atoms_list_t::support_selection () { return true; }

bool ws_atoms_list_t::support_rendering_bounding_box () { return m_geom->DIM > 0; }

std::string ws_atoms_list_t::compose_item_name () {
  return fmt::format("Type = [atom list], DIM = [{}d]", m_geom->DIM);
}

void ws_atoms_list_t::update (float delta_time) {
  ws_item_t::update(delta_time);

  if (m_cur_anim >= m_anim.size()) return; // wrong animation index
  if (m_anim[m_cur_anim].frame_data.empty()) return;
  //if (m_anim[m_cur_anim].frame_data[0].si)
  if (m_play_anim && animable()) {
      m_cur_anim_time += 1 / (m_anim_frame_time*60);
      if (m_cur_anim_time > m_anim[m_cur_anim].frame_data.size() - 1) {
          if (m_play_cyclic) m_cur_anim_time = 0.0f;
          else {
              m_play_anim = false;
              m_cur_anim_time = m_anim[m_cur_anim].frame_data.size() - 1;
            }
        } else {
          update_geom_to_anim(m_cur_anim, m_cur_anim_time);
        }

      //if current anim type equals static -> update to static and switch m_cur_anim_time = 0
      if (m_anim[m_cur_anim].m_anim_type == geom_anim_type::anim_static){
          m_cur_anim_time = 0.0f;
          m_play_anim = false;
          m_play_cyclic = false;
        }
    }
}

float ws_atoms_list_t::get_bb_prescaller () {
  if (m_geom->DIM == 3) return 1.5f;
  return 1.1f;
}

uint32_t ws_atoms_list_t::get_amount_of_selected_content() {
  return this->m_atom_sel.size();
}

void ws_atoms_list_t::on_begin_content_gizmo_translate(){
  //c_app::log(fmt::format("Start of translating node [{}] content", name));
  m_tws_tr->do_action(act_lock);
}

void ws_atoms_list_t::apply_intermediate_translate_content(const vector3<float> &pos) {
  bool someone_from_atoms_were_translated = false;
  for (const uint16_t &at_idx : m_atom_sel){
      vector3<float> acc_pos = m_geom->coord(at_idx) + pos;
      m_geom->change_pos(at_idx, acc_pos);
      someone_from_atoms_were_translated = true;
    }
  if (someone_from_atoms_were_translated) recalc_gizmo_barycenter();
}

void ws_atoms_list_t::on_end_content_gizmo_translate() {
  c_app::log(fmt::format("End of translating node [{}] content", m_name));
  m_tws_tr->do_action(act_unlock);
}

void ws_atoms_list_t::recalc_gizmo_barycenter() {
  //barycenter in local frame
  m_gizmo_barycenter = vector3<float>::Zero();

  if (!m_atom_idx_sel.empty() || m_geom->nat() == 0){
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

void ws_atoms_list_t::load_from_file(qc_file_format file_format, std::string file_name,
                                     bool auto_center) {

  std::setlocale(LC_ALL, "C");

  c_app::log(fmt::format("Loading geometry from file {} to ws_atom_list in workspace {}",
                         file_name, parent_ws->m_ws_name));

  std::ifstream qc_data(file_name);
  if (!qc_data) {
      c_app::log(fmt::format("Error in loading from file {}", file_name));
      return;
    }

  //clean geom and tws-tree
  m_tws_tr->do_action(act_lock | act_clear_all);
  m_ext_obs->first_data = true;

  m_name = extract_base_name(file_name);

  switch (file_format) {
    case qc_file_format::format_standart_xyz:
      m_geom->DIM = 0;
      read_xyz(qc_data, *(m_geom));
      break;

    case qc_file_format::format_multi_frame_xyz:
      m_geom->DIM = 0;
      m_anim.clear();
      read_xyz_multiframe(qc_data, *(m_geom), m_anim);
      break;

    case qc_file_format::format_vasp_poscar:
      m_geom->DIM = 3;
      m_geom->cell.DIM = 3;
      read_vasp_poscar(qc_data, *(m_geom));
      break;

    case qc_file_format::format_vasp_outcar_md:
      m_geom->DIM = 3;
      m_geom->cell.DIM = 3;
      read_vasp_outcar_md_with_frames(qc_data, *(m_geom), m_anim);
      break;

    default: c_app::log("File format not implemented");
    }

  qc_data.close();

  if (auto_center) {
      vector3<float> center(0.0, 0.0, 0.0);
      for (int i = 0; i < m_geom->nat(); i++)
        center += m_geom->pos(i);
      center *= (1.0f / m_geom->nat());
      for (int i = 0; i < m_geom->nat(); i++)
        m_geom->coord(i) = -center + m_geom->pos(i) ;

      m_ext_obs->aabb.min = -center + m_ext_obs->aabb.min;
      m_ext_obs->aabb.max = -center + m_ext_obs->aabb.max;
    }

  m_tws_tr->do_action(act_unlock | act_rebuild_all);
  geometry_changed();
  if (parent_ws) parent_ws->workspace_changed();

}

string ws_atoms_list_t::get_ws_item_class_name() {
  return "ws_atoms_list";
}

void ws_atoms_list_t::write_to_json(json &data) {
  ws_item_t::write_to_json(data);

  data[JSON_DIM] = m_geom->DIM;
  data[JSON_SHOW_IMG_ATOMS] = m_show_imaginary_atoms;
  data[JSON_SHOW_IMG_BONDS] = m_show_imaginary_bonds;
  data[JSON_SHOW_BONDS] = m_show_bonds;
  data[JSON_SHOW_ATOMS] = m_show_atoms;
  data[JSON_BT_SHOW_DSBL] = m_bonding_table_show_disabled_record;
  data[JSON_ATOM_SCALE] = m_atom_scale_factor;
  data[JSON_BOND_SCALE] = m_bond_scale_factor;
  data[JSON_CELL_COLOR] = json::array({m_cell_color[0], m_cell_color[1], m_cell_color[2]});
  data[JSON_BONDING_TABLE] = json::array({});

  for (auto&& [key, value] : m_tws_tr->m_bonding_table.m_dist) {
      json bt_rec = json::array({});
      bt_rec.push_back(m_geom->atom_of_type(key.m_a));
      bt_rec.push_back(m_geom->atom_of_type(key.m_b));
      bt_rec.push_back(value.m_bonding_dist);
      bt_rec.push_back(value.m_enabled);
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
      data[JSON_ATOMS].push_back(atom);
    }
}

void ws_atoms_list_t::read_from_json(json &data) {
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
    m_show_imaginary_atoms = data[JSON_SHOW_IMG_ATOMS];

  if (data.find(JSON_SHOW_IMG_BONDS) != data.end())
    m_show_imaginary_bonds = data[JSON_SHOW_IMG_BONDS];

  if (data.find(JSON_SHOW_BONDS) != data.end())
    m_show_atoms = data[JSON_SHOW_BONDS];

  if (data.find(JSON_SHOW_ATOMS) != data.end())
    m_show_bonds = data[JSON_SHOW_ATOMS];

  if (data.find(JSON_BT_SHOW_DSBL) != data.end())
    m_bonding_table_show_disabled_record = data[JSON_BT_SHOW_DSBL];

  m_tws_tr->do_action(act_lock | act_clear_all);

  m_ext_obs->first_data = true;

  if (m_geom->DIM>0) {
      if (data.find(JSON_CELL) != data.end()){
          for (uint8_t i = 0; i < m_geom->DIM; i++){
              vector3<float> cellv(data[JSON_CELL][i][0].get<float>(),
                  data[JSON_CELL][i][1].get<float>(),
                  data[JSON_CELL][i][2].get<float>());
              m_geom->cell.v[i] = cellv;
            }
        } else {
          m_geom->DIM = 0;
          c_app::log("Cannot load cell data for geom with DIM>0");
        }
    }

  if (data.find(JSON_ATOMS) != data.end())
    for (const auto &atom : data[JSON_ATOMS]){
        m_geom->add(atom[0].get<string>(),
            vector3<float>(atom[1].get<float>(), atom[2].get<float>(), atom[3].get<float>()));
      }

  if (data.find(JSON_BONDING_TABLE) != data.end()){
      for (auto &elem : data[JSON_BONDING_TABLE]){
          int type1 = m_geom->type_of_atom(elem[0].get<string>());
          int type2 = m_geom->type_of_atom(elem[1].get<string>());
          float dist = elem[2].get<float>();
          bool br_enabled = elem[3].get<bool>();
          m_tws_tr->m_bonding_table.m_dist[sym_key<uint32_t>(type1, type2)].m_bonding_dist = dist;
          m_tws_tr->m_bonding_table.m_dist[sym_key<uint32_t>(type1, type2)].m_enabled = br_enabled;
        }
      m_tws_tr->m_bonding_table.update_pair_max_dist_all();
    }

  geometry_changed();
  m_tws_tr->do_action(act_unlock | act_build_all);


}

