#ifndef QPP_CAD_WS_VOLUME_DATA_OBJ_INSP_H
#define QPP_CAD_WS_VOLUME_DATA_OBJ_INSP_H

#include <qppcad/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_volume_data.hpp>

namespace qpp {

  namespace cad {

    class ws_volume_data_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
//        ws_item_tab_widget_t *tab_geo_opt;
//        ws_item_tab_widget_t *tab_vibs;
        ws_volume_data_obj_insp_widget_t();
    };

  }

}
#endif
