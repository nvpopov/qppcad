#ifndef QPPCAD_GEOM_VIEW_TOOLS
#define QPPCAD_GEOM_VIEW_TOOLS

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

  namespace cad {

    struct geom_view_tools_t {

        static void shake_atoms(geom_view_t *gv, std::set<size_t> atoms_to_shake, float magn);
        static void py_shake_atoms(geom_view_t *gv, py::list atoms_to_shake, float magn);

        static void purify_boundary_atoms(geom_view_t *dst, geom_view_t *src);

        static vector3<float> dipole_moment(geom_view_t *gv);
        static float total_charge(geom_view_t *gv);
        static std::vector<scalar_partition_per_type_t<> > get_charge_partition(geom_view_t *gv);
        static py::list py_get_charge_partition(geom_view_t *gv);

        static void pertrub_via_normal_mode(geom_view_t *gv,
                                            generic_array_t<vector3<float>, float> &disp);
        static std::string pretty_print_selected_atoms(geom_view_t *gv, vector3<float> new_frame);

        static void flip_atom_in_cell(geom_view_t *gv,
                                      size_t at_id,
                                      size_t dim_id,
                                      float flip_magn = 1,
                                      bool rebuild_tree = false);
        static void flip_sel_atoms_in_cell(geom_view_t *gv, size_t dim_id, float flip_magn = 1);
        static void align_atoms_to_point(geom_view_t *gv, vector3<float> fpoint);

    };

  } // namespace cad

} // namespace qpp

#endif
