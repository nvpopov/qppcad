#include <qppcad/ui/qinline_tool_window.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

qinline_tool_window_t::qinline_tool_window_t(QWidget *parent) : qembed_window_t(parent) {

  m_btn_apply_tool = new QPushButton(tr("Apply"));
  m_btn_cancel_tool = new QPushButton(tr("Cancel"));

  connect(m_btn_apply_tool, &QPushButton::clicked,
          this, &qinline_tool_window_t::apply_triggered);

  connect(m_btn_cancel_tool, &QPushButton::clicked,
          this, &qinline_tool_window_t::close_triggered);

  header_lt->addWidget(m_btn_apply_tool);
  header_lt->addWidget(m_btn_cancel_tool);
  header_lt->addStretch(1);
  header_lt->setContentsMargins(9, 6, 4, 6);

  main_lt->setContentsMargins(2, 1, 4, 1);
  main_lt->addStretch(1);

  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

}

void qinline_tool_window_t::mark_as_horizontal() {

  app_state_t *astate = app_state_t::get_inst();
  setMaximumHeight(astate->size_guide.inline_tool_horizontal_max_height());
  setMinimumHeight(astate->size_guide.inline_tool_horizontal_min_height());
  m_is_horizontal = true;

}

void qinline_tool_window_t::mark_as_vertical() {

  app_state_t *astate = app_state_t::get_inst();
  setFixedWidth(astate->size_guide.inline_tool_vertical_max_width());
  m_is_vertical = true;

}

void qinline_tool_window_t::apply_triggered() {

  if (m_cur_wdgt) {
      m_cur_wdgt->on_apply();
    }

  app_state_t *astate = app_state_t::get_inst();
  if (m_is_vertical) astate->astate_evd->set_left_inline_tool_visibility(false);
  if (m_is_horizontal) astate->astate_evd->set_bottom_inline_tool_visibility(false);

}

void qinline_tool_window_t::close_triggered() {

  if (m_cur_wdgt) {
      m_cur_wdgt->on_cancel();
    }

  app_state_t *astate = app_state_t::get_inst();
  if (m_is_vertical) astate->astate_evd->set_left_inline_tool_visibility(false);
  if (m_is_horizontal) astate->astate_evd->set_bottom_inline_tool_visibility(false);

}
