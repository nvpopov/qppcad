#ifndef QPPCAD_API_GEOM_VIEW_GENERAL
#define QPPCAD_API_GEOM_VIEW_GENERAL

#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

namespace cad {

  //cloning stuff
  void api_gv_clone(geom_view_t *src, std::shared_ptr<ws_item_t> &dst);

  //ui helpers - atom override
  void api_gv_set_atom_override(geom_view_t *gv, int atom_id, bool state, bool hs_rec = false);
  void api_gv_get_atom_override(geom_view_t *gv, int atom_id, bool &state);
  void api_gv_toggle_atom_override(geom_view_t *gv, int atom_id, bool hs_rec = false);

  //ui helpers - custom label

  //selective visibility
  void api_gv_sv_set_for_selected(geom_view_t *gv, bool state, bool hs_rec = false);
  void api_gv_sv_set_hide_for_invselected(geom_view_t *gv, bool hs_rec = false);

  //xifelds
  void api_gv_xbool_invert(geom_view_t *gv, size_t field_id, bool hs_rec = false);
  void api_gv_xbool_invert_selected(geom_view_t *gv, size_t field_id, bool hs_rec = false);

  template<typename XFIELD_TYPE>
  void api_gv_xfill(geom_view_t *gv, size_t field_id, size_t atom_id, XFIELD_TYPE value, bool
                    hs_rec = false) {
    if (!gv)
      return;
    if (hs_rec)
      gv->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
    gv->m_geom->set_xfield<XFIELD_TYPE>(field_id, atom_id, value);
    if (hs_rec)
      gv->end_recording();
  }

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
    for (size_t i = 0; i < gv->m_geom->num_selected(); i++) {
      auto sel_rec = gv->m_geom->nth_selected(i);
      if (!sel_rec)
        return false;
      if (gv->m_geom->xfield<XFIELD>(xfield_id, (*sel_rec).m_atm) == xfield_val)
        return true;
    }
    return false;
  }

  //general transformations
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

  void api_gv_iupdate_interatomic_dist(geom_view_t *gv, float new_dist,
                                       const int at1, const int at2,
                                       const index id1, const index id2,
                                       pair_dist_mode_e mode, bool hs_rec = false);

  void api_gv_update_interatomic_dist(geom_view_t *gv, float new_dist,
                                      const int at1, const int at2,
                                      pair_dist_mode_e mode, bool hs_rec = false);

}

}

#endif
