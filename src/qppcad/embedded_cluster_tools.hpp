#ifndef QPPCAD_EMBEDDED_CLUSTER_TOOLS
#define QPPCAD_EMBEDDED_CLUSTER_TOOLS

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

  namespace cad {

    class embedded_cluster_tools {

      public:

        static void find_high_symmetry_qm_cluster(geom_view_t *uc,
                                                  vector3<float> sphere_center,
                                                  float qm_r_start_scan,
                                                  float qm_r_end_scan,
                                                  float symm_tolerance,
                                                  size_t total_steps);

        static void gen_spherical_cluster(geom_view_t* uc,
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

        static void set_qm_cluster_r(std::shared_ptr<geom_view_t> qm,
                                     std::shared_ptr<geom_view_t> cls,
                                     float new_r);

        static void move_sel_from_qm_to_cls(std::shared_ptr<geom_view_t> qm,
                                            std::shared_ptr<geom_view_t> cls);

        static void move_sel_from_qm_to_cls_cur();

        static void set_qm_cluster_r_cur(float new_r);

        static void deduce_embedding_context(std::shared_ptr<geom_view_t> &uc,
                                             std::shared_ptr<geom_view_t> &chg,
                                             std::shared_ptr<geom_view_t> &cls,
                                             std::shared_ptr<geom_view_t> &qm);

        static vector3<float> calc_dipole_moment();

        static void generate_molcas_embc_sp_input(std::string outdir);
        static void generate_orca_embc_sp_input(std::string outdir,
                                                std::vector<std::string> anion_list,
                                                bool merge_cls_and_chg = false);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
