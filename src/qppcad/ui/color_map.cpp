#include <qppcad/ui/color_map.hpp>

using namespace qpp;
using namespace qpp::cad;

vector3<float> qpp::cad::color_map_t::get_color(float pos) {

  float _pos = std::clamp(pos, 0.0f, 1.0f);
  vector3<float> retval{0.0f, 0.0f, 0.0f};

  size_t idx_1 = 0, idx_2 = 1;

  // in [0,n) :
  for (size_t i = 0; i < m_colors.size() - 1; i++)
    if (m_colors[i].m_pos >= _pos-0.05f && m_colors[i].m_pos < _pos+0.05f) {
        idx_1 = i;
        idx_2 = i+1;
        break;
      }

  return m_colors[idx_2].m_color * _pos + m_colors[idx_1].m_color * (1 - _pos);

}

void color_map_t::push_color(float _pos, vector3<float> _color) {
  m_colors.emplace_back(color_map_record_t{_pos, _color});
}

color_map_t::color_map_t(std::initializer_list<color_map_record_t> color_list) {
  m_colors = color_list;
}
