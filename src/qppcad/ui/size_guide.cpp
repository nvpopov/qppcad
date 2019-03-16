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

int size_guide_t::tool_panel_h() {
  return 42;
}

int size_guide_t::tool_panel_ws_selector_w() {
  return 170;
}

int size_guide_t::tool_panel_ws_selector_h() {
  return 30;
}

int size_guide_t::tool_panel_ctrl_h() {
  return 30;
}

int size_guide_t::tool_panel_ctrl_w() {
  return 30;
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

int size_guide_t::obj_insp_w() {
  return 430;
}

int size_guide_t::obj_insp_table_w() {
  return int(obj_insp_w() * 0.86);
}

int size_guide_t::obj_insp_item_list_max_h() {
  return 150;
}

int size_guide_t::obj_insp_ctrl_max_w() {
  return 80;
}

int size_guide_t::obj_insp_combo_max_w() {
  return 160;
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

int size_guide_t::obj_insp_lbl_w() {
  return 140;
}
