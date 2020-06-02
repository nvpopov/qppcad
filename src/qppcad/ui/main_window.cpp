#include <qppcad/ui/main_window.hpp>
#include <qppcad/core/app_state.hpp>

#include <qppcad/ui/app_settings_widget.hpp>
#include <qppcad/ui/add_new_ws_item_widget.hpp>
#include <qppcad/ui/toolbar_element.hpp>
#include <qppcad/ui/ptable_rich_widget.hpp>
#include <qppcad/ui/toolbar_element.hpp>

#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_measurement_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_labels_subsys.hpp>
#include <qppcad/ws_item/arrow_primitive/arrow_primitive.hpp>
#include <qppcad/python/python_simple_query.hpp>

#include <qppcad/core/platform.hpp>

#include <QDateTime>
#include <QColorDialog>

using namespace qpp;
using namespace qpp::cad;

main_window_t::main_window_t(QWidget *parent) : QMainWindow(parent) {

  m_main_wdgt = new QWidget;
  app_state_t* astate = app_state_t::get_inst();

  setAcceptDrops(true);
  setCentralWidget(m_main_wdgt);
  setMinimumHeight(astate->size_guide.main_window_h());
  setMinimumWidth(astate->size_guide.main_window_w());

  init_menus();
  build_bhv_menus_and_actions();
  build_bhv_tools_menus();
  control_bhv_menus_activity();
  control_bhv_tools_menus_activity();
  init_widgets();
  init_layouts();
  build_bhv_tool_panel();
  init_base_shortcuts();

  m_timer_one_sec = new QTimer(this);
  connect(m_timer_one_sec,
          &QTimer::timeout,
          this,
          QOverload<>::of(&main_window_t::timer_one_sec_shoot));
  m_timer_one_sec->setInterval(1000);
  m_timer_one_sec->start();
  timer_one_sec_shoot();

  connect(astate->astate_evd,
          &app_state_event_disp_t::wss_changed_signal,
          this,
          &main_window_t::wss_changed_slot);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_changed_signal,
          this,
          &main_window_t::cur_ws_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_changed_signal,
          this,
          &main_window_t::cur_ws_sel_item_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_edit_type_changed_signal,
          this,
          &main_window_t::cur_ws_edit_type_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_selection_changed_signal,
          this,
          &main_window_t::cur_ws_sel_atoms_list_sel_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::new_file_loaded_signal,
          this,
          &main_window_t::rebuild_recent_files_menu);

  connect(astate->astate_evd,
          &app_state_event_disp_t::set_left_inline_tool_visibility_signal,
          this,
          &main_window_t::inline_tool_left_ctrl_visibility);

  connect(astate->astate_evd,
          &app_state_event_disp_t::set_bottom_inline_tool_visibility_signal,
          this,
          &main_window_t::inline_tool_bottom_ctrl_visibility);

  wss_changed_slot();
  cur_ws_changed();
  cur_ws_edit_type_changed();
  cur_ws_sel_atoms_list_sel_changed();

  m_view_menu_show_modern_menu->setChecked(astate->m_show_modern_menu);
  show_modern_menu_state_changed(astate->m_show_modern_menu);

}

main_window_t::~main_window_t() {

}

void main_window_t::init_base_shortcuts() {

  m_sc_toggle_console = new QShortcut(this);
  m_sc_toggle_console->setKey(Qt::Key_QuoteLeft);
  connect(m_sc_toggle_console,
          &QShortcut::activated,
          this,
          &main_window_t::act_toggle_console);

  m_sc_enter_immersive_mode = new QShortcut(this);
  m_sc_enter_immersive_mode->setKey(Qt::Key_F12);
  connect(m_sc_enter_immersive_mode,
          &QShortcut::activated,
          this,
          &main_window_t::toggle_immersive_mode);

}

void main_window_t::init_menus() {

  m_file_menu  = menuBar()->addMenu(tr("&File"));

  m_file_menu_new_ws = new QAction(nullptr);
  m_file_menu_new_ws->setText(tr("New workspace"));
  m_file_menu_new_ws->setShortcut(QKeySequence(tr("Ctrl+n")));
  m_file_menu->addAction(m_file_menu_new_ws);
  connect(m_file_menu_new_ws,
          &QAction::triggered,
          this,
          &main_window_t::create_new_ws);

  m_file_menu_close_ws = new QAction(nullptr);
  m_file_menu_close_ws->setText(tr("Close workspace"));
  m_file_menu->addAction(m_file_menu_close_ws);
  connect(m_file_menu_close_ws,
          &QAction::triggered,
          this,
          &main_window_t::close_cur_ws);

  m_file_menu_close_all_ws = new QAction(nullptr);
  m_file_menu_close_all_ws->setText(tr("Close all workspaces"));
  m_file_menu->addAction(m_file_menu_close_all_ws);
  connect(m_file_menu_close_all_ws,
          &QAction::triggered,
          this,
          &main_window_t::close_all_ws);

  m_file_menu_open_ws = new QAction(nullptr);
  m_file_menu_open_ws->setText(tr("Open workspace"));
  m_file_menu_open_ws->setShortcut(QKeySequence(tr("Ctrl+o")));
  m_file_menu->addAction(m_file_menu_open_ws);
  connect(m_file_menu_open_ws,
          &QAction::triggered,
          this,
          &main_window_t::open_ws);

  m_file_menu_save_ws = new QAction(nullptr);
  m_file_menu_save_ws->setText(tr("Save workspace"));
  m_file_menu_save_ws->setShortcut(QKeySequence(tr("Ctrl+s")));
  m_file_menu->addSeparator();
  m_file_menu->addAction(m_file_menu_save_ws);
  connect(m_file_menu_save_ws,
          &QAction::triggered,
          this,
          &main_window_t::save_ws);

  m_file_menu_save_ws_as = new QAction(nullptr);
  m_file_menu_save_ws_as->setText(tr("Save workspace as"));
  m_file_menu->addAction(m_file_menu_save_ws_as);
  connect(m_file_menu_save_ws_as,
          &QAction::triggered,
          this,
          &main_window_t::save_ws_as);

  m_file_menu->addSeparator();
  m_file_menu_import_as_new_ws = m_file_menu->addMenu(tr("Import as new workspace"));
  m_file_menu_import_to_cur_ws = m_file_menu->addMenu(tr("Import to current workspace"));
  m_file_menu_export_sel_as = m_file_menu->addMenu(tr("Export selected item"));

  m_file_menu_recent_files = m_file_menu->addMenu(tr("Recent files"));
  for (int i = 0; i < qpp::cad::max_recent_files; i++) {

    QAction *recent_act = new QAction(m_file_menu_recent_files);
    m_file_menu_recent_entries[i] = recent_act;
    m_file_menu_recent_files->addAction(recent_act);
    connect(recent_act,
            &QAction::triggered,
            this,
            &main_window_t::recent_files_clicked);

  }

  m_file_menu_close_app = new QAction(nullptr);
  m_file_menu_close_app->setText(tr("Close"));
  m_file_menu_close_app->setShortcut(QKeySequence(tr("Ctrl+q")));
  m_file_menu->addSeparator();
  m_file_menu->addAction(m_file_menu_close_app);
  connect(m_file_menu_close_app, &QAction::triggered, this,
          &main_window_t::slot_shortcut_terminate_app);

  m_edit_menu  = menuBar()->addMenu(tr("&Edit"));
  m_edit_menu_undo = new QAction(nullptr);
  m_edit_menu_undo->setText(tr("Undo"));
  m_edit_menu_undo->setEnabled(false);
  m_edit_menu_undo->setShortcut(QKeySequence(tr("Ctrl+u")));
  m_edit_menu->addAction(m_edit_menu_undo);

  m_edit_menu_redo = new QAction(nullptr);
  m_edit_menu_redo->setText(tr("Redo"));
  m_edit_menu_redo->setEnabled(false);
  m_edit_menu_redo->setShortcut(QKeySequence(tr("Ctrl+r")));
  m_edit_menu->addAction(m_edit_menu_redo);

  m_edit_menu->addSeparator();
  m_edit_menu_ptable_wdgt = new QAction(nullptr);
  m_edit_menu_ptable_wdgt->setText(tr("Periodic table"));
  m_edit_menu->addAction(m_edit_menu_ptable_wdgt);
  connect(m_edit_menu_ptable_wdgt,
          &QAction::triggered,
          []() {
            ptable_rich_widget_t ptable_rich_wdgt; ptable_rich_wdgt.exec();
            app_state_t::get_inst()->make_viewport_dirty();
          });

  m_edit_menu->addSeparator();

  //Selection menu
  m_edit_menu_sel = m_edit_menu->addMenu(tr("Content selection"));
  m_edit_menu_sel_sel_all = new QAction(nullptr);
  m_edit_menu_sel_sel_all->setText(tr("Select all"));
  m_edit_menu_sel_sel_all->setShortcut(QKeySequence(tr("ctrl+a")));
  m_edit_menu_sel->addAction(m_edit_menu_sel_sel_all);
  connect(m_edit_menu_sel_sel_all,
          &QAction::triggered,
          this,
          &main_window_t::act_sel_all_cnt);

  m_edit_menu_sel_unsel_all = new QAction(nullptr);
  m_edit_menu_sel_unsel_all->setText(tr("Unselect all"));
  m_edit_menu_sel_unsel_all->setShortcut(QKeySequence(tr("shift+a")));
  m_edit_menu_sel->addAction(m_edit_menu_sel_unsel_all);
  connect(m_edit_menu_sel_unsel_all,
          &QAction::triggered,
          this,
          &main_window_t::act_unsel_all_cnt);

  m_edit_menu_sel_invert = new QAction(nullptr);
  m_edit_menu_sel_invert->setText(tr("Invert selection"));
  m_edit_menu_sel_invert->setShortcut(QKeySequence(tr("ctrl+i")));
  m_edit_menu_sel->addAction(m_edit_menu_sel_invert);
  connect(m_edit_menu_sel_invert,
          &QAction::triggered,
          this,
          &main_window_t::act_invert_sel_cnt);
  //End of selection menu

  m_edit_menu_switch_ws_edit_mode = new QAction(nullptr);
  m_edit_menu_switch_ws_edit_mode->setText(tr("Switch edit mode"));
  m_edit_menu_switch_ws_edit_mode->setShortcut(tr("ctrl+e"));
  connect(m_edit_menu_switch_ws_edit_mode,
          &QAction::triggered,
          this,
          &main_window_t::toggle_ws_edit_mode);

  m_edit_menu->addAction(m_edit_menu_switch_ws_edit_mode);

  // tools menu
  m_tools_menu = menuBar()->addMenu(tr("&Tools"));
  // end tools menu

  // workspace menu
  m_ws_menu = menuBar()->addMenu(tr("&Workspace"));

  m_ws_menu_rename_ws = new QAction(nullptr);
  m_ws_menu_rename_ws->setText(tr("Rename workspace"));
  m_ws_menu->addAction(m_ws_menu_rename_ws);
  connect(m_ws_menu_rename_ws,
          &QAction::triggered,
          this,
          &main_window_t::rename_cur_ws);

  m_ws_menu_bg_color = new QAction(nullptr);
  m_ws_menu_bg_color->setText(tr("Change background"));
  m_ws_menu->addAction(m_ws_menu_bg_color);
  connect(m_ws_menu_bg_color,
          &QAction::triggered,
          this,
          &main_window_t::change_cur_ws_bg);

  m_ws_copy_cam = new QAction(nullptr);
  m_ws_copy_cam->setText(tr("Copy camera to all"));
  m_ws_menu->addAction(m_ws_copy_cam);
  connect(m_ws_copy_cam,
          &QAction::triggered,
          this,
          &main_window_t::cam_copy_to_all);
  // end workspace menu

  // view menu
  m_view_menu = menuBar()->addMenu(tr("&View"));

  m_view_menu_show_oi = new QAction(nullptr);
  m_view_menu_show_oi->setText(tr("Show object inspector"));
  m_view_menu_show_oi->setCheckable(true);
  m_view_menu->addAction(m_view_menu_show_oi);
  connect(m_view_menu_show_oi,
          &QAction::toggled,
          this,
          &main_window_t::show_obj_insp_state_changed);

  m_view_menu_show_modern_menu = new QAction(nullptr);
  m_view_menu_show_modern_menu->setText(tr("Show modern menu"));
  m_view_menu_show_modern_menu->setCheckable(true);
  //view_menu->addAction(view_menu_show_modern_menu);
  connect(m_view_menu_show_modern_menu,
          &QAction::toggled,
          this,
          &main_window_t::show_modern_menu_state_changed);

  m_view_menu_show_gizmo = new QAction(nullptr);
  m_view_menu_show_gizmo->setText(tr("Show gizmo"));
  m_view_menu_show_gizmo->setCheckable(true);
  m_view_menu->addAction(m_view_menu_show_gizmo);
  connect(m_view_menu_show_gizmo,
          &QAction::toggled,
          this,
          &main_window_t::show_gizmo_state_changed);

  m_view_menu_toggle_fullscreen = new QAction(nullptr);
  m_view_menu_toggle_fullscreen->setText(tr("Toggle Fullscreen"));
  m_view_menu_toggle_fullscreen->setCheckable(true);
  m_view_menu_toggle_fullscreen->setShortcut(Qt::Key::Key_F11);
  m_view_menu->addAction(m_view_menu_toggle_fullscreen);
  connect(m_view_menu_toggle_fullscreen,
          &QAction::toggled,
          this,
          &main_window_t::toggle_fullscreen);

  m_view_menu_settings = new QAction(nullptr);
  m_view_menu_settings->setEnabled(false);
  m_view_menu_settings->setText(tr("Settings"));
  connect(m_view_menu_settings,
          &QAction::triggered,[](){app_settings_widget_t aset;aset.exec();});

  m_view_menu_ws_settings = new QAction(nullptr);
  m_view_menu_ws_settings->setEnabled(false);
  m_view_menu_ws_settings->setText(tr("Workspace settings"));
  m_view_menu->addAction(m_view_menu_settings);
  m_view_menu->addAction(m_view_menu_ws_settings);

  m_view_menu_log_wdgt = new QAction(nullptr);
  m_view_menu_log_wdgt->setText(tr("Show log"));
  m_view_menu_log_wdgt->setCheckable(true);
  connect(m_view_menu_log_wdgt,
          &QAction::toggled,
          this,
          &main_window_t::show_log_wdgt_state_changed);
  m_view_menu->addAction(m_view_menu_log_wdgt);

  m_view_menu_console = new QAction(nullptr);
  m_view_menu_console->setText(tr("Console"));
  m_view_menu_console->setShortcut(QKeySequence(tr("~")));
  m_view_menu_console->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
  connect(m_view_menu_console,
          &QAction::triggered,
          this,
          &main_window_t::act_toggle_console);
  m_view_menu->addAction(m_view_menu_console);

  m_view_menu_debug = m_view_menu->addMenu(tr("Debug"));

  m_view_menu_toggle_debug_tws_tree = new QAction(nullptr);
  m_view_menu_toggle_debug_tws_tree->setText(tr("Render tws-tree"));
  m_view_menu_toggle_debug_tws_tree->setCheckable(true);
  m_view_menu_debug->addAction(m_view_menu_toggle_debug_tws_tree);
  connect(m_view_menu_toggle_debug_tws_tree,
          &QAction::toggled,
          [](bool checked) {
            app_state_t* astate = app_state_t::get_inst();
            astate->m_debug_show_tws_tree = checked;
            astate->make_viewport_dirty();
          });

  // end of view menu

  // help menu
  m_help_menu  = menuBar()->addMenu(tr("&Help"));
  m_help_menu_about = new QAction(nullptr);
  m_help_menu_about->setText(tr("About"));
  m_help_menu->addAction(m_help_menu_about);
  connect(m_help_menu_about,
          &QAction::triggered,
          []() {
            QMessageBox::about(nullptr, "qpp::cad",
                               QObject::tr(
                                   "Site : <a href='https://github.com/nvpopov/qppcad'>"
                                   "https://github.com/nvpopov/qppcad"
                                   "</a><br>"
                                   "Git rev : %1<br>"
                                   "Build time : %2<br>"
                                   "Qt version : %3<br>"
                                   "Python : %4")
                                   .arg(QString::fromStdString(build_info_t::get_git_version()))
                                   .arg(QString::fromStdString(build_info_t::get_build_date()))
                                   .arg(QString::fromStdString(build_info_t::get_qt_version()))
                                   .arg(QString::fromStdString(build_info_t::get_python_version()))
                               );
          });

  // end of help menu

  m_modern_menu = new QMenu;
  //modern_menu->setLayoutDirection( Qt::RightToLeft );
  m_modern_menu->addMenu(m_file_menu);
  m_modern_menu->addMenu(m_edit_menu);
  m_modern_menu->addMenu(m_tools_menu);
  m_modern_menu->addMenu(m_ws_menu);
  m_modern_menu->addMenu(m_view_menu);
  m_modern_menu->addMenu(m_help_menu);

  m_menu_wdgt_hldr = new QWidget;
  m_menu_wdgt_hldr_lt = new QVBoxLayout;
  m_menu_wdgt_hldr_lt->setContentsMargins(1, 1, 1, 1);
  m_menu_wdgt_hldr->setLayout(m_menu_wdgt_hldr_lt);
  m_menu_wdgt_overview = new QLabel;
  m_menu_wdgt_overview->setProperty("s_class", "lbl_overview");
  m_menu_wdgt_hldr_lt->addWidget(m_menu_wdgt_overview);
  //m_menu_wdgt->setDefaultWidget(m_menu_wdgt_hldr);

  menuBar()->setCornerWidget(m_menu_wdgt_hldr);

}

void main_window_t::init_widgets() {

  app_state_t* astate = app_state_t::get_inst();

  m_tp_wdgt = new QFrame(nullptr);
  m_tp_wdgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_tp_wdgt->setFixedWidth(astate->size_guide.tool_panel_w());
  m_tp_wdgt->setProperty("border_class", "border_right");

  m_tp_modern_menu = new QPushButton(nullptr);
  m_tp_modern_menu->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_modern_menu->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_modern_menu->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                      astate->size_guide.tool_panel_icon_size()));
  m_tp_modern_menu->setToolTip(tr("Show modern menu"));
  m_tp_modern_menu->setIcon(QIcon("://images/more_vert-24px.svg"));
  connect(m_tp_modern_menu,
          &QPushButton::clicked,
          this,
          &main_window_t::modern_menu_clicked);
  //tp_overview->setText("[]");

  m_tp_print_screen = new QPushButton(nullptr);
  m_tp_print_screen->setProperty("s_class", "tp_cb");
  m_tp_print_screen->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_print_screen->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_print_screen->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                       astate->size_guide.tool_panel_icon_size()));
  m_tp_print_screen->setToolTip(tr("Save screenshot to current work dir"));
  m_tp_print_screen->setIcon(QIcon("://images/outline-camera_enhance-24px.svg"));
  connect(m_tp_print_screen,
          &QPushButton::pressed,
          this,
          &main_window_t::make_screenshot);

  m_tp_edit_mode = new QButtonGroup(nullptr);
  m_tp_edit_mode->setExclusive(true);
  connect(m_tp_edit_mode,
          static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
          this,
          &main_window_t::ws_edit_mode_selector_button_clicked);

  m_tp_edit_mode_item = new QPushButton(nullptr);
  m_tp_edit_mode_item->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_edit_mode_item->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_edit_mode_item->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                         astate->size_guide.tool_panel_icon_size()));
  m_tp_edit_mode_item->setToolTip(tr("Set current workspace's edit mode to <edit item>"));
  m_tp_edit_mode_item->setIcon(QIcon("://images/edit0.svg"));
  m_tp_edit_mode_item->setCheckable(true);

  m_tp_edit_mode_cnt= new QPushButton(nullptr);
  m_tp_edit_mode_cnt->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_edit_mode_cnt->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_edit_mode_cnt->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                        astate->size_guide.tool_panel_icon_size()));
  m_tp_edit_mode_cnt->setToolTip(tr("Set current workspace's edit mode to <edit content>"));
  m_tp_edit_mode_cnt->setIcon(QIcon("://images/edit1.svg"));
  m_tp_edit_mode_cnt->setCheckable(true);

  m_tp_edit_mode->addButton(m_tp_edit_mode_item, 0);
  m_tp_edit_mode->addButton(m_tp_edit_mode_cnt, 1);

  m_tp_edit_mode_start = new QFrame(nullptr);
  m_tp_edit_mode_start->setFrameShape(QFrame::VLine);
  m_tp_edit_mode_start->setFrameShadow(QFrame::Sunken);
  m_tp_edit_mode_start->setFixedWidth(3);
  m_tp_edit_mode_start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

  m_tp_edit_mode_end = new QFrame(nullptr);
  m_tp_edit_mode_end->setFrameShape(QFrame::VLine);
  m_tp_edit_mode_end->setFrameShadow(QFrame::Sunken);
  m_tp_edit_mode_end->setFixedWidth(3);
  m_tp_edit_mode_end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
  connect(m_tp_edit_mode,
          static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
          this,
          &main_window_t::ws_edit_mode_selector_button_clicked);

  m_tp_cam_tool = new QToolButton(nullptr);
  m_tp_cam_tool->setIcon(QIcon("://images/outline-videocam-24px.svg"));
  m_tp_cam_tool->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                   astate->size_guide.tool_panel_icon_size()));

  m_tp_cam_tool->setPopupMode(QToolButton::InstantPopup);
  m_tp_cam_tool->setArrowType(Qt::NoArrow);
  m_tp_cam_tool->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_cam_tool->setMinimumHeight(astate->size_guide.tool_panel_ctrl_h());
  connect(m_tp_cam_tool,
          &QToolButton::triggered,
          this,
          &main_window_t::tp_camera_tool_button_triggered);

  m_tp_scenic_rot_cam = new QPushButton(nullptr);
  m_tp_scenic_rot_cam->setToolTip(tr("Orbit the camera"));
  m_tp_scenic_rot_cam->setIcon(QIcon("://images/baseline-3d_rotation_x-24px.svg"));
  m_tp_scenic_rot_cam->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_scenic_rot_cam->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_scenic_rot_cam->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                         astate->size_guide.tool_panel_icon_size()));
  m_tp_scenic_rot_cam->setCheckable(true);
  connect(m_tp_scenic_rot_cam,
          &QPushButton::toggled,
          this,
          &main_window_t::tp_scenic_rotation_toggle);

  m_tp_cam_tool_act_x = new qextended_action(nullptr);
  m_tp_cam_tool_act_x->m_joined_data[0] = cam_tv_e::tv_x;
  m_tp_cam_tool_act_x->setText(tr( "x - axis"));
  m_tp_cam_tool->addAction(m_tp_cam_tool_act_x);

  m_tp_cam_tool_act_y = new qextended_action(nullptr);
  m_tp_cam_tool_act_y->m_joined_data[0] = cam_tv_e::tv_y;
  m_tp_cam_tool_act_y->setText(tr( "y - axis"));
  m_tp_cam_tool->addAction(m_tp_cam_tool_act_y);

  m_tp_cam_tool_act_z = new qextended_action(nullptr);
  m_tp_cam_tool_act_z->m_joined_data[0] = cam_tv_e::tv_z;
  m_tp_cam_tool_act_z->setText(tr( "z - axis"));
  m_tp_cam_tool->addAction(m_tp_cam_tool_act_z);

  m_tp_cam_tool_act_cc = new qextended_action(nullptr);
  m_tp_cam_tool_act_cc->m_joined_data[0] = cam_tv_e::tv_cc;
  m_tp_cam_tool_act_cc->setText(tr("cart. c."));
  m_tp_cam_tool->addAction(m_tp_cam_tool_act_cc);

  m_tp_cam_tool_act_a = new qextended_action(nullptr);
  m_tp_cam_tool_act_a->m_joined_data[0] = cam_tv_e::tv_a;
  m_tp_cam_tool_act_a->setText(tr( "a - axis"));
  m_tp_cam_tool->addAction(m_tp_cam_tool_act_a);

  m_tp_cam_tool_act_b = new qextended_action(nullptr);
  m_tp_cam_tool_act_b->m_joined_data[0] = cam_tv_e::tv_b;
  m_tp_cam_tool_act_b->setText(tr( "b - axis"));
  m_tp_cam_tool->addAction(m_tp_cam_tool_act_b);

  m_tp_cam_tool_act_c = new qextended_action(nullptr);
  m_tp_cam_tool_act_c->m_joined_data[0] = cam_tv_e::tv_c;
  m_tp_cam_tool_act_c->setText(tr( "c - axis"));
  m_tp_cam_tool->addAction(m_tp_cam_tool_act_c);

  m_tp_utility_frame_end = new QFrame(nullptr);
  m_tp_utility_frame_end->setFrameShape(QFrame::StyledPanel);
  m_tp_utility_frame_end->setFixedWidth(3);
  m_tp_utility_frame_end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

  m_tp_anim_fast_forward = new QPushButton(nullptr);
  m_tp_anim_fast_forward->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_anim_fast_forward->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_anim_fast_forward->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                            astate->size_guide.tool_panel_icon_size()));
  m_tp_anim_fast_forward->setToolTip(tr("Fast-forward to last frame of animated geometry"));
  m_tp_anim_fast_forward->setIcon(QIcon("://images/outline-fast_forward-24px.svg"));
  connect(m_tp_anim_fast_forward,
          &QPushButton::clicked,
          this,
          &main_window_t::tp_fast_forward_anim_clicked);

  m_tp_msr_dist = new QPushButton(nullptr);
  m_tp_msr_dist->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_msr_dist->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_msr_dist->setCheckable(true);
  m_tp_msr_dist->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                   astate->size_guide.tool_panel_icon_size()));
  m_tp_msr_dist->setToolTip(tr("Measure distance between pair of atoms"));
  m_tp_msr_dist->setIcon(QIcon("://images/dist.svg"));
  connect(m_tp_msr_dist,
          &QPushButton::toggled,
          this,
          &main_window_t::tp_dist_button_clicked);

  m_tp_toggle_atom_override = new QPushButton(nullptr);
  m_tp_toggle_atom_override->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_toggle_atom_override->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_toggle_atom_override->setCheckable(true);
  m_tp_toggle_atom_override->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                               astate->size_guide.tool_panel_icon_size()));
  m_tp_toggle_atom_override->setToolTip(tr("Toggle atom override"));
  m_tp_toggle_atom_override->setIcon(QIcon("://images/outline-my_location-24px.svg"));
  connect(m_tp_toggle_atom_override,
          &QPushButton::toggled,
          this,
          &main_window_t::tp_toggle_atom_override_button_clicked);

  m_tp_force_sel_lbl_vis = new QPushButton(nullptr);
  m_tp_force_sel_lbl_vis->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_force_sel_lbl_vis->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_force_sel_lbl_vis->setCheckable(true);
  m_tp_force_sel_lbl_vis->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                            astate->size_guide.tool_panel_icon_size()));
  m_tp_force_sel_lbl_vis->setToolTip(tr("Force labels selective visibility"));
  m_tp_force_sel_lbl_vis->setIcon(QIcon("://images/outline-font_download-24px.svg"));
  connect(m_tp_force_sel_lbl_vis,
          &QPushButton::toggled,
          this,
          &main_window_t::tp_force_sel_lbl_vis_button_clicked);

  m_tp_msr_angle = new QPushButton(nullptr);
  m_tp_msr_angle->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_msr_angle->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_msr_angle->setCheckable(true);
  m_tp_msr_angle->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                    astate->size_guide.tool_panel_icon_size()));
  m_tp_msr_angle->setToolTip(tr("Measure angle between atoms triple"));
  m_tp_msr_angle->setIcon(QIcon("://images/angle.svg"));
  connect(m_tp_msr_angle,
          &QPushButton::toggled,
          this,
          &main_window_t::tp_angle_button_clicked);

  m_tp_add_cube = new QPushButton(nullptr);
  m_tp_add_cube->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_add_cube->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_add_cube->setToolTip(tr("Embed cube between two atoms"));
  m_tp_add_cube->setIcon(QIcon("://images/add_cube.svg"));
  m_tp_add_cube->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                   astate->size_guide.tool_panel_icon_size()));
  connect(m_tp_add_cube,
          &QPushButton::clicked,
          this,
          &main_window_t::tp_add_cube_clicked);

  m_tp_add_arrow = new QPushButton(nullptr);
  m_tp_add_arrow->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_add_arrow->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_add_arrow->setToolTip(tr("Embed arrow between two atoms"));
  m_tp_add_arrow->setIcon(QIcon("://images/add_arrow.svg"));
  m_tp_add_arrow->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                    astate->size_guide.tool_panel_icon_size()));
  connect(m_tp_add_arrow,
          &QPushButton::clicked,
          this,
          &main_window_t::tp_add_arrow_clicked);

  m_tp_add_point_sym_group = new QPushButton(nullptr);
  m_tp_add_point_sym_group->setFixedWidth(astate->size_guide.tool_panel_ctrl_w());
  m_tp_add_point_sym_group->setFixedHeight(astate->size_guide.tool_panel_ctrl_h());
  m_tp_add_point_sym_group->setToolTip(tr("Construct point symmetry group"));
  m_tp_add_point_sym_group->setIcon(QIcon("://images/add_psg.svg"));
  m_tp_add_point_sym_group->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                                              astate->size_guide.tool_panel_icon_size()));
  connect(m_tp_add_point_sym_group,
          &QPushButton::clicked,
          this,
          &main_window_t::tp_add_point_sym_group_clicked);

  m_ws_viewer_wdgt = new ws_viewer_widget_t(nullptr);
  m_ws_viewer_wdgt_frame = new QFrame(nullptr);
  m_ws_viewer_wdgt_frame_lt = new QHBoxLayout;
  m_ws_viewer_wdgt_frame->setLayout(m_ws_viewer_wdgt_frame_lt);
  m_ws_viewer_wdgt_frame_lt->setContentsMargins(0,0,0,0);
  m_ws_viewer_wdgt_frame->setObjectName("ws_viewer_frame");
  m_ws_viewer_wdgt_frame_lt->addWidget(m_ws_viewer_wdgt);

  m_obj_insp_wdgt = new object_inspector_widget_t(nullptr);
  m_obj_insp_wdgt->setFixedWidth(astate->size_guide.obj_insp_w());

  m_py_console_wdgt = new python_console_widget_t(nullptr);

  m_ext_edtr_compositor = new ws_item_extended_editor_compositor_t(nullptr);
  m_inline_left_tool =
      new qinline_tool_window_t(qinline_tool_type_e::tool_vertical, nullptr);
  m_inline_left_tool->setVisible(false);

  m_inline_btm_tool =
      new qinline_tool_window_t(qinline_tool_type_e::tool_horizontal, nullptr);
  m_inline_btm_tool->setVisible(false);

}

void main_window_t::init_layouts() {

  app_state_t *astate = app_state_t::get_inst();

  m_main_lt = new QVBoxLayout;
  m_ws_tabbar_wdgt = new ws_tabbar_t(nullptr);
  m_log_wdgt = new log_widget_t(nullptr);

  m_wstabbar_overview_wrp = new QWidget;
  m_wstabbar_overview_wrp->setFixedHeight(astate->size_guide.ws_tabbar_height());
  m_wstabbar_overview_wrp_lt = new QHBoxLayout;
  m_wstabbar_overview_wrp->setLayout(m_wstabbar_overview_wrp_lt);
  m_wstabbar_overview_wrp_lt->setContentsMargins(0,0,0,0);
  m_wstabbar_overview_wrp_lt->setSpacing(0);
  m_wstabbar_overview_wrp->setObjectName("m_tp_overview_wrp");

  m_main_wdgt->setLayout(m_main_lt);

  m_main_lt->addWidget(m_tp_wdgt);
  m_main_lt->addWidget(m_wstabbar_overview_wrp);
  m_main_lt->setContentsMargins(0,0,0,0);
  m_main_lt->setSpacing(0);

  m_wstabbar_overview_wrp_lt->addWidget(m_ws_tabbar_wdgt);
  m_ws_tabbar_wdgt->raise();

  m_splitter_ws_viewer_ext_edt = new QSplitter(Qt::Horizontal);

  m_splitter_ws_viewer_ext_edt->addWidget(m_ws_viewer_wdgt_frame);
  m_splitter_ws_viewer_ext_edt->addWidget(m_ext_edtr_compositor);
  m_splitter_ws_viewer_ext_edt->setSizes(QList<int>({2, 1}));
  m_splitter_ws_viewer_ext_edt->setHandleWidth(0);

  m_splitter_ws_viewer_ext_edt->setContentsMargins(0,0,0,0);
  m_splitter_ws_viewer_ext_edt->setCollapsible(1, false);
  m_splitter_ws_viewer_ext_edt->setCollapsible(0, false);
  m_ext_edtr_compositor->hide();

  m_splitter_py_console_log_wdgt = new QSplitter(Qt::Horizontal);

  m_splitter_py_console_log_wdgt->addWidget(m_py_console_wdgt);
  m_splitter_py_console_log_wdgt->addWidget(m_log_wdgt);
  m_splitter_py_console_log_wdgt->setHandleWidth(1);
  m_splitter_py_console_log_wdgt->setCollapsible(0, false);
  m_splitter_py_console_log_wdgt->setCollapsible(1, false);

  m_splitter_ws_viewer_py_console_log = new QSplitter(Qt::Vertical);
  m_splitter_ws_viewer_py_console_log->addWidget(m_splitter_ws_viewer_ext_edt);
  m_splitter_ws_viewer_py_console_log->addWidget(m_inline_btm_tool);
  m_splitter_ws_viewer_py_console_log->addWidget(m_splitter_py_console_log_wdgt);
  m_splitter_ws_viewer_py_console_log->setHandleWidth(0);
  m_splitter_ws_viewer_py_console_log->setSizes(QList<int>({1, 2, 3}));
  m_splitter_ws_viewer_py_console_log->setContentsMargins(0,0,0,0);
  m_py_console_wdgt->hide();
  m_log_wdgt->hide();
  m_splitter_ws_viewer_py_console_log->setCollapsible(0, false);
  m_splitter_ws_viewer_py_console_log->setCollapsible(1, false);
  m_splitter_ws_viewer_py_console_log->setCollapsible(2, false);
  //m_splitter_ws_viewer_py_console_log->setCollapsible(3, false);

  m_layout_ws_viewer_obj_insp = new QSplitter(Qt::Horizontal);
  m_layout_ws_viewer_obj_insp->addWidget(m_tp_wdgt);
  m_layout_ws_viewer_obj_insp->addWidget(m_inline_left_tool);
  m_layout_ws_viewer_obj_insp->addWidget(m_splitter_ws_viewer_py_console_log);
  m_layout_ws_viewer_obj_insp->addWidget(m_obj_insp_wdgt);
  m_layout_ws_viewer_obj_insp->setContentsMargins(0,0,0,0);
  //m_layout_ws_viewer_obj_insp->setSizes(QList<int>({1, 6, 3}));
  m_layout_ws_viewer_obj_insp->setCollapsible(0, false);
  m_layout_ws_viewer_obj_insp->setCollapsible(1, false);
  m_layout_ws_viewer_obj_insp->setCollapsible(2, false);
  m_layout_ws_viewer_obj_insp->setCollapsible(3, false);
  //m_layout_ws_viewer_obj_insp->setCollapsible(4, false);

  m_layout_ws_viewer_obj_insp->setHandleWidth(0);
  m_main_lt->addWidget(m_layout_ws_viewer_obj_insp);

  m_tp_lt = new QVBoxLayout;
  m_tp_wdgt->setLayout(m_tp_lt);
  m_tp_lt->setContentsMargins(5, 4, 0, 0);
  m_tp_lt->setSpacing(1);

  m_tp_lt->addWidget(m_tp_edit_mode_start, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_edit_mode_item, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_edit_mode_cnt, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_edit_mode_end, 0, Qt::AlignLeft);

  m_tp_lt->addWidget(m_tp_print_screen, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_scenic_rot_cam, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_cam_tool, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_utility_frame_end, 0, Qt::AlignLeft);

  m_tp_lt->addWidget(m_tp_anim_fast_forward, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_msr_dist, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_msr_angle, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_force_sel_lbl_vis, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_toggle_atom_override, 0, Qt::AlignLeft);

  m_tp_lt->addWidget(m_tp_add_cube, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_add_arrow, 0, Qt::AlignLeft);
  m_tp_lt->addWidget(m_tp_add_point_sym_group, 0, Qt::AlignLeft);

  m_tp_lt->addStretch(1);
  //m_tp_lt->addWidget(m_tp_overview, 0, Qt::AlignRight);
  m_tp_lt->addWidget(m_tp_modern_menu, 0, Qt::AlignRight);
  m_tp_lt->addSpacing(10);
  //tool_panel_widget->stackUnder(ws_viewer_widget);
  m_ws_viewer_wdgt->lower();

}

void main_window_t::dragEnterEvent(QDragEnterEvent *event) {
  event->acceptProposedAction();
}

void main_window_t::dragMoveEvent(QDragMoveEvent *event) {
  event->acceptProposedAction();
}

void main_window_t::dragLeaveEvent(QDragLeaveEvent *event) {
  event->accept();
}

void main_window_t::dropEvent(QDropEvent *event) {

  const QMimeData *mimeData = event->mimeData();

  if (mimeData && mimeData->hasUrls()) {

    app_state_t* astate = app_state_t::get_inst();
    QList<QUrl> urlList = mimeData->urls();

    for (int i = 0; i < urlList.size(); i++) {

      QString native_path = urlList.at(i).toLocalFile();
      std::string native_path_str = native_path.toStdString();
      //astate->get_inst()->log(fmt::format("DRAG EN DROP EVENT {} {}", i, native_path_str));
      astate->ws_mgr->load_from_file_autodeduce(native_path_str, "", true, true);

    }

  }

}

void main_window_t::resizeEvent(QResizeEvent *event) {

  app_state_t::get_inst()->log(fmt::format("main_window::resizeEvent(width={}, height={})",
                                           event->size().width(),
                                           event->size().height()));

  QMainWindow::resizeEvent(event);

}

void main_window_t::timer_one_sec_shoot() {

  app_state_t *astate = app_state_t::get_inst();

  if (m_menu_wdgt_overview) {

    m_menu_wdgt_overview->setText(
        tr(" Frame(CPU): %1 ms. | Frame(GPU): %2 ms. | Used memory: %3 Mb ")
            .arg(astate->m_last_frame_time_cpu / 1000000.0, 6, 'f', 4)
            .arg(astate->m_last_frame_time_gpu / 1000000.0, 6, 'f', 4)
            .arg(platform_helper_t::get_mem_usage_mb()));
    menuBar()->adjustSize();

  }

}

void main_window_t::wss_changed_slot() {

  app_state_t* astate = app_state_t::get_inst();

  control_bhv_menus_activity();


  if (astate->ws_mgr->has_wss()) {

    if (!astate->m_immersive_mode) {

      m_wstabbar_overview_wrp->setVisible(true);
      m_tp_wdgt->setVisible(true);

    }

    m_file_menu_close_ws->setEnabled(true);
    m_ws_menu_rename_ws->setEnabled(true);
    m_view_menu_show_gizmo->setEnabled(true);
    m_view_menu_show_gizmo->setVisible(true);
    m_ws_copy_cam->setEnabled(true);
    m_ws_menu_bg_color->setEnabled(true);
    //obj_insp_widget->setVisible()

  } else {

    m_obj_insp_wdgt->setVisible(false);
    m_tp_wdgt->setVisible(false);
    //m_ws_tabbar_wdgt->setVisible(false);
    m_wstabbar_overview_wrp->setVisible(false);
    m_file_menu_close_ws->setEnabled(false);
    m_ws_menu_rename_ws->setEnabled(false);
    m_view_menu_show_gizmo->setEnabled(false);
    m_view_menu_show_gizmo->setVisible(false);
    m_ws_copy_cam->setEnabled(false);
    m_ws_menu_bg_color->setEnabled(false);

  }

  astate->log(fmt::format("main_window::workspaces_changed_slot(), total ws = {}",
                          astate->ws_mgr->m_ws.size()));

}

void main_window_t::ws_selector_selection_changed(int index) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_mgr->has_wss()) {

    auto current = astate->ws_mgr->get_cur_id();
    astate->log(fmt::format("ws_selector_selection_changed index: {}, ws_cur_id: {}",
                            index, *current));
    if (current) {
      astate->ws_mgr->set_cur_id(std::optional<size_t>(index));
      astate->make_viewport_dirty();
    }

  }

}

void main_window_t::show_obj_insp_state_changed(bool checked) {

  app_state_t* astate = app_state_t::get_inst();

  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_ignore);

  if (!ok) {
    m_obj_insp_wdgt->hide();
    return;
  }

  if (checked) {
    m_obj_insp_wdgt->show();
    cur_ws->m_show_obj_insp = true;
  } else {
    m_obj_insp_wdgt->hide();
    cur_ws->m_show_obj_insp = false;
  }

}

void main_window_t::show_gizmo_state_changed(bool checked) {

  app_state_t* astate = app_state_t::get_inst();

  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_ignore);

  if (ok) {
    cur_ws->m_gizmo->m_is_visible = checked;
    astate->make_viewport_dirty();
  }

}

void main_window_t::show_log_wdgt_state_changed(bool checked) {

  m_log_wdgt->setVisible(checked);

}

void main_window_t::show_modern_menu_state_changed(bool checked) {

  app_state_t* astate = app_state_t::get_inst();
  astate->m_show_modern_menu = checked;

  if (checked) {
    menuBar()->hide();
    m_tp_modern_menu->show();
  } else {
    menuBar()->show();
    m_tp_modern_menu->hide();
  }

}

void main_window_t::create_new_ws() {

  app_state_t* astate = app_state_t::get_inst();
  astate->ws_mgr->create_new_ws(true);
  wss_changed_slot();
  astate->make_viewport_dirty();

}

void main_window_t::open_ws() {

  app_state_t* astate = app_state_t::get_inst();
  QString file_name = QFileDialog::getOpenFileName(this,
                                                   "Open qpp::cad workspace",
                                                   astate->m_last_dir,
                                                   "*.json");

  if (file_name != "") {

    astate->ws_mgr->load_from_file(file_name.toStdString(), true);
    wss_changed_slot();

  }

}

void main_window_t::save_ws() {

  app_state_t* astate = app_state_t::get_inst();

  stop_update_cycle();

  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_mbox);

  if (ok) {

    QFileInfo check_file(QString::fromStdString(cur_ws->m_fs_path));

    if (check_file.exists() && check_file.isFile() && cur_ws->m_fs_path != "" &&
        !cur_ws->m_is_ws_imported) {

      cur_ws->m_is_ws_imported = false;
      cur_ws->save_ws_to_json(cur_ws->m_fs_path);

    } else {

      QString file_name = QFileDialog::getSaveFileName(this,
                                                       "Save qpp::cad workspace",
                                                       astate->m_last_dir,
                                                       "*.json");

      if (file_name != "") {

        if (!file_name.endsWith(".json")) file_name += ".json";
        cur_ws->save_ws_to_json(file_name.toStdString());
        cur_ws->m_is_ws_imported = false;
        cur_ws->m_fs_path = file_name.toStdString();

      }

    }
  }

  start_update_cycle();

  cur_ws_changed();

}

void main_window_t::save_ws_as() {

  app_state_t* astate = app_state_t::get_inst();

  stop_update_cycle();

  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_mbox);

  if (ok) {

    QString file_name = QFileDialog::getSaveFileName(this,
                                                     "Save qpp::cad workspace",
                                                     astate->m_last_dir,
                                                     "*.json");

    if (file_name != "") {

      cur_ws->save_ws_to_json(file_name.toStdString());
      cur_ws->m_is_ws_imported = false;
      cur_ws->m_fs_path = file_name.toStdString();

    }

  }

  cur_ws_changed();

  start_update_cycle();

}

void main_window_t::close_cur_ws() {

  app_state_t* astate = app_state_t::get_inst();
  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_mbox);

  if (ok) {

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Workspace -> Close"),
                                  tr("Do you really want to close the workspace?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
      cur_ws->m_marked_for_deletion = true;
    }

  }

}

void main_window_t::close_all_ws() {

  app_state_t* astate = app_state_t::get_inst();

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, tr("Workspaces -> Close"),
                                tr("Do you really want to close all workspaces?"),
                                QMessageBox::Yes | QMessageBox::No);

  if (reply == QMessageBox::Yes)
    for (auto ws : astate->ws_mgr->m_ws) ws->m_marked_for_deletion = true;

}

void main_window_t::rename_cur_ws() {

  app_state_t* astate = app_state_t::get_inst();
  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_mbox);

  if (ok) {

    bool ok_q;
    QString text = QInputDialog::getText(this, tr("Workspace -> Rename"),
                                         tr("User name:"), QLineEdit::Normal,
                                         QString::fromStdString(cur_ws->m_ws_name), &ok_q);
    if (ok_q && text != "") {
      cur_ws->m_ws_name = text.toStdString();
      astate->astate_evd->wss_changed();
    }

  }

}

void main_window_t::change_cur_ws_bg() {

  app_state_t* astate = app_state_t::get_inst();

  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_mbox);

  if (ok) {

    QColor _stored_color = QColor::fromRgbF(
        cur_ws->m_bg_color[0],
        cur_ws->m_bg_color[1],
        cur_ws->m_bg_color[2]
        );

    const QColor clr = QColorDialog::getColor(_stored_color, this,
                                              "Select workspace`s background color");
    if (clr.isValid()) {

      cur_ws->m_bg_color = vector3<float> {
          static_cast<float>(clr.redF()),
          static_cast<float>(clr.greenF()),
          static_cast<float>(clr.blueF())
      };

      astate->make_viewport_dirty();

    }

  }

}

void main_window_t::cam_copy_to_all() {

  app_state_t* astate = app_state_t::get_inst();
  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_mbox);
  if (!ok && !cur_ws) return;

  for (size_t i = 0; i < astate->ws_mgr->m_ws.size(); i++)
    if (cur_ws != astate->ws_mgr->m_ws[i])
      astate->ws_mgr->m_ws[i]->copy_cam(cur_ws);

}

void main_window_t::cur_ws_changed() {

  app_state_t* astate = app_state_t::get_inst();

  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_ignore);

  for (auto elem : {m_inline_left_tool, m_inline_btm_tool})
    if (elem && elem->is_active())
      elem->close_triggered();

  if (ok) {

    m_tp_scenic_rot_cam->blockSignals(true);
    m_tp_scenic_rot_cam->setChecked(cur_ws->m_scenic_rotation);
    m_tp_scenic_rot_cam->blockSignals(false);

    m_view_menu_show_oi->setChecked(cur_ws->m_show_obj_insp);
    m_view_menu_show_oi->setVisible(true);
    m_view_menu_show_gizmo->setChecked(cur_ws->m_gizmo->m_is_visible);

    std::string title_text = fmt::format("qpp::cad [ws_name: {}] - [path: {}]",
                                         cur_ws->m_ws_name, cur_ws->m_fs_path);
    this->setWindowTitle(QString::fromStdString(title_text));

  } else {

    m_view_menu_show_oi->setChecked(false);
    m_view_menu_show_oi->setVisible(false);

    this->setWindowTitle("qpp::cad");

  }

  wss_changed_slot();
  cur_ws_props_changed();
  cur_ws_sel_item_changed();

}

void main_window_t::cur_ws_sel_item_changed() {

  app_state_t* astate = app_state_t::get_inst();

  bool show_cam_button{false};
  bool show_fast_forward_button{false};

  auto [cur_ws, cur_itm, as_al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>();

  if (cur_itm) {

    if (cur_itm->get_flags() & ws_item_flags_cam_target_view) show_cam_button = true;

    if (as_al) {

      bool al_is_3d = as_al->m_geom->DIM == 3;
      //tp_camera_tool_act_a->setVisible(al_is_3d)
      m_tp_cam_tool_act_a->setVisible(al_is_3d);
      m_tp_cam_tool_act_b->setVisible(al_is_3d);
      m_tp_cam_tool_act_c->setVisible(al_is_3d);
      m_tp_cam_tool_act_x->setVisible(!al_is_3d);
      m_tp_cam_tool_act_y->setVisible(!al_is_3d);
      m_tp_cam_tool_act_z->setVisible(!al_is_3d);
      m_tp_cam_tool_act_cc->setVisible(!al_is_3d);

      show_fast_forward_button = as_al->m_anim->animable();

    }

  }

  m_tp_cam_tool->setVisible(show_cam_button);
  m_tp_anim_fast_forward->setVisible(show_fast_forward_button);

  cur_ws_edit_type_changed();
  control_bhv_menus_activity();
  control_bhv_tools_menus_activity();

}

void main_window_t::cur_ws_props_changed() {

  app_state_t* astate = app_state_t::get_inst();

  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_ignore);

  if (ok) {

    bool check_t = cur_ws->m_edit_type == ws_edit_e::edit_item;
    m_tp_edit_mode_item->blockSignals(true);
    m_tp_edit_mode_cnt->blockSignals(true);
    m_tp_edit_mode_item->setChecked(check_t);
    m_tp_edit_mode_cnt->setChecked(!check_t);
    m_tp_edit_mode_item->blockSignals(false);
    m_tp_edit_mode_cnt->blockSignals(false);

  }

  astate->make_viewport_dirty();

}

void main_window_t::cur_ws_edit_type_changed() {

  cur_ws_props_changed();
  cur_ws_sel_atoms_list_sel_changed();

}

void main_window_t::cur_ws_sel_atoms_list_sel_changed() {

  app_state_t* astate = app_state_t::get_inst();
  auto [cur_ws, cur_item, as_al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>();

  bool need_to_hide_al_cntls{true};
  bool need_to_hide_force_sel_lbl_vis{true};
  bool need_to_hide_atom_override{true};
  bool need_to_hide_add_primitives{true};
  bool need_to_hide_make_psg{true};

  if (ok) {

    /* detect selective labels */
    need_to_hide_force_sel_lbl_vis =
        cur_ws->m_edit_type == ws_edit_e::edit_item || as_al->m_atom_idx_sel.empty();

    if (!need_to_hide_force_sel_lbl_vis) {

      m_tp_force_sel_lbl_vis->show();

      m_tp_force_sel_lbl_vis->blockSignals(true);
      m_tp_force_sel_lbl_vis->setChecked(
          !as_al->any_of_sel_xfield_equal<bool>(xgeom_label_show, false)
          );
      m_tp_force_sel_lbl_vis->blockSignals(false);
      /* end of detect selective labels */

    }

    need_to_hide_make_psg = need_to_hide_force_sel_lbl_vis;
    m_tp_add_point_sym_group->show();

    /* detect atom override */
    if (!(as_al->m_atom_idx_sel.empty() || cur_ws->m_edit_type == ws_edit_e::edit_item)) {

      need_to_hide_atom_override = false;

      m_tp_toggle_atom_override->show();
      m_tp_toggle_atom_override->blockSignals(true);

      m_tp_toggle_atom_override->setChecked(
          !as_al->any_of_sel_xfield_equal<bool>(xgeom_override, false)
          );

      m_tp_toggle_atom_override->blockSignals(false);

    }
    /* end of detect atom override */

    /* add cube or arrow between 2 atoms */
    if (as_al->m_atom_idx_sel.size() == 2 && cur_ws->m_edit_type == ws_edit_e::edit_content) {

      m_tp_add_arrow->show();
      m_tp_add_cube->show();
      need_to_hide_add_primitives = false;

      m_tp_msr_angle->hide();
      m_tp_msr_dist->show();
      need_to_hide_al_cntls = false;
      auto it1 = as_al->m_atom_idx_sel.begin();
      auto it2 = ++(as_al->m_atom_idx_sel.begin());

      auto cur_sel =
          as_al->m_measure->is_bond_msr_exists(it1->m_atm, it2->m_atm, it1->m_idx, it2->m_idx);

      m_tp_msr_dist->blockSignals(true);
      m_tp_msr_dist->setChecked(cur_sel != std::nullopt);
      m_tp_msr_dist->blockSignals(false);

    }
    /* end of add cube or arrow between 2 atoms */

    /* angle between 3 atoms */
    if (as_al->m_atom_idx_sel.size() == 3 &&
        cur_ws->m_edit_type == ws_edit_e::edit_content &&
        as_al->m_atom_ord_sel.size() == 3) {

      need_to_hide_al_cntls = false;

      auto cur_sel = as_al->m_measure->is_angle_msr_exists(as_al->m_atom_ord_sel[0].m_atm,
                                                           as_al->m_atom_ord_sel[1].m_atm,
                                                           as_al->m_atom_ord_sel[2].m_atm,
                                                           as_al->m_atom_ord_sel[0].m_idx,
                                                           as_al->m_atom_ord_sel[1].m_idx,
                                                           as_al->m_atom_ord_sel[2].m_idx);

      m_tp_msr_dist->hide();
      m_tp_msr_angle->show();
      m_tp_msr_angle->blockSignals(true);
      m_tp_msr_angle->setChecked(cur_sel != std::nullopt);
      m_tp_msr_angle->blockSignals(false);

    }
    /* end of angle between 3 atoms */

  }

  if (need_to_hide_add_primitives) {

    m_tp_add_arrow->hide();
    m_tp_add_cube->hide();

  }

  if (need_to_hide_al_cntls) {

    m_tp_msr_dist->hide();
    m_tp_msr_angle->hide();
    m_tp_add_arrow->hide();
    m_tp_add_cube->hide();

  }

  if (need_to_hide_force_sel_lbl_vis) {
    m_tp_force_sel_lbl_vis->hide();
  }

  if (need_to_hide_atom_override) {
    m_tp_toggle_atom_override->hide();
  }

  if (need_to_hide_make_psg) {
    m_tp_add_point_sym_group->hide();
  }

}

void main_window_t::tp_dist_button_clicked(bool checked) {

  app_state_t* astate = app_state_t::get_inst();
  auto [cur_ws, cur_item, as_al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>();
  if (!ok) return;

  if (as_al->m_atom_idx_sel.size() == 2 && cur_ws->m_edit_type == ws_edit_e::edit_content) {

    m_tp_msr_dist->show();

    auto it1 = as_al->m_atom_idx_sel.begin();
    auto it2 = ++(as_al->m_atom_idx_sel.begin());

    auto cur_sel = as_al->m_measure->is_bond_msr_exists(it1->m_atm,
                                                        it2->m_atm,
                                                        it1->m_idx,
                                                        it2->m_idx);

    if (checked) as_al->m_measure->add_bond_msr(it1->m_atm,
                                     it2->m_atm,
                                     it1->m_idx,
                                     it2->m_idx);

    else as_al->m_measure->rm_bond_msr(*cur_sel);

  }


  astate->make_viewport_dirty();

}

void main_window_t::tp_angle_button_clicked(bool checked) {

  app_state_t* astate = app_state_t::get_inst();
  auto [cur_ws, cur_item, as_al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>();
  if (!ok) return;

  if (as_al->m_atom_idx_sel.size() == 3 && cur_ws->m_edit_type == ws_edit_e::edit_content) {

    m_tp_msr_angle->show();

    auto cur_sel = as_al->m_measure->is_angle_msr_exists(as_al->m_atom_ord_sel[0].m_atm,
                                                         as_al->m_atom_ord_sel[1].m_atm,
                                                         as_al->m_atom_ord_sel[2].m_atm,
                                                         as_al->m_atom_ord_sel[0].m_idx,
                                                         as_al->m_atom_ord_sel[1].m_idx,
                                                         as_al->m_atom_ord_sel[2].m_idx);

    if (checked) as_al->m_measure->add_angle_msr(as_al->m_atom_ord_sel[0].m_atm,
                                      as_al->m_atom_ord_sel[1].m_atm,
                                      as_al->m_atom_ord_sel[2].m_atm,
                                      as_al->m_atom_ord_sel[0].m_idx,
                                      as_al->m_atom_ord_sel[1].m_idx,
                                      as_al->m_atom_ord_sel[2].m_idx);

    else as_al->m_measure->rm_angle_msr(*cur_sel);

  }

  astate->make_viewport_dirty();

}

void main_window_t::ws_edit_mode_selector_button_clicked(int id) {

  app_state_t* astate = app_state_t::get_inst();
  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_mbox);

  if (ok) {
    if (id == 0) cur_ws->m_edit_type = ws_edit_e::edit_item;
    else cur_ws->m_edit_type = ws_edit_e::edit_content;
  }

  astate->astate_evd->cur_ws_edit_type_changed();
  cur_ws_props_changed();

}

void main_window_t::tp_force_sel_lbl_vis_button_clicked(bool checked) {

  app_state_t* astate = app_state_t::get_inst();
  auto [cur_ws, cur_item, as_al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>();
  if (!ok) return;

  if (as_al && cur_ws->m_edit_type == ws_edit_e::edit_content)
    for (auto &rec : as_al->m_atom_idx_sel)
      as_al->m_geom->xfield<bool>(xgeom_label_show, rec.m_atm) = checked;

  // if selective labels rendering unchecked - force it and select some random style
  if (!as_al->m_atom_idx_sel.empty() && !as_al->m_labels->m_selective_lbl.get_value()) {

    as_al->m_labels->m_selective_lbl.set_value(true);
    as_al->m_labels->m_style.set_value(geom_labels_style_e::show_id_type);
    astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();

  }

  astate->make_viewport_dirty();

}

void main_window_t::tp_toggle_atom_override_button_clicked(bool checked) {

  app_state_t* astate = app_state_t::get_inst();

  auto [cur_ws, cur_item, as_al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>();
  if (!ok || cur_ws->m_edit_type == ws_edit_e::edit_item) return;

  for (auto &rec : as_al->m_atom_idx_sel)
    if (rec.m_atm < as_al->m_geom->nat()) {

      as_al->m_geom->xfield<bool>(xgeom_override, rec.m_atm) = checked;

      if (as_al->m_geom->xfield<float>(xgeom_atom_r, rec.m_atm) < 0.01f) {

        auto ap_idx = ptable::number_by_symbol(as_al->m_geom->atom(rec.m_atm));
        float _rad = 1.0f;
        if (ap_idx) _rad = ptable::get_inst()->arecs[*ap_idx - 1].m_radius;
        as_al->m_geom->xfield<float>(xgeom_atom_r, rec.m_atm) = _rad;

      }

    }

  astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();
  astate->make_viewport_dirty();

}

void main_window_t::tp_camera_tool_button_triggered(QAction *action) {

  if (!action) return;

  qextended_action *ext_act = qobject_cast<qextended_action*>(action);
  if (!ext_act) return;

  app_state_t* astate = app_state_t::get_inst();

  auto [cur_ws, cur_item, as_al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>();
  if (!ok) return;

  cam_tv_e _tv = static_cast<cam_tv_e>(ext_act->m_joined_data[0]);

  if (as_al) as_al->apply_target_view(_tv);

  astate->make_viewport_dirty();

}

void main_window_t::tp_fast_forward_anim_clicked() {

  app_state_t* astate = app_state_t::get_inst();
  auto [cur_ws, cur_itm, as_al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>();

  if (!as_al || !as_al->m_anim->animable()) return;

  as_al->m_anim->m_cur_anim = as_al->m_anim->get_total_anims()-1;
  as_al->m_anim->update_current_frame_to_end();

  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();

}

void main_window_t::tp_add_arrow_clicked() {

  simple_query::embed_arrow();

}

void main_window_t::tp_add_cube_clicked() {

  simple_query::embed_cube();

}

void main_window_t::tp_add_point_sym_group_clicked() {

  simple_query::make_psg_view(0.1);

}

void main_window_t::tp_scenic_rotation_toggle() {

  app_state_t* astate = app_state_t::get_inst();
  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws();
  cur_ws->m_scenic_rotation = !cur_ws->m_scenic_rotation;

}

void main_window_t::apply_camera_view_change(cam_tv_e target_view) {

}

void main_window_t::toggle_ws_edit_mode() {

  app_state_t* astate = app_state_t::get_inst();
  //astate->log("RERERERERER");
  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws();

  if (ok) {
    cur_ws->toggle_edit_mode();
    cur_ws_changed();
  }

}

void main_window_t::toggle_fullscreen(bool checked) {

  if (checked) {

    setWindowState(windowState() | Qt::WindowFullScreen);

  } else {

    setWindowFlags(Qt::CustomizeWindowHint |
                   Qt::WindowStaysOnTopHint |
                   Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint |
                   Qt::WindowCloseButtonHint);
    show();

  }

}

void main_window_t::toggle_immersive_mode() {

  app_state_t* astate = app_state_t::get_inst();

  //  if (!astate->ws_mgr->has_wss()) return;

  astate->m_immersive_mode = !astate->m_immersive_mode;

  if (astate->m_immersive_mode) {

    menuBar()->hide();
    m_tp_wdgt->hide();
    m_ws_tabbar_wdgt->hide();
    m_obj_insp_wdgt->hide();
    m_view_menu_toggle_fullscreen->setChecked(true);

  } else {

    m_tp_wdgt->show();
    m_ws_tabbar_wdgt->show();
    m_obj_insp_wdgt->show();
    menuBar()->show();
    m_view_menu_toggle_fullscreen->setChecked(false);

  }

}

void main_window_t::start_update_cycle() {

  if (m_ws_viewer_wdgt && m_ws_viewer_wdgt->m_update_timer) {
    m_ws_viewer_wdgt->m_update_timer->start();
  }

}

void main_window_t::stop_update_cycle() {

  if (m_ws_viewer_wdgt && m_ws_viewer_wdgt->m_update_timer) {

    p_elapsed_time_in_event_loop =  m_ws_viewer_wdgt->m_update_timer->remainingTime();
    m_ws_viewer_wdgt->m_update_timer->stop();
    m_ws_viewer_wdgt->m_update_timer->setInterval(p_elapsed_time_in_event_loop);

  }

}

void main_window_t::act_sel_all_cnt() {

  app_state_t* astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>();
  if (al) al->sel_atoms(true);

}

void main_window_t::act_unsel_all_cnt() {

  app_state_t* astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>();
  if (al) al->sel_atoms(false);

}

void main_window_t::act_invert_sel_cnt() {

  app_state_t* astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>();
  if (al) al->inv_sel_atoms();

}

void main_window_t::act_toggle_console() {

  app_state_t* astate = app_state_t::get_inst();

  if (m_py_console_wdgt->isVisible()) {
    m_py_console_wdgt->hide();
    astate->m_show_console = true;
  } else {
    m_py_console_wdgt->show();
    m_py_console_wdgt->m_py_tedit->setFocus();
    astate->m_show_console = false;
  }

}

void main_window_t::rebuild_recent_files_menu() {

  if (!m_file_menu_recent_files) return;

  for (auto &elem : m_file_menu_recent_entries)
    elem->setVisible(false);

  app_state_t* astate = app_state_t::get_inst();

  for (int i = 0; i < astate->m_recent_files.size(); i++) {

    m_file_menu_recent_entries[i]->setVisible(true);
    std::string ff_name = astate->ws_mgr->m_bhv_mgr->get_ff_full_name(
        astate->m_recent_files[i].m_ff_id);

    std::string rec_menu_entry;
    if (!astate->m_recent_files[i].m_native)
      rec_menu_entry = fmt::format("{}) {} ({})",
                                   i,
                                   astate->m_recent_files[i].m_file_name,
                                   ff_name);
    else
      rec_menu_entry = fmt::format("{}) {} (qpp::cad workspace)",
                                   i,
                                   astate->m_recent_files[i].m_file_name);

    m_file_menu_recent_entries[i]->setText(QString::fromStdString(rec_menu_entry));

  }

}

void main_window_t::recent_files_clicked() {

  int idx = -1;
  app_state_t* astate = app_state_t::get_inst();

  QObject* obj = sender();
  for (int i = 0 ; i < m_file_menu_recent_entries.size(); i++)
    if (m_file_menu_recent_entries[i]->isVisible() && m_file_menu_recent_entries[i] == obj) {
      idx = i;
    }

  if (idx != -1 && idx < astate->m_recent_files.size()) {

    auto &rec_idx = astate->m_recent_files[idx];

    if (rec_idx.m_native) astate->ws_mgr->load_from_file(rec_idx.m_file_name, false);
    else {

      auto bhv_id = astate->ws_mgr->m_bhv_mgr->get_io_bhv_by_file_format(rec_idx.m_ff_id);
      if (bhv_id)
        astate->ws_mgr->import_from_file(rec_idx.m_file_name, *bhv_id, true, true);
      else
        astate->ws_mgr->load_from_file_autodeduce(rec_idx.m_file_name, "", true, true);

    }

  }

}

void main_window_t::inline_tool_left_ctrl_visibility(bool visible) {

  m_inline_left_tool->setVisible(visible);

}

void main_window_t::inline_tool_bottom_ctrl_visibility(bool visible) {

  m_inline_btm_tool->setVisible(visible);

}

void main_window_t::modern_menu_clicked() {

  m_modern_menu->exec(QCursor::pos());

}

void main_window_t::build_bhv_menus_and_actions() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_mgr->m_bhv_mgr.get();

  if (!bhv_mgr) return;

  //init groups for IMPORTED TO WS
  for (auto &ff_grp : bhv_mgr->m_file_format_groups) {

    //spawn top-level menus
    QMenu *new_menu_as_new_ws =
        m_file_menu_import_as_new_ws->addMenu(QString::fromStdString(ff_grp.second.m_full_name));

    QMenu *new_menu_to_cur_ws =
        m_file_menu_import_to_cur_ws->addMenu(QString::fromStdString(ff_grp.second.m_full_name));

    QMenu *new_menu_export_selected =
        m_file_menu_export_sel_as->addMenu(QString::fromStdString(ff_grp.second.m_full_name));

    m_file_menu_import_as_new_ws_menus.emplace(ff_grp.first, new_menu_as_new_ws);
    m_file_menu_import_to_cur_ws_menus.emplace(ff_grp.first, new_menu_to_cur_ws);
    m_file_menu_export_sel_as_menus.emplace(ff_grp.first, new_menu_export_selected);

    //iterate over file formats from group
    for (auto &ff : ff_grp.second.m_ffs_lookup)

      for (size_t i = 0; i < bhv_mgr->m_ws_item_io.size(); i++) {

        //deduce import to current ws
        if (bhv_mgr->m_ws_item_io[i]->m_accepted_file_format == ff &&
            bhv_mgr->m_ws_item_io[i]->can_load() &&
            bhv_mgr->m_ws_item_io[i]->m_menu_occupier &&
            bhv_mgr->m_ws_item_io[i]->m_can_be_imported_to_ws) {

          qextended_action *new_act = new qextended_action((nullptr));
          new_act->m_joined_data[0] = i;
          connect(new_act, &QAction::triggered,
                  this, &main_window_t::act_bhv_import_to_cur_ws);
          new_act->setText(
              QString::fromStdString(bhv_mgr->m_file_formats[ff].m_full_name));
          new_menu_to_cur_ws->addAction(new_act);

        }

        //deduce import as new ws
        if (bhv_mgr->m_ws_item_io[i]->m_accepted_file_format == ff &&
            bhv_mgr->m_ws_item_io[i]->can_load() &&
            bhv_mgr->m_ws_item_io[i]->m_menu_occupier &&
            bhv_mgr->m_ws_item_io[i]->m_can_be_imported_as_new_ws) {

          qextended_action *new_act = new qextended_action((nullptr));
          new_act->m_joined_data[0] = i;
          connect(new_act, &QAction::triggered,
                  this, &main_window_t::act_bhv_import_as_new_ws);
          new_act->setText(
              QString::fromStdString(bhv_mgr->m_file_formats[ff].m_full_name));
          new_menu_as_new_ws->addAction(new_act);

        }

        //deduce save selected item
        if (bhv_mgr->m_ws_item_io[i]->m_accepted_file_format == ff &&
            bhv_mgr->m_ws_item_io[i]->can_save() &&
            bhv_mgr->m_ws_item_io[i]->m_menu_occupier) {

          qextended_action *new_act = new qextended_action((nullptr));
          new_act->m_joined_data[0] = i;
          connect(new_act, &QAction::triggered,
                  this, &main_window_t::act_bhv_export_sel);
          new_act->setText(
              QString::fromStdString(bhv_mgr->m_file_formats[ff].m_full_name));
          new_menu_export_selected->addAction(new_act);
          m_file_menu_export_sel_as_acts.push_back(new_act);

        }

      }

  }

  //TODO: make lookup for bhv

}

void main_window_t::build_bhv_tools_menus() {

  app_state_t* astate = app_state_t::get_inst();

  using pss = std::tuple<size_t, std::string>;
  std::vector<std::tuple<size_t, std::string> > tool_grp_sort, tool_sort;

  for (auto &ffg : astate->ws_mgr->m_bhv_mgr->m_tools_groups)
    tool_grp_sort.push_back({ffg.first, ffg.second.m_full_name});

  std::sort(std::begin(tool_grp_sort),
            std::end(tool_grp_sort),
            [](const pss &a, const pss &b){return std::get<1>(a) > std::get<1>(b);});

  //construct tools groups
  for (auto &ffg : tool_grp_sort) {
    //astate->ws_mgr->m_bhv_mgr->m_tools_groups
    QMenu *new_menu = m_tools_menu->addMenu(
        QString::fromStdString(
            astate->ws_mgr->m_bhv_mgr->m_tools_groups[std::get<0>(ffg)].m_full_name)
        );
    m_tools_menu_grps.emplace(std::get<0>(ffg), new_menu);
  }


  for (auto &ff : astate->ws_mgr->m_bhv_mgr->m_tools_info)
    if (ff.second.m_show_in_menu) tool_sort.push_back({ff.first, ff.second.m_full_name});

  std::sort(std::begin(tool_sort),
            std::end(tool_sort),
            [](const pss &a, const pss &b){return std::get<1>(a) > std::get<1>(b);});

  //construct tools actions
  for (auto &ff : tool_sort) {

    //construct tool's action
    qextended_action *new_act = new qextended_action((nullptr));
    size_t tool_hash = std::get<0>(ff);

    new_act->m_joined_data[0] = tool_hash;

    connect(new_act, &QAction::triggered,
            this, &main_window_t::act_bhv_tools_menus_clicked);

    new_act->setText(
        QString::fromStdString(
            astate->ws_mgr->m_bhv_mgr->m_tools_info[tool_hash].m_full_name)
        );

    //locate menu for group
    auto it_g = m_tools_menu_grps.find(
        astate->ws_mgr->m_bhv_mgr->m_tools_info[tool_hash].m_group_hash
        );

    if (it_g != m_tools_menu_grps.end()) it_g->second->addAction(new_act);
    else m_tools_menu->addAction(new_act);

    m_tools_menu_acts.push_back(new_act);

  }

}

void main_window_t::build_bhv_tool_panel() {

  app_state_t *astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_mgr->m_bhv_mgr.get();

  m_toolbar_elements.reserve(bhv_mgr->m_toolbar_elements_info.size());

  for (auto &tb_info : bhv_mgr->m_toolbar_elements_info) {

    auto new_tb = tb_info.second.m_fabric();
    new_tb->init_element(m_tp_wdgt);
    m_toolbar_elements.push_back(new_tb);

  }

}

void main_window_t::act_bhv_tools_menus_clicked() {

  app_state_t* astate = app_state_t::get_inst();
  auto [cur_ws, cur_it, ok] = astate->ws_mgr->get_sel_tpl_itm_nc(error_ctx_ignore);

  //  if (!ok) {
  //   return;
  //  }

  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_mgr->m_bhv_mgr.get();

  qextended_action *ext_act = qobject_cast<qextended_action*>(sender());
  if (!ext_act) return;

  size_t t_hash = ext_act->m_joined_data[0];

  auto it_t = bhv_mgr->m_tools_info.find(t_hash);
  if (it_t != bhv_mgr->m_tools_info.end()) {

    if (it_t->second.m_item_required && !cur_it) {
      return;
    }

    switch (it_t->second.m_tool_type) {

    case ws_item_tool_invalid:
      break;

    case ws_item_tool_modal:
      bhv_mgr->exec_tool(cur_it.get(), t_hash);
      break;

    case ws_item_tool_inline_vertical:
      process_bhv_tool(it_t->first, cur_it.get());
      break;

    case ws_item_tool_popup:
      break;

    case ws_item_tool_inline_horizontal:
      process_bhv_tool(it_t->first, cur_it.get());
      break;

    }

  }

}

void main_window_t::process_bhv_tool(size_t tool_id, ws_item_t *sel_item) {

  app_state_t *astate = app_state_t::get_inst();
  auto &bhv_mgr = *astate->ws_mgr->m_bhv_mgr.get();

  //locate tool info
  auto it = bhv_mgr.m_tools_info.find(tool_id);
  if (it == bhv_mgr.m_tools_info.end()) {
    //invalid tool
    return;
  }

  if (it->second.m_tool_type == ws_item_tool_type_e::ws_item_tool_invalid ||
      it->second.m_tool_type == ws_item_tool_type_e::ws_item_tool_modal ||
      it->second.m_tool_type == ws_item_tool_type_e::ws_item_tool_popup) {
    //we can process only inline tools here
    return;

  }

  if (it->second.m_item_required) {
    if (!sel_item || it->second.m_accepted_type != sel_item->get_type())
      return;
  }

  std::shared_ptr<ws_item_inline_tool_widget_t> target{nullptr};
  //locate tool if exist
  auto it_trq = m_inline_tools.find(tool_id);
  if (it_trq != m_inline_tools.end() && it_trq->second != nullptr) {
    target = it_trq->second;
  }

  //try to construct
  if (!target) {

    auto it_fit = bhv_mgr.m_tools_info.find(tool_id);
    if (it_fit != bhv_mgr.m_tools_info.end()) {

      auto ws_item_tool = it_fit->second.m_fabric();
      target = std::shared_ptr<ws_item_inline_tool_widget_t>{ws_item_tool->construct_inline_tool()};
      m_inline_tools.insert({tool_id, target});

    }

  }

  if (!target) {
    return;
  }

  if (it->second.m_tool_type == ws_item_tool_type_e::ws_item_tool_inline_vertical) {

    if (m_inline_left_tool->m_cur_wdgt) {
      m_inline_left_tool->m_cur_wdgt->cancel();
      m_inline_left_tool->m_cur_wdgt->setVisible(false);
      m_inline_left_tool->m_main_lt->removeWidget(m_inline_left_tool->m_cur_wdgt);
    }

    m_inline_left_tool->m_cur_wdgt = target.get();
    m_inline_left_tool->m_main_lt->insertWidget(0, m_inline_left_tool->m_cur_wdgt);
    m_inline_left_tool->m_cur_wdgt->setVisible(true);
    m_inline_left_tool->m_ew_header->setText(QString::fromStdString(it->second.m_full_name));
    m_inline_left_tool->m_cur_wdgt->bind_item(sel_item);
    inline_tool_left_ctrl_visibility(true);
    inline_tool_bottom_ctrl_visibility(false);

  } else if (it->second.m_tool_type == ws_item_tool_type_e::ws_item_tool_inline_horizontal) {

    if (m_inline_btm_tool->m_cur_wdgt) {
      m_inline_btm_tool->m_cur_wdgt->cancel();
      m_inline_btm_tool->m_cur_wdgt->setVisible(false);
      m_inline_btm_tool->m_main_lt->removeWidget(m_inline_left_tool->m_cur_wdgt);
    }

    m_inline_btm_tool->m_cur_wdgt = target.get();
    m_inline_btm_tool->m_main_lt->insertWidget(0, m_inline_btm_tool->m_cur_wdgt);
    m_inline_btm_tool->m_cur_wdgt->setVisible(true);
    m_inline_btm_tool->m_ew_header->setText(QString::fromStdString(it->second.m_full_name));
    m_inline_btm_tool->m_cur_wdgt->bind_item(sel_item);
    inline_tool_left_ctrl_visibility(false);
    inline_tool_bottom_ctrl_visibility(true);

  }

}

void main_window_t::control_bhv_tools_menus_activity() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_mgr->m_bhv_mgr.get();

  std::shared_ptr<workspace_t> cur_ws = astate->ws_mgr->get_cur_ws();
  std::shared_ptr<ws_item_t> cur_it{nullptr};
  if (cur_ws) cur_it = cur_ws->get_sel_sp();

  for (auto elem : m_tools_menu_acts) {

    elem->setEnabled(false);

    if (cur_ws) {

      auto it_t = bhv_mgr->m_tools_info.find(elem->m_joined_data[0]);
      if (it_t != bhv_mgr->m_tools_info.end()) {

        if (!it_t->second.m_item_required) elem->setEnabled(true);
        if (cur_it
            && it_t->second.m_item_required
            && it_t->second.m_accepted_type == cur_it->get_type()
            && it_t->second.m_can_apply(cur_it.get())) elem->setEnabled(true);
      }

    }

  }

}

void main_window_t::act_bhv_import_to_cur_ws() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_mgr->m_bhv_mgr.get();

  qextended_action *ext_act = qobject_cast<qextended_action*>(sender());
  if (!ext_act) return;

  size_t b_id = ext_act->m_joined_data[0];

  //check that bhv is valid
  if (b_id < bhv_mgr->m_ws_item_io.size() &&
      bhv_mgr->m_ws_item_io[b_id]->can_load() &&
      bhv_mgr->m_ws_item_io[b_id]->m_can_be_imported_to_ws) {

    std::string file_name =
        QFileDialog::getOpenFileName(this,
                                     "Import to current workspace",
                                     astate->m_last_dir,
                                     "*").toStdString();
    if (!file_name.empty())
      astate->ws_mgr->import_from_file(file_name, b_id, false, false);

  }

}

void main_window_t::act_bhv_import_as_new_ws() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_mgr->m_bhv_mgr.get();

  qextended_action *ext_act = qobject_cast<qextended_action*>(sender());
  if (!ext_act) return;

  size_t b_id = ext_act->m_joined_data[0];

  //check that bhv is valid
  if (b_id < bhv_mgr->m_ws_item_io.size()
      && bhv_mgr->m_ws_item_io[b_id]->can_load()
      && bhv_mgr->m_ws_item_io[b_id]->m_can_be_imported_as_new_ws) {

    // astate->log(fmt::format("{}", b_id));
    std::string file_name = QFileDialog::getOpenFileName(this,
                                                         "Import as new workspace",
                                                         astate->m_last_dir,
                                                         "*").toStdString();
    if (!file_name.empty())
      astate->ws_mgr->import_from_file(file_name, b_id, true, true);

  }

}

void main_window_t::act_bhv_export_sel() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_mgr->m_bhv_mgr.get();

  qextended_action *ext_act = qobject_cast<qextended_action*>(sender());
  if (!ext_act) return;

  auto [cur_ws, cur_it, ok] = astate->ws_mgr->get_sel_tpl_itm_nc();

  if (!ok) return;

  size_t b_id = ext_act->m_joined_data[0];

  //check that bhv is valid
  if (b_id < bhv_mgr->m_ws_item_io.size() &&
      bhv_mgr->m_ws_item_io[b_id]->can_save() &&
      bhv_mgr->m_ws_item_io[b_id]->m_accepted_type == cur_it->get_type()) {

    QString qfile_name = QFileDialog::getSaveFileName(this,
                                                      "Export selected",
                                                      astate->m_last_dir
                                                      );

    if (qfile_name.size() != 0) {

      QFileInfo file_nfo(qfile_name);
      astate->m_last_dir = file_nfo.absoluteDir().canonicalPath();
      std::string file_name = qfile_name.toStdString();
      astate->ws_mgr->save_ws_item_to_file(file_name, cur_it, b_id);

    }

  }

}

void main_window_t::control_bhv_menus_activity() {

  app_state_t* astate = app_state_t::get_inst();

  if (!astate->ws_mgr) return;

  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_mgr->m_bhv_mgr.get();

  auto [cur_ws, cur_it, ok] = astate->ws_mgr->get_sel_tpl_itm_nc();

  if (cur_ws) m_file_menu_import_to_cur_ws->setEnabled(true);
  else m_file_menu_import_to_cur_ws->setEnabled(false);

  if (!ok) {
    //
    m_file_menu_export_sel_as->setEnabled(false);
  } else {

    if (cur_it) {

      m_file_menu_export_sel_as->setEnabled(true);

      for (auto &exp_act : m_file_menu_export_sel_as_acts) {

        size_t bhv_id = exp_act->m_joined_data[0];

        if (bhv_mgr->m_ws_item_io[bhv_id]->can_save() &&
            bhv_mgr->m_ws_item_io[bhv_id]->m_accepted_type == cur_it->get_type()) {
          //need_to_enable_export_menu = true;
          exp_act->setEnabled(true);
          //save item dialog
        } else {
          exp_act->setEnabled(false);
        }

      }

    }

  } // not ok

}

void main_window_t::make_screenshot() {

  app_state_t* astate = app_state_t::get_inst();
  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws(error_ctx_mbox);

  if (!ok) return;

  astate->m_ignore_scanline = true;

  QDateTime date = QDateTime::currentDateTime();
  QString date_s = date.toString("dd_MM_yyyy_hh_mm_ss");
  m_ws_viewer_wdgt->grabFramebuffer().save(tr("%1_screenshot.png").arg(date_s));

  astate->m_ignore_scanline = false;
  astate->make_viewport_dirty();

}

void main_window_t::slot_shortcut_terminate_app() {

  QApplication::quit();

}
