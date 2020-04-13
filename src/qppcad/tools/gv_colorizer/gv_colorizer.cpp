#include <qppcad/tools/gv_colorizer/gv_colorizer.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

gv_colorizer_widget_t::gv_colorizer_widget_t (QWidget *parent)
  : ws_item_inline_tool_widget_t(parent) {

}

bool gv_colorizer_widget_t::restore_cam_on_cancel() {
  return true;
}

void gv_colorizer_widget_t::on_apply() {

}

void gv_colorizer_widget_t::on_cancel() {

}

void gv_colorizer_widget_t::bind_item(ws_item_t *item) {

  ws_item_inline_tool_widget_t::bind_item(item);

}

void gv_colorizer_widget_t::updated_externally(uint32_t update_reason) {

}

ws_item_inline_tool_widget_t *gv_colorizer_tool_t::construct_inline_tool() {

  //TODO: test widget
  return new gv_colorizer_widget_t();

}

void gv_colorizer_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

}
