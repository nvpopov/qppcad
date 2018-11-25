#include <qppcad/ws_item_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;



ws_item_tab_widget_t *ws_item_obj_insp_widget_t::define_tab(QString tab_name) {

  ws_item_tab_widget_t *tmp = new ws_item_tab_widget_t;
  tmp->tab_scroll = new QScrollArea;
  tmp->tab_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  tmp->tab_scroll->setWidgetResizable(true);

  tmp->tab_inner_widget = new QWidget(tmp->tab_scroll);
  tmp->tab_scroll->setWidget(tmp->tab_inner_widget);

  tmp->tab_inner_widget_layout = new QVBoxLayout;
  tmp->tab_inner_widget_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
  tmp->tab_inner_widget->setLayout(tmp->tab_inner_widget_layout);

  addTab(tmp->tab_scroll, tab_name);

  return tmp;
}

void ws_item_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {
  m_binded_item = _binding_item;
}

void ws_item_obj_insp_widget_t::unbind_item() {
  m_binded_item = nullptr;
}

ws_item_obj_insp_widget_t::ws_item_obj_insp_widget_t() {


  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  tab_general = define_tab(tr("General"));

//  tab_general_child = new QWidget;

//  tab_general_child->setLayout(new QVBoxLayout);
//  tab_general_layout->addWidget(tab_general_child);

//  for(int i = 0; i < 27; i++)
//    tab_general_child->layout()->addWidget(new QPushButton("tre"));
//  //setUsesScrollButtons(false);
//  //setElideMode(Qt::ElideLeft);
}
