#include <qppcad/ui/qembed_window.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

qembed_window_t::qembed_window_t(QWidget *parent) : QFrame(parent) {

  app_state_t *astate = app_state_t::get_inst();

  m_main_lt = new QVBoxLayout;
  m_main_lt_zero_lvl = new QVBoxLayout;
  m_main_lt_zero_lvl->setContentsMargins(0,0,0,0);
  m_main_lt->setContentsMargins(0,0,0,0);
  setLayout(m_main_lt_zero_lvl);

  setFrameStyle(QFrame::Panel);
  setPalette(astate->m_bg_embwnd_pal);

  m_header_frm = new QFrame;
  m_header_frm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_header_frm->setProperty("s_class", "qembedwndhdr");
  m_header_lt = new QHBoxLayout;

  //auto x = fmt::format("{} {} {}",2,3);
  m_ew_header = new QLabel("qembed_window header");
  //ew_header->setProperty("s_class", "bold_label");

  m_header_frm->setLayout(m_header_lt);
  m_header_lt->setContentsMargins(9, 3, 4, 3);
  m_header_lt->addWidget(m_ew_header);

  m_main_lt_zero_lvl->addWidget(m_header_frm);
  m_main_lt_zero_lvl->addLayout(m_main_lt);
  m_main_lt_zero_lvl->setSpacing(1);

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
