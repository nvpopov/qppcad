#include <qppcad/ws_volume_data/ws_volume_data_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_volume_data_obj_insp_widget_t::ws_volume_data_obj_insp_widget_t() {

  general_volume_type = new QLabel(tr("Molecular orbitals"));
  tg_form_layout->addRow(tr("Type:"), general_volume_type);

}

void ws_volume_data_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {
  auto _tmp = dynamic_cast<ws_volume_data_t*>(_binding_item);

  if (_tmp) {
      b_vol = _tmp;
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
}

void ws_volume_data_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_vol) {
      if (b_vol->m_volume.m_has_negative_values) general_volume_type->setText(tr("MO"));
      else general_volume_type->setText(tr("DENSITY"));

    }
}

void ws_volume_data_obj_insp_widget_t::unbind_item() {

}
