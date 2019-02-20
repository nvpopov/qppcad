#ifndef QPP_CAD_WS_COMP_CHEM_DATA_OBJ_INSP_H
#define QPP_CAD_WS_COMP_CHEM_DATA_OBJ_INSP_H

#include <qppcad/ws_item_obj_insp_widget.hpp>
#include <qppcad/ccd_view/ccd_view_obj_insp_widget.hpp>
#include <qppcad/ccd_view/ccd_view.hpp>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

namespace qpp {

  namespace cad {

    class ccd_view_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        ccd_view_t *b_ccd{nullptr};
        ws_item_tab_widget_t *tab_geo_opt;
        ws_item_tab_widget_t *tab_vibs;

        QLabel *tab_info_program;
        QGroupBox *gb_gen_ccd_info;
        QFormLayout *gb_gen_ccd_info_lt;
        QLabel *gen_ccd_info_prog_name;
        QLabel *gen_ccd_info_run_type;

        //start tab geometry optimization
        QGroupBox *gb_go_conv_graph;
        QHBoxLayout *gb_go_conv_graph_lt;
        QLineSeries *gb_go_conv_series;
        QChart *gb_go_conv_chart;
        QChartView *gb_go_conv_chart_view;
        //end tab geometry optimization

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

        void update_geo_opt();
        ccd_view_obj_insp_widget_t();
    };

  }

}
#endif
