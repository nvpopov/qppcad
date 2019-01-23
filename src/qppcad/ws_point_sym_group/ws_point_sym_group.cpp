#include <qppcad/ws_point_sym_group/ws_point_sym_group.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_point_sym_group_t::ws_point_sym_group_t() {
  set_default_flags(ws_item_flags_default);
}

void ws_point_sym_group_t::gen_from_geom(xgeometry<float, periodic_cell<float> > &geom,
                                         float tolerance) {

  app_state_t* astate = app_state_t::get_inst();

  find_point_symm(m_ag, geom, tolerance);
  for (auto &t_elem : m_ag.group) {
      transform_record_t new_tr;
      analyze_transform(new_tr.m_axis, new_tr.m_phi, new_tr.m_inversion, t_elem);
      new_tr.m_is_plane = new_tr.m_phi > 3.1400f && new_tr.m_phi < 3.14169f && new_tr.m_inversion;
      astate->log(fmt::format("AT {} {} {} {}",
                              new_tr.m_axis, new_tr.m_phi, new_tr.m_inversion, new_tr.m_is_plane));
      m_atf.push_back(std::move(new_tr));
    }
}

void ws_point_sym_group_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                                 vector3<float> &out_look_at) {
  //do nothing
}

void ws_point_sym_group_t::render() {
  //do nothing
}

bool ws_point_sym_group_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string ws_point_sym_group_t::compose_item_name() {
  return "ws_point_sym_group";
}

void ws_point_sym_group_t::update(float delta_time) {
  //do nothing
}

float ws_point_sym_group_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t ws_point_sym_group_t::get_amount_of_selected_content() {
  return 0;
}

size_t ws_point_sym_group_t::get_content_count() {
  return 0;
}

void ws_point_sym_group_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void ws_point_sym_group_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

