#include <qppcad/ws_item/ws_matrix3/ws_matrix3_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_matrix3_obj_insp_widget_t::ws_matrix3_obj_insp_widget_t() {

  m_splr_matinfo = new qspoiler_widget_t(tr("Matrix data"));
  m_splr_matinfo_lt = new QVBoxLayout;
  m_splr_matinfo->add_content_layout(m_splr_matinfo_lt);

  m_matinfo = new qbinded_matrix3_input_t;
  m_splr_matinfo_lt->addWidget(m_matinfo);
  //init_form_lt(m_splr_matinfo_lt);

  m_tab_general->tab_inner_widget_lt->addWidget(m_splr_matinfo);
  m_tab_general->tab_inner_widget_lt->addStretch();

}

void ws_matrix3_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (!_binding_item) return;
  auto as_m3 = _binding_item->cast_as<ws_matrix3_t>();

  if (as_m3) {
      p_binded_m3 = as_m3;
    } else {
      p_binded_m3 = nullptr;
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
  
}

void ws_matrix3_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (p_binded_m3) {

      m_matinfo->bind_value(&p_binded_m3->m_data);

    }

}

void ws_matrix3_obj_insp_widget_t::unbind_item() {

  m_matinfo->unbind_value();

}
