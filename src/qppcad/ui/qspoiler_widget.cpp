#include <qppcad/ui/qspoiler_widget.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

qspoiler_widget_t::qspoiler_widget_t(const QString & title,
                                     QWidget *parent,
                                     bool _collapsable,
                                     int _spacing,
                                     int _max_width,
                                     bool header_add_spacing_at_the_end,
                                     int main_lt_spacing_at_the_end) : QFrame(parent) {

  m_collapsable = _collapsable;
  setMaximumWidth(_max_width);

  app_state_t *astate = app_state_t::get_inst();

  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(0, 0, 0, 0);
  setLayout(main_lt);

  top_frm = new QFrame;
  top_frm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  top_frm->setPalette(astate->m_bgfg_light_pal);
  top_frm->setFrameStyle(QFrame::Panel);
  top_frm->setAutoFillBackground(true);

  hbox_frm = new QHBoxLayout;
  hbox_frm->setContentsMargins(2, 0, 0, 0);
  top_frm->setLayout(hbox_frm);

  lbl_frm = new QLabel(title);
  lbl_frm->setAlignment(Qt::AlignCenter);
  lbl_frm->setProperty("s_class", "bold_label");

  action_btn = new QPushButton;
  action_btn->setFlat(true);
  action_btn->setFixedSize(QSize(astate->size_guide.spoiler_button_h(),
                                 astate->size_guide.spoiler_button_h()));
  action_btn->setIconSize(QSize(astate->size_guide.spoiler_button_icon_h(),
                                astate->size_guide.spoiler_button_icon_h()));
  action_btn->setVisible(m_collapsable);

  widget_list = new QWidget;
  widget_list->setContentsMargins(0, 0, 0, 0);

  widget_list_lt = new QVBoxLayout;
  widget_list_lt->setContentsMargins(0, 0, 0, 0);
  widget_list->setLayout(widget_list_lt);

  hbox_frm->addWidget(action_btn);
  hbox_frm->addWidget(lbl_frm);
  if (header_add_spacing_at_the_end) hbox_frm->addSpacing(24);

  main_lt->addWidget(top_frm);
  main_lt->addWidget(widget_list);
  main_lt->setSpacing(_spacing);
  main_lt->addSpacing(main_lt_spacing_at_the_end);

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

  if (!m_collapsable) return;

  m_state = !m_state;
  process_state();

}
