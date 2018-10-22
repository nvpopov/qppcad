#include <qppcad/ws_atoms_list_render_dlines.hpp>

namespace qpp {

  namespace cad {

    void ws_atoms_list_render_dlines::render (ws_atoms_list_t &al) {

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



  }

}
