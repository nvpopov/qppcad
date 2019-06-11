#ifndef QPPCAD_TOOL_COMPOSE_ANIM_FROM_FILES
#define QPPCAD_TOOL_COMPOSE_ANIM_FROM_FILES
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/geom_view_selector_widget.hpp>

namespace qpp {

  namespace cad {

    class compose_anim_from_files_widget_t : public QDialog {

        Q_OBJECT

      private:

      public:

        compose_anim_from_files_widget_t();

    };

    class compose_anim_from_files_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item, uint32_t _error_ctx) override;

    };

  } // namespace cad

} // namespace qpp

#endif
