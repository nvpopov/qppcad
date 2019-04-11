#ifndef QPP_CAD_PGF_PRODUCER
#define QPP_CAD_PGF_PRODUCER

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <geom/xgeom.hpp>
#include <symm/gen_cell.hpp>

namespace qpp {

  namespace cad {

    class geom_view_t;
    class psg_view_t;

    class pgf_producer_t : public ws_item_t {

        QPP_OBJECT(pgf_producer_t, ws_item_t)

      public:

        std::shared_ptr<ws_item_t> m_src{nullptr};
        std::shared_ptr<ws_item_t> m_dst{nullptr};
        std::shared_ptr<ws_item_t> m_ag{nullptr};

        /**
         * @brief m_orders_range
         * [0] - start
         * [1] - end
         * [2] - max
         * min = 0
         */
        std::vector<vector3<int> > m_orders_range{};

        geom_view_t *m_src_gv{nullptr};
        geom_view_t *m_dst_gv{nullptr};
        psg_view_t *m_psg{nullptr};

        xgeometry<float, gen_cell<float, qpp::matrix3<float> > > m_imd{0};

        pgf_producer_t();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_type_descr() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;
        void updated_internally(uint32_t update_reason) override;
        uint32_t get_amount_of_selected_content() override;
        size_t get_content_count() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data) override;

        void generate_geom();
        bool check_consistency();
        void compose_from_array_group();

    };

  }

}

#endif
