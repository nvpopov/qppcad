#include <qppcad/console.hpp>
#include <qppcad/app.hpp>
#include <qppcad/uniq_id.hpp>

using namespace qpp;

console_widget_t::console_widget_t(){
  m_active = false;
  m_id = get_uniq_id();
}

void console_widget_t::render(){
  string test ="dsdsd";
  app_state_t* astate =  &(c_app::get_state());

  if (m_active){
      astate->config_vote_pool.vote_for(DISABLE_MOUSE_CONTROL_IN_WORKSPACE, m_id);
      if (ImGui::Begin("Interactive console", &m_active, ImGuiWindowFlags_None)){
          ImGui::BeginChild("commands",
                            ImVec2(ImGui::GetWindowWidth()-20, ImGui::GetWindowHeight()-90),
                            true);

          for (auto i = 0; i < 45; i++){
              ImGui::Text(fmt::format("[{}]",i).c_str());
              ImGui::SameLine();
              ImGui::PushItemWidth(ImGui::GetWindowWidth()-40);
              ImGui::PushID(fmt::format("command {}",i).c_str());
              ImGui::InputTextMultiline("", test.data(), 5);
              ImGui::PopID();
              ImGui::Spacing();
            }



          ImGui::EndChild();
          ImGui::End();
        }
    } else {
      astate->config_vote_pool.unvote_for(DISABLE_MOUSE_CONTROL_IN_WORKSPACE, m_id);
    }
}
