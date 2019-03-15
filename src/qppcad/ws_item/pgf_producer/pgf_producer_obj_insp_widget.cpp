#include <qppcad/ws_item/pgf_producer/pgf_producer_obj_insp_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>

using namespace qpp;
using namespace qpp::cad;

pgf_producer_obj_insp_widget_t::pgf_producer_obj_insp_widget_t() {

  tab_cell_range = define_tab(tr("Define geometry_pg{f,d} cell"),
                        "://images/outline-tune-24px.svg");

  gb_pgf_data = new QGroupBox(tr("Pos. gen. form settings"));
  gb_pgf_data_lt = new QFormLayout;
  pre_init_gb(gb_pgf_data, gb_pgf_data_lt);

  pgf_data_src = new qbinded_ws_item_combobox;
  pgf_data_src->setMaximumWidth(def_gen_control_width);
  pgf_data_src->m_type_id = geom_view_t::get_type_static();

  pgf_data_dst = new qbinded_ws_item_combobox;
  pgf_data_dst->setMaximumWidth(def_gen_control_width);
  pgf_data_dst->m_type_id = geom_view_t::get_type_static();

  pgf_data_ag = new qbinded_ws_item_combobox;
  pgf_data_ag->setMaximumWidth(def_gen_control_width);
  pgf_data_ag->m_type_id = psg_view_t::get_type_static();

  gb_pgf_data_lt->addRow(tr("Source"), pgf_data_src);
  gb_pgf_data_lt->addRow(tr("Destination"), pgf_data_dst);
  gb_pgf_data_lt->addRow(tr("Array group"), pgf_data_ag);

  post_init_gb(gb_pgf_data, gb_pgf_data_lt);
  tab_general->tab_inner_widget_lt->addWidget(gb_pgf_data);

  tab_general->tab_inner_widget_lt->addStretch();
  tab_cell_range->tab_inner_widget_lt->addStretch();

}

void pgf_producer_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  auto p_b_rp = _binding_item->cast_as<pgf_producer_t>();
  if (p_b_rp) {
      b_pr = p_b_rp;
      pgf_data_src->bind_value(&b_pr->m_src, b_pr);
      pgf_data_dst->bind_value(&b_pr->m_dst, b_pr);
      pgf_data_ag->bind_value(&b_pr->m_ag, b_pr);
    }

}

void pgf_producer_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void pgf_producer_obj_insp_widget_t::unbind_item() {

  pgf_data_src->unbind_value();
  pgf_data_dst->unbind_value();
  pgf_data_ag->unbind_value();

  m_binded_item = nullptr;
  b_pr = nullptr;

}
