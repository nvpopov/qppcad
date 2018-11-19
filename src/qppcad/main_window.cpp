#include <qppcad/main_window.hpp>

using namespace qpp;
using namespace qpp::cad;

main_window::main_window(QWidget *parent) {
  main_widget = new QWidget;

  //tool_panel_widget->setSizePolicy(QSizePolicy::)
  setCentralWidget(main_widget);
  init_base_shortcuts();
  init_menus();
  init_widgets();
  init_layouts();
}

main_window::~main_window() {

}

void main_window::init_base_shortcuts() {
  sc_terminate_app = new QShortcut(this);
  sc_terminate_app->setKey(Qt::Key_Escape);
  connect(sc_terminate_app, SIGNAL(activated()), this, SLOT(slot_shortcut_terminate_app()));
}

void main_window::init_menus() {

  file_menu  = menuBar()->addMenu(tr("&File"));

  act_new_ws = new QAction();
  act_new_ws->setText(tr("New workspace"));
  act_new_ws->setShortcut(QKeySequence(tr("Ctrl+n")));
  file_menu->addAction(act_new_ws);

  act_open_ws = new QAction();
  act_open_ws->setText(tr("Open workspace"));
  act_open_ws->setShortcut(QKeySequence(tr("Ctrl+o")));
  file_menu->addAction(act_open_ws);

  act_save_ws = new QAction();
  act_save_ws->setText(tr("Save workspace"));
  act_save_ws->setShortcut(QKeySequence(tr("Ctrl+s")));
  file_menu->addAction(act_save_ws);

  act_save_ws_as = new QAction();
  act_save_ws_as->setText(tr("Save workspace as"));
  file_menu->addAction(act_save_ws_as);

  act_close_app = new QAction();
  act_close_app->setText(tr("Close"));
  act_close_app->setShortcut(QKeySequence(tr("Ctrl+q")));
  file_menu->addAction(act_close_app);

  edit_menu  = menuBar()->addMenu(tr("&Edit"));
  tools_menu = menuBar()->addMenu(tr("&Tools"));
  help_menu  = menuBar()->addMenu(tr("&Help"));

}

void main_window::init_widgets() {

  tool_panel_widget = new QWidget;
  //tool_panel_widget->setStyleSheet("background-color:black;");
  tool_panel_widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  tool_panel_widget->setMinimumHeight(45);
  tool_panel_widget->setMaximumHeight(45);

  ws_viewer_widget = new ws_viewer_widget_t(this);

  obj_inst_placeholder = new QWidget;
  obj_inst_placeholder->setMaximumWidth(400);

}

void main_window::init_layouts() {

  main_layout = new QVBoxLayout;
  main_widget->setLayout(main_layout);
  main_layout->addWidget(tool_panel_widget);
  main_layout->setContentsMargins(0,0,0,0);

  layout_ws_viewer_obj_insp = new QSplitter(Qt::Horizontal);
  layout_ws_viewer_obj_insp->addWidget(ws_viewer_widget);
  layout_ws_viewer_obj_insp->addWidget(obj_inst_placeholder);

  main_layout->addWidget(layout_ws_viewer_obj_insp);
//  layout_tools_main_window = new QGridLayout;
//  layout_tools_main_window->setContentsMargins(0,0,0,0);
//  main_widget->setLayout(layout_tools_main_window);
//  layout_tools_main_window->addWidget(tool_panel_widget, 0, 0, 1, 2);

//  layout_ws_viewer_obj_insp = new QSplitter(Qt::Horizontal);
//  layout_tools_main_window->addLayout()
//  layout_tools_main_window->addWidget(ws_viewer_placeholder, 1, 0, 1, 1);
//  layout_tools_main_window->addWidget(obj_inst_placeholder, 1, 1, 1, 1);
}

void main_window::slot_shortcut_terminate_app() {
  QApplication::quit();
}
