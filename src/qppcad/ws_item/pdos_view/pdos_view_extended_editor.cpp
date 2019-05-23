#include <qppcad/ws_item/pdos_view/pdos_view_extended_editor.hpp>
#include <QBarSeries>
#include <QScatterSeries>
#include <QSplineSeries>
#include <QValueAxis>

using namespace qpp;
using namespace qpp::cad;

pdos_view_extended_editor_t::pdos_view_extended_editor_t() {

  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(0,0,10,10);
  setLayout(main_lt);

}

void pdos_view_extended_editor_t::bind_to_item(ws_item_t *_binding_item) {

  //unbind old chart_view
  if (cur_chart_view) {
      cur_chart_view->setVisible(false);
      cur_chart_view->setParent(nullptr);
    }

  if (!_binding_item) {
      m_binded_nx = nullptr;
      cur_chart_view = nullptr;
      return;
    }

  auto as_nx = _binding_item->cast_as<pdos_view_t>();
  if (!as_nx) {
      m_binded_nx = nullptr;
      cur_chart_view = nullptr;
      return;
    }

  m_binded_nx = as_nx;
  //pdos_gen_chart_view->setChart(m_binded_nx->m_pdos_gen_chart);

  ws_item_extended_editor_t::bind_to_item(_binding_item);

  if (m_binded_nx) {
      cur_chart_view = m_binded_nx->m_pdos_chart_view;
      main_lt->addWidget(cur_chart_view);
      cur_chart_view->setVisible(true);
    }

}

void pdos_view_extended_editor_t::update_from_ws_item() {

  ws_item_extended_editor_t::update_from_ws_item();
  if (!m_binded_nx) return;
  //if (pdos_gen_chart_view->chart()) pdos_gen_chart_view->chart()->zoomReset();

}

void pdos_view_extended_editor_t::unbind_item() {

  ws_item_extended_editor_t::unbind_item();
  m_binded_nx = nullptr;
  //pdos_gen_chart_view->setChart(nullptr);

}

bool pdos_view_extended_editor_t::can_be_binded_to(ws_item_t *item) {
  return true;
}

QString pdos_view_extended_editor_t::header_name_hint() {
  return "pdos_view_extended_editor EDITOR";
}
