#include <qppcad/ws_item/cube_primitive/cube_primitive.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/core/json_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

cube_primitive_t::cube_primitive_t() : ws_item_t() {

  set_default_flags(ws_item_flags_default
                    | ws_item_flags_support_rendering
                    | ws_item_flags_support_moveto
                    | ws_item_flags_support_tr);

  begin_recording(hs_doc_rec_type_e::hs_doc_rec_init);

  add_hs_child(&m_render_mode);
  add_hs_child(&m_scale);
  add_hs_child(&m_color);
  add_hs_child(&m_alpha);
  add_hs_child(&m_alpha_enabled);

  m_render_mode.set_value(ws_cube_rendering_mode::render_solid);
  m_scale.set_value({1, 1, 1});
  m_color.set_value({1, 0, 0});
  m_alpha.set_value(0.9f);
  m_alpha_enabled.set_value(false);

  end_recording();

}

void cube_primitive_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                             vector3<float> &out_look_at) {
  //do nothing
}

void cube_primitive_t::render() {

  if (!m_is_visible.get_value()) return;

  app_state_t* astate = app_state_t::get_inst();
  auto scale = m_scale.get_value();

  if (m_render_mode.get_value() == ws_cube_rendering_mode::render_solid) {

    if (m_alpha_enabled.get_value()) {

      astate->dp->begin_render_line();
      vector3<float> sc_a {scale[0], 0, 0};
      vector3<float> sc_b {0, scale[1], 0};
      vector3<float> sc_c {0, 0, scale[2]};
      vector3<float> pos = m_pos.get_value() - scale * 0.5f;
      astate->dp->render_cell_3d(m_color.get_value(), sc_a, sc_b, sc_c, pos);
      astate->dp->end_render_line();

      astate->dp->begin_render_general_mesh(astate->sp_mvap_ssl);
      vector3<float> scalev = scale * 0.5f;
      astate->dp->render_general_mesh(m_pos.get_value(), scalev, vector3<float>{0},
                                      m_color.get_value(), astate->mesh_unit_cube,
                                      m_alpha.get_value(), astate->sp_mvap_ssl);
      astate->dp->end_render_general_mesh(astate->sp_mvap_ssl);

    }
    else {
      astate->dp->begin_render_general_mesh();
      vector3<float> vscale = scale * 0.5f;
      astate->dp->render_general_mesh(m_pos.get_value(), vscale, vector3<float>{0},
                                      m_color.get_value(), astate->mesh_unit_cube);
      astate->dp->end_render_general_mesh();
    }

  } else {

    astate->dp->begin_render_line();
    vector3<float> sc_a {scale[0], 0, 0};
    vector3<float> sc_b {0, scale[1], 0};
    vector3<float> sc_c {0, 0, scale[2]};
    vector3<float> pos = m_pos.get_value() - scale * 0.5f;
    astate->dp->render_cell_3d(m_color.get_value(), sc_a, sc_b, sc_c, pos);
    astate->dp->end_render_line();

  }

}

bool cube_primitive_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string cube_primitive_t::compose_type_descr() {
  return "cube_primitive";
}

void cube_primitive_t::update(float delta_time) {
  //do nothing
}

float cube_primitive_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t cube_primitive_t::get_num_cnt_selected() {
  return 0;
}

size_t cube_primitive_t::content_count() {
  return 0;
}

void cube_primitive_t::save_to_json(json &data) {

  ws_item_t::save_to_json(data);

  json_io::hs_save_vec3(JSON_WS_CUBE_P_SCALE, m_scale, data);
  json_io::hs_save_vec3(JSON_WS_CUBE_P_COLOR, m_color, data);
  json_io::hs_save_var(JSON_WS_CUBE_P_STYLE, m_render_mode, data);
  json_io::hs_save_var(JSON_WS_CUBE_P_EALPHA, m_alpha_enabled, data);
  json_io::hs_save_var(JSON_WS_CUBE_P_ALPHA, m_alpha, data);

}

void cube_primitive_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);

  json_io::hs_load_vec3(JSON_WS_CUBE_P_SCALE, m_scale, data);
  json_io::hs_load_vec3(JSON_WS_CUBE_P_COLOR, m_color, data);
  json_io::hs_load_var(JSON_WS_CUBE_P_STYLE, m_render_mode, data);
  json_io::hs_load_var(JSON_WS_CUBE_P_EALPHA, m_alpha_enabled, data);
  json_io::hs_load_var(JSON_WS_CUBE_P_ALPHA, m_alpha, data);

}

bool cube_primitive_t::can_be_written_to_json() {
  return true;
}

void cube_primitive_t::updated_externally(uint32_t update_reason) {
  ws_item_t::updated_externally(update_reason);
}

