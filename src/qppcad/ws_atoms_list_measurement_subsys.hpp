#ifndef QPP_CAD_WS_ATOMS_LIST_MEASURE_S
#define QPP_CAD_WS_ATOMS_LIST_MEASURE_S

#include <geom/geom.hpp>
#include <qppcad/app.hpp>
#include <vector>
#include <algorithm>

namespace qpp {

  namespace cad {

    template<typename AINT = uint32_t>
    struct measurement_bond_record {

        AINT at1;
        AINT at2;
        index idx1;
        index idx2;
        bool m_show{true};

        measurement_bond_record (const AINT _at1, const AINT _a2,
                                 const index _idx1, const index _idx2) :
          at1(_at1), at2(_a2), idx1(_idx1), idx2(_idx2) {}

    };

    template <typename DATA, typename REAL = float, typename AINT  = uint32_t>
    class ws_atoms_list_measurement_subsys_t {

      private:

        DATA* p_owner;
        std::vector<measurement_bond_record<AINT> > m_records;

      public:

        ws_atoms_list_measurement_subsys_t (DATA &_p_owner) {
          p_owner = &_p_owner;
        }

        REAL dist (const size_t idx) {
          vector3<float> l_s, l_e;
          l_s = p_owner->m_pos +
                p_owner->m_geom->pos(m_records[idx].at1,m_records[idx].idx1);
          l_e = p_owner->m_pos +
                p_owner->m_geom->pos(m_records[idx].at2,m_records[idx].idx2);
          return (l_e - l_s).norm();
        }

        void add_bond_measurement (const AINT atm1, const AINT atm2,
                                   const index idx1, const index idx2) {
          m_records.emplace_back(atm1, atm2, idx1, idx2);
        }

        void remove_bond_measurement (const size_t measure_idx) {
          if (measure_idx < m_records.size())
            m_records.erase(m_records.begin() + measure_idx);
        }

        std::optional<size_t> is_bond_measurement_exist (const AINT atm1,
                                                         const AINT atm2,
                                                         const index idx1,
                                                         const index idx2) {

          for (auto i = 0; i < m_records.size(); i++)
            if ((m_records[i].at1 == atm1 && m_records[i].at2 == atm2 &&
                 m_records[i].idx1 == idx1 && m_records[i].idx2 == idx2) ||
                (m_records[i].at1 == atm2 && m_records[i].at2 == atm1 &&
                 m_records[i].idx1 == idx2 && m_records[i].idx2 == idx1))
              return std::optional<size_t>(i);

          return std::nullopt;

        }

        void render () {
          //render bond measurements
          app_state_t* astate = &(c_app::get_state());
          astate->dp->begin_render_line_styled();

          vector3<float> l_s, l_e;
          glDisable(GL_DEPTH_TEST);
          for (auto &record : m_records)
            if (record.m_show) {
                l_s = p_owner->m_pos + p_owner->m_geom->pos(record.at1,record.idx1);
                l_e = p_owner->m_pos + p_owner->m_geom->pos(record.at2,record.idx2);
                astate->dp->render_line_styled(clr_black, l_s, l_e, 7.0f);
                astate->dp->render_line_styled(clr_white, l_s, l_e, 4.0f);
              }
          glEnable(GL_DEPTH_TEST);
          astate->dp->end_render_line_styled();

        }

        void render_overlay () {

          app_state_t* astate = &(c_app::get_state());

          ImDrawList* imdrw = ImGui::GetOverlayDrawList();
          //imdrw->AddText(ImVec2(222,200),  ImColor(1.0f, 1.0f, 1.0f, 1.0f), "SiSSSSSS");

          vector3<float> l_s, l_e;
          imdrw->PushClipRect(ImVec2(astate->viewport_xy_c[0], astate->viewport_xy_c[1]),
              ImVec2(astate->viewport_xy_c[0] + astate->viewport_size_c[0],
              astate->viewport_xy_c[1] + astate->viewport_size_c[1]));

          for (auto &record : m_records)
            if (record.m_show) {

                l_s = p_owner->m_pos + p_owner->m_geom->pos(record.at1,record.idx1);
                l_e = p_owner->m_pos + p_owner->m_geom->pos(record.at2,record.idx2);

                auto uproj = astate->camera->project((l_s+l_e)*0.5f);
                if (uproj) {
                    //                  std::cout
                    //                      << astate->viewport_xy_c[0] << " "
                    //                      << astate->viewport_xy_c[1] << " "
                    //                      << astate->viewport_size_c[0] << " "
                    //                                                    << astate->viewport_size_c[1]
                    //                                                    << " "
                    //                                                    << (*uproj)[0] << " "
                    //                                                    << (*uproj)[1] << std::endl;
                    imdrw->AddRectFilled(
                          ImVec2( (*uproj)[0]-12, (*uproj)[1]-6),
                        ImVec2( (*uproj)[0]+82, (*uproj)[1]+20),
                        ImColor(0.0f, 0.0f, 0.0f, 1.0f),
                        4.0f);
                    imdrw->AddRectFilled(
                          ImVec2( (*uproj)[0]-10, (*uproj)[1]-4),
                        ImVec2( (*uproj)[0]+80, (*uproj)[1]+18),
                        ImColor(1.0f, 1.0f, 1.0f, 1.0f),
                        4.0f);
                    imdrw->AddText(ImVec2( (*uproj)[0]+4, (*uproj)[1]-2),
                        ImColor(0.0f, 0.0f, 0.0f, 1.0f),
                        fmt::format("{}", (l_s-l_e).norm()).c_str());
                  }

              }

          imdrw->PopClipRect();

        }

        void render_ui_obj_inst () {

          if (ImGui::CollapsingHeader("Measurements")) {
              ImGui::Spacing();

              if (ImGui::TreeNode("Interatomic distances")) {
                  for (auto i = 0; i < m_records.size(); i++){
                      float dist_c = dist(i);
                      ImGui::Separator();
                      if (ImGui::TreeNode(fmt::format("[{}] [{}{}:{}{}] i=[{}:{}]",
                                                      i,
                                                      p_owner->m_geom->atom_name(m_records[i].at1),
                                                      m_records[i].at1,
                                                      p_owner->m_geom->atom_name(m_records[i].at2),
                                                      m_records[i].at2,
                                                      m_records[i].idx1,
                                                      m_records[i].idx2).c_str())){
                          ImGui::Spacing();
                          // ImGui::TextUnformatted("Type: interatomic distance");
                          if (ImGui::Button("Delete")) remove_bond_measurement(i);
                          ImGui::SameLine();

                          if (ImGui::Button("Copy")) {
                              c_app::copy_to_clipboard(fmt::format("{}",  dist_c).c_str());
                            }

                          ImGui::SameLine();
                          ImGui::Checkbox("Show", &m_records[i].m_show);
                          ImGui::TextUnformatted(fmt::format("Distance = {}", dist_c).c_str());
                          ImGui::TreePop();
                        }

                      if (i == m_records.size()-1) ImGui::Separator();
                    }
                  ImGui::TreePop();
                }
            }


        }

        void render_ui_context () {

          if (p_owner->m_atom_idx_sel.size() == 2) {
              if (ImGui::BeginMenu("Measurements")) {
                  auto it1 = p_owner->m_atom_idx_sel.begin();
                  auto it2 = ++(p_owner->m_atom_idx_sel.begin());

                  auto cur_sel = is_bond_measurement_exist(it1->m_atm, it2->m_atm,
                                                           it1->m_idx, it2->m_idx);

                  if (!cur_sel) {
                      if (ImGui::MenuItem("Add distance measurement"))
                        add_bond_measurement( it1->m_atm, it2->m_atm,
                                              it1->m_idx, it2->m_idx);
                    } else {
                      if (ImGui::MenuItem("Remove distance measurements"))
                        remove_bond_measurement(*cur_sel);
                    }

                  ImGui::EndMenu();
                }
            }

        } // end render_ui_context
    };

  }

}

#endif
