#ifndef QPP_CAD_WS_TRAJ_HIGHLIGHT_OBJECT_INSP
#define QPP_CAD_WS_TRAJ_HIGHLIGHT_OBJECT_INSP
#include <qppcad/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_traj_highlight/ws_traj_highlight.hpp>

namespace qpp {

  namespace cad {

    class ws_traj_highlight_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
      
        ws_traj_highlight_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  }

}
#endif