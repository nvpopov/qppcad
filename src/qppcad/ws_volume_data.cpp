#include <qppcad/ws_volume_data.hpp>
#include <qppcad/volume_tools.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


void ws_volume_data_t::gen_repr() {

}

void ws_volume_data_t::mc_polygonise(float _isolevel) {
  volume_helper::polygonise_volume_mc_naive(*(m_first_mesh), m_volume, _isolevel, 100);
}

ws_volume_data_t::ws_volume_data_t() : ws_item_t () {
  set_default_flags(ws_item_flags_default);

}

void ws_volume_data_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
                                             vector3<float> &vOutLookAt) {

}

void ws_volume_data_t::render() {
  app_state_t* astate = app_state_t::get_inst();

  if (m_ready_to_render) {

      astate->dp->begin_render_general_mesh();
      vector3<float> scale{1,1,1};
      vector3<float> rot{0};
      vector3<float> color{0.5f};
      if (m_volume.m_has_negative_values) {
          astate->dp->render_general_mesh(m_pos, scale, rot, clr_red, m_first_mesh);
          astate->dp->render_general_mesh(m_pos, scale, rot, clr_navy, m_second_mesh);
        } else {
          astate->dp->render_general_mesh(m_pos, scale, rot, clr_yellow, m_first_mesh);
        }

      astate->dp->end_render_general_mesh();
    }

  if (m_need_to_regenerate) {
      if (!m_first_mesh) m_first_mesh = new mesh_t();
      if (!m_second_mesh) m_second_mesh = new mesh_t();
      if (m_volume.m_has_negative_values) {
          volume_helper::polygonise_volume_mc_naive(*(m_first_mesh), m_volume,
                                                    def_isovalue_mo, 100);
          volume_helper::polygonise_volume_mc_naive(*(m_second_mesh), m_volume,
                                                    -def_isovalue_mo, 100);
        } else {
          volume_helper::polygonise_volume_mc_naive(*(m_first_mesh), m_volume,
                                                    def_isovalue_dens, 100);
        }
      m_ready_to_render = true;
      m_need_to_regenerate = false;
      astate->make_viewport_dirty();
    }
}

bool ws_volume_data_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string ws_volume_data_t::compose_item_name() {
  return "3d volume data";
}

void ws_volume_data_t::update(float delta_time) {

}

float ws_volume_data_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t ws_volume_data_t::get_amount_of_selected_content() {
  return 0;
}

size_t ws_volume_data_t::get_content_count() {
  return 0;
}

std::string ws_volume_data_t::get_ws_item_class_name() {
  return "ws_volume_data";
}

void ws_volume_data_t::write_to_json(json &data) {

}

void ws_volume_data_t::read_from_json(json &data) {

}
