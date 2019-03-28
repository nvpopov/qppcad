#include <qppcad/ws_item/ws_item_extended_editor_compositor.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


ws_item_extended_editor_compositor_t::ws_item_extended_editor_compositor_t() {

  app_state_t *astate = app_state_t::get_inst();
  setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  setPalette(astate->m_bg_light_pal);

  setMinimumWidth(300);

  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(2, 2, 1, 1);
  top_btn_lt = new QHBoxLayout;

  main_lt->addLayout(top_btn_lt);

  top_btn_close = new QPushButton(tr("X"));
  top_btn_lt->addWidget(top_btn_close);
  top_btn_lt->addStretch(0);
  top_btn_close->setFixedSize(QSize(astate->size_guide.tool_panel_ctrl_h(),
                                    astate->size_guide.tool_panel_ctrl_h()));

  ext_edt_plch = new QWidget;

  main_lt->addWidget(ext_edt_plch);
  setLayout(main_lt);

  connect(astate->astate_evd,
          &app_state_event_disp_t::extended_editor_open_requested_signal,
          this,
          &ws_item_extended_editor_compositor_t::open_requested);

}

void ws_item_extended_editor_compositor_t::open_requested() {

  show();

}
