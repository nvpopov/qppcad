#ifndef QPP_PYTHON_CONSOLE
#define QPP_PYTHON_CONSOLE

#include <qppcad/qppcad.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace qpp {
  class app_state_t;
  struct notebook_record_t {
      string m_rec;
      string m_answer;
      bool m_executed{false};
  };

  class console_widget_t {
    public:
      vector<notebook_record_t> nb_recs;
      uint32_t m_id;
      bool     m_active;
      console_widget_t(app_state_t *init_app_state);
      void toggle_visible();
      void render();
  };

}

#endif
