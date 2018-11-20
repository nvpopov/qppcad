#include <qppcad/app_state_event_disp.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


void app_state_event_disp_t::workspaces_changed() {

  app_state_t* astate = app_state_t::get_inst();
  astate->log("app_state_event_disp_t::workspaces_changed()");
  emit(workspaces_changed_signal());
}

app_state_event_disp_t::app_state_event_disp_t(QObject *parent) : QObject (parent) {

}
