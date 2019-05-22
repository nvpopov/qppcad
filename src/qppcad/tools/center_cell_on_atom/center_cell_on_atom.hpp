#ifndef QPPCAD_TOOL_CENTER_CELL_ON_ATOM
#define QPPCAD_TOOL_CENTER_CELL_ON_ATOM
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class center_cell_on_atom_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item, uint32_t _error_ctx) override;

    };

  } // namespace cad

} // namespace qpp

#endif
