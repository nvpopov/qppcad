#ifndef QPPCAD_TOOL_CLAMP_ATOMS_TO_CELL
#define QPPCAD_TOOL_CLAMP_ATOMS_TO_CELL

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

namespace cad {

class clamp_atoms_to_cell_tool_t : public ws_item_tool_t {
public:
  bool ignore_selection{true};
  void exec(ws_item_t *item, uint32_t _error_ctx) override;
};

} // namespace qpp::cad

} // namespace qpp

#endif
