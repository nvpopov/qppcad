#include <qppcad/ws_item_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;



ws_item_obj_insp_widget_t::ws_item_obj_insp_widget_t() {

  main_layout = new QVBoxLayout;
  //setMinimumHeight(500);

  setLayout(main_layout);
  tab_general = new QWidget;
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  addTab(tab_general, "General");
  tab_general_layout = new QVBoxLayout;
  tab_general_scroll_area = new QScrollArea;
//  /
  tab_general_layout->addWidget(tab_general_scroll_area);
  tab_general->setLayout(tab_general_layout);
  //setUsesScrollButtons(false);
  //setElideMode(Qt::ElideLeft);
}
