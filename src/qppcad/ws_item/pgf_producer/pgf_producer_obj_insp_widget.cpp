#include <qppcad/ws_item/pgf_producer/pgf_producer_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

pgf_producer_obj_insp_widget_t::pgf_producer_obj_insp_widget_t() {

  tab_general->tab_inner_widget_layout->addStretch();

}

void pgf_producer_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void pgf_producer_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void pgf_producer_obj_insp_widget_t::unbind_item() {

}
