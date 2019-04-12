#include <qppcad/ws_item/arrow_primitive/arrow_primitive_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

arrow_primitive_obj_insp_widget_t::arrow_primitive_obj_insp_widget_t() {

  gb_arrow_settings = new qspoiler_widget_t(tr("Arrow settings"));
  gb_arrow_settings_lt = new QFormLayout;
  gb_arrow_settings->add_content_layout(gb_arrow_settings_lt);

  ap_target = new qbinded_float3_input_t;
  ap_target->set_min_max_step(-10000, 10000, 0.01);
  ap_target->m_updated_internally_event = true;

  ap_unf_arrow_scale = new qbinded_float_spinbox_t;
  ap_unf_arrow_scale->set_min_max_step(0.01, 20, 0.05);
  ap_unf_arrow_scale->m_updated_internally_event = true;

  ap_unf_arrow_cap_len = new qbinded_float_spinbox_t;
  ap_unf_arrow_cap_len->set_min_max_step(0.01, 20, 0.05);
  ap_unf_arrow_cap_len->m_updated_internally_event = true;

  ap_unf_arrow_cap_scale = new qbinded_float_spinbox_t;
  ap_unf_arrow_cap_scale->set_min_max_step(0.01, 20, 0.05);
  ap_unf_arrow_cap_scale->m_updated_internally_event = true;

  ap_arrow_color = new qbinded_color3_input_t;

  gb_arrow_settings_lt->addRow(tr("Target[GLOBAL]"), ap_target);
  gb_arrow_settings_lt->addRow(tr("Arrow scale"), ap_unf_arrow_scale);
  gb_arrow_settings_lt->addRow(tr("Cap lenght"), ap_unf_arrow_cap_len);
  gb_arrow_settings_lt->addRow(tr("Cap scale"), ap_unf_arrow_cap_scale);
  gb_arrow_settings_lt->addRow(tr("Arrow color"), ap_arrow_color);

  init_form_lt(gb_arrow_settings_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_arrow_settings);
  tab_general->tab_inner_widget_lt->addStretch(1);
  ws_item_pos->m_updated_internally_event = true;

}

void arrow_primitive_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (_binding_item) {
      auto as_ap = _binding_item->cast_as<arrow_primitive_t>();
      if (as_ap) b_ap = as_ap;
      else as_ap = nullptr;
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  if (b_ap) {
      ap_target->bind_value(&b_ap->m_arrow_to, b_ap);
      ap_unf_arrow_scale->bind_value(&b_ap->m_unf_arrow_scale, b_ap);
      ap_unf_arrow_cap_len->bind_value(&b_ap->m_unf_arrow_cap_len, b_ap);
      ap_unf_arrow_cap_scale->bind_value(&b_ap->m_unf_arrow_cap_scale, b_ap);
      ap_arrow_color->bind_value(&b_ap->m_color);
    }

}

void arrow_primitive_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void arrow_primitive_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();
  ap_target->unbind_value();

}
