#ifndef QPPCAD_PDOS_VIEW_OBJECT_INSP
#define QPPCAD_PDOS_VIEW_OBJECT_INSP
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/pdos_view/pdos_view.hpp>
#include <QMenu>
#include <QAction>

namespace qpp {

  namespace cad {

    class pdos_view_obj_insp_widget_t final: public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:

        pdos_view_t *m_pdv{nullptr};

        // ui
        QMenu *m_data_target_menu;
        QAction *m_data_target_cp2k;
        QAction *m_data_target_vasp;
        QPushButton *tg_actions_add_channel;
        QPushButton *tg_actions_rebuild;

        qspoiler_widget_t *gb_pdos_info;
        QFormLayout *gb_pdos_info_lt;
        QLabel *pdos_info_is_spin_polarized;
        QLabel *pdos_info_num_species;
        QLabel *pdos_info_num_channels;
        qbinded_float_spinbox_t *pdos_ewindow_low;
        qbinded_float_spinbox_t *pdos_ewindow_high;
        qbinded_int_spinbox_t *pdos_steps;
        qbinded_float_spinbox_t *pdos_sigma;

        pdos_view_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

      public slots:

        void add_data_btn_clicked();
        void rebuild_btn_clicked();

    };

  } // namespace cad

} // namespace qpp

#endif
