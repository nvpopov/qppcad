#include <qppcad/ws_item/geom_view/geom_view_labels_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/core/app_state.hpp>
#include <QFontMetrics>

using namespace qpp;
using namespace qpp::cad;

geom_view_labels_subsys_t::geom_view_labels_subsys_t(geom_view_t &_p_owner) {
  p_owner = &_p_owner;
}

void geom_view_labels_subsys_t::render_overlay(QPainter &painter) {

  render_labels(painter);
  if (m_render_inplace_hud && p_owner->m_selected) render_in_place_overlay(painter);

}

void geom_view_labels_subsys_t::render_labels(QPainter &painter) {

  if (m_style == geom_labels_style_e::show_none) return;

  app_state_t* astate = app_state_t::get_inst();

  float _font_size = m_lbl_font_size;
  float _outline_size = m_outline_size;

  if (m_screen_scale) {
      float clmp_os = std::clamp(astate->camera->m_ortho_scale, 1.0f, 20.0f);
      _font_size = m_lbl_font_size * 10 / clmp_os;
      _outline_size = m_outline_size * 10 / clmp_os;
    }

  QPen rectpen(QPen(Qt::black, m_render_outlines ? _outline_size : 1, Qt::SolidLine));
  QColor text_fill_color = QColor::fromRgbF(1,1,1);

  QFont text_font_lb(astate->m_font_name, int(_font_size), QFont::Weight::Bold);

  painter.setBrush(text_fill_color);
  painter.setPen(rectpen);
  painter.setFont(text_font_lb);

  opt<vector2<float> > proj_pos;
  std::string label;
  QString label_qs;
  QPainterPath *text_path{nullptr};
  QFontMetrics fmetric(text_font_lb);
  QTransform transform;
  QRect font_rec;
  transform.reset();

  for (auto i = 0; i < p_owner->m_geom->nat(); i++) {

      if (p_owner->m_sel_vis && p_owner->m_geom->xfield<bool>(xgeom_sel_vis, i)) continue;

      if (!p_owner->m_geom->xfield<bool>(xgeom_label_show, i) && m_selective_lbl) continue;

      if (!p_owner->m_atom_type_to_hide.empty()) {
          auto it = p_owner->m_atom_type_to_hide.find(p_owner->m_geom->type_table(i));
          if (it != p_owner->m_atom_type_to_hide.end()) return;
        }

      proj_pos = astate->camera->project(p_owner->m_pos + p_owner->m_geom->pos(i));

      bool render_label{true};

      label = label_gen_fn(p_owner, m_style, i);
      render_label = !label.empty();

      /* render label */
      if (render_label) {

          label_qs = QString::fromStdString(label);
          font_rec = fmetric.boundingRect(label_qs);

          /* render outlines */
          if (m_render_outlines) {

              auto cached_pp = m_pp_cache.find({label, _font_size});

              if (cached_pp == m_pp_cache.end()) {
                  auto &new_pp = m_pp_cache[{label, _font_size}];
                  new_pp.addText(0, 0, text_font_lb, label_qs);
                  text_path = &new_pp;
                } else {
                  text_path = &cached_pp->second;
                }

              transform.reset();
              transform.translate((*proj_pos)[0] - fmetric.width(label_qs) / 2,
                                  (*proj_pos)[1] - font_rec.center().y());

              painter.setTransform(transform);
              if (text_path) painter.drawPath(*text_path);
              text_path = nullptr;

            } else {
              /* render default */
              painter.drawText(int((*proj_pos)[0]-font_rec.width()*0.5f),
                               int((*proj_pos)[1]-font_rec.height()*0.5f),
                               font_rec.width(), font_rec.height(),
                               Qt::AlignCenter, label_qs);

            }

        }
      /* end of render label */

    }

  painter.resetTransform();

}

void geom_view_labels_subsys_t::render_in_place_overlay(QPainter &painter) {

  app_state_t* astate = app_state_t::get_inst();

  painter.resetTransform();

  int w = painter.device()->width();
  int h = painter.device()->height();

  int sh = 70;
  int max_types = 10;
  int hidden_type = p_owner->m_atom_type_to_hide.size();
  int ntypes = std::min(p_owner->m_geom->n_atom_types(), max_types);
  int ntypes_wh = std::min(p_owner->m_geom->n_atom_types() - hidden_type, max_types);
  int w_h = 80;
  int padding_h = 16;
  int sph_padding = 5;
  int w_w = sh * ntypes_wh + sph_padding * (ntypes_wh+1);

  QRect lrect(static_cast<int>(w * m_inplace_offset[0] - w_w/2),
              static_cast<int>(h * m_inplace_offset[1] - w_h - padding_h), w_w, w_h);
  QPen rectpen(QPen(Qt::black, 3, Qt::SolidLine));
  QPen rectpen2(QPen(Qt::black, 2, Qt::SolidLine));
  QPainterPath path;
  path.addRoundedRect(lrect,10, 10);
  painter.fillPath(path, Qt::white);
  painter.setPen(rectpen);
  painter.drawPath(path);

  QFont text_font(astate->m_font_name, 28, QFont::Weight::ExtraBold);
  QColor text_fill_color = QColor::fromRgbF(1,1,1);
  QString text;

  QColor fill_color = QColor::fromRgbF(0, 0, 1);

  int r_i = 0;
  for (int i = 0; i < ntypes; i++) {

      auto it = p_owner->m_atom_type_to_hide.find(i);
      if (it != p_owner->m_atom_type_to_hide.end()) continue;

      painter.setPen(rectpen);
      QRect sph(
            static_cast<int>(w * m_inplace_offset[0] - w_w/2 + sh * r_i + sph_padding * (r_i+1)),
            static_cast<int>(h * m_inplace_offset[1] - w_h - padding_h + sph_padding),
            sh,
            sh);

      auto ap_idx = ptable::number_by_symbol(p_owner->m_geom->atom_of_type(i));

      if (ap_idx) {
          vector3<float> color = ptable::get_inst()->arecs[*ap_idx - 1].m_color_jmol;
          fill_color.setRgbF(color[0], color[1], color[2]);
        } else {
          fill_color = QColor::fromRgbF(0, 0, 1);
        }

      painter.setBrush(fill_color);
      painter.drawEllipse(sph);

      text = QString::fromStdString(p_owner->m_geom->atom_of_type(i));

      painter.setBrush(text_fill_color);
      painter.setPen(rectpen2);
      QPainterPath text_path;
      QFontMetrics fmetric(text_font);
      painter.setRenderHint(QPainter::Antialiasing);
      text_path.addText((sph.left() + sph.right()) / 2 - fmetric.width(text) / 2,
                        sph.bottom() - fmetric.height() / 2 + 2,
                        text_font,
                        text);
      painter.drawPath(text_path);

      r_i++;

    }

}

void geom_view_labels_subsys_t::labelize_sel_by_neighbours_count() {

  if (!p_owner) return;

  m_style = geom_labels_style_e::show_custom;

  for (auto &rec : p_owner->m_atom_idx_sel) {
      p_owner->m_geom->xfield<std::string>(xgeom_label_text, rec.m_atm) =
          fmt::format("{}_{}",
                      p_owner->m_geom->atom_name(rec.m_atm),
                      p_owner->m_tws_tr->n(rec.m_atm));
      p_owner->m_geom->xfield<bool>(xgeom_label_show, rec.m_atm) = true;
    }

}

void geom_view_labels_subsys_t::labelize_sel_by_dist_factor() {

  if (!p_owner) return;

  m_style = geom_labels_style_e::show_custom;

  //build classes

  std::vector<std::vector<atom_dist_rec_t<float> > > ngbs_dp;
  std::vector<size_t> ngbs_lookup;

  for (auto &rec : p_owner->m_atom_idx_sel) {

      ngbs_lookup.push_back(rec.m_atm);

      atom_dist_rec_t<float> new_rec;

      //iterate over all neighbours
      for (size_t i = 0; i < p_owner->m_tws_tr->n(rec.m_atm); i++) {

        }

    }

}

std::string geom_view_labels_subsys_t::label_gen_fn(geom_view_t *owner,
                                                    geom_labels_style_e lbl_style,
                                                    size_t atom_id) {

  switch (lbl_style) {

    case geom_labels_style_e::show_type : {
        return owner->m_geom->atom(atom_id);
      }

    case geom_labels_style_e::show_id : {
        return fmt::format("{}", atom_id);
      }

    case geom_labels_style_e::show_id_type : {
        return fmt::format("{}{}", owner->m_geom->atom(atom_id), atom_id);
      }

    case geom_labels_style_e::show_charge : {
        return fmt::format("{:2.2f}", owner->m_geom->xfield<float>(xgeom_charge, atom_id));
      }

    case geom_labels_style_e::show_charge_type : {
        return fmt::format("{}[{:2.2f}]",
                           owner->m_geom->atom(atom_id),
                           owner->m_geom->xfield<float>(xgeom_charge, atom_id));
      }

    case geom_labels_style_e::show_charge_id_type : {
        return fmt::format("{}{}[{:2.2f}]",
                           owner->m_geom->atom(atom_id),
                           atom_id,
                           owner->m_geom->xfield<float>(xgeom_charge, atom_id));
      }

    case geom_labels_style_e::show_custom : {
        if (owner->m_geom->xfield<bool>(xgeom_label_show, atom_id)) {
            return owner->m_geom->xfield<std::string>(xgeom_label_text, atom_id);
          }
      }

    default:
      break;

    }

  return "";

}
