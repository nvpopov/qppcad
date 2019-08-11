#ifndef QPP_CAD_TOOL_NN_DIST_MSR
#define QPP_CAD_TOOL_NN_DIST_MSR
#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class nn_dist_msr_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item, uint32_t _error_ctx) override;

    };

  }

}

#endif
