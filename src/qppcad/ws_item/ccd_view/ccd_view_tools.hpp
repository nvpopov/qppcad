#ifndef QPPCAD_CCD_VIEW_TOOLS
#define QPPCAD_CCD_VIEW_TOOLS

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view.hpp>

namespace qpp {

  namespace cad {

    struct ccd_view_tools_t {

        static std::vector<size_t> get_states_for_tddft_root(ccd_view_t *ccd,
                                                             size_t root_id,
                                                             float min_amplitude);

        static std::vector<float> get_amplitudes_for_tddft_root(ccd_view_t *ccd,
                                                                size_t root_id,
                                                                std::vector<size_t> &states);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif

