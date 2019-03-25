#include <qppcad/ws_item/node_book/node_book_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

node_book_obj_insp_widget_t::node_book_obj_insp_widget_t() {

  tab_general->tab_inner_widget_lt->addStretch();

}

void node_book_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void node_book_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void node_book_obj_insp_widget_t::unbind_item() {

}
