#include <qppcad/ws_item/pgf_producer/pgf_producer_obj_insp_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

pgf_producer_obj_insp_widget_t::pgf_producer_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  tab_cell_range = def_tab(tr("Define geometry_pg{f,d} cell"),
                           "://images/outline-tune-24px.svg",
                           "://images/outline-tune-24px_d.svg");

  gb_pgf_data = new qspoiler_widget_t(tr("Pos. gen. form settings"));
  gb_pgf_data_lt = new QFormLayout;
  gb_pgf_data->add_content_layout(gb_pgf_data_lt);

  pgf_data_src = new qbinded_ws_item_combobox_t;
  pgf_data_src->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());
  pgf_data_src->m_type_id = geom_view_t::get_type_static();
  pgf_data_src->m_updated_externally_event = true;

  pgf_data_dst = new qbinded_ws_item_combobox_t;
  pgf_data_dst->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());
  pgf_data_dst->m_type_id = geom_view_t::get_type_static();
  pgf_data_dst->m_updated_externally_event = true;

  pgf_data_ag = new qbinded_ws_item_combobox_t;
  pgf_data_ag->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());
  pgf_data_ag->m_type_id = psg_view_t::get_type_static();
  pgf_data_ag->m_updated_externally_event = true;

  gb_pgf_data_lt->addRow(tr("Source"), pgf_data_src);
  gb_pgf_data_lt->addRow(tr("Destination"), pgf_data_dst);
  gb_pgf_data_lt->addRow(tr("Array group"), pgf_data_ag);
  init_form_lt(gb_pgf_data_lt);

  gb_gen_cell = new qspoiler_widget_t(tr("Generalized cell"));
  gb_gen_cell_lt = new QFormLayout;
  gb_gen_cell->add_content_layout(gb_gen_cell_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_pgf_data);

  tab_general->tab_inner_widget_lt->addStretch();

  tab_cell_range->tab_inner_widget_lt->addWidget(gb_gen_cell);
  tab_cell_range->tab_inner_widget_lt->addStretch();

}

void pgf_producer_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  auto p_b_rp = _binding_item->cast_as<pgf_producer_t>();
  if (p_b_rp) {
      b_pr = p_b_rp;
      pgf_data_src->bind_value(&b_pr->m_src, b_pr);
      pgf_data_dst->bind_value(&b_pr->m_dst, b_pr);
      pgf_data_ag->bind_value(&b_pr->m_psg, b_pr);
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  //update_cell_indexes_ranges();

}

void pgf_producer_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();
  update_cell_indexes_ranges();

  set_tab_enabled(tab_cell_range, (b_pr && b_pr->m_src && b_pr->m_dst && b_pr->m_psg));

}

void pgf_producer_obj_insp_widget_t::unbind_item() {

  pgf_data_src->unbind_value();
  pgf_data_dst->unbind_value();
  pgf_data_ag->unbind_value();

  m_binded_item = nullptr;
  b_pr = nullptr;

  update_cell_indexes_ranges();

}

void pgf_producer_obj_insp_widget_t::update_cell_indexes_ranges() {

  app_state_t* astate = app_state_t::get_inst();

  astate->tlog(
        "pgf_prod..::update_cell_indexes_ranges() -> b_pr is valid ? = {}", b_pr != nullptr);

  if (b_pr) {

      //check size of controlers
      astate->tlog("pgf_prod..::update_cell_indexes_ranges() -> before resizing, ci_rng> = {},"
                   "orders_range> = {}", m_ci_ranges.size(), b_pr->m_orders_range.size());

      if (!b_pr->m_orders_range.empty() && (m_ci_ranges.size() < b_pr->m_orders_range.size()) ) {
          m_ci_ranges.resize(b_pr->m_orders_range.size());
        }

      //check existence of binded inputs
      for (size_t i = 0; i < b_pr->m_orders_range.size(); i++)
        if (!m_ci_ranges[i]) {
            m_ci_ranges[i] = std::make_shared<qbinded_int2b_input_t>();
          }

      //do we need to rebuild layout?
      if (m_last_ci_ranges_size != b_pr->m_orders_range.size()) {

          //clear layout
          while(gb_gen_cell_lt->count() > 0) {
              gb_gen_cell_lt->takeAt(0);
            }

          //fill layout with new elements
          for (size_t i = 0; i < b_pr->m_orders_range.size(); i++)
            gb_gen_cell_lt->addRow(tr("DIM [%1]").arg(i+1), m_ci_ranges[i].get());

          init_form_lt(gb_gen_cell_lt);

          m_last_ci_ranges_size = b_pr->m_orders_range.size();
        }

      //bind values
      astate->tlog("pgf_prod..::update_cell_indexes_ranges() -> bind values[size={}]",
                   m_ci_ranges.size());
      for (size_t i = 0; i < b_pr->m_orders_range.size(); i++)
        if (!m_ci_ranges[i]) m_ci_ranges[i]->bind_value(&b_pr->m_orders_range[i], b_pr);
    }
  else {

      //unbind all non null elements
      for (size_t i = 0; i < m_ci_ranges.size(); i++)
        if (m_ci_ranges[i]) m_ci_ranges[i]->unbind_value();

    }

}
