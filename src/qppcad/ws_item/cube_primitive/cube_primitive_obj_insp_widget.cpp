#include <qppcad/ws_item/cube_primitive/cube_primitive_obj_insp_widget.hpp>
#include <qppcad/ws_item/cube_primitive/cube_primitive.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

cube_primitive_obj_insp_widget_t::cube_primitive_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  tg_cube_params = new QGroupBox(tr("Cube parameters"));
  tg_cube_params_lt = new QFormLayout;

  pre_init_gb(tg_cube_params, tg_cube_params_lt);

  cube_param_color = new qbinded_color3_input;

  cube_param_scale = new qbinded_float3_input;
  cube_param_scale->set_min_max_step(0.05f, 20.0f, 0.05f);

  cube_render_mode = new qbinded_combobox;
  cube_render_mode->addItem(tr("Solid"));
  cube_render_mode->addItem(tr("Lines"));

  tg_cube_params_lt->addRow(tr("Color"), cube_param_color);
  tg_cube_params_lt->addRow(tr("Size[%1]").arg(astate->m_spatial_suffix), cube_param_scale);
  tg_cube_params_lt->addRow(tr("Render style"), cube_render_mode);

  post_init_gb(tg_cube_params, tg_cube_params_lt);

  tab_general->tab_inner_widget_lt->addWidget(tg_cube_params);

  tab_general->tab_inner_widget_lt->addStretch(1);

}

void cube_primitive_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  if (_binding_item && _binding_item->get_type() == cube_primitive_t::get_type_static()) {
      b_cp = _binding_item->cast_as<cube_primitive_t>();
      cube_param_scale->bind_value(&b_cp->m_scale);
      cube_param_color->bind_value(&b_cp->m_color);
      cube_render_mode->bind_value(reinterpret_cast<int*>(&b_cp->m_render_mode));
    } else {
      b_cp = nullptr;
    }

}

void cube_primitive_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void cube_primitive_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();

  cube_param_scale->unbind_value();
  cube_param_color->unbind_value();

  b_cp = nullptr;

}
