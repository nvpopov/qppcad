#include <qppcad/ws_atoms_list_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

void ws_atoms_list_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  auto _tmp = dynamic_cast<ws_atoms_list_t*>(_binding_item);

  if (_tmp) {
      m_binded_atoms_list = _tmp;
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
}

void ws_atoms_list_obj_insp_widget_t::update_from_ws_item() {
  ws_item_obj_insp_widget_t::update_from_ws_item();
  if (m_binded_atoms_list) {
      tg_geom_summary_total_atoms->setText(
            QString::fromStdString(fmt::format("{}", m_binded_atoms_list->m_geom->nat())));
      tg_geom_summary_total_types->setText(
            QString::fromStdString(fmt::format("{}", m_binded_atoms_list->m_geom->n_atom_types())));

      display_settings_draw_atoms->bind_value(&m_binded_atoms_list->m_show_atoms);
      display_settings_draw_bonds->bind_value(&m_binded_atoms_list->m_show_bonds);
      display_settings_draw_img_atoms->bind_value(&m_binded_atoms_list->m_show_imaginary_atoms);
      display_settings_draw_img_bonds->bind_value(&m_binded_atoms_list->m_show_imaginary_bonds);
      display_settings_atom_scale->bind_value(&m_binded_atoms_list->m_atom_scale_factor);
      display_settings_bond_scale->bind_value(&m_binded_atoms_list->m_bond_scale_factor);
      display_settings_render_style->bind_value(
            reinterpret_cast<int*>(&m_binded_atoms_list->m_cur_render_type));
      display_settings_labels_style->bind_value(
            reinterpret_cast<int*>(&m_binded_atoms_list->m_labels->m_style));
      tabBar()->setTabEnabled(2, m_binded_atoms_list->m_anim->animable());

      //update type table
      tg_type_summary_table->clearContents();
      tg_type_summary_table->setRowCount(m_binded_atoms_list->m_geom->n_atom_types());
      for (int i = 0; i < m_binded_atoms_list->m_geom->n_atom_types(); i++) {

          QString n_name_str = QString::fromStdString(m_binded_atoms_list->m_geom->atom_of_type(i));
          QTableWidgetItem *n_name = new QTableWidgetItem(n_name_str);
          n_name->setTextAlignment(Qt::AlignCenter);
          tg_type_summary_table->setItem(i,0, n_name);

          QString n_c_str = tr("%1").arg(m_binded_atoms_list->m_geom->get_atom_count_by_type(i));
          QTableWidgetItem *n_c = new QTableWidgetItem(n_c_str);
          n_c->setTextAlignment(Qt::AlignCenter);
          tg_type_summary_table->setItem(i,1, n_c);

          QString n_clr_str = "";
          QTableWidgetItem *n_clr = new QTableWidgetItem(n_clr_str);
          n_clr->setTextAlignment(Qt::AlignCenter);

          auto ap_idx = ptable::number_by_symbol(m_binded_atoms_list->m_geom->atom_of_type(i));
          vector3<float> bc(0.0, 0.0, 1.0);
          if (ap_idx) {bc = ptable::get_inst()->arecs[*ap_idx-1].aColorJmol;}
          QColor color_bck;
          color_bck.setRgbF(bc[0], bc[1], bc[2]);
          n_clr->setBackgroundColor(color_bck);
          tg_type_summary_table->setItem(i,2, n_clr);

        }
    }
}

void ws_atoms_list_obj_insp_widget_t::unbind_item() {
  ws_item_obj_insp_widget_t::unbind_item();
  display_settings_draw_atoms->unbind_value();
  display_settings_draw_bonds->unbind_value();
  display_settings_draw_img_atoms->unbind_value();
  display_settings_draw_img_bonds->unbind_value();
  display_settings_atom_scale->unbind_value();
  display_settings_bond_scale->unbind_value();
  display_settings_render_style->unbind_value();
  display_settings_labels_style->unbind_value();
}

ws_atoms_list_obj_insp_widget_t::ws_atoms_list_obj_insp_widget_t()
  : ws_item_obj_insp_widget_t() {

  tab_display = define_tab(tr("Display"));
  tab_animation = define_tab(tr("Animation"));
  tab_measurement = define_tab(tr("Measurement"));

  tg_geom_summary_widget = new QGroupBox;
  tg_geom_summary_layout = new QFormLayout;
  tg_geom_summary_layout->setLabelAlignment(Qt::AlignRight);
  tg_geom_summary_widget->setLayout(tg_geom_summary_layout);
  tg_geom_summary_widget->setTitle("Geometry summary");

  tab_general->tab_inner_widget_layout->addWidget(tg_geom_summary_widget);

  tg_geom_summary_total_atoms = new QLabel;
  tg_geom_summary_total_types = new QLabel;

  tg_geom_summary_layout->addRow(tr("Total atoms:"), tg_geom_summary_total_atoms);
  tg_geom_summary_layout->addRow(tr("Total atom types:"), tg_geom_summary_total_types);

  //display tab

  gb_display_settings = new QGroupBox(tr("Draw settings"));
  gb_display_settings_layout = new QFormLayout;
  gb_display_settings_layout->setLabelAlignment(Qt::AlignRight);
  gb_display_settings->setLayout(gb_display_settings_layout);


  tab_display->tab_inner_widget_layout->addWidget(gb_display_settings);
  display_settings_draw_atoms = new qbinded_checkbox;
  display_settings_draw_bonds = new qbinded_checkbox;
  display_settings_draw_img_atoms = new qbinded_checkbox;
  display_settings_draw_img_bonds = new qbinded_checkbox;

  display_settings_atom_scale = new qbinded_float_spinbox;
  display_settings_atom_scale->set_min_max_step(0.01, 3.0, 0.01);
  display_settings_bond_scale = new qbinded_float_spinbox;
  display_settings_bond_scale->set_min_max_step(0.01, 3.0, 0.01);

  display_settings_render_style = new qbinded_combobox;
  display_settings_render_style->addItem("Balls and sticks");
  display_settings_render_style->addItem("Dynamic lines");
  display_settings_render_style->addItem("X-atom lines");
  display_settings_render_style->addItem("Billboards");
  display_settings_render_style->addItem("Buffered billboards");

  display_settings_labels_style = new qbinded_combobox;
  display_settings_labels_style->addItem("None");
  display_settings_labels_style->addItem("Id");
  display_settings_labels_style->addItem("Type");
  display_settings_labels_style->addItem("Type and Id");

  gb_display_settings_layout->addRow(tr("Draw style"), display_settings_render_style);
  gb_display_settings_layout->addRow(tr("Draw atoms"), display_settings_draw_atoms);
  gb_display_settings_layout->addRow(tr("Draw bonds"), display_settings_draw_bonds);
  gb_display_settings_layout->addRow(tr("Draw img. atoms"), display_settings_draw_img_atoms);
  gb_display_settings_layout->addRow(tr("Draw img. bonds"), display_settings_draw_img_bonds);
  gb_display_settings_layout->addRow(tr("Atom scale"), display_settings_atom_scale);
  gb_display_settings_layout->addRow(tr("Bond scale"), display_settings_bond_scale);
  gb_display_settings_layout->addRow(tr("Labels style"), display_settings_labels_style);

  tg_type_summary_widget = new QGroupBox(tr("Type Summary"));
  tg_type_summary_layout = new QVBoxLayout;
  tg_type_summary_widget->setLayout(tg_type_summary_layout);
  tg_type_summary_table = new QTableWidget;
  tg_type_summary_table->setColumnCount(3);
  QStringList table_hdr;
  table_hdr.push_back("Name");
  table_hdr.push_back("Count");
  table_hdr.push_back("Color");
  tg_type_summary_table->setHorizontalHeaderLabels(table_hdr);
  tg_type_summary_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tg_type_summary_table->verticalHeader()->setVisible(false);
  tg_type_summary_table->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  tg_type_summary_layout->addWidget(tg_type_summary_table);
  tab_general->tab_inner_widget_layout->addWidget(tg_type_summary_widget);


}
