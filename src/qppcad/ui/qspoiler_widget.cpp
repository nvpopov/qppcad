#include <qppcad/ui/qspoiler_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

qspoiler_widget_t::qspoiler_widget_t(const QString & title,
                                     QWidget *parent) : QFrame(parent) {

  app_state_t *astate = app_state_t::get_inst();

  vbox_general_lt = new QVBoxLayout;
  vbox_general_lt->setContentsMargins(0, 0, 0, 0);
  setLayout(vbox_general_lt);
  setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  setPalette(astate->m_bg_light_pal);

  top_frm = new QFrame;
  top_frm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  top_frm->setPalette(astate->m_bgfg_light_pal);
  top_frm->setFrameStyle(QFrame::Panel);
  top_frm->setAutoFillBackground(true);

  hbox_frm = new QHBoxLayout;
  hbox_frm->setContentsMargins(2, 1, 2, 1);

  top_frm->setLayout(hbox_frm);

  lbl_frm = new QLabel(title);
  lbl_frm->setAlignment(Qt::AlignCenter);
  action_btn = new QPushButton;
  action_btn->setFlat(true);
  action_btn->setMaximumWidth(24);

  widget_list = new QWidget;

  widget_list_lt = new QVBoxLayout;
  widget_list_lt->setContentsMargins(10, 0, 10, 5);
  widget_list->setLayout(widget_list_lt);

  hbox_frm->addWidget(action_btn);
  hbox_frm->addWidget(lbl_frm);
  hbox_frm->addSpacing(24);

  vbox_general_lt->addWidget(top_frm);
  vbox_general_lt->addWidget(widget_list);

  connect(action_btn,
          &QPushButton::clicked,
          this,
          &qspoiler_widget_t::collapse_button_pressed);

  process_state();

}

void qspoiler_widget_t::add_content_layout(QLayout *new_lt) {

  widget_list_lt->addLayout(new_lt);

}

void qspoiler_widget_t::process_state() {

  app_state_t *astate = app_state_t::get_inst();

  if (m_state) {
      widget_list->show();
      action_btn->setIcon(*astate->icons.icon_arrow_up);
    } else {
      widget_list->hide();
      action_btn->setIcon(*astate->icons.icon_arrow_down);
    }

}

void qspoiler_widget_t::collapse_button_pressed() {

  m_state = !m_state;
  process_state();

}
