#ifndef QPPCAD_WS_CUBE_PRIMITIVE
#define QPPCAD_WS_CUBE_PRIMITIVE

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

  namespace cad {

    enum ws_cube_rendering_mode : int {
      render_solid,
      render_lines
    };

    class cube_primitive_t : public ws_item_t {

         QPP_OBJECT(cube_primitive_t, ws_item_t)

      public:

        ws_cube_rendering_mode m_render_mode{render_solid};
        vector3<float> m_scale{1, 1, 1};
        vector3<float> m_color{1, 0, 0};
        float m_alpha{0.9f};
        bool m_alpha_enabled{false};

        cube_primitive_t();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_type_descr() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;
        void updated_externally(uint32_t update_reason) override;
        uint32_t get_num_cnt_selected() override;
        size_t get_content_count() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data, repair_connection_info_t &rep_info) override;

        bool can_be_written_to_json() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
