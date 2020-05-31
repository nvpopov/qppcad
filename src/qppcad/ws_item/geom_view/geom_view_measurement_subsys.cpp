#include <qppcad/ws_item/geom_view/geom_view_measurement_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/core/json_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

float geom_view_msr_subsys_t::dist (const size_t idx) {

  vector3<float> l_s, l_e;
  l_s = p_owner->m_pos.get_value()
        + p_owner->m_geom->pos(m_dist_recs[idx].m_at1,m_dist_recs[idx].m_idx1);
  l_e = p_owner->m_pos.get_value()
        + p_owner->m_geom->pos(m_dist_recs[idx].m_at2,m_dist_recs[idx].m_idx2);
  return (l_e - l_s).norm();

}

void geom_view_msr_subsys_t::add_bond_msr (const uint32_t atm1, const uint32_t atm2,
                                           const index idx1, const index idx2) {

  if (!is_bond_msr_exists(atm1, atm2, idx1, idx2)) {
      m_dist_recs.emplace_back(atm1, atm2, idx1, idx2);
      app_state_t* astate = app_state_t::get_inst();
      astate->astate_evd->cur_ws_selected_item_measurements_changed();
    }

}

void geom_view_msr_subsys_t::add_angle_msr(
    const uint32_t atm1, const uint32_t atm2, const uint32_t atm3,
    const index idx1, const index idx2, const index idx3) {

  if (!is_angle_msr_exists(atm1, atm2, atm3, idx1, idx2, idx3)) {
      m_angle_recs.emplace_back(atm1, atm2, atm3, idx1, idx2, idx3);
      app_state_t* astate = app_state_t::get_inst();
      astate->astate_evd->cur_ws_selected_item_measurements_changed();
    }

}

geom_view_msr_subsys_t::geom_view_msr_subsys_t(geom_view_t &_p_owner) {

  p_owner = &_p_owner;

  m_render_dist.set_value(true); add_hs_child(&m_render_dist);
  m_render_angle.set_value(true); add_hs_child(&m_render_angle);
  m_render_dist_legend.set_value(false); add_hs_child(&m_render_dist_legend);
  m_render_angle_legend.set_value(false); add_hs_child(&m_render_angle_legend);

}

void geom_view_msr_subsys_t::rm_bond_msr (const size_t measure_idx) {

  if (measure_idx < m_dist_recs.size()) {
      m_dist_recs.erase(m_dist_recs.begin() + measure_idx);
      int r_dist_idx = m_cur_dist_rec_ui - 1;
      if (r_dist_idx >= measure_idx) m_cur_dist_rec_ui--;
      if (m_dist_recs.empty()) m_cur_dist_rec_ui = 0;
    }

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_selected_item_measurements_changed();

}

void geom_view_msr_subsys_t::rm_angle_msr(const size_t measure_idx) {

  if (measure_idx < m_angle_recs.size())
    m_angle_recs.erase(m_angle_recs.begin() + measure_idx);

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_selected_item_measurements_changed();

}


std::optional<size_t> geom_view_msr_subsys_t::is_bond_msr_exists (const uint32_t atm1, const uint32_t atm2,
                                                        const index idx1, const index idx2) {

  for (auto i = 0; i < m_dist_recs.size(); i++)
    if ((m_dist_recs[i].m_at1 == atm1 && m_dist_recs[i].m_at2 == atm2
        && m_dist_recs[i].m_idx1 == idx1 && m_dist_recs[i].m_idx2 == idx2)
        || (m_dist_recs[i].m_at1 == atm2 && m_dist_recs[i].m_at2 == atm1
        && m_dist_recs[i].m_idx1 == idx2 && m_dist_recs[i].m_idx2 == idx1))
      return std::optional<size_t>(i);

  return std::nullopt;

}

std::optional<size_t> geom_view_msr_subsys_t::is_angle_msr_exists(
    const uint32_t atm1, const uint32_t atm2, const uint32_t atm3,
    const index idx1, const index idx2, const index idx3) {

  for (auto i = 0; i < m_angle_recs.size(); i++)
    if (m_angle_recs[i].m_at1 == atm1
        && m_angle_recs[i].m_at2 == atm2
        && m_angle_recs[i].m_at3 == atm3
        && m_angle_recs[i].m_idx1 == idx1
        && m_angle_recs[i].m_idx2 == idx2
        && m_angle_recs[i].m_idx3 == idx3)
      return std::optional<size_t>(i);

  return std::nullopt;

}

void geom_view_msr_subsys_t::render() {
  //render bond measurements
  //deprecated
}

void geom_view_msr_subsys_t::render_overlay(QPainter &painter) {

  app_state_t* astate = app_state_t::get_inst();

  std::optional<vector2<float> > l_s, l_e;
  QPen linepen_dot_line(QPen(Qt::black, 4, Qt::DotLine, Qt::RoundCap));
  QPen linepen_solid_line(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
  QPen linepen_sel(QPen(Qt::red, 4, Qt::DotLine, Qt::RoundCap));

  QPen rectpen(QPen(Qt::black, 3, Qt::SolidLine));
  QPen rectpen2(QPen(Qt::black, 1, Qt::SolidLine));

  QColor text_fill_color = QColor::fromRgbF(1,1,1);

  painter.setFont(QFont(astate->m_font_name, 13));
  painter.resetTransform();

  if (m_render_dist.get_value())
    for (size_t i = 0; i < m_dist_recs.size(); i++) {

        auto &record = m_dist_recs[i];

        if (record.m_show.get_value()) {

            if (record.m_at1 >= static_cast<uint32_t>(p_owner->m_geom->nat())
                || record.m_at2 >= static_cast<uint32_t>(p_owner->m_geom->nat())) {
                continue;
              }

            l_s = astate->camera->project(
                    p_owner->m_pos.get_value() + p_owner->m_geom->pos(record.m_at1,record.m_idx1));

            l_e = astate->camera->project(
                    p_owner->m_pos.get_value() + p_owner->m_geom->pos(record.m_at2,record.m_idx2));

            if (!l_s || !l_e) continue;

            vector3<float> bond_color = record.m_bond_color.get_value();
            float pair_term_width = record.m_pair_term_width.get_value();
            auto delta_offset = record.m_delta_offset.get_value();

            float dist = (p_owner->m_geom->pos(record.m_at1,record.m_idx1) -
                          p_owner->m_geom->pos(record.m_at2,record.m_idx2)).norm();

            vector2<float> mid = (*l_s + *l_e) * 0.5f;

            QFont font(astate->m_font_name, record.m_font_size.get_value());
            painter.setFont(font);

            QLineF linef(round((*l_s)[0]) + 0.5, round((*l_s)[1]) + 0.5,
                round((*l_e)[0]) + 0.5, round((*l_e)[1]) + 0.5);

            auto line_len = linef.length();

            //compose pen
            Qt::PenStyle pen_style;

            switch (record.m_line_render_style.get_value()) {
              case msr_line_style_e::msr_line_dashed : {
                  pen_style = Qt::PenStyle::DashLine;
                  break;
                }
              case msr_line_style_e::msr_line_solid: {
                  pen_style = Qt::PenStyle::SolidLine;
                  break;
                }
              case msr_line_style_e::msr_line_ddashed: {
                  pen_style = Qt::PenStyle::DashDotLine;
                  break;
                }
              case msr_line_style_e::msr_line_dddashed: {
                  pen_style = Qt::PenStyle::DashDotDotLine;
                  break;
                }
              case msr_line_style_e::msr_line_dotted: {
                  pen_style = Qt::PenStyle::DotLine;
                  break;
                }
              }

            QColor pen_color =
                ((i + 1 == m_cur_dist_rec_ui) && p_owner->m_selected) ?
                  QColor::fromRgbF(1, 0, 0) :
                  QColor::fromRgbF(bond_color[0], bond_color[1], bond_color[2]);
            QPen linepen_inline(
                QPen(pen_color, record.m_line_size.get_value(), pen_style, Qt::RoundCap)
                );

            QPen linepen_inline2(
                  QPen(pen_color, record.m_line_size.get_value(), Qt::PenStyle::SolidLine,
                     Qt::RoundCap));

            painter.setPen(linepen_inline);
            painter.drawLine(linef);

            double angle =
                180 * std::atan2(linef.y2() - linef.y1(), linef.x2() - linef.x1()) / qpp::pi;

            angle = angle + std::ceil(-angle / 360) * 360;
            if (angle > 90 && angle < 270) angle = angle + 180;

            /* draw line terminators
             we now at (mid[0] + record.m_delta_offset[0], mid[1] + record.m_delta_offset[1]) */

            painter.setPen(linepen_inline2);
            painter.translate(mid[0], mid[1]);
            painter.rotate(angle);

            if (record.m_pair_term_style.get_value() == msr_pair_term_style_first ||
                record.m_pair_term_style.get_value() == msr_pair_term_style_both)
              painter.drawLine(-(line_len)*0.5, pair_term_width,
                               -(line_len)*0.5, -pair_term_width);

            if (record.m_pair_term_style.get_value() == msr_pair_term_style_second ||
                record.m_pair_term_style.get_value() == msr_pair_term_style_both)
              painter.drawLine( (line_len)*0.505, pair_term_width,
                                (line_len)*0.505, -pair_term_width);

            /* end draw line terminators */

            painter.resetTransform();
            painter.translate(mid[0] + delta_offset[0], mid[1] + delta_offset[1]);
            painter.rotate(angle + record.m_delta_angle.get_value());

            QString _label_text =
                record.m_show_custom_label.get_value() ?
                  QString::fromStdString(record.m_custom_label_text.get_value()) :
                  QString("%1 Å").arg(QString::number(
                                        dist,
                                        'f',
                                        astate->m_spatial_measurements_digits_count));

            QFontMetrics fmetric(font);

            //13 = 70
            //26 = 140 ?
            const float rect_w = fmetric.boundingRect(_label_text).width() * 1.2;
            const float rect_h = fmetric.boundingRect(_label_text).height() * 1.2;

            if (record.m_show_label.get_value())
              switch (record.m_label_render_style.get_value()) {

                case msr_label_style_e::msr_label_std : {
                    QRect text_rect(-rect_w*0.5f, rect_h*0.5f, rect_w, rect_h);
                    painter.setPen(rectpen);
                    painter.drawText(text_rect, Qt::AlignCenter, _label_text);
                    break;
                  }

                case msr_label_style_e::msr_label_border : {
                    QPainterPath path;
                    QRect text_rect(-rect_w*0.5f, rect_h*0.5f, rect_w, rect_h);
                    path.addRoundedRect(text_rect, 10, 10);
                    painter.fillPath(path, Qt::white);
                    painter.setPen(rectpen);
                    painter.drawPath(path);
                    painter.drawText(text_rect, Qt::AlignCenter, _label_text);
                    break;
                  }

                case msr_label_style_e::msr_label_outline : {
                    QPainterPath text_path;
                    QRect text_rect(-rect_w*0.5f, rect_h*0.5f, rect_w, rect_h);
                    painter.setPen(rectpen2);
                    painter.setBrush(text_fill_color);
                    text_path.addText(text_rect.left(), text_rect.bottom(), font, _label_text);
                    painter.drawPath(text_path);
                    break;
                  }

                }

            painter.resetTransform();

          }

      }


  painter.resetTransform();
  painter.setFont(QFont(astate->m_font_name, 13));

  if (m_render_angle.get_value())
    for (size_t q = 0; q < m_angle_recs.size(); q++) {

        auto &record = m_angle_recs[q];

        if (record.m_show.get_value()) {

            if (record.m_at1 >= p_owner->m_geom->nat()
                || record.m_at2 >= p_owner->m_geom->nat()
                || record.m_at3 >= p_owner->m_geom->nat())
              continue;

            std::optional<vector2<float> > l_f, l_s, l_t;

            l_f = astate->camera->project(
                    p_owner->m_pos.get_value() + p_owner->m_geom->pos(record.m_at1,record.m_idx1));

            l_s = astate->camera->project(
                    p_owner->m_pos.get_value() + p_owner->m_geom->pos(record.m_at2,record.m_idx2));

            l_t = astate->camera->project(
                    p_owner->m_pos.get_value() + p_owner->m_geom->pos(record.m_at3,record.m_idx3));


            if (l_f && l_s && l_t) {

                vector3<float> dir_f_s =
                    p_owner->m_pos.get_value() + p_owner->m_geom->pos(record.m_at1,record.m_idx1) -
                    p_owner->m_geom->pos(record.m_at2,record.m_idx2);

                vector3<float> dir_t_s =
                    p_owner->m_pos.get_value() + p_owner->m_geom->pos(record.m_at3,record.m_idx3) -
                    p_owner->m_geom->pos(record.m_at2,record.m_idx2);

                auto l1 = dir_f_s.normalized();
                auto l2 = dir_t_s.normalized();
                auto angle_color = record.m_angle_color.get_value();

                float real_angle = std::acos(l1.dot(l2)) * 180 / float(pi);

                QLineF line_f_s(0, 0, round((*l_f)[0]-(*l_s)[0]), round((*l_f)[1]-(*l_s)[1]));
                QLineF line_t_s(0, 0, round((*l_t)[0]-(*l_s)[0]), round((*l_t)[1]-(*l_s)[1]));

                float line_len = std::min(line_f_s.length(), line_t_s.length()) * 0.2f;
                int angle1 =
                    (std::atan2((*l_f)[1]- (*l_s)[1], (*l_f)[0] - (*l_s)[0])
                    * 180 / float(pi)) ;
                int angle2 =
                    (std::atan2((*l_t)[1]- (*l_s)[1], (*l_t)[0] - (*l_s)[0])
                    * 180 / float(pi)) ;

                angle1 -= 360. * std::floor((angle1) * (1. / 360.));
                angle2 -= 360. * std::floor((angle2) * (1. / 360.));

                angle1 = 360 - angle1;
                angle2 = 360 - angle2;

                float fangle1 = std::min(angle1, angle2);
                float fangle2 = std::max(angle1, angle2);

                float fangle_delta{fangle2 - fangle1};
                float inv_angle_delta = (360-fangle2)+fangle1;
                if (fangle_delta > inv_angle_delta) {
                    fangle_delta = -inv_angle_delta;
                  }

                QColor pen_color =
                    ((q + 1 == m_cur_angle_rec_ui) && p_owner->m_selected) ?
                      QColor::fromRgbF(1, 0, 0) :
                      QColor::fromRgbF(angle_color[0], angle_color[1], angle_color[2]);
                QPen linepen_inline(
                      QPen(pen_color, 2, Qt::PenStyle::SolidLine, Qt::RoundCap));

                painter.setPen(linepen_inline);

                painter.translate((*l_s)[0], (*l_s)[1]);

                painter.drawLine(line_f_s);
                painter.drawLine(line_t_s);

                for (int i = 1; i <= record.m_order.get_value(); i++) {
                    float new_r = line_len + line_len * i / 3.0;
                    painter.drawArc(-new_r, -new_r, 2*new_r, 2*new_r,
                                    (fangle1) * 16, fangle_delta * 16);
                  }

                float text_r = line_len + line_len * (record.m_order.get_value() + 2) / 3.0;

                vector2<float> nd = (((*l_f) + (*l_t) - 2*(*l_s))).normalized();
                nd *= text_r;

                const float text_rect_size = 70;

                const QChar degreeChar(0260);

                painter.drawText(nd[0] - text_rect_size * 0.5f,
                    nd[1] - text_rect_size * 0.5f,
                    text_rect_size,
                    text_rect_size,
                    Qt::AlignCenter,
                    QString("%1%2").arg(QString::number(real_angle, 'f', 2)).arg(degreeChar));

                painter.resetTransform();
              }

          }

      }

  painter.resetTransform();

}

void geom_view_msr_subsys_t::notify_atom_has_been_deleted(const uint32_t atm) {

  for (auto it = m_dist_recs.begin(); it != m_dist_recs.end(); ) {
      if ((*it).m_at1 == atm || (*it).m_at2 == atm)
        m_dist_recs.erase(it);
      else
        ++it;
    }

  for (auto it = m_angle_recs.begin(); it != m_angle_recs.end(); ) {
      if ((*it).m_at1 == atm || (*it).m_at2 == atm || (*it).m_at3 == atm)
        m_angle_recs.erase(it);
      else
        ++it;
    }

}

void geom_view_msr_subsys_t::dist_sel_atoms(size_t msr_id) {

  if (msr_id < m_dist_recs.size() && p_owner) {
      p_owner->sel_atoms(false);
      p_owner->sel_atom(m_dist_recs[msr_id].m_at1, m_dist_recs[msr_id].m_idx1);
      p_owner->sel_atom(m_dist_recs[msr_id].m_at2, m_dist_recs[msr_id].m_idx2);
    }

}

void geom_view_msr_subsys_t::dist_select_selected_atoms() {
  dist_sel_atoms(m_cur_dist_rec_ui - 1);
}

void geom_view_msr_subsys_t::dist_copy(size_t msr_id) {

  app_state_t* astate = app_state_t::get_inst();

  if (msr_id < m_dist_recs.size() && p_owner)
    for (size_t i = 0; i < m_dist_recs.size(); i++)
      if (i != msr_id) {
          m_dist_recs[i].m_bond_color = m_dist_recs[msr_id].m_bond_color;
          m_dist_recs[i].m_font_size = m_dist_recs[msr_id].m_font_size;
          m_dist_recs[i].m_label_render_style = m_dist_recs[msr_id].m_label_render_style;
          m_dist_recs[i].m_line_render_style = m_dist_recs[msr_id].m_line_render_style;
          m_dist_recs[i].m_line_size = m_dist_recs[msr_id].m_line_size;
          m_dist_recs[i].m_show = m_dist_recs[msr_id].m_show;
          m_dist_recs[i].m_show_label = m_dist_recs[msr_id].m_show_label;
          m_dist_recs[i].m_show_custom_label = m_dist_recs[msr_id].m_show_custom_label;
          m_dist_recs[i].m_custom_label_text = m_dist_recs[msr_id].m_custom_label_text;
        }

  astate->make_viewport_dirty();

}

void geom_view_msr_subsys_t::dist_copy_selected() {
  dist_copy(m_cur_dist_rec_ui - 1);
}

void geom_view_msr_subsys_t::dist_delete_selected() {

  app_state_t* astate = app_state_t::get_inst();
  rm_bond_msr(m_cur_dist_rec_ui - 1);
  astate->make_viewport_dirty();

}

void geom_view_msr_subsys_t::save_to_json(json &data) {

  json msr_object;

  json msr_dists = json::array({});

  for (auto &rec : m_dist_recs) {

      json msr_dist_inst;
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_AT1] = rec.m_at1;
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_AT2] = rec.m_at2;
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_SHOW] = rec.m_show.get_value();
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_SH_LBL] = rec.m_show_label.get_value();

      json_helper::hs_save_vec3(JSON_GEOM_VIEW_MSR_DIST_COLOR, rec.m_bond_color, msr_dist_inst);

      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_LSIZE] = rec.m_line_size.get_value();
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_FSIZE] = rec.m_font_size.get_value();
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_LSTYLE] = rec.m_line_render_style.get_value();
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_FSTYLE] = rec.m_label_render_style.get_value();
      msr_dist_inst[JSON_GEOM_VIEW_MSR_SHOW_CL] = rec.m_show_custom_label.get_value();
      msr_dist_inst[JSON_GEOM_VIEW_MSR_CL_TEXT] = rec.m_custom_label_text.get_value();
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_DANGLE] = rec.m_delta_angle.get_value();
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_TRM_ST] = rec.m_pair_term_style.get_value();
      msr_dist_inst[JSON_GEOM_VIEW_MSR_DIST_TRM_WD] = rec.m_pair_term_width.get_value();

      json_helper::hs_save_vec3(JSON_GEOM_VIEW_MSR_DIST_DOFFSET, rec.m_delta_offset, msr_dist_inst);

      if (p_owner->m_geom->DIM != 0) {
          json_helper::save_index(JSON_GEOM_VIEW_MSR_DIST_IDX1, rec.m_idx1, msr_dist_inst);
          json_helper::save_index(JSON_GEOM_VIEW_MSR_DIST_IDX2, rec.m_idx2, msr_dist_inst);
        }

      msr_dists.push_back(msr_dist_inst);

    }

  json msr_angles = json::array({});

  for (auto &rec : m_angle_recs) {

      json msr_angle_inst;
      msr_angle_inst[JSON_GEOM_VIEW_MSR_ANGLE_AT1] = rec.m_at1;
      msr_angle_inst[JSON_GEOM_VIEW_MSR_ANGLE_AT2] = rec.m_at2;
      msr_angle_inst[JSON_GEOM_VIEW_MSR_ANGLE_AT3] = rec.m_at3;
      msr_angle_inst[JSON_GEOM_VIEW_MSR_ANGLE_SHOW] = rec.m_show.get_value();
      msr_angle_inst[JSON_GEOM_VIEW_MSR_ANGLE_ORDER] = rec.m_order.get_value();

      if (p_owner->m_geom->DIM != 0) {
          json_helper::save_index(JSON_GEOM_VIEW_MSR_ANGLE_IDX1, rec.m_idx1, msr_angle_inst);
          json_helper::save_index(JSON_GEOM_VIEW_MSR_ANGLE_IDX2, rec.m_idx2, msr_angle_inst);
          json_helper::save_index(JSON_GEOM_VIEW_MSR_ANGLE_IDX3, rec.m_idx3, msr_angle_inst);
        }

      msr_angles.push_back(msr_angle_inst);

    }

  msr_object[JSON_GEOM_VIEW_MSR_DIST] = msr_dists;
  msr_object[JSON_GEOM_VIEW_MSR_ANGLE] = msr_angles;
  msr_object[JSON_GEOM_VIEW_MSR_SHOW_DIST] = m_render_dist.get_value();
  msr_object[JSON_GEOM_VIEW_MSR_SHOW_ANGLE] = m_render_angle.get_value();
  msr_object[JSON_GEOM_VIEW_MSR_SHOW_LDIST] = m_render_dist_legend.get_value();
  msr_object[JSON_GEOM_VIEW_MSR_SHOW_LANGLE] = m_render_angle_legend.get_value();

  data[JSON_GEOM_VIEW_MSR] = msr_object;

}

void geom_view_msr_subsys_t::load_from_json(json &data) {

  auto msr_object = data.find(JSON_GEOM_VIEW_MSR);
  if (msr_object == data.end()) return;

  json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_SHOW_DIST, m_render_dist, *msr_object);
  json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_SHOW_ANGLE, m_render_angle, *msr_object);
  json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_SHOW_LDIST, m_render_dist_legend, *msr_object);
  json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_SHOW_LANGLE, m_render_angle_legend, *msr_object);

  auto msr_dist = msr_object.value().find(JSON_GEOM_VIEW_MSR_DIST);
  if (msr_dist != msr_object.value().end())
    for (auto &msr_record : msr_dist.value()) {

        size_t at1 = msr_record[JSON_GEOM_VIEW_MSR_DIST_AT1];
        size_t at2 = msr_record[JSON_GEOM_VIEW_MSR_DIST_AT2];
        index idx1 = index::D(p_owner->m_geom->DIM).all(0);
        index idx2 = index::D(p_owner->m_geom->DIM).all(0);

        if (p_owner->m_geom->DIM != 0) {
            idx1 = json_helper::load_index(JSON_GEOM_VIEW_MSR_DIST_IDX1, msr_record);
            idx2 = json_helper::load_index(JSON_GEOM_VIEW_MSR_DIST_IDX2, msr_record);
          }

        add_bond_msr(at1, at2, idx1, idx2);

        auto &last_msr = m_dist_recs.back();

        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_DIST_SHOW, last_msr.m_show, msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_DIST_SH_LBL, last_msr.m_show_label, msr_record);
        json_helper::hs_load_vec3(JSON_GEOM_VIEW_MSR_DIST_COLOR, last_msr.m_bond_color, msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_DIST_LSIZE, last_msr.m_line_size, msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_DIST_FSIZE, last_msr.m_font_size, msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_DIST_LSTYLE, last_msr.m_line_render_style,
                              msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_DIST_FSTYLE, last_msr.m_label_render_style,
                              msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_SHOW_CL, last_msr.m_show_custom_label,
                              msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_CL_TEXT, last_msr.m_custom_label_text,
                              msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_DIST_DANGLE, last_msr.m_delta_angle,
                                 msr_record);
        json_helper::hs_load_vec3(JSON_GEOM_VIEW_MSR_DIST_DOFFSET, last_msr.m_delta_offset,
                               msr_record);

        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_DIST_TRM_ST, last_msr.m_pair_term_style,
                              msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_DIST_TRM_WD, last_msr.m_pair_term_width,
                              msr_record);

      }

  auto msr_angle = msr_object.value().find(JSON_GEOM_VIEW_MSR_ANGLE);
  if (msr_angle != msr_object.value().end())
    for (auto &msr_record : msr_angle.value()) {

        size_t at1 = msr_record[JSON_GEOM_VIEW_MSR_ANGLE_AT1];
        size_t at2 = msr_record[JSON_GEOM_VIEW_MSR_ANGLE_AT2];
        size_t at3 = msr_record[JSON_GEOM_VIEW_MSR_ANGLE_AT3];
        index idx1 = index::D(p_owner->m_geom->DIM).all(0);
        index idx2 = index::D(p_owner->m_geom->DIM).all(0);
        index idx3 = index::D(p_owner->m_geom->DIM).all(0);

        if (p_owner->m_geom->DIM != 0) {
            idx1 = json_helper::load_index(JSON_GEOM_VIEW_MSR_ANGLE_IDX1, msr_record);
            idx2 = json_helper::load_index(JSON_GEOM_VIEW_MSR_ANGLE_IDX2, msr_record);
            idx3 = json_helper::load_index(JSON_GEOM_VIEW_MSR_ANGLE_IDX3, msr_record);
          }

        add_angle_msr(at1, at2, at3, idx1, idx2, idx3);

        auto &last_msr = m_angle_recs.back();
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_ANGLE_SHOW, last_msr.m_show, msr_record);
        json_helper::hs_load_var(JSON_GEOM_VIEW_MSR_ANGLE_ORDER, last_msr.m_order, msr_record);

      }

}
