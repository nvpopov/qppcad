#ifndef QPP_CAD_WS_COMP_CHEM_DATA
#define QPP_CAD_WS_COMP_CHEM_DATA

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item.hpp>
#include <io/comp_chem_data.hpp>

namespace qpp {

  namespace cad {

    class ws_comp_chem_data_t : public ws_item_t {

      public:

        ws_comp_chem_data_t();

        void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                   vector3<float> &vOutLookAt) override ;
        void render() override;
        void render_overlay() override;
        void render_ui() override;
        void render_work_panel_ui() override;
        void td_context_menu_edit_item() override;
        void td_context_menu_edit_content() override;
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
