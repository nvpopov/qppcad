#ifndef QPPCAD_WS_MATRIX3
#define QPPCAD_WS_MATRIX3
#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

namespace cad {

class ws_matrix3_t final : public ws_item_t {

  QPP_OBJECT(ws_matrix3_t, ws_item_t)

public:

  hs_prop_m3f_t m_data;

  ws_matrix3_t();

  void vote_for_view_vectors(vector3<float> &out_look_pos,
                             vector3<float> &out_look_at) override ;
  void render() override;
  bool mouse_click(ray_t<float> *click_ray) override;

  std::string compose_type_descr() override;
  void update(float delta_time) override;
  float get_bb_prescaller() override;
  void updated_externally(uint32_t update_reason) override;
        uint32_t get_num_cnt_selected() override;
  size_t content_count() override;

  void save_to_json(json &data) override;
  void load_from_json(json &data, repair_connection_info_t &rep_info) override;
  bool can_be_written_to_json() override;

};

} // namespace qpp::cad

} // namespace qpp

#endif
