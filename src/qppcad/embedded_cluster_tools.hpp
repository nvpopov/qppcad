#ifndef QPP_CAD_EMBEDDED_CLUSTER_TOOLS
#define QPP_CAD_EMBEDDED_CLUSTER_TOOLS

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_atoms_list.hpp>

namespace qpp {

  namespace cad {

    class embedded_cluster_tools {

      public:
        static void gen_spherical_cluster(ws_atoms_list_t *uc,
                                          vector3<float> displ,
                                          float cluster_r = 30,
                                          float cls_r = 12);

        static void gen_spherical_cluster_cur(vector3<float> displ,
                                          float cluster_r = 30,
                                          float cls_r = 12);

    };

  }

}

#endif
