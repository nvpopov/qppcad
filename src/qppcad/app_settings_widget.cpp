#include <qppcad/app_settings_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

app_settings_widget_t::app_settings_widget_t(QWidget *parent) : QDialog (parent) {

  setWindowTitle(tr("Application settings"));
  setMinimumWidth(350);
  settings_splitter_layout = new QHBoxLayout;
  settings_layout = new QVBoxLayout;
  setLayout(settings_layout);

  settings_entries = new QListWidget;
  settings_entries->addItem(tr("General"));
  settings_entries->addItem(tr("Appearance"));
  settings_entries->addItem(tr("Console"));
  settings_entries->addItem(tr("Periodic table"));
  settings_entries->setMaximumWidth(150);

  settings_stacked_pages = new QStackedWidget;
  settings_stacked_pages->setMinimumWidth(600);
  settings_stacked_pages->setMinimumHeight(500);

  settings_page_general = new QWidget;
  settings_page_general->setMinimumWidth(200);

  settings_page_appearance = new QWidget;
  settings_page_appearance->setMinimumWidth(200);

  settings_page_console = new QWidget;
  settings_page_console->setMinimumWidth(200);

  settings_page_ptable = new QWidget;
  settings_page_ptable->setMinimumWidth(200);

  settings_stacked_pages->addWidget(settings_page_general);
  settings_stacked_pages->addWidget(settings_page_appearance);
  settings_stacked_pages->addWidget(settings_page_console);
  settings_stacked_pages->addWidget(settings_page_ptable);

  settings_layout->addLayout(settings_splitter_layout);
  settings_splitter_layout->addWidget(settings_entries);
  settings_splitter_layout->addWidget(settings_stacked_pages);

//  settings_splitter->addWidget(settings_entries);
//  settings_splitter->addWidget(settings_stacked_pages);


  connect(settings_entries, SIGNAL(activated(int)),
             settings_stacked_pages, SLOT(setCurrentIndex(int)));

  settings_entries->setCurrentRow(0);

  settings_close = new QPushButton(tr("Close"));
  settings_close->setMaximumWidth(75);

  settings_save = new QPushButton(tr("Save"));
  settings_save->setMaximumWidth(75);

  settings_button_layout = new QHBoxLayout;
  settings_button_layout->setAlignment(Qt::AlignRight);
  //settings_button_layout->addStretch(0);
  settings_button_layout->addWidget(settings_close, 0, Qt::AlignRight);
  settings_button_layout->addWidget(settings_save, 0, Qt::AlignRight);
  settings_layout->addLayout(settings_button_layout);

  connect(settings_close, SIGNAL(clicked()), this, SLOT(accept()));
  connect(settings_save, SIGNAL(clicked()), this, SLOT(accept()));
}
