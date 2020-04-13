#ifndef QPPCAD_TOOL_GV_COLORIZER
#define QPPCAD_TOOL_GV_COLORIZER
#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>
#include <qppcad/ui/ws_item_inline_tool_widget.hpp>
#include <qppcad/core/iupdatable.hpp>

#include <QWidget>
#include <QApplication>

namespace qpp {

  namespace cad {

    class gv_colorizer_widget_t : public ws_item_inline_tool_widget_t,
                                 public iupdatable_externally_t {
        Q_OBJECT

      private:

      public:

	gv_colorizer_widget_t(QWidget *parent = nullptr);
        bool restore_cam_on_cancel() override;
        void on_apply() override;
        void on_cancel() override;
        void bind_item(ws_item_t *item) override;
        void updated_externally(uint32_t update_reason) override;

    };

    class gv_colorizer_tool_t : public ws_item_tool_t {
    
      public:
      
        void exec(ws_item_t *item, uint32_t _error_ctx) override;
        ws_item_inline_tool_widget_t *construct_inline_tool() override;

    };
    
  } // namespace qpp::cad
 
} // namespace qpp

#endif
