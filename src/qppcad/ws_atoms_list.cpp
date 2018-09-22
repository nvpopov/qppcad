#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/ws_atoms_list_context_menu.hpp>
#include <qppcad/ws_atoms_list_obj_insp.hpp>
#include <qppcad/app.hpp>
#include <io/geomio.hpp>
#include <io/vasp_io.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_atoms_list_t::ws_atoms_list_t():ws_item_t(){


  m_geom = make_unique<xgeometry<float, periodic_cell<float> > >(3,"rg1");

  m_geom->set_format({"atom", "number", "charge", "x", "y", "z", "show", "sel",
                      "cc", "ccr", "ccg", "ccb"},

  {type_string, type_int, type_real, type_real, type_real, type_real,
   type_bool, type_bool, type_bool, type_real, type_real, type_real});

  m_geom->DIM = 0;
  m_geom->cell.DIM = 0;

  m_ext_obs = make_unique<extents_observer_t<float> >(*m_geom);
  m_tws_tr  = make_unique<tws_tree_t<float> >(*m_geom);
  m_tws_tr->m_auto_bonding = true;

  m_show_imaginary_atoms = true;
  m_show_imaginary_bonds = true;
  m_show_atoms = true;
  m_show_bonds = true;
  m_draw_line_in_dist_measurement = false;

  //parent->add_item_to_workspace(this->shared_from_this());

}

void ws_atoms_list_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                            vector3<float> &out_look_at){
  if(m_geom->nat() > 2){
      out_look_at += (m_ext_obs->aabb.max + m_ext_obs->aabb.min) / 2.0;
      vector3<float> bb_size = m_ext_obs->aabb.max - m_ext_obs->aabb.min;
      float size = bb_size.norm();

      out_look_pos  +=
          2.35f * m_ext_obs->aabb.max.normalized() * clamp<float>(size, 10.0, 100.0);
    }
  else out_look_pos += vector3<float>(0.0, 0.0, -5.0);

}

void ws_atoms_list_t::geometry_changed(){
  m_aabb = m_ext_obs->aabb;
}

void ws_atoms_list_t::render(){
  ws_item_t::render();
  //we need it for lambda fn

  app_state_t* astate = &(c_app::get_state());
  vector3<float> _pos = m_pos;
  if (app_state_c->dp != nullptr){

      if (astate->debug_show_tws_tree){
          astate->dp->begin_render_aabb();
          m_tws_tr->apply_visitor( [astate, _pos](tws_node_t<float> *in_node, int deep_level){
              astate->dp->render_aabb(clr_maroon, in_node->m_bb.min+_pos, in_node->m_bb.max+_pos);});
          astate->dp->end_render_aabb();
        }

      if (m_geom->DIM == 3 && m_is_visible && m_draw_cell){
          astate->dp->begin_render_line();
          vector3<float> cell_clr = clr_black;
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
          for (uint16_t j = 0; j < m_tws_tr->n(i); j++){

              uint16_t id1 = i;
              uint16_t id2 = m_tws_tr->table_atm(i,j);
              index idx2 = m_tws_tr->table_idx(i,j);

              render_bond(id1, index::D(m_geom->DIM).all(0), id2, idx2);

              if(idx2 != index::D(m_geom->DIM).all(0) && m_show_imaginary_bonds)
                render_bond(id2, idx2, id1, index::D(m_geom->DIM).all(0));
            }

      if (m_geom->DIM > 0 && m_show_imaginary_bonds && m_show_bonds)
        for (const auto &img_atom : m_tws_tr->m_img_atoms)
          for (const auto &img_bond : img_atom.m_img_bonds){

              uint16_t id1 = img_atom.m_atm;
              uint16_t id2 = img_bond.m_atm;

              index idx1 = img_atom.m_idx;
              index idx2 = img_bond.m_idx;

              render_bond(id1, idx1, id2, idx2);
            }

      astate->dp->end_render_bond();

    }

  //render measurement
  if (m_selected && m_draw_line_in_dist_measurement && m_atom_idx_selection.size() == 2){
      auto fa = m_atom_idx_selection.cbegin();
      auto la  = ++(m_atom_idx_selection.cbegin());
      astate->dp->begin_render_line();
      astate->dp->render_line(clr_white, m_geom->pos(fa->m_atm, fa->m_idx),
                              m_geom->pos(la->m_atm, la->m_idx), 6.0f);
      astate->dp->end_render_line();
    }
}

void ws_atoms_list_t::render_atom(const uint16_t at_num, const index &at_index){

  auto ap_idx = ptable::number_by_symbol(m_geom->atom(at_num));
  float dr_rad = 0.4f;
  vector3<float> color(0.0, 0.0, 1.0);

  if(ap_idx){
      dr_rad = ptable::get_inst()->arecs[*ap_idx-1].aRadius * m_atom_scale_factor;
      color = ptable::get_inst()->arecs[*ap_idx-1].aColorJmol;
    }

  if(parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT){
      if(m_atom_idx_selection.find(atom_index_set_key(at_num, at_index)) !=
         m_atom_idx_selection.end()
         && m_selected)
        color = vector3<float>(0.43f, 0.55f, 0.12f);
    }

  app_state_c->dp->render_atom(color, m_geom->pos(at_num, at_index) + m_pos, dr_rad);
}

void ws_atoms_list_t::render_bond(const uint16_t atNum1, const index &atIndex1,
                                  const uint16_t atNum2, const index &atIndex2){
  auto ap_idx = ptable::number_by_symbol(m_geom->atom(atNum1));
  vector3<float> bcolor(0.0, 0.0, 1.0);
  if(ap_idx){bcolor = ptable::get_inst()->arecs[*ap_idx-1].aColorJmol;}
  app_state_c->dp->render_bond(bcolor, m_geom->pos(atNum1, atIndex1) + m_pos,
                               m_geom->pos(atNum2, atIndex2) + m_pos,
                               m_bond_scale_factor);
}

void ws_atoms_list_t::render_ui(){
  ws_item_t::render_ui();
  ws_atoms_list_obj_insp_helper::render_ui(this);
}

void ws_atoms_list_t::td_context_menu_edit_item(){
  ws_item_t::td_context_menu_edit_item();
}

void ws_atoms_list_t::td_context_menu_edit_content(){
  ws_item_t::td_context_menu_edit_content();
  ws_atoms_list_context_menu_helper::render_content_edit_menu(this);
}

bool ws_atoms_list_t::mouse_click(ray_t<float> *click_ray){

  if (click_ray){

      vector<tws_query_data_t<float> > res;
      //we need to translate ray in world frame to local geometry frame
      ray_t<float> local_geom_ray;
      local_geom_ray.start = click_ray->start - m_pos;
      local_geom_ray.dir = click_ray->dir;
      m_tws_tr->query_ray<query_ray_add_all<float> >(&local_geom_ray, res, m_atom_scale_factor);
      //std::cout << "res_size = " << res.size() << std::endl;
      std::sort(res.begin(), res.end(), tws_query_data_sort_by_dist<float>);
      recalc_gizmo_barycenter();

      if (!res.empty()){
          if (parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT && m_selected ){
              atom_index_set_key iskey(res[0].m_atm, res[0].m_idx);
              auto atom_sel_it = m_atom_idx_selection.find(iskey);
              if (atom_sel_it == m_atom_idx_selection.end()){
                  m_atom_idx_selection.insert(iskey);
                  m_atom_selection.insert(res[0].m_atm);
                } else {
                  m_atom_idx_selection.erase(atom_sel_it);
                  auto it = m_atom_selection.find(res[0].m_atm);
                  if (it != m_atom_selection.end())
                    m_atom_selection.erase(m_atom_selection.find(res[0].m_atm));
                }
            };

          recalc_gizmo_barycenter();
          return true;
        } else {
          //TODO: need refractoring
          if (parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT && m_selected ) {
              m_atom_selection.clear();
              m_atom_idx_selection.clear();
            }
        }
    }
  return false;
}

void ws_atoms_list_t::select_atoms(bool all){
  if (all) {
      m_atom_selection.clear();
      m_atom_idx_selection.clear();
      for (auto i = 0; i < m_geom->nat(); i++){
          m_atom_selection.insert(i);
          m_atom_idx_selection.insert(atom_index_set_key(i, index::D(m_geom->DIM).all(0)));
        }

    } else {
      m_atom_selection.clear();
      m_atom_idx_selection.clear();
    }
}

void ws_atoms_list_t::invert_selected_atoms(){
  m_atom_idx_selection.clear();
  for (auto i = 0; i < m_geom->nat(); i++){
      auto it = m_atom_selection.find(i);

      if (it != m_atom_selection.end()){
          m_atom_selection.erase(it);
          for (iterator idx(index::D(m_geom->DIM).all(-1), index::D(m_geom->DIM).all(1));
               !idx.end(); idx++ ) {
              auto i2 = m_atom_idx_selection.find(atom_index_set_key(i, idx));
              if (i2 != m_atom_idx_selection.end()) m_atom_idx_selection.erase(i2);
            }
        } else {
          m_atom_selection.insert(i);
          m_atom_idx_selection.insert(atom_index_set_key(i, index::D(m_geom->DIM).all(0)));
        }
    }
}

void ws_atoms_list_t::insert_atom(const int atom_type, const vector3<float> &pos){
  m_geom->add(m_geom->atom_of_type(atom_type), pos);
}

void ws_atoms_list_t::insert_atom(const string &atom_name, const vector3<float> &pos){
  m_geom->add(atom_name, pos);
}

void ws_atoms_list_t::update_atom(const int at_id, const vector3<float> &pos){
  m_geom->change_pos(at_id, pos);
}

void ws_atoms_list_t::update_atom(const int at_id, const string &at_name){
  m_geom->change(at_id, at_name, m_geom->pos(at_id));
}

void ws_atoms_list_t::delete_selected_atoms(){
  vector<int> all_atom_num;
  all_atom_num.reserve(m_atom_idx_selection.size());

  //get unique selected atoms
  for(auto &elem : m_atom_idx_selection) all_atom_num.push_back(elem.m_atm);
  auto uniq_atoms_last = std::unique(all_atom_num.begin(), all_atom_num.end());
  all_atom_num.erase(uniq_atoms_last, all_atom_num.end());

  //sort by ancending order
  std::sort(all_atom_num.begin(), all_atom_num.end());

  m_atom_idx_selection.clear();
  m_atom_selection.clear();

  for (uint16_t delta = 0; delta < all_atom_num.size(); delta++) {
      if (delta == 0 && all_atom_num.size() > 1) m_tws_tr->freeze();
      if ((delta == all_atom_num.size() - 1) && all_atom_num.size() > 1) m_tws_tr->unfreeze();
      m_geom->erase(all_atom_num[delta] - delta);
    }
}

bool ws_atoms_list_t::support_translation(){return true;}

bool ws_atoms_list_t::support_rotation(){return false;}

bool ws_atoms_list_t::support_scaling(){return  false;}

bool ws_atoms_list_t::support_content_editing(){return true;}

bool ws_atoms_list_t::support_selection(){return true;}

bool ws_atoms_list_t::support_rendering_bounding_box(){return m_geom->DIM > 0;}

std::string ws_atoms_list_t::compose_item_name(){
  return fmt::format("Type = [atom list], DIM = [{}d]", m_geom->DIM);
}

void ws_atoms_list_t::update(float delta_time){
  ws_item_t::update(delta_time);
}

float ws_atoms_list_t::get_bb_prescaller(){
  if (m_geom->DIM == 3) return 1.5f;
  return 1.1f;
}

uint32_t ws_atoms_list_t::get_amount_of_selected_content(){
  return this->m_atom_selection.size();
}

void ws_atoms_list_t::on_begin_content_gizmo_translate(){
  //c_app::log(fmt::format("Start of translating node [{}] content", name));
  m_tws_tr->m_auto_bonding = false;
  m_tws_tr->m_auto_build   = false;
}

void ws_atoms_list_t::apply_intermediate_translate_content(const vector3<float> &pos){
  bool someone_from_atoms_were_translated = false;
  for (const uint16_t &at_idx : m_atom_selection){
      vector3<float> acc_pos = m_geom->coord(at_idx) + pos;
      m_geom->change_pos(at_idx, acc_pos);
      someone_from_atoms_were_translated = true;
    }
  if (someone_from_atoms_were_translated) recalc_gizmo_barycenter();
}

void ws_atoms_list_t::on_end_content_gizmo_translate(){
  c_app::log(fmt::format("End of translating node [{}] content", m_name));
  m_tws_tr->m_auto_bonding = true;
  m_tws_tr->m_auto_build   = true;
}

void ws_atoms_list_t::recalc_gizmo_barycenter(){
  //barycenter in local frame
  m_gizmo_barycenter = vector3<float>::Zero();

  if (!m_atom_idx_selection.empty() || m_geom->nat() == 0){
      for (const auto& atm_idx : m_atom_idx_selection)
        m_gizmo_barycenter += m_geom->pos(atm_idx.m_atm, atm_idx.m_idx);
      m_gizmo_barycenter /= m_atom_idx_selection.size();
    }
  else m_gizmo_barycenter = m_aabb.min;

}

const vector3<float> ws_atoms_list_t::get_gizmo_content_barycenter(){
  return m_gizmo_barycenter;
}

void ws_atoms_list_t::shift(const vector3<float> shift){
  m_tws_tr->m_auto_bonding = false;
  m_tws_tr->m_auto_build   = false;

  for (int i = 0; i < m_geom->nat(); i++)
    m_geom->coord(i) = shift + m_geom->pos(i) ;

  m_ext_obs->aabb.min = shift + m_ext_obs->aabb.min;
  m_ext_obs->aabb.max = shift + m_ext_obs->aabb.max;
  m_tws_tr->apply_shift(shift);

  m_tws_tr->m_auto_bonding = true;
  m_tws_tr->m_auto_build   = true;
  geometry_changed();
}

void ws_atoms_list_t::load_from_file(qc_file_format file_format,
                                     std::string file_name,
                                     bool auto_center){

  c_app::log(fmt::format("Loading geometry from file {} to ws_atom_list in workspace {}",
                         file_name, parent_ws->m_ws_name));

  std::ifstream qc_data(file_name);
  if (!qc_data) {
      c_app::log(fmt::format("Error in loading from file {}", file_name));
      return;
    }

  //clean geom and tws-tree
  m_tws_tr->m_auto_bonding = false;
  m_tws_tr->m_auto_build   = false;
  m_tws_tr->clr_ntable();
  m_tws_tr->clr_tree();
  m_ext_obs->first_data = true;

  m_name = extract_base_name(file_name);

  switch (file_format) {
    case qc_file_format::format_standart_xyz:
      m_geom->DIM = 0;
      read_xyz(qc_data, *(m_geom));
      break;

    case qc_file_format::format_vasp_poscar:
      m_geom->DIM = 3;
      m_geom->cell.DIM = 3;
      read_vasp_poscar(qc_data, *(m_geom));
      break;

    default: c_app::log("File format not implemented");
    }

  qc_data.close();

  if(auto_center){
      vector3<float> center(0.0, 0.0, 0.0);
      for (int i = 0; i < m_geom->nat(); i++)
        center += m_geom->pos(i);
      center *= (1.0f / m_geom->nat());
      for (int i = 0; i < m_geom->nat(); i++)
        m_geom->coord(i) = -center + m_geom->pos(i) ;

      m_ext_obs->aabb.min = -center + m_ext_obs->aabb.min;
      m_ext_obs->aabb.max = -center + m_ext_obs->aabb.max;
    }

  m_tws_tr->manual_build();
  m_tws_tr->find_all_neighbours();
  m_tws_tr->m_auto_bonding = true;
  m_tws_tr->m_auto_build   = true;

  geometry_changed();

  if (parent_ws) parent_ws->workspace_changed();

}

void ws_atoms_list_t::rebuild_ngbt(){

  need_to_rebuild_nbt = false;
}
