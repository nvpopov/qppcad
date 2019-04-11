#ifndef QPP_CAD_PGF_PRODUCER_OBJECT_INSP
#define QPP_CAD_PGF_PRODUCER_OBJECT_INSP

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/pgf_producer/pgf_producer.hpp>

namespace qpp {

  namespace cad {

    class pgf_producer_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:

        pgf_producer_t *b_pr{nullptr};
        ws_item_tab_widget_t *tab_cell_range;
        qspoiler_widget_t *gb_pgf_data;
        QFormLayout *gb_pgf_data_lt;
        qbinded_ws_item_combobox_t *pgf_data_src;
        qbinded_ws_item_combobox_t *pgf_data_dst;
        qbinded_ws_item_combobox_t *pgf_data_ag;

        qspoiler_widget_t *gb_gen_cell;
        QFormLayout *gb_gen_cell_lt;

        /**
         * @brief m_ci_ranges
         * cell indexes range
         */
        std::vector<std::shared_ptr<qbinded_int2b_input_t> > m_ci_ranges;

        int m_last_ci_ranges_size{-1};

        pgf_producer_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

        void update_cell_indexes_ranges();

    };

  }

}

#endif
