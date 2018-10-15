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

  ImVec4 bgc{1.0f, 1.0f, 1.0f, m_console_alpha};
  ImVec4 fc{0.0f, 0.0f, 0.0f, 1.0f};

  //std::string test ="dsdsd";
  app_state_t* astate =  &(c_app::get_state());

  if (m_active){
      //astate->config_vote_pool.vote_for(DISABLE_MOUSE_CONTROL_IN_WORKSPACE, m_id);
      float console_height = (m_total_output_lines * m_line_height) * m_show_output;
      float height = m_total_com_lines * m_line_height + console_height + 42;

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

          if (ImGui::Button("Clear output"))
            m_output[m_console_type].clear();

          const char* items[] = {"Simple query", "Python"};
          ImGui::SameLine();
          ImGui::PushItemWidth(150);
          ImGui::Combo("Interpreter", reinterpret_cast<int*>(&m_console_type), items, 2);

          ImGui::SameLine();
          ImGui::VerticalSeparator();

          ImGui::SameLine();
          ImGui::Checkbox("Show Output", &m_show_output);

          ImGui::SameLine();
          ImGui::VerticalSeparator();

          ImGui::SameLine();
          ImGui::SliderInt("Lines", &m_total_output_lines, 1, 30);

          ImGui::SameLine();
          ImGui::VerticalSeparator();

          ImGui::SameLine();
          ImGui::SliderFloat("Transparency", &m_console_alpha, 0.1f, 1.0f);

          ImGui::SameLine();
          ImGui::VerticalSeparator();

          ImGui::PushItemWidth(0);
          ImGui::SameLine();
          if (ImGui::Button("Close")) m_active = false;

          if (m_show_output) {
              ImGui::BeginChild("console_output_region", ImVec2(0, console_height));
              ImGui::PushItemWidth(ImGui::GetWindowWidth());
              ImGui::PushID("command_output");
              ImGui::InputTextMultiline("", &m_output[m_console_type],
                                        ImVec2(ImGui::GetWindowWidth(), console_height),
                                        ImGuiInputTextFlags_ReadOnly);
              ImGui::PopID();
              ImGui::EndChild();

            }
          //ImGui::PushItemWidth(ImGui::GetWindowWidth());
          if (m_console_type == console_type_t::simple_query) {

              ImGui::PushID("command_edit_sq");
              ImGui::PushItemWidth(ImGui::GetWindowWidth()-15);
              if (ImGui::InputText("", &m_command, ImGuiInputTextFlags_CallbackCompletion |
                                   ImGuiInputTextFlags_EnterReturnsTrue |
                                   ImGuiInputTextFlags_CallbackHistory |
                                   ImGuiInputTextFlags_CallbackAlways,
                                   &console_widget_t::simple_query_input_callback, this)) {
                  std::string c_output;
                  astate->sq_manager->execute(m_command, c_output);
                  m_output[console_type_t::simple_query] += c_output;
                  if (m_command != "") m_sq_history.push_back(m_command);
                  m_command = "";
                  ImGui::SetKeyboardFocusHere(-1);
                }
              ImGui::PopID();
              //              ImGui::SetKeyboardFocusHere(-1);
              //              ImGui::SetItemDefaultFocus();

            } else {
              ImGui::PushID("command_edit_py");
              if (ImGui::InputTextMultiline("", &m_command,
                                            ImVec2(ImGui::GetWindowWidth() - 15,
                                                   m_total_com_lines * m_line_height),
                                            ImGuiInputTextFlags_CtrlEnterForNewLine |
                                            ImGuiInputTextFlags_CallbackAlways |
                                            ImGuiInputTextFlags_CallBackUnfocus,
                                            &console_widget_t::InputTextCallback, this)) {
                }
              ImGui::PopID();
            }
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

  return -1;
}

int console_widget_t::simple_query_input_callback(ImGuiInputTextCallbackData *data){
  //if (data->)
  if (!data->UserData) return -1;

  auto cw = static_cast<console_widget_t*>(data->UserData);

  if (!cw) return -1;
//  if (data->EventChar == '`') {
//      data->DeleteChars(data->BufTextLen-1, 1);
//      data->BufDirty = true;
//    }

  switch (data->EventFlag) {

    case ImGuiInputTextFlags_CallbackHistory: {

        const int prev_history_pos = cw->m_history_pos;
        if (data->EventKey == ImGuiKey_UpArrow){
            if (cw->m_history_pos == -1)
              cw->m_history_pos = cw->m_sq_history.size() - 1;
            else if (cw->m_history_pos > 0)
              cw->m_history_pos--;
          }

        else if (data->EventKey == ImGuiKey_DownArrow){
            if (cw->m_history_pos != -1)
              if (++(cw->m_history_pos) >= cw->m_sq_history.size())
                cw->m_history_pos = -1;
          }

        if (prev_history_pos != cw->m_history_pos)
          if (!cw->m_sq_history.empty()){
            const char* history_str =
                (cw->m_history_pos >= 0) ? cw->m_sq_history[cw->m_history_pos].c_str() : "";
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, history_str);
          }
      }

    }
  return -1;
}
