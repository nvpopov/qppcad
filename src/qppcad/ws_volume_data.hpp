#ifndef QPP_CAD_WS_VOLUME_DATA
#define QPP_CAD_WS_VOLUME_DATA

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item.hpp>
#include <io/cube.hpp>
#include <qppcad/mesh.hpp>

namespace qpp {

  namespace cad {

    class ws_volume_data_t : public ws_item_t {

      public:

        cube_header_t<float> m_ch;
        std::vector<float> m_cube_field;
        mesh_t *m_mesh{nullptr};
        bool m_ready_to_render{false};
        bool m_need_to_regenerate{false};
        float m_isolevel{qpp::def_isovalue_dens};

        void gen_repr();
        void mc_polygonise(float _isolevel);

        ws_volume_data_t();

        void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                   vector3<float> &vOutLookAt) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_item_name() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;

        uint32_t get_amount_of_selected_content() override;
        size_t get_content_count() override;

        std::string get_ws_item_class_name() override ;
        void write_to_json(json &data) override;
        void read_from_json(json &data) override;

    };

  }

}

#endif
