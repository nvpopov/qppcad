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
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QFileDialog>
#include <qppcad/ws_viewer_widget.hpp>
#include <qppcad/object_inspector_widget.hpp>

namespace qpp {

  namespace cad {
    class main_window : public QMainWindow {
        Q_OBJECT

      public:
        explicit main_window(QWidget *parent = 0);
        ~main_window();
        void init_base_shortcuts();
        void init_menus();
        void init_widgets();
        void init_layouts();

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
        QWidget *ws_viewer_placeholder;
        QWidget *obj_inst_placeholder;
        ws_viewer_widget_t *ws_viewer_widget;
        object_inspector_widget_t *obj_insp_widget;
        //End of widgets

        //Menus
        QMenu *file_menu;
        QAction *act_new_ws;
        QAction *act_open_ws;

        QMenu *menu_import;

        QAction *menu_import_xyz;

        QMenu *menu_import_cp2k;
        QAction *menu_import_cp2k_output;

        QMenu *menu_import_vasp;
        QAction *menu_import_vasp_outcar;
        QAction *menu_import_vasp_poscar;

        QMenu *menu_import_firefly;
        QAction *menu_import_firefly_output;

        QAction *act_save_ws;
        QAction *act_save_ws_as;
        QAction *act_close_app;

        QMenu *edit_menu;
        QAction *act_undo;
        QAction *act_redo;
        QAction *act_settings;

        QMenu *tools_menu;
        QAction *act_sc_generator;

        QMenu *help_menu;
        QAction *act_about;
        //end of menus

        //Layouts
        QGridLayout *layout_tools_main_window;
        QVBoxLayout *main_layout;
        QSplitter   *layout_ws_viewer_obj_insp;
        //end of layouts

        //Shortcuts
        QShortcut       *sc_terminate_app;

        //End of shortcuts

      private slots:
        void slot_shortcut_terminate_app();
        void workspaces_changed_slot();
        void ws_selector_selection_changed(int index);
        void tp_show_obj_insp_state_changed(int state);
        void import_file(QString dialog_name, QString file_ext,  qc_file_fmt file_fmt);
        void create_new_workspace();
        void open_workspace();
        void save_workspace();
        void save_workspace_as();
        void close_current_workspace();
        void rename_current_workspace();
        void current_workspace_changed();
    };
  }

}

#endif
