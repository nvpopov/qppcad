#include <qppcad/ws_volume_data/ws_volume_data.hpp>
#include <qppcad/volume_tools.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


void ws_volume_data_t::gen_repr() {

}

void ws_volume_data_t::mc_polygonise(float _isolevel) {
  //volume_helper::polygonise_volume_mc_naive(*(m_first_mesh), m_volume, _isolevel, 100);
}

ws_volume_data_t::ws_volume_data_t() : ws_item_t () {

  set_default_flags(ws_item_flags_default |
                    ws_item_flags_support_tr |
                    ws_item_flags_support_moveto |
                    ws_item_flags_support_rendering );

  //TODO: Tag deprecated
  m_tag = ws_item_tag::tag_ws_volume_data;

}

void ws_volume_data_t::vote_for_view_vectors(vector3<float> &v_out_look_pos,
                                             vector3<float> &v_out_look_at) {

}

void ws_volume_data_t::render() {

  ws_item_t::render();
  app_state_t* astate = app_state_t::get_inst();

  for (auto &vol : m_volumes) {

      if (vol.m_ready_to_render && m_is_visible) {

          shader_program_t *custom_sp = vol.m_transparent_volume ? astate->sp_mvap_ssl : nullptr;
          astate->dp->begin_render_general_mesh(custom_sp);
          vector3<float> scale{1,1,1};
          vector3<float> rot{0};
          vector3<float> color{0.5f};

          //astate->glapi->glDisable(GL_CULL_FACE);
          if (vol.m_volume_type == ws_volume_t::volume_mo) {
              astate->dp->render_general_mesh(m_pos, scale, rot, vol.m_color_pos,
                                              vol.m_first_mesh, vol.m_alpha, custom_sp);
              astate->dp->render_general_mesh(m_pos, scale, rot, vol.m_color_neg,
                                              vol.m_second_mesh, vol.m_alpha, custom_sp);
            }

          if (vol.m_volume_type == ws_volume_t::volume_density) {
              astate->dp->render_general_mesh(m_pos, scale, rot, vol.m_color_vol,
                                              vol.m_first_mesh, vol.m_alpha, custom_sp);
            }
          //astate->glapi->glEnable(GL_CULL_FACE);
          astate->dp->end_render_general_mesh(custom_sp);

        }


      if (vol.m_need_to_regenerate) {

          if (!vol.m_first_mesh) vol.m_first_mesh = new mesh_t();
          if (!vol.m_second_mesh) vol.m_second_mesh = new mesh_t();

          if (vol.m_volume_type == ws_volume_t::volume_mo) {
              volume_helper::polygonise_vol_mc(*(vol.m_first_mesh),
                                               vol.m_volume,
                                               vol.m_isolevel, 100);
              volume_helper::polygonise_vol_mc(*(vol.m_second_mesh),
                                               vol.m_volume,
                                               -vol.m_isolevel, 100);
            }

          if (vol.m_volume_type == ws_volume_t::volume_density) {
              volume_helper::polygonise_vol_mc(*(vol.m_first_mesh),
                                               vol.m_volume,
                                               vol.m_isolevel, 100);
            }

          vol.m_ready_to_render = true;
          vol.m_need_to_regenerate = false;
          astate->make_viewport_dirty();
        }

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

void ws_volume_data_t::updated_internally() {

  for (auto &vol : m_volumes) {
      vol.m_need_to_regenerate = true;
      vol.m_ready_to_render = false;
    }


}

void ws_volume_data_t::save_to_json(json &data) {

}

void ws_volume_data_t::load_from_json(json &data) {

}

void ws_volume_data_t::update_isolevel(float new_isolevel) {

  app_state_t* astate = app_state_t::get_inst();
//  for (auto &vol : m_volumes) {
//    m_isolevel = new_isolevel;
//    m_ready_to_render = false;
//    m_need_to_regenerate = true;
//    }

  astate->make_viewport_dirty();

}

void ws_volume_data_t::load_from_stream(std::basic_istream<char, TRAITS> &inp,
                                        geometry<float, periodic_cell<float> > &geom,
                                        std::string &fname) {

  ws_volume_record_t new_vol_rec;

  read_cube(inp, geom, new_vol_rec.m_volume);
  new_vol_rec.m_need_to_regenerate = true;
  new_vol_rec.m_volume_name = fmt::format("CUBE {}", m_volumes.size());

  m_name = fmt::format("v_{}", fname);

  if (new_vol_rec.m_volume.m_has_negative_values) {
      new_vol_rec.m_volume_type = ws_volume_t::volume_mo;
      new_vol_rec.m_isolevel = qpp::def_isovalue_mo;
    } else {
      new_vol_rec.m_volume_type = ws_volume_t::volume_density;
      new_vol_rec.m_isolevel = qpp::def_isovalue_dens;
    }

  m_volumes.emplace_back(std::move(new_vol_rec));

}
