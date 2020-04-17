#include <qppcad/ui/size_guide.hpp>

using namespace qpp;
using namespace qpp::cad;

size_guide_t::size_guide_t() {

}

int size_guide_t::main_window_h() {
  return 300 ;
}

int size_guide_t::main_window_w() {
  return 600;
}

int size_guide_t::tool_left_part_w() {
  return 340;
}

int size_guide_t::tool_panel_h() {
  return 42;
}

int size_guide_t::tool_panel_h_exact() {
  return 48;
}

int size_guide_t::tool_panel_w() {
  return tool_panel_ctrl_sys_w();
}

int size_guide_t::tool_panel_ws_selector_w() {
  return 180;
}

int size_guide_t::tool_panel_ws_selector_h() {
  return 30;
}

int size_guide_t::tool_panel_ctrl_h() {
  return 36;
}

int size_guide_t::tool_panel_ctrl_w() {
  return 36;
}

int size_guide_t::tool_panel_edit_sel_w() {
  return 58;
}

int size_guide_t::tool_panel_ctrl_sys_w() {
  return 46;
}

int size_guide_t::tool_panel_checkable_ctrl_w() {
  return 64;
}

int size_guide_t::tool_panel_icon_size() {
  return 26;
}

int size_guide_t::inline_tool_vertical_max_width() {
  return size_guide_t::obj_insp_w();
}

int size_guide_t::inline_tool_horizontal_max_height() {
  return 300;
}

int size_guide_t::inline_tool_horizontal_min_height() {
  return 220;
}

int size_guide_t::obj_insp_w() {
  return 386;
}

int size_guide_t::obj_insp_splr_w() {
  return int(obj_insp_w());
}

int size_guide_t::obj_insp_table_w() {
  return int(obj_insp_w() * 0.99);
}

int size_guide_t::obj_insp_anim_w(){
  return int(obj_insp_w() * 0.99);
}

int size_guide_t::obj_insp_item_list_max_h() {
  return 170;
}

int size_guide_t::obj_insp_ctrl_max_w() {
  return 80;
}

int size_guide_t::obj_insp_combo_max_w() {
  return 170;
}

int size_guide_t::obj_insp_combo_max_w_v2() {
  return 180;
}

int size_guide_t::obj_insp_ctrl_t2_w() {
  return 42;
}

int size_guide_t::obj_insp_ctrl_t2_h() {
  return 21;
}

int size_guide_t::obj_insp_button_w() {
  return 80;
}

int size_guide_t::obj_insp_button_h() {
  return 25;
}

int size_guide_t::obj_insp_lbl_w() {
  return 135;
}

int size_guide_t::obj_insp_list_wdgt_h() {
  return 120;
}

int size_guide_t::ext_editor_btn_h() {
  return 19;
}

int size_guide_t::ext_editor_icon() {
  return 18;
}

int size_guide_t::spoiler_button_h() {
  return 22;
}

int size_guide_t::spoiler_button_icon_h() {
  return 20;
}

int size_guide_t::common_tools_panel_w() {
  return 320;
}

int size_guide_t::common_tools_panel_label_w() {
  return 100;
}

int size_guide_t::common_tools_panel_label_w_big() {
  return 140;
}

int size_guide_t::script_editor_button_w() {
  return 28;
}

int size_guide_t::script_edtior_button_icon_w() {
  return 26;
}

int size_guide_t::common_button_fixed_w() {
  return 100;
}

int size_guide_t::node_book_inplace_par_width() {
  return 100;
}

int size_guide_t::ws_tabbar_height() {
  return 32;
}

int size_guide_t::ws_item_modal_tool_small_width() {
  return 320;
}

int size_guide_t::ws_item_modal_tool_small_gb_width() {
  return 300;
}

int size_guide_t::ws_item_modal_tool_small_form_label_width() {
  return 170;
}
