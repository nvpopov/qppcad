#include <qppcad/ui/qspoiler_widget.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

qspoiler_widget_t::qspoiler_widget_t(const QString & title,
                                     QWidget *parent,
                                     bool collapsable,
                                     int spacing,
                                     int max_width,
                                     bool header_add_spacing_at_the_end,
                                     int main_lt_spacing_at_the_end) : QFrame(parent) {

  m_collapsable = collapsable;

  app_state_t *astate = app_state_t::get_inst();

  if (max_width == -1) setFixedWidth(astate->size_guide.obj_insp_splr_w());
  else setFixedWidth(max_width);

  m_main_lt = new QVBoxLayout;
  m_main_lt->setContentsMargins(0, 0, 0, 0);
  setLayout(m_main_lt);

  m_top_frm = new QFrame;
  m_top_frm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_top_frm->setProperty("s_class", "qsplrhdr");
  //top_frm->setFrameStyle(QFrame::StyledPanel);

  m_hbox_frm = new QHBoxLayout;
  m_hbox_frm->setContentsMargins(2, 2, 0, 2);
  m_top_frm->setLayout(m_hbox_frm);

  m_lbl_frm = new QLabel(title);
  m_lbl_frm->setAlignment(Qt::AlignHCenter);
  m_lbl_frm->setProperty("s_class", "qsplrhdr");

  m_action_btn = new QPushButton;
  m_action_btn->setFlat(true);
  m_action_btn->setFixedSize(QSize(astate->size_guide.spoiler_button_h(),
                                   astate->size_guide.spoiler_button_h()));
  m_action_btn->setIconSize(QSize(astate->size_guide.spoiler_button_icon_h(),
                                  astate->size_guide.spoiler_button_icon_h()));
  m_action_btn->setVisible(m_collapsable);

  if (!m_collapsable) m_hbox_frm->addSpacing(astate->size_guide.spoiler_button_h());

  m_widget_list = new QWidget;
  m_widget_list->setContentsMargins(0, 0, 0, 0);

  m_widget_list_lt = new QVBoxLayout;
  m_widget_list_lt->setContentsMargins(0, 0, 8, 0);
  m_widget_list->setLayout(m_widget_list_lt);

  m_hbox_frm->addWidget(m_action_btn);
  m_hbox_frm->addWidget(m_lbl_frm);
  if (header_add_spacing_at_the_end) m_hbox_frm->addSpacing(24);

  m_main_lt->addWidget(m_top_frm);
  m_main_lt->addWidget(m_widget_list);
  m_main_lt->setSpacing(spacing);
  m_main_lt->addSpacing(main_lt_spacing_at_the_end);

  connect(m_action_btn,
          &QPushButton::clicked,
          this,
          &qspoiler_widget_t::collapse_button_pressed);

  process_state();

}

void qspoiler_widget_t::set_top_border(bool border_enabled) {
  if (m_top_frm) m_top_frm->setProperty("s_class", border_enabled ? "qsplrhdr" : "none");
}

void qspoiler_widget_t::add_content_layout(QLayout *new_lt) {

  m_widget_list_lt->addLayout(new_lt);

}

void qspoiler_widget_t::process_state() {

  app_state_t *astate = app_state_t::get_inst();

  if (m_state) {
      m_widget_list->show();
      m_action_btn->setIcon(*astate->icons.icon_arrow_up);
    } else {
      m_widget_list->hide();
      m_action_btn->setIcon(*astate->icons.icon_arrow_down);
    }

}

void qspoiler_widget_t::collapse_button_pressed() {

  if (!m_collapsable) return;

  m_state = !m_state;
  process_state();

}
