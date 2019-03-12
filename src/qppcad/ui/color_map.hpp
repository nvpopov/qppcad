#ifndef QPP_CAD_COLOR_MAP_H
#define QPP_CAD_COLOR_MAP_H

#include <data/color.hpp>
#include <qppcad/qppcad.hpp>
#include <vector>
#include <initializer_list>

namespace qpp {

  namespace cad {

    class color_map_record_t {

      public:

        float m_pos;
        vector3<float> m_color;
        color_map_record_t(const float _m_pos, const vector3<float> &_m_color)
          : m_pos(_m_pos), m_color(_m_color) {}
    };

    class color_map_t {

      public:

        std::vector<color_map_record_t> m_colors;

        vector3<float> get_color(float pos);
        void push_color(float _pos, vector3<float> _color);

        color_map_t(std::initializer_list<color_map_record_t> color_list);
        color_map_t() = default;
        bool operator <(const color_map_t &another_color_map) const {

          auto calc_color_map_hash = [](const color_map_t &inp) -> float {
            vector3<float> sum_all_colors(0.0f);
            for (auto &color_rec : inp.m_colors)
              sum_all_colors += color_rec.m_color * (color_rec.m_pos+0.5f);
            return sum_all_colors.norm();
          };

          return calc_color_map_hash(*this) < calc_color_map_hash(another_color_map);

        }

    };

  }

}


#endif
