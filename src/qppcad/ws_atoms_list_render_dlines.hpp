#ifndef QPP_CAD_WS_ATOMS_RS_DL
#define QPP_CAD_WS_ATOMS_RS_DL
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/app_state.hpp>

namespace qpp {

  namespace cad {

    struct ws_atoms_list_render_dlines {

        static void render(ws_atoms_list_t &al);

        static void render_bond(ws_atoms_list_t &al, const uint32_t at_num1,
                                const index &at_index1, const uint32_t at_num2,
                                const index &at_index2) {
          auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(at_num1));
          vector3<float> bcolor(0.0, 0.0, 1.0);
          if (ap_idx) {
              bcolor = ptable::get_inst()->arecs[*ap_idx - 1].aColorJmol;
            }

          app_state_t* astate = app_state_t::get_inst();
          astate->dp->render_line(bcolor,
                                  al.m_pos + al.m_geom->pos(at_num1, at_index1),
                                  al.m_pos + (al.m_geom->pos(at_num2, at_index2) +
                                              al.m_geom->pos(at_num1, at_index1)) * 0.5f,
                                  al.m_bond_scale_factor * 3.0f);
        }
    };

  }

}

#endif
