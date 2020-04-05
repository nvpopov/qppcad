#ifndef QPPCAD_WS_ATOMS_LIST_COLORIZER
#define QPPCAD_WS_ATOMS_LIST_COLORIZER

#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    class geom_view_t;

    class geom_view_colorizer_helper {

      public:

        /**
         * @brief changes color of atoms in geom_view_t according to distance
         *
         * @param al - geom_view_t src
         *
         * @param min_dist - distance for colorize
         *
         * @param min_dist_color - apply color at a distance lesser than min_dist
         *
         * @param over_dist_color - apply color at a distance greater than min_dist
         *
         * @param fill_def_color_on_miss - if true, then apply color from ptable at a distance
         * greater than min_dist
         *
         * @param affect_pairs
         *
         * @param atom_type1 - name of first atomic specie
         *
         * @param atom_type2 - name of second atomic specie
         *
         */
        static void colorize_by_distance(geom_view_t* al, float min_dist,
                                         vector3<float> min_dist_color,
                                         vector3<float> over_dist_color,
                                         bool fill_def_color_on_miss, bool affect_pairs,
                                         std::string atom_type1, std::string atom_type2);

        static void py_colorize_by_distance(float min_dist,
                                            vector3<float> min_dist_color,
                                            vector3<float> over_dist_color);

        static void py_colorize_by_distance_with_pairs(float min_dist,
                                                       vector3<float> min_dist_color,
                                                       vector3<float> over_dist_color,
                                                       std::string atom_type1,
                                                       std::string atom_type2);

        static void colorize_by_xfield(geom_view_t *gv, const vector3<float> color_low,
                                       const vector3<float> color_high, const size_t xfield_id);

        static void colorize_by_category(geom_view_t *gv, std::vector<size_t> &cat_data,
                                         std::vector<vector3<float> > &clr);

        static void colorize_by_sublattices(geom_view_t *gv, float eps = 0.1f);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
