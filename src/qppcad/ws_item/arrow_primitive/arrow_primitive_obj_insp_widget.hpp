#ifndef QPPCAD_WS_ARROW_PRIMITIVE_OBJECT_INSP
#define QPPCAD_WS_ARROW_PRIMITIVE_OBJECT_INSP
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/arrow_primitive/arrow_primitive.hpp>

namespace qpp {

  namespace cad {

    class arrow_primitive_obj_insp_widget_t final: public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:

        arrow_primitive_t *b_ap{nullptr};

        qspoiler_widget_t *gb_arrow_settings;
        QFormLayout *gb_arrow_settings_lt;
        qbinded_float3_input_t *ap_target;
        qbinded_float_spinbox_t *ap_unf_arrow_scale;
        qbinded_float_spinbox_t *ap_unf_arrow_cap_len;
        qbinded_float_spinbox_t *ap_unf_arrow_cap_scale;
        qbinded_color3_input_t *ap_arrow_color;

        arrow_primitive_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
