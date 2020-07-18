#include <qppcad/ws_item/geom_view/geom_view_render_dlines.hpp>
#include <qppcad/core/app_state.hpp>
namespace qpp {

  namespace cad {

    void geom_view_render_dlines::render (geom_view_t &al) {

      app_state_t* astate = app_state_t::get_inst();
      index all_null = index::D(al.m_geom->get_DIM()).all(0);

      //auto draw_atoms = al.m_draw_atoms.get_value();
      //auto draw_img_atoms = al.m_draw_img_atoms.get_value();
      auto draw_bonds = al.m_draw_bonds.get_value();
      auto draw_img_bonds = al.m_draw_img_bonds.get_value();

      astate->dp->begin_render_line();
      if (draw_bonds)
        for (uint16_t i = 0; i < al.m_geom->nat(); i++)
          for (uint16_t j = 0; j < al.m_tws_tr->n(i); j++) {
              uint16_t id1 = i;
              uint16_t id2 = al.m_tws_tr->table_atm(i, j);
              index idx2 = al.m_tws_tr->table_idx(i, j);

              if (idx2 == all_null || draw_img_bonds)
                render_bond(al, id1, all_null, id2, idx2);

              if (idx2 != all_null && draw_img_bonds)
                render_bond(al, id2, idx2, id1, all_null);
            }
      astate->dp->end_render_line();

    }



  }

}
