#ifndef QPPCAD_ARROW_ARRAY
#define QPPCAD_ARROW_ARRAY
#include <qppcad/core/qppcad.hpp>
#include <data/color.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

  namespace cad {

    class geom_view_t;

    class arrow_array_t final: public ws_item_t {

        QPP_OBJECT(arrow_array_t, ws_item_t)

      public:

        geom_view_t *m_binded_gv{nullptr};
        std::shared_ptr<ws_item_t> m_src{nullptr};

        hs_prop_float_t m_unf_arrow_len{};
        hs_prop_float_t m_unf_arrow_cap_len;
        hs_prop_float_t m_unf_arrow_scale;
        hs_prop_float_t m_unf_arrow_cap_scale;
        hs_prop_float_t m_displ_eps;
        hs_prop_bool_t m_affected_by_sv;
        hs_prop_v3f_t m_color;

        arrow_array_t();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_type_descr() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;
        void updated_externally(uint32_t update_reason) override;
        uint32_t get_num_cnt_selected() override;
        size_t content_count() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data, repair_connection_info_t &rep_info) override;
        bool can_be_written_to_json() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
