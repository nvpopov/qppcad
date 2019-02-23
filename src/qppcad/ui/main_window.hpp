#ifndef QPP_CAD_MAIN_WINDOW_H
#define QPP_CAD_MAIN_WINDOW_H

#include <qppcad/qppcad.hpp>
#include <qppcad/ui/ws_viewer_widget.hpp>
#include <qppcad/ui/object_inspector_widget.hpp>
#include <qppcad/python/python_console_widget.hpp>
#include <qppcad/ui/qextended_action.hpp>
#include <QMainWindow>
#include <QShortcut>
#include <QApplication>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QMimeData>
#include <QFileDialog>

namespace qpp {

  namespace cad {

    class main_window : public QMainWindow {
        Q_OBJECT

      private:
        int p_elapsed_time_in_event_loop{0};

      public:
        explicit main_window(QWidget *parent = 0);
        ~main_window();
        void init_base_shortcuts();
        void init_menus();
        void init_widgets();
        void init_layouts();
        void change_camera_buttons_visible(bool cart_c, bool cell_c);

        //Widgets
        QWidget *main_widget;
        QWidget *tool_panel_widget;
        QHBoxLayout *tool_panel_layout;

        QLabel *tp_ws_selector_label;
        QComboBox *tp_ws_selector;
        QPushButton *tp_add_ws;
        QPushButton *tp_rnm_ws;
        QPushButton *tp_rm_ws;
        QCheckBox *tp_show_obj_insp;
        QCheckBox *tp_show_gizmo;
        QPushButton *tp_print_screen;

        QFrame *tp_edit_mode_start;
        QButtonGroup *tp_edit_mode;
        QPushButton *tp_edit_mode_item;
        QPushButton *tp_edit_mode_content;
        QFrame *tp_edit_mode_end;

        int tp_button_width{36};
        int tp_button_height{30};

        QPushButton *tp_camera_x;
        QPushButton *tp_camera_y;
        QPushButton *tp_camera_z;
        QPushButton *tp_camera_cart_c;
        QPushButton *tp_camera_a;
        QPushButton *tp_camera_b;
        QPushButton *tp_camera_c;

        QPushButton *tp_measure_dist;
        QPushButton *tp_measure_angle;

        QWidget *ws_viewer_placeholder;
        QWidget *obj_inst_placeholder;

        python_console_widget_t *py_console_widget;
        ws_viewer_widget_t *ws_viewer_widget;
        object_inspector_widget_t *obj_insp_widget;
        //End of widgets

        //Menus
        QMenu *file_menu;
        QAction *file_menu_new_ws;
        QAction *file_menu_open_ws;

        QMenu *file_menu_import_as_new_ws;
        std::map<size_t, QMenu*> file_menu_import_as_new_ws_menus;
        std::vector<qextended_action*> file_menu_import_as_new_ws_acts;

        QMenu *file_menu_import_to_cur_ws;
        std::map<size_t, QMenu*> file_menu_import_to_cur_ws_menus;
        std::vector<qextended_action*> file_menu_import_to_cur_ws_acts;

        QMenu *file_menu_export_sel_as;
        std::map<size_t, QMenu*> file_menu_export_sel_as_menus;
        std::vector<qextended_action*> file_menu_export_sel_as_acts;

        QAction *file_menu_save_ws;
        QAction *file_menu_save_ws_as;
        QMenu *file_menu_recent_files;
        std::array<QAction*, max_recent_files> file_menu_recent_entries;
        QAction *file_menu_close_app;

        QMenu *edit_menu;
        QAction *edit_menu_undo;
        QAction *edit_menu_redo;
        QAction *edit_menu_switch_ws_edit_mode;
        QAction *edit_menu_console;
        QAction *edit_menu_settings;
        QAction *edit_menu_ws_settings;

        QMenu *edit_menu_debug;
        QAction *edit_menu_toggle_debug_info;
        QAction *edit_menu_toggle_debug_tws_tree;
        QAction *edit_menu_toggle_sel_deque;

        QMenu *edit_menu_selection;
        QAction *edit_menu_selection_select_all;
        QAction *edit_menu_selection_unselect_all;
        QAction *edit_menu_selection_invert;

        QMenu *tools_menu;
        std::map<size_t, QMenu*> tools_menu_groups;
        std::vector<qextended_action*> tools_menu_actions;

        QMenu *help_menu;
        QAction *help_menu_about;
        //end of menus

        //Layouts
        QGridLayout *layout_tools_main_window;
        QVBoxLayout *main_layout;
        QSplitter   *layout_ws_viewer_obj_insp;
        QWidget *widget_ws_viewer_py_console;
        QSplitter *splitter_ws_viewer_extended_editor;
        QSplitter *splitter_editor_py_console;
        QSplitter   *splitter_ws_viewer_py_console;
        //end of layouts

        //Shortcuts
        QShortcut       *sc_terminate_app;
        QShortcut       *sc_toggle_console;

        //End of shortcuts
      protected:

        void dragEnterEvent(QDragEnterEvent *event) override;
        void dragMoveEvent(QDragMoveEvent *event) override;
        void dragLeaveEvent(QDragLeaveEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

      public slots:
        void slot_shortcut_terminate_app();

        void wss_changed_slot();

        void ws_selector_selection_changed(int index);
        void tp_show_obj_insp_state_changed(int state);
        void tp_show_gizmo_state_changed(int state);

        void create_new_ws();
        void open_ws();
        void save_ws();
        void save_ws_as();
        void close_cur_ws();
        void rename_cur_ws();

        void cur_ws_changed();
        void cur_ws_selected_item_changed();
        void cur_ws_properties_changed();
        void cur_ws_edit_type_changed();
        void cur_ws_selected_atoms_list_selection_changed();

        void tp_dist_button_clicked(bool checked);
        void tp_angle_button_clicked(bool checked);
        void ws_edit_mode_selector_button_clicked(int id);

        void apply_camera_view_change(cam_target_view_t target_view);

        void toggle_ws_edit_mode();
        void start_update_cycle();
        void stop_update_cycle();

        void action_select_all_content();
        void action_unselect_all_content();
        void action_invert_selected_content();
        void action_toggle_console();

        void rebuild_recent_files_menu();
        void recent_files_clicked();

        void build_bhv_menus_and_actions();
        void build_bhv_tools_menus();
        void action_bhv_tools_menus_clicked();
        void control_bhv_tools_menus_activity();
        void action_bhv_import_to_cur_workspace();
        void action_bhv_import_as_new_workspace();
        void action_bhv_export_selected();
        void control_bhv_menus_activity();

        void make_screenshot();

    };
  }

}

#endif
