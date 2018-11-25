#include <qppcad/ws_atoms_list_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_atoms_list_obj_insp_widget_t::ws_atoms_list_obj_insp_widget_t()
  : ws_item_obj_insp_widget_t() {

  tab_display = define_tab(tr("Display"));
  tab_animation = define_tab(tr("Animation"));
  tab_measurement = define_tab(tr("Measurement"));

}
