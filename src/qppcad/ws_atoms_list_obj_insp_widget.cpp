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
    }
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
  gb_display_settings->setLayout(gb_display_settings_layout);

  tab_display->tab_inner_widget_layout->addWidget(gb_display_settings);
  display_settings_draw_atoms = new qbinded_checkbox;
  display_settings_draw_bonds = new qbinded_checkbox;
  display_settings_draw_img_atoms = new qbinded_checkbox;
  display_settings_draw_img_bonds = new qbinded_checkbox;

  gb_display_settings_layout->addRow(tr("Draw atoms"), display_settings_draw_atoms);
  gb_display_settings_layout->addRow(tr("Draw bonds"), display_settings_draw_bonds);
  gb_display_settings_layout->addRow(tr("Draw imaginary atoms"), display_settings_draw_img_atoms);
  gb_display_settings_layout->addRow(tr("Draw imaginary atoms"), display_settings_draw_img_bonds);

}
