#include <qppcad/ws_arrow_primitive/ws_arrow_primitive.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_arrow_primitive_t::ws_arrow_primitive_t() {
  set_default_flags(ws_item_flags_default);
}

void ws_arrow_primitive_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                                 vector3<float> &out_look_at) {
  //do nothing
}

void ws_arrow_primitive_t::render() {
  //do nothing
}

bool ws_arrow_primitive_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string ws_arrow_primitive_t::compose_item_name() {
  return "ws_arrow_primitive";
}

void ws_arrow_primitive_t::update(float delta_time) {
  //do nothing
}

float ws_arrow_primitive_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t ws_arrow_primitive_t::get_amount_of_selected_content() {
  return 0;
}

size_t ws_arrow_primitive_t::get_content_count() {
  return 0;
}

bool ws_arrow_primitive_t::can_be_written_to_json() {
  return true;
}

void ws_arrow_primitive_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void ws_arrow_primitive_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

void ws_arrow_primitive_t::updated_internally() {
}

