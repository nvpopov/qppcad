#include <qppcad/ws_item/geom_view/geom_view_render_bs.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void geom_view_render_bs::render(geom_view_t &al) {

  app_state_t* astate = app_state_t::get_inst();
  index all_null = index::D(al.m_geom->get_DIM()).all(0);

  float spec = al.m_draw_specular.get_value() ? 1.0f : 0.0f;
  auto draw_atoms = al.m_draw_atoms.get_value();
  auto draw_img_atoms = al.m_draw_img_atoms.get_value();
  auto draw_bonds = al.m_draw_bonds.get_value();
  auto draw_img_bonds = al.m_draw_img_bonds.get_value();
  auto shading_specular_power = al.m_shading_specular_power.get_value();
  astate->dp->begin_atom_render(shading_specular_power, spec);

  // draw {0,..} atoms
  for (uint32_t i = 0; i < al.m_geom->nat(); i++)
    if (draw_atoms
        && al.m_atom_type_to_hide.find(al.m_geom->type_table(i)) == al.m_atom_type_to_hide.end())
      render_atom(al, i, all_null);

  // draw imaginary atoms
  if (al.m_geom->get_DIM() > 0 && draw_atoms && draw_img_atoms)
    for (const auto &at_img : al.m_tws_tr->m_img_atoms)
      if (al.m_atom_type_to_hide.find(al.m_geom->type_table(at_img.m_atm)) ==
          al.m_atom_type_to_hide.end())
        render_atom(al, at_img.m_atm, at_img.m_idx);

  astate->dp->end_atom_render();
  // atom render end

  // bond render
  astate->dp->begin_render_2c_bond(shading_specular_power, spec);

  if (draw_bonds)
    for (uint32_t i = 0; i < al.m_geom->nat(); i++)
      for (uint32_t j = 0; j < al.m_tws_tr->n(i); j++) {
          uint32_t id1 = i;
          uint32_t id2 = al.m_tws_tr->table_atm(i, j);
          index idx2 = al.m_tws_tr->table_idx(i, j);

          //bond is pure real
          if (idx2 == all_null && id1 < id2)
            render_bond(al, id1, all_null, id2, idx2);

          //bond is mixed
          if (idx2 != all_null && draw_img_bonds)
            render_bond(al, id2, idx2, id1, all_null);
        }

  //pure imaginary bond
  if (al.m_geom->get_DIM() > 0 && draw_img_bonds && draw_bonds)
    for (const auto &img_atom : al.m_tws_tr->m_img_atoms)
      for (const auto &img_bond : img_atom.m_img_bonds) {
          uint32_t id1 = img_atom.m_atm;
          uint32_t id2 = img_bond.m_atm;

          index idx1 = img_atom.m_idx;
          index idx2 = img_bond.m_idx;

          //idx1 is always img
          if (idx2 != all_null && id1 < id2)
            render_bond(al, id1, idx1, id2, idx2);
        }

  astate->dp->end_render_2c_bond();

}

void geom_view_render_bs::render_suprematic(geom_view_t &al) {

  app_state_t* astate = app_state_t::get_inst();
  index all_null = index::D(al.m_geom->get_DIM()).all(0);

  float spec = al.m_draw_specular.get_value() ? 1.0f : 0.0f;
  auto draw_atoms = al.m_draw_atoms.get_value();
  auto draw_img_atoms = al.m_draw_img_atoms.get_value();
  auto draw_bonds = al.m_draw_bonds.get_value();
  auto draw_img_bonds = al.m_draw_img_bonds.get_value();

  astate->dp->depth_func(draw_pipeline_depth_func::depth_lequal);
  for (auto is_backpass : {true, false}) {

      if (is_backpass) {
          astate->dp->cull_func(draw_pipeline_cull_func::cull_front);
        } else {
          astate->dp->cull_func(draw_pipeline_cull_func::cull_back);
        }

      astate->dp->begin_atom_render_suprematic();

      // draw {0,..} atoms
      for (uint32_t i = 0; i < al.m_geom->nat(); i++)
        if (draw_atoms &&
            al.m_atom_type_to_hide.find(al.m_geom->type_table(i)) ==
            al.m_atom_type_to_hide.end())
          render_atom_suprematic(al, i, all_null, is_backpass);

      // draw imaginary atoms
      if (al.m_geom->get_DIM() > 0 && draw_atoms && draw_img_atoms)
        for (const auto &at_img : al.m_tws_tr->m_img_atoms)
          if (al.m_atom_type_to_hide.find(al.m_geom->type_table(at_img.m_atm)) ==
              al.m_atom_type_to_hide.end())
            render_atom_suprematic(al, at_img.m_atm, at_img.m_idx, is_backpass);

      astate->dp->end_atom_render_suprematic();
      // atom render end

    }

  astate->dp->depth_func(draw_pipeline_depth_func::depth_less);
  for (auto is_backpass : {true, false}) {

      if (is_backpass) {
          astate->dp->cull_func(draw_pipeline_cull_func::cull_front);
        } else {
          astate->dp->cull_func(draw_pipeline_cull_func::cull_back);
        }

      // bond render
      astate->dp->begin_render_2c_bond_suprematic();

      if (draw_bonds)
        for (uint32_t i = 0; i < al.m_geom->nat(); i++)
          for (uint32_t j = 0; j < al.m_tws_tr->n(i); j++) {
              uint32_t id1 = i;
              uint32_t id2 = al.m_tws_tr->table_atm(i, j);
              index idx2 = al.m_tws_tr->table_idx(i, j);

              //bond is pure real
              if (idx2 == all_null && id1 < id2)
                render_bond_suprematic(al, id1, all_null, id2, idx2, is_backpass);

              //bond is mixed
              if (idx2 != all_null && draw_img_bonds)
                render_bond_suprematic(al, id2, idx2, id1, all_null, is_backpass);
            }

      //pure imaginary bond
      if (al.m_geom->get_DIM() > 0 && draw_img_bonds && draw_bonds)
        for (const auto &img_atom : al.m_tws_tr->m_img_atoms)
          for (const auto &img_bond : img_atom.m_img_bonds) {
              uint32_t id1 = img_atom.m_atm;
              uint32_t id2 = img_bond.m_atm;

              index idx1 = img_atom.m_idx;
              index idx2 = img_bond.m_idx;

              //idx1 is always img
              if (idx2 != all_null && id1 < id2)
                render_bond_suprematic(al, id1, idx1, id2, idx2, is_backpass);
            }

      astate->dp->end_render_2c_bond_suprematic();

    }

  astate->dp->depth_func(draw_pipeline_depth_func::depth_lequal);

}

void geom_view_render_bs::render_atom (geom_view_t &al,
                                       const uint32_t at_num,
                                       const index &at_index) {

  if (al.m_sel_vis.get_value() && al.m_geom->xfield<bool>(xgeom_sel_vis_hide, at_num)) return;

  app_state_t* astate = app_state_t::get_inst();

  auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(at_num));
  float dr_rad = 0.4f;
  float pre_rad = 0.4f;
  vector3<float> color(0.0, 0.0, 1.0);

  if (ap_idx) {
      pre_rad = ptable::get_inst()->arecs[*ap_idx - 1].m_radius;
      color = ptable::get_inst()->arecs[*ap_idx - 1].m_color_jmol;
    }

  if (al.m_geom->xfield<bool>(xgeom_override, at_num)) {
      pre_rad = al.m_geom->xfield<float>(xgeom_atom_r, at_num);
    }

  if (!al.m_type_color_override.empty()) {
      auto it = al.m_type_color_override.find(al.m_geom->type_table(at_num));
      if (it != al.m_type_color_override.end()) color = it->second;
    }

  dr_rad = pre_rad * al.m_atom_scale_factor.get_value();

  if (al.m_color_mode.get_value() == geom_view_color_e::color_from_xgeom ||
      al.m_geom->xfield<bool>(xgeom_override, at_num)) {
      color[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num);
      color[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num);
      color[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num);
    }

  if (al.m_parent_ws->get_edit_type() == ws_edit_e::edit_content) {
      if (al.m_geom->iselected(at_num, at_index) && al.m_selected)
        color = vector3<float>(0.43f, 0.55f, 0.12f);
    }

  astate->dp->render_atom(color, al.m_geom->pos(at_num, at_index) + al.m_pos.get_value(), dr_rad);

}

void geom_view_render_bs::render_atom_suprematic(geom_view_t &al,
                                                 const uint32_t at_num,
                                                 const index &at_index,
                                                 bool backpass) {

  if (al.m_sel_vis.get_value() && al.m_geom->xfield<bool>(xgeom_sel_vis_hide, at_num)) return;

  app_state_t* astate = app_state_t::get_inst();

  auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(at_num));
  float dr_rad = 0.4f;
  float pre_rad = 0.4f;
  vector3<float> color(0.0, 0.0, 0.0);

  if (ap_idx) {
      pre_rad = ptable::get_inst()->arecs[*ap_idx - 1].m_radius;
      color = ptable::get_inst()->arecs[*ap_idx - 1].m_color_jmol;
    }

  if (al.m_geom->xfield<bool>(xgeom_override, at_num)) {
      pre_rad = al.m_geom->xfield<float>(xgeom_atom_r, at_num);
    }

  if (!al.m_type_color_override.empty()) {
      auto it = al.m_type_color_override.find(al.m_geom->type_table(at_num));
      if (it != al.m_type_color_override.end())
        color = it->second;
    }

  dr_rad = pre_rad * al.m_atom_scale_factor.get_value();

  if (al.m_color_mode.get_value() == geom_view_color_e::color_from_xgeom
      || al.m_geom->xfield<bool>(xgeom_override, at_num)) {
      color[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num);
      color[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num);
      color[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num);
    }

  if (al.m_parent_ws->get_edit_type() == ws_edit_e::edit_content) {
      if (al.m_geom->iselected(at_num, at_index) && al.m_selected)
        color = vector3<float>(0.43f, 0.55f, 0.12f);
    }

  astate->dp->render_atom_suprematic(backpass ? vector3<float>(0,0,0) : color,
                                     al.m_geom->pos(at_num, at_index) + al.m_pos.get_value(),
                                     backpass ? dr_rad * 1.1f : dr_rad);

}

void geom_view_render_bs::render_bond (geom_view_t &al,
                                       const uint32_t at_num1,
                                       const index &at_index1,
                                       const uint32_t at_num2,
                                       const index &at_index2) {

  app_state_t* astate = app_state_t::get_inst();

  if (al.m_sel_vis.get_value() && al.m_sel_vis_affect_bonds.get_value() &&
      (al.m_geom->xfield<bool>(xgeom_sel_vis_hide, at_num1) ||
       al.m_geom->xfield<bool>(xgeom_sel_vis_hide, at_num2) )) return;

  if (al.m_atom_type_to_hide_bond.find(al.m_geom->type_table(at_num1)) !=
      al.m_atom_type_to_hide_bond.end() ||
      al.m_atom_type_to_hide_bond.find(al.m_geom->type_table(at_num2)) !=
      al.m_atom_type_to_hide_bond.end()) return;

  auto ap_idx1 = ptable::number_by_symbol(al.m_geom->atom(at_num1));
  vector3<float> bcolor1(0.0, 0.0, 1.0);
  if (ap_idx1) {
      bcolor1 = ptable::get_inst()->arecs[*ap_idx1 - 1].m_color_jmol;
    }

  auto ap_idx2 = ptable::number_by_symbol(al.m_geom->atom(at_num2));
  vector3<float> bcolor2(0.0, 0.0, 1.0);
  if (ap_idx1) {
      bcolor2 = ptable::get_inst()->arecs[*ap_idx2 - 1].m_color_jmol;
    }

  if (!al.m_type_color_override.empty()) {
      auto it1 = al.m_type_color_override.find(al.m_geom->type_table(at_num1));
      if (it1 != al.m_type_color_override.end())
        bcolor1 = it1->second;
      auto it2 = al.m_type_color_override.find(al.m_geom->type_table(at_num2));
      if (it2 != al.m_type_color_override.end())
        bcolor2 = it2->second;
    }

  if (al.m_color_mode.get_value() == geom_view_color_e::color_from_xgeom) {
      bcolor1[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num1);
      bcolor1[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num1);
      bcolor1[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num1);

      bcolor2[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num2);
      bcolor2[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num2);
      bcolor2[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num2);
    }

  if (al.m_geom->xfield<bool>(xgeom_override, at_num1)) {
      bcolor1[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num1);
      bcolor1[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num1);
      bcolor1[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num1);
    }

  if (al.m_geom->xfield<bool>(xgeom_override, at_num2)) {
      bcolor2[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num2);
      bcolor2[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num2);
      bcolor2[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num2);
    }

  astate->dp->render_2c_bond(bcolor1, bcolor2,
                             al.m_geom->pos(at_num1, at_index1) + al.m_pos.get_value(),
                             al.m_geom->pos(at_num2, at_index2) + al.m_pos.get_value(),
                             al.m_bond_scale_factor.get_value());
}

void geom_view_render_bs::render_bond_suprematic(geom_view_t &al,
                                                 const uint32_t at_num1,
                                                 const index &at_index1,
                                                 const uint32_t at_num2,
                                                 const index &at_index2,
                                                 bool backpass) {

  app_state_t* astate = app_state_t::get_inst();

  if (al.m_sel_vis.get_value() && al.m_sel_vis_affect_bonds.get_value() &&
      (al.m_geom->xfield<bool>(xgeom_sel_vis_hide, at_num1) ||
       al.m_geom->xfield<bool>(xgeom_sel_vis_hide, at_num2) )) return;

  if (al.m_atom_type_to_hide_bond.find(al.m_geom->type_table(at_num1)) !=
      al.m_atom_type_to_hide_bond.end() ||
      al.m_atom_type_to_hide_bond.find(al.m_geom->type_table(at_num2)) !=
      al.m_atom_type_to_hide_bond.end()) return;

  auto ap_idx1 = ptable::number_by_symbol(al.m_geom->atom(at_num1));
  auto ap_idx2 = ptable::number_by_symbol(al.m_geom->atom(at_num2));
  vector3<float> bcolor1(0.0, 0.0, 0.0);
  vector3<float> bcolor2(0.0, 0.0, 0.0);

  if (ap_idx1) {
      bcolor1 = ptable::get_inst()->arecs[*ap_idx1 - 1].m_color_jmol;
    }

  if (ap_idx1) {
      bcolor2 = ptable::get_inst()->arecs[*ap_idx2 - 1].m_color_jmol;
    }

  if (!al.m_type_color_override.empty()) {
      auto it1 = al.m_type_color_override.find(al.m_geom->type_table(at_num1));
      if (it1 != al.m_type_color_override.end())
        bcolor1 = it1->second;
      auto it2 = al.m_type_color_override.find(al.m_geom->type_table(at_num2));
      if (it2 != al.m_type_color_override.end())
        bcolor2 = it2->second;
    }

  if (al.m_color_mode.get_value() == geom_view_color_e::color_from_xgeom) {
      bcolor1[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num1);
      bcolor1[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num1);
      bcolor1[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num1);

      bcolor2[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num2);
      bcolor2[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num2);
      bcolor2[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num2);
    }

  if (al.m_geom->xfield<bool>(xgeom_override, at_num1)) {
      bcolor1[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num1);
      bcolor1[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num1);
      bcolor1[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num1);
    }

  if (al.m_geom->xfield<bool>(xgeom_override, at_num2)) {
      bcolor2[0] = al.m_geom->xfield<float>(xgeom_ccr, at_num2);
      bcolor2[1] = al.m_geom->xfield<float>(xgeom_ccg, at_num2);
      bcolor2[2] = al.m_geom->xfield<float>(xgeom_ccb, at_num2);
    }

  astate->dp->render_2c_bond_suprematic(
        backpass ? vector3<float>(0,0,0) : bcolor1,
        backpass ? vector3<float>(0,0,0) : bcolor2,
        al.m_geom->pos(at_num1, at_index1) + al.m_pos.get_value(),
        al.m_geom->pos(at_num2, at_index2) + al.m_pos.get_value(),
        backpass ? al.m_bond_scale_factor.get_value() * 1.2 : al.m_bond_scale_factor.get_value());

}

