#ifndef QPP_CAD_RENDER_BS
#define QPP_CAD_RENDER_BS
#include <qppcad/app.hpp>
#include <qppcad/ws_atoms_list.hpp>

namespace qpp::cad {

  struct ws_atoms_list_render_bs {
      static void render(ws_atoms_list_t &al) {
        app_state_t *astate = &(c_app::get_state());
        index all_null = index::D(al.m_geom->DIM).all(0);
        astate->dp->begin_atom_render(al.m_shading_specular_power);

        // draw {0,..} atoms
        for (uint32_t i = 0; i < al.m_geom->nat(); i++)
          if (al.m_show_atoms &&
              al.m_atom_type_to_hide.find(al.m_geom->type_table(i)) == al.m_atom_type_to_hide.end())
            render_atom(al, i, index::D(al.m_geom->DIM).all(0));

        // draw imaginary atoms that appear due to periodic
        if (al.m_geom->DIM > 0 && al.m_show_atoms && al.m_show_imaginary_atoms)
          for (const auto &at_img : al.m_tws_tr->m_img_atoms)
            if (al.m_atom_type_to_hide.find(al.m_geom->type_table(at_img.m_atm)) ==
                al.m_atom_type_to_hide.end())
              render_atom(al, at_img.m_atm, at_img.m_idx);

        astate->dp->end_atom_render();
        // atom render end

        // bond render
        astate->dp->begin_render_bond();

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

        if (al.m_geom->DIM > 0 && al.m_show_imaginary_bonds && al.m_show_bonds)
          for (const auto &img_atom : al.m_tws_tr->m_img_atoms)
            for (const auto &img_bond : img_atom.m_img_bonds) {
                uint16_t id1 = img_atom.m_atm;
                uint16_t id2 = img_bond.m_atm;

                index idx1 = img_atom.m_idx;
                index idx2 = img_bond.m_idx;

                render_bond(al, id1, idx1, id2, idx2);
              }

        astate->dp->end_render_bond();
      }

      static void render_atom(ws_atoms_list_t &al, const uint32_t at_num,
                              const index &at_index) {
        auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(at_num));
        float dr_rad = 0.4f;
        vector3<float> color(0.0, 0.0, 1.0);

        if (ap_idx) {
            dr_rad = ptable::get_inst()->arecs[*ap_idx - 1].aRadius *
                     al.m_atom_scale_factor;
            color = ptable::get_inst()->arecs[*ap_idx - 1].aColorJmol;
          }

        if (al.parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT) {
            if (al.m_atom_idx_sel.find(atom_index_set_key(at_num, at_index)) !=
                al.m_atom_idx_sel.end() &&
                al.m_selected)
              color = vector3<float>(0.43f, 0.55f, 0.12f);
          }

        al.app_state_c->dp->render_atom(
              color, al.m_geom->pos(at_num, at_index) + al.m_pos, dr_rad);
      }

      static void render_bond(ws_atoms_list_t &al, const uint32_t at_num1,
                              const index &at_index1, const uint32_t at_num2,
                              const index &at_index2) {
        auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(at_num1));
        vector3<float> bcolor(0.0, 0.0, 1.0);
        if (ap_idx) {
            bcolor = ptable::get_inst()->arecs[*ap_idx - 1].aColorJmol;
          }
        al.app_state_c->dp->render_bond(
              bcolor, al.m_geom->pos(at_num1, at_index1) + al.m_pos,
              al.m_geom->pos(at_num2, at_index2) + al.m_pos, al.m_bond_scale_factor);
      }
  };

}  // namespace qpp::cad

#endif
