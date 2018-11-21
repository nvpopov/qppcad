#include <qppcad/app_state_event_disp.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


void app_state_event_disp_t::workspaces_changed() {
  app_state_t* astate = app_state_t::get_inst();
  astate->log("DEBUG: app_state_event_disp_t::workspaces_changed()");
  emit(workspaces_changed_signal());
}

void app_state_event_disp_t::current_workspace_changed() {
  app_state_t* astate = app_state_t::get_inst();
  astate->log("DEBUG: app_state_event_disp_t::current_workspace_changed()");
  emit(current_workspace_changed_signal());
}

void app_state_event_disp_t::current_workspace_selected_item_changed() {
  app_state_t* astate = app_state_t::get_inst();
  astate->log("DEBUG: app_state_event_disp_t::current_workspace_selected_item_changed()");
  emit(current_workspace_selected_item_changed_signal());
}

app_state_event_disp_t::app_state_event_disp_t(QObject *parent) : QObject (parent) {

}
