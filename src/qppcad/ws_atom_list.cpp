#include <qppcad/ws_atom_list.hpp>
#include <qppcad/app.hpp>
#include <io/geomio.hpp>
#include <io/vasp_io.hpp>

using namespace qpp;

ws_atom_list_t::ws_atom_list_t(workspace_t* parent):ws_item_t(parent){

  bNeedToRebuildNBT = true;
  iDim = 3;
  //cell = new periodic_cell<float>({20.0, 0.0, 0.0}, {0.0, 20.0, 0.0}, {0.0, 0.0, 20.0});

  geom = new xgeometry<float, periodic_cell<float> >(3,"rg1");
  geom->set_format({"atom", "number", "charge", "x", "y", "z", "show", "sel"},

  {type_string, type_int, type_real, type_real, type_real, type_real,
   type_bool, type_bool});

  geom->DIM = 0;
  geom->cell.DIM = 0;

  ext_obs = new extents_observer<float>(*geom);
  tws_tr = new tws_tree<float>(*geom);
  tws_tr->bAutoBonding = true;

}

void ws_atom_list_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
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

void ws_atom_list_t::update(){
  aabb = ext_obs->aabb;
}

void ws_atom_list_t::render(){
  ws_item_t::render();
  app_state_t* astate = &(c_app::get_state());

  if (app_state_c->dp != nullptr){

      if (astate->bDebugDrawRTree){
          astate->dp->begin_render_aabb();
          tws_tr->apply_visitor(
                [astate](tws_node<float> *inNode, int deepLevel){
            astate->dp->render_aabb(clr_maroon, inNode->bb.min, inNode->bb.max);
          });

          astate->dp->end_render_aabb();
        }

      if (geom->DIM == 3){
          astate->dp->begin_render_line();
          if (bSelected) astate->dp->render_cell_3d(clr_red, geom->cell.v[0],
              geom->cell.v[1], geom->cell.v[2], 2.0f);
          else astate->dp->render_cell_3d(clr_black, geom->cell.v[0], geom->cell.v[1],
              geom->cell.v[2], 1.1f);
          astate->dp->end_render_line();
        }

      // atom render start
      astate->dp->begin_atom_render();

      // draw {0,..} atoms
      for (int i = 0; i < geom->nat(); i++) render_atom(i, index::D(geom->DIM).all(0));

      // draw imaginay atoms
      if (geom->DIM > 0) for (uint16_t i = 0; i < tws_tr->imgAtoms.size(); i++)
            render_atom(tws_tr->imgAtoms[i]->atm, tws_tr->imgAtoms[i]->idx);

      astate->dp->end_atom_render();
      // atom render end

      // bond render
      astate->dp->begin_render_bond();


      for (uint16_t i = 0; i < geom->nat(); i++)
        for (uint16_t j = 0; j < tws_tr->n(i); j++){

            uint16_t id1 = i;
            uint16_t id2 = tws_tr->table_atm(i,j);
            index idx2 = tws_tr->table_idx(i,j);

            render_bond(id1, index::D(geom->DIM).all(0), id2, idx2);

            if(idx2 != index::D(geom->DIM).all(0))
              render_bond(id2, idx2, id1, index::D(geom->DIM).all(0));

          }

      if (geom->DIM > 0)
        for (uint16_t i = 0; i < tws_tr->imgAtoms.size(); i++)
          for (uint16_t j = 0; j < tws_tr->imgAtoms[i]->imBonds.size(); j++){

              uint16_t id1 = tws_tr->imgAtoms[i]->atm;
              uint16_t id2 = tws_tr->imgAtoms[i]->imBonds[j]->atm;

              index idx1 = tws_tr->imgAtoms[i]->idx;
              index idx2 = tws_tr->imgAtoms[i]->imBonds[j]->idx;

              render_bond(id1, idx1, id2, idx2);
            }


      astate->dp->end_render_bond();

    }
}

void ws_atom_list_t::render_atom(const uint16_t atNum, const index &atIndex){

  int ap_idx = ptable::number_by_symbol(geom->atom(atNum));
  float fDrawRad = 0.4f;
  vector3<float> color(0.0, 0.0, 1.0);
  if(ap_idx != -1){
      fDrawRad = ptable::get_inst()->arecs[ap_idx-1].aRadius * app_state_c->fAtomRadiusScaleFactor;
      color = ptable::get_inst()->arecs[ap_idx-1].aColorJmol;
    }

  if((parent_ws->cur_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT) &&
     (geom->xfield<bool>("sel", atNum)) && bSelected)
    color = vector3<float>(0.43f, 0.55f, 0.12f);

  app_state_c->dp->render_atom(color, geom->pos(atNum, atIndex), fDrawRad);
}

void ws_atom_list_t::render_bond(const uint16_t atNum1, const index &atIndex1,
                               const uint16_t atNum2, const index &atIndex2){
  int ap_idx = ptable::number_by_symbol(geom->atom(atNum1));
  vector3<float> bcolor(0.0, 0.0, 1.0);
  if(ap_idx != -1){bcolor = ptable::get_inst()->arecs[ap_idx-1].aColorJmol;}
  app_state_c->dp->render_bond(bcolor, geom->pos(atNum1, atIndex1), geom->pos(atNum2, atIndex2),
                               app_state_c->fBondScaleFactor);
}

void ws_atom_list_t::render_ui(){
  ImGui::Separator();
  ImGui::Button("Rebond");
  ImGui::SameLine();
  ImGui::Button("Delete");
  static bool bVisible;
  ImGui::SameLine();
  ImGui::Checkbox("Show" , &bVisible);
  ImGui::Separator();
  int _dim = iDim;
  ImGui::Text("Geom. dim:"); ImGui::SameLine();
  ImGui::SliderInt("", &_dim, 0, 3);
  iDim = _dim;

  if (ImGui::TreeNode("Atoms")){
      for (int i = 0; i < geom->nat(); i++)
        if (ImGui::TreeNode(fmt::format("{}:{}", geom->atom(i), i).c_str())){
            //            ImGui::SameLine();
            //            ImGui::Button("eeeee");
            //  ImGui::Columns(2);
            //            float _x = geom->pos(i)[0];
            //            float _y = geom->pos(i)[1];
            //            float _z = geom->pos(i)[2];

            float vec3f[3] = { geom->pos(i)[0], geom->pos(i)[1], geom->pos(i)[2]};

            //            ImGui::Text("X");
            //            ImGui::Text("Y");
            //            ImGui::Text("Z");
            //ImGui::Se
            ImGui::Text("Pos.:");
            ImGui::SameLine();
            ImGui::PushID("flt3");
            ImGui::InputFloat3("", vec3f);
            ImGui::PopID();


            geom->coord(i) = vector3<float>(vec3f[0], vec3f[1], vec3f[2]);
            //   ImGui::Columns(1);
            ImGui::TreePop();
          }
      ImGui::TreePop();
    }
}

bool ws_atom_list_t::mouse_click(ray<float> *ray){
  if (ray){
      std::vector<tws_query_data<float>* > res;
      tws_tr->query_ray<query_ray_add_all<float> >(ray, &res);
      //std::cout << "res_size = " << res.size() << std::endl;
      std::sort(res.begin(), res.end(), tws_query_data_sort_by_dist<float>);

      if (res.size() > 0){
          //std::cout << res[0]->atm << std::endl;
          if ((parent_ws->cur_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT) && bSelected &&
              (res[0]->idx == index::D(geom->DIM).all(0)))
            geom->xfield<bool>("sel", res[0]->atm ) = !(geom->xfield<bool>("sel", res[0]->atm ));
          return true;
        }
    }
  return false;
}

bool ws_atom_list_t::support_translation(){
  return true;
}

bool ws_atom_list_t::support_rotation(){
  return false;
}

bool ws_atom_list_t::support_scaling(){
  return  false;
}

bool ws_atom_list_t::support_content_editing(){
  return true;
}

bool ws_atom_list_t::support_selection(){
  return true;
}

bool ws_atom_list_t::support_rendering_bounding_box(){
  return geom->DIM > 0;
}

std::string ws_atom_list_t::compose_item_name(){
  return fmt::format("Type = [atom list], DIM = [{}d]", geom->DIM);
}

float ws_atom_list_t::get_bb_prescaller(){
  if (geom->DIM == 3) return 1.5f;
  return 1.1f;
}

void ws_atom_list_t::shift(const vector3<float> vShift){
  tws_tr->bAutoBonding = false;
  tws_tr->bAutoBuild   = false;

  for (int i = 0; i < geom->nat(); i++)
    geom->coord(i) = vShift + geom->pos(i) ;

  ext_obs->aabb.min = vShift + ext_obs->aabb.min;
  ext_obs->aabb.max = vShift + ext_obs->aabb.max;
  tws_tr->apply_shift(vShift);

  tws_tr->bAutoBonding = true;
  tws_tr->bAutoBuild   = true;
  update();
}

void ws_atom_list_t::load_from_file(qc_file_format eFileFormat,
                                  std::string sFileName,
                                  bool bAutoCenter){

  c_app::log(fmt::format("Loading geometry from file {} to ws_atom_list in workspace {}",
                         sFileName, parent_ws->ws_name));

  std::ifstream fQCData(sFileName);
  if (!(fQCData.good())) {
      c_app::log(fmt::format("Error in loading from file {}", sFileName));
      return;
    }

  //clean geom and tws-tree
  tws_tr->bAutoBonding = false;
  tws_tr->bAutoBuild   = false;
  tws_tr->clear_ntable();
  tws_tr->clear_tree();
  ext_obs->bFirstData = true;

  name = extract_base_name(sFileName);

  switch (eFileFormat) {
    case qc_file_format::format_standart_xyz:
      geom->DIM = 0;
      read_xyz(fQCData, *(geom));
      break;

    case qc_file_format::format_vasp_poscar:
      geom->DIM = 3;
      geom->cell.DIM = 3;
      read_vasp_poscar(fQCData, *(geom));
      break;

    default: c_app::log("File format not implemented");

    }

  fQCData.close();

  if(bAutoCenter){
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
  tws_tr->bAutoBonding = true;
  tws_tr->bAutoBuild   = true;

  update();

}

void ws_atom_list_t::rebuild_ngbt(){

  bNeedToRebuildNBT = false;
}
