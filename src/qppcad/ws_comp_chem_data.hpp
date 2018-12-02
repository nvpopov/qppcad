#ifndef QPP_CAD_WS_COMP_CHEM_DATA
#define QPP_CAD_WS_COMP_CHEM_DATA

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item.hpp>
#include <io/comp_chem_data.hpp>

namespace qpp {

  namespace cad {

    class ws_comp_chem_data_t : public ws_item_t {

      public:
        std::unique_ptr<comp_chem_program_data_t<float> > m_ccd{nullptr};
        std::vector<std::shared_ptr<ws_item_t> > m_connected_items; // todo: need to change to weak
        std::vector<size_t> m_connected_items_stride;

        int m_cur_step{0};
        int m_cur_vib{-1};
        ws_comp_chem_data_t();

        void manual_step_update(const int dir);
        void manual_update_vib();
        void fill_custom_colors_of_geom_anim(const std::string color_map_name);

        void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                   vector3<float> &vOutLookAt) override ;
        void render() override;
        void update_joined_atoms_list_animation(size_t step_idx);
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
