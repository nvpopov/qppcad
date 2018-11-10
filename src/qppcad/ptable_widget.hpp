#ifndef QPP_CAD_PTABLE_WIDGET
#define QPP_CAD_PTABLE_WIDGET

#include <qppcad/qppcad.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stl.h>

namespace qpp {

  namespace cad {

    class app_state_t;

    class ptable_widget_t {

      public:

        bool m_active{false};
        bool m_filter_name{true};
        bool m_filter_symbol{true};
        std::string m_filter{""};
        ptable_widget_t(app_state_t *astate);
        void render();

    };

  }

}

#endif
