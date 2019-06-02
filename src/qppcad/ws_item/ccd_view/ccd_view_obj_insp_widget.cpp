#include <qppcad/ws_item/ccd_view/ccd_view_obj_insp_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void ccd_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (_binding_item && _binding_item->get_type() == ccd_view_t::get_type_static()) {

      ccd_view_t *dp = _binding_item->cast_as<ccd_view_t>();

      if (dp) {
          // std::cout << "accepted ccd_view_t in ::bind_to_item" << std::endl;
          b_ccdv = dp;
        }
      else {
          //std::cout << "ALL IS WRONG" << std::endl;
          b_ccdv = nullptr;
        }

    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void ccd_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_ccdv) {

      gen_info_prog_name->setText(
            QString::fromStdString(qpp::ccdprog2str[b_ccdv->m_ccd->m_comp_chem_program]));

      gen_info_run_type->setText(QString::fromStdString(qpp::ccdrt2str[b_ccdv->m_ccd->m_run_t]));

      switch (b_ccdv->m_ccd->m_run_t) {

        case comp_chem_program_run_e::rt_unknown :
          set_tab_enabled(tab_vibs, false);
          set_tab_enabled(tab_geo_opt, false);
          break;

        case comp_chem_program_run_e::rt_energy :
          set_tab_enabled(tab_vibs, false);
          set_tab_enabled(tab_geo_opt, false);
          break;

        case comp_chem_program_run_e::rt_geo_opt :
          set_tab_enabled(tab_vibs, false);
          set_tab_enabled(tab_geo_opt, true);
          update_geo_opt();
          break;

        case comp_chem_program_run_e::rt_vib :
          set_tab_enabled(tab_vibs, true);
          set_tab_enabled(tab_geo_opt, false);
          break;

        default :
          set_tab_enabled(tab_vibs, false);
          set_tab_enabled(tab_geo_opt, false);
          break;
        }

    }

}

void ccd_view_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();
  b_ccdv = nullptr;

}

void ccd_view_obj_insp_widget_t::update_geo_opt() {

  tgo_steps_ex->blockSignals(true);
  tgo_steps_ex->clear();
  tgo_steps_ex->setEnabled(!(!b_ccdv || !b_ccdv->m_ccd || b_ccdv->m_ccd->m_steps.empty()));

  if (b_ccdv) {
      //rebuild steps list
      for (size_t i = 0; i < b_ccdv->m_ccd->m_steps.size(); i++) {
          QString nl = tr("Step № %1, E= %2 [au]")
                       .arg(i)
                       .arg( QString::number(b_ccdv->m_ccd->m_steps[i].m_toten, 'g', 15));
          tgo_steps_ex->addItem(nl);
        }

      //select step
      tgo_steps_ex->setCurrentRow(b_ccdv->m_cur_step);
      ui_cur_selected_step_item_changed();
    }

  tgo_steps_ex->blockSignals(false);

}

void ccd_view_obj_insp_widget_t::update_geo_opt_step_info() {

}

ccd_view_obj_insp_widget_t::ccd_view_obj_insp_widget_t() : ws_item_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  tab_geo_opt = def_tab(tr("Geometry opt."),
                        "://images/baseline-graphic_eq-24px.svg",
                        "://images/baseline-graphic_eq-24px_d.svg");

  tab_vibs = def_tab(tr("Vibrational analysis"),
                     "://images/vib_anal.svg",
                     "://images/vib_anal_d.svg");

  ws_item_is_visible->hide();
  ws_item_is_visible_label->hide();

  gb_gen_ccd_info = new qspoiler_widget_t(tr("CCD info"));
  gb_gen_ccd_info_lt = new QFormLayout;
  gb_gen_ccd_info->add_content_layout(gb_gen_ccd_info_lt);

  gen_info_prog_name = new QLabel;
  gen_info_run_type = new QLabel;

  gb_gen_ccd_info_lt->addRow(tr("Program"), gen_info_prog_name);
  gb_gen_ccd_info_lt->addRow(tr("Run type"), gen_info_run_type);
  init_form_lt(gb_gen_ccd_info_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_gen_ccd_info);
  tab_general->tab_inner_widget_lt->addStretch(0);

  gb_normal_modes = new qspoiler_widget_t(tr("Normal modes"));
  gb_normal_modes_lt = new QVBoxLayout;
  gb_normal_modes->add_content_layout(gb_normal_modes_lt);

  tab_vibs->tab_inner_widget_lt->addWidget(gb_normal_modes);
  tab_vibs->tab_inner_widget_lt->addStretch(0);

  //tab geo opt
  tgo_select_step = new qspoiler_widget_t(tr("Geometry optimization step"));
  tgo_select_step_lt = new QHBoxLayout;
  tgo_select_step->add_content_layout(tgo_select_step_lt);

  tgo_steps_ex = new QListWidget;
  tgo_steps_ex->setFixedHeight(astate->size_guide.obj_insp_list_wdgt_h());
  tgo_select_step_lt->addWidget(tgo_steps_ex);
  connect(tgo_steps_ex,
          &QListWidget::itemSelectionChanged,
          this,
          &ccd_view_obj_insp_widget_t::ui_cur_selected_step_item_changed);

  tgo_step_info = new qspoiler_widget_t(tr("Step info"));
  tgo_step_info_lt = new QFormLayout;
  tgo_step_info->add_content_layout(tgo_step_info_lt);

  tgo_step_info_etotal = new QLabel;
  tgo_step_info_dipole_moment = new QLabel;
  tgo_step_info_gr_min = new QLabel;
  tgo_step_info_gr_max = new QLabel;
  tgo_step_info_gr_av = new QLabel;

  tgo_step_info_lt->addRow(tr("Energy[a.u]"), tgo_step_info_etotal);
  tgo_step_info_lt->addRow(tr("Dipole moment"), tgo_step_info_dipole_moment);
  tgo_step_info_lt->addRow(tr("Gradient min"), tgo_step_info_gr_min);
  tgo_step_info_lt->addRow(tr("Gradient max"), tgo_step_info_gr_max);
  tgo_step_info_lt->addRow(tr("Gradient aver."), tgo_step_info_gr_av);

  init_form_lt(tgo_step_info_lt);

  tab_geo_opt->tab_inner_widget_lt->addWidget(tgo_select_step);
  tab_geo_opt->tab_inner_widget_lt->addWidget(tgo_step_info);
  tab_geo_opt->tab_inner_widget_lt->addStretch(1);
  //end tab geo opt

}

void ccd_view_obj_insp_widget_t::ui_cur_selected_step_item_changed() {

  const auto float_digits = 6;
  const auto vector_template = "x = %1 \ny = %2 \nz = %3";

  tgo_step_info_etotal->setText("-");
  tgo_step_info_dipole_moment->setText("-");
  tgo_step_info_gr_min->setText("-");
  tgo_step_info_gr_max->setText("-");
  tgo_step_info_gr_av->setText("-");

  if (!b_ccdv) return;
  if (b_ccdv->m_ccd->m_run_t != comp_chem_program_run_e::rt_geo_opt) return;
  if (tgo_steps_ex->selectedItems().empty()) return;

  b_ccdv->m_cur_step = tgo_steps_ex->currentRow();
  b_ccdv->update_connected_items();

  if (b_ccdv->m_cur_step < b_ccdv->m_ccd->m_steps.size()) {

      auto &step = b_ccdv->m_ccd->m_steps[b_ccdv->m_cur_step];

      tgo_step_info_etotal->setText(tr("%1").arg(QString::number(step.m_toten, 'g', 15)));

      tgo_step_info_dipole_moment->setText(
            tr(vector_template)
            .arg(QString::number(step.m_dipole_moment[0], 'g', float_digits))
            .arg(QString::number(step.m_dipole_moment[1], 'g', float_digits))
            .arg(QString::number(step.m_dipole_moment[2], 'g', float_digits))
          );

      tgo_step_info_gr_min->setText(
            tr(vector_template)
            .arg(QString::number(step.m_gradient_min[0], 'g', float_digits))
            .arg(QString::number(step.m_gradient_min[1], 'g', float_digits))
            .arg(QString::number(step.m_gradient_min[2], 'g', float_digits))
          );

      tgo_step_info_gr_max->setText(
            tr(vector_template)
            .arg(QString::number(step.m_gradient_max[0], 'g', float_digits))
            .arg(QString::number(step.m_gradient_max[1], 'g', float_digits))
            .arg(QString::number(step.m_gradient_max[2], 'g', float_digits))
          );

      tgo_step_info_gr_av->setText(
            tr(vector_template)
            .arg(QString::number(step.m_gradient_average[0], 'g', float_digits))
            .arg(QString::number(step.m_gradient_average[1], 'g', float_digits))
            .arg(QString::number(step.m_gradient_average[2], 'g', float_digits))
          );

    }

}
