#ifndef QPP_CAD_GEOM_VIEW_EXTENDED_EDITOR
#define QPP_CAD_GEOM_VIEW_EXTENDED_EDITOR

#include <qppcad/ws_item/ws_item_extended_editor.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

  namespace cad {

    class geom_view_extended_editor_t : public ws_item_extended_editor_t {

        Q_OBJECT

      public:

        geom_view_extended_editor_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  }

}

#endif
