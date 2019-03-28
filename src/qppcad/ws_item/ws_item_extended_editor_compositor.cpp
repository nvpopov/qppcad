#include <qppcad/ws_item/ws_item_extended_editor_compositor.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


ws_item_extended_editor_compositor_t::ws_item_extended_editor_compositor_t(QWidget *parent) :
  qembed_window_t (parent) {

  app_state_t *astate = app_state_t::get_inst();

  setMinimumWidth(300);

  ext_edt_plch = new QWidget;
  main_lt->addWidget(ext_edt_plch);

  connect(astate->astate_evd,
          &app_state_event_disp_t::extended_editor_open_requested_signal,
          this,
          &ws_item_extended_editor_compositor_t::open_requested);

}

void ws_item_extended_editor_compositor_t::open_requested() {

  show();

}
