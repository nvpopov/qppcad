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

  if (m_src && m_src->m_parent_ws) {

    if (m_restore_epoch_on_cancel && m_epoch_before)
      m_src->m_parent_ws->checkout_to_epoch(*m_epoch_before);

  }

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

  if (m_src && m_src->m_parent_ws) {

    if (restore_cam_on_cancel()) m_src->m_parent_ws->m_camera->push_cam_state();
    m_epoch_before = std::optional<epoch_t>{m_src->m_parent_ws->get_cur_epoch()};

  } else {

    m_epoch_before = std::nullopt;

  }

}
