#include <qppcad/ws_volume_data/ws_volume_data.hpp>
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
  set_default_flags(ws_item_flags_default | ws_item_flags_support_translation |
                    ws_item_flags_support_moveto | ws_item_flags_support_rendering );
  m_tag = ws_item_tag::tag_ws_volume_data;
}

void ws_volume_data_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
                                             vector3<float> &vOutLookAt) {

}

void ws_volume_data_t::render() {

  app_state_t* astate = app_state_t::get_inst();

  if (m_ready_to_render && m_is_visible) {

      astate->dp->begin_render_general_mesh(astate->sp_mvap_ssl);
      vector3<float> scale{1,1,1};
      vector3<float> rot{0};
      vector3<float> color{0.5f};

      if (m_volume_type == ws_volume_t::volume_mo) {
          astate->dp->render_general_mesh(m_pos, scale, rot, clr_red,
                                          m_first_mesh, m_alpha, astate->sp_mvap_ssl);
          astate->dp->render_general_mesh(m_pos, scale, rot, clr_navy, m_second_mesh,
                                          m_alpha, astate->sp_mvap_ssl);
        }

      if (m_volume_type == ws_volume_t::volume_density) {
          astate->dp->render_general_mesh(m_pos, scale, rot, clr_yellow, m_first_mesh,
                                          m_alpha, astate->sp_mvap_ssl);
        }

      astate->dp->end_render_general_mesh(astate->sp_mvap_ssl);
    }


  if (m_need_to_regenerate) {

      if (!m_first_mesh) m_first_mesh = new mesh_t();

      if (!m_second_mesh) m_second_mesh = new mesh_t();

      if (m_volume_type == ws_volume_t::volume_mo) {
          volume_helper::polygonise_volume_mc_naive(*(m_first_mesh), m_volume, m_isolevel, 100);
          volume_helper::polygonise_volume_mc_naive(*(m_second_mesh), m_volume, -m_isolevel, 100);
        }

      if (m_volume_type == ws_volume_t::volume_density) {
          volume_helper::polygonise_volume_mc_naive(*(m_first_mesh), m_volume, m_isolevel, 100);
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

void ws_volume_data_t::save_to_json(json &data) {

}

void ws_volume_data_t::load_from_json(json &data) {

}

void ws_volume_data_t::update_isolevel(float new_isolevel) {

  app_state_t* astate = app_state_t::get_inst();

  m_isolevel = new_isolevel;
  m_ready_to_render = false;
  m_need_to_regenerate = true;

  astate->make_viewport_dirty();

}

void ws_volume_data_t::load_from_stream(std::basic_istream<char, TRAITS> &inp,
                                        geometry<float, periodic_cell<float> > &geom,
                                        std::string &fname) {
  read_cube(inp, geom, m_volume);
  m_need_to_regenerate = true;
  m_name = fmt::format("v_{}", fname);

  if (m_volume.m_has_negative_values) {
      m_volume_type = ws_volume_t::volume_mo;
      m_isolevel = qpp::def_isovalue_mo;
    }
  else {
      m_volume_type = ws_volume_t::volume_density;
      m_isolevel = qpp::def_isovalue_dens;
    }
}
