#include <qppcad/ws_point_sym_group/ws_point_sym_group_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_point_sym_group_obj_insp_widget_t::ws_point_sym_group_obj_insp_widget_t() {

  tg_info_summary_widget = new QGroupBox;
  tg_info_summary_layout = new QFormLayout;
  tg_info_summary_layout->setLabelAlignment(Qt::AlignRight);
  tg_info_summary_widget->setLayout(tg_info_summary_layout);
  tg_info_summary_widget->setTitle("Symmetry group summary");
  tab_general->tab_inner_widget_layout->addWidget(tg_info_summary_widget);
  tg_info_sym_gr = new QLabel;
  tg_info_total_sym_op = new QLabel;
  tg_info_summary_layout->addRow(tr("Sym. gr. name "), tg_info_sym_gr);
  tg_info_summary_layout->addRow(tr("Total num. of op. "), tg_info_total_sym_op);
  tab_general->tab_inner_widget_layout->addStretch();
}

void ws_point_sym_group_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
  if (_binding_item && _binding_item->get_type() == ws_point_sym_group_t::get_type_static()) {
      b_pg = _binding_item->cast_as<ws_point_sym_group_t>();
      tg_info_sym_gr->setText(QString::fromStdString(b_pg->m_ag.name));
      tg_info_total_sym_op->setText(tr("%1").arg(b_pg->m_ag.size()));
    }
}

void ws_point_sym_group_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void ws_point_sym_group_obj_insp_widget_t::unbind_item() {

}
