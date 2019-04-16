#ifndef QPPCAD_WS_POINT_SYM_GROUP
#define QPPCAD_WS_POINT_SYM_GROUP
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <geom/xgeom.hpp>
#include <symm/point_groups.hpp>

namespace qpp {

  namespace cad {

    const auto AXIS_COLORIZE_SIZE = 10;

    struct transform_record_t {

        vector3<float> m_axis;
        float m_phi;
        bool m_roto_inv{false};
        bool m_is_plane;
        bool m_is_visible{true};
        int m_order{1};
        matrix4<float> m_arrow_body_mat;
        matrix4<float> m_arrow_cap_mat;
        vector3<float> m_ri_cube_center;
        vector3<float> m_color{1,1,1};

    };

    class psg_view_t : public ws_item_t {

        QPP_OBJECT(psg_view_t, ws_item_t)

      public:

        std::shared_ptr<array_group<matrix3<float> > > m_ag{nullptr};
        point_group_axes<float> m_pg_axes;
        std::vector<transform_record_t> m_atf;

        float m_plane_alpha{0.64f};
        bool m_plane_alpha_enabled{false};
        float m_plane_scale{6.0f};
        vector3<float> m_plane_color{0, 0.9f, 0};

        float m_arrow_len{9.0f};
        float m_arrow_cap_len{0.51f};
        float m_arrow_scale{0.15f};
        float m_arrow_cap_scale{0.26f};

        std::array<vector3<float>, AXIS_COLORIZE_SIZE> m_axes_color_by_order;

        int m_current_repr;
        bool m_show_planes{true};
        bool m_show_axes{true};

        psg_view_t();

        void gen_from_geom(xgeometry<float, periodic_cell<float> > &geom,
                           float tolerance,
                           bool nested);

        void recalc_render_data();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_type_descr() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;
        void updated_externally(uint32_t update_reason) override;
        void on_leader_call() override ;

        uint32_t get_amount_of_selected_content() override;
        size_t get_content_count() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data, repair_connection_info_t &rep_info) override;

        void regenerate_atf();
        void update_view();
        void update_axes_color();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
