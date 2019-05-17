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

  pdos_gen_chart = new QChart();
  pdos_gen_chart_view = new QChartView;
  pdos_gen_chart_view->setChart(pdos_gen_chart);

  main_lt->addWidget(pdos_gen_chart_view);

}

void pdos_view_extended_editor_t::bind_to_item(ws_item_t *_binding_item) {


  if (!_binding_item) {
      m_binded_nx = nullptr;
      return;
    }

  auto as_nx = _binding_item->cast_as<pdos_view_t>();
  if (!as_nx) {
      m_binded_nx = nullptr;
      return;
    }

  m_binded_nx = as_nx;

  ws_item_extended_editor_t::bind_to_item(_binding_item);

}

void pdos_view_extended_editor_t::update_from_ws_item() {

  ws_item_extended_editor_t::update_from_ws_item();
  pdos_gen_chart->removeAllSeries();

  if (!m_binded_nx) return;

  for (auto &rec : m_binded_nx->m_pdos_recs) {
       QLineSeries* series = new QLineSeries();
       //series->setUseOpenGL(true);
       for (auto &inner_rec : rec.m_data)
         if (inner_rec(0,0) > m_binded_nx->m_pdos_ewindow_low &&
             inner_rec(0,0) < m_binded_nx->m_pdos_ewindow_high) {
           // sum over occupancy
           float occ = 0;
           for (size_t i = 2; i < inner_rec.size(); i++)
             occ += inner_rec(0,i);
           series->append(inner_rec(0,0), rec.m_is_alpha ? occ : -occ);
         }
       series->setName(QString("%1 %2")
                       .arg(QString::fromStdString(rec.m_specie_name))
                       .arg(rec.m_is_alpha ? "↑" : "↓")
                       );
       pdos_gen_chart->addSeries(series);
    }

  pdos_gen_chart->createDefaultAxes();

}

void pdos_view_extended_editor_t::unbind_item() {

  ws_item_extended_editor_t::unbind_item();
  m_binded_nx = nullptr;

}

bool pdos_view_extended_editor_t::can_be_binded_to(ws_item_t *item) {
  return true;
}

QString pdos_view_extended_editor_t::header_name_hint() {
  return "pdos_view_extended_editor EDITOR";
}
