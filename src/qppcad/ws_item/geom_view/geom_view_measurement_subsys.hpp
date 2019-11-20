#ifndef QPPCAD_WS_ATOMS_LIST_MEASURE_S
#define QPPCAD_WS_ATOMS_LIST_MEASURE_S

#include <geom/geom.hpp>
#include <qppcad/core/qppcad.hpp>
#include <QPainter>
#include <vector>
#include <algorithm>
#include <qppcad/ws_item/ws_item_subsystem.hpp>

namespace qpp {

  namespace cad {

    class geom_view_t;

    enum msr_line_style_e{
      msr_line_solid,
      msr_line_dotted,
      msr_line_dashed,
      msr_line_ddashed,
      msr_line_dddashed
    };

    enum msr_label_style_e : int {
      msr_label_std,
      msr_label_outline,
      msr_label_border
    };

    enum msr_pair_term_style : int {
      msr_pair_term_style_none,
      msr_pair_term_style_first,
      msr_pair_term_style_second,
      msr_pair_term_style_both
    };

    template<typename AINT = uint32_t>
    struct msr_bond_rec_t {

      AINT m_at1;
      AINT m_at2;
      index m_idx1;
      index m_idx2;
      bool m_show{true};
      bool m_show_label{true};
      bool m_show_custom_label{false};
      std::string m_custom_label_text;
      vector3<float> m_bond_color{0};

      int m_line_size{2};
      int m_font_size{13};

      int m_pair_term_width{8};
      msr_pair_term_style m_pair_term_style{msr_pair_term_style::msr_pair_term_style_both};

      float m_delta_angle{0};
      vector3<int> m_delta_offset{0, 0, -1};

      msr_line_style_e m_line_render_style{msr_line_style_e::msr_line_dashed};
      msr_label_style_e m_label_render_style{msr_label_style_e::msr_label_border};

      msr_bond_rec_t (const AINT _atm1, const AINT _atm2,
                      const index _idx1, const index _idx2) :
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
      bool m_show_label{true};
      vector3<float> m_angle_color{0};

      msr_angle_rec_t(const AINT _at1, const AINT _at2, const AINT _at3,
                      const index _idx1, const index _idx2, const index _idx3) :
        m_at1(_at1), m_at2(_at2), m_at3(_at3), m_idx1(_idx1), m_idx2(_idx2), m_idx3(_idx3) {}

    };


    class geom_view_msr_subsys_t : public ws_item_subsystem_t {

      QPP_OBJECT(geom_view_msr_subsys_t, ws_item_subsystem_t)

    private:

      geom_view_t* p_owner;

    public:

      std::vector<msr_bond_rec_t<uint32_t> > m_dist_recs;
      std::vector<msr_angle_rec_t<uint32_t> > m_angle_recs;

      bool m_render_dist{true};
      bool m_render_angle{true};
      bool m_render_dist_legend{false};
      bool m_render_angle_legend{false};

      int m_cur_dist_rec_ui{0}; // 0 - none, real = +1
      int m_cur_angle_rec_ui{0}; // 0 - none, real = +1

      geom_view_msr_subsys_t (geom_view_t &_p_owner);

      float dist(const size_t idx);

      void add_bond_msr(const uint32_t atm1, const uint32_t atm2,
                        const index idx1, const index idx2);

      void add_angle_msr(const uint32_t atm1, const uint32_t atm2, const uint32_t atm3,
                         const index idx1, const index idx2, const index idx3);

      void rm_bond_msr(const size_t measure_idx);
      void rm_angle_msr(const size_t measure_idx);

      opt<size_t> is_bond_msr_exists(const uint32_t atm1, const uint32_t atm2,
                                     const index idx1, const index idx2);

      opt<size_t> is_angle_msr_exists(const uint32_t atm1, const uint32_t atm2, const uint32_t atm3,
                                      const index idx1, const index idx2, const index idx3);
      void render ();
      void render_overlay(QPainter &painter);
      void notify_atom_has_been_deleted(const uint32_t atm);

      void dist_sel_atoms(size_t msr_id);
      void dist_select_selected_atoms();
      void dist_copy(size_t msr_id);
      void dist_copy_selected();
      void dist_delete_selected();

      void save_to_json(json &data) override;
      void load_from_json(json &data) override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
