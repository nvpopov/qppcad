#include <qppcad/ws_cube_primitive/ws_cube_primitive_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_cube_primitive_obj_insp_widget_t::ws_cube_primitive_obj_insp_widget_t() {

  tab_general->tab_inner_widget_layout->addStretch();
}

void ws_cube_primitive_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void ws_cube_primitive_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void ws_cube_primitive_obj_insp_widget_t::unbind_item() {

}
