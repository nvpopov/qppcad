#ifndef QPPCAD_PYTHON_SQ_H
#define QPPCAD_PYTHON_SQ_H

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
//#include <pybind11/eigen.h>
#pragma pop_macro("slots")

namespace py = pybind11;

#include <qppcad/core/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <qppcad/core/workspace.hpp>

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

        static std::string gen_multijob_from_anim(geom_view_t *gv, size_t anim_id);
        static std::string gen_coord_section(geom_view_t *gv,
                                             std::vector<std::string> &is_point_charge,
                                             std::vector<std::string> &basis_less_cnt,
                                             std::map<std::string, std::string> &ecp_name);

    };

    struct generic_qc_helper_t {

        static std::string get_raw_coords_section(std::shared_ptr<geom_view_t> gv);

    };



  } // namespace cad


} // namespace qpp

#endif
