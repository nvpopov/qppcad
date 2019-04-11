#ifndef QPP_CAD_WS_ARROW_PRIMITIVE
#define QPP_CAD_WS_ARROW_PRIMITIVE

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

  namespace cad {

    class arrow_primitive_t : public ws_item_t {

        QPP_OBJECT(arrow_primitive_t, ws_item_t)

      public:

        matrix4<float> m_model_body;
        matrix4<float> m_model_arrow;
        vector3<float> m_arrow_to{0};
        vector3<float> m_color{0};
        float m_axis_scale{1.0f};
        bool m_need_to_recalc_render_data{true};

        arrow_primitive_t();

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
        bool can_be_written_to_json() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data) override;

        void recalc_render_data();

    };

  }

}

#endif
