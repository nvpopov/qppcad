#ifndef QPPCAD_GEOM_VIEW_H
#define QPPCAD_GEOM_VIEW_H

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")
namespace py = pybind11;

#include <qppcad/core/qppcad.hpp>
#include <geom/xgeom.hpp>
#include <geom/geom_anim.hpp>
#include <geom/extents_observer.hpp>
#include <qppcad/core/history_stream_geom_adapter.hpp>
#include <geom/tws_tree.hpp>
#include <data/color.hpp>
#include <symm/index_set.hpp>
#include <io/comp_chem_data.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/render/camera.hpp>
#include <qppcad/ws_item/geom_view/geom_view_render_buffered_billboards.hpp>
#include <deque>

namespace qpp {

namespace cad {

/** Fwd decls */

class geom_view_anim_subsys_t;
class geom_view_msr_subsys_t;
class geom_view_labels_subsys_t;
class geom_view_lat_planes_subsys_t;
class geom_view_sel_groups_subsys_t;
class ws_item_t;

template<typename REAL = float>
struct scalar_partition_per_type_t {
  REAL value{0};
  size_t atype{0};
  size_t count{0};
};

template<typename REAL = float>
struct atom_dist_rec_t {
  size_t atom_type;
  REAL dist;
};

template<typename REAL = float>
struct atom_tdist_rec_t {
  size_t atom_id;
  size_t atom_type;
  REAL dist;
};

/**
* @brief The geom_view_render_style_e enum
*/
enum geom_view_render_style_e : int {
  ball_and_stick,
  suprematic_atoms,
  dynamic_lines,
  xatom_lines,
  billboards,
  buffered_billboards
};

/**
* @brief The geom_view_role_e enum
*/
enum geom_view_role_e : int {
  r_generic,
  r_uc,
  r_embc_qm,
  r_embc_cls,
  r_embc_chg
};

/**
* @brief The geom_view_color_e enum
*/
enum geom_view_color_e : int {
  color_from_ptable,
  color_from_xgeom
};

/**
* @brief The pair_dist_mode_e enum
*/
enum pair_dist_mode_e : int {
  transform_both,
  fix_first,
  fix_second
};

const int xgeom_charge       = 4;
const int xgeom_sel_vis_hide = 6;
const int xgeom_label_show   = 7;
const int xgeom_ccr          = 8;
const int xgeom_ccg          = 9;
const int xgeom_ccb          = 10;
const int xgeom_label_text   = 11;
const int xgeom_override     = 12;
const int xgeom_atom_r       = 13;
const int xgeom_external     = 14;

const int max_sel_in_deque = 4;

/**
     * @brief The geom_view_t class
     */
class geom_view_t final : public ws_item_t {

  QPP_OBJECT(geom_view_t, ws_item_t)

private:

  bool p_content_tr_started{false};

public:

  std::shared_ptr<xgeometry<float, periodic_cell<float> > > m_geom{};
  std::shared_ptr<geom_view_anim_subsys_t> m_anim{};
  std::shared_ptr<geom_view_msr_subsys_t> m_measure{};
  std::shared_ptr<geom_view_labels_subsys_t> m_labels{};
  std::shared_ptr<geom_view_lat_planes_subsys_t> m_lat_planes{};
  std::shared_ptr<geom_view_sel_groups_subsys_t> m_selg{};
  std::unique_ptr<bonding_table<float> > m_bt{};
  std::unique_ptr<tws_tree_t<float, periodic_cell<float> > > m_tws_tr{};
  std::unique_ptr<extents_observer_t<float, periodic_cell<float> > > m_ext_obs{};
  //std::set<uint16_t>                                                           m_atom_sel;
  std::unordered_set<atom_index_set_key, atom_index_set_key_hash> m_atom_idx_sel;
  std::deque<atom_index_set_key> m_atom_ord_sel;
  std::unique_ptr<geom_view_render_buffered_billboards_t> m_bs{};
  std::unordered_set<size_t> m_atom_type_to_hide;
  std::unordered_set<size_t> m_atom_type_to_hide_bond;
  std::unordered_map<size_t, vector3<float> > m_type_color_override;

  bool m_need_to_update_overview{false};
  vector3<float>   m_gizmo_barycenter;

  hs_prop_float_t m_shading_specular_power;
  hs_prop_float_t m_atom_scale_factor;
  hs_prop_float_t m_bond_scale_factor;
  hs_prop_v3f_t   m_cell_color;
  hs_prop_v3f_t   m_new_atom_pos;

  hs_prop_int_t m_render_style;
  hs_prop_v3i_t m_subcells_range;

  hs_prop_int_t m_color_mode;
  hs_prop_int_t m_role;

  hs_prop_bool_t m_draw_img_atoms;
  hs_prop_bool_t m_draw_img_bonds;
  hs_prop_bool_t m_draw_specular;
  hs_prop_bool_t m_draw_bonds;
  hs_prop_bool_t m_draw_atoms;

  hs_prop_bool_t m_bt_show_disabled_record;
  hs_prop_bool_t m_draw_cell; ///
  hs_prop_bool_t m_draw_subcells;
  hs_prop_bool_t m_draw_cell_vectors;

  hs_prop_float_t m_cell_vectors_ratio;
  hs_prop_v3f_t m_cell_vector_offset;
  hs_prop_v3f_t m_cell_vector_color;

  hs_prop_v3f_t m_subcell_color;
  hs_prop_bool_t m_sel_vis;
  hs_prop_bool_t m_sel_vis_affect_bonds;

  hist_doc_xgeom_proxy_t<float, periodic_cell<float>> m_xgeom_proxy;

  geom_view_t();

  void vote_for_view_vectors(vector3<float> &out_look_pos,
                             vector3<float> &out_look_at) override ;

  /**
   * @brief target_view
   * @param _target_view
   * @param look_from
   * @param look_to
   * @param look_up
   * @param need_to_update_camera
   */
  void target_view(cam_tv_e target_view_src,
                   vector3<float> &look_from,
                   vector3<float> &look_to,
                   vector3<float> &look_up,
                   bool &need_to_update_camera) override;

  void geometry_changed();
  void render() override;
  void render_overlay(QPainter &painter) override;

  void rebond();
  void set_cell_within_eps(float value);

  bool mouse_click(ray_t<float> *click_ray) override;
  void mouse_double_click(ray_t<float> *ray) override;

  /**
   * @brief sel_atoms
   * @param all
   */
  void sel_atoms(bool all);

  /**
   * @brief sel_atom
   * @param atom_id
   */
  void sel_atom(int atom_id);
  void toggle_atom_sel(int atom_id);

  /**
   * @brief sel_atom
   * @param atom_id
   * @param atom_idx
   */
  void sel_atom(int atom_id, index atom_idx);
  void sel_visible();

  void unsel_atom(int atom_id);
  void unsel_atom(int atom_id, index atom_idx);

  void sel_by_type(const int item_type_to_select);
  void unsel_by_type(const int item_type_to_unselect);
  void inv_sel_atoms();
  void sel_by_box(vector3<float> start_pos, vector3<float> end_pos);
  void sq_sel_by_box(const float box_scale);

  void ins_atom(const int atom_type, const vector3<float> &pos);
  void ins_atom(const std::string &atom_name, const vector3<float> &pos);

  void upd_atom(const int at_id, const vector3<float> &pos);
  void upd_atom(const int at_id, const std::string &at_name);
  void upd_atom(const int at_id, const std::string &at_name, const vector3<float> &pos);

  void transform_atom(const int at_id, const matrix3<float> &tm);
  void transform_atom(const int at_id, const matrix4<float> &tm);

  void swap_atoms(const size_t at1, const size_t at2, bool swap_names = true);

  void sv_modify_selected(bool state);
  void sv_hide_invert_selected();

  template <typename TRANSFORM_CLASS>
  void transform_sel(const TRANSFORM_CLASS &tm) {
    for (auto &elem : m_atom_idx_sel)
      if (elem.m_idx == index::D(m_geom->get_DIM()).all(0)) transform_atom(elem.m_atm, tm);
    recalc_gizmo_barycenter();
  }

  template <typename XFIELD>
  void xfill(size_t field_id, XFIELD value) {
    index zero = index::D(m_geom->get_DIM()).all(0);
    for (auto &elem : m_atom_idx_sel)
      if (elem.m_idx == zero) m_geom->xfield<XFIELD>(field_id, elem.m_atm) = value;
  }

  template <typename XFIELD>
  bool any_of_sel_xfield_equal(int xfield_id, XFIELD xfield_val) {

    auto cnt_override =
        [this, &xfield_id, &xfield_val](const atom_index_set_key &rec) -> bool {
      return this->m_geom->xfield<XFIELD>(xfield_id, rec.m_atm) == xfield_val;
    };

    return std::any_of(m_atom_idx_sel.begin(), m_atom_idx_sel.end(), cnt_override);

  }

  void xbool_invert_selected(size_t field_id);
  void copy_from_xgeom(xgeometry<float, periodic_cell<float> > &xgeom_inst);
  void copy_to_xgeom(xgeometry<float, periodic_cell<float> > &xgeom_inst,
                     bool copy_selected = false, bool copy_cell = true);
  void copy_cell(geom_view_t &src, bool rebuild_tree = true);
  void set_cell(std::optional<vector3<float>> a,
                std::optional<vector3<float>> b = std::nullopt,
                std::optional<vector3<float>> c = std::nullopt);

  std::shared_ptr<ws_item_t> clone_on_the_spot();

  void load_color_from_static_anim();

  vector3<float> get_xcolor(const size_t atm);
  void set_xcolorv(const size_t atm, const vector3<float> color);
  void set_xcolorf(const size_t atm, const float _r, const float _g, const float _b);

  std::tuple<float, float> get_min_max_xfield(const size_t xfield_id);

  void sel_atom_ngbs(const int at_id);
  void sel_selected_atoms_ngbs();

  void update_inter_atomic_dist(float new_dist,
                                const int at1, const int at2,
                                const index id1, const index id2,
                                pair_dist_mode_e mode);
  void update_inter_atomic_dist_ex(float new_dist, const int at1, const int at2,
                                   pair_dist_mode_e mode);

  void translate_selected(const vector3<float> &t_vec);
  void delete_selected_atoms();
  void delete_atoms(std::set<int> &to_delete);

  void begin_structure_change();
  void end_structure_change();
  void refine_from_frac_coord();

  std::string compose_type_descr() override;
  std::string compose_overview() override;
  void update(float delta_time) override;
  float get_bb_prescaller() override;
  bool is_bb_visible() override;

  uint32_t get_num_cnt_selected() override;
  size_t get_content_count() override;

  void on_begin_content_gizmo_translate() override;
  void apply_intermediate_translate_content(const vector3<float> &new_pos) override;
  void on_end_content_gizmo_translate() override;
  void recalc_gizmo_barycenter();
  const vector3<float> get_gizmo_content_barycenter() override;
  void updated_externally(uint32_t update_reason) override;

  void shift(const vector3<float> shift);

  void save_to_json(json &data) override;
  void load_from_json(json &data, repair_connection_info_t &rep_info) override;
  bool can_be_written_to_json() override;

  py::list get_sel_atoms(int index_offset);
  py::list get_unsel_atoms(int index_offset);
  void copy_settings(geom_view_t *src);
  void copy_xgeom_aux(geom_view_t *src);
  void copy_measurements(geom_view_t *src);

};

} // namespace qpp::cad

} // namespace qpp

#endif
