#ifndef QPPCAD_PYTHON_SQ_H
#define QPPCAD_PYTHON_SQ_H

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
//#include <pybind11/eigen.h>
#pragma pop_macro("slots")

namespace py = pybind11;

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <qppcad/workspace.hpp>

namespace qpp {

  namespace cad {

    class geom_view_t;

    struct cp2k_helper_t {

        static void save_neb_data(std::shared_ptr<ws_item_t> g_start,
                                  std::shared_ptr<ws_item_t> g_end,
                                  std::string path);

        static std::string gen_fixed_atoms_section(py::list &fixed_atoms);

    };

    struct orca_helper_t {

        static std::string gen_multijob_from_anim(std::shared_ptr<geom_view_t> gv, size_t anim_id);


    };

    struct generic_qc_helper_t {

        static void gen_pair_dist_calc_data(std::shared_ptr<geom_view_t> gv,
                                            size_t at1, size_t at2, size_t num_frames,
                                            float start_r, float end_r);

        static std::string get_raw_coords_section(std::shared_ptr<geom_view_t> gv);

    };



  } // namespace cad


} // namespace qpp

#endif
