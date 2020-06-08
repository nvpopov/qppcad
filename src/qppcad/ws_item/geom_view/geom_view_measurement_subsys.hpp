#ifndef QPPCAD_WS_ATOMS_LIST_MEASURE_S
#define QPPCAD_WS_ATOMS_LIST_MEASURE_S

#include <geom/geom.hpp>
#include <qppcad/core/qppcad.hpp>
#include <QPainter>
#include <vector>
#include <algorithm>
#include <qppcad/ws_item/ws_item_subsystem.hpp>
#include <qppcad/core/history_stream_array_proxy.hpp>

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
struct msr_bond_rec_t : public hist_doc_base_t {

  AINT m_at1;
  AINT m_at2;
  index m_idx1;
  index m_idx2;

  hs_prop_bool_t m_show;
  hs_prop_bool_t m_show_label;
  hs_prop_bool_t m_show_custom_label;
  hs_prop_str_t m_custom_label_text;
  hs_prop_v3f_t m_bond_color;

  hs_prop_int_t m_line_size;
  hs_prop_int_t m_font_size;

  hs_prop_int_t m_pair_term_width;
  hs_prop_int_t m_pair_term_style;

  hs_prop_float_t m_delta_angle;
  hs_prop_v3i_t m_delta_offset;

  hs_prop_int_t m_line_render_style;
  hs_prop_int_t m_label_render_style;

  msr_bond_rec_t (const AINT atm1, const AINT atm2, const index idx1, const index idx2)
      : m_at1(atm1), m_at2(atm2), m_idx1(idx1), m_idx2(idx2) {

    m_show.set_value(true); add_hs_child(&m_show);
    m_show_label.set_value(true); add_hs_child(&m_show_label);
    m_show_custom_label.set_value(false); add_hs_child(&m_show_custom_label);
    m_custom_label_text.set_value(""); add_hs_child(&m_custom_label_text);
    m_bond_color.set_value({0}); add_hs_child(&m_bond_color);
    m_line_size.set_value(2); add_hs_child(&m_line_size);
    m_font_size.set_value(13); add_hs_child(&m_font_size);
    m_pair_term_width.set_value(8); add_hs_child(&m_pair_term_width);
    m_pair_term_style.set_value(msr_pair_term_style_both); add_hs_child(&m_pair_term_style);
    m_delta_angle.set_value(0); add_hs_child(&m_delta_angle);
    m_delta_offset.set_value({0, 0, -1}); add_hs_child(&m_delta_offset);
    m_line_render_style.set_value(msr_line_dashed); add_hs_child(&m_line_render_style);
    m_label_render_style.set_value(msr_label_border); add_hs_child(&m_label_render_style);

  }

};

template<typename AINT = uint32_t>
struct msr_angle_rec_t : public hist_doc_base_t {

  AINT m_at1;
  AINT m_at2;
  AINT m_at3;
  index m_idx1;
  index m_idx2;
  index m_idx3;

  hs_prop_int_t m_order;

  hs_prop_bool_t m_show;
  hs_prop_bool_t m_show_label;
  hs_prop_v3f_t m_angle_color;

  msr_angle_rec_t(const AINT at1, const AINT at2, const AINT at3,
                  const index idx1, const index idx2, const index idx3) :
                                                                          m_at1(at1), m_at2(at2), m_at3(at3), m_idx1(idx1), m_idx2(idx2), m_idx3(idx3) {

    m_order.set_value(1); add_hs_child(&m_order);
    m_show.set_value(true); add_hs_child(&m_show);
    m_show_label.set_value(true); add_hs_child(&m_show_label);
    m_angle_color.set_value({0}); add_hs_child(&m_angle_color);

  }

};


class geom_view_msr_subsys_t : public ws_item_subsystem_t {

  QPP_OBJECT(geom_view_msr_subsys_t, ws_item_subsystem_t)

private:

  geom_view_t* p_owner;

public:

  //std::vector<msr_bond_rec_t<uint32_t> > m_dist_recs;
  //std::vector<msr_angle_rec_t<uint32_t> > m_angle_recs;

  hist_doc_array_proxy_t<msr_bond_rec_t<uint32_t>,
                         hs_arr_sptr_policy<msr_bond_rec_t<uint32_t>>> m_dist_recs;

  hist_doc_array_proxy_t<msr_angle_rec_t<uint32_t>,
                         hs_arr_sptr_policy<msr_angle_rec_t<uint32_t>>> m_angle_recs;

  hs_prop_bool_t m_render_dist;
  hs_prop_bool_t m_render_angle;
  hs_prop_bool_t m_render_dist_legend;
  hs_prop_bool_t m_render_angle_legend;

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

  void dist_sel_atoms(size_t msr_id);
  void dist_select_selected_atoms();
  void dist_copy(size_t msr_id);
  void dist_copy_selected();
  void dist_delete_selected();

  void save_to_json(json &data) override;
  void load_from_json(json &data) override;

  hs_result_e on_epoch_changed(epoch_t prev_epoch) override;

};

} // namespace qpp::cad

} // namespace qpp

#endif
