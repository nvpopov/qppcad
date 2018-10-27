#include <qppcad/ws_atoms_list_obj_insp.hpp>

namespace qpp {

  namespace cad {

    void ws_atoms_list_obj_insp_helper::render_ui (ws_atoms_list_t &al) {

      app_state_t* astate = &(c_app::get_state());

      if (ImGui::CollapsingHeader("Summary")){
          ImGui::Spacing();
          ImGui::Columns(2);
          ImGui::Text("Total atoms:");
          ImGui::Text("Atom types:");
          ImGui::NextColumn();
          ImGui::TextUnformatted(fmt::format("{0}", al.m_geom->nat()).c_str(), nullptr);
          ImGui::TextUnformatted(fmt::format("{}", al.m_geom->n_atom_types()).c_str(),
                                 nullptr);
          ImGui::Columns(1);
          ImGui::Spacing();
        }


      if (ImGui::CollapsingHeader("Atom types")) {
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

          ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 0));
          ImGui::BeginGroup();

          for (uint8_t i = 0; i < al.m_geom->n_types(); i++) {
              ImGui::TextUnformatted(al.m_geom->atom_of_type(i).c_str(), nullptr);
              ImGui::NextColumn();

              ImGui::TextUnformatted(
                    fmt::format("{}", al.m_geom->get_atom_count_by_type(i)).c_str(), nullptr);
              ImGui::NextColumn();


              ImDrawList* draw_list = ImGui::GetWindowDrawList();
              const ImVec2 p = ImGui::GetCursorScreenPos();
              float p_x = p.x + 8 + 6;
              float p_y = p.y + 10;
              ImVec2 p_n(p_x, p_y);
              auto ap_idx = ptable::number_by_symbol(al.m_geom->atom_of_type(i));
              vector3<float> bc(0.0, 0.0, 1.0);
              if (ap_idx) {bc = ptable::get_inst()->arecs[*ap_idx-1].aColorJmol;}
              draw_list->AddCircleFilled(p_n, 8, ImColor(ImVec4(bc[0], bc[1], bc[2], 1.0f)));
              ImGui::Dummy(ImVec2(16, 16));
              ImGui::NextColumn();

              bool _l_type_to_hide = false;
              auto it = al.m_atom_type_to_hide.find(i);
              if (it != al.m_atom_type_to_hide.end()) _l_type_to_hide = true;

              ImGui::PushID(i);
              ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.8f, 0.8f));
              if (ImGui::Checkbox("", &_l_type_to_hide)) astate->make_viewport_dirty();
              ImGui::PopStyleVar(1);
              ImGui::PopID();

              if (_l_type_to_hide) al.m_atom_type_to_hide.insert(i);
              else {
                  auto it2 = al.m_atom_type_to_hide.find(i);
                  if (it2 != al.m_atom_type_to_hide.end()) al.m_atom_type_to_hide.erase(it2);
                }
              ImGui::NextColumn();
              ImGui::Separator();
            }

          ImGui::Columns(1);
          ImGui::Spacing();

          ImGui::EndGroup();
          ImGui::PopStyleVar();
          ImGui::Spacing();
        }


      if (ImGui::CollapsingHeader("Display and styling")){

          ImGui::Spacing();
          const char* items[] = { "Balls & sticks", "Dynamic lines", "X Atoms lines",
                                "Billboards"};

          if (ImGui::Combo("Style", reinterpret_cast<int*>(&al.m_cur_render_type),
                           items, 4)) astate->make_viewport_dirty();

          if (al.m_cur_render_type == ws_atoms_list_render_type::ball_and_stick ||
              al.m_cur_render_type == ws_atoms_list_render_type::billboards) {
              ImGui::Checkbox("Draw specular", &al.m_draw_specular);
              if (al.m_draw_specular)
                if (ImGui::SliderFloat("Sp. power", &al.m_shading_specular_power,
                                       0.5f, 15.0f, "%.4f", 1)) astate->make_viewport_dirty();
            }

          al.m_labels->render_ui();

          if (ImGui::SliderFloat("Atom size", &al.m_atom_scale_factor, 0.25f, 2.0f, "%.4f", 1))
            astate->make_viewport_dirty();

          if (ImGui::SliderFloat("Bond size", &al.m_bond_scale_factor, 0.02f, 2.0f, "%.4f", 1))
            astate->make_viewport_dirty();

          if (ImGui::Checkbox("Show atoms", &al.m_show_atoms)) astate->make_viewport_dirty();
          if (ImGui::Checkbox("Show bonds", &al.m_show_bonds)) astate->make_viewport_dirty();

          //ImGui::Checkbox("Show bonds with imaginary atom", &al.m_show_bond_with_img);
          if (al.m_geom->DIM > 0) {

              if (ImGui::Checkbox("Draw periodic cell", &al.m_draw_cell))
                astate->make_viewport_dirty();

              if (ImGui::Checkbox("Show imaginary atoms", &al.m_show_imaginary_atoms))
                astate->make_viewport_dirty();

              if (ImGui::Checkbox("Show imaginary bonds", &al.m_show_imaginary_bonds))
                astate->make_viewport_dirty();
            }

          if (ImGui::ColorEdit3("Cell ", al.m_cell_color.data()))
            astate->make_viewport_dirty();
          ImGui::Spacing();

        }

      if (ImGui::CollapsingHeader("Bonding table")) {

          ImGui::Spacing();
          bool rebuild_ngb{false};
          ImGui::Separator();
          ImGui::Spacing();
          ImGui::Checkbox("Show disabled", &al.m_bonding_table_show_disabled_record);
          ImGui::SameLine();
          if (ImGui::Button("Rebond new")){
              al.m_tws_tr->m_bonding_table.init_default(al.m_geom.get());
              rebuild_ngb = true;
            }
          ImGui::Spacing();
          ImGui::Separator();

          for (auto &elem : al.m_tws_tr->m_bonding_table.m_dist)
            if (al.m_bonding_table_show_disabled_record || elem.second.m_enabled) {
                if (ImGui::TreeNode(
                      fmt::format("[{}] - [{}]",
                                  al.m_geom->atom_of_type(elem.first.m_a),
                                  al.m_geom->atom_of_type(elem.first.m_b)).c_str())) {
                    ImGui::PushItemWidth(60);

                    if (ImGui::Checkbox("Enabled", &(elem.second.m_enabled)))
                      rebuild_ngb = true;

                    if (elem.second.m_enabled){
                        ImGui::SameLine();
                        ImGui::PushItemWidth(110);
                        if (ImGui::SliderFloat("Distance", &(elem.second.m_bonding_dist),
                                               0.01f, 10.0f)) {
                            al.m_tws_tr->m_bonding_table.update_pair_max_dist(elem.first.m_a,
                                                                              elem.first.m_b);
                            rebuild_ngb = true;
                          }
                      }
                    ImGui::TreePop();
                  }
                ImGui::Spacing();
              }

          ImGui::Spacing();

          if (rebuild_ngb) {
              al.m_tws_tr->do_action(act_rebuild_ntable);
              astate->make_viewport_dirty();
            }

        }

      //begin modify section
      if (ImGui::CollapsingHeader("Modify")){

          //check that we select 1 or 2 atoms
          if (al.m_atom_idx_sel.size()!=1 && al.m_atom_idx_sel.size()!=2){
              ImGui::Spacing();
              ImGui::BulletText("Select one atom to edit it");
              ImGui::BulletText("Select two atoms to specify \ndistance between them");
            }

          // if we selected one atom - show single modify widget
          else if (al.m_atom_idx_sel.size()==1) {
              ImGui::BulletText("Modify single atom:");
              static std::string custom_atom_name = "";

              static int last_selected = -1;
              if (last_selected != al.m_atom_idx_sel.begin()->m_atm){
                  custom_atom_name = al.m_geom->atom(al.m_atom_idx_sel.begin()->m_atm);
                  last_selected = al.m_atom_idx_sel.begin()->m_atm;
                }

              //ImGui::Separator();
              vector3<float> pos = al.m_geom->pos(al.m_atom_idx_sel.begin()->m_atm);
              ImGui::PushItemWidth(190);
              if (ImGui::InputFloat3("Edit position", pos.data())){
                  al.update_atom(al.m_atom_idx_sel.begin()->m_atm, pos);
                  astate->make_viewport_dirty();
                }

              ImGui::PushItemWidth(70);
              ImGui::InputText("Edit name", &custom_atom_name);
              ImGui::SameLine();
              if (ImGui::Button("Edit", ImVec2(100, 0)))
                if (custom_atom_name != "") {
                    al.update_atom(al.m_atom_idx_sel.begin()->m_atm, custom_atom_name);
                    astate->make_viewport_dirty();
                  }

            } // end one atom selection

          // if we selected two atoms - show distance modify widget
          else if (al.m_atom_idx_sel.size() == 2){
              ImGui::BulletText("Modify distance between real atoms");
              auto it1 = al.m_atom_idx_sel.begin();
              auto it2 = it1++;
              if (it1->m_idx != index::D(al.m_geom->DIM).all(0) ||
                  it2->m_idx != index::D(al.m_geom->DIM).all(0) ){
                  ImGui::BulletText("One of selected atom are imaginary");
                } else {
                  float dist_btw = (al.m_geom->pos(it1->m_atm, it1->m_idx) -
                                    al.m_geom->pos(it2->m_atm, it2->m_idx)).norm();

                  if (ImGui::SliderFloat("Distance", &dist_btw, 0.1f, 15.0f)){
                      vector3<float> r_btw = (al.m_geom->pos(it1->m_atm, it1->m_idx) +
                                              al.m_geom->pos(it2->m_atm, it2->m_idx))*0.5f;
                      vector3<float> dir_f =
                          (al.m_geom->pos(it1->m_atm, it1->m_idx) - r_btw).normalized();
                      vector3<float> dir_s =
                          (al.m_geom->pos(it2->m_atm, it2->m_idx) - r_btw).normalized();
                      al.m_geom->change_pos(it1->m_atm, r_btw + dir_f * dist_btw * 0.5f);
                      al.m_geom->change_pos(it2->m_atm, r_btw + dir_s * dist_btw * 0.5f);
                      astate->make_viewport_dirty();
                    }
                }
            } //end two atom selection

        }//end modify section

      if (ImGui::CollapsingHeader("Add atoms")) {
          static std::string custom_atom_name;
          ImGui::Separator();
          ImGui::PushItemWidth(200);
          ImGui::InputFloat3("Position", al.m_new_atom_pos.data());
          ImGui::InputText("Name", &custom_atom_name);
          ImGui::SameLine();
          if (ImGui::Button("Add", ImVec2(40, 0))){
              if (custom_atom_name != "") al.insert_atom(custom_atom_name,al.m_new_atom_pos);
              astate->make_viewport_dirty();
            }

        }

      // start animation block
      if (al.m_anim->animable()) {
          if (ImGui::CollapsingHeader("Animations")) {
              ImGui::Spacing();
              ImGui::TextUnformatted(
                    fmt::format("Total anims : {}", al.m_anim->m_anim_data.size()).c_str(),
                    nullptr);
              ImGui::PushItemWidth(140);

              ImGui::Separator();

              std::vector<std::string>  vStr;
              vStr.reserve(10);
              std::vector<char*>  vChar;
              for (size_t i = 0; i < al.m_anim->get_total_anims(); i++)
                vStr.push_back(al.m_anim->m_anim_data[i].m_anim_name);
              std::transform(vStr.begin(), vStr.end(), std::back_inserter(vChar),vec_str_to_char);

              ImGui::PushItemWidth(150);
              if (ImGui::Combo("Current animation", &al.m_anim->m_cur_anim, vChar.data(),
                               al.m_anim->get_total_anims())){
                  al.m_anim->m_cur_anim_time = 0.0f;
                  al.m_anim->update_geom_to_anim();
                  astate->make_viewport_dirty();
                }
              ImGui::Checkbox("Rebuild bonds", &al.m_anim->m_rebuild_bonds_in_anim);

              if (al.m_anim->get_cur_anim_type() != geom_anim_type::anim_static) {
                  ImGui::SliderFloat("Frame time(sec.)", &al.m_anim->m_anim_frame_time,
                                     0.01f, 3.0f);
                  ImGui::Checkbox("Play in cycle", &al.m_anim->m_play_cyclic);

                  ImGui::TextUnformatted(fmt::format("Frames count: {}",
                                                     al.m_anim->current_frame_count()).c_str(),
                                         nullptr);

                  ImGui::Separator();
                  ImGui::PushItemWidth(240);
                  if (ImGui::SliderFloat("Timeline", &al.m_anim->m_cur_anim_time, 0.0f,
                                         (al.m_anim->current_frame_count() - 1))){
                      if (!al.m_anim->m_play_anim) al.m_anim->update_geom_to_anim();
                    }

                  ImGui::ToggleButton("Play", &al.m_anim->m_play_anim);
                  ImGui::SameLine();
                  if (ImGui::Button("Begin")) {
                      al.m_anim->m_cur_anim_time = 0.0f;
                      al.m_anim->update_geom_to_anim();
                      astate->make_viewport_dirty();
                    }

                  ImGui::SameLine();
                  if (ImGui::Button("End")) {
                      al.m_anim->m_cur_anim_time = al.m_anim->current_frame_count() - 1;
                      al.m_anim->update_geom_to_anim();
                      astate->make_viewport_dirty();
                    }

                  ImGui::SameLine();
                  if (ImGui::Button("+Frame")) {
                      //TODO: unimplemented
                      al.m_anim->manual_frame_manipulate(+1.0f);
                      astate->make_viewport_dirty();
                    }

                  ImGui::SameLine();
                  if (ImGui::Button("-Frame")) {
                      al.m_anim->manual_frame_manipulate(-1.0f);
                      astate->make_viewport_dirty();
                    }
                }
            }
        }
      // end animation bloc

      if (ImGui::CollapsingHeader("Export")) {
          ImGui::Spacing();
          if (ImGui::Button("VASP POSCAR")) {
              al.dialog_save_to_file(qc_file_fmt::vasp_poscar);
            }

          ImGui::SameLine();
          if (ImGui::Button("XYZ")) {
              al.dialog_save_to_file(qc_file_fmt::standart_xyz);
            }

          ImGui::SameLine();
          if (ImGui::Button("CP2K coord.")) {
              al.dialog_save_to_file(qc_file_fmt::cp2k_coord_cell_section);
            }
          ImGui::Spacing();
        }

    }


  }

}
