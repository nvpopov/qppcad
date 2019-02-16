#include <qppcad/ws_arrow_primitive/ws_arrow_primitive_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_arrow_primitive_obj_insp_widget_t::ws_arrow_primitive_obj_insp_widget_t() {
  tab_general->tab_inner_widget_layout->addStretch(1);
  ws_item_pos->m_updated_internally_event = true;
}

void ws_arrow_primitive_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
}

void ws_arrow_primitive_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void ws_arrow_primitive_obj_insp_widget_t::unbind_item() {

}
