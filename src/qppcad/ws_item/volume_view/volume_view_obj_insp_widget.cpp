#include <qppcad/ws_item/volume_view/volume_view_obj_insp_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

volume_view_obj_insp_widget_t::volume_view_obj_insp_widget_t() {

  general_volume_type = new QLabel(tr("Molecular orbitals"));
  vol_isovalue = new qbinded_float_spinbox_t;
  vol_isovalue->set_min_max_step(-0.03, 0.03, 0.001, 3);
  vol_isovalue->m_updated_externally_event = true;

  vol_color_pos = new qbinded_color3_input_t;
  vol_color_neg = new qbinded_color3_input_t;
  vol_color_vol = new qbinded_color3_input_t;
  vol_transparent = new qbinded_checkbox_t;
  vol_alpha = new qbinded_float_spinbox_t;
  vol_alpha->set_min_max_step(0.1, 1.0, 0.05, 2);
  vol_render_permanent = new qbinded_checkbox_t;

  gb_volume_detail = new qspoiler_widget_t(tr("Volume details"));
  gb_volume_detail_lt = new QFormLayout;
  gb_volume_detail->add_content_layout(gb_volume_detail_lt);

  cb_current_volume = new QComboBox;
  cb_current_volume->setEditable(false);

  //cb_current_volume->setMaximumWidth(def_control_width);

  gb_volume_detail_lt->addRow(tr("Current volume"), cb_current_volume);
  gb_volume_detail_lt->addRow(tr("Permanent"), vol_render_permanent);
  gb_volume_detail_lt->addRow(tr("Type"), general_volume_type);
  gb_volume_detail_lt->addRow(tr("Isolevel"), vol_isovalue);
  gb_volume_detail_lt->addRow(tr("Transparent"), vol_transparent);
  gb_volume_detail_lt->addRow(tr("Alpha"), vol_alpha);
  gb_volume_detail_lt->addRow(tr("Color positive"), vol_color_pos);
  gb_volume_detail_lt->addRow(tr("Color negative"), vol_color_neg);
  gb_volume_detail_lt->addRow(tr("Color density"), vol_color_vol);
  init_form_lt(gb_volume_detail_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_volume_detail);
  tab_general->tab_inner_widget_lt->addStretch();

  connect(cb_current_volume,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &volume_view_obj_insp_widget_t::cur_volume_index_changed);

}

void volume_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  auto _tmp = _binding_item->cast_as<volume_view_t>();

  if (_tmp) {
      b_vol = _tmp;
      update_binded_volume_controls();
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void volume_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_vol) {
      //      if (b_vol->m_volume.m_has_negative_values) general_volume_type->setText(tr("MO"));
      //      else general_volume_type->setText(tr("DENSITY"));
      cb_current_volume->blockSignals(true);
      cb_current_volume->clear();
      for (size_t i = 0; i < b_vol->m_volumes.size(); i++) {
          cb_current_volume->addItem(
                QString::fromStdString(b_vol->m_volumes[i].m_volume.m_name));
        }
      cb_current_volume->setCurrentIndex(b_vol->m_current_volume);
      cb_current_volume->blockSignals(false);
    }
}

void volume_view_obj_insp_widget_t::unbind_item() {

  vol_isovalue->unbind_value();
  vol_color_pos->unbind_value();
  vol_color_neg->unbind_value();
  vol_color_vol->unbind_value();
  vol_alpha->unbind_value();
  vol_transparent->unbind_value();
  vol_render_permanent->unbind_value();
}

void volume_view_obj_insp_widget_t::update_binded_volume_controls() {

  if (b_vol && b_vol->m_current_volume < b_vol->m_volumes.size()) {

      auto cvi = b_vol->m_current_volume;
      auto &cv = b_vol->m_volumes[cvi];

      vol_isovalue->bind_value(&cv.m_isolevel, b_vol);
      vol_color_pos->bind_value(&cv.m_color_pos);
      vol_color_neg->bind_value(&cv.m_color_neg);
      vol_color_vol->bind_value(&cv.m_color_vol);
      vol_render_permanent->bind_value(&cv.m_render_permanent);
      vol_alpha->bind_value(&cv.m_alpha);
      vol_transparent->bind_value(&cv.m_transparent_volume);

    }
}

void volume_view_obj_insp_widget_t::cur_volume_index_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();

  if (b_vol) {
      b_vol->m_current_volume = index;
      update_binded_volume_controls();
    }

  astate->make_viewport_dirty();
}
