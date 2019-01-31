#ifndef QPP_CAD_WS_POINT_SYM_GROUP
#define QPP_CAD_WS_POINT_SYM_GROUP
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item.hpp>
#include <geom/xgeom.hpp>
#include <symm/point_groups.hpp>

namespace qpp {

  namespace cad {

    class transform_record_t {
      public:
        vector3<float> m_axis;
        float m_phi;
        bool m_inversion;
        bool m_is_plane;
        matrix4<float> m_render_mat;
        matrix4<float> m_render_mat_aux;
    };

    class ws_point_sym_group_t : public ws_item_t {

        QPP_OBJECT(ws_point_sym_group_t, ws_item_t)

      public:
        array_group<matrix3<float> > m_ag;
        //genform_group<matrix3<float> > m_gf_grp;
        point_group_axes<float> m_pg_axes;
        std::vector<transform_record_t> m_atf;
        float m_plane_alpha{0.8f};

        ws_point_sym_group_t();
        void gen_from_geom(xgeometry<float, periodic_cell<float> > &geom,
                           float tolerance);
        void recalc_render_data();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_item_name() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;

        uint32_t get_amount_of_selected_content() override;
        size_t get_content_count() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data) override;

    };

  }

}
#endif
