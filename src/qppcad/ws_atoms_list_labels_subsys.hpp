#ifndef QPP_CAD_WS_ATOMS_LIST_LABELS_S
#define QPP_CAD_WS_ATOMS_LIST_LABELS_S

#include <geom/geom.hpp>
#include <vector>
#include <algorithm>
#include <QPainter>

namespace qpp {

  namespace cad {

    enum ws_atoms_list_labels_style : int {
      show_none,
      show_id,
      show_type,
      show_id_type,
      show_custom
    };

    class ws_atoms_list_t;

    class ws_atoms_list_labels_subsys_t {

      private:

        ws_atoms_list_t* p_owner;

      public:

        ws_atoms_list_labels_style m_style{ws_atoms_list_labels_style::show_none};

        ws_atoms_list_labels_subsys_t (ws_atoms_list_t &_p_owner);


        void render_overlay (QPainter &painter);

    };

  }

}

#endif
