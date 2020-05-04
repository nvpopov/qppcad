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

    class cube_primitive_t final: public ws_item_t {

         QPP_OBJECT(cube_primitive_t, ws_item_t)

      public:

        hs_prop_int_t m_render_mode;
        hs_prop_v3f_t m_scale;
        hs_prop_v3f_t m_color;
        hs_prop_float_t m_alpha{};
        hs_prop_bool_t m_alpha_enabled{};

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
