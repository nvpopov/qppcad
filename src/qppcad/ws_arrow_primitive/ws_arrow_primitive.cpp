#include <qppcad/ws_arrow_primitive/ws_arrow_primitive.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_arrow_primitive_t::ws_arrow_primitive_t() {

  set_default_flags(ws_item_flags_default |
                    ws_item_flags_support_rendering |
                    ws_item_flags_support_moveto |
                    ws_item_flags_support_tr |
                    ws_item_flags_translate_emit_upd_event);

}

void ws_arrow_primitive_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                                 vector3<float> &out_look_at) {
  //do nothing
}

void ws_arrow_primitive_t::render() {

  if (!m_is_visible) return;

  app_state_t* astate = app_state_t::get_inst();

  if (m_need_to_recalc_render_data) {
      recalc_render_data();
      m_need_to_recalc_render_data = false;
    }
  //draw axes
  astate->dp->begin_render_general_mesh();
  astate->dp->render_general_mesh(m_model_body, clr_red, astate->mesh_cylinder);
  astate->dp->render_general_mesh(m_model_arrow, clr_red, astate->mesh_unit_cone);
  astate->dp->end_render_general_mesh();

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

void ws_arrow_primitive_t::recalc_render_data() {

  matrix4<float> mat_model = matrix4<float>::Identity();
  float scale_val = m_axis_scale * 1.0f;
  float arrow_scale = 0.5f;

  vector3<float> start = m_pos;
  vector3<float> end = m_arrow_to - (m_arrow_to-m_pos).normalized() * arrow_scale;

  mat_model.block<3,1>(0,3) = start;
  mat_model.block<3,1>(0,2) = end - start;

  vector3<float> vec_axis_norm = mat_model.block<3,1>(0,2).normalized();

  mat_model.block<3,1>(0,0) = vec_axis_norm.unitOrthogonal() * scale_val;
  mat_model.block<3,1>(0,1) = vec_axis_norm.cross(mat_model.block<3,1>(0,0));
  mat_model.block<3,1>(0,3) = start ;

  m_model_body = mat_model;

  //aux
  vector3<float> start_aux = m_arrow_to - (m_arrow_to-m_pos).normalized() * arrow_scale;
  vector3<float> end_aux = m_arrow_to;

//  start_aux *= m_axis_len_mod;
//  end_aux *= m_axis_len_mod;

  matrix4<float> mat_model_aux = matrix4<float>::Identity();
  mat_model_aux.block<3,1>(0,3) = start_aux;
  mat_model_aux.block<3,1>(0,2) = end_aux - start_aux;

  vector3<float> vec_axis_norm_aux = mat_model_aux.block<3,1>(0,2).normalized();
  mat_model_aux.block<3,1>(0,0) = vec_axis_norm_aux.unitOrthogonal() * scale_val;
  mat_model_aux.block<3,1>(0,1) = vec_axis_norm_aux.cross(mat_model_aux.block<3,1>(0,0));
  mat_model_aux.block<3,1>(0,3) = start_aux;

  m_model_arrow = mat_model_aux;
}

void ws_arrow_primitive_t::updated_internally() {

  recalc_render_data();

}

