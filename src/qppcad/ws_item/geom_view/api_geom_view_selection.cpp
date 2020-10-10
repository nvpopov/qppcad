#include <qppcad/ws_item/geom_view/api_geom_view_selection.hpp>
#include <qppcad/core/app_state.hpp>

namespace qpp {

namespace cad {

void api_gv_sel_visible_atoms(geom_view_t *gv, bool hs_rec) {
  if (!gv)
    return;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (size_t i = 0; i < gv->m_geom->nat(); i++)
    if (!gv->m_geom->xfield<bool>(xg_sv_h, i))
      gv->sel_atom(i);
  if (hs_rec)
    gv->end_recording();
}

void api_gv_sel_atom_ngbs(geom_view_t *gv, const int at_id, bool hs_rec) {
  if (!gv)
    return;
  for (int i = 0; i < gv->m_tws_tr->n(at_id); i++)
    if (gv->m_tws_tr->table_idx(at_id, i).is_zero())
      gv->sel_atom(gv->m_tws_tr->table_atm(at_id, i), hs_rec);
}

void api_gv_sel_selected_atoms_ngbs(geom_view_t *gv, bool hs_rec) {
  if (!gv)
    return;
  std::set<int> stored_sel;
  for (auto i = 0; i < gv->m_geom->num_selected(); i++) {
    auto rec = gv->m_geom->nth_selected(i);
    if (rec && (*rec).m_idx.is_zero())
      stored_sel.insert((*rec).m_atm);
  }
  if (stored_sel.empty())
    return;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (auto &rec : stored_sel)
    api_gv_sel_atom_ngbs(gv, rec, false);
  if (hs_rec)
    gv->end_recording();
}

void api_gv_sel_atoms_by_box(geom_view_t *gv, vector3<float> s_pos, vector3<float> e_pos, bool hs_rec) {
  if (!gv)
    return;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (size_t i = 0; i < gv->m_geom->nat(); i++) {
    aabb_3d_t<float> aabb{s_pos, e_pos};
    auto is_inside_aabb = aabb.test_point(gv->m_geom->pos(i));
    if (is_inside_aabb)
      gv->sel_atom(i, false);
  }
  if (hs_rec)
    gv->end_recording();
}

void api_gv_sel_by_box_sq(geom_view_t *gv, const float box_scale, bool hs_rec) {
  if (!gv)
    return;
  if (gv->m_geom->num_aselected() != 2)
    return;
  auto idx1 = gv->m_geom->nth_aselected(0);
  auto idx2 = gv->m_geom->nth_aselected(1);
  auto pos1 = gv->m_geom->pos(idx1->m_atm, idx1->m_idx);
  auto pos2 = gv->m_geom->pos(idx2->m_atm, idx2->m_idx);
  vector3<float> center = (pos1 + pos2) * 0.5f;
  vector3<float> len{0};
  for (size_t i = 0; i < 3; i++)
    len[i] = std::max(std::max(pos1[i], pos2[i]) - std::min(pos1[i], pos2[i]), 0.1f);
  auto pos1_r = center - len * 0.5f * box_scale;
  auto pos2_r = center + len * 0.5f * box_scale;
  api_gv_sel_atoms_by_box(gv, pos1_r, pos2_r, hs_rec);
}

void api_gv_sel_atoms_by_type(geom_view_t *gv, const int atom_type, bool hs_rec) {
  app_state_t* astate = app_state_t::get_inst();
  if (!gv)
    return;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (auto i = 0; i < gv->m_geom->nat(); i++)
    if (gv->m_geom->type_table(i) == atom_type)
      gv->sel_atom(i);
  if (hs_rec)
    gv->end_recording();
  if (gv->m_parent_ws)
    gv->m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
}

void api_gv_unsel_atoms_by_type(geom_view_t *gv, const int atom_type, bool hs_rec) {
  app_state_t* astate = app_state_t::get_inst();
  if (!gv)
    return;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (auto i = 0; i < gv->m_geom->nat(); i++)
    if (gv->m_geom->type_table(i) == atom_type)
      gv->unsel_atom(i);
  if (hs_rec)
    gv->end_recording();
  gv->recalc_gizmo_barycenter();
  if (gv->m_parent_ws)
    gv->m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
}

void api_gv_inv_selected_atoms(geom_view_t *gv, bool hs_rec) {
  app_state_t* astate = app_state_t::get_inst();
  if (!gv)
    return;
  std::set<int> sel_atm;
  index zero = index::D(gv->m_geom->get_DIM()).all(0);
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (int i = 0; i < gv->m_geom->nat(); i++)
    gv->m_geom->toggle_selected(i);
  if (hs_rec)
    gv->end_recording();
  gv->recalc_gizmo_barycenter();
  if (gv->m_parent_ws)
    gv->m_parent_ws->m_gizmo->update_gizmo(0.01f);
  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_atoms_list_selection_changed();
}

}

}


