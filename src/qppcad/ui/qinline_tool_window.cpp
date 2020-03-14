#include <qppcad/ui/qinline_tool_window.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

qinline_tool_window_t::qinline_tool_window_t(qinline_tool_type_e inline_tool_type,
                                             QWidget *parent) : qembed_window_t(parent) {

  m_inline_tool_type = inline_tool_type;
  setProperty("border_class", "border_right");

  m_btn_apply_tool = new QPushButton(tr("Apply"));
  m_btn_cancel_tool = new QPushButton(tr("Cancel"));

  connect(m_btn_apply_tool, &QPushButton::clicked,
          this, &qinline_tool_window_t::apply_triggered);

  connect(m_btn_cancel_tool, &QPushButton::clicked,
          this, &qinline_tool_window_t::close_triggered);

  m_header_lt->addWidget(m_btn_apply_tool);
  m_header_lt->addWidget(m_btn_cancel_tool);
  m_header_lt->addStretch(1);
  m_header_lt->setContentsMargins(9, 6, 4, 6);

  m_main_lt->setContentsMargins(2, 1, 4, 1);
  m_main_lt->addStretch(1);

  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  if (m_inline_tool_type == qinline_tool_type_e::tool_vertical) mark_as_vertical();
  else mark_as_horizontal();

}

void qinline_tool_window_t::mark_as_horizontal() {

  app_state_t *astate = app_state_t::get_inst();
  setMaximumHeight(astate->size_guide.inline_tool_horizontal_max_height());
  setMinimumHeight(astate->size_guide.inline_tool_horizontal_min_height());
  m_inline_tool_type = qinline_tool_type_e::tool_horizontal;

}

void qinline_tool_window_t::mark_as_vertical() {

  app_state_t *astate = app_state_t::get_inst();
  setFixedWidth(astate->size_guide.inline_tool_vertical_max_width());
  m_inline_tool_type = qinline_tool_type_e::tool_vertical;

}

bool qinline_tool_window_t::is_active() {

  return m_cur_wdgt && m_cur_wdgt->isVisible();

}

void qinline_tool_window_t::apply_triggered() {

  if (m_cur_wdgt) {
      m_cur_wdgt->apply();
    }

  app_state_t *astate = app_state_t::get_inst();

  if (m_inline_tool_type == qinline_tool_type_e::tool_vertical) {
      astate->astate_evd->set_left_inline_tool_visibility(false);
    } else {
      astate->astate_evd->set_bottom_inline_tool_visibility(false);
    }

}

void qinline_tool_window_t::close_triggered() {

  if (m_cur_wdgt) {
      m_cur_wdgt->cancel();
    }

  app_state_t *astate = app_state_t::get_inst();

  if (m_inline_tool_type == qinline_tool_type_e::tool_vertical) {
      astate->astate_evd->set_left_inline_tool_visibility(false);
    } else {
      astate->astate_evd->set_bottom_inline_tool_visibility(false);
    }

}
