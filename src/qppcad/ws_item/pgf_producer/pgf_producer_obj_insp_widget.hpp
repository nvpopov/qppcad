#ifndef QPP_CAD_PGF_PRODUCER_OBJECT_INSP
#define QPP_CAD_PGF_PRODUCER_OBJECT_INSP

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/pgf_producer/pgf_producer.hpp>

namespace qpp {

  namespace cad {

    class pgf_producer_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:

        ws_item_tab_widget_t *tab_cell_range;
        QGroupBox *gb_pgf_data;
        QFormLayout *gb_pgf_data_lt;
        qbinded_ws_item_combobox *pgf_data_src;
        qbinded_ws_item_combobox *pgf_data_dst;
        qbinded_ws_item_combobox *pgf_data_ag;

        pgf_producer_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  }

}

#endif
