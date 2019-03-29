#ifndef QPP_CAD_TOOL_COLORIZE_BY_DIST
#define QPP_CAD_TOOL_COLORIZE_BY_DIST
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class colorize_by_dist_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item) override;

    };

  }

}

#endif
