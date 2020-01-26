#include <qppcad/ws_item/ccd_view/ccd_view_obj_insp_widget.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void ccd_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (_binding_item && _binding_item->get_type() == ccd_view_t::get_type_static()) {

      ccd_view_t *dp = _binding_item->cast_as<ccd_view_t>();

      if (dp) {
          b_ccdv = dp;
        }
      else {
          b_ccdv = nullptr;
        }

    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void ccd_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_ccdv) {

      m_gen_info_prg_name->setText(
            QString::fromStdString(qpp::ccdprog2str[b_ccdv->m_ccd->m_comp_chem_program]));

      m_gen_info_run_type->setText(QString::fromStdString(qpp::ccdrt2str[b_ccdv->m_ccd->m_run_t]));

      //tddft tab
      set_tab_enabled(m_tab_tddft, !b_ccdv->m_ccd->m_tddft_trans_rec.empty());
      m_ttd_info_total_trans->setText(QString("%1").arg(b_ccdv->m_ccd->m_tddft_trans_rec.size()));

      switch (b_ccdv->m_ccd->m_run_t) {

        case comp_chem_program_run_e::rt_unknown :
          set_tab_enabled(m_tab_vibs, false);
          set_tab_enabled(m_tab_geo_opt, false);
          break;

        case comp_chem_program_run_e::rt_energy :
          set_tab_enabled(m_tab_vibs, false);
          set_tab_enabled(m_tab_geo_opt, false);
          break;

        case comp_chem_program_run_e::rt_geo_opt :
          set_tab_enabled(m_tab_vibs, false);
          set_tab_enabled(m_tab_geo_opt, true);
          m_tgo_step_copy_charges->bind_value(reinterpret_cast<int*>(&b_ccdv->m_copy_charges),
                                            b_ccdv);
          update_geo_opt();
          break;

        case comp_chem_program_run_e::rt_vib :
          set_tab_enabled(m_tab_vibs, true);
          set_tab_enabled(m_tab_geo_opt, false);
          update_vib_anal();
          break;

        default :
          set_tab_enabled(m_tab_vibs, false);
          set_tab_enabled(m_tab_geo_opt, false);
          break;
        }

    }

}

void ccd_view_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();
  m_tgo_step_copy_charges->unbind_value();
  b_ccdv = nullptr;

}

void ccd_view_obj_insp_widget_t::update_geo_opt() {

//  tgo_steps_ex->blockSignals(true);
//  tgo_steps_ex->clear();
//  tgo_steps_ex->setEnabled(!(!b_ccdv || !b_ccdv->m_ccd || b_ccdv->m_ccd->m_steps.empty()));

  if (b_ccdv) {
      update_geo_opt_step_info_lbl();
      ui_cur_sel_step_item_changed();
    }

//  tgo_steps_ex->blockSignals(false);

}

void ccd_view_obj_insp_widget_t::update_geo_opt_step_info() {

}

void ccd_view_obj_insp_widget_t::update_geo_opt_step_info_lbl() {

  if (b_ccdv) {
      m_tgo_steps_current->setText(
            tr(" %1 / %2")
            .arg(b_ccdv->m_cur_step + 1)
            .arg(b_ccdv->m_ccd->m_steps.size())
            );
    } else {
      m_tgo_steps_current->setText("0 / 0");
    }

}

void ccd_view_obj_insp_widget_t::update_vib_anal() {

  m_vib_modes_list_wdgt->blockSignals(true);
  m_vib_modes_list_wdgt->clear();

  if (b_ccdv) {

      for (size_t i = 0; i < b_ccdv->m_ccd->m_vibs.size(); i++)
        m_vib_modes_list_wdgt->addItem(QString("[%1] Freq. = %2 cm-1, Intens. = %3")
                                        .arg(i)
                                        .arg(b_ccdv->m_ccd->m_vibs[i].m_frequency)
                                        .arg(b_ccdv->m_ccd->m_vibs[i].m_intensity));

      if (b_ccdv->m_cur_vib >= 0 && b_ccdv->m_cur_vib < m_vib_modes_list_wdgt->count())
        m_vib_modes_list_wdgt->setCurrentRow(b_ccdv->m_cur_vib);

    }

  m_vib_modes_list_wdgt->blockSignals(false);

}

ccd_view_obj_insp_widget_t::ccd_view_obj_insp_widget_t() : ws_item_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  m_tab_geo_opt = def_tab(tr("Geometry opt."),
                        "://images/baseline-graphic_eq-24px.svg",
                        "://images/baseline-graphic_eq-24px_d.svg");

  m_tab_vibs = def_tab(tr("Vibrational analysis"),
                     "://images/vib_anal.svg",
                     "://images/vib_anal_d.svg");

  m_tab_tddft = def_tab(tr("TDDFT"),
                      "://images/sync_alt-24px.svg",
                      "://images/sync_alt-24px_d.svg");

//  ws_item_is_visible->hide();
//  ws_item_is_visible_label->hide();

  m_gb_gen_ccd_info = new qspoiler_widget_t(tr("CCD Info"));
  m_gb_gen_ccd_info_lt = new QFormLayout;
  m_gb_gen_ccd_info->add_content_layout(m_gb_gen_ccd_info_lt);

  m_gen_info_prg_name = new QLabel;
  m_gen_info_run_type = new QLabel;

  m_gb_gen_ccd_info_lt->addRow(tr("Program"), m_gen_info_prg_name);
  m_gb_gen_ccd_info_lt->addRow(tr("Run type"), m_gen_info_run_type);
  init_form_lt(m_gb_gen_ccd_info_lt);

  m_tab_general->tab_inner_widget_lt->addWidget(m_gb_gen_ccd_info);
  m_tab_general->tab_inner_widget_lt->addStretch(0);

  m_gb_vib_modes = new qspoiler_widget_t(tr("Vibrational Modes"));
 // gb_vib_modes->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  m_gb_vib_modes_lt = new QVBoxLayout;
  m_gb_vib_modes->add_content_layout(m_gb_vib_modes_lt);

  m_vib_modes_list_wdgt = new QListWidget;
//  vib_modes_list_wdgt->setSizePolicy(
//        QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding)
//        );
  connect(m_vib_modes_list_wdgt,
          &QListWidget::currentRowChanged,
          this,
          &ccd_view_obj_insp_widget_t::vib_anal_current_row_changed);

  m_gb_vib_modes_lt->addWidget(m_vib_modes_list_wdgt);

  m_tab_vibs->tab_inner_widget_lt->addWidget(m_gb_vib_modes);
  m_tab_vibs->tab_inner_widget_lt->addStretch(0);

  //tab geo opt
  m_tgo_sel_step = new qspoiler_widget_t(tr("Geometry Optimization Step"));
  m_tgo_sel_step_lt = new QFormLayout;
  m_tgo_sel_step->add_content_layout(m_tgo_sel_step_lt);

  m_tgo_steps_current = new QLabel;
  m_tgo_acts_lt = new QHBoxLayout;
  m_tgo_acts_lt->setContentsMargins(7, 0, 7, 0);

  m_tgo_steps_fwd = new QPushButton(tr("+"));
  connect(m_tgo_steps_fwd,
          &QPushButton::clicked,
          this,
          &ccd_view_obj_insp_widget_t::ui_step_forward);

  m_tgo_steps_bck = new QPushButton(tr("-"));
  connect(m_tgo_steps_bck,
          &QPushButton::clicked,
          this,
          &ccd_view_obj_insp_widget_t::ui_step_backward);

  m_tgo_steps_begin = new QPushButton(tr("bgn"));
  connect(m_tgo_steps_begin,
          &QPushButton::clicked,
          this,
          &ccd_view_obj_insp_widget_t::ui_step_to_the_begin);

  m_tgo_steps_end = new QPushButton(tr("end"));
  connect(m_tgo_steps_end,
          &QPushButton::clicked,
          this,
          &ccd_view_obj_insp_widget_t::ui_step_to_the_end);

  m_tgo_acts_lt->addWidget(m_tgo_steps_fwd);
  m_tgo_acts_lt->addWidget(m_tgo_steps_bck);
  m_tgo_acts_lt->addWidget(m_tgo_steps_begin);
  m_tgo_acts_lt->addWidget(m_tgo_steps_end);

  m_tgo_sel_step_lt->addRow(tr("Current step"), m_tgo_steps_current);
  m_tgo_sel_step_lt->addRow(tr("Actions"), m_tgo_acts_lt);

  init_form_lt(m_tgo_sel_step_lt);

  m_tgo_step_info = new qspoiler_widget_t(tr("Step Info"));
  m_tgo_step_info_lt = new QFormLayout;
  m_tgo_step_info->add_content_layout(m_tgo_step_info_lt);

  m_tgo_step_info_etotal = new QLabel;
  m_tgo_step_info_dipole_moment = new QLabel;
  m_tgo_step_info_gr_min = new QLabel;
  m_tgo_step_info_gr_max = new QLabel;
  m_tgo_step_info_gr_av = new QLabel;
  m_tgo_step_info_gr_norm = new QLabel;

  m_tgo_step_copy_charges = new qbinded_combobox_t;
  m_tgo_step_copy_charges->m_updated_externally_event = true;
  m_tgo_step_copy_charges->m_upd_flag = ws_item_updf_regenerate_content;
  m_tgo_step_copy_charges->addItem("Do not Copy");
  m_tgo_step_copy_charges->addItem("Mulliken Charges");
  m_tgo_step_copy_charges->addItem("Mulliken Spin Pop.");
  m_tgo_step_copy_charges->addItem("Lowdin Charges");
  m_tgo_step_copy_charges->addItem("Lowdin Spin Pop.");

  m_tgo_step_info_lt->addRow(tr("Energy[a.u]"), m_tgo_step_info_etotal);
  m_tgo_step_info_lt->addRow(tr("Dipole moment"), m_tgo_step_info_dipole_moment);
  m_tgo_step_info_lt->addRow(tr("Gradient min"), m_tgo_step_info_gr_min);
  m_tgo_step_info_lt->addRow(tr("Gradient max"), m_tgo_step_info_gr_max);
  m_tgo_step_info_lt->addRow(tr("Gradient aver."), m_tgo_step_info_gr_av);
  m_tgo_step_info_lt->addRow(tr("Gradient norm."), m_tgo_step_info_gr_norm);
  m_tgo_step_info_lt->addRow(tr("Copy charges"), m_tgo_step_copy_charges);

  init_form_lt(m_tgo_step_info_lt);

  m_ttd_info = new qspoiler_widget_t(tr("TDDFT info"));
  m_ttd_info_lt = new QFormLayout;
  m_ttd_info->add_content_layout(m_ttd_info_lt);
  m_ttd_info_total_trans = new QLabel;
  m_ttd_info_lt->addRow(tr("Total transitions"), m_ttd_info_total_trans);
  init_form_lt(m_ttd_info_lt);

  m_tab_tddft->tab_inner_widget_lt->addWidget(m_ttd_info);
  m_tab_tddft->tab_inner_widget_lt->addStretch(1);

  m_tab_geo_opt->tab_inner_widget_lt->addWidget(m_tgo_sel_step);
  m_tab_geo_opt->tab_inner_widget_lt->addWidget(m_tgo_step_info);
  m_tab_geo_opt->tab_inner_widget_lt->addStretch(1);
  //end tab geo opt

}

void ccd_view_obj_insp_widget_t::resizeEvent(QResizeEvent *event) {

  m_vib_modes_list_wdgt->setFixedHeight(height() * 0.85);

  ws_item_obj_insp_widget_t::resizeEvent(event);

}

void ccd_view_obj_insp_widget_t::ui_cur_sel_step_item_changed() {

  const auto float_digits = 6;
  const auto vector_template = "x = %1\ny = %2\nz = %3";
  const auto gr_norm_template = "min = %1\nmax = %2\naver. = %3";
  const auto null_text = "-";

  m_tgo_step_info_etotal->setText(null_text);
  m_tgo_step_info_dipole_moment->setText(null_text);
  m_tgo_step_info_gr_min->setText(null_text);
  m_tgo_step_info_gr_max->setText(null_text);
  m_tgo_step_info_gr_av->setText(null_text);
  m_tgo_step_info_gr_norm->setText(null_text);

  if (!b_ccdv) return;
  if (b_ccdv->m_ccd->m_run_t != comp_chem_program_run_e::rt_geo_opt) return;
  //if (tgo_steps_ex->selectedItems().empty()) return;

  //b_ccdv->m_cur_step = tgo_steps_ex->currentRow();
  b_ccdv->update_connected_items();

  if (b_ccdv->m_cur_step < b_ccdv->m_ccd->m_steps.size()) {

      auto &step = b_ccdv->m_ccd->m_steps[b_ccdv->m_cur_step];

      m_tgo_step_info_etotal->setText(tr("%1").arg(QString::number(step.m_toten, 'g', 15)));

      m_tgo_step_info_dipole_moment->setText(
            tr(vector_template)
            .arg(QString::number(step.m_dipole_moment[0], 'g', float_digits))
            .arg(QString::number(step.m_dipole_moment[1], 'g', float_digits))
            .arg(QString::number(step.m_dipole_moment[2], 'g', float_digits))
          );

      m_tgo_step_info_gr_min->setText(
            tr(vector_template)
            .arg(QString::number(step.m_grad_min[0], 'g', float_digits))
            .arg(QString::number(step.m_grad_min[1], 'g', float_digits))
            .arg(QString::number(step.m_grad_min[2], 'g', float_digits))
          );

      m_tgo_step_info_gr_max->setText(
            tr(vector_template)
            .arg(QString::number(step.m_grad_max[0], 'g', float_digits))
            .arg(QString::number(step.m_grad_max[1], 'g', float_digits))
            .arg(QString::number(step.m_grad_max[2], 'g', float_digits))
          );

      m_tgo_step_info_gr_av->setText(
            tr(vector_template)
            .arg(QString::number(step.m_grad_aver[0], 'g', float_digits))
            .arg(QString::number(step.m_grad_aver[1], 'g', float_digits))
            .arg(QString::number(step.m_grad_aver[2], 'g', float_digits))
          );

      m_tgo_step_info_gr_av->setText(
            tr(vector_template)
            .arg(QString::number(step.m_grad_aver[0], 'g', float_digits))
            .arg(QString::number(step.m_grad_aver[1], 'g', float_digits))
            .arg(QString::number(step.m_grad_aver[2], 'g', float_digits))
          );

      m_tgo_step_info_gr_norm->setText(
            tr(gr_norm_template)
            .arg(QString::number(step.m_grad_norm_min, 'g', float_digits))
            .arg(QString::number(step.m_grad_norm_max, 'g', float_digits))
            .arg(QString::number(step.m_grad_norm_average, 'g', float_digits))
          );

    }

}

void ccd_view_obj_insp_widget_t::ui_step_forward() {

  if (b_ccdv) b_ccdv->traverse_step_manual(1);
  update_geo_opt_step_info_lbl();
  ui_cur_sel_step_item_changed();

}

void ccd_view_obj_insp_widget_t::ui_step_backward() {

  if (b_ccdv) b_ccdv->traverse_step_manual(-1);
  update_geo_opt_step_info_lbl();
  ui_cur_sel_step_item_changed();

}

void ccd_view_obj_insp_widget_t::ui_step_to_the_begin() {

  if (b_ccdv) b_ccdv->traverse_step_boundary(true);
  update_geo_opt_step_info_lbl();
  ui_cur_sel_step_item_changed();

}

void ccd_view_obj_insp_widget_t::ui_step_to_the_end() {

  if (b_ccdv) b_ccdv->traverse_step_boundary(false);
  update_geo_opt_step_info_lbl();
  ui_cur_sel_step_item_changed();


}

void ccd_view_obj_insp_widget_t::vib_anal_current_row_changed(int current_row) {

  if (b_ccdv &&
      (b_ccdv->m_ccd->m_run_t == comp_chem_program_run_e::rt_vib ||
       b_ccdv->m_ccd->m_run_t == comp_chem_program_run_e::rt_raman) &&
      !b_ccdv->m_ccd->m_vibs.empty() &&
      current_row >= 0 &&
      current_row < b_ccdv->m_ccd->m_vibs.size()) {

        b_ccdv->m_cur_vib = current_row;
        b_ccdv->update_connected_items();

    }

}
