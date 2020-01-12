#include <qppcad/ui/ws_item_inline_tool_widget.hpp>
#include <qppcad/core/workspace.hpp>

using namespace qpp;
using namespace qpp::cad;


ws_item_inline_tool_widget_t::ws_item_inline_tool_widget_t(QWidget *parent) : QWidget (parent) {

}

void ws_item_inline_tool_widget_t::apply() {

  on_apply();

}

void ws_item_inline_tool_widget_t::cancel() {

  if (m_src && m_src->m_parent_ws && restore_cam_on_cancel()) {

      m_src->m_parent_ws->m_camera->pop_cam_state();

    }

  on_cancel();

}

bool ws_item_inline_tool_widget_t::restore_cam_on_cancel() {
  return false;
}

void ws_item_inline_tool_widget_t::on_apply() {

}

void ws_item_inline_tool_widget_t::on_cancel() {



}

void ws_item_inline_tool_widget_t::bind_item(ws_item_t *item) {

  m_src = item;

  if (m_src && m_src->m_parent_ws && restore_cam_on_cancel()) {

      m_src->m_parent_ws->m_camera->push_cam_state();

    }

}
