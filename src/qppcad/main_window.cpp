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
  bool connect_st1 = QObject::connect(astate->astate_evd,
                                      SIGNAL(workspaces_changed_signal()),
                                      this, SLOT(workspaces_changed_slot()));

  bool connect_st2 = QObject::connect(astate->astate_evd,
                                      SIGNAL(current_workspace_changed_signal()),
                                      this, SLOT(current_workspace_changed()));
  workspaces_changed_slot();
  current_workspace_changed();
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
  connect(act_new_ws, &QAction::triggered, this, &main_window::create_new_workspace);

  act_open_ws = new QAction();
  act_open_ws->setText(tr("Open workspace"));
  act_open_ws->setShortcut(QKeySequence(tr("Ctrl+o")));
  file_menu->addAction(act_open_ws);
  connect(act_open_ws, &QAction::triggered, this, &main_window::open_workspace);

  menu_import = file_menu->addMenu(tr("Import"));
  menu_import_xyz = new QAction;
  menu_import_xyz->setText("XYZ");
  menu_import->addAction(menu_import_xyz);
  connect(menu_import_xyz, &QAction::triggered, this,
          [this](){this->import_file("Import XYZ file", "*", qc_file_fmt::standart_xyz);});

  menu_import_cp2k = menu_import->addMenu("CP2K");
  menu_import_cp2k_output = new QAction;
  menu_import_cp2k_output->setText(tr("OUTPUT"));
  menu_import_cp2k->addAction(menu_import_cp2k_output);
  connect(menu_import_cp2k_output, &QAction::triggered, this,
          [this](){this->import_file("Import CP2K output", "*", qc_file_fmt::cp2k_output);});

  menu_import_firefly = menu_import->addMenu("Firefly");
  menu_import_firefly_output = new QAction;
  menu_import_firefly_output->setText(tr("OUTPUT"));
  menu_import_firefly->addAction(menu_import_firefly_output);
  connect(menu_import_firefly_output, &QAction::triggered, this,
          [this](){this->import_file("Import Firefly output", "*", qc_file_fmt::firefly_output);});

  menu_import_vasp = menu_import->addMenu("VASP");
  menu_import_vasp_poscar = new QAction;
  menu_import_vasp_poscar->setText(tr("POSCAR"));
  connect(menu_import_vasp_poscar,&QAction::triggered, this,
          [this](){this->import_file("Import VASP POSCAR", "*", qc_file_fmt::vasp_poscar);});

  menu_import_vasp->addAction(menu_import_vasp_poscar);
  menu_import_vasp_outcar = new QAction;
  menu_import_vasp_outcar->setText(tr("OUTCAR"));
  menu_import_vasp->addAction(menu_import_vasp_outcar);
  connect(menu_import_vasp_outcar,&QAction::triggered, this,
          [this](){this->import_file("Import VASP OUTCAR", "*", qc_file_fmt::vasp_outcar_md);});

  act_save_ws = new QAction();
  act_save_ws->setText(tr("Save workspace"));
  act_save_ws->setShortcut(QKeySequence(tr("Ctrl+s")));
  file_menu->addSeparator();
  file_menu->addAction(act_save_ws);
  connect(act_save_ws, &QAction::triggered, this, &main_window::save_workspace);

  act_save_ws_as = new QAction();
  act_save_ws_as->setText(tr("Save workspace as"));
  file_menu->addAction(act_save_ws_as);
  connect(act_save_ws_as, &QAction::triggered, this, &main_window::save_workspace_as);

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
  tools_menu_generators = tools_menu->addMenu(tr("Generators"));

  act_sc_generator = new QAction();
  act_sc_generator->setText(tr("Supercell generator"));
  tools_menu_generators->addAction(act_sc_generator);

  tools_quick_geom_export = tools_menu->addMenu(tr("Export selected geometry"));
  tools_quick_geom_export_xyz = new QAction;
  tools_quick_geom_export_xyz->setText("XYZ");
  tools_quick_geom_export->addAction(tools_quick_geom_export_xyz);

  tools_quick_geom_export_cp2k_coord = new QAction;
  tools_quick_geom_export_cp2k_coord->setText("CP2K coord section");
  tools_quick_geom_export->addAction(tools_quick_geom_export_cp2k_coord);

  tools_quick_geom_export_vasp_poscar = new QAction;
  tools_quick_geom_export_vasp_poscar->setText("VASP POSCAR");
  tools_quick_geom_export->addAction(tools_quick_geom_export_vasp_poscar);

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
  tool_panel_widget->setStyleSheet("padding-bottom:-5px; padding-top:-5px;"
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
  connect(tp_add_ws, &QPushButton::pressed, this, &main_window::create_new_workspace);

  tp_rm_ws = new QPushButton;
  tp_rm_ws->setText("-");
  tp_rm_ws->setMinimumWidth(30);
  tp_rm_ws->setMinimumHeight(30);
  connect(tp_rm_ws, &QPushButton::pressed, this, &main_window::close_current_workspace);

  tp_rnm_ws = new QPushButton;
  tp_rnm_ws->setText("RN");
  tp_rnm_ws->setMinimumWidth(30);
  tp_rnm_ws->setMinimumHeight(30);
  connect(tp_rnm_ws, &QPushButton::pressed, this, &main_window::rename_current_workspace);

  tp_show_obj_insp = new QCheckBox;
  tp_show_obj_insp->setCheckState(Qt::Checked);
  tp_show_obj_insp->setText("Inspector");
  tp_show_obj_insp->setMinimumHeight(30);
  QObject::connect(tp_show_obj_insp, SIGNAL(stateChanged(int)),
                   this, SLOT(tp_show_obj_insp_state_changed(int)));
  tp_show_obj_insp->setStyleSheet("border:1px solid gray; border-radius:2px; padding-left:5px; "
                                  "padding-right:5px; "
                                  "QCheckBox::indicator { width: 21px;height: 21px;}");

  tp_show_gizmo = new QCheckBox;
  tp_show_gizmo->setCheckState(Qt::Checked);
  tp_show_gizmo->setText("Gizmo");
  tp_show_gizmo->setMinimumHeight(30);
  tp_show_gizmo->setStyleSheet("border:1px solid gray; border-radius:2px; padding-left:5px; "
                               "padding-right:5px; "
                               "QCheckBox::indicator { width: 21px;height: 21px;}");
  QObject::connect(tp_show_gizmo, SIGNAL(stateChanged(int)),
                   this, SLOT(tp_show_gizmo_state_changed(int)));

  ws_viewer_widget = new ws_viewer_widget_t(this);
  ws_viewer_widget->setStyleSheet("margin-top:-15px;");

  obj_insp_widget = new object_inspector_widget_t();
  obj_insp_widget->setMaximumWidth(400);
  //obj_insp_widget->setStyleSheet("margin-top:-15px;");
}

void main_window::init_layouts() {

  main_layout = new QVBoxLayout;
  main_widget->setLayout(main_layout);
  main_layout->addWidget(tool_panel_widget);
  main_layout->setContentsMargins(0,0,0,0);
  main_layout->setSpacing(0);

  layout_ws_viewer_obj_insp = new QSplitter(Qt::Horizontal);
  layout_ws_viewer_obj_insp->addWidget(ws_viewer_widget);
  layout_ws_viewer_obj_insp->addWidget(obj_insp_widget);
  layout_ws_viewer_obj_insp->setContentsMargins(0,0,0,0);
  //layout_ws_viewer_obj_insp->setStyleSheet("margin-top:0px;");
  layout_ws_viewer_obj_insp->setHandleWidth(15);
  main_layout->addWidget(layout_ws_viewer_obj_insp);

  tool_panel_layout = new QHBoxLayout;
  tool_panel_widget->setLayout(tool_panel_layout);
  //tool_panel_layout->addWidget(tp_ws_selector_label, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_ws_selector, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_add_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_rm_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_rnm_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_show_obj_insp, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_show_gizmo, 0, Qt::AlignLeft);

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

void main_window::workspaces_changed_slot() {

  app_state_t* astate = app_state_t::get_inst();
  tp_ws_selector->clear();

  tp_ws_selector->blockSignals(true);

  if (astate->ws_manager->has_wss()) {
      tp_rm_ws->setEnabled(true);
      tp_rnm_ws->setEnabled(true);
      tp_show_gizmo->setEnabled(true);
      for (auto &ws : astate->ws_manager->m_ws) {
          QString dest = QString::fromStdString(ws->m_ws_name);
          tp_ws_selector->addItem(dest);
        }

      tp_ws_selector->setCurrentIndex(*(astate->ws_manager->get_current_id()));

    } else {
      tp_rm_ws->setEnabled(false);
      tp_rnm_ws->setEnabled(false);
      tp_show_gizmo->setEnabled(false);
    }

  tp_ws_selector->blockSignals(false);
  astate->log("main_window::workspaces_changed_slot()");

}

void main_window::ws_selector_selection_changed(int index) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto current = astate->ws_manager->get_current_id();
      astate->log(fmt::format("ws_selector_selection_changed index: {}, ws_cur_id: {}",
                              index, *current));
      if (current) {
          astate->ws_manager->set_current(index);
          astate->make_viewport_dirty();
        }
    }
}

void main_window::tp_show_obj_insp_state_changed(int state) {

  if (state == Qt::Checked) {
      obj_insp_widget->show();
    }

  if (state == Qt::Unchecked) {
      obj_insp_widget->hide();
    }
}

void main_window::tp_show_gizmo_state_changed(int state) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          cur_ws->m_gizmo->m_is_visible = state == Qt::Checked;
          astate->make_viewport_dirty();
        }
    }

}

void main_window::import_file(QString dialog_name,
                              QString file_ext,
                              qc_file_fmt file_fmt) {
  app_state_t* astate = app_state_t::get_inst();
  QString fileName = QFileDialog::getOpenFileName(this, dialog_name, file_ext);

  if (fileName != "") {
      astate->ws_manager->import_file_as_new_workspace(fileName.toStdString(), file_fmt);
      workspaces_changed_slot();
    }
}

void main_window::create_new_workspace() {
  app_state_t* astate = app_state_t::get_inst();
  astate->ws_manager->query_create_new_workspace(true);
  workspaces_changed_slot();
  astate->make_viewport_dirty();
}

void main_window::open_workspace() {
  app_state_t* astate = app_state_t::get_inst();
  QString file_name = QFileDialog::getOpenFileName(this, "Open qpp::cad workspace", "*.json");
  if (file_name != "") {
      astate->ws_manager->load_workspace_from_file(file_name.toStdString());
      workspaces_changed_slot();
    }
}

void main_window::save_workspace() {

  app_state_t* astate = app_state_t::get_inst();
  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          QFileInfo check_file(QString::fromStdString(cur_ws->m_fs_path));
          if (check_file.exists() && check_file.isFile() && cur_ws->m_fs_path != "") {
              cur_ws->save_workspace_to_json(cur_ws->m_fs_path);
            } else {
              QString file_name = QFileDialog::getSaveFileName(this, "Save qpp::cad workspace",
                                                               "*.json");
              if (file_name != "") {
                  cur_ws->save_workspace_to_json(file_name.toStdString());
                  cur_ws->m_fs_path = file_name.toStdString();
                }
            }
        }
    }
  current_workspace_changed();
}

void main_window::save_workspace_as() {

  app_state_t* astate = app_state_t::get_inst();
  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          QString file_name = QFileDialog::getSaveFileName(this, "Save qpp::cad workspace",
                                                           "*.json");
          if (file_name != "") {
              cur_ws->save_workspace_to_json(file_name.toStdString());
              cur_ws->m_fs_path = file_name.toStdString();
            }
        }
    }
  current_workspace_changed();
}

void main_window::close_current_workspace() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          QMessageBox::StandardButton reply;
          reply = QMessageBox::question(this, tr("Workspace -> Close"),
                                        tr("Do you really want to close the workspace?"),
                                        QMessageBox::Yes | QMessageBox::No);
          if (reply == QMessageBox::Yes) {

            }
          else if (reply == QMessageBox::No) {

            }
        }
    }
}

void main_window::rename_current_workspace() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          bool ok;
          QString text = QInputDialog::getText(this, tr("Workspace -> Rename"),
                                               tr("User name:"), QLineEdit::Normal,
                                               QString::fromStdString(cur_ws->m_ws_name), &ok);
          if (ok && text != "") {
              cur_ws->m_ws_name = text.toStdString();
              workspaces_changed_slot();
            }
        }
    }
}

void main_window::current_workspace_changed() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          tp_show_gizmo->setChecked(cur_ws->m_gizmo->m_is_visible);
          std::string title_text = fmt::format("qpp::cad [ws_name: {}] - [path: {}]",
                                               cur_ws->m_ws_name, cur_ws->m_fs_path);
          this->setWindowTitle(QString::fromStdString(title_text));
        }
      else {
          this->setWindowTitle("qpp::cad");
        }
    }

}

void main_window::slot_shortcut_terminate_app() {
  QApplication::quit();
}
