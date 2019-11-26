#include <qppcad/ws_item/traj_hl/traj_hl_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

traj_hl_obj_insp_widget_t::traj_hl_obj_insp_widget_t() {

  m_gb_traj_settings = new qspoiler_widget_t(tr("Trajectory settings"));
  m_gb_traj_settings_lt = new QFormLayout;
  m_gb_traj_settings->add_content_layout(m_gb_traj_settings_lt);

  m_combo_traj_style = new qbinded_combobox_t;
  m_combo_traj_style->addItem(tr("Points"));
  m_combo_traj_style->addItem(tr("Lines"));
  m_combo_traj_style->addItem(tr("LinesLoop"));
  m_combo_traj_style->addItem(tr("XMarks"));
  m_combo_traj_style->addItem(tr("Spheres"));

  m_disp_traj_color = new qbinded_color3_input_t;
  m_elem_size = new qbinded_float_spinbox_t;

  m_gb_traj_settings_lt->addRow(tr("Trajectory color"), m_disp_traj_color);
  m_gb_traj_settings_lt->addRow(tr("Trajectory style"), m_combo_traj_style);
  m_gb_traj_settings_lt->addRow(tr("Element size"), m_elem_size);
  init_form_lt(m_gb_traj_settings_lt);

  tab_general->tab_inner_widget_lt->addWidget(m_gb_traj_settings);
  tab_general->tab_inner_widget_lt->addStretch(1);

}

void traj_hl_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  if (_binding_item && _binding_item->get_type() == traj_hl_t::get_type_static()) {

      b_th = _binding_item->cast_as<traj_hl_t>();
      m_disp_traj_color->bind_value(&b_th->m_traj_color, b_th);
      m_combo_traj_style->bind_value(reinterpret_cast<int*>(&b_th->m_traj_style));
      m_elem_size->bind_value(&b_th->m_elem_size);

    }

}

void traj_hl_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void traj_hl_obj_insp_widget_t::unbind_item() {

  m_combo_traj_style->unbind_value();
  m_disp_traj_color->unbind_value();
  m_elem_size->unbind_value();

}
