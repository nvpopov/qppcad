#include <qppcad/ws_item/pdos_view/pdos_view.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

pdos_view_t::pdos_view_t() {
  set_default_flags(ws_item_flags_default);
}

void pdos_view_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                          vector3<float> &out_look_at) {
  //do nothing
}

void pdos_view_t::render() {
 //do nothing
}

bool pdos_view_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string pdos_view_t::compose_type_descr() {
  return "pdos_view";
}

void pdos_view_t::update(float delta_time) {
  //do nothing
}

float pdos_view_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t pdos_view_t::get_num_cnt_selected() {
  return 0;
}

size_t pdos_view_t::get_content_count() {
  return 0;
}

void pdos_view_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void pdos_view_t::load_from_json(json &data, repair_connection_info_t &rep_info) {
  ws_item_t::load_from_json(data, rep_info);
}

void pdos_view_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

}

