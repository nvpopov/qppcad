#ifndef QPPCAD_WS_MATRIX3_OBJECT_INSP
#define QPPCAD_WS_MATRIX3_OBJECT_INSP
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/ws_matrix3/ws_matrix3.hpp>

namespace qpp {

  namespace cad {

    class ws_matrix3_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
      
        qspoiler_widget_t *m_splr_matinfo;
        QFormLayout *m_splr_matinfo_lt;

        qbinded_matrix3_input_t *m_matinfo;

        ws_matrix3_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
