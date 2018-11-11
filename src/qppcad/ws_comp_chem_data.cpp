#include <qppcad/ws_comp_chem_data.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_comp_chem_data_t::ws_comp_chem_data_t() {
  set_default_flags(ws_item_flags_default);
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
      ImGui::TextUnformatted(ccdrt2str[m_ccd->run_t].c_str());
      ImGui::Separator();

      ImGui::NextColumn();
      ImGui::TextUnformatted("Calculated by");
      ImGui::NextColumn();
      ImGui::TextUnformatted(ccdprog2str[m_ccd->comp_chem_program].c_str());
      ImGui::Columns(1);

      ImGui::Separator();
      ImGui::Spacing();
    }

  if (m_ccd->run_t == comp_chem_program_run_t::rt_geo_opt)
    if (ImGui::CollapsingHeader("Geometry optimization")) {
        static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
                   ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
      }

  if (m_ccd->run_t == comp_chem_program_run_t::rt_vib)
    if (ImGui::CollapsingHeader("Vibrational analysis")) {

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
