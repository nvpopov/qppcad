#ifndef QPPCAD_WS_ATOMS_LIST_ANYM_SUBSYS
#define QPPCAD_WS_ATOMS_LIST_ANYM_SUBSYS

#include <geom/geom.hpp>
#include <geom/geom_anim.hpp>
#include <geom/tws_tree.hpp>
#include <qppcad/ws_item/ws_item_subsystem.hpp>
#include <vector>
#include <algorithm>

namespace qpp {

namespace cad {

class geom_view_t;

class geom_view_anim_subsys_t : public ws_item_subsystem_t {

  QPP_OBJECT(geom_view_anim_subsys_t, ws_item_subsystem_t)

private:
    geom_view_t* p_owner;

public:
  std::vector<geom_anim_record_t<float>>  m_anim_data;
  float m_cur_anim_time{0.0f};
  int m_cur_anim{0};
  bool m_force_non_animable;

  hs_prop_float_t m_anim_frame_time;
  hs_prop_bool_t m_rebuild_bonds_in_anim;
  hs_prop_bool_t m_play_cyclic;
  hs_prop_bool_t m_play_anim;
  hs_prop_bool_t m_interpolate_anim;

  explicit geom_view_anim_subsys_t (geom_view_t &_p_owner);

  void update_geom_to_anim (const int anim_id, const float current_frame);
  void update_and_set_anim (const int anim_id, const float current_frame);
  void update_geom_to_anim ();

  void update_current_frame_to(const int new_frame);
  void update_current_frame_to_begin();
  void update_current_frame_to_end();

  void update (const float delta_time) override ;
  bool animable () const;
  void next_anim();
  void prev_anim();
  void traverse_anim(int travel_dir);

  /**
   * @brief make_interpolated_anim
   * @param new_anim_name
   * @param num_frames
   * @param anim1_id
   * @param anim1_frm
   * @param anim2_id
   * @param anim2_frm
   */
  void make_interpolated_anim(std::string new_anim_name,
                              size_t num_frames,
                              size_t anim1_id,
                              size_t anim1_frm,
                              size_t anim2_id,
                              size_t anim2_frm,
                              bool bounce = false);

  void make_animable();
  void make_anim(const std::string &anim_name,
                 const geom_anim_e anim_type,
                 const size_t num_frames);
  void make_static_anim(bool do_it_anyway = false);

  int current_frame_truncated();

  /**
   * @brief commit_atom_pos
   * @param atom_id
   * @param lerp_from_start
   * @param propagate_to_the_end
   */
  void commit_atom_pos(size_t atom_id,
                       bool lerp_from_start = false,
                       bool propagate_to_the_end = false);

  void manual_frame_manipulate(const int frame_mod);

  geom_anim_record_t<float>* get_current_anim();
  geom_anim_e get_cur_anim_type() const;
  geom_anim_e get_anim_type_by_idx(size_t anim_idx) const;

  size_t get_total_anims () const;
  size_t frame_count (const size_t anim_id) const;
  size_t current_frame_count () const;

  bool is_cell_animable(size_t anim_id, size_t frame_id);
  vector3<float> get_cell_vectors(size_t anim_id, size_t frame_id, size_t cell_id);

  vector3<float> get_atom_pos(size_t anim_id, size_t frame_id, size_t atom_id);
};

} // namespace qpp::cad

} // namespace qpp

#endif
