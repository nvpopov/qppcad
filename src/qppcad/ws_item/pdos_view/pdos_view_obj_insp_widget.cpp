#include <qppcad/ws_item/pdos_view/pdos_view_obj_insp_widget.hpp>
#include <io/cp2k_pdos.hpp>
#include <QFileDialog>

using namespace qpp;
using namespace qpp::cad;

pdos_view_obj_insp_widget_t::pdos_view_obj_insp_widget_t() {

  tg_actions_add_channel = new QPushButton(tr("Add data"));
  connect(tg_actions_add_channel,
          &QPushButton::clicked,
          this,
          &pdos_view_obj_insp_widget_t::add_data_btn_clicked);

  tg_actions_layout->addWidget(tg_actions_add_channel);

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
  pdos_ewindow_low->set_min_max_step(-1000, 1000, 0.01, 4);
  pdos_ewindow_high = new qbinded_float_spinbox_t;
  pdos_ewindow_high->set_min_max_step(-1000, 1000, 0.01, 4);

  gb_pdos_info_lt->addRow(tr("Num. of channels"), pdos_info_num_channels);
  gb_pdos_info_lt->addRow(tr("Num. of species"), pdos_info_num_species);
  gb_pdos_info_lt->addRow(tr("Is spin polarized"), pdos_info_is_spin_polarized);
  gb_pdos_info_lt->addRow(tr("E_low  [eV]"), pdos_ewindow_low);
  gb_pdos_info_lt->addRow(tr("E_high [eV]"), pdos_ewindow_high);

  init_form_lt(gb_pdos_info_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_pdos_info);
  tab_general->tab_inner_widget_lt->addStretch();

}

void pdos_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (_binding_item) {
      auto as_pdv = _binding_item->cast_as<pdos_view_t>();
      if (as_pdv) m_pdv = as_pdv;
    }

  if (m_pdv) {
      pdos_ewindow_low->bind_value(&m_pdv->m_pdos_ewindow_low);
      pdos_ewindow_high->bind_value(&m_pdv->m_pdos_ewindow_high);
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

}

void pdos_view_obj_insp_widget_t::add_data_btn_clicked() {

  auto res = m_data_target_menu->exec(QCursor::pos());
  if (res) {

      QStringList ls = QFileDialog::getOpenFileNames();

      for (auto &file_name : ls) {

          bool succes{false};
          auto _file_name = file_name.toStdString();
          pdos_data_t<float> pdos_rec;
          std::ifstream pdos_file(_file_name);

          if (res == m_data_target_cp2k) {
              read_cp2k_pdos(_file_name, pdos_file, pdos_rec);
              succes = true;
            } else if (res == m_data_target_vasp) {

            }

          if (succes && m_pdv) m_pdv->m_pdos_recs.push_back(std::move(pdos_rec));

        }

    }

  update_from_ws_item();

}
