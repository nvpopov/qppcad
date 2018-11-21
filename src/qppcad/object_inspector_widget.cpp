#include <qppcad/object_inspector_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

object_inspector_widget_t::object_inspector_widget_t() {

  obj_insp_layout = new QHBoxLayout;
  setLayout(obj_insp_layout);

  ws_items_gb = new QGroupBox;
  ws_items_gb->setTitle(tr("Workspace items:"));
  //ws_items_gb->setMaximumHeight(340);
  ws_items_gb_layout = new QHBoxLayout;
  ws_items_gb->setStyleSheet("QGroupBox::title {"
                             "subcontrol-origin: margin;"
                             "subcontrol-position: top center;"
                             "padding-left: 10px;"
                             "padding-right: 10px;"
                             "padding-top: 0px;"
                             "margin-bottom:25px;}");
  ws_items_gb->setFlat(false);
  ws_items_gb->setLayout(ws_items_gb_layout);
  ws_items_list = new QListWidget;
  ws_items_list->setMaximumHeight(300);


  ws_items_gb_layout->layout()->addWidget(ws_items_list);

  obj_insp_layout->addWidget(ws_items_gb);
  obj_insp_layout->setAlignment(ws_items_gb, Qt::AlignTop);
  obj_insp_layout->setContentsMargins(0,0,15,15);

}
