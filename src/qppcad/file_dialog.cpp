#include <qppcad/file_dialog.hpp>
#include <qppcad/app.hpp>

using namespace qpp;

void file_dialog_t::require_dialog(file_dialog_kind kind_of_dialog){
  kind = kind_of_dialog;
  activated = true;
}

void file_dialog_t::render(){
  app_state_t *astate = &(c_app::get_state());

  if (activated) {
      ImGui::OpenPopup(file_dialog_headers[uint8_t(kind)].c_str());
      astate->disable_mouse_camera_control = true;
    } else {
      astate->disable_mouse_camera_control = false;
    }

  ImVec2 dialog_pos = ImVec2(astate->wWidth / 2 - 350, astate->wHeight / 2 - 250);
  ImGui::SetNextWindowPos(dialog_pos);
  ImGui::SetNextWindowSize(ImVec2(600,500));
  if (ImGui::BeginPopupModal(file_dialog_headers[uint8_t(kind)].c_str(),
                             &activated,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav)){

      ImGui::SetItemDefaultFocus();
      ImGui::EndPopup();
    }
}
