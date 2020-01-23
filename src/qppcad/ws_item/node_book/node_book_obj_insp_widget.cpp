#include <qppcad/ws_item/node_book/node_book_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

node_book_obj_insp_widget_t::node_book_obj_insp_widget_t() : ws_item_obj_insp_widget_t () {

  gb_nb_settings = new qspoiler_widget_t(tr("Node book settings"));
  gb_nb_settings_lt = new QFormLayout;
  gb_nb_settings->add_content_layout(gb_nb_settings_lt);

  nb_highlight_dirty_nodes = new qbinded_checkbox_t;
  nb_auto_recompute = new qbinded_checkbox_t;

  gb_nb_settings_lt->addRow(tr("Auto recompute"), nb_auto_recompute);
  gb_nb_settings_lt->addRow(tr("Highlight dirty"), nb_highlight_dirty_nodes);

  init_form_lt(gb_nb_settings_lt);

  m_tab_general->tab_inner_widget_lt->addWidget(gb_nb_settings);
  m_tab_general->tab_inner_widget_lt->addStretch();

}

void node_book_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  if (auto _as_nb = _binding_item->cast_as<node_book_t>(); _as_nb != nullptr) {

      b_nb = _as_nb;

      nb_highlight_dirty_nodes->bind_value(&b_nb->m_highlight_dirty_nodes);
      nb_auto_recompute->bind_value(&b_nb->m_auto_recompute);

    }

}

void node_book_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void node_book_obj_insp_widget_t::unbind_item() {

  nb_highlight_dirty_nodes->unbind_value();
  nb_auto_recompute->unbind_value();

}
