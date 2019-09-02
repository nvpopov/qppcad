#include <qppcad/ws_item/volume_view/volume_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view_tools.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void volume_view_tools_t::sum_volumes(std::vector<volume_view_t*> src,
                                      std::vector<size_t> vol_idx,
                                      std::vector<float> prefactor,
                                      volume_view_t *dst,
                                      std::optional<size_t> dst_id) {

  app_state_t *astate = app_state_t::get_inst();

  auto reduce_and = [](std::vector<bool> &bool_vec) {
    return std::accumulate(bool_vec.begin(), bool_vec.end(), true, std::logical_and<bool>());
  };

  /* determine whether all input data dimensions are valid */
  std::set<size_t> input_data_dim;
  input_data_dim.insert(src.size());
  input_data_dim.insert(vol_idx.size());
  input_data_dim.insert(prefactor.size());

  if (input_data_dim.size() != 1) {
      throw std::invalid_argument("Error: Input data dimensions are invalid!");
    }

  /* determine whether all volume_views are valid */
  std::vector<bool> src_not_null_v;
  src_not_null_v.resize(src.size(), false);
  std::transform(src.begin(), src.end(), src_not_null_v.begin(),
                 [](volume_view_t *vv_src){ return vv_src != nullptr;});
  if (!reduce_and(src_not_null_v)) {
      throw std::invalid_argument("Error: Invalid volume views!");
    }

  /* determine whether all volume views indicies are valid */
  std::vector<size_t> src_vv_c; // volume count per volume view
  src_vv_c.resize(src.size(), false);
  std::transform(src.begin(), src.end(), src_vv_c.begin(),
                 [](volume_view_t *vv_src){ return vv_src->m_volumes.size();});

  std::vector<bool> src_vv_idx_valid_v;
  src_vv_idx_valid_v.reserve(src.size());
  std::transform(vol_idx.begin(), vol_idx.end(), src_vv_c.begin(),
                 std::back_inserter(src_vv_idx_valid_v),
                 [](size_t pvol_idx, size_t psrc_vv_c){ return pvol_idx < psrc_vv_c;});
  if (!reduce_and(src_vv_idx_valid_v)) {
      throw std::invalid_argument("Error: Invalid indicies of volume views!");
    }

  /* determine whether all volumes have equal structure */
  scalar_volume_t<float> &first_vol = src[0]->m_volumes[vol_idx[0]]->m_volume;
  std::vector<bool> src_v_equal_v;
  src_v_equal_v.reserve(src.size());
  std::transform(src.begin(), src.end(), vol_idx.begin(), std::back_inserter(src_v_equal_v),
                 [&first_vol](volume_view_t *vv_src, size_t pvol_idx){
                   return vv_src->m_volumes[pvol_idx]->m_volume.is_structure_equals(first_vol);
                 });
  if (!reduce_and(src_v_equal_v)) {
      throw std::invalid_argument("Error: Src volume`s structure are different !");
    }

  if (dst_id && *dst_id >= dst->m_volumes.size()) {
      throw std::invalid_argument("Error: Dst id is wrong!");
    }

  std::shared_ptr<ws_volume_record_t> new_vol_rec;

  if (dst_id) {
      new_vol_rec = dst->m_volumes[*dst_id];
    } else {
      new_vol_rec = std::make_shared<ws_volume_record_t>();
    }

  new_vol_rec->copy_from(*src[0]->m_volumes[vol_idx[0]], true);

  auto &volume = new_vol_rec->m_volume;

  for (int ix = 0; ix < volume.m_steps[0]; ix++)
    for (int iy = 0; iy < volume.m_steps[1]; iy++)
      for (int iz = 0; iz < volume.m_steps[2]; iz++) {

          float accum{0};

          for (size_t vol_i = 0; vol_i < src.size(); vol_i++)
            accum += prefactor[vol_i] *
                get_field_value_at(ix, iy, iz, src[vol_i]->m_volumes[vol_idx[vol_i]]->m_volume);

          set_field_value_at(ix, iy, iz, accum, volume);

  }

  new_vol_rec->m_need_to_regenerate = true;
  new_vol_rec->m_ready_to_render = false;
  new_vol_rec->m_volume.m_name = fmt::format("sum_{}", dst->m_volumes.size());

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
