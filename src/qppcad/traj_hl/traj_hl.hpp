#ifndef QPP_CAD_WS_TRAJ_HIGHLIGHT
#define QPP_CAD_WS_TRAJ_HIGHLIGHT

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item.hpp>
#include <qppcad/render/mesh.hpp>

namespace qpp {

  namespace cad {

    class geom_view_t;

    class traj_hl_t : public ws_item_t {

        QPP_OBJECT(traj_hl_t, ws_item_t)

      public:
        geom_view_t *b_al;
        std::unique_ptr<mesh_t> m_line_mesh;
        bool m_need_to_rebuild{true};
        size_t m_anim_id{1};
        size_t m_atm_id{0};
        vector3<float> m_traj_color{1, 0, 0};
        traj_hl_t();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_item_name() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;
        void updated_internally() override;
        uint32_t get_amount_of_selected_content() override;
        size_t get_content_count() override;

        void on_leader_changed() override;
        void on_leader_call() override;

        void rebuild_line_mesh();

        void save_to_json(json &data) override;
        void load_from_json(json &data) override;

    };

  }

}

#endif
