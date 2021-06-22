#ifndef QPPCAD_GEOM_VIEW_CHECKERS
#define QPPCAD_GEOM_VIEW_CHECKERS

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

namespace cad {

template<bool ALLOW_0D, bool ALLOW_1D, bool ALLOW_2D, bool ALLOW_3D>
auto gv_accept_dim(ws_item_t *item) -> bool {
  if (!item)
    return false;
  auto as_gv = item->cast_as<geom_view_t>();
  if (!as_gv)
    return false;
  if (ALLOW_0D && as_gv->m_geom->get_DIM() == 0)
    return true;
  if (ALLOW_1D && as_gv->m_geom->get_DIM() == 1)
    return true;
  if (ALLOW_2D && as_gv->m_geom->get_DIM() == 2)
    return true;
  if (ALLOW_3D && as_gv->m_geom->get_DIM() == 3)
    return true;
  return false;
}

}

}
#endif
