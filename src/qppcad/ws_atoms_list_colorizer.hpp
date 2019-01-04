#ifndef QPP_CAD_WS_ATOMS_LIST_COLORIZER
#define QPP_CAD_WS_ATOMS_LIST_COLORIZER

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_atoms_list.hpp>

namespace qpp {

  namespace cad {

    class ws_atoms_list_colorizer_helper {
      public:

        static void colorize_by_distance(ws_atoms_list_t* al, float min_dist,
                                         vector3<float> min_dist_color,
                                         vector3<float> over_dist_color);

        static void py_colorize_by_distance(float min_dist,
                                            vector3<float> min_dist_color,
                                            vector3<float> over_dist_color);

    };

  }

}
#endif
