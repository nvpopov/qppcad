#include <qppcad/ws_atoms_list/ws_atoms_list_render_billboards.hpp>
#include <qppcad/app_state.hpp>

namespace qpp {

  namespace cad {

    void ws_atoms_list_render_billboards::render(ws_atoms_list_t &al) {

      // std::cout << "fhfg" << std::endl;
      app_state_t* astate = app_state_t::get_inst();

      astate->mesh_zup_quad->begin_render_batch();
      astate->sp_bs_sphere->begin_shader_program();
      astate->sp_bs_sphere->set_u(sp_u_name::m_proj, astate->camera->m_mat_proj.data());

      //(GL_CULL_FACE);
      vector3<float> color(0.0, 0.0, 1.0);
      float dr_rad = 0.4f;
      auto cached_last_atom_type = -1;
      auto cached_last_atom_selected = false;
      Eigen::Transform<float, 3, Eigen::Affine> t;
      matrix4<float> mat_model_view;

      astate->sp_default->set_u(sp_u_name::f_specular_intensity, &al.m_shading_specular_power);
      index null_idx = index::D(al.m_geom->DIM).all(0);

      float draw_specular = al.m_draw_specular;
      astate->sp_default->set_u(sp_u_name::f_specular_alpha, &draw_specular);

      for (uint32_t i = 0; i < al.m_geom->nat(); i++) {

          if (cached_last_atom_type != al.m_geom->type(i)) {
              auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(i));
              if (ap_idx) {
                  dr_rad = ptable::get_inst()->arecs[*ap_idx - 1].m_radius *
                           al.m_atom_scale_factor * 2.0f;
                  color = ptable::get_inst()->arecs[*ap_idx - 1].m_color_jmol;
                }
              astate->sp_bs_sphere->set_u(sp_u_name::v_color, color.data());
              astate->sp_bs_sphere->set_u(sp_u_name::f_scale, &dr_rad);
              cached_last_atom_type = al.m_geom->type(i);
            }

          if (al.m_parent_ws->m_edit_type == ws_edit_t::edit_content) {
              if (al.m_atom_idx_sel.find(atom_index_set_key(i, null_idx)) != al.m_atom_idx_sel.end()
                  && al.m_selected) {
                  color = vector3<float>(0.43f, 0.55f, 0.12f);
                  astate->sp_bs_sphere->set_u(sp_u_name::v_color, color.data());

                }
            }

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

    void ws_atoms_list_render_billboards::render_atom (ws_atoms_list_t &al, const uint32_t at_num,
                                                       const index &at_index) {

    }

    void ws_atoms_list_render_billboards::render_bond (ws_atoms_list_t &al,
                                                       const uint32_t at_num1,
                                                       const index &at_index1,
                                                       const uint32_t at_num2,
                                                       const index &at_index2) {


    }


  }

}
