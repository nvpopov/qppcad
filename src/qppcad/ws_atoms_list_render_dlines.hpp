#ifndef QPP_CAD_WS_ATOMS_RS_DL
#define QPP_CAD_WS_ATOMS_RS_DL
#include <qppcad/app.hpp>
#include <qppcad/ws_atoms_list.hpp>

namespace qpp::cad {

  struct ws_atoms_list_render_dlines {

      static void render(ws_atoms_list_t &al){
        app_state_t *astate = &(c_app::get_state());
        index all_null = index::D(al.m_geom->DIM).all(0);

        astate->dp->begin_render_line();
        if (al.m_show_bonds)
          for (uint16_t i = 0; i < al.m_geom->nat(); i++)
            for (uint16_t j = 0; j < al.m_tws_tr->n(i); j++) {
                uint16_t id1 = i;
                uint16_t id2 = al.m_tws_tr->table_atm(i, j);
                index idx2 = al.m_tws_tr->table_idx(i, j);

                if (idx2 == all_null || al.m_show_imaginary_bonds)
                  render_bond(al, id1, all_null, id2, idx2);

                if (idx2 != all_null && al.m_show_imaginary_bonds)
                  render_bond(al, id2, idx2, id1, all_null);
              }
        astate->dp->end_render_line();
      }

      static void render_bond(ws_atoms_list_t &al, const uint32_t at_num1,
                              const index &at_index1, const uint32_t at_num2,
                              const index &at_index2) {
        auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(at_num1));
        vector3<float> bcolor(0.0, 0.0, 1.0);
        if (ap_idx) {
            bcolor = ptable::get_inst()->arecs[*ap_idx - 1].aColorJmol;
          }

        app_state_t *astate = &(c_app::get_state());
        astate->dp->render_line(bcolor,
                                al.m_pos + al.m_geom->pos(at_num1, at_index1),
                                al.m_pos + (al.m_geom->pos(at_num2, at_index2) +
                                al.m_geom->pos(at_num1, at_index1)) * 0.5f,
                                al.m_bond_scale_factor * 3.0f);
      }
  };

}

#endif
