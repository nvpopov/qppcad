#include <qppcad/ws_item/geom_view/geom_view_extended_editor.hpp>

using namespace qpp;
using namespace qpp::cad;

geom_view_extended_editor_t::geom_view_extended_editor_t() {

  ext_editor_gv_label = new QLabel(tr("geom_view_extended_editor_t"));
  main_lt = new QVBoxLayout;
  setLayout(main_lt);
  main_lt->addWidget(ext_editor_gv_label);

}

void geom_view_extended_editor_t::bind_to_item(ws_item_t *_binding_item) {

}

void geom_view_extended_editor_t::update_from_ws_item() {

}

void geom_view_extended_editor_t::unbind_item() {

}
