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

        qspoiler_widget_t *gb_psg_view_settings;
        QFormLayout *gb_psg_view_settings_lt;

        QLabel *tg_info_sym_gr;
        QLabel *tg_info_total_sym_op;
        QLabel *tg_num_axes;
        QLabel *tg_num_planes;
        QLabel *tg_bounded_info;
        QLabel *tg_has_inverse;

        qbinded_checkbox_t *disp_vs_plane_alpha_enabled;
        qbinded_checkbox_t *disp_vs_show_axes;
        qbinded_checkbox_t *disp_vs_show_planes;

        qbinded_float_spinbox_t *disp_vs_plane_scale;
        qbinded_float_spinbox_t *disp_vs_plane_alpha;
        qbinded_float_spinbox_t *disp_vs_arrow_len;
        qbinded_float_spinbox_t *disp_vs_arrow_scale;
        qbinded_float_spinbox_t *disp_vs_arrow_cap_len;
        qbinded_float_spinbox_t *disp_vs_arrow_cap_scale;

        ws_item_tab_widget_t *tab_display;
        qspoiler_widget_t *gb_colorize;
        QFormLayout *gb_colorize_lt;
        qbinded_color3_input_t *disp_vs_plane_color;
        std::array<qbinded_color3_input_t*, AXIS_COLORIZE_SIZE> gb_colorize_ctrls;

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
