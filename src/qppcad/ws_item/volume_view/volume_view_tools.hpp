#ifndef QPPCAD_VOLUME_VIEW_TOOLS
#define QPPCAD_VOLUME_VIEW_TOOLS

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/volume_view/volume_view.hpp>

namespace qpp {

  namespace cad {

    struct volume_view_tools_t {

      static void sum_volumes(volume_view_t *src1, size_t vol1_idx, float prefactor1,
                              volume_view_t *src2, size_t vol2_idx, float prefactor2,
                              volume_view_t *dst, std::optional<size_t> dst_id = std::nullopt);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
