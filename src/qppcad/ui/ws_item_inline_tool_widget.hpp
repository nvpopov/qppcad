#ifndef QPPCAD_WS_ITEM_INLINE_TOOL
#define QPPCAD_WS_ITEM_INLINE_TOOL

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <QWidget>

namespace qpp {

  namespace cad {

    class ws_item_inline_tool_widget_t : public QWidget {

      Q_OBJECT

    public:

      ws_item_t *m_src{nullptr};
      std::optional<epoch_t> m_epoch_before;
      bool m_restore_epoch_on_cancel{false};

      ws_item_inline_tool_widget_t(QWidget *parent = nullptr);

      void apply();
      void cancel();

      virtual bool restore_cam_on_cancel();
      virtual void on_apply();
      virtual void on_cancel();
      virtual void bind_item(ws_item_t *item);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
