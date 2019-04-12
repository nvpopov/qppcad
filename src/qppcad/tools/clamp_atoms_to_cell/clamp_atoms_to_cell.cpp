#include <qppcad/tools/clamp_atoms_to_cell/clamp_atoms_to_cell.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void clamp_atoms_to_cell_tool_t::exec(ws_item_t *item) {

  app_state_t *astate = app_state_t::get_inst();
  astate->log("clamp_atoms_to_cell_tool_t::exec()");

  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>();

  if (!cur_ws || !cur_it || !al) return;

  if (al->m_geom->DIM != 3) {
      QMessageBox::warning(nullptr,
                           QObject::tr("Supercell generation"),
                           QObject::tr("m_geom.DIM !=3")
                           );
      return;
    }

  for (int i = 0; i < al->m_geom->nat(); i++)
    if (al->m_atom_idx_sel.find(
          atom_index_set_key(i, index::D(al->m_geom->DIM).all(0)))
        != al->m_atom_idx_sel.end() || ignore_selection) {
        vector3<float> pos = al->m_geom->pos(i);
        al->m_geom->change_pos(i, al->m_geom->cell.reduce(pos));
      }

  astate->make_viewport_dirty();

}
