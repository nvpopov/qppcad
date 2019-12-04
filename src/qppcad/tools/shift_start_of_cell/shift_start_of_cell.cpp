#include <qppcad/tools/shift_start_of_cell/shift_start_of_cell.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void shift_start_of_cell_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  astate->log("shift_center_of_cell_tool_t::exec()");

  if (!item) {
      QMessageBox::warning(nullptr, QObject::tr("Shift start of cell to atoms"),
                           QObject::tr("Invalid item."));
      return;
    }

  auto al = item->cast_as<geom_view_t>();
  if (!al) {
      QMessageBox::warning(nullptr, QObject::tr("Shift start of cell to atoms"),
                           QObject::tr("Invalid item."));
      return;
    }

  if (al->m_geom->DIM == 0) {
      QMessageBox::warning(nullptr, QObject::tr("Shift start of cell to atoms"),
                           QObject::tr("The structure must be 3d."));
      return;
    }

  if (!al->m_parent_ws) {
      QMessageBox::warning(nullptr, QObject::tr("Shift start of cell to atoms"),
                           QObject::tr("Invalid workspace"));
      return;
    }

  if (al->m_atom_idx_sel.empty()) {
      QMessageBox::warning(nullptr, QObject::tr("Shift start of cell to atoms"),
                           QObject::tr("The list of selected atoms is empty."));
      return;
    }

  // compute new center
  vector3<float> new_center{0};

  for (auto &rec : al->m_atom_idx_sel)
    new_center += al->m_geom->pos(rec.m_atm, rec.m_idx);

  new_center /= al->m_atom_idx_sel.size();

//  for (size_t i = 0; i < al->m_geom->nat(); i++)
//    al->m_geom->coord(i) -= new_center;

  geom_view_tools_t::translate_atoms_in_cell(al, -new_center, false);

  astate->make_viewport_dirty();

}
