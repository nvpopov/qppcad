#include <qppcad/ws_item/volume_view/volume_view_obj_insp_widget.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

volume_view_obj_insp_widget_t::volume_view_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  m_general_vol_type = new QLabel(tr("Molecular orbitals"));
//  m_vol_isovalue = new qbinded_float_spinbox_t;
//  m_vol_isovalue->set_min_max_step(-0.8, 0.8, 0.0001, 4);
//  m_vol_isovalue->m_updated_externally_event = true;
//  m_vol_isovalue->m_upd_flag = ws_item_updf_regenerate_content;

  m_vol_gen_settings = new qbinded_bool_named_vector_t({"Always", "Transparent"});

  m_vol_color_pos = new qbinded_color3_input_t;
  m_vol_color_neg = new qbinded_color3_input_t;
  m_vol_color_vol = new qbinded_color3_input_t;

  //m_vol_transparent = new qbinded_checkbox_t;
//  m_vol_alpha = new qbinded_float_spinbox_t;
//  m_vol_alpha->set_min_max_step(0.1, 1.0, 0.05, 2);
  //m_vol_render_permanent = new qbinded_checkbox_t;

  m_vol_isoval_alpha = new qbinded_float_named_vector_t({"Isov.", "Alpha"});
  m_vol_isoval_alpha->set_min_max_step_ex({-0.8, 0}, {0.8, 1.0}, {0.001, 0.005},
                                          {3, std::nullopt});
  m_vol_isoval_alpha->m_updated_externally_event = {true, false};
  m_vol_isoval_alpha->m_upd_flag = {ws_item_updf_regenerate_content, ws_item_updf_generic};
  m_gb_volume_detail = new qspoiler_widget_t(tr("Volume Details"));
  m_gb_volume_detail_lt = new QFormLayout;
  m_gb_volume_detail->add_content_layout(m_gb_volume_detail_lt);

  m_gb_volume_clr = new qspoiler_widget_t(tr("Volume colors"));
  m_gb_volume_clr_lt = new QFormLayout;
  m_gb_volume_clr->add_content_layout(m_gb_volume_clr_lt);

  m_cb_current_vol = new QComboBox;
  m_cb_current_vol->setFixedWidth(astate->size_guide.obj_insp_combo_max_w());
  m_cb_current_vol->setEditable(false);

  m_gb_volume_detail_lt->addRow(tr("Current"), m_cb_current_vol);
  m_gb_volume_detail_lt->addRow(tr("Appearance"), m_vol_gen_settings);
  m_gb_volume_detail_lt->addRow(tr("Type"), m_general_vol_type);
  m_gb_volume_detail_lt->addRow("", m_vol_isoval_alpha);
  init_form_lt(m_gb_volume_detail_lt);

  m_gb_volume_clr_lt->addRow(tr("Positive"), m_vol_color_pos);
  m_gb_volume_clr_lt->addRow(tr("Negative"), m_vol_color_neg);
  m_gb_volume_clr_lt->addRow(tr("Density"), m_vol_color_vol);
  init_form_lt(m_gb_volume_clr_lt);

  m_tab_general->tab_inner_widget_lt->addWidget(m_gb_volume_detail);
  m_tab_general->tab_inner_widget_lt->addWidget(m_gb_volume_clr);
  m_tab_general->tab_inner_widget_lt->addStretch();

  connect(m_cb_current_vol,
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

      m_cb_current_vol->blockSignals(true);
      m_cb_current_vol->clear();

      for (size_t i = 0; i < b_vol->m_volumes.size(); i++) {
          m_cb_current_vol->addItem(QString("[%1]%2")
                                     .arg(i)
                                     .arg(QString::fromStdString(
                                            b_vol->m_volumes[i]->m_volume.m_name)
                                          )
                                     );
        }

      m_cb_current_vol->setCurrentIndex(b_vol->m_current_vol);
      m_cb_current_vol->blockSignals(false);

    }

}

void volume_view_obj_insp_widget_t::unbind_item() {

  //m_vol_isovalue->unbind_value();
  m_vol_isoval_alpha->unbind_value();
  m_vol_color_pos->unbind_value();
  m_vol_color_neg->unbind_value();
  m_vol_color_vol->unbind_value();
  //m_vol_alpha->unbind_value();
  m_vol_gen_settings->unbind_value();

}

void volume_view_obj_insp_widget_t::update_binded_volume_controls() {

  if (b_vol && b_vol->m_current_vol < b_vol->m_volumes.size()) {

      auto cvi = b_vol->m_current_vol;
      auto &cv = b_vol->m_volumes[cvi];

      m_general_vol_type->setText(
            cv->m_volume_type != ws_volume_t::volume_mo ?
            tr("Density") : tr("Molecular orbitals"));

      m_vol_isoval_alpha->bind_value({&cv->m_isolevel, &cv->m_alpha}, b_vol);
      m_vol_color_pos->bind_value(&cv->m_color_pos);
      m_vol_color_neg->bind_value(&cv->m_color_neg);
      m_vol_color_vol->bind_value(&cv->m_color_vol);
      m_vol_gen_settings->bind_value({&cv->m_render_permanent, &cv->m_transparent_volume});
     // m_vol_alpha->bind_value(&cv->m_alpha);

    }

}

void volume_view_obj_insp_widget_t::cur_volume_index_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();

  if (b_vol) {
      b_vol->m_current_vol = index;
      update_binded_volume_controls();
    }

  astate->make_viewport_dirty();

}
