#ifndef QPPCAD_WS_ATOMS_LIST_CONTEXT_MENU
#define QPPCAD_WS_ATOMS_LIST_CONTEXT_MENU
#include <qppcad/ws_atoms_list.hpp>

namespace qpp {

  namespace cad {

    struct ws_atoms_list_context_menu_helper {

        static void render_item_edit_menu (ws_atoms_list_t &al) {

        }

        static void render_content_edit_menu (ws_atoms_list_t &al) {

          if (al.support_content_editing()) {

              if (ImGui::BeginMenu("Selection")) {
                  if (ImGui::BeginMenu("Select by type")){
                      for (auto i = 0; i < al.m_geom->n_atom_types(); i++)
                        if (ImGui::MenuItem(al.m_geom->atom_of_type(i).c_str()))
                          al.select_by_type(i);
                      ImGui::EndMenu();
                    }
                  if (ImGui::MenuItem("Select all")) al.select_atoms(true);
                  if (ImGui::MenuItem("Unselect all")) al.select_atoms(false);
                  if (ImGui::MenuItem("Invert selection")) al.invert_selected_atoms();
                  ImGui::EndMenu();
                }

              if (al.m_geom->DIM == 3) {
                  if (ImGui::BeginMenu("Axial scale")) {
                      static float axs_a{1.0f}, axs_b{1.0f}, axs_c{1.0f};
                      ImGui::DragFloat("Scale a", &axs_a, 0.01f, 0.01f, 1.5f);
                      ImGui::DragFloat("Scale b", &axs_b, 0.01f, 0.01f, 1.5f);
                      ImGui::DragFloat("Scale c", &axs_c, 0.01f, 0.01f, 1.5f);
                      if (ImGui::Button("Apply Scale")) {
                          al.apply_axial_scale(axs_a, axs_b, axs_c);
                        }
                      ImGui::EndMenu();
                    }
                }

              if (al.m_atom_idx_sel.size() == 2){
                  if (ImGui::BeginMenu("Append new atom between")){

                      auto it1 = al.m_atom_idx_sel.begin();
                      auto it2 = it1++;
                      vector3<float> r_btw{0.0, 0.0, 0.0};

                      if (it1 != al.m_atom_idx_sel.end() && it2 != al.m_atom_idx_sel.end())
                        r_btw = (al.m_geom->pos(it1->m_atm, it1->m_idx) +
                                 al.m_geom->pos(it2->m_atm, it2->m_idx))*0.5f;

                      for (auto at_type = 0; at_type < al.m_geom->n_types(); at_type++)
                        if (ImGui::MenuItem(al.m_geom->atom_of_type(at_type).c_str())) {
                            al.insert_atom(at_type, r_btw);
                          }

                      static std::string custom_atom_name;
                      ImGui::PushItemWidth(100);
                      ImGui::PushID(1);
                      ImGui::InputText("", &custom_atom_name);
                      ImGui::PopID();
                      ImGui::SameLine();
                      if (ImGui::Button("Add custom")){
                          al.m_geom->add(custom_atom_name, r_btw);
                        }
                      ImGui::EndMenu();
                    }



                  if (ImGui::BeginMenu("Two atoms manipulation")){
                      auto it1 = al.m_atom_idx_sel.begin();
                      auto it2 = it1++;
                      float dist_btw = (al.m_geom->pos(it1->m_atm, it1->m_idx) -
                                        al.m_geom->pos(it2->m_atm, it2->m_idx)).norm();

                      if (ImGui::SliderFloat("Distance between two atoms", &dist_btw, 0.1f, 5.0f)){

                          vector3<float> r_btw = (al.m_geom->pos(it1->m_atm, it1->m_idx) +
                                                  al.m_geom->pos(it2->m_atm, it2->m_idx))*0.5f;
                          vector3<float> dir_f =
                              (al.m_geom->pos(it1->m_atm, it1->m_idx) - r_btw).normalized();
                          vector3<float> dir_s =
                              (al.m_geom->pos(it2->m_atm, it2->m_idx) - r_btw).normalized();

                          al.m_geom->change_pos(it1->m_atm, r_btw + dir_f * dist_btw * 0.5f);
                          al.m_geom->change_pos(it2->m_atm, r_btw + dir_s * dist_btw * 0.5f);
                        }

                      ImGui::EndMenu();
                    }
                }

              if (al.m_atom_sel.size() > 0) {
                  if (ImGui::MenuItem(fmt::format("Delete selected atoms({})",
                                                  al.m_atom_sel.size()).c_str())){
                      al.delete_selected_atoms();
                    }

                  if (ImGui::BeginMenu("Barycentric n-atom scale")) {
                      static float uniform_scale{0.9f};
                      ImGui::SliderFloat("Uniform scale amount", &uniform_scale, 0.001f, 2.0f);

                      if (ImGui::Button("Apply scaling")) {
                          vector3<float> center{0.0f, 0.0f, 0.0f};
                          for (auto &rec : al.m_atom_sel) center += al.m_geom->pos(rec);
                          center /= al.m_atom_sel.size();

                          for (auto &rec : al.m_atom_sel) {
                              vector3<float> new_pos_dist = center - al.m_geom->pos(rec);
                              vector3<float> new_pos = al.m_geom->pos(rec) +
                                                       (1-uniform_scale) * new_pos_dist ;
                              al.update_atom(rec, new_pos);
                            }
                        }

                      ImGui::EndMenu();
                    }

                }

              ImGui::Separator();

              if (al.m_atom_idx_sel.size() == 0){
                  if (ImGui::BeginMenu("Add new atom")){
                      static std::string custom_atom_name;
                      ImGui::BulletText("Add new atom in local atom list frame");
                      ImGui::Separator();
                      ImGui::InputFloat3("Position", al.m_new_atom_pos.data());
                      ImGui::InputText("New atom name", &custom_atom_name);
                      ImGui::SameLine();
                      if (ImGui::Button("Add"))
                        if (custom_atom_name != "") al.insert_atom(custom_atom_name,
                                                                   al.m_new_atom_pos);
                      ImGui::EndMenu();
                    }
                }

              al.m_measure->render_ui_context();

            }
        } // end render_content_edit_menu
    };
  }
}

#endif
