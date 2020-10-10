#ifndef QPPCAD_API_GEOM_VIEW_SELECTION
#define QPPCAD_API_GEOM_VIEW_SELECTION

#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

namespace cad {

void api_gv_sel_visible_atoms(geom_view_t *gv, bool hs_rec = false);
void api_gv_sel_atom_ngbs(geom_view_t *gv, const int at_id, bool hs_rec = false);
void api_gv_sel_selected_atoms_ngbs(geom_view_t *gv, bool hs_rec = false);
void api_gv_sel_atoms_by_box(geom_view_t *gv, vector3<float> s_pos, vector3<float> e_pos, bool hs_rec=false);
void api_gv_sel_by_box_sq(geom_view_t *gv, const float box_scale, bool hs_rec = false);
void api_gv_sel_atoms_by_type(geom_view_t *gv, const int atom_type, bool hs_rec = false);
void api_gv_unsel_atoms_by_type(geom_view_t *gv, const int atom_type, bool hs_rec = false);
void api_gv_inv_selected_atoms(geom_view_t *gv, bool hs_rec = false);

}

}

#endif
