#ifndef QPPCAD_APP_SETTINGS_WIDGET_H
#define QPPCAD_APP_SETTINGS_WIDGET_H
#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QSplitter>
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>

namespace qpp {

  namespace cad {

    class app_settings_widget_t : public QDialog {
        Q_OBJECT
      public:
        explicit app_settings_widget_t(QWidget *parent = 0);
        QVBoxLayout *settings_layout;
        QListWidget *settings_entries;
        QHBoxLayout *settings_splitter_layout;

        QHBoxLayout *settings_button_layout;
        QPushButton *settings_close;
        QPushButton *settings_save;

        QStackedWidget *settings_stacked_pages;
        QWidget *settings_page_general;
        QWidget *settings_page_appearance;
        QWidget *settings_page_console;
        QWidget *settings_page_ptable;
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
