#include <qppcad/ws_item/ws_item_extended_editor.hpp>

using namespace qpp;
using namespace qpp::cad;

extended_editor_geom_policy_e ws_item_extended_editor_t::get_geom_policy() {

  return extended_editor_geom_policy_e::always_min;

}

int ws_item_extended_editor_t::get_minimum_width() {

  return 300;

}

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
