#include <qppcad/ws_comp_chem_data_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;


ws_comp_chem_data_obj_insp_widget_t::ws_comp_chem_data_obj_insp_widget_t()
  : ws_item_obj_insp_widget_t() {

  tab_geo_opt = define_tab(tr("Geometry opt."));
  tab_vibs = define_tab(tr("Vibrational analysis"));
}
