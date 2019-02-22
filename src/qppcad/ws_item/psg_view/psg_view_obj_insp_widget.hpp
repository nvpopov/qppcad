#ifndef QPP_CAD_WS_POINT_SYM_GROUP_OBJECT_INSP
#define QPP_CAD_WS_POINT_SYM_GROUP_OBJECT_INSP

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>

namespace qpp {

  namespace cad {

    class psg_view_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        psg_view_t *b_pg{nullptr};

        psg_view_obj_insp_widget_t();

        QGroupBox *tg_info_summary_widget;
        QFormLayout *tg_info_summary_lt;
        QLabel *tg_info_sym_gr;
        QLabel *tg_info_total_sym_op;
        qbinded_checkbox *tg_plane_alpha_enabled;
        qbinded_float_spinbox *tg_plane_scale;
        qbinded_float_spinbox *tg_axis_scale;
        qbinded_float_spinbox *tg_axis_len_mod;
        QGroupBox *tg_general_appearance;
        QFormLayout *tg_general_appearance_lt;

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  }

}
#endif
