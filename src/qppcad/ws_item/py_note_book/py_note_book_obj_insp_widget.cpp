#include <qppcad/ws_item/py_note_book/py_note_book_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

py_note_book_obj_insp_widget_t::py_note_book_obj_insp_widget_t() {

  tab_general->tab_inner_widget_lt->addStretch();

}

void py_note_book_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void py_note_book_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void py_note_book_obj_insp_widget_t::unbind_item() {

}
