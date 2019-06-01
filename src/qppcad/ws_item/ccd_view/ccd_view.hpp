#ifndef QPPCAD_WS_COMP_CHEM_DATA
#define QPPCAD_WS_COMP_CHEM_DATA

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <io/comp_chem_data.hpp>

namespace qpp {

  namespace cad {

    class ccd_view_t : public ws_item_t {

       QPP_OBJECT(ccd_view_t, ws_item_t)

      public:

        std::unique_ptr<comp_chem_program_data_t<float> > m_ccd{nullptr};
        std::vector<size_t> m_connected_items_stride;

        int m_cur_step{0};
        int m_cur_vib{-1};
        ccd_view_t();

        void manual_step_update(const int dir);
        void manual_update_vib();
        void fill_custom_colors_of_geom_anim(const std::string color_map_name);
        void update_connected_items();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        void update_joined_atoms_list_animation(size_t step_idx);
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_type_descr() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;
        void updated_externally(uint32_t update_reason) override;
        uint32_t get_num_cnt_selected() override;
        size_t get_content_count() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data, repair_connection_info_t &rep_info) override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
