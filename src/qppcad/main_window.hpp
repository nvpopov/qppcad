#ifndef QPP_CAD_MAIN_WINDOW_H
#define QPP_CAD_MAIN_WINDOW_H

#include <qppcad/qppcad.hpp>
#include <QMainWindow>
#include <QShortcut>
#include <QApplication>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <qppcad/ws_viewer_widget.hpp>

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
        QWidget *ws_viewer_placeholder;
        QWidget *obj_inst_placeholder;
        ws_viewer_widget_t *ws_viewer_widget;
        //End of widgets

        //Menus
        QMenu *file_menu;
        QAction *act_new_ws;
        QAction *act_open_ws;
        QAction *act_save_ws;
        QAction *act_save_ws_as;
        QAction *act_close_app;
        QMenu *edit_menu;
        QMenu *tools_menu;
        QMenu *help_menu;
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
    };
  }

}

#endif
