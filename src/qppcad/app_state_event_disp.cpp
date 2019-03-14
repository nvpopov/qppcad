#include <qppcad/app_state_event_disp.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


void app_state_event_disp_t::wss_changed() {
  emit(wss_changed_signal());
}

void app_state_event_disp_t::cur_ws_changed() {
  emit(cur_ws_changed_signal());
}

void app_state_event_disp_t::cur_ws_edit_type_changed(){
   emit(cur_ws_edit_type_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_changed() {
  emit(cur_ws_selected_item_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_content_changed() {
  emit(cur_ws_selected_item_content_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_frame_changed() {
  emit(cur_ws_selected_item_frame_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_atoms_list_selection_changed() {
   emit(cur_ws_selected_atoms_list_selection_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_atoms_list_cell_changed() {
  emit(cur_ws_selected_atoms_list_cell_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_position_changed(){
  emit(cur_ws_selected_item_position_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_atoms_list_selected_content_changed() {
  emit(cur_ws_selected_atoms_list_selected_content_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_measurements_changed() {
  emit(cur_ws_selected_item_measurements_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_need_to_update_obj_insp() {
  emit(cur_ws_selected_item_need_to_update_obj_insp_signal());
}

void app_state_event_disp_t::new_file_loaded(const std::string &file_name,
                                             const size_t ff,
                                             const bool is_native) {
  app_state_t::get_inst()->add_recent_file(file_name, is_native, ff);
  emit(new_file_loaded_signal());
}

void app_state_event_disp_t::python_console_clear_requested() {
  emit(python_console_clear_requested_signal());
}

void app_state_event_disp_t::python_console_focus_requested() {
  emit(python_console_focus_requested_signal());
}

void app_state_event_disp_t::python_console_font_size_updated(){
  emit(python_console_font_size_updated_signal());
}

app_state_event_disp_t::app_state_event_disp_t(QObject *parent) : QObject (parent) {

}
