#ifndef QPPCAD_TOOL_SHIFT_CENTER_OF_CELL
#define QPPCAD_TOOL_SHIFT_CENTER_OF_CELL
#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

namespace cad {

class shift_start_of_cell_tool_t : public ws_item_tool_t {

public:

  void exec(ws_item_t *item, uint32_t _error_ctx) override;

};

} // namespace cad

} // namespace qpp

#endif
