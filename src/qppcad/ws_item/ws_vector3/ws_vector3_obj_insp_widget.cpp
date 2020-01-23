#include <qppcad/ws_item/ws_vector3/ws_vector3_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_vector3_obj_insp_widget_t::ws_vector3_obj_insp_widget_t() {

  m_tab_general->tab_inner_widget_lt->addStretch(1);

}

void ws_vector3_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
  
  if (_binding_item && _binding_item->get_type() == ws_vector3_t::get_type_static()) {

      b_v3 = _binding_item->cast_as<ws_vector3_t>();

    } else {

      b_v3 = nullptr;

    }

}

void ws_vector3_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void ws_vector3_obj_insp_widget_t::unbind_item() {

}
