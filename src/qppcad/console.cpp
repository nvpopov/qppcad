#include <qppcad/console.hpp>
#include <qppcad/app.hpp>
#include <qppcad/uniq_id.hpp>

using namespace qpp;
using namespace qpp::cad;

console_widget_t::console_widget_t (app_state_t *astate) {

  m_active = false;
  m_id = uniq_id_provider::get_uniq_id();
  m_line_height = 28;
  m_total_com_lines = 1;
  m_output.resize(2);

  astate->kb_manager->connect("python_console_toggle", this, &console_widget_t::toggle_visible);

  //  astate->kb_manager->connect("console_nl", this, &console_widget_t::newline_event);
  //  astate->kb_manager->connect("console_apply", this, &console_widget_t::apply_event);
#ifdef PYTHON_CONSOLE
  scope = py::module::import("__main__").attr("__dict__");
#endif

}

void console_widget_t::toggle_visible () {
  m_active = !m_active;
}

void console_widget_t::process_command (std::string &command) {

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

void console_widget_t::render () {

  ImVec4 bgc{1.0f, 1.0f, 1.0f, 1.0f};
  ImVec4 fc{0.0f, 0.0f, 0.0f, 1.0f};

  std::string test ="dsdsd";
  app_state_t* astate =  &(c_app::get_state());

  if (m_active){
      //astate->config_vote_pool.vote_for(DISABLE_MOUSE_CONTROL_IN_WORKSPACE, m_id);
      float console_height = 170;
      float height = m_total_com_lines * m_line_height + console_height + 51;

      ImGui::SetNextWindowSize(ImVec2(astate->viewport_size_c[0], height));
      ImGui::SetNextWindowPos(ImVec2(0, astate->viewport_size[1] - height));

      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleColor(ImGuiCol_WindowBg, bgc);
      ImGui::PushStyleColor(ImGuiCol_ChildBg, bgc);
      ImGui::PushStyleColor(ImGuiCol_FrameBg, bgc);
      ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, bgc);
      ImGui::PushStyleColor(ImGuiCol_PopupBg, bgc);
      ImGui::PushStyleColor(ImGuiCol_Text, fc);

      if (ImGui::Begin("Interactive console", &m_active,
                       ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
                       ImGuiWindowFlags_NoResize)) {

          ImGui::Button("Clear output");

          const char* items[] = {"Simple query", "Python"};
          ImGui::SameLine();
          ImGui::PushItemWidth(150);
          ImGui::Combo("Interpreter", reinterpret_cast<int*>(&m_console_type), items, 2);
          ImGui::PushItemWidth(0);

          ImGui::BeginChild("console_output_region", ImVec2(0, console_height));
          ImGui::PushItemWidth(ImGui::GetWindowWidth());
          ImGui::PushID("command_output");
          ImGui::InputTextMultiline("", &m_output[m_console_type],
                                    ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()),
                                    ImGuiInputTextFlags_ReadOnly);
          ImGui::PopID();
          ImGui::EndChild();
          ImGui::PushID("command_edit");
          //ImGui::PushItemWidth(ImGui::GetWindowWidth());
          if (m_console_type == console_type_t::simple_query) {

              ImGui::PushItemWidth(ImGui::GetWindowWidth()-15);
              if (ImGui::InputText("", &m_command, ImGuiInputTextFlags_CallbackCompletion |
                                   ImGuiInputTextFlags_EnterReturnsTrue,
                  &console_widget_t::simple_query_input_callback, this)) {
                  std::string c_output;
                  astate->sq_manager->execute(m_command, c_output);
                  m_output[console_type_t::simple_query] += c_output;
                  m_command = "";
                  ImGui::SetFocusID(ImGui::GetID("command_edit"), ImGui::GetCurrentWindow());
                  ImGuiContext& g = *ImGui::GetCurrentContext();
                  g.NavDisableHighlight = false;
                }

            } else {
              if (ImGui::InputTextMultiline("", &m_command,
                                            ImVec2(ImGui::GetWindowWidth() - 15,
                                                   m_total_com_lines * m_line_height),
                                            ImGuiInputTextFlags_CtrlEnterForNewLine |
                                            ImGuiInputTextFlags_CallbackAlways |
                                            ImGuiInputTextFlags_CallBackUnfocus,
                                            &console_widget_t::InputTextCallback, this)){
                  //std::cout << "test11111\n";
                }
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

      ImGui::PopStyleColor(6);
      ImGui::PopStyleVar();

    } else {
      //astate->config_vote_pool.unvote_for(DISABLE_MOUSE_CONTROL_IN_WORKSPACE, m_id);
    }

}

int console_widget_t::InputTextCallback (ImGuiInputTextCallbackData *data) {

  ImGuiContext& g = *GImGui;
  const ImGuiIO& io = g.IO;
  //  if ((data->EventFlag & ImGuiInputTextFlags_CallBackUnfocus) !=0) {
  //      std::cout<< "(data->EventFlag & ImGuiInputTextFlags_CallBackUnfocus)\n ";
  //    }

  if (data->EventKey == ImGuiKey_Enter){

      std::string buf_cont(data->Buf);
      static_cast<console_widget_t*>(data->UserData)->process_command(buf_cont);
      data->DeleteChars(0, data->BufTextLen);

    }



}

int console_widget_t::simple_query_input_callback(ImGuiInputTextCallbackData *data){
  //if (data->)
}
