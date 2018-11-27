#ifndef QPP_CAD_ASTATE_EVENT_DISP
#define QPP_CAD_ASTATE_EVENT_DISP

#include <qppcad/qppcad.hpp>
#include <QObject>
#include <QMetaObject>

namespace qpp {

  namespace cad {

    class app_state_event_disp_t : public QObject {
        Q_OBJECT
      signals:
        void workspaces_changed_signal();
        void current_workspace_changed_signal();
        void current_workspace_selected_item_changed_signal();
        void current_workspace_selected_item_content_changed_signal();
        void current_workspace_selected_item_frame_changed_signal();
      public:
        void workspaces_changed();
        void current_workspace_changed();
        void current_workspace_selected_item_changed();
        void current_workspace_selected_item_content_changed();
        void current_workspace_selected_item_frame_changed();
        explicit app_state_event_disp_t(QObject *parent = 0);
    };

  }

}

#endif
