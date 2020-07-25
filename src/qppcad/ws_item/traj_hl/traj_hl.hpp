#ifndef QPPCAD_WS_TRAJ_HIGHLIGHT
#define QPPCAD_WS_TRAJ_HIGHLIGHT

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/render/mesh.hpp>

namespace qpp {

namespace cad {

class geom_view_t;

enum traj_hl_style_e {
  traj_hl_style_points,
  traj_hl_style_lines,
  traj_hl_style_lines_loop,
  traj_hl_style_xmarks,
  traj_hl_style_spheres
};

class traj_hl_t final: public ws_item_t {

  QPP_OBJECT(traj_hl_t, ws_item_t)

public:

  geom_view_t *m_binded_gv{};
  std::shared_ptr<ws_item_t> m_src{};
  std::unique_ptr<mesh_t> m_line_mesh;

  bool m_need_to_rebuild{true};
  size_t m_anim_id{1};
  size_t m_atom_id{0};

  hs_prop_int_t m_traj_style;
  hs_prop_float_t m_elem_size;
  hs_prop_v3f_t m_traj_color;

  traj_hl_t();

  void vote_for_view_vectors(vector3<float> &out_look_pos,
                             vector3<float> &out_look_at) override ;
  void render() override;
  bool mouse_click(ray_t<float> *click_ray) override;

  std::string compose_type_descr() override;
  void update(float delta_time) override;
  float get_bb_prescaller() override;
  void updated_externally(uint32_t update_reason) override;
  uint32_t get_num_cnt_selected() override;
  size_t get_content_count() override;

  void on_leader_changed() override;
  void on_leader_call() override;

  void rebuild_line_mesh();

  void save_to_json(json &data) override;
  void load_from_json(json &data, repair_connection_info_t &rep_info) override;

};

} // namespace qpp::cad

} // namespace qpp

#endif
