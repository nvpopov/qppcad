#ifndef QPPCAD_API_GEOM_VIEW_GENERAL
#define QPPCAD_API_GEOM_VIEW_GENERAL

#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

namespace cad {

  //selective visibility
  void api_gv_sv_set_for_selected(geom_view_t *gv, bool state, bool hs_rec = false);
  void api_gv_sv_set_hide_for_invselected(geom_view_t *gv, bool hs_rec = false);

  //xfields

}

}

#endif
