#include <qppcad/ui/ws_item_inline_tool_widget.hpp>

using namespace qpp;
using namespace qpp::cad;


ws_item_inline_tool_widget_t::ws_item_inline_tool_widget_t(QWidget *parent) : QWidget (parent) {

}

void ws_item_inline_tool_widget_t::on_apply() {

}

void ws_item_inline_tool_widget_t::on_cancel() {

}

void ws_item_inline_tool_widget_t::bind_item(ws_item_t *item) {

  m_src = item;

}
