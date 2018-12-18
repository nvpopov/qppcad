#include <qppcad/app_settings_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

app_settings_widget_t::app_settings_widget_t(QWidget *parent) : QDialog (parent) {

  setWindowTitle(tr("Application settings"));
  setMinimumWidth(350);
  settings_splitter = new QSplitter(this);
  settings_layout = new QVBoxLayout;
  setLayout(settings_layout);

  settings_entries = new QListWidget;
  settings_entries->addItem(tr("General"));
  settings_entries->addItem(tr("Appearance"));
  settings_entries->addItem(tr("Console"));
  settings_entries->setMaximumWidth(150);

  settings_stacked_pages = new QStackedWidget;

  settings_page_general = new QWidget;
  settings_page_general->setMinimumWidth(200);

  settings_page_appearance = new QWidget;
  settings_page_appearance->setMinimumWidth(200);

  settings_page_console = new QWidget;
  settings_page_console->setMinimumWidth(200);

  settings_stacked_pages->addWidget(settings_page_general);
  settings_stacked_pages->addWidget(settings_page_appearance);
  settings_stacked_pages->addWidget(settings_page_console);

  settings_layout->addWidget(settings_splitter);
  settings_splitter->addWidget(settings_entries);
  settings_splitter->addWidget(settings_stacked_pages);

  settings_splitter->setCollapsible(0, false);
  settings_splitter->setCollapsible(1, false);

  connect(settings_entries, SIGNAL(activated(int)),
             settings_stacked_pages, SLOT(setCurrentIndex(int)));

  settings_entries->setCurrentRow(0);

}
