#ifndef QPPCAD_WS_COMP_CHEM_DATA_OBJ_INSP_H
#define QPPCAD_WS_COMP_CHEM_DATA_OBJ_INSP_H

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view_obj_insp_widget.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view.hpp>
#include <QListWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

namespace qpp {

  namespace cad {

    class ccd_view_obj_insp_widget_t final: public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:

        ccd_view_t *b_ccdv{nullptr};

        ws_item_tab_widget_t *m_tab_geo_opt;
        ws_item_tab_widget_t *m_tab_vibs;
        ws_item_tab_widget_t *m_tab_tddft;

        QLabel *m_tab_info_prg;
        qspoiler_widget_t *m_gb_gen_ccd_info;
        QFormLayout *m_gb_gen_ccd_info_lt;
        QLabel *m_gen_info_prg_name;
        QLabel *m_gen_info_run_type;

        qspoiler_widget_t *m_tgo_sel_step;
        QFormLayout *m_tgo_sel_step_lt;
        //QListWidget *tgo_steps_ex;
        QHBoxLayout *m_tgo_acts_lt;
        QLabel *m_tgo_steps_current;
        QPushButton *m_tgo_steps_fwd;
        QPushButton *m_tgo_steps_bck;
        QPushButton *m_tgo_steps_begin;
        QPushButton *m_tgo_steps_end;

        qspoiler_widget_t *m_tgo_step_info;
        QFormLayout *m_tgo_step_info_lt;
        QLabel *m_tgo_step_info_etotal;
        QLabel *m_tgo_step_info_dipole_moment;
        QLabel *m_tgo_step_info_gr_min;
        QLabel *m_tgo_step_info_gr_max;
        QLabel *m_tgo_step_info_gr_av;
        QLabel *m_tgo_step_info_gr_norm;
        qbinded_combobox_t *m_tgo_step_copy_charges;

        qspoiler_widget_t *m_ttd_info;
        QFormLayout *m_ttd_info_lt;
        QLabel *m_ttd_info_total_trans;

        qspoiler_widget_t *m_gb_vib_modes;
        QVBoxLayout *m_gb_vib_modes_lt;
        QListWidget *m_vib_modes_list_wdgt;

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

        void update_geo_opt();
        void update_geo_opt_step_info();
        void update_geo_opt_step_info_lbl();

        void update_vib_anal();
        ccd_view_obj_insp_widget_t();

      protected:

        void resizeEvent(QResizeEvent *event) override;

      public slots:

        void ui_cur_sel_step_item_changed();
        void ui_step_forward();
        void ui_step_backward();
        void ui_step_to_the_begin();
        void ui_step_to_the_end();

        void vib_anal_current_row_changed(int current_row);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
