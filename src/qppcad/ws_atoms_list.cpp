#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/app.hpp>
#include <io/geomio.hpp>
#include <io/vasp_io.hpp>

using namespace qpp;

ws_atoms_list_t::ws_atoms_list_t(workspace_t* parent):ws_item_t(parent){


  geom = unique_ptr<xgeometry<float, periodic_cell<float> > >(
        new xgeometry<float, periodic_cell<float> >(3,"rg1")
        );

  geom->set_format({"atom", "number", "charge", "x", "y", "z", "show", "sel"},

  {type_string, type_int, type_real, type_real, type_real, type_real,
   type_bool, type_bool});

  geom->DIM = 0;
  geom->cell.DIM = 0;

  ext_obs = unique_ptr<extents_observer_t<float> >(new extents_observer_t<float>(*geom));
  tws_tr  = unique_ptr<tws_tree_t<float> >(new tws_tree_t<float>(*geom));
  tws_tr->auto_bonding = true;

  show_imaginary_atoms = true;
  show_imaginary_bonds = true;
  show_atoms = true;
  show_bonds = true;
  parent->add_item_to_workspace(this);

}

void ws_atoms_list_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
                                            vector3<float> &vOutLookAt){
  if(geom->nat() > 2){
      vOutLookAt += (ext_obs->aabb.max + ext_obs->aabb.min) / 2.0;
      vector3<float> vSize = ext_obs->aabb.max - ext_obs->aabb.min;
      float fSize = vSize.norm();

      vOutLookPos  +=
          ext_obs->aabb.max.normalized() * clamp<float>(fSize, 10.0, 100.0);
    }
  else vOutLookPos += vector3<float>(0.0, 0.0, -5.0);

}

void ws_atoms_list_t::geometry_changed(){
  aabb = ext_obs->aabb;
}

void ws_atoms_list_t::render(){
  ws_item_t::render();
  //we need it for lambda fn

  app_state_t* astate = &(c_app::get_state());
  vector3<float> _pos = pos;
  if (app_state_c->dp != nullptr){

      if (astate->debug_show_tws_tree){
          astate->dp->begin_render_aabb();
          tws_tr->apply_visitor( [astate, _pos](tws_node_t<float> *in_node, int deep_level){
              astate->dp->render_aabb(clr_maroon, in_node->bb.min+_pos, in_node->bb.max+_pos);});
          astate->dp->end_render_aabb();
        }

      if (geom->DIM == 3 && b_show && b_draw_cell){
          astate->dp->begin_render_line();
          vector3<float> cell_clr = clr_black;
          if (is_selected){
              if(parent_ws->cur_edit_type == ws_edit_type::EDIT_WS_ITEM)  cell_clr = clr_red;
              if(parent_ws->cur_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT) cell_clr =clr_lime;
            }

          app_state_c->dp->render_cell_3d(
                cell_clr, geom->cell.v[0], geom->cell.v[1],geom->cell.v[2], pos, 1.1f);
          astate->dp->end_render_line();
        }

      if (!b_show) return;

      // atom render start
      astate->dp->begin_atom_render();

      // draw {0,..} atoms
      for (int i = 0; i < geom->nat(); i++)
        if (show_atoms) render_atom(i, index::D(geom->DIM).all(0));

      // draw imaginary atoms that appear due to periodic
      if (geom->DIM > 0 && show_atoms && show_imaginary_atoms)
        for (uint16_t i = 0; i < tws_tr->img_atoms.size(); i++)
          render_atom(tws_tr->img_atoms[i].atm, tws_tr->img_atoms[i].idx);

      astate->dp->end_atom_render();
      // atom render end

      // bond render
      astate->dp->begin_render_bond();

      if (show_bonds)
        for (uint16_t i = 0; i < geom->nat(); i++)
          for (uint16_t j = 0; j < tws_tr->n(i); j++){

              uint16_t id1 = i;
              uint16_t id2 = tws_tr->table_atm(i,j);
              index idx2 = tws_tr->table_idx(i,j);

              render_bond(id1, index::D(geom->DIM).all(0), id2, idx2);

              if(idx2 != index::D(geom->DIM).all(0) && show_imaginary_bonds)
                render_bond(id2, idx2, id1, index::D(geom->DIM).all(0));
            }

      if (geom->DIM > 0 && show_imaginary_bonds)
        for (uint16_t i = 0; i < tws_tr->img_atoms.size(); i++)
          for (uint16_t j = 0; j < tws_tr->img_atoms[i].img_bonds.size(); j++){

              uint16_t id1 = tws_tr->img_atoms[i].atm;
              uint16_t id2 = tws_tr->img_atoms[i].img_bonds[j]->atm;

              index idx1 = tws_tr->img_atoms[i].idx;
              index idx2 = tws_tr->img_atoms[i].img_bonds[j]->idx;

              render_bond(id1, idx1, id2, idx2);
            }


      astate->dp->end_render_bond();

    }
}

void ws_atoms_list_t::render_atom(const uint16_t atNum, const index &atIndex){

  int ap_idx = ptable::number_by_symbol(geom->atom(atNum));
  float fDrawRad = 0.4f;
  vector3<float> color(0.0, 0.0, 1.0);
  if(ap_idx != -1){
      fDrawRad = ptable::get_inst()->arecs[ap_idx-1].aRadius * app_state_c->atom_radius_scale_factor;
      color = ptable::get_inst()->arecs[ap_idx-1].aColorJmol;
    }

  if(parent_ws->cur_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT)
    if(atom_selection.find(atNum) != atom_selection.end() && is_selected)
      color = vector3<float>(0.43f, 0.55f, 0.12f);

  app_state_c->dp->render_atom(color, geom->pos(atNum, atIndex) + pos, fDrawRad);
}

void ws_atoms_list_t::render_bond(const uint16_t atNum1, const index &atIndex1,
                                  const uint16_t atNum2, const index &atIndex2){
  int ap_idx = ptable::number_by_symbol(geom->atom(atNum1));
  vector3<float> bcolor(0.0, 0.0, 1.0);
  if(ap_idx != -1){bcolor = ptable::get_inst()->arecs[ap_idx-1].aColorJmol;}
  app_state_c->dp->render_bond(bcolor, geom->pos(atNum1, atIndex1)+ pos,
                               geom->pos(atNum2, atIndex2)+ pos,
                               app_state_c->bond_radius_scale_factor);
}

void ws_atoms_list_t::render_ui(){
  ws_item_t::render_ui();

  if (ImGui::CollapsingHeader("Summary")){
      ImGui::Spacing();
      ImGui::Columns(2);
      ImGui::Text("Total atoms:");
      ImGui::Text("Atom types:");
      ImGui::NextColumn();
      ImGui::Text(fmt::format("{}", geom->nat()).c_str());
      ImGui::Text(fmt::format("{}", geom->n_atom_types()).c_str());
      ImGui::Columns(1);
      ImGui::Spacing();
    }


  if (ImGui::CollapsingHeader("Atom types")){
      ImGui::Spacing();
      ImGui::Columns(3);
      ImGui::Text("Atom type");
      ImGui::NextColumn();
      ImGui::Text("Count");
      ImGui::NextColumn();
      ImGui::Text("Color");
      ImGui::NextColumn();
      ImGui::Separator();
      for (uint8_t i = 0; i < geom->n_types(); i++){
          ImGui::Text(geom->atom_of_type(i).c_str());
        }
      ImGui::NextColumn();

      for (uint8_t i = 0; i < geom->n_types(); i++){
          ImGui::Text(fmt::format("{}",geom->get_atom_count_by_type(i)).c_str());
        }
      ImGui::NextColumn();

      ImDrawList* draw_list = ImGui::GetWindowDrawList();

      for (uint8_t i = 0; i < geom->n_types(); i++){
          const ImVec2 p = ImGui::GetCursorScreenPos();
          float p_x = p.x + 8 + 6;
          float p_y = p.y + 8;
          ImVec2 p_n(p_x, p_y);
          int ap_idx = ptable::number_by_symbol(geom->atom_of_type(i));
          vector3<float> bc(0.0, 0.0, 1.0);
          if(ap_idx != -1) {bc = ptable::get_inst()->arecs[ap_idx-1].aColorJmol;}
          draw_list->AddCircleFilled(p_n, 8, ImColor(ImVec4(bc[0], bc[1], bc[2], 1.0f)));
          ImGui::Dummy(ImVec2(16, 16));
        }
      ImGui::NextColumn();

      ImGui::Columns(1);
      ImGui::Spacing();
    }

  if (ImGui::CollapsingHeader("Display and styling")){
      ImGui::Checkbox("Show atoms", &show_atoms);
      ImGui::Checkbox("Show bonds", &show_bonds);
      if (geom->DIM > 0) {
          ImGui::Checkbox("Draw periodic cell", &b_draw_cell);
          ImGui::Checkbox("Show imaginary atoms", &show_imaginary_atoms);
          ImGui::Checkbox("Show imaginary bonds", &show_imaginary_bonds);
        }

    }

  if (ImGui::CollapsingHeader("Modify")){

    }
}

bool ws_atoms_list_t::mouse_click(ray_t<float> *click_ray){
  if (click_ray){
      vector<unique_ptr<tws_query_data_t<float> > > res;
      //we need to translate ray in world frame to local geometry frame
      ray_t<float> local_geom_ray;
      local_geom_ray.start = click_ray->start - pos;
      local_geom_ray.dir = click_ray->dir;
      tws_tr->query_ray<query_ray_add_all<float> >(&local_geom_ray, res);
      //std::cout << "res_size = " << res.size() << std::endl;
      std::sort(res.begin(), res.end(), tws_query_data_sort_by_dist<float>);
      recalc_gizmo_barycenter();
      if (res.size() > 0){
          if (parent_ws->cur_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT && is_selected ){

              auto atom_sel_it = atom_selection.find(res[0]->atm);
              if (atom_sel_it == atom_selection.end())
                atom_selection.insert(res[0]->atm);
              else
                atom_selection.erase(atom_sel_it);
            };
          recalc_gizmo_barycenter();
          return true;
        }
    }
  return false;
}

bool ws_atoms_list_t::support_translation(){return true;}

bool ws_atoms_list_t::support_rotation(){return false;}

bool ws_atoms_list_t::support_scaling(){return  false;}

bool ws_atoms_list_t::support_content_editing(){return true;}

bool ws_atoms_list_t::support_selection(){return true;}

bool ws_atoms_list_t::support_rendering_bounding_box(){return geom->DIM > 0;}

std::string ws_atoms_list_t::compose_item_name(){
  return fmt::format("Type = [atom list], DIM = [{}d]", geom->DIM);
}

void ws_atoms_list_t::update(float delta_time){
  ws_item_t::update(delta_time);
}

float ws_atoms_list_t::get_bb_prescaller(){
  if (geom->DIM == 3) return 1.5f;
  return 1.1f;
}

uint32_t ws_atoms_list_t::get_amount_of_selected_content(){
  return this->atom_selection.size();
}

void ws_atoms_list_t::on_begin_content_gizmo_translate(){
  c_app::log(fmt::format("Start of translating node [{}] content", name));
  tws_tr->auto_bonding = false;
  tws_tr->auto_build   = false;
}

void ws_atoms_list_t::apply_intermediate_translate_content(const vector3<float> &pos){
  bool someone_from_atoms_were_translated = false;
  for (const uint16_t &at_idx : atom_selection){
      vector3<float> acc_pos = geom->coord(at_idx) + pos;
      geom->change_pos(at_idx, acc_pos);
      someone_from_atoms_were_translated = true;
    }
  if (someone_from_atoms_were_translated) recalc_gizmo_barycenter();
}

void ws_atoms_list_t::on_end_content_gizmo_translate(){
  c_app::log(fmt::format("End of translating node [{}] content", name));
  tws_tr->auto_bonding = true;
  tws_tr->auto_build   = true;
}

void ws_atoms_list_t::recalc_gizmo_barycenter(){
  //barycenter in local frame
  gizmo_barycenter = vector3<float>::Zero();

  if (atom_selection.size() > 0 || geom->nat() == 0){
      for (const auto& atm_idx : atom_selection)
        gizmo_barycenter += geom->pos(atm_idx);
      gizmo_barycenter /= atom_selection.size();
    }
  else gizmo_barycenter = aabb.min;

}

const vector3<float> ws_atoms_list_t::get_gizmo_content_barycenter(){
  return gizmo_barycenter;
}

void ws_atoms_list_t::shift(const vector3<float> vShift){
  tws_tr->auto_bonding = false;
  tws_tr->auto_build   = false;

  for (int i = 0; i < geom->nat(); i++)
    geom->coord(i) = vShift + geom->pos(i) ;

  ext_obs->aabb.min = vShift + ext_obs->aabb.min;
  ext_obs->aabb.max = vShift + ext_obs->aabb.max;
  tws_tr->apply_shift(vShift);

  tws_tr->auto_bonding = true;
  tws_tr->auto_build   = true;
  geometry_changed();
}

void ws_atoms_list_t::load_from_file(qc_file_format file_format,
                                     std::string file_name,
                                     bool auto_center){

  c_app::log(fmt::format("Loading geometry from file {} to ws_atom_list in workspace {}",
                         file_name, parent_ws->ws_name));

  std::ifstream qc_data(file_name);
  if (!(qc_data.good())) {
      c_app::log(fmt::format("Error in loading from file {}", file_name));
      return;
    }

  //clean geom and tws-tree
  tws_tr->auto_bonding = false;
  tws_tr->auto_build   = false;
  tws_tr->clear_ntable();
  tws_tr->clear_tree();
  ext_obs->first_data = true;

  name = extract_base_name(file_name);

  switch (file_format) {
    case qc_file_format::format_standart_xyz:
      geom->DIM = 0;
      read_xyz(qc_data, *(geom));
      break;

    case qc_file_format::format_vasp_poscar:
      geom->DIM = 3;
      geom->cell.DIM = 3;
      read_vasp_poscar(qc_data, *(geom));
      break;

    default: c_app::log("File format not implemented");

    }

  qc_data.close();

  if(auto_center){
      vector3<float> vCenter(0.0, 0.0, 0.0);
      for (int i = 0; i < geom->nat(); i++)
        vCenter += geom->pos(i);
      vCenter *= (1.0f / geom->nat());
      for (int i = 0; i < geom->nat(); i++)
        geom->coord(i) = -vCenter + geom->pos(i) ;

      ext_obs->aabb.min = -vCenter + ext_obs->aabb.min;
      ext_obs->aabb.max = -vCenter + ext_obs->aabb.max;
    }

  tws_tr->manual_build();
  tws_tr->find_all_neighbours();
  tws_tr->auto_bonding = true;
  tws_tr->auto_build   = true;

  geometry_changed();

  if (parent_ws) parent_ws->workspace_changed();

}

void ws_atoms_list_t::rebuild_ngbt(){

  need_to_rebuild_nbt = false;
}
