#ifndef QPPCAD_ARROW_ARRAY_OBJECT_INSP
#define QPPCAD_ARROW_ARRAY_OBJECT_INSP

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/arrow_array/arrow_array.hpp>

namespace qpp {

  namespace cad {

    class arrow_array_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:

        arrow_array_t *b_aa;
        qspoiler_widget_t *gb_aa_settings;
        QFormLayout *gb_aa_settings_lt;
        qbinded_ws_item_combobox_t *aa_settings_src;
        qbinded_float_spinbox_t *aa_unf_arrow_len;
        qbinded_float_spinbox_t *aa_unf_arrow_scale;
        qbinded_float_spinbox_t *aa_unf_arrow_cap_len;
        qbinded_float_spinbox_t *aa_unf_arrow_cap_scale;
        qbinded_color3_input_t *aa_arrow_color;
        qbinded_checkbox_t *aa_affected_by_sv;

        arrow_array_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
