#include <qppcad/ui/color_map.hpp>

using namespace qpp;
using namespace qpp::cad;

vector3<float> color_map_helper::get_by_threshold(const std::vector<vector3<float>> &cmp,
                                                  float thrd) {

  if (cmp.empty()) return {0, 0, 0};
  float clamped_threshold = std::clamp(thrd, 0.0f, 1.0f);
  return cmp[(size_t)(clamped_threshold * (cmp.size() - 1))];

}
