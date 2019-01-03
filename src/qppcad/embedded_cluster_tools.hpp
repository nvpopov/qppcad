#ifndef QPP_CAD_EMBEDDED_CLUSTER_TOOLS
#define QPP_CAD_EMBEDDED_CLUSTER_TOOLS

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_atoms_list.hpp>

namespace qpp {

  namespace cad {

    class embedded_cluster_tools {

      public:
        static void gen_spherical_cluster(std::shared_ptr<ws_atoms_list_t> uc,
                                          vector3<float> displ,
                                          float cluster_r = 30,
                                          float cls_r = 12,
                                          bool generate_qm = false,
                                          float qm_r = 5,
                                          bool do_legacy = true);

        static void gen_spherical_cluster_cur(vector3<float> displ,
                                          float cluster_r = 30,
                                          float cls_r = 12, bool do_legacy = true);

        static void gen_spherical_cluster_cur_qm(vector3<float> displ,
                                          float cluster_r = 30,
                                          float cls_r = 12,
                                          float qm_r = 2, bool do_legacy = true);

        static vector3<float> calc_dipole_moment();

    };

  }

}

#endif
