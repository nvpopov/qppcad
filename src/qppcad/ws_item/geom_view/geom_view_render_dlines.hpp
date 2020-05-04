#ifndef QPPCAD_WS_ATOMS_RS_DL
#define QPPCAD_WS_ATOMS_RS_DL

#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/core/app_state.hpp>

namespace qpp {

  namespace cad {

    struct geom_view_render_dlines {

        static void render(geom_view_t &al);

        static void render_bond(geom_view_t &al, const uint32_t at_num1,
                                const index &at_index1, const uint32_t at_num2,
                                const index &at_index2) {
          auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(at_num1));
          vector3<float> bcolor(0.0, 0.0, 1.0);
          if (ap_idx) {
              bcolor = ptable::get_inst()->arecs[*ap_idx - 1].m_color_jmol;
            }

          app_state_t* astate = app_state_t::get_inst();
          astate->dp->render_line(bcolor,
                                  al.m_pos.get_value() + al.m_geom->pos(at_num1, at_index1),
                                  al.m_pos.get_value() + (al.m_geom->pos(at_num2, at_index2)
                                  + al.m_geom->pos(at_num1, at_index1)) * 0.5f);
        }
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
