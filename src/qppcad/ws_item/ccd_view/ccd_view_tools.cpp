#include <qppcad/ws_item/ccd_view/ccd_view_tools.hpp>
#include <qppcad/ws_item/volume_view/volume_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view_tools.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

std::vector<size_t> ccd_view_tools_t::get_states_for_tddft_root(ccd_view_t *ccd,
                                                                size_t root_id,
                                                                float min_amplitude) {

  std::set<size_t> rets;
  if (!ccd) return {};

  if (root_id < ccd->m_ccd->m_tddft_trans_rec.size()) {

      bool all_lhs_eq = ccd->m_ccd->m_tddft_trans_rec[root_id].m_all_lhs_equal;
      bool all_rhs_eq = ccd->m_ccd->m_tddft_trans_rec[root_id].m_all_rhs_equal;

      for (auto &tr_rec : ccd->m_ccd->m_tddft_trans_rec[root_id].m_transition)
        if (tr_rec.m_amplitude > min_amplitude) {
          if (!all_rhs_eq) rets.insert(tr_rec.m_to);
          if (!all_lhs_eq) rets.insert(tr_rec.m_from);
        }

    }

  std::vector<size_t> retv(rets.size());
  std::copy(rets.begin(), rets.end(), retv.begin());

  return  retv;

}

std::vector<float> ccd_view_tools_t::get_amplitudes_for_tddft_root(ccd_view_t *ccd,
                                                                   size_t root_id,
                                                                   std::vector<size_t> &states) {

  if (!ccd) return {};

  std::vector<float> retv;
  if (root_id < ccd->m_ccd->m_tddft_trans_rec.size()) {

      for (auto &tr_rec : ccd->m_ccd->m_tddft_trans_rec[root_id].m_transition) {

          if (std::find(states.begin(), states.end(), tr_rec.m_from) != states.end() ||
              std::find(states.begin(), states.end(), tr_rec.m_to) != states.end())
            retv.push_back(tr_rec.m_amplitude);

        }

    }

  return retv;

}

void ccd_view_tools_t::get_data_for_cube_sum_by_root(
    ccd_view_t *ccd,
    size_t root_id,
    float min_amplitude,
    std::vector<std::shared_ptr<volume_view_t> > &vvs,
    std::vector<size_t> &vols,
    std::vector<float> &ampls) {

  if (!ccd) return;
  if (root_id >= ccd->m_ccd->m_tddft_trans_rec.size()) return;

  std::vector<size_t> target_states = get_states_for_tddft_root(ccd, root_id, min_amplitude);
  std::vector<float> target_ampl = get_amplitudes_for_tddft_root(ccd, root_id, target_states);
  //std::vector<bool> target_accepted{target_states.size(), false};

  std::vector<std::shared_ptr<volume_view_t> > all_vv;
  std::vector<size_t> all_vr;
  std::vector<float> all_ampl;

  app_state_t *astate = app_state_t::get_inst();

  size_t ts_i{0}; // counter for target_states
  size_t vr_i{0}; // counter for volumes of volume_view

  for (auto target_state : target_states) {

      for (auto ws : astate->ws_mgr->m_ws)
        for (auto ws_itm : ws->m_ws_items)
          if (volume_view_t *vv = ws_itm->cast_as<volume_view_t>(); vv != nullptr) {

              vr_i = 0;

              for (auto vol_rec : vv->m_volumes) { // begin iterate over volume records

                  if (vol_rec->m_state_id == target_state) {

                      auto sp_vol_v = std::static_pointer_cast<volume_view_t>(ws_itm);
                      if (!sp_vol_v) continue;

                      all_vv.push_back(sp_vol_v);
                      all_vr.push_back(vr_i);
                      all_ampl.push_back(target_ampl[ts_i]);

                    }

                  vr_i++;

                } // end iterate over volume records

            }

      ts_i++;

    }

  vvs = all_vv;
  vols = all_vr;
  ampls = all_ampl;

}

void ccd_view_tools_t::build_volume_for_root(
    ccd_view_t *ccd,
    volume_view_t *target_vv,
    size_t root_id,
    float min_amplitude) {

  if (!ccd || !target_vv) return;

  std::vector<std::shared_ptr<volume_view_t> > vvs;
  std::vector<size_t> vols;
  std::vector<float> ampls;

  std::vector<volume_view_t*> vvs_as_p;
  std::transform(vvs.begin(), vvs.end(), std::back_inserter(vvs_as_p),
                 [](auto vv_sp){return vv_sp.get();});

  ccd_view_tools_t::get_data_for_cube_sum_by_root(ccd, root_id, min_amplitude, vvs, vols, ampls);
  volume_view_tools_t::sum_volumes(vvs_as_p, vols, ampls, target_vv);

}
