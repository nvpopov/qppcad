#include <qppcad/ws_atoms_list/ws_atoms_list_labels_subsys.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/app_state.hpp>
#include <QFontMetrics>

using namespace qpp;
using namespace qpp::cad;

ws_atoms_list_labels_subsys_t::ws_atoms_list_labels_subsys_t(ws_atoms_list_t &_p_owner) {
  p_owner = &_p_owner;
}

void ws_atoms_list_labels_subsys_t::render_overlay(QPainter &painter) {

  if (m_render_inplace_hud && p_owner->m_selected) render_in_place_overlay(painter);

  if (m_style == ws_atoms_list_labels_style::show_none) return;

  app_state_t* astate = app_state_t::get_inst();

  painter.setPen(Qt::black);
  painter.setFont(QFont(astate->m_font_name, 16));


  //  app_state_t* astate = &(c_app::get_state());
  //  ImDrawList* imdrw = ImGui::GetOverlayDrawList();

  std::optional<vector2<float> > proj_pos;
  for (auto i = 0; i < p_owner->m_geom->nat(); i++) {

      if (p_owner->m_sel_vis &&
          p_owner->m_geom->xfield<bool>(xgeom_sel_vis, i)) continue;

      proj_pos = astate->camera->project(p_owner->m_pos + p_owner->m_geom->pos(i));

      std::string label;/* = fmt::format("{}", i);*/

      bool render_label{true};

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

        case ws_atoms_list_labels_style::show_charge : {
            label = fmt::format("{:2.2f}", p_owner->m_geom->xfield<float>(xgeom_charge, i));
            break;
          }

        case ws_atoms_list_labels_style::show_custom : {
            if (p_owner->m_geom->xfield<bool>(xgeom_label_state, i)) {
                label = p_owner->m_geom->xfield<std::string>(xgeom_label_text, i);
              } else {
                render_label = false;
              }
          }

        default:
          break;

        }

      if (render_label) {
          const int rect_size = 60;
          painter.drawText(
                int((*proj_pos)[0]-rect_size*0.5f),
              int((*proj_pos)[1]-rect_size*0.5f),
              rect_size, rect_size,
              Qt::AlignCenter, QString::fromStdString(label));
        }

    }
}

void ws_atoms_list_labels_subsys_t::render_in_place_overlay(QPainter &painter) {

  app_state_t* astate = app_state_t::get_inst();

  painter.resetTransform();

  int w = painter.device()->width();
  int h = painter.device()->height();

  int sh = 70;
  int max_types = 10;
  int ntypes = std::min(p_owner->m_geom->n_atom_types(), max_types);
  int w_h = 80;
  int padding_h = 10;
  int sph_padding = 5;
  int w_w = sh * ntypes + sph_padding * (ntypes+1);

  QRect lrect(w/2 - w_w/2, h - w_h - padding_h, w_w, w_h);
  QPen rectpen(QPen(Qt::black, 3, Qt::SolidLine));
  QPen rectpen2(QPen(Qt::black, 2, Qt::SolidLine));
  QPainterPath path;
  path.addRoundedRect(lrect,10, 10);
  painter.fillPath(path, Qt::white);
  painter.setPen(rectpen);
  painter.drawPath(path);

  QFont text_font(astate->m_font_name, 31, QFont::Weight::ExtraBold);
  QColor text_fill_color = QColor::fromRgbF(1,1,1);
  QString text;

  QColor fill_color = QColor::fromRgbF(0, 0, 1);

  for (int i = 0; i < ntypes; i++) {
     painter.setPen(rectpen);
     QRect sph(w/2 - w_w/2 + sh*i + sph_padding*(i+1), h - w_h - padding_h + sph_padding, sh, sh);

     auto ap_idx = ptable::number_by_symbol(p_owner->m_geom->atom_of_type(i));

     if (ap_idx) {
         vector3<float> color = ptable::get_inst()->arecs[*ap_idx - 1].m_color_jmol;
         fill_color.setRgbF(color[0], color[1], color[2]);
       } else fill_color = QColor::fromRgbF(0, 0, 1);

     painter.setBrush(fill_color);
     painter.drawEllipse(sph);

     text = QString::fromStdString(p_owner->m_geom->atom_of_type(i));
//     painter.setFont(text_font);
//     painter.drawText(sph, Qt::AlignCenter, text);

//     painter.setPen(rectpen2);
//     painter.setFont(text_font_small);
//     painter.drawText(sph, Qt::AlignCenter, text);

     painter.setBrush(text_fill_color);
     painter.setPen(rectpen2);
     QPainterPath text_path;
     QFontMetrics fmetric(text_font);
     painter.setRenderHint(QPainter::Antialiasing);
     text_path.addText((sph.left() + sph.right()) / 2 - fmetric.width(text) / 2,
                       sph.bottom() - fmetric.height() / 2 + padding_h,
                       text_font,
                       text);
     painter.drawPath(text_path);

    }

}
