#include <qppcad/ws_item/py_note_book/py_note_book_extended_editor.hpp>

using namespace qpp;
using namespace qpp::cad;

py_note_book_extended_editor_t::py_note_book_extended_editor_t() {

  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(0,0,10,10);
  setLayout(main_lt);

}

void py_note_book_extended_editor_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_extended_editor_t::bind_to_item(_binding_item);

  if (!_binding_item) {
      m_binded_nx = nullptr;
      return;
    }

  auto as_nx = _binding_item->cast_as<py_note_book_t>();
  if (!as_nx) {
      m_binded_nx = nullptr;
      return;
    }

  m_binded_nx = as_nx;

}

void py_note_book_extended_editor_t::update_from_ws_item() {

  ws_item_extended_editor_t::update_from_ws_item();

}

void py_note_book_extended_editor_t::unbind_item() {

  ws_item_extended_editor_t::unbind_item();
  m_binded_nx = nullptr;

}

bool py_note_book_extended_editor_t::can_be_binded_to(ws_item_t *item) {
  return true;
}

QString py_note_book_extended_editor_t::header_name_hint() {
  return "py_note_book EDITOR";
}
