#include <qppcad/ws_item/geom_view/geom_view_measurement_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/app_state.hpp>

namespace qpp {

  namespace cad {

    float geom_view_measurement_subsys_t::dist (const size_t idx) {
      vector3<float> l_s, l_e;
      l_s = p_owner->m_pos +
            p_owner->m_geom->pos(m_dist_recs[idx].m_at1,m_dist_recs[idx].m_idx1);
      l_e = p_owner->m_pos +
            p_owner->m_geom->pos(m_dist_recs[idx].m_at2,m_dist_recs[idx].m_idx2);
      return (l_e - l_s).norm();
    }

    void geom_view_measurement_subsys_t::add_bond_measurement (const uint32_t atm1,
                                                               const uint32_t atm2,
                                                               const index idx1,
                                                               const index idx2) {

      if (!is_bond_measurement_exist(atm1, atm2, idx1, idx2)) {
          m_dist_recs.emplace_back(atm1, atm2, idx1, idx2);
          app_state_t* astate = app_state_t::get_inst();
          astate->astate_evd->cur_ws_selected_item_measurements_changed();
        }
    }

    void geom_view_measurement_subsys_t::add_angle_measurement(
        const uint32_t atm1,
        const uint32_t atm2,
        const uint32_t atm3,
        const index idx1,
        const index idx2,
        const index idx3) {

      if (!is_angle_measurement_exist(atm1, atm2, atm3, idx1, idx2, idx3)) {
          m_angle_recs.emplace_back(atm1, atm2, atm3, idx1, idx2, idx3);
          app_state_t* astate = app_state_t::get_inst();
          astate->astate_evd->cur_ws_selected_item_measurements_changed();
        }
    }

    geom_view_measurement_subsys_t::geom_view_measurement_subsys_t(
        geom_view_t &_p_owner) {
      p_owner = &_p_owner;
    }

    void geom_view_measurement_subsys_t::remove_bond_measurement (const size_t measure_idx) {

      if (measure_idx < m_dist_recs.size())
        m_dist_recs.erase(m_dist_recs.begin() + measure_idx);

      app_state_t* astate = app_state_t::get_inst();
      astate->astate_evd->cur_ws_selected_item_measurements_changed();

    }

    void geom_view_measurement_subsys_t::remove_angle_measurement(const size_t measure_idx) {

      if (measure_idx < m_angle_recs.size())
        m_angle_recs.erase(m_angle_recs.begin() + measure_idx);

      app_state_t* astate = app_state_t::get_inst();
      astate->astate_evd->cur_ws_selected_item_measurements_changed();

    }


    std::optional<size_t> geom_view_measurement_subsys_t::is_bond_measurement_exist (
        const uint32_t atm1,
        const uint32_t atm2,
        const index idx1,
        const index idx2) {

      for (auto i = 0; i < m_dist_recs.size(); i++)
        if ((m_dist_recs[i].m_at1 == atm1 && m_dist_recs[i].m_at2 == atm2 &&
             m_dist_recs[i].m_idx1 == idx1 && m_dist_recs[i].m_idx2 == idx2) ||
            (m_dist_recs[i].m_at1 == atm2 && m_dist_recs[i].m_at2 == atm1 &&
             m_dist_recs[i].m_idx1 == idx2 && m_dist_recs[i].m_idx2 == idx1))
          return std::optional<size_t>(i);

      return std::nullopt;

    }

    std::optional<size_t> geom_view_measurement_subsys_t::is_angle_measurement_exist(
        const uint32_t atm1,
        const uint32_t atm2,
        const uint32_t atm3,
        const index idx1,
        const index idx2,
        const index idx3) {

      for (auto i = 0; i < m_angle_recs.size(); i++)
        if (m_angle_recs[i].m_at1 == atm1 &&
            m_angle_recs[i].m_at2 == atm2 &&
            m_angle_recs[i].m_at3 == atm3 &&
            m_angle_recs[i].m_idx1 == idx1 &&
            m_angle_recs[i].m_idx2 == idx2 &&
            m_angle_recs[i].m_idx3 == idx3)
          return std::optional<size_t>(i);

      return std::nullopt;

    }

    void geom_view_measurement_subsys_t::render() {
      //render bond measurements
      //deprecated
    }

    void geom_view_measurement_subsys_t::render_overlay(QPainter &painter) {

      app_state_t* astate = app_state_t::get_inst();

      std::optional<vector2<float> > l_s, l_e;
      QPen linepen(QPen(Qt::black, 4, Qt::DotLine, Qt::RoundCap));
      QPen linepen_2(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));

      QPen rectpen(QPen(Qt::black, 3, Qt::SolidLine));
      painter.setFont(QFont(astate->m_font_name, 13));
      painter.resetTransform();

      for (auto &record : m_dist_recs)

        if (record.m_show) {

            if (record.m_at1 >= p_owner->m_geom->nat() || record.m_at2 >= p_owner->m_geom->nat()) {
                continue;
              }

            l_s = astate->camera->project(
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at1,record.m_idx1));

            l_e = astate->camera->project(
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at2,record.m_idx2));

            if (!l_s || !l_e) continue;

            float dist = (p_owner->m_geom->pos(record.m_at1,record.m_idx1) -
                          p_owner->m_geom->pos(record.m_at2,record.m_idx2)).norm();

            vector2<float> mid = (*l_s + *l_e) * 0.5f;
            const float rect_w = 70;
            const float rect_h = 30;

            QLineF linef(round((*l_s)[0]) + 0.5, round((*l_s)[1]) + 0.5,
                round((*l_e)[0]) + 0.5, round((*l_e)[1]) + 0.5);
            painter.setPen(linepen);
            painter.drawLine(linef);

            double angle = 180 * std::atan2(linef.y2()-linef.y1(), linef.x2()-linef.x1()) / qpp::pi;

            angle = angle + std::ceil( -angle / 360 ) * 360;
            if (angle > 90 && angle < 270) angle = angle + 180;

            painter.translate(mid[0], mid[1]);
            painter.rotate(angle);
            QPainterPath path;
            QRect text_rect(-rect_w*0.5f, rect_h*0.5f, rect_w, rect_h);
            path.addRoundedRect(text_rect, 10, 10);
            painter.fillPath(path, Qt::white);
            painter.setPen(rectpen);
            painter.drawPath(path);
            painter.drawText(text_rect,
                             Qt::AlignCenter,
                             QString("%1 Å").arg(QString::number(dist, 'f', 2)));
            painter.resetTransform();
          }

      painter.resetTransform();

      for (auto &record : m_angle_recs)
        if (record.m_show) {

            if (record.m_at1 >= p_owner->m_geom->nat() ||
                record.m_at2 >= p_owner->m_geom->nat() ||
                record.m_at3 >= p_owner->m_geom->nat())
              continue;

            std::optional<vector2<float> > l_f, l_s, l_t;

            l_f = astate->camera->project(
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at1,record.m_idx1));

            l_s = astate->camera->project(
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at2,record.m_idx2));

            l_t = astate->camera->project(
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at3,record.m_idx3));


            if (l_f && l_s && l_t) {

                vector3<float> dir_f_s =
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at1,record.m_idx1) -
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at2,record.m_idx2);

                vector3<float> dir_t_s =
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at3,record.m_idx3) -
                    p_owner->m_pos + p_owner->m_geom->pos(record.m_at2,record.m_idx2);

                float l1 = dir_f_s.norm();
                float l2 = dir_t_s.norm();
                float real_angle = 180 - std::acos(dir_f_s.dot(dir_t_s) / (l1 * l2)) * 180 / M_PI;

                QLineF line_f_s(0, 0, round((*l_f)[0]-(*l_s)[0]), round((*l_f)[1]-(*l_s)[1]));
                QLineF line_t_s(0, 0, round((*l_t)[0]-(*l_s)[0]), round((*l_t)[1]-(*l_s)[1]));

                float line_len = std::min(line_f_s.length(), line_t_s.length()) * 0.2f;
                int angle1 =
                    (std::atan2((*l_f)[1]- (*l_s)[1], (*l_f)[0] - (*l_s)[0]) * 180 / M_PI) ;
                int angle2 =
                    (std::atan2((*l_t)[1]- (*l_s)[1], (*l_t)[0] - (*l_s)[0]) * 180 / M_PI) ;

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

                painter.setPen(linepen_2);

                painter.translate((*l_s)[0], (*l_s)[1]);

                painter.drawLine(line_f_s);
                painter.drawLine(line_t_s);

                for (int i = 1; i <= record.m_order; i++) {
                    float new_r = line_len + line_len * i / 3.0;
                    painter.drawArc(-new_r, -new_r, 2*new_r, 2*new_r,
                                    (fangle1) * 16, fangle_delta * 16);
                  }

                float text_r = line_len + line_len * (record.m_order + 2) / 3.0;

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

      painter.resetTransform();

    }

    void geom_view_measurement_subsys_t::notify_atom_has_been_deleted(const uint32_t atm) {

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


  }

}
