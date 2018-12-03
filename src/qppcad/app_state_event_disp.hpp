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
        void wss_changed_signal();
        void cur_ws_changed_signal();

        void cur_ws_edit_type_changed_signal();
        void cur_ws_selected_item_changed_signal();
        void cur_ws_selected_item_content_changed_signal();
        void cur_ws_selected_item_frame_changed_signal();
        void cur_ws_selected_atoms_list_selection_changed_signal();
        void cur_ws_selected_atoms_list_selected_content_changed_signal();
        void cur_ws_selected_atoms_list_cell_changed_signal();
        void cur_ws_selected_item_position_changed_signal();

      public:
        void wss_changed();
        void cur_ws_changed();
        void cur_ws_edit_type_changed();
        void cur_ws_selected_item_changed();
        void cur_ws_selected_item_content_changed();
        void cur_ws_selected_item_frame_changed();
        void cur_ws_selected_atoms_list_selection_changed();
        void cur_ws_selected_atoms_list_cell_changed();
        void cur_ws_selected_item_position_changed();
        void cur_ws_selected_atoms_list_selected_content_changed();

        explicit app_state_event_disp_t(QObject *parent = 0);
    };

  }

}

#endif
