#ifndef QPP_CAD_WS_ATOMS_LIST_LABELS_S
#define QPP_CAD_WS_ATOMS_LIST_LABELS_S

#include <geom/geom.hpp>
#include <vector>
#include <algorithm>
#include <QPainter>

namespace qpp {

  namespace cad {

    enum geom_labels_style_e : int {
      show_none,
      show_id,
      show_type,
      show_id_type,
      show_charge,
      show_custom
    };

    class geom_view_t;

    class geom_view_labels_subsys_t {

      private:

        geom_view_t* p_owner;

      public:

        int m_label_font_size{17};
        geom_labels_style_e m_style{geom_labels_style_e::show_none};
        bool m_render_inplace_hud{false};
        bool m_selective_label_render{false};
        bool m_screen_scale{false};
        bool m_render_outlines{false};
        geom_view_labels_subsys_t (geom_view_t &_p_owner);

        void render_overlay(QPainter &painter);
        void render_labels(QPainter &painter);
        void render_in_place_overlay(QPainter &painter);

    };

  }

}

#endif
