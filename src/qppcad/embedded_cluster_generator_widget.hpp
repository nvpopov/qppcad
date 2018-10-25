#ifndef QPPCAD_EMBEDDED_CLUSTER_WIDGET
#define QPPCAD_EMBEDDED_CLUSTER_WIDGET

#include <qppcad/qppcad.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stl.h>

namespace qpp {

    namespace cad {

      class app_state_t;

      class embedded_cluster_generator_widget_t {

        private:
          bool m_active{false};

        public:
          embedded_cluster_generator_widget_t(app_state_t *astate);
          void set_active(bool active);
          bool get_active();
          void render();
      };

    }

}

#endif
