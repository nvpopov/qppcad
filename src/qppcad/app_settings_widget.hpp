#ifndef QPP_CAD_APP_SETTINGS_WIDGET_H
#define QPP_CAD_APP_SETTINGS_WIDGET_H
#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QSplitter>
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QListWidget>
#include <QStackedWidget>

namespace qpp {

  namespace cad {

    class app_settings_widget_t : public QDialog {
        Q_OBJECT
      public:
        explicit app_settings_widget_t(QWidget *parent = 0);
        QVBoxLayout *settings_layout;
        QListWidget *settings_entries;
        QSplitter *settings_splitter;

        QStackedWidget *settings_stacked_pages;
        QWidget *settings_page_general;
        QWidget *settings_page_appearance;
        QWidget *settings_page_console;

    };

  }

}


#endif
