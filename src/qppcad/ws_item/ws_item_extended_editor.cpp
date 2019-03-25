#include <qppcad/ws_item/ws_item_extended_editor.hpp>

using namespace qpp;
using namespace qpp::cad;

void ws_item_extended_editor_t::bind_to_item(ws_item_t *_binding_item) {

  m_binded_item = _binding_item;

}

void ws_item_extended_editor_t::unbind_item() {

  m_binded_item = nullptr;

}

void ws_item_extended_editor_t::update_from_ws_item() {

}

ws_item_extended_editor_t::ws_item_extended_editor_t() : QWidget() {

}
