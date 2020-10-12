#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/api_geom_view_general.hpp>
#include <qppcad/core/app_state.hpp>

namespace qpp {

namespace cad {

void api_gv_sv_set_for_selected(geom_view_t *gv, bool state, bool hs_rec) {
  if (!gv)
    return;
  app_state_t *astate = app_state_t::get_inst();
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (auto i = 0; i < gv->m_geom->num_selected(); i++) {
    auto rec = gv->m_geom->nth_selected(i);
    if (rec && (*rec).m_idx.is_zero())
      gv->m_geom->set_xfield<bool>(xg_sv_h, (*rec).m_atm, state);
  }
  if (!gv->m_sel_vis.get_value()) {
    gv->m_sel_vis.set_value(true);
    gv->m_sel_vis_affect_bonds.set_value(true);
  }
  if (hs_rec)
    gv->end_recording();
  astate->make_viewport_dirty();
}

void api_gv_sv_set_hide_for_invselected(geom_view_t *gv, bool hs_rec) {
  if (!gv)
    return;
  app_state_t *astate = app_state_t::get_inst();
  std::unordered_set<size_t> cap_idx;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (auto i = 0; i < gv->m_geom->num_selected(); i++) {
    auto rec = gv->m_geom->nth_selected(i);
    if (rec)
      cap_idx.insert((*rec).m_atm);
  }
  for (size_t i = 0; i < gv->m_geom->nat(); i++)
    if (cap_idx.find(i) == cap_idx.end())
      gv->m_geom->set_xfield<bool>(xg_sv_h, i, true);
  if (!gv->m_sel_vis.get_value()) {
    gv->m_sel_vis.set_value(true);
    gv->m_sel_vis_affect_bonds.set_value(true);
  }
  if (hs_rec)
    gv->end_recording();
  astate->make_viewport_dirty();
}

void api_gv_xbool_invert(geom_view_t *gv, size_t field_id, bool hs_rec) {
  if (!gv)
    return;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (auto i = 0; i < gv->m_geom->nat(); i++) {
    gv->m_geom->set_xfield<bool>(field_id, i, !gv->m_geom->xfield<bool>(field_id, i));
  }
  if (hs_rec)
    gv->end_recording();
}

void api_gv_xbool_invert_selected(geom_view_t *gv, size_t field_id, bool hs_rec) {
  if (!gv)
    return;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  for (auto i = 0; i < gv->m_geom->num_selected(); i++) {
    auto rec = gv->m_geom->nth_selected(i);
    if (rec && (*rec).m_idx.is_zero())
      gv->m_geom->set_xfield<bool>(field_id, (*rec).m_atm,
                                   !gv->m_geom->xfield<bool>(field_id, (*rec).m_atm));
  }
  if (hs_rec)
    gv->end_recording();
}

void api_gv_set_atom_override(geom_view_t *gv, int atom_id, bool state, bool hs_rec) {
  if (!gv) {
    return;
  }
  if (atom_id >= gv->m_geom->nat()) {
    return;
  }
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  gv->m_geom->set_xfield<bool>(xg_override, atom_id, state);
  if (gv->m_geom->xfield<float>(xg_atom_r, atom_id) < 0.01f) {
    auto ap_idx = ptable::number_by_symbol(gv->m_geom->atom(atom_id));
    float at_rad = 1.0f;
    if (ap_idx)
      at_rad = ptable::get_inst()->arecs[*ap_idx - 1].m_radius;
    gv->m_geom->set_xfield<float>(xg_atom_r, atom_id, at_rad);
  }
  if (hs_rec)
    gv->end_recording();
}

void api_gv_get_atom_override(geom_view_t *gv, int atom_id, bool &state) {
  if (!gv) {
    state = false;
    return;
  }
  if (atom_id >= gv->m_geom->nat()) {
    state = false;
    return;
  }
  state = gv->m_geom->xfield<bool>(xg_override, atom_id);
}

void api_gv_toggle_atom_override(geom_view_t *gv, int atom_id, bool hs_rec) {
  if (!gv) {
    return;
  }
  if (atom_id >= gv->m_geom->nat()) {
    return;
  }
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  gv->m_geom->set_xfield<bool>(xg_override, atom_id,
                               !gv->m_geom->xfield<bool>(xg_override, atom_id));
  if (gv->m_geom->xfield<float>(xg_atom_r, atom_id) < 0.01f) {
    auto ap_idx = ptable::number_by_symbol(gv->m_geom->atom(atom_id));
    float at_rad = 1.0f;
    if (ap_idx)
      at_rad = ptable::get_inst()->arecs[*ap_idx - 1].m_radius;
    gv->m_geom->set_xfield<float>(xg_atom_r, atom_id, at_rad);
  }
  if (hs_rec)
    gv->end_recording();
}

void api_gv_clone(geom_view_t *src, std::shared_ptr<ws_item_t> &dst) {
  if (!src) {
    dst = nullptr;
    return;
  }
  app_state_t *astate = app_state_t::get_inst();
  auto cloned = astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(geom_view_t::get_type_static());
  if (!cloned) {
    dst = nullptr;
    return;
  }
  cloned->m_name.set_value(fmt::format("{}_clone", src->m_name.get_value()));
  auto c_gv = cloned->cast_as<geom_view_t>();
  if (!c_gv) {
    dst = nullptr;
    return;
  }
  c_gv->m_tws_tr->do_action(act_lock | act_clear_all);
  src->copy_to_xgeom(*(c_gv->m_geom));
  c_gv->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
  c_gv->m_tws_tr->do_action(act_rebuild_ntable);
  if (src->m_parent_ws) {
    src->m_parent_ws->add_item_to_ws(cloned);
  }

  dst = cloned;
}

void api_gv_iupdate_interatomic_dist(geom_view_t *gv, float new_dist, const int at1, const int at2,
                                     const index id1, const index id2,
                                     pair_dist_mode_e mode, bool hs_rec) {
  app_state_t* astate = app_state_t::get_inst();
  if (!gv)
    return;
  if (at1 >= gv->m_geom->nat() || at2 >= gv->m_geom->nat())
    return;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  vector3<float> r_btw = (gv->m_geom->pos(at1, id1) + gv->m_geom->pos(at2, id2))*0.5f;
  vector3<float> dir_f = (gv->m_geom->pos(at1, id1) - r_btw).normalized();
  vector3<float> dir_s = (gv->m_geom->pos(at2, id2) - r_btw).normalized();
  switch (mode) {
  case pair_dist_mode_e::transform_both : {
    gv->m_geom->change_pos(at1, r_btw + dir_f * new_dist * 0.5f);
    gv->m_geom->change_pos(at2, r_btw + dir_s * new_dist * 0.5f);
    break;
  }
  case pair_dist_mode_e::fix_first : {
    gv->m_geom->change_pos(at1, r_btw + dir_f * new_dist * 0.5f );
    break;
  }
  case pair_dist_mode_e::fix_second : {
    gv->m_geom->change_pos(at2, r_btw + dir_s * new_dist * 0.5f);
    break;
  }
  }
  if (hs_rec)
    gv->end_recording();
  astate->make_viewport_dirty();
}

void api_gv_update_interatomic_dist(geom_view_t *gv, float new_dist, const int at1, const int at2,
                                    pair_dist_mode_e mode, bool hs_rec) {
  if (!gv)
    return;
  if (hs_rec)
    gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  api_gv_iupdate_interatomic_dist(gv, new_dist, at1, at2,
                                  index::D(gv->m_geom->get_DIM()).all(0),
                                  index::D(gv->m_geom->get_DIM()).all(0), mode, false);
  if (hs_rec)
    gv->end_recording();
}

}

}
