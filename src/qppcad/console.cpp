#include <qppcad/console.hpp>
#include <qppcad/app.hpp>
#include <qppcad/uniq_id.hpp>

using namespace qpp;
using namespace qpp::cad;

console_widget_t::console_widget_t(app_state_t *astate){
  m_active = false;
  m_id = uniq_id_provider::get_uniq_id();
  m_line_height = 28;
  m_total_com_lines = 1;
  astate->kb_manager->connect("python_console_toggle", this, &console_widget_t::toggle_visible);
  //  astate->kb_manager->connect("console_nl", this, &console_widget_t::newline_event);
  //  astate->kb_manager->connect("console_apply", this, &console_widget_t::apply_event);
#ifdef PYTHON_CONSOLE
  scope = py::module::import("__main__").attr("__dict__");
#endif
}

void console_widget_t::toggle_visible(){
  m_active = !m_active;
}

void console_widget_t::process_command(string &command){
  m_command = "";
#ifdef PYTHON_CONSOLE
  try {
    py::exec(command, scope);
    //m_output += py::cast<string>(s.str()) + "\n";
  }
  catch (const py::error_already_set&){

  }
#endif
}

void console_widget_t::render(){
  string test ="dsdsd";
  app_state_t* astate =  &(c_app::get_state());

  if (m_active){
      //astate->config_vote_pool.vote_for(DISABLE_MOUSE_CONTROL_IN_WORKSPACE, m_id);
      if (ImGui::Begin("Interactive console", &m_active,
                       ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoCollapse)){
          ImGui::BeginChild("console_output_region", ImVec2(0, ImGui::GetWindowHeight() - 62
                                                            - m_total_com_lines * m_line_height));
          ImGui::PushItemWidth(ImGui::GetWindowWidth());
          ImGui::PushID("command_output");
          ImGui::InputTextMultiline("", &m_output,
                                    ImVec2(0, ImGui::GetWindowHeight()));
          ImGui::PopID();
          ImGui::EndChild();
          ImGui::PushID("command_edit");
          ImGui::PushItemWidth(ImGui::GetWindowWidth());
          if (ImGui::InputTextMultiline("", &m_command, ImVec2(0, m_total_com_lines * m_line_height),
                                        ImGuiInputTextFlags_CtrlEnterForNewLine |
                                        ImGuiInputTextFlags_CallbackAlways |
                                        ImGuiInputTextFlags_CallBackUnfocus,
                                        &console_widget_t::InputTextCallback, this)){
              //std::cout << "test11111\n";
            }
          ImGui::PopID();

          //          ImGui::BeginChild("commands",
          //                            ImVec2(ImGui::GetWindowWidth()-20, ImGui::GetWindowHeight()-90),
          //                            true);

          //          for (auto i = 0; i < 45; i++){
          //              ImGui::Text(fmt::format("[{}]",i).c_str());
          //              ImGui::SameLine();
          //              ImGui::PushItemWidth(ImGui::GetWindowWidth()-40);
          //              ImGui::PushID(fmt::format("command {}",i).c_str());
          //              ImGui::InputTextMultiline("", test.data(), 5);
          //              ImGui::PopID();
          //              ImGui::Spacing();
          //            }
          //          ImGui::EndChild();

          ImGui::End();
        }
    } else {
      //astate->config_vote_pool.unvote_for(DISABLE_MOUSE_CONTROL_IN_WORKSPACE, m_id);
    }
}

int console_widget_t::InputTextCallback(ImGuiInputTextCallbackData *data){
  ImGuiContext& g = *GImGui;
  const ImGuiIO& io = g.IO;
//  if ((data->EventFlag & ImGuiInputTextFlags_CallBackUnfocus) !=0) {
//      std::cout<< "(data->EventFlag & ImGuiInputTextFlags_CallBackUnfocus)\n ";
//    }

  if (data->EventKey == ImGuiKey_Enter){
      string buf_cont(data->Buf);
      static_cast<console_widget_t*>(data->UserData)->process_command(buf_cont);
      data->DeleteChars(0, data->BufTextLen);

    }



}
