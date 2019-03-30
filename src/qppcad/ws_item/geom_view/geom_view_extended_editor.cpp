#include <qppcad/ws_item/geom_view/geom_view_extended_editor.hpp>

using namespace qpp;
using namespace qpp::cad;

geom_view_extended_editor_t::geom_view_extended_editor_t() {

  ext_editor_gv_label = new QLabel(tr("geom_view_extended_editor_t"));
  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(0,0,10,0);
  setLayout(main_lt);

//  xgeom_tv = new QTableView(this);
//  xgeom_tv->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//  xgeom_tv->verticalHeader()->hide();

//  main_lt->addWidget(xgeom_tv);
  main_lt->addStretch(1);

}

void geom_view_extended_editor_t::bind_to_item(ws_item_t *_binding_item) {

}

void geom_view_extended_editor_t::update_from_ws_item() {

}

void geom_view_extended_editor_t::unbind_item() {

}
