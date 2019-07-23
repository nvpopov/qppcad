#include <qppcad/ws_item/geom_view/geom_view_render_billboards.hpp>
#include <qppcad/app_state.hpp>

namespace qpp {

  namespace cad {

    void geom_view_render_billboards::render(geom_view_t &al) {

      // std::cout << "fhfg" << std::endl;
      app_state_t* astate = app_state_t::get_inst();

      astate->mesh_zup_quad->begin_render_batch();
      astate->sp_bs_sphere->begin_shader_program();
      astate->sp_bs_sphere->set_u(sp_u_name::m_proj, astate->camera->m_mat_proj.data());

      //(GL_CULL_FACE);

      auto zero_idx = index::D(al.m_geom->DIM).all(0);

      vector3<float> color(0.0, 0.0, 1.0);
      float dr_rad = 0.4f;

      Eigen::Transform<float, 3, Eigen::Affine> t;
      matrix4<float> mat_model_view;

      astate->sp_default->set_u(sp_u_name::f_specular_intensity, &al.m_shading_specular_power);
      index null_idx = index::D(al.m_geom->DIM).all(0);

      float draw_specular = al.m_draw_specular;
      astate->sp_default->set_u(sp_u_name::f_specular_alpha, &draw_specular);

      for (uint32_t i = 0; i < al.m_geom->nat(); i++) {

          auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(i));
          if (ap_idx) {
              dr_rad = ptable::get_inst()->arecs[*ap_idx - 1].m_radius*2;
              color = ptable::get_inst()->arecs[*ap_idx - 1].m_color_jmol;
            }

          if (al.m_geom->xfield<bool>(xgeom_override, i)) {
              dr_rad = al.m_geom->xfield<float>(xgeom_atom_r, i);
            }

          if (!al.m_type_color_override.empty()) {
              auto it = al.m_type_color_override.find(al.m_geom->type_table(i));
              if (it != al.m_type_color_override.end()) color = it->second;
            }

          dr_rad = dr_rad * al.m_atom_scale_factor;

          if (al.m_color_mode == geom_view_color_e::color_from_xgeom ||
              al.m_geom->xfield<bool>(xgeom_override, i)) {
              color[0] = al.m_geom->xfield<float>(xgeom_ccr, i);
              color[1] = al.m_geom->xfield<float>(xgeom_ccg, i);
              color[2] = al.m_geom->xfield<float>(xgeom_ccb, i);
            }

          if (al.m_parent_ws->m_edit_type == ws_edit_e::edit_content) {
              if (al.m_atom_idx_sel.find(atom_index_set_key(i, zero_idx)) !=
                  al.m_atom_idx_sel.end() && al.m_selected)
                color = vector3<float>(0.43f, 0.55f, 0.12f);
            }

          astate->sp_bs_sphere->set_u(sp_u_name::v_color, color.data());
          astate->sp_bs_sphere->set_u(sp_u_name::f_scale, &dr_rad);

          t = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
          //          t.prerotate(matrix3<float>::Identity());
          //          t.prescale(vector3<float>(dr_rad, dr_rad, dr_rad));
          t.pretranslate(al.m_geom->pos(i)+al.m_pos);
          mat_model_view = astate->camera->m_mat_view * t.matrix();

          astate->sp_bs_sphere->set_u(sp_u_name::m_model_view, mat_model_view.data());
          astate->mesh_zup_quad->render_batch();

        }

      astate->sp_bs_sphere->end_shader_program();
      astate->mesh_zup_quad->end_render_batch();
      //glEnable(GL_CULL_FACE);

    }

    void geom_view_render_billboards::render_atom (geom_view_t &al, const uint32_t at_num,
                                                   const index &at_index) {

    }

    void geom_view_render_billboards::render_bond (geom_view_t &al,
                                                   const uint32_t at_num1,
                                                   const index &at_index1,
                                                   const uint32_t at_num2,
                                                   const index &at_index2) {


    }

  }

}
