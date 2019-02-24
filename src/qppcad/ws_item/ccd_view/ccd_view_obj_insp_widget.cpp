#include <qppcad/ws_item/ccd_view/ccd_view_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

void ccd_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (_binding_item && _binding_item->get_type() ==
      ccd_view_t::get_type_static()) {
      ccd_view_t *dp = _binding_item->cast_as<ccd_view_t>();
      if (dp) {
          // std::cout << "accepted ccd_view_t in ::bind_to_item" << std::endl;
          b_ccd = dp;
        }
      else {
          //std::cout << "ALL IS WRONG" << std::endl;
          b_ccd = nullptr;
        }
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
}

void ccd_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_ccd) {
      // std::cout << "ALL IS GOOD" << std::endl;
      gen_ccd_info_prog_name->setText(
            QString::fromStdString(
              qpp::ccdprog2str[b_ccd->m_ccd->m_comp_chem_program]
            )
          );
      gen_ccd_info_run_type->setText(
            QString::fromStdString(
              qpp::ccdrt2str[b_ccd->m_ccd->m_run_t]
            )
          );
      switch (b_ccd->m_ccd->m_run_t) {

        case comp_chem_program_run_t::rt_unknown :
          tabBar()->setTabEnabled(1, false);
          tabBar()->setTabEnabled(2, false);
          break;

        case comp_chem_program_run_t::rt_energy :
          tabBar()->setTabEnabled(1, false);
          tabBar()->setTabEnabled(2, false);
          break;

        case comp_chem_program_run_t::rt_geo_opt :
          tabBar()->setTabEnabled(1, true);
          tabBar()->setTabEnabled(2, false);
          update_geo_opt();
          break;

        default :
          break;
        }

      //
    }
}

void ccd_view_obj_insp_widget_t::unbind_item() {
  ws_item_obj_insp_widget_t::unbind_item();
  b_ccd = nullptr;
}

void ccd_view_obj_insp_widget_t::update_geo_opt() {

  gb_go_conv_series->clear();

  if (b_ccd->m_ccd->m_steps.empty()) {
      gb_go_conv_graph->setVisible(false);
      return;
    } else {
      gb_go_conv_graph->setVisible(true);
    }

  float max_energy = b_ccd->m_ccd->m_steps[0].m_toten;
  float min_energy = b_ccd->m_ccd->m_steps[0].m_toten;

  for (int i = 0 ; i < b_ccd->m_ccd->m_steps.size(); i++) {

      if (max_energy < b_ccd->m_ccd->m_steps[i].m_toten)
        max_energy = b_ccd->m_ccd->m_steps[i].m_toten;

      if (min_energy > b_ccd->m_ccd->m_steps[i].m_toten)
        min_energy = b_ccd->m_ccd->m_steps[i].m_toten;

      gb_go_conv_series->append(
            i, b_ccd->m_ccd->m_steps[i].m_toten);
    }

  gb_go_conv_chart->removeSeries(gb_go_conv_series);
  gb_go_conv_chart->addSeries(gb_go_conv_series);
  gb_go_conv_chart->axisX()->setMin(0);
  gb_go_conv_chart->axisX()->setMax(gb_go_conv_series->count());
  gb_go_conv_chart->axisY()->setRange(min_energy, max_energy);
  //gb_go_conv_chart->axisY()->labe
}

ccd_view_obj_insp_widget_t::ccd_view_obj_insp_widget_t()
  : ws_item_obj_insp_widget_t() {

  tab_geo_opt = define_tab(tr("Geometry opt."),
                           "://images/settings.svg",
                           "://images/settings.svg");
  tab_vibs = define_tab(tr("Vibrational analysis"),
                        "://images/settings.svg",
                        "://images/settings.svg");

  ws_item_is_visible->hide();
  ws_item_is_visible_label->hide();

  gb_gen_ccd_info = new QGroupBox(tr("CCD info"));
  gb_gen_ccd_info_lt = new QFormLayout;
  gen_ccd_info_prog_name = new QLabel;
  gen_ccd_info_run_type = new QLabel;

  pre_init_group_box(gb_gen_ccd_info, gb_gen_ccd_info_lt);
  gb_gen_ccd_info_lt->addRow(tr("Program"), gen_ccd_info_prog_name);
  gb_gen_ccd_info_lt->addRow(tr("Run type"), gen_ccd_info_run_type);
  post_init_group_box(gb_gen_ccd_info, gb_gen_ccd_info_lt);

  tab_general->tab_inner_widget_layout->addWidget(gb_gen_ccd_info);
  tab_general->tab_inner_widget_layout->addStretch(0);

  gb_go_conv_graph = new QGroupBox(tr("Energy convergence"));
  gb_go_conv_graph_lt = new QHBoxLayout;
  gb_go_conv_graph->setLayout(gb_go_conv_graph_lt);

  gb_go_conv_series = new QLineSeries();
  gb_go_conv_chart = new QChart();
  gb_go_conv_chart_view = new QChartView(gb_go_conv_chart);
  gb_go_conv_chart_view->setRenderHint(QPainter::Antialiasing);
  gb_go_conv_chart->setMargins(QMargins(2,2,2,2));
  gb_go_conv_graph_lt->addWidget(gb_go_conv_chart_view);
  gb_go_conv_chart->legend()->hide();
  gb_go_conv_chart->addSeries(gb_go_conv_series);
  gb_go_conv_chart->createDefaultAxes();
  gb_go_conv_chart->axisX()->setTitleText(tr("Steps"));
  gb_go_conv_chart->axisY()->setTitleText(tr("Energy, eV"));

  tab_geo_opt->tab_inner_widget_layout->addWidget(gb_go_conv_graph);
  tab_geo_opt->tab_inner_widget_layout->addStretch(0);

  //tg_form_layout->
}
