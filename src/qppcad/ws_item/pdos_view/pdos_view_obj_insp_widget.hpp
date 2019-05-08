#ifndef QPP_CAD_PDOS_VIEW_OBJECT_INSP
#define QPP_CAD_PDOS_VIEW_OBJECT_INSP
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/pdos_view/pdos_view.hpp>

namespace qpp {

  namespace cad {

    class pdos_view_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
      
        pdos_view_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  }

}
#endif