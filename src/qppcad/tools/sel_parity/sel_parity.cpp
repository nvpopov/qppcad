#include <qppcad/tools/sel_parity/sel_parity.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void sel_parity_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  //app_state_t *astate = app_state_t::get_inst();

  if (!item) {
      QMessageBox::warning(nullptr, QObject::tr("Select by parity"),
                           QObject::tr("ws_item == nullptr"));
      return;
    }

  auto al = item->cast_as<geom_view_t>();
  if (!al) {
      QMessageBox::warning(nullptr, QObject::tr("Select by parity"),
                           QObject::tr("ws_item.cast<geom_view_t>() == nullptr"));
      return;
    }

  index zero = index::D(al->m_geom->get_DIM()).all(0);

  for (auto i = 0; i < al->m_geom->num_aselected(); i++) {
    auto rec = al->m_geom->nth_selected(i);
    if (!rec) continue;
    if ((*rec).m_idx == zero) {
        std::array<int, 2> parity_d{-1,1};
        for (auto &p_x : parity_d)
          for (auto &p_y : parity_d)
            for (auto &p_z : parity_d) {
                vector3<float> new_pos{0};
                new_pos[0] = al->m_geom->coord((*rec).m_atm)[0] * p_x;
                new_pos[1] = al->m_geom->coord((*rec).m_atm)[1] * p_y;
                new_pos[2] = al->m_geom->coord((*rec).m_atm)[2] * p_z;
                std::vector<tws_node_content_t<float> > res;
                const float eps_dist = 0.01;
                al->m_tws_tr->query_sphere(eps_dist, new_pos, res);
                for (auto &res_elem : res)
                  if (res_elem.m_idx == zero) al->sel_atom(res_elem.m_atm);
              }
      }

  }

}
