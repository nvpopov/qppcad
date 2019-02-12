#include <qppcad/ws_cube_primitive/ws_cube_primitive_obj_insp_widget.hpp>
#include <qppcad/ws_cube_primitive/ws_cube_primitive.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_cube_primitive_obj_insp_widget_t::ws_cube_primitive_obj_insp_widget_t() {

  tg_cube_params = new QGroupBox(tr("Cube parameters"));
  tg_cube_params_lt = new QFormLayout;

  pre_init_group_box(tg_cube_params, tg_cube_params_lt);
  cube_param_color = new qbinded_color3_input;
  cube_param_scale = new qbinded_float3_input;
  cube_param_scale->set_min_max_step(0.05f, 20.0f, 0.05f);
  cube_param_scale->set_default_suffix();
  tg_cube_params_lt->addRow(tr("Color"), cube_param_color);
  tg_cube_params_lt->addRow(tr("Size"), cube_param_scale);
  post_init_group_box(tg_cube_params, tg_cube_params_lt);
  tab_general->tab_inner_widget_layout->addWidget(tg_cube_params);
  tab_general->tab_inner_widget_layout->addStretch();
}

void ws_cube_primitive_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
  if (_binding_item && _binding_item->get_type() == ws_cube_primitive_t::get_type_static()) {
      b_cp = _binding_item->cast_as<ws_cube_primitive_t>();
      cube_param_scale->bind_value(&b_cp->m_scale);
      cube_param_color->bind_value(&b_cp->m_color);
    } else {
      b_cp = nullptr;
    }
}

void ws_cube_primitive_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void ws_cube_primitive_obj_insp_widget_t::unbind_item() {
  cube_param_scale->unbind_value();
  cube_param_color->unbind_value();
}
