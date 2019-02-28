#ifndef QPP_CAD_WS_VOLUME_DATA
#define QPP_CAD_WS_VOLUME_DATA

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <io/cube.hpp>
#include <qppcad/render/mesh.hpp>
#include <data/color.hpp>

namespace qpp {

  namespace cad {

    enum ws_volume_t  : int {
      volume_mo,
      volume_density
    };

    class ws_volume_record_t {

      public:

        scalar_volume_t<float> m_volume;
        mesh_t *m_first_mesh{nullptr};
        mesh_t *m_second_mesh{nullptr};

        //std::string m_volume_name; move to scalar_volume_t::m_volume name

        bool m_ready_to_render{false};
        bool m_need_to_regenerate{false};
        bool m_transparent_volume{false};
        bool m_render_permanent{false};
        float m_alpha{0.75f};
        float m_isolevel{qpp::def_isovalue_dens};

        vector3<float> m_color_pos{clr_red};
        vector3<float> m_color_neg{clr_navy};
        vector3<float> m_color_vol{clr_yellow};

        ws_volume_t m_volume_type;

    };

    class volume_view_t : public ws_item_t {

        QPP_OBJECT(volume_view_t, ws_item_t)

      public:

        std::vector<ws_volume_record_t> m_volumes;
        int m_current_volume{0};

        void gen_repr();
        void mc_polygonise(float _isolevel);

        volume_view_t();

        void vote_for_view_vectors(vector3<float> &v_out_look_pos,
                                   vector3<float> &v_out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_type_descr() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;

        uint32_t get_amount_of_selected_content() override;
        size_t get_content_count() override;
        void updated_internally() override;

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
