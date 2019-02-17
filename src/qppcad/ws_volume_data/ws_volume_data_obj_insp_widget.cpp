#include <qppcad/ws_volume_data/ws_volume_data_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_volume_data_obj_insp_widget_t::ws_volume_data_obj_insp_widget_t() {

  general_volume_type = new QLabel(tr("Molecular orbitals"));
  vol_isovalue = new qbinded_float_spinbox;
  vol_isovalue->set_min_max_step(-0.03, 0.03, 0.001, 3);
  vol_isovalue->m_updated_internally_event = true;

  vol_color_pos = new qbinded_color3_input;
  vol_color_neg = new qbinded_color3_input;
  vol_color_vol = new qbinded_color3_input;
  vol_transparent = new qbinded_checkbox;
  vol_alpha = new qbinded_float_spinbox;
  vol_alpha->set_min_max_step(0.1, 1.0, 0.05, 2);

  gb_volume_detail = new QGroupBox(tr("Volume details"));
  gb_volume_detail_lt = new QFormLayout;

  cb_current_volume = new QComboBox;
  cb_current_volume->setEditable(false);
  cb_current_volume->setMaximumWidth(def_control_width);

  pre_init_group_box(gb_volume_detail, gb_volume_detail_lt);
  gb_volume_detail_lt->addRow(tr("Current volume :"), cb_current_volume);
  post_init_group_box(gb_volume_detail, gb_volume_detail_lt);


//  tg_form_layout->addRow(tr("Type"), general_volume_type);
//  tg_form_layout->addRow(tr("Isolevel"), vol_isovalue);
//  tg_form_layout->addRow(tr("Transparent"), vol_transparent);
//  tg_form_layout->addRow(tr("Alpha"), vol_alpha);
//  tg_form_layout->addRow(tr("Color positive"), vol_color_pos);
//  tg_form_layout->addRow(tr("Color negative"), vol_color_neg);
//  tg_form_layout->addRow(tr("Color density"), vol_color_vol);

  tab_general->tab_inner_widget_layout->addWidget(gb_volume_detail);
  tab_general->tab_inner_widget_layout->addStretch();

}

void ws_volume_data_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  auto _tmp = dynamic_cast<ws_volume_data_t*>(_binding_item);

  if (_tmp) {
      b_vol = _tmp;
//      vol_isovalue->bind_value(&b_vol->m_isolevel, b_vol);
//      vol_color_pos->bind_value(&b_vol->m_color_pos);
//      vol_color_neg->bind_value(&b_vol->m_color_neg);
//      vol_color_vol->bind_value(&b_vol->m_color_vol);
//      vol_alpha->bind_value(&b_vol->m_alpha);
//      vol_transparent->bind_value(&b_vol->m_transparent_volume);
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void ws_volume_data_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_vol) {
//      if (b_vol->m_volume.m_has_negative_values) general_volume_type->setText(tr("MO"));
//      else general_volume_type->setText(tr("DENSITY"));
    }
}

void ws_volume_data_obj_insp_widget_t::unbind_item() {

  vol_isovalue->unbind_value();
  vol_color_pos->unbind_value();
  vol_color_neg->unbind_value();
  vol_color_vol->unbind_value();
  vol_alpha->unbind_value();
  vol_transparent->unbind_value();
}
