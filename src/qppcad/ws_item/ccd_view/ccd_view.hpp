#ifndef QPPCAD_WS_COMP_CHEM_DATA
#define QPPCAD_WS_COMP_CHEM_DATA

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")
namespace py = pybind11;

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <io/comp_chem_data.hpp>

namespace qpp {

  namespace cad {

    enum ccd_copy_charges_mode : int {

      do_not_copy_charges,
      copy_mulliken,
      copy_mulliken_spin,
      copy_lowdin,
      copy_lowdin_spin

    };


    class geom_view_t;

    class ccd_view_t final: public ws_item_t {

       QPP_OBJECT(ccd_view_t, ws_item_t)

      public:

        std::shared_ptr<comp_chem_program_data_t<float> > m_ccd{nullptr};
        std::vector<size_t> m_connected_items_stride;

        int m_cur_step{0};
        int m_cur_vib{-1};
        ccd_copy_charges_mode m_copy_charges{ccd_copy_charges_mode::do_not_copy_charges};
        ccd_view_t();

        void manual_step_update(const int dir);
        void manual_update_vib();
        void fill_custom_colors_of_geom_anim(const std::string &color_map_name);
        void update_connected_items();
        void update_charges(geom_view_t *gv, size_t start_atom, size_t end_atom);

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

        void traverse_step_manual(int delta_step);
        void traverse_step_boundary(bool to_the_begin);

        std::vector<size_t> query_vib_by_atoms(std::vector<size_t> atoms, float gate);
        py::list py_query_vib_by_atoms(py::list atoms, float gate);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
