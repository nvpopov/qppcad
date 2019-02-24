#include <qppcad/ws_item/pgf_producer/pgf_producer_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

pgf_producer_obj_insp_widget_t::pgf_producer_obj_insp_widget_t() {

  tab_cell_range = define_tab(tr("Define geometry_pg{f,d} cell"),
                        "://images/outline-tune-24px.svg");

  gb_pgf_data = new QGroupBox(tr("Pgf settings"));
  gb_pgf_data_lt = new QFormLayout;
  pre_init_group_box(gb_pgf_data, gb_pgf_data_lt);

  pgf_data_src = new qbinded_ws_item_combobox;
  pgf_data_src->setMaximumWidth(def_gen_control_width);

  pgf_data_dst = new qbinded_ws_item_combobox;
  pgf_data_dst->setMaximumWidth(def_gen_control_width);

  pgf_data_ag = new qbinded_ws_item_combobox;
  pgf_data_ag->setMaximumWidth(def_gen_control_width);

  gb_pgf_data_lt->addRow(tr("Source"), pgf_data_src);
  gb_pgf_data_lt->addRow(tr("Destination"), pgf_data_dst);
  gb_pgf_data_lt->addRow(tr("Array group"), pgf_data_ag);

  post_init_group_box(gb_pgf_data, gb_pgf_data_lt);
  tab_general->tab_inner_widget_layout->addWidget(gb_pgf_data);

  tab_general->tab_inner_widget_layout->addStretch();
  tab_cell_range->tab_inner_widget_layout->addStretch();

}

void pgf_producer_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void pgf_producer_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void pgf_producer_obj_insp_widget_t::unbind_item() {

}
