#include <qppcad/ws_item/arrow_primitive/arrow_primitive_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

arrow_primitive_obj_insp_widget_t::arrow_primitive_obj_insp_widget_t() {

  tab_general->tab_inner_widget_layout->addStretch(1);
  ws_item_pos->m_updated_internally_event = true;

}

void arrow_primitive_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
}

void arrow_primitive_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void arrow_primitive_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();

}
