#include <qppcad/ws_volume_data.hpp>

using namespace qpp;
using namespace qpp::cad;


ws_volume_data_t::ws_volume_data_t() {
  set_default_flags(ws_item_flags_default);
}

void ws_volume_data_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
                                             vector3<float> &vOutLookAt) {

}

void ws_volume_data_t::render() {

}

bool ws_volume_data_t::mouse_click(ray_t<float> *click_ray) {

}

std::string ws_volume_data_t::compose_item_name() {
  return "3d volume data";
}

void ws_volume_data_t::update(float delta_time) {

}

float ws_volume_data_t::get_bb_prescaller() {

}

uint32_t ws_volume_data_t::get_amount_of_selected_content() {

}

size_t ws_volume_data_t::get_content_count() {

}

std::string ws_volume_data_t::get_ws_item_class_name() {
  return "ws_volume_data";
}

void ws_volume_data_t::write_to_json(json &data) {

}

void ws_volume_data_t::read_from_json(json &data) {

}
