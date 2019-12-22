#ifndef QPPCAD_QINLINE_TOOL_WINDOW
#define QPPCAD_QINLINE_TOOL_WINDOW

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ui/qembed_window.hpp>
#include <QPushButton>

namespace qpp {

  namespace cad {

    class qinline_tool_window_t : public qembed_window_t {

      Q_OBJECT

    public:

      QPushButton *m_btn_apply_tool;
      QPushButton *m_btn_cancel_tool;

      explicit qinline_tool_window_t(QWidget *parent = nullptr);

      void mark_as_horizontal();
      void mark_as_vertical();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
