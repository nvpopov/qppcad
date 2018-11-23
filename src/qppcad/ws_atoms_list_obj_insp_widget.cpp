#include <qppcad/ws_atoms_list_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_atoms_list_obj_insp_widget_t::ws_atoms_list_obj_insp_widget_t()
  : ws_item_obj_insp_widget_t() {

  tab_display = new QWidget;
  tab_animation = new QWidget;
  tab_measurement = new QWidget;
  addTab(tab_display, "Display");
  addTab(tab_animation, "Animation");
  addTab(tab_measurement, "Measurement");

}
