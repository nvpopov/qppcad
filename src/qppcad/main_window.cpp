#include <qppcad/main_window.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/app_settings_widget.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>

using namespace qpp;
using namespace qpp::cad;

main_window::main_window(QWidget *parent) {

  main_widget = new QWidget;
  app_state_t* astate = app_state_t::get_inst();
  //tool_panel_widget->setSizePolicy(QSizePolicy::)
  setAcceptDrops(true);
  setCentralWidget(main_widget);
  setMinimumHeight(300);
  setMinimumWidth(600);
  init_base_shortcuts();
  init_menus();
  build_bhv_menus_and_actions();
  control_bhv_menus_activity();
  init_widgets();
  init_layouts();

  connect(astate->astate_evd, &app_state_event_disp_t::wss_changed_signal,
          this, &main_window::wss_changed_slot);

  connect(astate->astate_evd, &app_state_event_disp_t::cur_ws_changed_signal,
          this, &main_window::cur_ws_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_changed_signal,
          this, &main_window::cur_ws_selected_item_changed);

  connect(astate->astate_evd, &app_state_event_disp_t::cur_ws_edit_type_changed_signal,
          this, &main_window::cur_ws_edit_type_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_selection_changed_signal,
          this, &main_window::cur_ws_selected_atoms_list_selection_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::new_file_loaded_signal,
          this, &main_window::rebuild_recent_files_menu);

  wss_changed_slot();
  cur_ws_changed();
  cur_ws_edit_type_changed();

  setStyleSheet("QPushButton:checked{"
                "background-color: rgb(150, 150, 150);}");

}

main_window::~main_window() {

}

void main_window::init_base_shortcuts() {
  //  sc_terminate_app = new QShortcut(this);
  //  sc_terminate_app->setKey(Qt::Key_Escape);
  //  connect(sc_terminate_app, SIGNAL(activated()), this, SLOT(slot_shortcut_terminate_app()));

  sc_toggle_console = new QShortcut(this);
  sc_toggle_console->setKey(Qt::Key_QuoteLeft);
  connect(sc_toggle_console, &QShortcut::activated, this, &main_window::action_toggle_console);

}

void main_window::init_menus() {

  file_menu  = menuBar()->addMenu(tr("&File"));

  file_menu_new_ws = new QAction(this);
  file_menu_new_ws->setText(tr("New workspace"));
  file_menu_new_ws->setShortcut(QKeySequence(tr("Ctrl+n")));
  file_menu->addAction(file_menu_new_ws);
  connect(file_menu_new_ws, &QAction::triggered, this, &main_window::create_new_ws);

  file_menu_open_ws = new QAction(this);
  file_menu_open_ws->setText(tr("Open workspace"));
  file_menu_open_ws->setShortcut(QKeySequence(tr("Ctrl+o")));
  file_menu->addAction(file_menu_open_ws);
  connect(file_menu_open_ws, &QAction::triggered, this, &main_window::open_ws);

  file_menu_save_ws = new QAction(this);
  file_menu_save_ws->setText(tr("Save workspace"));
  file_menu_save_ws->setShortcut(QKeySequence(tr("Ctrl+s")));
  file_menu->addSeparator();
  file_menu->addAction(file_menu_save_ws);
  connect(file_menu_save_ws, &QAction::triggered, this, &main_window::save_ws);

  file_menu_save_ws_as = new QAction(this);
  file_menu_save_ws_as->setText(tr("Save workspace as"));
  file_menu->addAction(file_menu_save_ws_as);
  connect(file_menu_save_ws_as, &QAction::triggered, this, &main_window::save_ws_as);

  file_menu->addSeparator();
  file_menu_import_as_new_ws = file_menu->addMenu(tr("Import as new workspace"));
  file_menu_import_to_cur_ws = file_menu->addMenu(tr("Import to current workspace"));
  file_menu_export_sel_as = file_menu->addMenu(tr("Export selected item"));

  file_menu_recent_files = file_menu->addMenu(tr("Recent files"));

  for (int i = 0; i < qpp::cad::max_recent_files; i++) {
      QAction *recent_act = new QAction(file_menu_recent_files);
      file_menu_recent_entries[i] = recent_act;
      file_menu_recent_files->addAction(recent_act);
      connect(recent_act, &QAction::triggered, this,
              &main_window::recent_files_clicked);
    }

  file_menu_close_app = new QAction(this);
  file_menu_close_app->setText(tr("Close"));
  file_menu_close_app->setShortcut(QKeySequence(tr("Ctrl+q")));
  file_menu->addSeparator();
  file_menu->addAction(file_menu_close_app);
  connect(file_menu_close_app, &QAction::triggered, this,
          &main_window::slot_shortcut_terminate_app);

  edit_menu  = menuBar()->addMenu(tr("&Edit"));
  edit_menu_undo = new QAction(this);
  edit_menu_undo->setText(tr("Undo"));
  edit_menu_undo->setShortcut(QKeySequence(tr("Ctrl+u")));
  edit_menu->addAction(edit_menu_undo);

  edit_menu_redo = new QAction(this);
  edit_menu_redo->setText(tr("Redo"));
  edit_menu_redo->setShortcut(QKeySequence(tr("Ctrl+r")));
  edit_menu->addAction(edit_menu_redo);

  edit_menu->addSeparator();

  //Selection menu
  edit_menu_selection = edit_menu->addMenu(tr("Content selection"));
  edit_menu_selection_select_all = new QAction(this);
  edit_menu_selection_select_all->setText(tr("Select all"));
  edit_menu_selection_select_all->setShortcut(QKeySequence(tr("ctrl+a")));
  edit_menu_selection->addAction(edit_menu_selection_select_all);
  connect(edit_menu_selection_select_all, &QAction::triggered,
          this, &main_window::action_select_all_content);

  edit_menu_selection_unselect_all = new QAction(this);
  edit_menu_selection_unselect_all->setText(tr("Unselect all"));
  edit_menu_selection_unselect_all->setShortcut(QKeySequence(tr("shift+a")));
  edit_menu_selection->addAction(edit_menu_selection_unselect_all);
  connect(edit_menu_selection_unselect_all, &QAction::triggered,
          this, &main_window::action_unselect_all_content);

  edit_menu_selection_invert = new QAction(this);
  edit_menu_selection_invert->setText(tr("Invert selection"));
  edit_menu_selection_invert->setShortcut(QKeySequence(tr("ctrl+i")));
  edit_menu_selection->addAction(edit_menu_selection_invert);
  connect(edit_menu_selection_invert, &QAction::triggered,
          this, &main_window::action_invert_selected_content);
  //End of selection menu

  edit_menu_switch_ws_edit_mode = new QAction(this);
  edit_menu_switch_ws_edit_mode->setText(tr("Switch edit mode"));
  edit_menu_switch_ws_edit_mode->setShortcut(Qt::Key::Key_Tab);
  connect(edit_menu_switch_ws_edit_mode, SIGNAL(triggered()), this, SLOT(toggle_ws_edit_mode()));
  edit_menu->addAction(edit_menu_switch_ws_edit_mode);

  edit_menu->addSeparator();

  edit_menu_settings = new QAction(this);
  edit_menu_settings->setText(tr("Settings"));
  connect(edit_menu_settings, &QAction::triggered,[](){app_settings_widget_t aset;aset.exec();} );

  edit_menu_ws_settings = new QAction(this);
  edit_menu_ws_settings->setText(tr("Workspace settings"));
  edit_menu->addSeparator();
  edit_menu->addAction(edit_menu_settings);
  edit_menu->addAction(edit_menu_ws_settings);

  edit_menu_console = new QAction(this);
  edit_menu_console->setText(tr("Console"));
  //edit_menu_console->setCheckable(true);
  edit_menu_console->setShortcut(QKeySequence(tr("~")));
  edit_menu_console->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
  connect(edit_menu_console, &QAction::triggered,
          this, &main_window::action_toggle_console);
  edit_menu->addAction(edit_menu_console);

  edit_menu_debug = edit_menu->addMenu(tr("Debug"));

  edit_menu_toggle_debug_info = new QAction(this);
  edit_menu_toggle_debug_info->setText(tr("Show frame info"));
  edit_menu_toggle_debug_info->setCheckable(true);
  edit_menu_debug->addAction(edit_menu_toggle_debug_info);
  connect(edit_menu_toggle_debug_info, &QAction::toggled,
          [](bool checked){
      app_state_t* astate = app_state_t::get_inst();
      astate->m_show_debug_frame_stats = checked;
      astate->make_viewport_dirty();
    });

  edit_menu_toggle_debug_tws_tree = new QAction(this);
  edit_menu_toggle_debug_tws_tree->setText(tr("Render tws-tree"));
  edit_menu_toggle_debug_tws_tree->setCheckable(true);
  edit_menu_debug->addAction(edit_menu_toggle_debug_tws_tree);
  connect(edit_menu_toggle_debug_tws_tree, &QAction::toggled,
          [](bool checked){
      app_state_t* astate = app_state_t::get_inst();
      astate->m_debug_show_tws_tree = checked;
      astate->make_viewport_dirty();
    });

  edit_menu_toggle_sel_deque = new QAction(this);
  edit_menu_toggle_sel_deque->setText(tr("Show selection deque"));
  edit_menu_toggle_sel_deque->setCheckable(true);
  edit_menu_debug->addAction(edit_menu_toggle_sel_deque);
  connect(edit_menu_toggle_debug_tws_tree, &QAction::toggled,
          [](bool checked){
      app_state_t* astate = app_state_t::get_inst();
      astate->m_debug_show_sel_deque = checked;
      astate->make_viewport_dirty();
    });

  tools_menu = menuBar()->addMenu(tr("&Tools"));
  tools_menu_generators = tools_menu->addMenu(tr("Generators"));

  tools_menu_axial_scale = new QAction(this);
  tools_menu_axial_scale->setText(tr("Axial scale"));
  tools_menu->addAction(tools_menu_axial_scale);
  connect(tools_menu_axial_scale, &QAction::triggered, this, &main_window::dialog_axial_scale);

  tools_menu_sc_generator = new QAction(this);
  tools_menu_sc_generator->setText(tr("Supercell generator"));
  tools_menu_generators->addAction(tools_menu_sc_generator);
  connect(tools_menu_sc_generator, &QAction::triggered, this,
          &main_window::dialog_supercell_generation);

  help_menu  = menuBar()->addMenu(tr("&Help"));
  help_menu_about = new QAction(this);
  help_menu_about->setText(tr("About"));
  help_menu->addAction(help_menu_about);

}

void main_window::init_widgets() {

  tool_panel_widget = new QWidget;
  //tool_panel_widget->setStyleSheet("background-color:black;");
  tool_panel_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  tool_panel_widget->setMaximumHeight(45);
  //tool_panel_widget->setStyleSheet("border-bottom: 1px solid gray;margin-bottom:0px;");
  tool_panel_widget->setStyleSheet("padding-bottom:-5px; padding-top:-5px;"
                                   "QLabel {color:white;}");

  tp_ws_selector = new QComboBox;
  QObject::connect(tp_ws_selector,
                   SIGNAL(currentIndexChanged(int)),
                   this,
                   SLOT(ws_selector_selection_changed(int)));

  tp_ws_selector->setStyleSheet("padding:4px;");
  tp_ws_selector->setMinimumWidth(150);
  tp_ws_selector->setMinimumHeight(tp_button_height);

  tp_add_ws = new QPushButton;
  tp_add_ws->setMinimumWidth(30);
  tp_add_ws->setMinimumHeight(tp_button_height);
  tp_add_ws->setText("+");
  connect(tp_add_ws, &QPushButton::pressed, this, &main_window::create_new_ws);

  tp_rm_ws = new QPushButton;
  tp_rm_ws->setText("-");
  tp_rm_ws->setMinimumWidth(30);
  tp_rm_ws->setMinimumHeight(tp_button_height);
  connect(tp_rm_ws, &QPushButton::pressed, this, &main_window::close_cur_ws);

  tp_rnm_ws = new QPushButton;
  tp_rnm_ws->setText("RN");
  tp_rnm_ws->setMinimumWidth(30);
  tp_rnm_ws->setMinimumHeight(tp_button_height);
  connect(tp_rnm_ws, &QPushButton::pressed, this, &main_window::rename_cur_ws);

  tp_show_obj_insp = new QCheckBox;
  tp_show_obj_insp->setCheckState(Qt::Checked);
  tp_show_obj_insp->setText("INS");
  tp_show_obj_insp->setMinimumHeight(tp_button_height);
  QObject::connect(tp_show_obj_insp,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(tp_show_obj_insp_state_changed(int)));

  tp_show_obj_insp->setStyleSheet("border:1px solid gray; border-radius:2px; padding-left:5px; "
                                  "padding-right:5px; "
                                  "QCheckBox::indicator { width: 21px;height: 21px;}");

  tp_show_gizmo = new QCheckBox;
  tp_show_gizmo->setCheckState(Qt::Checked);
  tp_show_gizmo->setText("GZM");
  tp_show_gizmo->setMinimumHeight(tp_button_height);
  tp_show_gizmo->setStyleSheet("border:1px solid gray; border-radius:2px; padding-left:5px; "
                               "padding-right:5px; "
                               "QCheckBox::indicator { width: 21px;height: 21px;}");
  QObject::connect(tp_show_gizmo, SIGNAL(stateChanged(int)),
                   this, SLOT(tp_show_gizmo_state_changed(int)));

  //  tp_show_node_editor = new QCheckBox;
  //  tp_show_node_editor->setCheckState(Qt::Unchecked);
  //  tp_show_node_editor->setText("NODES");
  //  tp_show_node_editor->setMinimumHeight(tp_button_height);
  //  tp_show_node_editor->setStyleSheet("border:1px solid gray; border-radius:2px; padding-left:5px;"
  //                                     "padding-right:5px; "
  //                                     "QCheckBox::indicator { width: 21px;height: 21px;}");

  tp_edit_mode = new QButtonGroup;
  tp_edit_mode->setExclusive(true);
  QObject::connect(tp_edit_mode, SIGNAL(buttonClicked(int)),
                   this, SLOT(ws_edit_mode_selector_button_clicked(int)));

  tp_edit_mode_item = new QPushButton;
  tp_edit_mode_item->setText(tr("ITM"));
  tp_edit_mode_item->setMinimumWidth(40);
  tp_edit_mode_item->setMinimumHeight(tp_button_height);
  tp_edit_mode_item->setCheckable(true);

  tp_edit_mode_content= new QPushButton;
  tp_edit_mode_content->setText(tr("CNT"));
  tp_edit_mode_content->setMinimumWidth(40);
  tp_edit_mode_content->setMinimumHeight(tp_button_height);
  tp_edit_mode_content->setCheckable(true);

  tp_edit_mode->addButton(tp_edit_mode_item, 0);
  tp_edit_mode->addButton(tp_edit_mode_content, 1);

  tp_edit_mode_start = new QFrame;
  tp_edit_mode_start->setFrameShape(QFrame::VLine);
  tp_edit_mode_start->setFrameShadow(QFrame::Sunken);
  tp_edit_mode_start->setFixedWidth(2);
  tp_edit_mode_start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

  tp_edit_mode_end = new QFrame;
  tp_edit_mode_end->setFrameShape(QFrame::VLine);
  tp_edit_mode_end->setFrameShadow(QFrame::Sunken);
  tp_edit_mode_end->setFixedWidth(2);
  tp_edit_mode_end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);


  tp_camera_x = new QPushButton(tr("C:X"));
  tp_camera_x->setMinimumWidth(34);
  tp_camera_x->setMinimumHeight(tp_button_height);
  connect(tp_camera_x, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view_t::tv_x_axis);});

  tp_camera_y = new QPushButton(tr("C:Y"));
  tp_camera_y->setMinimumWidth(34);
  tp_camera_y->setMinimumHeight(tp_button_height);
  connect(tp_camera_y, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view_t::tv_y_axis);});

  tp_camera_z = new QPushButton(tr("C:Z"));
  tp_camera_z->setMinimumWidth(34);
  tp_camera_z->setMinimumHeight(tp_button_height);
  connect(tp_camera_z, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view_t::tv_z_axis);});

  tp_camera_cart_c = new QPushButton(tr("C:C"));
  tp_camera_cart_c->setMinimumWidth(34);
  tp_camera_cart_c->setMinimumHeight(tp_button_height);
  connect(tp_camera_cart_c, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view_t::tv_cart_center);});

  tp_camera_a = new QPushButton(tr("C:a"));
  tp_camera_a->setMinimumWidth(34);
  tp_camera_a->setMinimumHeight(tp_button_height);
  connect(tp_camera_a, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view_t::tv_a_axis);});

  tp_camera_b = new QPushButton(tr("C:b"));
  tp_camera_b->setMinimumWidth(34);
  tp_camera_b->setMinimumHeight(tp_button_height);
  connect(tp_camera_b, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view_t::tv_b_axis);});

  tp_camera_c = new QPushButton(tr("C:c"));
  tp_camera_c->setMinimumWidth(34);
  tp_camera_c->setMinimumHeight(tp_button_height);
  connect(tp_camera_c, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view_t::tv_c_axis);});

  change_camera_buttons_visible(false, false);

  tp_measure_dist = new QPushButton(tr("DIST"));
  tp_measure_dist->setMinimumWidth(40);
  tp_measure_dist->setMinimumHeight(tp_button_height);
  tp_measure_dist->setCheckable(true);
  connect(tp_measure_dist, &QPushButton::toggled, this, &main_window::tp_dist_button_clicked);

  tp_measure_angle = new QPushButton(tr("ANGLE"));
  tp_measure_angle->setMinimumWidth(60);
  tp_measure_angle->setMinimumHeight(tp_button_height);
  tp_measure_angle->setCheckable(true);
  connect(tp_measure_angle, &QPushButton::toggled, this, &main_window::tp_angle_button_clicked);

  ws_viewer_widget = new ws_viewer_widget_t(this);
  ws_viewer_widget->setStyleSheet("margin-top:-15px;");

  obj_insp_widget = new object_inspector_widget_t();
  obj_insp_widget->setMaximumWidth(400);
  obj_insp_widget->setMinimumWidth(200);

  py_console_widget = new python_console_widget_t(this);
  widget_ws_viewer_py_console = new QWidget();

}

void main_window::init_layouts() {

  main_layout = new QVBoxLayout;
  main_widget->setLayout(main_layout);
  main_layout->addWidget(tool_panel_widget);
  main_layout->setContentsMargins(0,0,0,0);
  main_layout->setSpacing(0);

  splitter_ws_viewer_py_console = new QSplitter(Qt::Vertical);
  splitter_ws_viewer_py_console->addWidget(ws_viewer_widget);
  splitter_ws_viewer_py_console->addWidget(py_console_widget);
  splitter_ws_viewer_py_console->setHandleWidth(15);
  splitter_ws_viewer_py_console->setSizes(QList<int>({3, 1}));
  py_console_widget->hide();
  splitter_ws_viewer_py_console->setCollapsible(1, false);
  splitter_ws_viewer_py_console->setCollapsible(0, false);

  layout_ws_viewer_obj_insp = new QSplitter(Qt::Horizontal);
  layout_ws_viewer_obj_insp->addWidget(splitter_ws_viewer_py_console);
  layout_ws_viewer_obj_insp->addWidget(obj_insp_widget);
  layout_ws_viewer_obj_insp->setContentsMargins(0,0,0,0);
  layout_ws_viewer_obj_insp->setCollapsible(1, false);

  layout_ws_viewer_obj_insp->setHandleWidth(15);
  main_layout->addWidget(layout_ws_viewer_obj_insp);

  tool_panel_layout = new QHBoxLayout;
  tool_panel_widget->setLayout(tool_panel_layout);

  tool_panel_layout->addWidget(tp_ws_selector, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_add_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_rm_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_rnm_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_show_obj_insp, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_show_gizmo, 0, Qt::AlignLeft);
  //  tool_panel_layout->addWidget(tp_show_node_editor, 0, Qt::AlignLeft);

  tool_panel_layout->addWidget(tp_edit_mode_start, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_edit_mode_item, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_edit_mode_content, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_edit_mode_end, 0, Qt::AlignLeft);

  tool_panel_layout->addWidget(tp_camera_x, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_y, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_z, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_cart_c, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_a, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_b, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_c, 0, Qt::AlignLeft);

  tool_panel_layout->addWidget(tp_measure_dist, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_measure_angle, 0, Qt::AlignLeft);

  tool_panel_layout->addStretch(1);

}

void main_window::change_camera_buttons_visible(bool cart_c, bool cell_c) {
  tp_camera_x->setVisible(cart_c);
  tp_camera_y->setVisible(cart_c);
  tp_camera_z->setVisible(cart_c);
  tp_camera_cart_c->setVisible(cart_c);
  tp_camera_a->setVisible(cell_c);
  tp_camera_b->setVisible(cell_c);
  tp_camera_c->setVisible(cell_c);
}

void main_window::dragEnterEvent(QDragEnterEvent *event) {
  event->acceptProposedAction();
}

void main_window::dragMoveEvent(QDragMoveEvent *event) {
  event->acceptProposedAction();
}

void main_window::dragLeaveEvent(QDragLeaveEvent *event) {
  event->accept();
}

void main_window::dropEvent(QDropEvent *event) {

  const QMimeData *mimeData = event->mimeData();
  if (mimeData && mimeData->hasUrls()) {
      app_state_t* astate = app_state_t::get_inst();
      QList<QUrl> urlList = mimeData->urls();
      for (int i = 0; i < urlList.size(); i++) {
          QString native_path = urlList.at(i).toLocalFile();
          std::string native_path_str = native_path.toStdString();
          astate->get_inst()->log(fmt::format("DRAG EN DROP EVENT {} {}", i, native_path_str));
          astate->ws_manager->load_from_file_autodeduce(native_path_str);
        }
    }
}

void main_window::resizeEvent(QResizeEvent *event) {

  if (event->size().width() < 800) {
      tp_add_ws->hide();
      tp_rnm_ws->hide();
      tp_rm_ws->hide();
      tp_show_gizmo->hide();
      // tp_show_node_editor->hide();
    } else {
      tp_add_ws->show();
      tp_rnm_ws->show();
      tp_rm_ws->show();
      tp_show_gizmo->show();
      // tp_show_node_editor->show();
    }

  QMainWindow::resizeEvent(event);
}

void main_window::wss_changed_slot() {

  app_state_t* astate = app_state_t::get_inst();

  control_bhv_menus_activity();

  tp_ws_selector->blockSignals(true);

  tp_ws_selector->clear();

  if (astate->ws_manager->has_wss()) {
      tp_rm_ws->setEnabled(true);
      tp_rnm_ws->setEnabled(true);
      tp_show_gizmo->setEnabled(true);
      for (auto ws : astate->ws_manager->m_ws) {
          QString dest = QString::fromStdString(ws->m_ws_name);
          //astate->log(fmt::format("DEBUG COMBOBOX: add new item {}", dest.toStdString()));
          tp_ws_selector->addItem(dest);
        }

      tp_ws_selector->setCurrentIndex(*(astate->ws_manager->get_cur_id()));

    } else {
      tp_rm_ws->setEnabled(false);
      tp_rnm_ws->setEnabled(false);
      tp_show_gizmo->setEnabled(false);
    }

  tp_ws_selector->blockSignals(false);
  astate->log(fmt::format("main_window::workspaces_changed_slot(), total ws = {}",
                          astate->ws_manager->m_ws.size()));

}

void main_window::ws_selector_selection_changed(int index) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto current = astate->ws_manager->get_cur_id();
      astate->log(fmt::format("ws_selector_selection_changed index: {}, ws_cur_id: {}",
                              index, *current));
      if (current) {
          astate->ws_manager->set_cur_id(index);
          astate->make_viewport_dirty();
        }
    }
}

void main_window::tp_show_obj_insp_state_changed(int state) {

  app_state_t* astate = app_state_t::get_inst();

  if (state == Qt::Checked) {
      obj_insp_widget->show();
      astate->m_show_object_inspector = true;
    }

  if (state == Qt::Unchecked) {
      obj_insp_widget->hide();
      astate->m_show_object_inspector = false;
    }
}

void main_window::tp_show_gizmo_state_changed(int state) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          cur_ws->m_gizmo->m_is_visible = state == Qt::Checked;
          astate->make_viewport_dirty();
          std::cout << "STATE DEBUG " << state << std::endl;
        }
    }

}

void main_window::create_new_ws() {
  app_state_t* astate = app_state_t::get_inst();
  astate->ws_manager->create_new_ws(true);
  wss_changed_slot();
  astate->make_viewport_dirty();
}

void main_window::open_ws() {
  app_state_t* astate = app_state_t::get_inst();
  QString file_name = QFileDialog::getOpenFileName(this, "Open qpp::cad workspace", "*.json");
  if (file_name != "") {
      astate->ws_manager->load_from_file(file_name.toStdString(), qc_file_fmt::qppcad_json);
      wss_changed_slot();
    }
}

void main_window::save_ws() {

  app_state_t* astate = app_state_t::get_inst();

  stop_update_cycle();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          QFileInfo check_file(QString::fromStdString(cur_ws->m_fs_path));
          if (check_file.exists() && check_file.isFile() && cur_ws->m_fs_path != "" &&
              !cur_ws->m_is_ws_imported) {
              cur_ws->m_is_ws_imported = false;
              cur_ws->save_ws_to_json(cur_ws->m_fs_path);
            } else {
              QString file_name = QFileDialog::getSaveFileName(this, "Save qpp::cad workspace",
                                                               "*.json");
              if (file_name != "") {
                  cur_ws->save_ws_to_json(file_name.toStdString());
                  cur_ws->m_is_ws_imported = false;
                  cur_ws->m_fs_path = file_name.toStdString();
                }
            }
        }
    }

  start_update_cycle();

  cur_ws_changed();
}

void main_window::save_ws_as() {

  app_state_t* astate = app_state_t::get_inst();

  stop_update_cycle();


  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          QString file_name = QFileDialog::getSaveFileName(this, "Save qpp::cad workspace",
                                                           "*.json");
          if (file_name != "") {
              cur_ws->save_ws_to_json(file_name.toStdString());
              cur_ws->m_is_ws_imported = false;
              cur_ws->m_fs_path = file_name.toStdString();
            }
        }
    }
  cur_ws_changed();

  start_update_cycle();

}

void main_window::close_cur_ws() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
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

void main_window::rename_cur_ws() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          bool ok;
          QString text = QInputDialog::getText(this, tr("Workspace -> Rename"),
                                               tr("User name:"), QLineEdit::Normal,
                                               QString::fromStdString(cur_ws->m_ws_name), &ok);
          if (ok && text != "") {
              cur_ws->m_ws_name = text.toStdString();
              wss_changed_slot();
            }
        }
    }
}

void main_window::cur_ws_changed() {

  app_state_t* astate = app_state_t::get_inst();

  change_camera_buttons_visible(false, false);

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          tp_show_gizmo->setChecked(cur_ws->m_gizmo->m_is_visible);
          std::string title_text = fmt::format("qpp::cad [ws_name: {}] - [path: {}]",
                                               cur_ws->m_ws_name, cur_ws->m_fs_path);
          this->setWindowTitle(QString::fromStdString(title_text));
          tools_menu_sc_generator->setEnabled(true);
        } else {
          this->setWindowTitle("qpp::cad");
        }
    } else {
      tools_menu_sc_generator->setEnabled(false);
      this->setWindowTitle("qpp::cad");
    }

  wss_changed_slot();
  cur_ws_properties_changed();
  cur_ws_selected_item_changed();
}

void main_window::cur_ws_selected_item_changed() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto cur_it_as_al = dynamic_cast<ws_atoms_list_t*>(cur_it);
          if (cur_it_as_al) {
              //tp_measure_dist->show();
              if (cur_it_as_al->m_geom->DIM == 3) change_camera_buttons_visible(false, true);
              else change_camera_buttons_visible(true, false);
            }
          else {
              change_camera_buttons_visible(false, false);
              //tp_measure_dist->hide();
            }
        }
    }

  cur_ws_edit_type_changed();
  control_bhv_menus_activity();
}

void main_window::cur_ws_properties_changed() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          bool check_t = cur_ws->m_edit_type == ws_edit_t::edit_item;
          tp_edit_mode_item->blockSignals(true);
          tp_edit_mode_content->blockSignals(true);
          tp_edit_mode_item->setChecked(check_t);
          tp_edit_mode_content->setChecked(!check_t);
          tp_edit_mode_item->blockSignals(false);
          tp_edit_mode_content->blockSignals(false);
        }
    }

  astate->make_viewport_dirty();
}

void main_window::cur_ws_edit_type_changed() {
  cur_ws_properties_changed();
  cur_ws_selected_atoms_list_selection_changed();
}

void main_window::cur_ws_selected_atoms_list_selection_changed() {

  app_state_t* astate = app_state_t::get_inst();

  bool need_to_hide_al_cntls{true};

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {

          auto cur_it = cur_ws->get_selected();
          auto cur_it_as_al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (cur_it_as_al) {
              if (cur_it_as_al->m_atom_idx_sel.size() == 2 &&
                  cur_ws->m_edit_type == ws_edit_t::edit_content) {
                  tp_measure_angle->hide();
                  tp_measure_dist->show();
                  need_to_hide_al_cntls = false;
                  auto it1 = cur_it_as_al->m_atom_idx_sel.begin();
                  auto it2 = ++(cur_it_as_al->m_atom_idx_sel.begin());

                  auto cur_sel = cur_it_as_al->m_measure->is_bond_measurement_exist(
                                   it1->m_atm, it2->m_atm, it1->m_idx, it2->m_idx);
                  tp_measure_dist->blockSignals(true);
                  tp_measure_dist->setChecked(cur_sel != std::nullopt);
                  tp_measure_dist->blockSignals(false);
                }

              if (cur_it_as_al->m_atom_idx_sel.size() == 3 &&
                  cur_ws->m_edit_type == ws_edit_t::edit_content &&
                  cur_it_as_al->m_atom_ord_sel.size() == 3) {
                  need_to_hide_al_cntls = false;

                  auto cur_sel = cur_it_as_al->m_measure->is_angle_measurement_exist(
                                   cur_it_as_al->m_atom_ord_sel[0].m_atm,
                      cur_it_as_al->m_atom_ord_sel[1].m_atm,
                      cur_it_as_al->m_atom_ord_sel[2].m_atm,
                      cur_it_as_al->m_atom_ord_sel[0].m_idx,
                      cur_it_as_al->m_atom_ord_sel[1].m_idx,
                      cur_it_as_al->m_atom_ord_sel[2].m_idx);

                  tp_measure_dist->hide();
                  tp_measure_angle->show();
                  tp_measure_angle->blockSignals(true);
                  tp_measure_angle->setChecked(cur_sel != std::nullopt);
                  tp_measure_angle->blockSignals(false);
                }

            }

        }
    }

  if (need_to_hide_al_cntls) {
      tp_measure_dist->hide();
      tp_measure_angle->hide();
    }

}

void main_window::tp_dist_button_clicked(bool checked) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {

          auto cur_it = cur_ws->get_selected();
          auto cur_it_as_al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (cur_it_as_al) {

              if (cur_it_as_al->m_atom_idx_sel.size() == 2 &&
                  cur_ws->m_edit_type == ws_edit_t::edit_content) {
                  tp_measure_dist->show();

                  auto it1 = cur_it_as_al->m_atom_idx_sel.begin();
                  auto it2 = ++(cur_it_as_al->m_atom_idx_sel.begin());

                  auto cur_sel = cur_it_as_al->m_measure->is_bond_measurement_exist(
                                   it1->m_atm, it2->m_atm, it1->m_idx, it2->m_idx);

                  if (checked) cur_it_as_al->m_measure->add_bond_measurement(
                        it1->m_atm, it2->m_atm,it1->m_idx, it2->m_idx);
                  else cur_it_as_al->m_measure->remove_bond_measurement(*cur_sel);
                }
            }

        }
    }

  astate->make_viewport_dirty();

}

void main_window::tp_angle_button_clicked(bool checked) {
  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {

          auto cur_it = cur_ws->get_selected();
          auto cur_it_as_al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (cur_it_as_al) {

              if (cur_it_as_al->m_atom_idx_sel.size() == 3 &&
                  cur_ws->m_edit_type == ws_edit_t::edit_content) {
                  tp_measure_angle->show();

                  auto cur_sel = cur_it_as_al->m_measure->is_angle_measurement_exist(
                                   cur_it_as_al->m_atom_ord_sel[0].m_atm,
                      cur_it_as_al->m_atom_ord_sel[1].m_atm,
                      cur_it_as_al->m_atom_ord_sel[2].m_atm,
                      cur_it_as_al->m_atom_ord_sel[0].m_idx,
                      cur_it_as_al->m_atom_ord_sel[1].m_idx,
                      cur_it_as_al->m_atom_ord_sel[2].m_idx);

                  if (checked) cur_it_as_al->m_measure->add_angle_measurement(
                        cur_it_as_al->m_atom_ord_sel[0].m_atm,
                      cur_it_as_al->m_atom_ord_sel[1].m_atm,
                      cur_it_as_al->m_atom_ord_sel[2].m_atm,
                      cur_it_as_al->m_atom_ord_sel[0].m_idx,
                      cur_it_as_al->m_atom_ord_sel[1].m_idx,
                      cur_it_as_al->m_atom_ord_sel[2].m_idx);

                  else cur_it_as_al->m_measure->remove_angle_measurement(*cur_sel);
                }
            }

        }
    }

  astate->make_viewport_dirty();
}

void main_window::ws_edit_mode_selector_button_clicked(int id) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          if (id == 0) cur_ws->m_edit_type = ws_edit_t::edit_item;
          else cur_ws->m_edit_type = ws_edit_t::edit_content;
        }
    }
  astate->astate_evd->cur_ws_edit_type_changed();
  cur_ws_properties_changed();

}

void main_window::apply_camera_view_change(cam_target_view_t target_view) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (al) {

              vector3<float> look_from;
              vector3<float> look_to;
              vector3<float> look_up{0.0, 1.0, 0.0};
              bool need_to_update_camera{false};

              al->target_view(target_view, look_from, look_to, look_up, need_to_update_camera);

              if (need_to_update_camera) {
                  astate->camera->m_view_point = look_from;
                  astate->camera->m_look_at = look_to;
                  astate->camera->m_look_up = look_up;
                  astate->camera->orthogonalize_gs();
                  astate->camera->update_camera();
                  //astate->make_viewport_dirty();
                }

            } // end of if (cur_it_as_al)

        }
    }

  astate->make_viewport_dirty();
}

void main_window::toggle_ws_edit_mode() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          cur_ws->toggle_edit_mode();
          cur_ws_changed();
        }
    }
}

void main_window::start_update_cycle() {

  if (ws_viewer_widget && ws_viewer_widget->m_update_timer) {
      ws_viewer_widget->m_update_timer->start();
    }

}

void main_window::stop_update_cycle() {

  if (ws_viewer_widget && ws_viewer_widget->m_update_timer) {
      p_elapsed_time_in_event_loop =  ws_viewer_widget->m_update_timer->remainingTime();
      ws_viewer_widget->m_update_timer->stop();
      ws_viewer_widget->m_update_timer->setInterval(p_elapsed_time_in_event_loop);
    }

}

void main_window::dialog_supercell_generation() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (al) {
              if (al->m_geom->DIM == 3) {
                  super_cell_widget_t scw;
                  int ret_code = scw.exec();
                  int rep_a = scw.get_replication_coeff(0);
                  int rep_b = scw.get_replication_coeff(1);
                  int rep_c = scw.get_replication_coeff(2);

                  if (ret_code == QDialog::Accepted && (rep_a + rep_b + rep_c >= 1)) {
                      al->make_super_cell(rep_a + 1, rep_b + 1, rep_c + 1);
                      astate->make_viewport_dirty();
                    }

                } else QMessageBox::warning(this, tr("Supercell generation"),
                                            tr("m_geom.DIM !=3"));

            } else QMessageBox::warning(this, tr("Supercell generation"),
                                        tr("ws_item.type != ws_atoms_list"));

        } else QMessageBox::warning(this, tr("Supercell generation"),
                                    tr("Workspace not select"));

    }

}

void main_window::dialog_axial_scale() {
  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (al) {
              if (al->m_geom->DIM == 3) {
                  axial_scale_widget_t asw;
                  int ret_code = asw.exec();
                  double sc_a = asw.get_scale_value(0);
                  double sc_b = asw.get_scale_value(1);
                  double sc_c = asw.get_scale_value(2);

                  if (ret_code == QDialog::Accepted) {
                      //  al->make_super_cell(rep_a + 1, rep_b + 1, rep_c + 1);
                      al->apply_axial_scale(float(sc_a), float(sc_b), float(sc_c));
                      astate->make_viewport_dirty();
                    }
                } else QMessageBox::warning(this, tr("Axial scale"), tr("m_geom.DIM !=3"));
            }
          else { // is not an atoms list
              QMessageBox::warning(this, tr("Axial scale"),
                                   tr("ws_item.type != ws_atoms_list"));
            }

        } else {
          QMessageBox::warning(this, tr("Axial scale"),
                               tr("Workspace not select"));
        }
    }
}

void main_window::action_select_all_content() {

  app_state_t* astate = app_state_t::get_inst();

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (cur_ws && cur_ws->m_edit_type == ws_edit_t::edit_content) {

      auto cur_it = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

      if (cur_it) {
          cur_it->select_atoms(true);
        }
    }

}

void main_window::action_unselect_all_content() {

  app_state_t* astate = app_state_t::get_inst();

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (cur_ws && cur_ws->m_edit_type == ws_edit_t::edit_content) {

      auto cur_it = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

      if (cur_it) {
          cur_it->select_atoms(false);
        }
    }
}

void main_window::action_invert_selected_content() {

  app_state_t* astate = app_state_t::get_inst();

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (cur_ws && cur_ws->m_edit_type == ws_edit_t::edit_content) {

      auto cur_it = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

      if (cur_it) {
          cur_it->invert_selected_atoms();
        }
    }
}

void main_window::action_toggle_console() {

  app_state_t* astate = app_state_t::get_inst();

  if (py_console_widget->isVisible()) {
      py_console_widget->hide();
      astate->m_show_console = true;
    } else {
      py_console_widget->show();
      py_console_widget->py_tedit->setFocus();
      astate->m_show_console = false;
    }
}

void main_window::rebuild_recent_files_menu() {

  if (!file_menu_recent_files) return;

  for (auto &elem : file_menu_recent_entries)
    elem->setVisible(false);

  app_state_t* astate = app_state_t::get_inst();
  for (int i = 0; i < astate->m_recent_files.size(); i++) {
      file_menu_recent_entries[i]->setVisible(true);
      file_menu_recent_entries[i]->setText(
            QString::fromStdString(fmt::format("{}) {} ({})",
                                               i,
                                               astate->m_recent_files[i].m_file_name,
                                               qc_file_fmt_helper::get_file_format_hint(
                                                 astate->m_recent_files[i].m_file_format)))
            );
    }

}

void main_window::recent_files_clicked() {

  int idx = -1;

  QObject* obj = sender();
  for (int i = 0 ; i < file_menu_recent_entries.size(); i++)
    if (file_menu_recent_entries[i]->isVisible() && file_menu_recent_entries[i] == obj) {
        idx = i;
      }

  if (idx != -1) {
      app_state_t* astate = app_state_t::get_inst();
      astate->ws_manager->load_from_file(astate->m_recent_files[idx].m_file_name,
                                         astate->m_recent_files[idx].m_file_format);
    }

}

void main_window::build_bhv_menus_and_actions() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_manager->m_bhv_mgr.get();

  if (!bhv_mgr) return;

  //init groups for IMPORTED TO WS
  for (auto &ff_grp : bhv_mgr->m_file_format_groups) {

      //spawn top-level menus
      QMenu *new_menu_as_new_ws =
          file_menu_import_as_new_ws->addMenu(QString::fromStdString(ff_grp.second.m_full_name));

      QMenu *new_menu_to_cur_ws =
          file_menu_import_to_cur_ws->addMenu(QString::fromStdString(ff_grp.second.m_full_name));

      QMenu *new_menu_export_selected =
          file_menu_export_sel_as->addMenu(QString::fromStdString(ff_grp.second.m_full_name));

      file_menu_import_as_new_ws_menus.emplace(ff_grp.first, new_menu_as_new_ws);
      file_menu_import_to_cur_ws_menus.emplace(ff_grp.first, new_menu_to_cur_ws);
      file_menu_export_sel_as_menus.emplace(ff_grp.first, new_menu_export_selected);

      //iterate over file formats from group
      for (auto &ff : ff_grp.second.m_ffs_lookup)

        for (size_t i = 0; i < bhv_mgr->m_ws_item_io.size(); i++) {

            //deduce import to current ws
            if (bhv_mgr->m_ws_item_io[i]->m_accepted_file_format == ff &&
                bhv_mgr->m_ws_item_io[i]->can_load() &&
                bhv_mgr->m_ws_item_io[i]->m_menu_occupier &&
                bhv_mgr->m_ws_item_io[i]->m_can_be_imported_to_ws) {
                qextended_action *new_act = new qextended_action(this);
                new_act->m_joined_data[0] = i;
                connect(new_act, &QAction::triggered,
                        this, &main_window::action_bhv_import_to_cur_workspace);
                new_act->setText(
                      QString::fromStdString(bhv_mgr->m_file_formats[ff].m_full_name));
                new_menu_to_cur_ws->addAction(new_act);
              }

            //deduce import as new ws
            if (bhv_mgr->m_ws_item_io[i]->m_accepted_file_format == ff &&
                bhv_mgr->m_ws_item_io[i]->can_load() &&
                bhv_mgr->m_ws_item_io[i]->m_menu_occupier &&
                bhv_mgr->m_ws_item_io[i]->m_can_be_imported_as_new_ws) {
                qextended_action *new_act = new qextended_action(this);
                new_act->m_joined_data[0] = i;
                connect(new_act, &QAction::triggered,
                        this, &main_window::action_bhv_import_as_new_workspace);
                new_act->setText(
                      QString::fromStdString(bhv_mgr->m_file_formats[ff].m_full_name));
                new_menu_as_new_ws->addAction(new_act);
              }

            //deduce save selected item
            if (bhv_mgr->m_ws_item_io[i]->m_accepted_file_format == ff &&
                bhv_mgr->m_ws_item_io[i]->can_save() &&
                bhv_mgr->m_ws_item_io[i]->m_menu_occupier) {
                std::cout <<
                             fmt::format("i={} can_save={}\n",
                                         i,
                                         bhv_mgr->m_ws_item_io[i]->can_save())
                          << std::endl;
                qextended_action *new_act = new qextended_action(this);
                new_act->m_joined_data[0] = i;
                connect(new_act, &QAction::triggered,
                        this, &main_window::action_bhv_export_selected);
                new_act->setText(
                      QString::fromStdString(bhv_mgr->m_file_formats[ff].m_full_name));
                new_menu_export_selected->addAction(new_act);
                file_menu_export_sel_as_acts.push_back(new_act);
              }
          }
    }
  //TODO: make lookup for bhv


}

void main_window::action_bhv_import_to_cur_workspace() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_manager->m_bhv_mgr.get();

  qextended_action *ext_act = qobject_cast<qextended_action*>(sender());
  if (!ext_act) return;

  size_t b_id = ext_act->m_joined_data[0];

  //check that bhv is valid
  if (b_id < bhv_mgr->m_ws_item_io.size() &&
      bhv_mgr->m_ws_item_io[b_id]->can_load() &&
      bhv_mgr->m_ws_item_io[b_id]->m_can_be_imported_to_ws) {
      // astate->log(fmt::format("{}", b_id));
      std::string file_name = QFileDialog::getOpenFileName(this,
                                                           "dialog_name",
                                                           "*.*").toStdString();
      if (!file_name.empty()) astate->ws_manager->import_from_file(file_name, b_id, false);
    }

}

void main_window::action_bhv_import_as_new_workspace() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_manager->m_bhv_mgr.get();

  qextended_action *ext_act = qobject_cast<qextended_action*>(sender());
  if (!ext_act) return;

  size_t b_id = ext_act->m_joined_data[0];

  //check that bhv is valid
  if (b_id < bhv_mgr->m_ws_item_io.size() &&
      bhv_mgr->m_ws_item_io[b_id]->can_load() &&
      bhv_mgr->m_ws_item_io[b_id]->m_can_be_imported_as_new_ws) {
      // astate->log(fmt::format("{}", b_id));
      std::string file_name = QFileDialog::getOpenFileName(this,
                                                           "dialog_name",
                                                           "*.*").toStdString();
      if (!file_name.empty()) astate->ws_manager->import_from_file(file_name, b_id);
    }

}

void main_window::action_bhv_export_selected() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_manager->m_bhv_mgr.get();

  qextended_action *ext_act = qobject_cast<qextended_action*>(sender());
  if (!ext_act) return;

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (!cur_ws) return;

  auto cur_it = cur_ws->get_selected_sp();
  if (!cur_it) return;

  size_t b_id = ext_act->m_joined_data[0];

  //check that bhv is valid
  if (b_id < bhv_mgr->m_ws_item_io.size() &&
      bhv_mgr->m_ws_item_io[b_id]->can_save() &&
      bhv_mgr->m_ws_item_io[b_id]->m_accepted_type == cur_it->get_type()) {
      // astate->log(fmt::format("{}", b_id));
      std::string file_name = QFileDialog::getSaveFileName(this,
                                                           "dialog_name",
                                                           "*.*").toStdString();
      if (!file_name.empty())
        astate->ws_manager->m_bhv_mgr->save_ws_item_to_file(file_name, cur_it, b_id);
    }

}

void main_window::control_bhv_menus_activity() {

  app_state_t* astate = app_state_t::get_inst();

  if (!astate->ws_manager) return;

  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_manager->m_bhv_mgr.get();

  if (!astate->ws_manager->has_wss()) {
      file_menu_import_to_cur_ws->setEnabled(false);
      file_menu_export_sel_as->setEnabled(false);
    } else {
      file_menu_import_to_cur_ws->setEnabled(true);
      bool need_to_enable_export_menu{false};
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it = cur_ws->get_selected_sp();
          if (cur_it) {
              for (auto &exp_act : file_menu_export_sel_as_acts) {
                  size_t bhv_id = exp_act->m_joined_data[0];
                  if (bhv_mgr->m_ws_item_io[bhv_id]->can_save() &&
                      bhv_mgr->m_ws_item_io[bhv_id]->m_accepted_type == cur_it->get_type()) {
                      need_to_enable_export_menu = true;
                      exp_act->setEnabled(true);
                      //save item dialog
                    } else {
                      exp_act->setEnabled(false);
                    }
                }
            } else {
              for (auto &exp_act : file_menu_export_sel_as_acts) exp_act->setEnabled(false);
            }
        }
    }

}

void main_window::slot_shortcut_terminate_app() {
  QApplication::quit();
}
