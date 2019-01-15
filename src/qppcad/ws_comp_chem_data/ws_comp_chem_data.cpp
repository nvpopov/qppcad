#include <qppcad/ws_comp_chem_data/ws_comp_chem_data.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_comp_chem_data_t::ws_comp_chem_data_t() {

  set_default_flags(ws_item_flags_default);
   m_tag = ws_item_tag::tag_ws_comp_chem_data;
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
          al->m_anim->m_cur_anim = m_cur_vib + 1;
          al->m_anim->m_cur_anim_time = 0.0f;
          al->m_anim->m_play_anim = true;
          al->m_anim->m_play_cyclic = true;
          al->m_anim->m_anim_frame_time = 0.1f;
        }
    }
}

void ws_comp_chem_data_t::fill_custom_colors_of_geom_anim(const std::string color_map_name) {

  app_state_t* astate = app_state_t::get_inst();

//  auto color_map_it = astate->m_color_maps.find(color_map_name);

//  if (color_map_it == astate->m_color_maps.end()) return;
//  if (m_connected_items.size() != m_connected_items_stride.size()) return;

//  auto &color_map = color_map_it->second;

//  for (size_t i = 0; i < m_connected_items.size(); i++) {

//      ws_atoms_list_t *al = dynamic_cast<ws_atoms_list_t*>(m_connected_items[i].get());

//      if (al && al->m_anim->get_total_anims() == 2)  {

//          int stride = m_connected_items_stride[i];

//          //iterate over all frames
//          for (size_t fc = 0; fc < al->m_anim->m_anim_data[1].frame_data.size(); fc++) {

//              al->m_anim->m_anim_data[1].frame_data[fc].atom_color.resize(al->m_geom->nat());
//              for (size_t ac = 0; ac < al->m_geom->nat(); ac++) {
//                  fmt::print(std::cout, "!!!!!!\n");
//                  float at_grd = std::clamp(m_ccd->m_steps[fc].m_atoms_grads[ac+stride].norm(),
//                      m_ccd->m_global_gradient_norm_min, m_ccd->m_global_gradient_norm_max);
//                  float norm_l =
//                      m_ccd->m_global_gradient_norm_max - m_ccd->m_global_gradient_norm_min;
//                  float at_grd_dist = norm_l - at_grd;

//                  al->m_anim->m_anim_data[1].frame_data[fc].atom_color[ac] =
//                      color_map.get_color(at_grd_dist / norm_l);
//                }

//            }

//        }
//    }

}

void ws_comp_chem_data_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
                                                vector3<float> &vOutLookAt) {
  //do nothing
}

void ws_comp_chem_data_t::render() {

}

//void ws_comp_chem_data_t::render_overlay() {

//}

//void ws_comp_chem_data_t::render_ui() {
//  ws_item_t::render_ui();

//  if (!m_ccd) {
//      ImGui::BulletText("No data loaded");
//      return;
//    }

//  if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen)) {
//      ImGui::Spacing();
//      ImGui::TextUnformatted("Summary:");
//      ImGui::Spacing();

//      ImGui::Columns(2);
//      ImGui::Separator();
//      ImGui::TextUnformatted("Calculation type");
//      ImGui::NextColumn();
//      ImGui::TextUnformatted(ccdrt2str[m_ccd->m_run_t].c_str());
//      ImGui::Separator();

//      ImGui::NextColumn();
//      ImGui::TextUnformatted("Calculated by");
//      ImGui::NextColumn();
//      ImGui::TextUnformatted(ccdprog2str[m_ccd->m_comp_chem_program].c_str());
//      ImGui::Columns(1);

//      ImGui::Separator();
//      ImGui::Spacing();
//    }

//  if (m_ccd->m_run_t == comp_chem_program_run_t::rt_geo_opt)
//    if (ImGui::CollapsingHeader("Geometry optimization")) {

//        ImGui::Spacing();
//        if (ImGui::TreeNode("Actions:")) {
//            if (ImGui::Button("Load colors")) {
//                fill_custom_colors_of_geom_anim("semapwhore");
//              }
//            ImGui::Separator();
//            ImGui::TreePop();
//          }
//        ImGui::Spacing();

//        ImGui::TextUnformatted("Step info:");
//        ImGui::Separator();
//        ImGui::Spacing();

//        if (ImGui::Button("+", ImVec2(30,0))) manual_step_update(1);
//        ImGui::SameLine();
//        if (ImGui::Button("-", ImVec2(30,0))) manual_step_update(-1);
//        ImGui::SameLine();

//        ImGui::PushItemWidth(160);
//        if (ImGui::SliderInt("Current step", &m_cur_step, 0, m_ccd->m_steps.size() - 1))
//          update_joined_atoms_list_animation(m_cur_step);
//        ImGui::PopItemWidth();

//        ImGui::Spacing();
//        ImGui::TextUnformatted("Global info:");
//        ImGui::Columns(2);
//        ImGui::SetColumnWidth(0, 120);
//        ImGui::Separator();
//        ImGui::TextUnformatted("Nrm. grd. max");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted(fmt::format("{} eV", m_ccd->m_global_gradient_norm_max).c_str());
//        ImGui::Separator();
//        ImGui::NextColumn();
//        ImGui::TextUnformatted("Nrm. grd. min");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted(fmt::format("{} eV", m_ccd->m_global_gradient_norm_min).c_str());
//        // ImGui::Separator();
//        ImGui::Columns(1);
//        ImGui::Spacing();

//        ImGui::TextUnformatted("Step info:");

//        ImGui::Columns(2);
//        ImGui::SetColumnWidth(0, 120);
//        ImGui::Separator();
//        ImGui::TextUnformatted("Final energy");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted(fmt::format("{} eV",
//                                           m_ccd->m_steps[m_cur_step].m_toten).c_str());
//        ImGui::NextColumn();
//        ImGui::Separator();
//        ImGui::TextUnformatted("Scf steps");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted(fmt::format("{}",
//                                           m_ccd->m_steps[m_cur_step].m_scf_steps.size()).c_str());
//        ImGui::NextColumn();
//        ImGui::Separator();
//        ImGui::TextUnformatted("Max grd.");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted(fmt::format("{}",
//                                           m_ccd->m_steps[m_cur_step].m_gradient_max).c_str());
//        ImGui::NextColumn();
//        ImGui::Separator();
//        ImGui::TextUnformatted("Min grd.");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted(fmt::format("{}",
//                                           m_ccd->m_steps[m_cur_step].m_gradient_min).c_str());
//        //        ImGui::NextColumn();
//        //        ImGui::Separator();
//        //        ImGui::TextUnformatted("Avg grd.");
//        //        ImGui::NextColumn();
//        //        ImGui::TextUnformatted(fmt::format("{}",
//        //                                           m_ccd->m_steps[m_cur_step].m_gradient_average).c_str());
//        ImGui::NextColumn();
//        ImGui::Separator();
//        ImGui::TextUnformatted("Nrm. grd. max");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted(fmt::format("{}",
//                                           m_ccd->m_steps[m_cur_step].m_gradient_norm_max).c_str());
//        ImGui::NextColumn();
//        ImGui::Separator();
//        ImGui::TextUnformatted("Nrm. grd. min");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted(fmt::format("{}",
//                                           m_ccd->m_steps[m_cur_step].m_gradient_norm_min).c_str());
//        ImGui::NextColumn();
//        ImGui::Separator();
//        ImGui::TextUnformatted("Nrm. grd. avg");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted(
//              fmt::format("{}",m_ccd->m_steps[m_cur_step].m_gradient_norm_average).c_str());

//        ImGui::Columns(1);
//      }

//  if (m_ccd->m_run_t == comp_chem_program_run_t::rt_vib)
//    if (ImGui::CollapsingHeader("Vibrational analysis")) {
//        ImGui::Columns(3);
//        ImGui::Separator();
//        ImGui::TextUnformatted("Number");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted("Freq. cm-1");
//        ImGui::NextColumn();
//        ImGui::TextUnformatted("Intensity");
//        ImGui::NextColumn();
//        ImGui::Separator();

//        for (size_t i = 0; i < m_ccd->m_vibs.size(); i++) {

//            if (ImGui::Selectable(fmt::format("{}", i).c_str(), m_cur_vib == i,
//                                  ImGuiSelectableFlags_SpanAllColumns)) {
//                m_cur_vib = i;
//                manual_update_vib();
//              }
//            ImGui::NextColumn();

//            ImGui::TextUnformatted(fmt::format("{} cm-1", m_ccd->m_vibs[i].m_frequency).c_str());
//            ImGui::NextColumn();

//            ImGui::TextUnformatted(fmt::format("{}", m_ccd->m_vibs[i].m_intensity).c_str());
//            // ImGui::Separator();
//            ImGui::NextColumn();

//          }
//        //ImGui::Separator();
//        ImGui::Columns(1);
//      }
//}

void ws_comp_chem_data_t::update_joined_atoms_list_animation(size_t step_idx) {

  for (auto &items : m_connected_items) {
      ws_atoms_list_t *al = dynamic_cast<ws_atoms_list_t*>(items.get());
      if (al && al->m_anim->get_total_anims() > 1
          && al->m_anim->m_anim_data[1].frames.size() == m_ccd->m_steps.size())  {
          //fmt::print(std::cout, "Hallelujiah\n");
          al->m_anim->update_and_set_anim(1, step_idx);
        }
    }

}


bool ws_comp_chem_data_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}


std::string ws_comp_chem_data_t::compose_item_name() {
  return "comp. chem. data";
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

void ws_comp_chem_data_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void ws_comp_chem_data_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}
