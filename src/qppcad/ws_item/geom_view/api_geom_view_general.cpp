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
      gv->m_geom->set_xfield<bool>(field_id,
                                   (*rec).m_atm,
                                   !gv->m_geom->xfield<bool>(field_id, (*rec).m_atm));
  }
  if (hs_rec)
    gv->end_recording();
}

}

}
