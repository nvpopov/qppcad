#include <qppcad/ui/ws_tabbar.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_tabbar_t::ws_tabbar_t(QWidget *parent) : QTabBar (parent) {

  app_state_t *astate = app_state_t::get_inst();

  setFixedHeight(30);
  setDrawBase(false);
  setExpanding(false);
  update_tabs();
  cur_ws_changed();

  connect(astate->astate_evd,
          &app_state_event_disp_t::wss_changed_signal,
          this,
          &ws_tabbar_t::wss_changed_slot);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_changed_signal,
          this,
          &ws_tabbar_t::cur_ws_changed);

  connect(this,
          &ws_tabbar_t::currentChanged,
          this,
          &ws_tabbar_t::current_changed);

}

void ws_tabbar_t::update_tabs() {

  app_state_t *astate = app_state_t::get_inst();

  blockSignals(true);

  while(count() != 0) removeTab(0);

  for (size_t i = 0; i < astate->ws_mgr->m_ws.size(); i++)
    addTab(QString::fromStdString(astate->ws_mgr->m_ws[i]->m_ws_name));

  blockSignals(false);

}

void ws_tabbar_t::cur_ws_changed() {

  //update_tabs();

  app_state_t *astate = app_state_t::get_inst();

  blockSignals(true);
  auto cur_ws_idx = astate->ws_mgr->get_cur_id();
  if (cur_ws_idx) setCurrentIndex(*cur_ws_idx);
  else setCurrentIndex(-1);
  blockSignals(false);

}

void ws_tabbar_t::wss_changed_slot() {

  app_state_t *astate = app_state_t::get_inst();
  astate->tlog("@@@ DEBUG: ws_tabbar_t::wss_changed_slot()");
  update_tabs();

  blockSignals(true);
  auto cur_ws_idx = astate->ws_mgr->get_cur_id();
  if (cur_ws_idx) setCurrentIndex(*cur_ws_idx);
  else setCurrentIndex(-1);
  blockSignals(false);

}

void ws_tabbar_t::current_changed(int current) {

  app_state_t *astate = app_state_t::get_inst();

  if (current >= 0 && current < astate->ws_mgr->m_ws.size())
    astate->ws_mgr->set_cur_id(opt<size_t>(current));

}
