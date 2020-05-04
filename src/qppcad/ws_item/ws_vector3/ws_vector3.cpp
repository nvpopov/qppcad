#include <qppcad/ws_item/ws_vector3/ws_vector3.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_vector3_t::ws_vector3_t() : ws_item_t() {
  set_default_flags(ws_item_flags_default
                    | ws_item_flags_support_tr
                    | ws_item_flags_support_sel);
}

ws_vector3_t::operator vector3<float>() const {
  return m_pos.get_value();
}

void ws_vector3_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                          vector3<float> &out_look_at) {
  //do nothing
}

void ws_vector3_t::render() {
 //do nothing
}

bool ws_vector3_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string ws_vector3_t::compose_type_descr() {
  return "ws_vector3";
}

void ws_vector3_t::update(float delta_time) {
  //do nothing
}

float ws_vector3_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t ws_vector3_t::get_num_cnt_selected() {
  return 0;
}

size_t ws_vector3_t::get_content_count() {
  return 0;
}

void ws_vector3_t::save_to_json(json &data) {

  ws_item_t::save_to_json(data);

}

void ws_vector3_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);

}

bool ws_vector3_t::can_be_written_to_json() {

  return true;

}

void ws_vector3_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

}

