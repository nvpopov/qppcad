#include <qppcad/tools/center_cell_on_atom/center_cell_on_atom.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void center_cell_on_atom_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  astate->log("center_cell_on_atom_tool_t::exec()");

  if (!item) {
      QMessageBox::warning(nullptr, QObject::tr("Center cell on atom"),
                           QObject::tr("Invalid item."));
      return;
    }

  auto al = item->cast_as<geom_view_t>();
  if (!al) {
      QMessageBox::warning(nullptr, QObject::tr("Center cell on atom"),
                           QObject::tr("Invalid item."));
      return;
    }

  if (al->m_geom->get_DIM() == 0) {
      QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                           QObject::tr("The structure must be 3d."));
      return;
    }

  if (!al->m_parent_ws) {
      QMessageBox::warning(nullptr, QObject::tr("Center cell on atom"),
                           QObject::tr("Invalid workspace"));
      return;
    }

  if (al->m_geom->no_aselected()) {
    QMessageBox::warning(nullptr, QObject::tr("Center cell on atom"),
                         QObject::tr("The list of selected atoms is empty."));
    return;
  }

  // compute new center
  vector3<float> new_center{0};

//  for (auto &rec : al->m_atom_idx_sel)
//    new_center += al->m_geom->pos(rec.m_atm, rec.m_idx);

  for (auto i = 0; i < al->m_geom->num_aselected(); i++) {
    auto rec = al->m_geom->nth_aselected(i);
    if (!rec) continue;
    new_center += al->m_geom->pos((*rec).m_atm, (*rec).m_idx);
  }

  new_center /= al->m_geom->num_aselected();

  geom_view_tools_t::center_cell_on(al, new_center, false);

  astate->make_viewport_dirty();

}
