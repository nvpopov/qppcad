#include <qppcad/ws_item/volume_view/volume_view.hpp>
#include <qppcad/render/volume_tools.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


void volume_view_t::gen_repr() {

}

void volume_view_t::mc_polygonise(float _isolevel) {
  //volume_helper::polygonise_volume_mc_naive(*(m_first_mesh), m_volume, _isolevel, 100);
}

volume_view_t::volume_view_t() : ws_item_t () {

  set_default_flags(ws_item_flags_default |
                    ws_item_flags_support_tr |
                    ws_item_flags_support_moveto |
                    ws_item_flags_support_rendering );

}

void volume_view_t::vote_for_view_vectors(vector3<float> &v_out_look_pos,
                                          vector3<float> &v_out_look_at) {

}

void volume_view_t::render() {

  ws_item_t::render();
  app_state_t* astate = app_state_t::get_inst();

  for (size_t i = 0; i < m_volumes.size(); i++) {

      if (m_volumes[i]->m_ready_to_render && m_is_visible &&
          (i == m_current_volume || m_volumes[i]->m_render_permanent)) {

          shader_program_t *custom_sp =
              m_volumes[i]->m_transparent_volume ? astate->sp_mvap_ssl : nullptr;
          astate->dp->begin_render_general_mesh(custom_sp);
          vector3<float> scale{1,1,1};
          vector3<float> rot{0};
          vector3<float> color{0.5f};

          //astate->glapi->glDisable(GL_CULL_FACE);
          if (m_volumes[i]->m_volume_type == ws_volume_t::volume_mo) {
              astate->dp->render_general_mesh(m_pos,
                                              scale,
                                              rot,
                                              m_volumes[i]->m_color_pos,
                                              m_volumes[i]->m_first_mesh,
                                              m_volumes[i]->m_alpha,
                                              custom_sp);

              astate->dp->render_general_mesh(m_pos,
                                              scale,
                                              rot,
                                              m_volumes[i]->m_color_neg,
                                              m_volumes[i]->m_second_mesh,
                                              m_volumes[i]->m_alpha,
                                              custom_sp);
            }

          if (m_volumes[i]->m_volume_type == ws_volume_t::volume_density) {
              astate->dp->render_general_mesh(m_pos,
                                              scale,
                                              rot,
                                              m_volumes[i]->m_color_vol,
                                              m_volumes[i]->m_first_mesh,
                                              m_volumes[i]->m_alpha,
                                              custom_sp);
            }
          //astate->glapi->glEnable(GL_CULL_FACE);
          astate->dp->end_render_general_mesh(custom_sp);

        }

      if (m_volumes[i]->m_need_to_regenerate) {

          if (!m_volumes[i]->m_first_mesh) m_volumes[i]->m_first_mesh = new mesh_t();
          if (!m_volumes[i]->m_second_mesh) m_volumes[i]->m_second_mesh = new mesh_t();

          if (m_volumes[i]->m_volume_type == ws_volume_t::volume_mo) {
              volume_helper::polygonise_vol_mc(*(m_volumes[i]->m_first_mesh),
                                               m_volumes[i]->m_volume,
                                               m_volumes[i]->m_isolevel,
                                               100,
                                               m_volumes[i]->m_volume.m_addr_mode);
              volume_helper::polygonise_vol_mc(*(m_volumes[i]->m_second_mesh),
                                               m_volumes[i]->m_volume,
                                               -m_volumes[i]->m_isolevel,
                                               100,
                                               m_volumes[i]->m_volume.m_addr_mode);
            }

          if (m_volumes[i]->m_volume_type == ws_volume_t::volume_density) {
              volume_helper::polygonise_vol_mc(*(m_volumes[i]->m_first_mesh),
                                               m_volumes[i]->m_volume,
                                               m_volumes[i]->m_isolevel,
                                               100,
                                               m_volumes[i]->m_volume.m_addr_mode);
            }

          m_volumes[i]->m_ready_to_render = true;
          m_volumes[i]->m_need_to_regenerate = false;
          astate->make_viewport_dirty();

        }

    }

}

bool volume_view_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string volume_view_t::compose_type_descr() {
  return "3d volume data";
}

void volume_view_t::update(float delta_time) {

}

float volume_view_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t volume_view_t::get_num_cnt_selected() {
  return 0;
}

size_t volume_view_t::get_content_count() {
  return 0;
}

void volume_view_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

  //regenerate meshes
  if (update_reason & ws_item_updf_regenerate_content) {
      for (auto &vol : m_volumes) {
          vol->m_need_to_regenerate = true;
          vol->m_ready_to_render = false;
        }
    }

}

void volume_view_t::save_to_json(json &data) {

}

void volume_view_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

}

void volume_view_t::load_from_stream(std::basic_istream<char, TRAITS> &inp,
                                     geometry<float, periodic_cell<float> > &geom,
                                     std::string &fname) {

  ws_volume_record_t new_vol_rec;

  read_cube(inp, geom, new_vol_rec.m_volume);
  new_vol_rec.m_need_to_regenerate = true;

  m_name = fmt::format("v_{}", fname);

  if (new_vol_rec.m_volume.m_has_negative_values) {
      new_vol_rec.m_volume_type = ws_volume_t::volume_mo;
      new_vol_rec.m_isolevel = qpp::def_isovalue_mo;
    } else {
      new_vol_rec.m_volume_type = ws_volume_t::volume_density;
      new_vol_rec.m_isolevel = qpp::def_isovalue_dens;
    }

  new_vol_rec.m_volume.m_name = "From CUBE";

  auto new_vol_rec_sp = std::make_shared<ws_volume_record_t>(std::move(new_vol_rec));
  m_volumes.push_back(new_vol_rec_sp);

}

void volume_view_t::volume_cut_sph(size_t volume_id,
                                   vector3<float> sph_cnt,
                                   float sph_rad,
                                   bool cut_inner) {

  app_state_t *astate = app_state_t::get_inst();

  if (volume_id >= m_volumes.size()) return;

  vector3<float> gp{0};
  float rad{0};
  float val{0};
  bool rad_gr_sph_rad{false};

  auto &volume = m_volumes[volume_id]->m_volume;

  for (int ix = 0; ix < volume.m_steps[0]; ix++)
    for (int iy = 0; iy < volume.m_steps[1]; iy++)
      for (int iz = 0; iz < volume.m_steps[2]; iz++) {

          gp = ix * volume.m_axis[0] + iy * volume.m_axis[1] + iz * volume.m_axis[2];
          val = get_field_value_at(ix, iy, iz, volume);
          rad = (gp - sph_cnt).norm();
          rad_gr_sph_rad = rad > sph_rad;

          if (cut_inner) {
              if (!rad_gr_sph_rad)
                set_field_value_at(ix, iy, iz, 0.0f, volume);
            } else {
              if (rad_gr_sph_rad)
                set_field_value_at(ix, iy, iz, val * std::exp(-(rad-sph_rad)), volume);
            }
        }

  m_volumes[volume_id]->m_need_to_regenerate = true;
  m_volumes[volume_id]->m_ready_to_render = false;
  astate->make_viewport_dirty();

}

void volume_view_t::volume_cut_fnc(size_t volume_id,
                                   std::function<bool (vector3<float>&)> cut_fnc) {

  app_state_t *astate = app_state_t::get_inst();

  if (volume_id >= m_volumes.size()) return;

  vector3<float> gp{0};
  bool cut_fnc_res{false};

  auto &volume = m_volumes[volume_id]->m_volume;

  for (int ix = 0; ix < volume.m_steps[0]; ix++)
    for (int iy = 0; iy < volume.m_steps[1]; iy++)
      for (int iz = 0; iz < volume.m_steps[2]; iz++) {

          gp = ix * volume.m_axis[0] + iy * volume.m_axis[1] + iz * volume.m_axis[2];
          cut_fnc_res = cut_fnc(gp);

          if (cut_fnc_res) set_field_value_at(ix, iy, iz, 0.0f, volume);

        }

  m_volumes[volume_id]->m_need_to_regenerate = true;
  m_volumes[volume_id]->m_ready_to_render = false;
  astate->make_viewport_dirty();

}

