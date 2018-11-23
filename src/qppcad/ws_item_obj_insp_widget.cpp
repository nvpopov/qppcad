#include <qppcad/ws_item_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_item_obj_insp_widget_t::ws_item_obj_insp_widget_t() {

  main_layout = new QVBoxLayout;
  setLayout(main_layout);
  tab_general = new QWidget;
  tab_general->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  addTab(tab_general, "General");
  //setUsesScrollButtons(false);
  //setElideMode(Qt::ElideLeft);
}
