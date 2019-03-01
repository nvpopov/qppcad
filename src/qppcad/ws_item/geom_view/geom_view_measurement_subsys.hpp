#ifndef QPP_CAD_WS_ATOMS_LIST_MEASURE_S
#define QPP_CAD_WS_ATOMS_LIST_MEASURE_S

#include <geom/geom.hpp>
#include <qppcad/qppcad.hpp>
#include <QPainter>
#include <vector>
#include <algorithm>

namespace qpp {

  namespace cad {

    class geom_view_t;

    enum msr_line_render_style : int {
      msr_line_solid,
      msr_line_dashed,
    };

    template<typename AINT = uint32_t>
    struct msr_bond_rec_t {

      AINT m_at1;
      AINT m_at2;
      index m_idx1;
      index m_idx2;
      bool m_show{true};
      msr_line_render_style m_line_render_style{msr_line_render_style::msr_line_dashed};

      msr_bond_rec_t (const AINT _atm1,
                      const AINT _atm2,
                      const index _idx1,
                      const index _idx2) :
        m_at1(_atm1), m_at2(_atm2), m_idx1(_idx1), m_idx2(_idx2) {}

    };

    template<typename AINT = uint32_t>
    struct msr_angle_rec_t {

      AINT m_at1;
      AINT m_at2;
      AINT m_at3;
      index m_idx1;
      index m_idx2;
      index m_idx3;

      int m_order{1};

      bool m_show{true};

      msr_angle_rec_t (const AINT _at1,
                       const AINT _at2,
                       const AINT _at3,
                       const index _idx1,
                       const index _idx2,
                       const index _idx3) :
        m_at1(_at1), m_at2(_at2), m_at3(_at3), m_idx1(_idx1), m_idx2(_idx2), m_idx3(_idx3) {}
    };


    class geom_view_msr_subsys_t {

    private:

      geom_view_t* p_owner;

    public:

      std::vector<msr_bond_rec_t<uint32_t> > m_dist_recs;
      std::vector<msr_angle_rec_t<uint32_t> > m_angle_recs;

      int m_cur_dist_rec_ui{0}; // 0 - none, real = +1
      int m_cur_angle_rec_ui{0}; // 0 - none, real = +1

      geom_view_msr_subsys_t (geom_view_t &_p_owner);

      float dist(const size_t idx);

      void add_bond_msr(const uint32_t atm1,
                        const uint32_t atm2,
                        const index idx1,
                        const index idx2);

      void add_angle_msr(const uint32_t atm1,
                         const uint32_t atm2,
                         const uint32_t atm3,
                         const index idx1,
                         const index idx2,
                         const index idx3);

      void rm_bond_msr(const size_t measure_idx);
      void rm_angle_msr(const size_t measure_idx);

      std::optional<size_t> is_bond_msr_exists(const uint32_t atm1,
                                               const uint32_t atm2,
                                               const index idx1,
                                               const index idx2);

      std::optional<size_t> is_angle_msr_exists(const uint32_t atm1,
                                                const uint32_t atm2,
                                                const uint32_t atm3,
                                                const index idx1,
                                                const index idx2,
                                                const index idx3);
      void render ();
      void render_overlay(QPainter &painter);
      void notify_atom_has_been_deleted(const uint32_t atm);

    };

  }

}

#endif
