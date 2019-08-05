#include <qppcad/tools/clamp_atoms_to_cell/clamp_atoms_to_cell.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>

using namespace qpp;
using namespace qpp::cad;

void clamp_atoms_to_cell_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();
  astate->log("clamp_atoms_to_cell_tool_t::exec()");

  auto [cur_ws, cur_it, al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>(_error_ctx);

  if (!ok) return;

  if (al->m_geom->DIM != 3) {
      QMessageBox::warning(nullptr,
                           QObject::tr("Supercell generation"),
                           QObject::tr("Structure`s dimension is not equal to 3")
                           );
      return;
    }

  geom_view_tools_t::clamp_atoms_to_cell(al, ignore_selection);

  astate->make_viewport_dirty();

}
