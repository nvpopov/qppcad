#ifndef QPPCAD_WS_VOLUME_DATA_OBJ_INSP_H
#define QPPCAD_WS_VOLUME_DATA_OBJ_INSP_H

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/volume_view/volume_view.hpp>

namespace qpp {

  namespace cad {

    class volume_view_obj_insp_widget_t final: public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:

        volume_view_t *b_vol{nullptr};
        QLabel *m_general_volume_type;
        qbinded_float_spinbox_t *m_vol_isovalue;

        qbinded_checkbox_t *m_vol_render_permanent;
        qbinded_checkbox_t *m_vol_transparent;
        qbinded_float_spinbox_t *m_vol_alpha;

        qbinded_color3_input_t *m_vol_color_pos;
        qbinded_color3_input_t *m_vol_color_neg;
        qbinded_color3_input_t *m_vol_color_vol;

        qspoiler_widget_t *m_gb_volume_detail;
        QFormLayout *m_gb_volume_detail_lt;
        QComboBox *m_cb_current_volume;

        volume_view_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
        void update_binded_volume_controls();

      public slots:

        void cur_volume_index_changed(int index);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
