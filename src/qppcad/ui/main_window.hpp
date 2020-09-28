#ifndef QPPCAD_MAIN_WINDOW_H
#define QPPCAD_MAIN_WINDOW_H

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ui/ws_viewer_widget.hpp>
#include <qppcad/ui/object_inspector_widget.hpp>
#include <qppcad/python/python_console_widget.hpp>
#include <qppcad/ui/qextended_action.hpp>
#include <qppcad/ws_item/ws_item_extended_editor_compositor.hpp>
#include <qppcad/render/camera.hpp>
#include <qppcad/ui/ws_tabbar.hpp>
#include <qppcad/ui/log_widget.hpp>
#include <qppcad/ui/qinline_tool_window.hpp>
#include <qppcad/ui/ws_item_inline_tool_widget.hpp>

#include <QMainWindow>
#include <QShortcut>
#include <QApplication>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QMimeData>
#include <QFileDialog>
#include <QTimer>

namespace qpp {

namespace cad {

class toolbar_element_t;

struct stored_action_hotkeys_t {
  QAction *act{};
  QShortcut *shortcut{};
};

class main_window_t : public QMainWindow {

  Q_OBJECT

private:

  int p_elapsed_time_in_event_loop{0};

public:

  std::vector<stored_action_hotkeys_t> m_shortcuts;

  QTimer *m_timer_one_sec;

  //Menu area widget
  QWidget *m_menu_wdgt_hldr;
  QVBoxLayout *m_menu_wdgt_hldr_lt;
  QLabel *m_menu_wdgt_overview;

  //Widgets
  QWidget *m_main_wdgt; //main application widget
  QFrame *m_tp_wdgt; //tool panel widget
  QWidget *m_wstabbar_overview_wrp;
  QHBoxLayout *m_wstabbar_overview_wrp_lt;
  QVBoxLayout *m_tp_lt; //tool panel layout
  //QLabel *m_tp_overview; //tool panel overbiew information(right text)
  QPushButton *m_tp_modern_menu;

  QFrame *m_tp_edit_mode_start;
  QButtonGroup *m_tp_edit_mode;
  QPushButton *m_tp_edit_mode_item;
  QPushButton *m_tp_edit_mode_cnt;
  QFrame *m_tp_edit_mode_end;

  QPushButton *m_tp_print_screen;
  QToolButton *m_tp_cam_tool;
  qextended_action *m_tp_cam_tool_act_x;
  qextended_action *m_tp_cam_tool_act_y;
  qextended_action *m_tp_cam_tool_act_z;
  qextended_action *m_tp_cam_tool_act_cc;
  qextended_action *m_tp_cam_tool_act_a;
  qextended_action *m_tp_cam_tool_act_b;
  qextended_action *m_tp_cam_tool_act_c;
  QPushButton *m_tp_scenic_rot_cam;
  QFrame *m_tp_utility_frame_end;

  QPushButton *m_tp_anim_fast_forward;
  QPushButton *m_tp_force_sel_lbl_vis;
  QPushButton *m_tp_toggle_atom_override;
  QPushButton *m_tp_msr_dist;
  QPushButton *m_tp_msr_angle;
  QPushButton *m_tp_add_cube;
  QPushButton *m_tp_add_arrow;
  QPushButton *m_tp_add_point_sym_group;

  QWidget *m_ws_viewer_plch;
  QWidget *m_obj_inst_plch;
  qinline_tool_window_t *m_inline_left_tool;
  qinline_tool_window_t *m_inline_btm_tool;
  std::map<size_t, std::shared_ptr<ws_item_inline_tool_widget_t> > m_inline_tools;
  ws_item_extended_editor_compositor_t *m_ext_edtr_compositor;

  python_console_widget_t *m_py_console_wdgt;
  QFrame *m_ws_viewer_wdgt_frame;
  QHBoxLayout *m_ws_viewer_wdgt_frame_lt;
  ws_viewer_widget_t *m_ws_viewer_wdgt;
  object_inspector_widget_t *m_obj_insp_wdgt;

  ws_tabbar_t *m_ws_tabbar_wdgt;
  log_widget_t *m_log_wdgt;
  //End of widgets

  //Menus
  QMenu *m_modern_menu;
  std::map<std::string, QAction*> m_actions_lookup;
  QMenu *m_file_menu;
  QAction *m_file_menu_new_ws;
  QAction *m_file_menu_close_ws;
  QAction *m_file_menu_close_all_ws;
  QAction *m_file_menu_open_ws;

  QMenu *m_file_menu_import_as_new_ws;
  std::map<size_t, QMenu*> m_file_menu_import_as_new_ws_menus;
  std::vector<qextended_action*> m_file_menu_import_as_new_ws_acts;

  QMenu *m_file_menu_import_to_cur_ws;
  std::map<size_t, QMenu*> m_file_menu_import_to_cur_ws_menus;
  std::vector<qextended_action*> m_file_menu_import_to_cur_ws_acts;

  QMenu *m_file_menu_export_sel_as;
  std::map<size_t, QMenu*> m_file_menu_export_sel_as_menus;
  std::vector<qextended_action*> m_file_menu_export_sel_as_acts;

  QAction *m_file_menu_save_ws;
  QAction *m_file_menu_save_ws_as;
  QMenu *m_file_menu_recent_files;
  std::array<QAction*, max_recent_files> m_file_menu_recent_entries;
  QAction *m_file_menu_close_app;

  QMenu *m_edit_menu;
  QAction *m_edit_menu_undo;
  QAction *m_edit_menu_redo;
  QAction *m_edit_menu_switch_ws_edit_mode;
  QAction *m_edit_menu_ptable_wdgt;
  QMenu *m_edit_menu_sel;
  QAction *m_edit_menu_sel_sel_all;
  QAction *m_edit_menu_sel_unsel_all;
  QAction *m_edit_menu_sel_invert;

  QMenu *m_tools_menu;
  std::map<size_t, QMenu*> m_tools_menu_grps;
  std::vector<qextended_action*> m_tools_menu_acts;

  QMenu *m_ws_menu;
  QAction *m_ws_menu_rename_ws;
  QAction *m_ws_menu_bg_color;
  QAction *m_ws_copy_cam;

  QMenu *m_view_menu;
  QAction *m_view_menu_show_modern_menu;
  QAction *m_view_menu_show_oi;
  QAction *m_view_menu_show_gizmo;
  QAction *m_view_menu_toggle_fullscreen;
  QAction *m_view_menu_console;
  QAction *m_view_menu_log_wdgt;
  QAction *m_view_menu_settings;
  QAction *m_view_menu_ws_settings;
  QMenu *m_view_menu_debug;
  QAction *m_view_menu_toggle_debug_tws_tree;

  QMenu *m_help_menu;
  QAction *m_help_menu_about;
  // End of Menus

  // Layouts
  QGridLayout *m_tools_main_window_lt;
  QVBoxLayout *m_main_lt;
  QSplitter *m_layout_ws_viewer_obj_insp;
  QSplitter *m_splitter_ws_viewer_ext_edt;
  QSplitter *m_splitter_editor_py_console;
  QSplitter *m_splitter_ws_viewer_py_console_log;
  QSplitter *m_splitter_py_console_log_wdgt;
  // End of Layouts

  // Shortcuts
  QShortcut *m_sc_terminate_app;
  QShortcut *m_sc_toggle_console;
  QShortcut *m_sc_enter_immersive_mode;
  // End of Shortcuts

  // Toolpanel
  std::vector<std::shared_ptr<toolbar_element_t> > m_toolbar_elements;
  // End of Toolpanel

  explicit main_window_t(QWidget *parent = nullptr);
  ~main_window_t() override;

  void init_base_shortcuts();
  void init_menus();
  void init_widgets();
  void init_layouts();

protected:

  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dragLeaveEvent(QDragLeaveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

public slots:

  void timer_one_sec_shoot();

  void slot_shortcut_terminate_app();

  void wss_changed_slot();

  void ws_selector_selection_changed(int index);
  void show_obj_insp_state_changed(bool checked);
  void show_gizmo_state_changed(bool checked);
  void show_log_wdgt_state_changed(bool checked);
  void show_modern_menu_state_changed(bool checked);
  void request_undo_redo_buttons_update();

  void create_new_ws();
  void open_ws();
  void save_ws();
  void save_ws_as();
  void close_cur_ws();
  void close_all_ws();
  void rename_cur_ws();
  void change_cur_ws_bg();
  void cam_copy_to_all();

  void cur_ws_changed();
  void cur_ws_sel_item_changed();
  void cur_ws_props_changed();
  void cur_ws_edit_type_changed();
  void cur_ws_sel_atoms_list_sel_changed();

  void tp_dist_button_clicked(bool checked);
  void tp_angle_button_clicked(bool checked);
  void ws_edit_mode_selector_button_clicked(int id);
  void tp_force_sel_lbl_vis_button_clicked(bool checked);
  void tp_toggle_atom_override_button_clicked(bool checked);
  void tp_camera_tool_button_triggered(QAction *action);
  void tp_fast_forward_anim_clicked();
  void tp_add_arrow_clicked();
  void tp_add_cube_clicked();
  void tp_add_point_sym_group_clicked();
  void tp_scenic_rotation_toggle();

  void apply_camera_view_change(cam_tv_e target_view);

  void toggle_ws_edit_mode();
  void toggle_fullscreen(bool checked);
  void toggle_immersive_mode();
  void start_update_cycle();
  void stop_update_cycle();

  void act_sel_all_cnt();
  void act_unsel_all_cnt();
  void act_invert_sel_cnt();
  void act_toggle_console();

  void act_undo();
  void act_redo();

  void rebuild_recent_files_menu();
  void recent_files_clicked();

  void inline_tool_left_ctrl_visibility(bool visible);
  void inline_tool_bottom_ctrl_visibility(bool visible);

  void modern_menu_clicked();

  /**
   * ws_item_t io behaviour related stuff
   */
  void build_bhv_menus_and_actions();
  void build_bhv_tools_menus();
  void build_bhv_tool_panel();
  void act_bhv_tools_menus_clicked();
  void process_bhv_tool(size_t tool_id, ws_item_t *sel_item);
  void control_bhv_tools_menus_activity();
  void act_bhv_import_to_cur_ws();
  void act_bhv_import_as_new_ws();
  void act_bhv_export_sel();
  void control_bhv_menus_activity();

  void make_screenshot();

};

} // namespace qpp::cad

} // namespace qpp

#endif
