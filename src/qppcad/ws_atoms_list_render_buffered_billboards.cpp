#include <qppcad/ws_atoms_list_render_buffered_billboards.hpp>
#include <qppcad/ws_atoms_list.hpp>

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
        }

      m_tb_pos->init();
      m_tb_color_s->init();

    }

    template<typename DATA, typename REAL, typename AINT>
    void ws_atoms_list_render_buffered_billboards_t<DATA, REAL, AINT>::render () {

      m_tb_pos->bind(0);
      m_tb_color_s->bind(1);


      m_tb_pos->unbind(0);
      m_tb_color_s->unbind(1);

    }

    template class
    ws_atoms_list_render_buffered_billboards_t<ws_atoms_list_t, float>;

  }

}
