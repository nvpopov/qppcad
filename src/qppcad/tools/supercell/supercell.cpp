#include <qppcad/tools/supercell/supercell.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void supercell_tool_t::exec(ws_item_t *item) {

  app_state_t *astate = app_state_t::get_inst();
  astate->log("Supercell tools::exec()");

}
