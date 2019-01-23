#ifndef QPP_CAD_WS_POINT_SYM_GROUP_OBJECT_INSP
#define QPP_CAD_WS_POINT_SYM_GROUP_OBJECT_INSP
#include <qppcad/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_point_sym_group/ws_point_sym_group.hpp>

namespace qpp {

  namespace cad {

    class ws_point_sym_group_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        ws_point_sym_group_t *b_pg;

        ws_point_sym_group_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  }

}
#endif
