#ifndef QPPCAD_WS_POINT_SYM_GROUP_OBJECT_INSP
#define QPPCAD_WS_POINT_SYM_GROUP_OBJECT_INSP

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>

namespace qpp {

  namespace cad {

    class psg_view_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        psg_view_t *b_pg{nullptr};

        psg_view_obj_insp_widget_t();

        qspoiler_widget_t *gb_psg_summary;
        QFormLayout *gb_psg_summary_lt;
        QLabel *tg_info_sym_gr;
        QLabel *tg_info_total_sym_op;
        qbinded_checkbox_t *tg_plane_alpha_enabled;

        qbinded_checkbox_t *cb_show_axes;
        qbinded_checkbox_t *cb_show_planes;

        qbinded_float_spinbox_t *tg_plane_scale;
        qbinded_float_spinbox_t *tg_axis_scale;
        qbinded_float_spinbox_t *tg_axis_len_mod;

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
