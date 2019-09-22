#include <qppcad/ws_item/compl_list_view/compl_list_view_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

compl_list_view_obj_insp_widget_t::compl_list_view_obj_insp_widget_t()
  : ws_item_obj_insp_widget_t() {

  tab_compl_list = def_tab(tr("List data"), "://images/settings.svg");

  clv_info_wdgt = new qspoiler_widget_t(tr("Compliance List Info"));
  clv_info_wdgt_lt = new QFormLayout;
  clv_info_wdgt->add_content_layout(clv_info_wdgt_lt);

  clv_total_recs = new QLabel;
  clv_info_wdgt_lt->addRow(tr("Total entries"), clv_total_recs);
  init_form_lt(clv_info_wdgt_lt);

  tab_general->tab_inner_widget_lt->addWidget(clv_info_wdgt);
  tab_general->tab_inner_widget_lt->addStretch(0);

}

void compl_list_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (_binding_item && _binding_item->get_type() == compl_list_view_t::get_type_static()) {

      compl_list_view_t *dp = _binding_item->cast_as<compl_list_view_t>();

      if (dp) {

          b_clv = dp;
          clv_total_recs->setText(QString("%1").arg(b_clv->m_compl_list.size()));

        }

      else {

          b_clv = nullptr;
          clv_total_recs->setText(tr("Empty"));

        }

    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void compl_list_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_clv) {

    }

}

void compl_list_view_obj_insp_widget_t::unbind_item() {

  b_clv = nullptr;

}
