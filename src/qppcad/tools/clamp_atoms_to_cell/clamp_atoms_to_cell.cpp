#include <qppcad/tools/clamp_atoms_to_cell/clamp_atoms_to_cell.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void clamp_atoms_to_cell_tool_t::exec(ws_item_t *item) {

  //  for (int i = 0; i < m_geom->nat(); i++)
  //    if (m_atom_idx_sel.find(atom_index_set_key(i, index::D(m_geom->DIM).all(0))) !=
  //        m_atom_idx_sel.end() || ignore_selection) {
  //        vector3<float> pos = m_geom->pos(i);
  //        m_geom->change_pos(i, m_geom->cell.reduce(pos));
  //      }

  app_state_t *astate = app_state_t::get_inst();
  astate->log("clamp_atoms_to_cell_tool_t::exec()");

  if (astate->ws_mgr->has_wss()) {

      auto cur_ws = astate->ws_mgr->get_cur_ws();

      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto al = dynamic_cast<geom_view_t*>(cur_it);

          if (al) {
              if (al->m_geom->DIM == 3) {

                  for (int i = 0; i < al->m_geom->nat(); i++)
                    if (al->m_atom_idx_sel.find(
                          atom_index_set_key(i, index::D(al->m_geom->DIM).all(0)))
                        != al->m_atom_idx_sel.end() || ignore_selection) {
                        vector3<float> pos = al->m_geom->pos(i);
                        al->m_geom->change_pos(i, al->m_geom->cell.reduce(pos));
                      }

                } else QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                                            QObject::tr("m_geom.DIM !=3"));

            } else QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                                        QObject::tr("ws_item.type != geom_view"));

        } else QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                                    QObject::tr("Workspace not select"));

    }

  astate->make_viewport_dirty();

}
