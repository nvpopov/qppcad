#include <qppcad/ws_traj_highlight/ws_traj_highlight_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_traj_highlight_obj_insp_widget_t::ws_traj_highlight_obj_insp_widget_t() {

  gb_traj_settings = new QGroupBox(tr("Trajectory settings"));
  gb_traj_settings_lt = new QFormLayout;

  pre_init_group_box(gb_traj_settings, gb_traj_settings_lt);
  disp_traj_color = new qbinded_color3_input;
  gb_traj_settings_lt->addRow(tr("Trajectory color"), disp_traj_color);
  post_init_group_box(gb_traj_settings, gb_traj_settings_lt);

  tab_general->tab_inner_widget_layout->addWidget(gb_traj_settings);
  tab_general->tab_inner_widget_layout->addStretch();

}

void ws_traj_highlight_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  if (_binding_item && _binding_item->get_type() == ws_traj_highlight_t::get_type_static()) {
      b_th = _binding_item->cast_as<ws_traj_highlight_t>();
      disp_traj_color->bind_value(&b_th->m_traj_color, b_th);
    }
}

void ws_traj_highlight_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void ws_traj_highlight_obj_insp_widget_t::unbind_item() {
  disp_traj_color->unbind_value();
}
