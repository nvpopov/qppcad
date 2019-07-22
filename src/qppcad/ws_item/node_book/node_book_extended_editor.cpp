#include <qppcad/ws_item/node_book/node_book_extended_editor.hpp>

using namespace qpp;
using namespace qpp::cad;

node_book_extended_editor_t::node_book_extended_editor_t() {

  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(0,0,10,10);
  setLayout(main_lt);

  m_gr_view = new node_book_graphics_view_t;
  main_lt->addWidget(m_gr_view);

}

void node_book_extended_editor_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_extended_editor_t::bind_to_item(_binding_item);

  if (!_binding_item) {
      m_binded_nb = nullptr;
      m_gr_view->setScene(nullptr);
      return;
    }

  auto as_nb = _binding_item->cast_as<node_book_t>();
  if (!as_nb) {
      m_binded_nb = nullptr;
      m_gr_view->setScene(nullptr);
      return;
    }

  m_binded_nb = as_nb;
  m_gr_view->setScene(m_binded_nb->m_scene);

  if (m_binded_first_time) {
      m_binded_first_time = false;
    } else {
      m_gr_view->updateSceneRect(m_stored_scene_rect);
    }

}

void node_book_extended_editor_t::update_from_ws_item() {

  ws_item_extended_editor_t::update_from_ws_item();
  m_gr_view->update();

}

void node_book_extended_editor_t::unbind_item() {

  ws_item_extended_editor_t::unbind_item();

  if (m_gr_view->scene() && m_binded_nb) {
      m_stored_scene_rect = m_gr_view->get_visible_rect();
    }

  m_binded_nb = nullptr;
  m_gr_view->setScene(nullptr);

}

bool node_book_extended_editor_t::can_be_binded_to(ws_item_t *item) {
  return true;
}

QString node_book_extended_editor_t::header_name_hint() {
  return "NODE BOOK EDITOR";
}
