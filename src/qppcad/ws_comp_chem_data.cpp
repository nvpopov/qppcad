#include <qppcad/ws_comp_chem_data.hpp>
#include <qppcad/ws_atoms_list.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_comp_chem_data_t::ws_comp_chem_data_t() {
  set_default_flags(ws_item_flags_default);
}

void ws_comp_chem_data_t::manual_step_update(const int dir) {
  auto old_step = m_cur_step;
  auto new_step = m_cur_step + dir;
  if (new_step >= 0 && new_step < m_ccd->m_steps.size()) m_cur_step = new_step;
  if (old_step != m_cur_step) update_joined_atoms_list_animation(m_cur_step);
}

void ws_comp_chem_data_t::manual_update_vib() {
  for (auto &items : m_connected_items) {
      ws_atoms_list_t *al = dynamic_cast<ws_atoms_list_t*>(items.get());
      if (al && al->m_anim->get_total_anims() == m_ccd->m_vibs.size() + 1)  {
          //fmt::print(std::cout, "Hallelujiah\n");
          al->m_anim->m_cur_anim = m_cur_vib + 1;
          al->m_anim->m_cur_anim_time = 0.0f;
          al->m_anim->m_play_anim = true;
          al->m_anim->m_play_cyclic = true;
          al->m_anim->m_anim_frame_time = 0.1f;
        }
    }
}

void ws_comp_chem_data_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
                                                vector3<float> &vOutLookAt) {
  //do nothing
}

void ws_comp_chem_data_t::render() {

}

void ws_comp_chem_data_t::render_overlay() {

}

void ws_comp_chem_data_t::render_ui() {
  ws_item_t::render_ui();

  if (!m_ccd) {
      ImGui::BulletText("No data loaded");
      return;
    }

  if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::Spacing();
      ImGui::TextUnformatted("Summary:");
      ImGui::Spacing();

      ImGui::Columns(2);
      ImGui::Separator();
      ImGui::TextUnformatted("Calculation type");
      ImGui::NextColumn();
      ImGui::TextUnformatted(ccdrt2str[m_ccd->m_run_t].c_str());
      ImGui::Separator();

      ImGui::NextColumn();
      ImGui::TextUnformatted("Calculated by");
      ImGui::NextColumn();
      ImGui::TextUnformatted(ccdprog2str[m_ccd->m_comp_chem_program].c_str());
      ImGui::Columns(1);

      ImGui::Separator();
      ImGui::Spacing();
    }

  if (m_ccd->m_run_t == comp_chem_program_run_t::rt_geo_opt)
    if (ImGui::CollapsingHeader("Geometry optimization")) {
        ImGui::Spacing();

        if (ImGui::Button("+", ImVec2(30,0))) manual_step_update(1);
        ImGui::SameLine();
        if (ImGui::Button("-", ImVec2(30,0))) manual_step_update(-1);
        ImGui::SameLine();

        ImGui::PushItemWidth(160);
        if (ImGui::SliderInt("Current step", &m_cur_step, 0, m_ccd->m_steps.size() - 1))
          update_joined_atoms_list_animation(m_cur_step);
        ImGui::PopItemWidth();

        ImGui::Spacing();

        ImGui::TextUnformatted("Step info:");

        ImGui::Columns(2);
        ImGui::Separator();
        ImGui::TextUnformatted("Final energy");
        ImGui::NextColumn();
        ImGui::TextUnformatted(fmt::format("{} eV",
                                           m_ccd->m_steps[m_cur_step].m_toten).c_str());
        ImGui::NextColumn();
        ImGui::Separator();
        ImGui::TextUnformatted("Scf steps");
        ImGui::NextColumn();
        ImGui::TextUnformatted(fmt::format("{}",
                                           m_ccd->m_steps[m_cur_step].m_scf_steps.size()).c_str());
        ImGui::Columns(1);
      }

  if (m_ccd->m_run_t == comp_chem_program_run_t::rt_vib)
    if (ImGui::CollapsingHeader("Vibrational analysis")) {
        ImGui::Columns(3);
        ImGui::Separator();
        ImGui::TextUnformatted("Number");
        ImGui::NextColumn();
        ImGui::TextUnformatted("Freq. cm-1");
        ImGui::NextColumn();
        ImGui::TextUnformatted("Intensity");
        ImGui::NextColumn();
        ImGui::Separator();

        for (size_t i = 0; i < m_ccd->m_vibs.size(); i++) {

            if (ImGui::Selectable(fmt::format("{}", i).c_str(), m_cur_vib == i,
                                  ImGuiSelectableFlags_SpanAllColumns)) {
                        m_cur_vib = i;
                        manual_update_vib();
                    }
            ImGui::NextColumn();

            ImGui::TextUnformatted(fmt::format("{} cm-1", m_ccd->m_vibs[i].m_frequency).c_str());
            ImGui::NextColumn();

            ImGui::TextUnformatted(fmt::format("{}", m_ccd->m_vibs[i].m_intensity).c_str());
            ImGui::Separator();
            ImGui::NextColumn();

          }
        ImGui::Separator();
        ImGui::Columns(1);
      }
}

void ws_comp_chem_data_t::update_joined_atoms_list_animation(size_t step_idx) {

  for (auto &items : m_connected_items) {
      ws_atoms_list_t *al = dynamic_cast<ws_atoms_list_t*>(items.get());
      if (al && al->m_anim->get_total_anims() > 1
          && al->m_anim->m_anim_data[1].frame_data.size() == m_ccd->m_steps.size())  {
          //fmt::print(std::cout, "Hallelujiah\n");
          al->m_anim->update_and_set_anim(1, step_idx);
        }
    }

}

void ws_comp_chem_data_t::render_work_panel_ui() {

}

void ws_comp_chem_data_t::td_context_menu_edit_item() {

}

void ws_comp_chem_data_t::td_context_menu_edit_content() {

}

bool ws_comp_chem_data_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}


std::string ws_comp_chem_data_t::compose_item_name() {
  return "Type = [comp. chem. data]";
}

void ws_comp_chem_data_t::update(float delta_time) {

}

float ws_comp_chem_data_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t ws_comp_chem_data_t::get_amount_of_selected_content() {
  return 0;
}

size_t ws_comp_chem_data_t::get_content_count() {
  return 0;
}

std::string ws_comp_chem_data_t::get_ws_item_class_name() {
  return "ws_comp_chem_data";
}

void ws_comp_chem_data_t::write_to_json(json &data) {

}

void ws_comp_chem_data_t::read_from_json(json &data) {

}
