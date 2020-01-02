#ifndef QPPCAD_WS_ITEM_INLINE_TOOL
#define QPPCAD_WS_ITEM_INLINE_TOOL

#include <qppcad/core/qppcad.hpp>
#include <QWidget>

namespace qpp {

  namespace cad {

    class ws_item_inline_tool_widget_t : public QWidget {

      Q_OBJECT

    public:

      ws_item_inline_tool_widget_t(QWidget *parent = nullptr);
      virtual void on_apply();
      virtual void on_cancel();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
