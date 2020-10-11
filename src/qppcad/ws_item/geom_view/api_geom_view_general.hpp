#ifndef QPPCAD_API_GEOM_VIEW_GENERAL
#define QPPCAD_API_GEOM_VIEW_GENERAL

#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

namespace cad {

  //selective visibility
  void api_gv_sv_set_for_selected(geom_view_t *gv, bool state, bool hs_rec = false);
  void api_gv_sv_set_hide_for_invselected(geom_view_t *gv, bool hs_rec = false);

  //xifelds
  void api_gv_xbool_invert(geom_view_t *gv, size_t field_id, bool hs_rec = false);
  void api_gv_xbool_invert_selected(geom_view_t *gv, size_t field_id, bool hs_rec = false);

  template <typename XFIELD>
  void api_gv_xfill_selected(geom_view_t *gv, size_t field_id, XFIELD value, bool hs_rec = false) {
    if (!gv)
      return;
    if (hs_rec)
      gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
    for (size_t i = 0; i < gv->m_geom->num_selected(); i++)
        gv->m_geom->set_xfield<XFIELD>(field_id, (*gv->m_geom->nth_aselected(i)).m_atm, value);
    if (hs_rec)
      gv->end_recording();
  }

  template <typename XFIELD>
  bool api_gv_any_of_sel_xfield_equal(geom_view_t *gv,int xfield_id, XFIELD xfield_val) {
    if (!gv)
      return false;
    for (size_t i = 0; i < gv->m_geom->num_selected(); i++)
      if (gv->m_geom->xfield<XFIELD>(xfield_id, i) == xfield_val)
        return true;
    return false;
  }

  //general transformationss
  template <typename TRANSFORM_CLASS>
  void api_gv_transform_sel(geom_view_t *gv,const TRANSFORM_CLASS &tm, bool hs_rec = false) {
    if (!gv)
      return;
    auto num_selected = gv->m_geom->num_selected();
    if (num_selected == 0)
      return;
    if (hs_rec)
      gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
    for (auto i = 0; i < gv->m_geom->num_selected(); i++) {
      auto nth_atom = gv->m_geom->nth_selected(i);
      if (nth_atom)
        gv->transform_atom((*nth_atom).m_atm, tm);
    }
    gv->recalc_gizmo_barycenter();
    if (hs_rec)
      gv->end_recording();
  }

}

}

#endif
