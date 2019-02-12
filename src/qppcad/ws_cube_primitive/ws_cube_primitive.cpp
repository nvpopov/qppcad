#include <qppcad/ws_cube_primitive/ws_cube_primitive.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_cube_primitive_t::ws_cube_primitive_t() {
  set_default_flags(ws_item_flags_default | ws_item_flags_support_rendering |
                    ws_item_flags_support_moveto | ws_item_flags_support_translation);
}

void ws_cube_primitive_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                                vector3<float> &out_look_at) {
  //do nothing
}

void ws_cube_primitive_t::render() {

  if (!m_is_visible) return;

  app_state_t* astate = app_state_t::get_inst();

  if (m_render_mode == ws_cube_rendering_mode::ws_cube_rendering_mode_solid) {
      astate->dp->begin_render_general_mesh();
      astate->dp->render_cube(m_pos, m_scale, m_color);
      astate->dp->end_render_general_mesh();
    } else {

    }

}

bool ws_cube_primitive_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string ws_cube_primitive_t::compose_item_name() {
  return "ws_cube_primitive";
}

void ws_cube_primitive_t::update(float delta_time) {
  //do nothing
}

float ws_cube_primitive_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t ws_cube_primitive_t::get_amount_of_selected_content() {
  return 0;
}

size_t ws_cube_primitive_t::get_content_count() {
  return 0;
}

void ws_cube_primitive_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void ws_cube_primitive_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

void ws_cube_primitive_t::updated_internally() {
}

