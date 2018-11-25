#ifndef QPP_CAD_WS_ATOMS_LIST_OBJ_INSP_H
#define QPP_CAD_WS_ATOMS_LIST_OBJ_INSP_H

#include <qppcad/ws_item_obj_insp_widget.hpp>

namespace qpp {

  namespace cad {

    class ws_atoms_list_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        ws_item_tab_widget_t *tab_display;
        ws_item_tab_widget_t *tab_animation;
        ws_item_tab_widget_t *tab_measurement;
        ws_atoms_list_obj_insp_widget_t();
    };

  }

}
#endif
