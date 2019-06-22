#ifndef QPPCAD_ASTATE_EVENT_DISP
#define QPPCAD_ASTATE_EVENT_DISP

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
        void cur_ws_selected_item_measurements_changed_signal();
        void cur_ws_selected_item_position_changed_signal();
        void cur_ws_selected_item_need_to_update_obj_insp_signal();

        void obj_insp_tab_open_requested_signal(int tab_id);

        void extended_editor_open_requested_signal();
        void extended_editor_open_requested_with_order_signal(size_t editor_id);
        void extended_editor_close_requested_signal();

        void new_file_loaded_signal();
        void python_console_clear_requested_signal();
        void python_console_focus_requested_signal();
        void python_console_font_size_updated_signal();

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
        void cur_ws_selected_item_measurements_changed();
        void cur_ws_selected_item_need_to_update_obj_insp();
        void obj_insp_tab_open_requested(int tab_id);

        void extended_editor_open_requested();
        void extended_editor_open_requested_with_order(size_t editor_id);
        void extended_editor_close_requested();

        void new_file_loaded(const std::string &file_name,
                             const size_t ff,
                             const bool is_native);

        void python_console_clear_requested();
        void python_console_focus_requested();
        void python_console_font_size_updated();

        explicit app_state_event_disp_t(QObject *parent = 0);
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
