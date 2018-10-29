#include <qppcad/ws_atoms_list_render_buffered_billboards.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/app.hpp>

namespace qpp {

  namespace cad {

    template<typename DATA, typename REAL, typename AINT>
    void ws_atoms_list_render_buffered_billboards_t<DATA, REAL, AINT>::init () {

      m_tb_pos = std::make_unique<texture_buffer_t<REAL> >();
      m_tb_pos->m_data.resize(p_owner->m_geom->nat(), 4);
      m_tb_color_s = std::make_unique<texture_buffer_t<REAL> >();
      m_tb_color_s->m_data.resize(p_owner->m_geom->nat(), 4);

      for (auto i = 0 ; i < p_owner->m_geom->nat(); i++) {
          vector3<REAL> ap = p_owner->m_geom->pos(i) + p_owner->m_pos;
          vector4<REAL> pos_data{ap[0], ap[1], ap[2], REAL(0.0)};
          m_tb_pos->m_data.row(i) = pos_data;
          auto ap_idx = ptable::number_by_symbol(p_owner->m_geom->atom(i));
          REAL rad = ptable::get_inst()->arecs[*ap_idx - 1].aRadius *
                     p_owner->m_atom_scale_factor * 2.0f;
          vector3<REAL> clr = ptable::get_inst()->arecs[*ap_idx - 1].aColorJmol;
          vector4<REAL> clr_data{clr[0], clr[1], clr[2], rad};
          m_tb_color_s->m_data.row(i) = clr_data;

          std::cout << fmt::format("BDEBUG {} {} {} {} | {} {} {} {}\n",
                                     m_tb_pos->m_data.row(i)[0],
              m_tb_pos->m_data.row(i)[1],
              m_tb_pos->m_data.row(i)[2],
              m_tb_pos->m_data.row(i)[3],
               m_tb_color_s->m_data.row(i)[0],
              m_tb_color_s->m_data.row(i)[1],
              m_tb_color_s->m_data.row(i)[2],
              m_tb_color_s->m_data.row(i)[3]
              );
        }

      m_tb_pos->init();
      m_tb_color_s->init();

    }

    template<typename DATA, typename REAL, typename AINT>
    void ws_atoms_list_render_buffered_billboards_t<DATA, REAL, AINT>::render () {

      app_state_t *astate = &(c_app::get_state());

      astate->buf_bs_program->begin_shader_program();

      astate->buf_bs_program->set_u(sp_u_name::m_proj, astate->camera->m_mat_proj.data());
      astate->buf_bs_program->set_u(sp_u_name::m_view, astate->camera->m_mat_view.data());

      m_tb_pos->bind(0);
      m_tb_color_s->bind(1);
      astate->buf_bs_program->set_u_sampler(sp_u_name::texture_0, GL_TEXTURE0);
      astate->buf_bs_program->set_u_sampler(sp_u_name::texture_1, GL_TEXTURE1);

      astate->zup_quad->begin_render_batch();


      glDrawElementsInstanced( GL_TRIANGLES,
                               6,
                               GL_UNSIGNED_INT,
                               0,
                               p_owner->m_geom->nat() );

      astate->zup_quad->end_render_batch();
      m_tb_pos->unbind(0);
      m_tb_color_s->unbind(1);
      astate->buf_bs_program->end_shader_program();

    }

    template class
    ws_atoms_list_render_buffered_billboards_t<ws_atoms_list_t, float>;

  }

}
