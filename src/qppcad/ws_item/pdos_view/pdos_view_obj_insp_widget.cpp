#include <qppcad/ws_item/pdos_view/pdos_view_obj_insp_widget.hpp>
#include <io/cp2k_pdos.hpp>
#include <qppcad/app_state.hpp>
#include <QFileDialog>

using namespace qpp;
using namespace qpp::cad;

pdos_view_obj_insp_widget_t::pdos_view_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  tg_actions_add_channel = new QPushButton(tr("Add data"));
  connect(tg_actions_add_channel,
          &QPushButton::clicked,
          this,
          &pdos_view_obj_insp_widget_t::add_data_btn_clicked);
  //tg_actions_add_channel->setFixedHeight(astate->size_guide.obj_insp_button_h());

  tg_actions_layout->addWidget(tg_actions_add_channel, 1, 0);
  tg_actions_layout->setRowMinimumHeight(1, astate->size_guide.obj_insp_button_h()+5);

  m_data_target_menu = new QMenu(this);
  m_data_target_cp2k = new QAction(tr("CP2K"));
  m_data_target_vasp = new QAction(tr("VASP"));

  m_data_target_menu->addAction(m_data_target_cp2k);
  m_data_target_menu->addAction(m_data_target_vasp);

  gb_pdos_info = new qspoiler_widget_t("PDOS info");
  gb_pdos_info_lt = new QFormLayout;
  gb_pdos_info->add_content_layout(gb_pdos_info_lt);

  pdos_info_is_spin_polarized = new QLabel;
  pdos_info_num_species = new QLabel;
  pdos_info_num_channels = new QLabel;

  pdos_ewindow_low = new qbinded_float_spinbox_t;
  pdos_ewindow_low->set_min_max_step(-1000, 1000, 0.001, 4);
  pdos_ewindow_high = new qbinded_float_spinbox_t;
  pdos_ewindow_high->set_min_max_step(-1000, 1000, 0.001, 4);
  pdos_steps = new qbinded_int_spinbox_t;
  pdos_steps->set_min_max_step(1, 10000, 1);
  pdos_sigma = new qbinded_float_spinbox_t;
  pdos_sigma->set_min_max_step(-10, 10, 0.001, 4);

  gb_pdos_info_lt->addRow(tr("Num. of channels"), pdos_info_num_channels);
  gb_pdos_info_lt->addRow(tr("Num. of species"), pdos_info_num_species);
  gb_pdos_info_lt->addRow(tr("Is spin polarized"), pdos_info_is_spin_polarized);
  gb_pdos_info_lt->addRow(tr("E_w_low "), pdos_ewindow_low);
  gb_pdos_info_lt->addRow(tr("E_w_high"), pdos_ewindow_high);
  gb_pdos_info_lt->addRow(tr("Steps"), pdos_steps);
  gb_pdos_info_lt->addRow(tr("Sigma"), pdos_sigma);

  init_form_lt(gb_pdos_info_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_pdos_info);
  tab_general->tab_inner_widget_lt->addStretch(1);

}

void pdos_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (_binding_item) {
      auto as_pdv = _binding_item->cast_as<pdos_view_t>();
      if (as_pdv) m_pdv = as_pdv;
    }

  if (m_pdv) {
      pdos_ewindow_low->bind_value(&m_pdv->m_pdos_ewindow_low);
      pdos_ewindow_high->bind_value(&m_pdv->m_pdos_ewindow_high);
      pdos_sigma->bind_value(&m_pdv->m_pdos_sigma);
      pdos_steps->bind_value(&m_pdv->m_smearing_steps);
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void pdos_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (m_pdv) {
      pdos_info_num_channels->setText(QString("%1").arg(m_pdv->get_num_channels()));
      pdos_info_num_species->setText(QString("%1").arg(m_pdv->get_num_species()));
      pdos_info_is_spin_polarized->setText(m_pdv->is_spin_polarized() ? "Yes" : "No");
    }

}

void pdos_view_obj_insp_widget_t::unbind_item() {

  pdos_ewindow_low->unbind_value();
  pdos_ewindow_high->unbind_value();
  pdos_sigma->unbind_value();
  pdos_steps->unbind_value();

}

void pdos_view_obj_insp_widget_t::add_data_btn_clicked() {

  auto res = m_data_target_menu->exec(QCursor::pos());
  if (res && m_pdv) {

      QStringList ls = QFileDialog::getOpenFileNames();
      comp_chem_program_e ccd_prog{comp_chem_program_e::pr_unknown};

      if (res == m_data_target_cp2k) ccd_prog = comp_chem_program_e::pr_cp2k;
      if (res == m_data_target_vasp) ccd_prog = comp_chem_program_e::pr_vasp;

      for (auto &file_name : ls) {
          std::string file_name_ss = file_name.toStdString();
          m_pdv->add_data_from_file(file_name_ss, ccd_prog);
      }

      m_pdv->rebuild_plots();

    }

  update_from_ws_item();

}
