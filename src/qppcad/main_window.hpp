#ifndef QPP_CAD_MAIN_WINDOW_H
#define QPP_CAD_MAIN_WINDOW_H

#include <qppcad/qppcad.hpp>
#include <QMainWindow>
#include <QShortcut>
#include <QApplication>
#include <QMenuBar>

namespace qpp {

  namespace cad {
    class main_window : public QMainWindow {
        Q_OBJECT

      public:
        explicit main_window(QWidget *parent = 0);
        ~main_window();
        void init_base_shortcuts();
        void init_menus();
        //Widgets
        QWidget *main_widget;

        //End of widgets

        //Menus
        QMenu *file_menu;
        QMenu *edit_menu;
        QMenu *help_menu;
        //end of menus

        //Shortcuts
         QShortcut       *sc_terminate_app;

        //End of shortcuts

      private slots:
        void slot_shortcut_terminate_app();
    };
  }

}

#endif
