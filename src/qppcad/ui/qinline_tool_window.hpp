#ifndef QPPCAD_QINLINE_TOOL_WINDOW
#define QPPCAD_QINLINE_TOOL_WINDOW

#include <qppcad/core/qppcad.hpp>
#include <atomic>
#include <qppcad/ui/qembed_window.hpp>
#include <qppcad/ui/ws_item_inline_tool_widget.hpp>
#include <QPushButton>

namespace qpp {

namespace cad {

enum class qinline_tool_type_e {
  tool_vertical,
  tool_horizontal
};

class qinline_tool_window_t : public qembed_window_t {

  Q_OBJECT

private:

  std::atomic_bool p_currently_applying{false};

public:

  QPushButton                  *m_btn_apply_tool;
  QPushButton                  *m_btn_cancel_tool;
  ws_item_inline_tool_widget_t *m_cur_wdgt{nullptr};
  qinline_tool_type_e           m_inline_tool_type;

  explicit qinline_tool_window_t(qinline_tool_type_e inline_tool_type,
                                 QWidget *parent = nullptr);

  void mark_as_horizontal();
  void mark_as_vertical();

  bool is_active();

public slots:

  void apply_triggered();
  void close_triggered();

};

} // namespace qpp::cad

} // namespace qpp

#endif
