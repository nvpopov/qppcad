#include <qppcad/tools/nanostructures/nanostructures.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

nanostructures_widget_t::nanostructures_widget_t (QWidget *parent)
  : ws_item_inline_tool_widget_t(parent) {

}

bool nanostructures_widget_t::restore_cam_on_cancel() {
  return true;
}

void nanostructures_widget_t::on_apply() {

}

void nanostructures_widget_t::on_cancel() {

}

void nanostructures_widget_t::bind_item(ws_item_t *item) {

  ws_item_inline_tool_widget_t::bind_item(item);

}

void nanostructures_widget_t::updated_externally(uint32_t update_reason) {

}

ws_item_inline_tool_widget_t *nanostructures_tool_t::construct_inline_tool() {

  //TODO: test widget
  return new nanostructures_widget_t();

}

void nanostructures_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

}
