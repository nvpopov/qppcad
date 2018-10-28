#include <qppcad/ws_atoms_list_render_billboards.hpp>

namespace qpp {

  namespace cad {

    void ws_atoms_list_render_billboards::render(ws_atoms_list_t &al) {

      // std::cout << "fhfg" << std::endl;
      app_state_t *astate = &(c_app::get_state());

      astate->zup_quad->begin_render_batch();
      astate->bs_sphere_program->begin_shader_program();
      astate->bs_sphere_program->set_u(sp_u_name::m_proj, astate->camera->m_mat_proj.data());

      //(GL_CULL_FACE);
      vector3<float> color(0.0, 0.0, 1.0);
      float dr_rad = 0.4f;
      auto cached_last_atom_type = -1;
      Eigen::Transform<float, 3, Eigen::Affine> t;
      matrix4<float> mat_model_view;

      astate->default_program->set_u(sp_u_name::f_specular_intensity, &al.m_shading_specular_power);

      float draw_specular = al.m_draw_specular;
      astate->default_program->set_u(sp_u_name::f_specular_alpha, &draw_specular);

      for (uint32_t i = 0; i < al.m_geom->nat(); i++) {

          if (cached_last_atom_type != al.m_geom->type(i)) {
              auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(i));
              if (ap_idx) {
                  dr_rad = ptable::get_inst()->arecs[*ap_idx - 1].aRadius *
                           al.m_atom_scale_factor * 2.0f;
                  color = ptable::get_inst()->arecs[*ap_idx - 1].aColorJmol;
                }
              astate->bs_sphere_program->set_u(sp_u_name::v_color, color.data());
              astate->bs_sphere_program->set_u(sp_u_name::f_scale, &dr_rad);
              cached_last_atom_type = al.m_geom->type(i);
            }

          t = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
//          t.prerotate(matrix3<float>::Identity());
//          t.prescale(vector3<float>(dr_rad, dr_rad, dr_rad));
          t.pretranslate(al.m_geom->pos(i)+al.m_pos);
          mat_model_view = astate->camera->m_mat_view * t.matrix();

          astate->bs_sphere_program->set_u(sp_u_name::m_model_view, mat_model_view.data());
          astate->zup_quad->render_batch();

        }

      astate->bs_sphere_program->end_shader_program();
      astate->zup_quad->end_render_batch();
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
