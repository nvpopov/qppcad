#include <qppcad/ui/qembed_window.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

qembed_window_t::qembed_window_t(QWidget *parent) : QFrame(parent) {

  app_state_t *astate = app_state_t::get_inst();

  main_lt = new QVBoxLayout;
  main_lt_zero_lvl = new QVBoxLayout;
  main_lt_zero_lvl->setContentsMargins(0,0,0,0);
  main_lt->setContentsMargins(0,0,0,0);
  setLayout(main_lt_zero_lvl);

  setFrameStyle(QFrame::Panel);
  setPalette(astate->m_bg_embwnd_pal);

  header_frm = new QFrame;
  header_frm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  header_frm->setProperty("s_class", "qembedwndhdr");
  header_lt = new QHBoxLayout;

  ew_header = new QLabel("qembed_window header");
  //ew_header->setProperty("s_class", "bold_label");

  header_frm->setLayout(header_lt);
  header_lt->setContentsMargins(9, 3, 4, 3);
  header_lt->addWidget(ew_header);

  main_lt_zero_lvl->addWidget(header_frm);
  main_lt_zero_lvl->addLayout(main_lt);
  main_lt_zero_lvl->setSpacing(1);

}

qembed_window_sub_header_t::qembed_window_sub_header_t(QWidget *parent) : QFrame(parent) {

  //app_state_t *astate = app_state_t::get_inst();

  m_main_lt = new QVBoxLayout;
  m_main_lt->setContentsMargins(10, 0, 0, 0);
  setLayout(m_main_lt);

  m_text = new QLabel;
  m_text->setText(tr("Subheader"));

  m_main_lt->addWidget(m_text);

  setFixedHeight(26);

}
