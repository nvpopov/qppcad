#ifndef QPPCAD_CCD_VIEW_TOOLS
#define QPPCAD_CCD_VIEW_TOOLS

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view.hpp>

namespace qpp {

namespace cad {

class volume_view_t;

struct ccd_view_tools_t {

  static std::vector<size_t> get_states_for_tddft_root(ccd_view_t *ccd,
                                                       size_t root_id,
                                                       float min_amplitude);

  static std::vector<float> get_amplitudes_for_tddft_root(ccd_view_t *ccd,
                                                          size_t root_id,
                                                          std::vector<size_t> &states);

  /**
         * @brief this function searches for the corresponding volume_view with required
         * state number and spin values among all open workspaces
         * @param ccd
         * @param root_id
         * @param min_amplitude
         * @param vvs
         * @param vols
         */
  static void get_data_for_cube_sum_by_root(
      ccd_view_t *ccd,
      size_t root_id,
      float min_amplitude,
      std::vector<std::shared_ptr<volume_view_t> > &vvs,
      std::vector<size_t> &vols,
      std::vector<float> &ampls);

  static void build_volume_for_root(
      ccd_view_t *ccd,
      volume_view_t *target_vv,
      size_t root_id,
      float min_amplitude);

};

} // namespace qpp::cad

} // namespace qpp

#endif

