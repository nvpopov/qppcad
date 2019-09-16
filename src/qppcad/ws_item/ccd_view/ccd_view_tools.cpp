#include <qppcad/ws_item/ccd_view/ccd_view_tools.hpp>

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
