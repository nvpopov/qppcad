#ifndef QPPCAD_WS_ATOMS_LIST_LABELS_S
#define QPPCAD_WS_ATOMS_LIST_LABELS_S

#include <geom/geom.hpp>
#include <vector>
#include <algorithm>
#include <QPainter>
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

        int m_lbl_font_size{17};
        geom_labels_style_e m_style{geom_labels_style_e::show_none};
        bool m_render_inplace_hud{false};
        vector2<float> m_inplace_offset{0.5f,1};
        bool m_selective_lbl{false};
        bool m_screen_scale{false};
        bool m_render_outlines{true};
        bool m_render_axis_labels{true};
        float m_outline_size{1};
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
