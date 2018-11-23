#include <qppcad/ws_comp_chem_data_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;


ws_comp_chem_data_obj_insp_widget_t::ws_comp_chem_data_obj_insp_widget_t()
  : ws_item_obj_insp_widget_t() {
  addTab(new QWidget, "ws_comp_chem_data");
  addTab(new QWidget, "ws_comp_chem_data1");
  addTab(new QWidget, "ws_comp_chem_data2");
  addTab(new QWidget, "ws_comp_chem_data3");
  addTab(new QWidget, "ws_comp_chem_data4");
}
