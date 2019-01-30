#ifndef QPP_CAD_TOOL_CLAMP_ATOMS_TO_CELL
#define QPP_CAD_TOOL_CLAMP_ATOMS_TO_CELL
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class clamp_atoms_to_cell_tool_t : public ws_item_tool_t {
      public:
        bool ignore_selection{true};
        void exec(ws_item_t *item) override;
    };
  }

}

#endif
