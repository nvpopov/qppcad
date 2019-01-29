#ifndef QPP_CAD_TOOL_SUPERCELL
#define QPP_CAD_TOOL_SUPERCELL
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class supercell_tool_t : public ws_item_tool_t {
      public:
        void exec(ws_item_t *item) override;
    };
  }

}

#endif
