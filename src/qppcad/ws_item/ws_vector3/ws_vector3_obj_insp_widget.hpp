#ifndef QPPCAD_WS_VECTOR3_OBJECT_INSP
#define QPPCAD_WS_VECTOR3_OBJECT_INSP
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/ws_vector3/ws_vector3.hpp>

namespace qpp {

  namespace cad {

    class ws_vector3_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
      
        ws_vector3_t *b_v3;

        ws_vector3_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
