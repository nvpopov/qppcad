#include <qppcad/ws_item/pgf_producer/pgf_producer.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

pgf_producer_t::pgf_producer_t() {
  set_default_flags(ws_item_flags_default);
}

void pgf_producer_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                          vector3<float> &out_look_at) {
  //do nothing
}

void pgf_producer_t::render() {
 //do nothing
}

bool pgf_producer_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string pgf_producer_t::compose_item_name() {
  return "pgf_producer";
}

void pgf_producer_t::update(float delta_time) {
  //do nothing
}

float pgf_producer_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t pgf_producer_t::get_amount_of_selected_content() {
  return 0;
}

size_t pgf_producer_t::get_content_count() {
  return 0;
}

void pgf_producer_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void pgf_producer_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

void pgf_producer_t::updated_internally() {
}

