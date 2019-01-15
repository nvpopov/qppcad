#ifndef QPP_CAD_EMBEDDED_CLUSTER_TOOLS
#define QPP_CAD_EMBEDDED_CLUSTER_TOOLS

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>

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

        static void set_qm_cluster_r(std::shared_ptr<ws_atoms_list_t> qm,
                                     std::shared_ptr<ws_atoms_list_t> cls,
                                     float new_r);

        static void move_sel_from_qm_to_cls(std::shared_ptr<ws_atoms_list_t> qm,
                                            std::shared_ptr<ws_atoms_list_t> cls);

        static void move_sel_from_qm_to_cls_cur();

        static void set_qm_cluster_r_cur(float new_r);

        static void deduce_embedding_context(std::shared_ptr<ws_atoms_list_t> &uc,
                                             std::shared_ptr<ws_atoms_list_t> &chg,
                                             std::shared_ptr<ws_atoms_list_t> &cls,
                                             std::shared_ptr<ws_atoms_list_t> &qm);

        static vector3<float> calc_dipole_moment();

    };

  }

}

#endif
