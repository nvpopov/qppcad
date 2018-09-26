#ifndef QPPCAD_WS_ATOMS_LIST_OBJ_INSP
#define QPPCAD_WS_ATOMS_LIST_OBJ_INSP
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/imgui_addons.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"

namespace qpp::cad {
  struct ws_atoms_list_obj_insp_helper {
      static void render_ui(ws_atoms_list_t *al){
        if (ImGui::CollapsingHeader("Summary")){
            ImGui::Spacing();
            ImGui::Columns(2);
            ImGui::Text("Total atoms:");
            ImGui::Text("Atom types:");
            ImGui::NextColumn();
            ImGui::Text(fmt::format(" {} ", al->m_geom->nat()).c_str());
            ImGui::Text(fmt::format(" {} ", al->m_geom->n_atom_types()).c_str());
            ImGui::Columns(1);
            ImGui::Spacing();
          }


        if (ImGui::CollapsingHeader("Atom types")){
            ImGui::Spacing();
            ImGui::Columns(4);
            ImGui::SetColumnWidth(0, 60);
            ImGui::SetColumnWidth(1, 70);
            ImGui::SetColumnWidth(2, 70);
            ImGui::SetColumnWidth(3, 70);
            ImGui::Text("Type");
            ImGui::NextColumn();
            ImGui::Text("Count");
            ImGui::NextColumn();
            ImGui::Text("Color");
            ImGui::NextColumn();
            ImGui::Text("Hide");
            ImGui::NextColumn();
            ImGui::Separator();
            for (uint8_t i = 0; i < al->m_geom->n_types(); i++){
                ImGui::Text(al->m_geom->atom_of_type(i).c_str());
              }
            ImGui::NextColumn();

            for (uint8_t i = 0; i < al->m_geom->n_types(); i++){
                ImGui::Text(fmt::format("{}", al->m_geom->get_atom_count_by_type(i)).c_str());
              }
            ImGui::NextColumn();

            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            for (uint8_t i = 0; i < al->m_geom->n_types(); i++){
                const ImVec2 p = ImGui::GetCursorScreenPos();
                float p_x = p.x + 8 + 6;
                float p_y = p.y + 10;
                ImVec2 p_n(p_x, p_y);
                auto ap_idx = ptable::number_by_symbol(al->m_geom->atom_of_type(i));
                vector3<float> bc(0.0, 0.0, 1.0);
                if(ap_idx) {bc = ptable::get_inst()->arecs[*ap_idx-1].aColorJmol;}
                draw_list->AddCircleFilled(p_n, 8, ImColor(ImVec4(bc[0], bc[1], bc[2], 1.0f)));
                ImGui::Dummy(ImVec2(16, 16));
              }

            ImGui::NextColumn();
            for (uint8_t i = 0; i < al->m_geom->n_types(); i++){
                bool _l_type_to_hide = false;
                auto it = al->m_atom_type_to_hide.find(i);
                if (it != al->m_atom_type_to_hide.end()) _l_type_to_hide = true;

                ImGui::PushID(i);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.8f, 0.8f));
                ImGui::Checkbox("", &_l_type_to_hide);
                ImGui::PopStyleVar(1);
                ImGui::PopID();

                if (_l_type_to_hide) al->m_atom_type_to_hide.insert(i);
                else {
                    auto it2 = al->m_atom_type_to_hide.find(i);
                    if (it2 != al->m_atom_type_to_hide.end()) al->m_atom_type_to_hide.erase(it2);
                  }
              }
            ImGui::Columns(1);
            ImGui::Spacing();
          }

        if (ImGui::CollapsingHeader("Display and styling")){

            ImGui::SliderFloat("Atom size", &al->m_atom_scale_factor, 0.25f, 2.0f, "%.4f", 1);
            ImGui::SliderFloat("Bond size", &al->m_bond_scale_factor, 0.02f, 2.0f, "%.4f", 1);

            ImGui::Checkbox("Show atoms", &al->m_show_atoms);
            ImGui::Checkbox("Show bonds", &al->m_show_bonds);

            if (al->m_geom->DIM > 0) {
                ImGui::Checkbox("Draw periodic cell", &al->m_draw_cell);
                ImGui::Checkbox("Show imaginary atoms", &al->m_show_imaginary_atoms);
                ImGui::Checkbox("Show imaginary bonds", &al->m_show_imaginary_bonds);
              }

            ImGui::ColorEdit3("Cell ", al->m_cell_color.data());

          }

        if (ImGui::CollapsingHeader("Bonding table")){
            ImGui::Spacing();
            bool rebuild_ngb{false};
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Checkbox("Show disabled", &al->m_bonding_table_show_disabled_record);
            ImGui::SameLine();
            if (ImGui::Button("Rebond new")){
                al->m_tws_tr->m_bonding_table.init_default(al->m_geom.get());
                rebuild_ngb = true;
              }
            ImGui::Spacing();
            ImGui::Separator();
            for (auto &elem : al->m_tws_tr->m_bonding_table.m_dist)
              if (al->m_bonding_table_show_disabled_record || elem.second.m_enabled){
                  if (ImGui::TreeNode(fmt::format("[{}] - [{}]",
                                                  al->m_geom->atom_of_type(elem.first.m_a),
                                                  al->m_geom->atom_of_type(elem.first.m_b)).c_str())){
                      ImGui::PushItemWidth(60);

                      if (ImGui::Checkbox("Enabled", &(elem.second.m_enabled))) rebuild_ngb = true;

                      if (elem.second.m_enabled){
                          ImGui::SameLine();
                          ImGui::PushItemWidth(140);
                          if (ImGui::SliderFloat("Distance", &(elem.second.m_bonding_dist), 0.01f, 10.0f)){
                              al->m_tws_tr->m_bonding_table.update_pair_max_dist(elem.first.m_a,
                                                                                 elem.first.m_b);
                              rebuild_ngb = true;
                            }
                        }
                      ImGui::TreePop();
                    }
                  ImGui::Spacing();
                }
            if (rebuild_ngb) al->m_tws_tr->clr_ngb_and_rebuild();
          }

        //begin modify section
        if (ImGui::CollapsingHeader("Modify")){
            //check that we select 1 or 2 atoms
            if (al->m_atom_idx_sel.size()!=1 && al->m_atom_idx_sel.size()!=2){
                ImGui::Spacing();
                ImGui::BulletText("Select one atom to edit it");
                ImGui::BulletText("Select two atoms to specify \ndistance between them");


              }
            // if we selected one atom - show single modify widget
            else if (al->m_atom_idx_sel.size()==1) {
                ImGui::BulletText("Modify single atom:");
                static string custom_atom_name =
                    al->m_geom->atom(al->m_atom_idx_sel.begin()->m_atm);
                //ImGui::Separator();
                vector3<float> pos = al->m_geom->pos(al->m_atom_idx_sel.begin()->m_atm);
                if (ImGui::InputFloat3("Position", pos.data())){
                    al->update_atom(al->m_atom_idx_sel.begin()->m_atm, pos);
                  }
                ImGui::PushItemWidth(70);
                ImGui::InputText("Atom name", &custom_atom_name);
                ImGui::SameLine();
                if (ImGui::Button("Edit", ImVec2(100, 0)))
                  if (custom_atom_name != "") al->update_atom(
                        al->m_atom_idx_sel.begin()->m_atm, custom_atom_name);
              } // end one atom selection

            // if we selected two atoms - show distance modify widget
            else if (al->m_atom_idx_sel.size() == 2){
                ImGui::BulletText("Modify distance between real atoms");
                auto it1 = al->m_atom_idx_sel.begin();
                auto it2 = it1++;
                if (it1->m_idx != index::D(al->m_geom->DIM).all(0) ||
                    it2->m_idx != index::D(al->m_geom->DIM).all(0) ){
                    ImGui::BulletText("One of selected atom are imaginary");
                  } else {
                    float dist_btw = (al->m_geom->pos(it1->m_atm, it1->m_idx) -
                                      al->m_geom->pos(it2->m_atm, it2->m_idx)).norm();

                    if (ImGui::SliderFloat("Distance", &dist_btw, 0.1f, 15.0f)){
                        vector3<float> r_btw = (al->m_geom->pos(it1->m_atm, it1->m_idx) +
                                                al->m_geom->pos(it2->m_atm, it2->m_idx))*0.5f;
                        vector3<float> dir_f = (al->m_geom->pos(it1->m_atm, it1->m_idx) - r_btw).normalized();
                        vector3<float> dir_s = (al->m_geom->pos(it2->m_atm, it2->m_idx) - r_btw).normalized();
                        al->m_geom->change_pos(it1->m_atm, r_btw + dir_f * dist_btw * 0.5f);
                        al->m_geom->change_pos(it2->m_atm, r_btw + dir_s * dist_btw * 0.5f);
                      }
                  }
              } //end two atom selection

          }//end modify section

        if (ImGui::CollapsingHeader("Add atoms")){
            static string custom_atom_name;
            ImGui::Separator();
            ImGui::InputFloat3("Position", al->m_new_atom_pos.data());
            ImGui::PushItemWidth(70);
            ImGui::InputText("Atom name", &custom_atom_name);
            ImGui::SameLine();
            if (ImGui::Button("Add", ImVec2(100, 0)))
              if (custom_atom_name != "") al->insert_atom(custom_atom_name,
                                                          al->m_new_atom_pos);
          }

        if (al->animable()){
            if (ImGui::CollapsingHeader("Animations")){
                ImGui::Spacing();
                ImGui::BulletText(fmt::format("Total animations : {}", al->m_anim.size()).c_str());
                ImGui::PushItemWidth(140);

                ImGui::Separator();

                std::vector<std::string>  vStr;
                vStr.reserve(10);
                std::vector<char*>  vChar;
                for (size_t i = 0; i < al->m_anim.size(); i++)
                  vStr.push_back(al->m_anim[i].anim_name);
                std::transform(vStr.begin(), vStr.end(), std::back_inserter(vChar),vec_str_to_char);

                ImGui::PushItemWidth(150);
                ImGui::Combo("Current animation", &al->m_cur_anim, vChar.data(), al->m_anim.size());
                ImGui::SliderFloat("Time per frame(sec.)", &al->m_anim_frame_time, 0.1f, 5.0f);
                ImGui::Checkbox("Play in cycle", &al->m_play_cyclic);
                ImGui::Text(fmt::format("Frames count: {}",
                                        al->m_anim[al->m_cur_anim].frame_data.size()).c_str());
                ImGui::Separator();
                ImGui::PushItemWidth(240);
                if (ImGui::SliderFloat("Timeline", &al->m_cur_anim_time,
                                       0.0f, (al->m_anim[al->m_cur_anim].frame_data.size() - 1))){
                    if (!al->m_play_anim) al->update_geom_to_anim(al->m_cur_anim, al->m_cur_anim_time);
                  }

                ImGui::ToggleButton("Play", &al->m_play_anim);
              }
          }
        if (ImGui::CollapsingHeader("Export")){
            if (ImGui::Button("VASP POSCAR")){

              }

            ImGui::SameLine();
            if (ImGui::Button("XYZ")){

              }
          }

        //  if (ImGui::CollapsingHeader("Measurements")){
        //      ImGui::Spacing();
        //      if (m_atom_idx_selection.size() != 2) {
        //          ImGui::BulletText("Select 2 atoms to measure distance");
        //          ImGui::BulletText("Select 3 atoms to measure angle");
        //        } else if (m_atom_idx_selection.size() == 2) {
        //          auto first = m_atom_idx_selection.cbegin();
        //          auto last  = ++(m_atom_idx_selection.cbegin());
        //          float dist = (m_geom->pos(first->m_atm, first->m_idx) -
        //                        m_geom->pos(last->m_atm, last->m_idx)).norm();
        //          ImGui::Columns(2);
        //          ImGui::Separator();
        //          ImGui::Text("First atom");
        //          ImGui::Text("Second atom");
        //          ImGui::Text("Distance");
        //          ImGui::NextColumn();
        //          ImGui::Text(fmt::format("{}, Index = {}",
        //                                  m_geom->atom_of_type(m_geom->type_table(first->m_atm)),
        //                                  first->m_idx).c_str());
        //          ImGui::Text(fmt::format("{}, Index = {}",
        //                                  m_geom->atom_of_type(m_geom->type_table(last->m_atm)),
        //                                  last->m_idx).c_str());
        //          ImGui::Text(fmt::format("{} Angs.", dist).c_str());
        //          ImGui::Columns(1);
        //          ImGui::Separator();
        //          ImGui::Checkbox("Draw a line between the measured atoms", &m_draw_line_in_dist_measurement);


        //        }

        //    }
      }
  };
}
#pragma clang diagnostic pop
#endif
