#ifndef QPP_CAD_MAIN_WINDOW_H
#define QPP_CAD_MAIN_WINDOW_H

#include <qppcad/qppcad.hpp>
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
#include <QFileDialog>
#include <qppcad/ws_viewer_widget.hpp>
#include <qppcad/object_inspector_widget.hpp>
#include <qppcad/super_cell_widget.hpp>
#include <qppcad/axial_scale_widget.hpp>
#include <qppcad/python_console_widget.hpp>

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
        QPushButton *tp_camera_a;
        QPushButton *tp_camera_b;
        QPushButton *tp_camera_c;

        QPushButton *tp_measure_dist;

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

        QMenu *file_menu_import;

        QAction *file_menu_import_xyz;

        QMenu *file_menu_import_cp2k;
        QAction *file_menu_import_cp2k_output;

        QMenu *file_menu_import_vasp;
        QAction *file_menu_import_vasp_outcar;
        QAction *file_menu_import_vasp_poscar;

        QMenu *file_menu_import_firefly;
        QAction *file_menu_import_firefly_output;

        QAction *file_menu_save_ws;
        QAction *file_menu_save_ws_as;
        QAction *file_menu_close_app;

        QMenu *edit_menu;
        QAction *edit_menu_undo;
        QAction *edit_menu_redo;
        QAction *edit_menu_switch_ws_edit_mode;
        QAction *edit_menu_console;
        QAction *edit_menu_settings;
        QAction *edit_menu_ws_settings;

        QMenu *edit_menu_selection;
        QAction *edit_menu_selection_select_all;
        QAction *edit_menu_selection_unselect_all;
        QAction *edit_menu_selection_invert;

        QMenu *tools_menu;
        QAction *tools_menu_axial_scale;
        QMenu *tools_menu_generators;
        QAction *tools_menu_sc_generator;
        QMenu *tools_menu_quick_geom_export;
        QAction *tools_menu_quick_geom_export_xyz;
        QAction *tools_menu_quick_geom_export_vasp_poscar;
        QAction *tools_menu_quick_geom_export_cp2k_coord;

        QMenu *help_menu;
        QAction *help_menu_about;
        //end of menus

        //Layouts
        QGridLayout *layout_tools_main_window;
        QVBoxLayout *main_layout;
        QSplitter   *layout_ws_viewer_obj_insp;
        QWidget *widget_ws_viewer_py_console;
        QSplitter   *splitter_ws_viewer_py_console;
        //end of layouts

        //Shortcuts
        QShortcut       *sc_terminate_app;

        //End of shortcuts

      private slots:
        void slot_shortcut_terminate_app();

        void wss_changed_slot();

        void ws_selector_selection_changed(int index);
        void tp_show_obj_insp_state_changed(int state);
        void tp_show_gizmo_state_changed(int state);

        void import_file(QString dialog_name, QString file_ext,  qc_file_fmt file_fmt);
        void export_selected_geometry(QString dialog_name, qc_file_fmt file_fmt);

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
        void ws_edit_mode_selector_button_clicked(int id);

        void apply_camera_view_change(cam_target_view_t target_view);

        void toggle_ws_edit_mode();
        void start_update_cycle();
        void stop_update_cycle();
        void dialog_supercell_generation();
        void dialog_axial_scale();

        void action_select_all_content();
        void action_unselect_all_content();
        void action_invert_selected_content();
        void action_toggle_console();

    };
  }

}

#endif
