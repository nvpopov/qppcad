#include <qppcad/ws_point_sym_group/ws_point_sym_group_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_point_sym_group_obj_insp_widget_t::ws_point_sym_group_obj_insp_widget_t() {

}

void ws_point_sym_group_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
  if (_binding_item && _binding_item->get_type() == ws_point_sym_group_t::get_type_static()) {
      b_pg = _binding_item->cast_as<ws_point_sym_group_t>();
    }
}

void ws_point_sym_group_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void ws_point_sym_group_obj_insp_widget_t::unbind_item() {

}
