#include <qppcad/main_window.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

main_window::main_window(QWidget *parent) {
  main_widget = new QWidget;
  app_state_t* astate = app_state_t::get_inst();
  //tool_panel_widget->setSizePolicy(QSizePolicy::)
  setCentralWidget(main_widget);
  setMinimumHeight(300);
  setMinimumWidth(600);
  init_base_shortcuts();
  init_menus();
  init_widgets();
  init_layouts();
  bool connect_st = QObject::connect(astate->astate_evd,
                                     SIGNAL(workspaces_changed_signal()),
                                     this, SLOT(ws_selector_changed()));
  ws_selector_changed();
  //astate->log(fmt::format("Connection status: {}", connect_st));
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
  file_menu->addSeparator();
  file_menu->addAction(act_save_ws);

  act_save_ws_as = new QAction();
  act_save_ws_as->setText(tr("Save workspace as"));
  file_menu->addAction(act_save_ws_as);

  act_close_app = new QAction();
  act_close_app->setText(tr("Close"));
  act_close_app->setShortcut(QKeySequence(tr("Ctrl+q")));
  file_menu->addSeparator();
  file_menu->addAction(act_close_app);

  edit_menu  = menuBar()->addMenu(tr("&Edit"));
  act_undo = new QAction();
  act_undo->setText(tr("Undo"));
  act_undo->setShortcut(QKeySequence(tr("Ctrl+u")));
  edit_menu->addAction(act_undo);

  act_redo = new QAction();
  act_redo->setText(tr("Redo"));
  act_redo->setShortcut(QKeySequence(tr("Ctrl+r")));
  edit_menu->addAction(act_redo);

  act_settings = new QAction();
  act_settings->setText(tr("Settings"));
  edit_menu->addSeparator();
  edit_menu->addAction(act_settings);

  tools_menu = menuBar()->addMenu(tr("&Tools"));
  act_sc_generator = new QAction();
  act_sc_generator->setText(tr("Supercell generator"));
  tools_menu->addAction(act_sc_generator);

  help_menu  = menuBar()->addMenu(tr("&Help"));
  act_about = new QAction();
  act_about->setText(tr("About"));
  help_menu->addAction(act_about);
}

void main_window::init_widgets() {

  tool_panel_widget = new QWidget;
  //tool_panel_widget->setStyleSheet("background-color:black;");
  tool_panel_widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  tool_panel_widget->setMinimumHeight(5);
  //tool_panel_widget->setMaximumHeight(35);
  //tool_panel_widget->setStyleSheet("border-bottom: 1px solid gray;margin-bottom:0px;");
  tool_panel_widget->setStyleSheet("margin-bottom:0px; "
                                   "QLabel {color:white;}");

  tp_ws_selector = new QComboBox;
  QObject::connect(tp_ws_selector,
                   SIGNAL(currentIndexChanged(int)),
                   this,
                   SLOT(ws_selector_selection_changed(int)));
  //tp_ws_selector->setParent(tool_panel_widget);
  tp_ws_selector->setStyleSheet("padding:4px;");
  tp_ws_selector->setMinimumWidth(150);
  tp_ws_selector->setMinimumHeight(30);
  //  tp_ws_selector_label = new QLabel;
  //  tp_ws_selector_label->setText(tr("Current workspace:"));

  tp_add_ws = new QPushButton;
  tp_add_ws->setMinimumWidth(30);
  tp_add_ws->setMinimumHeight(30);
  tp_add_ws->setText("+");

  tp_rm_ws = new QPushButton;
  tp_rm_ws->setText("-");
  tp_rm_ws->setMinimumWidth(30);
  tp_rm_ws->setMinimumHeight(30);

  tp_rnm_ws = new QPushButton;
  tp_rnm_ws->setText("RN");
  tp_rnm_ws->setMinimumWidth(30);
  tp_rnm_ws->setMinimumHeight(30);

  ws_viewer_widget = new ws_viewer_widget_t(this);
  ws_viewer_widget->setStyleSheet("margin-top:0px;");

  obj_inst_placeholder = new QWidget;
  obj_inst_placeholder->setMaximumWidth(400);
  obj_inst_placeholder->setStyleSheet("margin-top:0px;");
}

void main_window::init_layouts() {

  main_layout = new QVBoxLayout;
  main_widget->setLayout(main_layout);
  main_layout->addWidget(tool_panel_widget);
  main_layout->setContentsMargins(0,0,0,0);

  layout_ws_viewer_obj_insp = new QSplitter(Qt::Horizontal);
  layout_ws_viewer_obj_insp->addWidget(ws_viewer_widget);
  layout_ws_viewer_obj_insp->addWidget(obj_inst_placeholder);
  layout_ws_viewer_obj_insp->setContentsMargins(0,0,0,0);
  layout_ws_viewer_obj_insp->setStyleSheet("margin-top:0px;");
  layout_ws_viewer_obj_insp->setHandleWidth(15);
  main_layout->addWidget(layout_ws_viewer_obj_insp);

  tool_panel_layout = new QHBoxLayout;
  tool_panel_widget->setLayout(tool_panel_layout);
  //tool_panel_layout->addWidget(tp_ws_selector_label, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_ws_selector, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_add_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_rm_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_rnm_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addStretch(1);
  //  layout_tools_main_window = new QGridLayout;
  //  layout_tools_main_window->setContentsMargins(0,0,0,0);
  //  main_widget->setLayout(layout_tools_main_window);
  //  layout_tools_main_window->addWidget(tool_panel_widget, 0, 0, 1, 2);

  //  layout_ws_viewer_obj_insp = new QSplitter(Qt::Horizontal);
  //  layout_tools_main_window->addLayout()
  //  layout_tools_main_window->addWidget(ws_viewer_placeholder, 1, 0, 1, 1);
  //  layout_tools_main_window->addWidget(obj_inst_placeholder, 1, 1, 1, 1);
}

void main_window::ws_selector_changed() {

  app_state_t* astate = app_state_t::get_inst();
  tp_ws_selector->clear();
  for (auto &ws : astate->ws_manager->m_ws) {
      QString dest = QString::fromStdString(ws->m_ws_name);
      tp_ws_selector->addItem(dest);
    }
  if (astate->ws_manager->has_wss()) {
      tp_ws_selector->setCurrentIndex(*(astate->ws_manager->get_current_id()));
    }
  //astate->log("main_window::ws_selector_changed()");

}

void main_window::ws_selector_selection_changed(int index) {
  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto current = astate->ws_manager->get_current_id();
      astate->log(fmt::format("swtiching ws: {}", index));
      if (current && index != *current) {
          astate->ws_manager->set_current(index);
          astate->make_viewport_dirty();
        }
    }
}

void main_window::slot_shortcut_terminate_app() {
  QApplication::quit();
}
