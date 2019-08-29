#include <qppcad/ws_item/volume_view/volume_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view_tools.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void volume_view_tools_t::sum_volumes(volume_view_t *src1, size_t vol1_idx, float prefactor1,
                                      volume_view_t *src2, size_t vol2_idx, float prefactor2,
                                      volume_view_t *dst, std::optional<size_t> dst_id) {

  app_state_t *astate = app_state_t::get_inst();

  if (!src1 || !src2 || !dst) {
      return;
    }

  if (vol1_idx >= src1->m_volumes.size()) {
      return;
    }

  if (vol2_idx >= src2->m_volumes.size()) {
      return;
    }

  auto vol1 = src1->m_volumes[vol1_idx];
  auto vol2 = src2->m_volumes[vol2_idx];

  if (!vol1->m_volume.is_structure_equals(vol2->m_volume)) {
      return;
    }

  if (dst_id && *dst_id >= dst->m_volumes.size()) {
      return;
    }

  std::shared_ptr<ws_volume_record_t> new_vol_rec;

  if (dst_id) {
      new_vol_rec = dst->m_volumes[*dst_id];
    } else {
      new_vol_rec = std::make_shared<ws_volume_record_t>();
    }

  new_vol_rec->copy_from(*vol1, true);

  auto &volume = new_vol_rec->m_volume;

  for (int ix = 0; ix < volume.m_steps[0]; ix++)
    for (int iy = 0; iy < volume.m_steps[1]; iy++)
      for (int iz = 0; iz < volume.m_steps[2]; iz++) {

          auto val1 = get_field_value_at(ix, iy, iz, vol1->m_volume);
          auto val2 = get_field_value_at(ix, iy, iz, vol2->m_volume);

          set_field_value_at(ix, iy, iz, val1 * prefactor1 + val2 * prefactor2, volume);

  }

  new_vol_rec->m_need_to_regenerate = true;
  new_vol_rec->m_ready_to_render = false;

  if (!dst_id) dst->m_volumes.push_back(new_vol_rec);

  if (dst->m_selected) astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();

}

void volume_view_tools_t::volume_cut_sph(volume_view_t *dst,
                                         size_t volume_id,
                                         vector3<float> sph_cnt,
                                         float sph_rad,
                                         bool cut_inner) {

  app_state_t *astate = app_state_t::get_inst();

  if (!dst) {
      return;
    }

  if (volume_id >= dst->m_volumes.size()) return;

  vector3<float> gp{0};
  float rad{0};
  float val{0};
  bool rad_gr_sph_rad{false};

  auto &volume = dst->m_volumes[volume_id]->m_volume;

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

  dst->m_volumes[volume_id]->m_need_to_regenerate = true;
  dst->m_volumes[volume_id]->m_ready_to_render = false;
  astate->make_viewport_dirty();

}

void volume_view_tools_t::volume_cut_fnc(volume_view_t *dst,
                                         size_t volume_id,
                                         std::function<bool (vector3<float> &)> cut_fnc) {

  app_state_t *astate = app_state_t::get_inst();

  if (!dst) {
      return;
    }

  if (volume_id >= dst->m_volumes.size()) return;

  vector3<float> gp{0};
  bool cut_fnc_res{false};

  auto &volume = dst->m_volumes[volume_id]->m_volume;

  for (int ix = 0; ix < volume.m_steps[0]; ix++)
    for (int iy = 0; iy < volume.m_steps[1]; iy++)
      for (int iz = 0; iz < volume.m_steps[2]; iz++) {

          gp = ix * volume.m_axis[0] + iy * volume.m_axis[1] + iz * volume.m_axis[2];
          cut_fnc_res = cut_fnc(gp);

          if (cut_fnc_res) set_field_value_at(ix, iy, iz, 0.0f, volume);

        }

  dst->m_volumes[volume_id]->m_need_to_regenerate = true;
  dst->m_volumes[volume_id]->m_ready_to_render = false;
  astate->make_viewport_dirty();

}
