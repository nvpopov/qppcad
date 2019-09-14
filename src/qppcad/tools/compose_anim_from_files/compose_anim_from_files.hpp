#ifndef QPPCAD_TOOL_COMPOSE_ANIM_FROM_FILES
#define QPPCAD_TOOL_COMPOSE_ANIM_FROM_FILES

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/geom_view_selector_widget.hpp>
#include <QDialogButtonBox>

namespace qpp {

  namespace cad {

    class compose_anim_widget_t : public QDialog {

        Q_OBJECT

      public:

        QVBoxLayout *main_lt;
        qgeom_view_selector_widget_t *gv_selector;
        QDialogButtonBox *dialog_bb;

        compose_anim_widget_t();

    };

    class compose_anim_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item, uint32_t _error_ctx) override;

    };

  } // namespace cad

} // namespace qpp

#endif
