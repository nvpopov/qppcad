#ifndef QPPCAD_WS_CUBE_PRIMITIVE_OBJECT_INSP
#define QPPCAD_WS_CUBE_PRIMITIVE_OBJECT_INSP

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>

namespace qpp {

  namespace cad {

    class cube_primitive_t;

    class cube_primitive_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        cube_primitive_t *b_cp{nullptr};
        qspoiler_widget_t *tg_cube_params;
        QFormLayout *tg_cube_params_lt;
        qbinded_float3_input_t *cube_param_scale;
        qbinded_color3_input_t *cube_param_color;
        qbinded_combobox_t *cube_render_mode;
        qbinded_checkbox_t *cube_render_alpha_enabled;
        qbinded_float_spinbox_t *cube_render_alpha;

        cube_primitive_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
