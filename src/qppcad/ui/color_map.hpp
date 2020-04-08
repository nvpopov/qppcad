#ifndef QPPCAD_COLOR_MAP_H
#define QPPCAD_COLOR_MAP_H

#include <data/color.hpp>
#include <qppcad/core/qppcad.hpp>
#include <qppcad/ui/color_map_rainbow.hpp>

namespace qpp {

  namespace cad {

  struct color_map_helper {
    static vector3<float> get_by_threshold(const std::vector<vector3<float>> &cmp, float thrd);
  };

  } // namespace qpp::cad

} // namespace qpp

#endif
