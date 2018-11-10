#include <qppcad/ws_comp_chem_data.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_comp_chem_data_t::ws_comp_chem_data_t() {
  set_default_flags(ws_item_flags_default);
}

void ws_comp_chem_data_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
                                                vector3<float> &vOutLookAt) {
  //do nothing
}

void ws_comp_chem_data_t::render() {

}

void ws_comp_chem_data_t::render_overlay() {

}

void ws_comp_chem_data_t::render_ui() {
  ws_item_t::render_ui();
  ImGui::BulletText("I am a placeholder");
}

void ws_comp_chem_data_t::render_work_panel_ui() {

}

void ws_comp_chem_data_t::td_context_menu_edit_item() {

}

void ws_comp_chem_data_t::td_context_menu_edit_content() {

}

bool ws_comp_chem_data_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}


std::string ws_comp_chem_data_t::compose_item_name() {
  return "Type = [comp. chem. data]";
}

void ws_comp_chem_data_t::update(float delta_time) {

}

float ws_comp_chem_data_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t ws_comp_chem_data_t::get_amount_of_selected_content() {
  return 0;
}

size_t ws_comp_chem_data_t::get_content_count() {
  return 0;
}

std::string ws_comp_chem_data_t::get_ws_item_class_name() {
  return "ws_comp_chem_data";
}

void ws_comp_chem_data_t::write_to_json(json &data) {

}

void ws_comp_chem_data_t::read_from_json(json &data) {

}
