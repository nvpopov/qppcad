#include <qppcad/ws_atoms_list_measurement_subsys.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/app_state.hpp>

namespace qpp {

  namespace cad {

    float ws_atoms_list_measurement_subsys_t::dist (const size_t idx) {
      vector3<float> l_s, l_e;
      l_s = p_owner->m_pos +
            p_owner->m_geom->pos(m_dist_recs[idx].m_at1,m_dist_recs[idx].m_idx1);
      l_e = p_owner->m_pos +
            p_owner->m_geom->pos(m_dist_recs[idx].m_at2,m_dist_recs[idx].m_idx2);
      return (l_e - l_s).norm();
    }

    void ws_atoms_list_measurement_subsys_t::add_bond_measurement (const uint32_t atm1,
                                                                   const uint32_t atm2,
                                                                   const index idx1,
                                                                   const index idx2) {
      if (!is_bond_measurement_exist(atm1, atm2, idx1, idx2))
        m_dist_recs.emplace_back(atm1, atm2, idx1, idx2);
    }

    void ws_atoms_list_measurement_subsys_t::add_angle_measurement(
        const uint32_t atm1,
        const uint32_t atm2,
        const uint32_t atm3,
        const index idx1,
        const index idx2,
        const index idx3) {
      if (!is_angle_measurement_exist(atm1, atm2, atm3, idx1, idx2, idx3))
        m_angle_recs.emplace_back(atm1, atm2, atm3, idx1, idx2, idx3);
    }

    ws_atoms_list_measurement_subsys_t::ws_atoms_list_measurement_subsys_t(
        ws_atoms_list_t &_p_owner) {
      p_owner = &_p_owner;
    }

    void ws_atoms_list_measurement_subsys_t::remove_bond_measurement (const size_t measure_idx) {
      if (measure_idx < m_dist_recs.size())
        m_dist_recs.erase(m_dist_recs.begin() + measure_idx);
    }

    void ws_atoms_list_measurement_subsys_t::remove_angle_measurement(const size_t measure_idx) {
      if (measure_idx < m_angle_recs.size())
        m_angle_recs.erase(m_angle_recs.begin() + measure_idx);
    }


    std::optional<size_t> ws_atoms_list_measurement_subsys_t::is_bond_measurement_exist (
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

    std::optional<size_t> ws_atoms_list_measurement_subsys_t::is_angle_measurement_exist(
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

      for (auto &record : m_dist_recs)
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


  }

}
