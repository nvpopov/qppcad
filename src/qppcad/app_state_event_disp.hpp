#ifndef QPP_CAD_ASTATE_EVENT_DISP
#define QPP_CAD_ASTATE_EVENT_DISP

#include <qppcad/qppcad.hpp>
#include <QObject>

namespace qpp {

  namespace cad {

    class app_state_event_disp_t : public QObject {
        Q_OBJECT
      signals:
          void workspaces_changed();
    };

  }

}

#endif
