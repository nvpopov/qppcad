#ifndef QPP_CAD_SUPER_CELL_WIDGET
#define QPP_CAD_SUPER_CELL_WIDGET

#include <qppcad/qppcad.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stl.h>


namespace qpp {

  namespace cad {

    class app_state_t;

    class super_cell_widget_t {

      private:

      public:

        bool m_last_frame_inactive{true};
        super_cell_widget_t(app_state_t *astate);
    };

  }

}

#endif
