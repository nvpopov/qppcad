#ifndef QPP_CAD_WS_VOLUME_DATA
#define QPP_CAD_WS_VOLUME_DATA

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item.hpp>
#include <io/cube.hpp>
#include <qppcad/mesh.hpp>

namespace qpp {

  namespace cad {
    enum ws_volume_t  : int {
      volume_mo,
      volume_density
    };

    class ws_volume_data_t : public ws_item_t {

        QPP_OBJECT(ws_volume_data_t, ws_item_t)

      public:

        scalar_volume_t<float> m_volume;
        mesh_t *m_first_mesh{nullptr};
        mesh_t *m_second_mesh{nullptr};

        bool m_ready_to_render{false};
        bool m_need_to_regenerate{false};
        float m_alpha{0.75f};
        float m_isolevel{qpp::def_isovalue_dens};
        ws_volume_t m_volume_type;

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

        void save_to_json(json &data) override;
        void load_from_json(json &data) override;

        void update_isolevel(float new_isolevel);

        void load_from_stream(std::basic_istream<CHAR,TRAITS> & inp,
                              geometry<float, periodic_cell<float> > &geom,
                              std::string &fname);

    };

  }

}

#endif
