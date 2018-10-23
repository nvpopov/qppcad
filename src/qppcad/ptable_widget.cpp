#include <qppcad/ptable_widget.hpp>
#include <qppcad/imgui_addons.hpp>
#include <io/strfun.hpp>
#include <data/ptable.hpp>

using namespace qpp::cad;

ptable_widget_t::ptable_widget_t(app_state_t *astate) {

}

void ptable_widget_t::render () {

  if (!m_active) return;

  ImGui::Begin("Periodic table editor", &m_active);

  //      ImGui::Separator();
  //      ImGui::Spacing();
  if (ImGui::Button("Apply to current workspace", ImVec2(0, 24))) {

    }

  ImGui::SameLine();
  if (ImGui::Button("Apply globally", ImVec2(0, 24))) {

    }

  ImGui::Spacing();
  ImGui::Separator();

  ImGui::InputText("Filter", &m_filter);
  ImGui::SameLine();
  ImGui::ToggleButton("NAME", &m_filter_name);
  ImGui::SameLine();
  ImGui::ToggleButton("SYMBOL", &m_filter_symbol);
  ImGui::Separator();

  ImGui::BeginChild("Periodic table data");
  ImGui::Columns(6);

  //      ImGui::SetColumnWidth(0, 70);
  //      ImGui::SetColumnWidth(1, 100);
  //      ImGui::SetColumnWidth(2, 110);
  //      ImGui::SetColumnWidth(3, 190);

  ImGui::TextUnformatted("N");
  ImGui::NextColumn();
  ImGui::TextUnformatted("Symbol");
  ImGui::NextColumn();
  ImGui::TextUnformatted("Name");
  ImGui::NextColumn();
  ImGui::TextUnformatted("Radius");
  ImGui::NextColumn();
  ImGui::TextUnformatted("Color");
  ImGui::NextColumn();
  ImGui::TextUnformatted("Cov. radii");
  ImGui::Columns(1);

  ImGui::Separator();


  for (auto &ptable_rec : ptable::get_inst()->arecs)
    if ((find_string_ci(ptable_rec.aName, m_filter) && m_filter_name) ||
        (find_string_ci(ptable_rec.aSymbol, m_filter)  && m_filter_symbol) || m_filter == "") {

        ImGui::Columns(6);
        //          ImGui::SetColumnWidth(0, 70);
        //          ImGui::SetColumnWidth(1, 100);
        //          ImGui::SetColumnWidth(2, 110);
        //          ImGui::SetColumnWidth(3, 190);
        ImGui::TextUnformatted(fmt::format("{}", ptable_rec.aNumber).c_str());
        ImGui::NextColumn();
        ImGui::TextUnformatted(ptable_rec.aSymbol.c_str());
        ImGui::NextColumn();
        ImGui::TextUnformatted(ptable_rec.aName.c_str());
        ImGui::NextColumn();
        ImGui::PushID(ptable_rec.aName.c_str());
        ImGui::InputFloat("", &ptable_rec.aRadius, 0.0f, 0.1f);
        ImGui::PopID();
        ImGui::NextColumn();
        ImGui::PushID(ptable_rec.aName.c_str());
        ImGui::ColorEdit3("", ptable_rec.aColorJmol.data(), ImGuiColorEditFlags_NoInputs);
        ImGui::PopID();
        ImGui::NextColumn();
        ImGui::PushID(ptable_rec.aNumber);
        ImGui::InputFloat("", &ptable_rec.aCovRad_Slater);
        ImGui::PopID();
        ImGui::Columns(1);
        ImGui::Separator();

      }

  ImGui::EndChild();

  ImGui::End();

}

