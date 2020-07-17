#ifndef QPPCAD_WS_ATOMS_LIST_LABELS_S
#define QPPCAD_WS_ATOMS_LIST_LABELS_S

#include <geom/geom.hpp>
#include <vector>
#include <algorithm>
#include <QPainter>
#include <QPainterPath>
#include <qppcad/ws_item/geom_view/geom_view_labels_style.hpp>
#include <qppcad/ws_item/ws_item_subsystem.hpp>

namespace qpp {

  namespace cad {

    class geom_view_t;

    class geom_view_labels_subsys_t: public ws_item_subsystem_t {

      QPP_OBJECT(geom_view_labels_subsys_t, ws_item_subsystem_t)

      public:

        geom_view_t* p_owner;
        std::map<std::tuple<std::string, size_t>, QPainterPath> m_pp_cache;

        hs_prop_int_t m_lbl_font_size;
        hs_prop_int_t m_style;
        hs_prop_bool_t m_render_inplace_hud;
        hs_prop_v2f_t m_inplace_offset;
        hs_prop_bool_t m_selective_lbl;
        hs_prop_bool_t m_screen_scale;
        hs_prop_bool_t m_render_outlines;
        hs_prop_bool_t m_render_axis_labels;
        hs_prop_float_t m_outline_size;

        explicit geom_view_labels_subsys_t (geom_view_t &_p_owner);

        void render_overlay(QPainter &painter);
        void render_labels(QPainter &painter);
        void render_in_place_overlay(QPainter &painter);

        void labelize_sel_by_neighbours_count();
        void labelize_sel_by_dist_factor();

        static std::string label_gen_fn(geom_view_t *owner,
                                        geom_labels_style_e lbl_style,
                                        size_t atom_id);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
