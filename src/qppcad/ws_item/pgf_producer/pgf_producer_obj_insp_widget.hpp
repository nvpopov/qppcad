#ifndef QPP_CAD_PGF_PRODUCER_OBJECT_INSP
#define QPP_CAD_PGF_PRODUCER_OBJECT_INSP

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/pgf_producer/pgf_producer.hpp>

namespace qpp {

  namespace cad {

    class pgf_producer_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:

        pgf_producer_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  }

}

#endif
