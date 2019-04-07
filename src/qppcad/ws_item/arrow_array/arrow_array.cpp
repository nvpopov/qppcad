#include <qppcad/ws_item/arrow_array/arrow_array.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

arrow_array_t::arrow_array_t() {
  set_default_flags(ws_item_flags_default);
}

void arrow_array_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                          vector3<float> &out_look_at) {
  //do nothing
}

void arrow_array_t::render() {
 //do nothing
}

bool arrow_array_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string arrow_array_t::compose_type_descr() {
  return "arrow_array";
}

void arrow_array_t::update(float delta_time) {
  //do nothing
}

float arrow_array_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t arrow_array_t::get_amount_of_selected_content() {
  return 0;
}

size_t arrow_array_t::get_content_count() {
  return 0;
}

void arrow_array_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void arrow_array_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

void arrow_array_t::updated_internally() {
}

