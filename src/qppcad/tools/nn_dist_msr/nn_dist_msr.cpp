#include <qppcad/tools/nn_dist_msr/nn_dist_msr.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_measurement_subsys.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void nn_dist_msr_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!item) {
      QMessageBox::warning(nullptr, QObject::tr("nn_dist_msr"),
                           QObject::tr("ws_item == nullptr"));
      return;
    }

  auto al = item->cast_as<geom_view_t>();
  if (!al) {
      QMessageBox::warning(nullptr, QObject::tr("nn_dist_msr"),
                           QObject::tr("ws_item.cast<geom_view_t>() == nullptr"));
      return;
    }

  for (auto i = 0; i < al->m_geom->num_aselected(); i++) {
    auto rec = al->m_geom->nth_selected(i);
    if (!rec) continue;

    for (size_t i = 0; i < al->m_tws_tr->n((*rec).m_atm); i++) {
      al->m_measure->add_bond_msr((*rec).m_atm, al->m_tws_tr->table_atm((*rec).m_atm, i),
                                  (*rec).m_idx, al->m_tws_tr->table_idx((*rec).m_atm, i));
    }

  }
  astate->make_viewport_dirty();

}
