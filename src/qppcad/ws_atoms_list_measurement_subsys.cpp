#include <qppcad/ws_atoms_list_measurement_subsys.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/app_state.hpp>

namespace qpp {

  namespace cad {

    float ws_atoms_list_measurement_subsys_t::dist (const size_t idx) {
      vector3<float> l_s, l_e;
      l_s = p_owner->m_pos +
            p_owner->m_geom->pos(m_records[idx].m_at1,m_records[idx].m_idx1);
      l_e = p_owner->m_pos +
            p_owner->m_geom->pos(m_records[idx].m_at2,m_records[idx].m_idx2);
      return (l_e - l_s).norm();
    }

    void ws_atoms_list_measurement_subsys_t::add_bond_measurement (const uint32_t atm1,
                                                                   const uint32_t atm2,
                                                                   const index idx1,
                                                                   const index idx2) {
      m_records.emplace_back(atm1, atm2, idx1, idx2);
    }

    ws_atoms_list_measurement_subsys_t::ws_atoms_list_measurement_subsys_t(
        ws_atoms_list_t &_p_owner) {
      p_owner = &_p_owner;
    }

    void ws_atoms_list_measurement_subsys_t::remove_bond_measurement (const size_t measure_idx) {
      if (measure_idx < m_records.size())
        m_records.erase(m_records.begin() + measure_idx);
    }


    std::optional<size_t> ws_atoms_list_measurement_subsys_t::is_bond_measurement_exist (
        const uint32_t atm1, const uint32_t atm2, const index idx1, const index idx2) {

      for (auto i = 0; i < m_records.size(); i++)
        if ((m_records[i].m_at1 == atm1 && m_records[i].m_at2 == atm2 &&
             m_records[i].m_idx1 == idx1 && m_records[i].m_idx2 == idx2) ||
            (m_records[i].m_at1 == atm2 && m_records[i].m_at2 == atm1 &&
             m_records[i].m_idx1 == idx2 && m_records[i].m_idx2 == idx1))
          return std::optional<size_t>(i);

      return std::nullopt;

    }

    void ws_atoms_list_measurement_subsys_t::render() {
      //render bond measurements
      //deprecated
    }

    void ws_atoms_list_measurement_subsys_t::render_overlay(QPainter &painter) {

      app_state_t* astate = app_state_t::get_inst();

      std::optional<vector2<float> > l_s, l_e;
      QPen linepen(QPen(Qt::black, 6, Qt::DotLine, Qt::RoundCap));
      QPen rectpen(QPen(Qt::black, 3, Qt::SolidLine));
      painter.setFont(QFont("Hack-Regular", 13));

      for (auto &record : m_records)
        if (record.m_show) {
            l_s = astate->camera->project(
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at1,record.m_idx1));

            l_e = astate->camera->project(
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at2,record.m_idx2));

            float dist = (p_owner->m_geom->pos(record.m_at1,record.m_idx1) -
                          p_owner->m_geom->pos(record.m_at2,record.m_idx2)).norm();

            vector2<float> mid = (*l_s + *l_e) * 0.5f;
            const float rect_size = 100;

            QLineF linef(round((*l_s)[0]) + 0.5, round((*l_s)[1]) + 0.5,
                round((*l_e)[0]) + 0.5, round((*l_e)[1]) + 0.5);
            painter.setPen(linepen);
            painter.drawLine(linef);

            double angle = 180 * std::atan2(linef.y2()-linef.y1(), linef.x2()-linef.x1()) / qpp::pi;

            angle = angle + std::ceil( -angle / 360 ) * 360;
            //std::cout << angle << std::endl;
            if (angle > 90 && angle < 270) angle = angle+180;

            painter.translate(mid[0], mid[1]);
            painter.rotate(angle);
            QPainterPath path;
            path.addRoundedRect(QRectF(-rect_size*0.5f, 35*0.5f + 10, 100, 35),
                                10, 10);
            painter.fillPath(path, Qt::white);
            painter.setPen(rectpen);
            painter.drawPath(path);
            painter.drawText(-rect_size*0.5f, -rect_size*0.15f + 10,
                              rect_size, rect_size,
                              Qt::AlignCenter, QString::fromStdString(fmt::format("{}", dist)) );
            painter.resetTransform();
          }
      //      painter->restore();
    }

    //    void ws_atoms_list_measurement_subsys_t::render_overlay() {

    //      app_state_t* astate = &(c_app::get_state());

    //      ImDrawList* imdrw = ImGui::GetOverlayDrawList();
    //      //imdrw->AddText(ImVec2(222,200),  ImColor(1.0f, 1.0f, 1.0f, 1.0f), "SiSSSSSS");

    //      vector3<float> l_s, l_e;

    //      for (auto &record : m_records)
    //        if (record.m_show) {

    //            l_s = p_owner->m_pos + p_owner->m_geom->pos(record.at1,record.idx1);
    //            l_e = p_owner->m_pos + p_owner->m_geom->pos(record.at2,record.idx2);

    //            auto uproj = astate->camera->project((l_s+l_e)*0.5f);
    //            if (uproj) {
    //                imdrw->AddRectFilled(ImVec2( (*uproj)[0] - 12, (*uproj)[1] - 6),
    //                    ImVec2( (*uproj)[0] + 82, (*uproj)[1] + 20),
    //                    ImColor(0.0f, 0.0f, 0.0f, 1.0f),
    //                    4.0f);
    //                imdrw->AddRectFilled(ImVec2( (*uproj)[0] - 10, (*uproj)[1] - 4),
    //                    ImVec2( (*uproj)[0] + 80, (*uproj)[1] + 18),
    //                    ImColor(1.0f, 1.0f, 1.0f, 1.0f),
    //                    4.0f);
    //                imdrw->AddText(ImVec2( (*uproj)[0] + 8, (*uproj)[1] - 4),
    //                    ImColor(0.0f, 0.0f, 0.0f, 1.0f),
    //                    fmt::format("{}", (l_s-l_e).norm()).c_str());
    //              }

    //          }

    //    }

    //    void ws_atoms_list_measurement_subsys_t::render_ui_obj_inst() {

    //      if (ImGui::CollapsingHeader("Measurements")) {
    //          ImGui::Spacing();

    //          if (ImGui::TreeNode("Interatomic distances")) {
    //              for (auto i = 0; i < m_records.size(); i++) {
    //                  float dist_c = dist(i);
    //                  ImGui::Separator();

    //                  if (ImGui::TreeNode(fmt::format("[{}] [{}{}:{}{}] i=[{}:{}]",
    //                                                  i,
    //                                                  p_owner->m_geom->atom_name(m_records[i].at1),
    //                                                  m_records[i].at1,
    //                                                  p_owner->m_geom->atom_name(m_records[i].at2),
    //                                                  m_records[i].at2,
    //                                                  m_records[i].idx1,
    //                                                  m_records[i].idx2).c_str())){
    //                      ImGui::Spacing();
    //                      // ImGui::TextUnformatted("Type: interatomic distance");
    //                      if (ImGui::Button("Delete")) remove_bond_measurement(i);
    //                      ImGui::SameLine();

    //                      if (ImGui::Button("Copy"))
    //                        c_app::copy_to_clipboard(fmt::format("{}",  dist_c).c_str());

    //                      ImGui::SameLine();
    //                      ImGui::Checkbox("Show", &m_records[i].m_show);
    //                      ImGui::TextUnformatted(fmt::format("Distance = {}", dist_c).c_str());
    //                      ImGui::TreePop();
    //                    }

    //                  if (i == m_records.size()-1) ImGui::Separator();
    //                }
    //              if (m_records.empty()) ImGui::BulletText("No measurements");
    //              ImGui::TreePop();
    //            }

    //        }

    //    }

    //    void ws_atoms_list_measurement_subsys_t::render_ui_context() {

    //      if (p_owner->m_atom_idx_sel.size() == 2) {
    //          if (ImGui::BeginMenu("Measurements")) {
    //              auto it1 = p_owner->m_atom_idx_sel.begin();
    //              auto it2 = ++(p_owner->m_atom_idx_sel.begin());

    //              auto cur_sel = is_bond_measurement_exist(it1->m_atm, it2->m_atm,
    //                                                       it1->m_idx, it2->m_idx);

    //              if (!cur_sel) {
    //                  if (ImGui::MenuItem("Add distance measurement"))
    //                    add_bond_measurement( it1->m_atm, it2->m_atm,
    //                                          it1->m_idx, it2->m_idx);
    //                } else {
    //                  if (ImGui::MenuItem("Remove distance measurements"))
    //                    remove_bond_measurement(*cur_sel);
    //                }

    //              ImGui::EndMenu();
    //            }
    //        }

    //    }

  }

}
