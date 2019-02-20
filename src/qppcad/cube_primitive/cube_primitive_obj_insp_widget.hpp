#ifndef QPP_CAD_WS_CUBE_PRIMITIVE_OBJECT_INSP
#define QPP_CAD_WS_CUBE_PRIMITIVE_OBJECT_INSP

#include <qppcad/ws_item_obj_insp_widget.hpp>

namespace qpp {

  namespace cad {

    class cube_primitive_t;

    class cube_primitive_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        cube_primitive_t *b_cp{nullptr};
        QGroupBox *tg_cube_params;
        QFormLayout *tg_cube_params_lt;
        qbinded_float3_input *cube_param_scale;
        qbinded_color3_input *cube_param_color;
        qbinded_combobox *cube_render_mode;

        cube_primitive_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  }

}

#endif
