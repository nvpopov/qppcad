#include <qppcad/ws_atoms_list_labels_subsys.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_atoms_list_labels_subsys_t::ws_atoms_list_labels_subsys_t(ws_atoms_list_t &_p_owner) {
  p_owner = &_p_owner;
}

void ws_atoms_list_labels_subsys_t::render_overlay(QPainter *painter) {

  if (m_style == show_none) return;

  app_state_t* astate = app_state_t::get_inst();

  painter->setPen(Qt::black);
  painter->setFont(QFont("Hack-Regular", 16));


//  app_state_t* astate = &(c_app::get_state());
//  ImDrawList* imdrw = ImGui::GetOverlayDrawList();

  std::optional<vector2<float> > proj_pos;
  for (auto i = 0; i < p_owner->m_geom->nat(); i++) {
      proj_pos = astate->camera->project(p_owner->m_pos + p_owner->m_geom->pos(i));

      std::string label;/* = fmt::format("{}", i);*/

      switch (m_style) {

        case ws_atoms_list_labels_style::show_type : {
            label = p_owner->m_geom->atom(i);
            break;
          }

        case ws_atoms_list_labels_style::show_id : {
            label = fmt::format("{}", i);
            break;
          }

        case ws_atoms_list_labels_style::show_id_type : {
            label = fmt::format("{}{}", p_owner->m_geom->atom(i), i);
            break;
          }

        default:
          break;

        }

       const float rect_size = 60;
       painter->drawText((*proj_pos)[0]-rect_size*0.5f, (*proj_pos)[1]-rect_size*0.5f,
           rect_size, rect_size,
           Qt::AlignCenter, QString::fromStdString(label));
      //ImGui::SetWindowFontScale(2.0f);

//      imdrw->AddText(
//            ImVec2((*proj_pos)[0]-label.length()*5, (*proj_pos)[1]-label.length()*5),
//          ImColor(0.0f, 0.0f, 0.0f, 1.0f),
//          label.c_str());
//      ImGui::PopFont();
    }
}
