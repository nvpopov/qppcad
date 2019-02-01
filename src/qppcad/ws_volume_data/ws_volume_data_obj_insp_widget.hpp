#ifndef QPP_CAD_WS_VOLUME_DATA_OBJ_INSP_H
#define QPP_CAD_WS_VOLUME_DATA_OBJ_INSP_H

#include <qppcad/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_volume_data/ws_volume_data.hpp>

namespace qpp {

  namespace cad {

    class ws_volume_data_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
//        ws_item_tab_widget_t *tab_geo_opt;
//        ws_item_tab_widget_t *tab_vibs;
        ws_volume_data_t *b_vol{nullptr};
        QLabel *general_volume_type;
        qbinded_float_spinbox *vol_isovalue;

        qbinded_color3_input *vol_color_pos;
        qbinded_color3_input *vol_color_neg;
        qbinded_color3_input *vol_color_vol;

        ws_volume_data_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  }

}
#endif
