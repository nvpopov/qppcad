#include <qppcad/ws_item/ws_matrix3/ws_matrix3.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_matrix3_t::ws_matrix3_t() : ws_item_t() {

  set_default_flags(ws_item_flags_default);

  add_hs_child(&m_data);

}

void ws_matrix3_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                         vector3<float> &out_look_at) {
  //do nothing
}

void ws_matrix3_t::render() {
  //do nothing
}

bool ws_matrix3_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string ws_matrix3_t::compose_type_descr() {
  return "ws_matrix3";
}

void ws_matrix3_t::update(float delta_time) {
  //do nothing
}

float ws_matrix3_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t ws_matrix3_t::get_num_cnt_selected() {
  return 0;
}

size_t ws_matrix3_t::content_count() {
  return 0;
}

void ws_matrix3_t::save_to_json(json &data) {

  ws_item_t::save_to_json(data);

  data[JSON_WS_MATRIX3_DATA] = json::array({});

  auto mdata = m_data.get_value();

  for (size_t i = 0; i < 3; i++)
    for (size_t q = 0; q < 3; q++)
      data[JSON_WS_MATRIX3_DATA].push_back(mdata(i,q));

}

void ws_matrix3_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);

  auto data_it = data.find(JSON_WS_MATRIX3_DATA);

  matrix3<float> mdata{0};

  if (data_it != data.end())
    for (size_t i = 0; i < 3; i++)
      for (size_t q = 0; q < 3; q++) {
        size_t idx = i*3 + q;
        mdata(i,q) = data_it.value()[idx];
      }

  m_data.set_value(std::move(mdata));

}

bool ws_matrix3_t::can_be_written_to_json() {
  return true;
}

void ws_matrix3_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

}

