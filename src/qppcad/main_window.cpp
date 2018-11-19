#include <qppcad/main_window.hpp>

using namespace qpp;
using namespace qpp::cad;

main_window::main_window(QWidget *parent) {
  main_widget = new QWidget;
  setCentralWidget(main_widget);
  init_base_shortcuts();
  init_menus();
}

main_window::~main_window() {

}

void main_window::init_base_shortcuts() {
  sc_terminate_app = new QShortcut(this);
  sc_terminate_app->setKey(Qt::Key_Escape);
  connect(sc_terminate_app, SIGNAL(activated()), this, SLOT(slot_shortcut_terminate_app()));
}

void main_window::init_menus() {
  file_menu = menuBar()->addMenu(tr("&File"));
  edit_menu = menuBar()->addMenu(tr("&Edit"));
  help_menu = menuBar()->addMenu(tr("&Help"));
}

void main_window::slot_shortcut_terminate_app() {
  QApplication::quit();
}
