#ifndef QPP_WS_ATOM_LIST_H
#define QPP_WS_ATOM_LIST_H

#include <qppcad/qppcad.hpp>
#include <geom/xgeom.hpp>
#include <geom/geom_anim.hpp>
#include <geom/extents_observer.hpp>
#include <geom/tws_tree.hpp>
#include <data/color.hpp>
#include <symm/index_set.hpp>
#include <io/comp_chem_data.hpp>
#include <qppcad/ws_item.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/file_formats.hpp>
#include <qppcad/ws_atoms_list_anim_subsys.hpp>
#include <qppcad/ws_atoms_list_measurement_subsys.hpp>
//#include <qppcad/ws_atoms_list_labels_subsys.hpp>
#include <qppcad/ws_atoms_list_lattice_planes_subsys.hpp>
#include <qppcad/ws_atoms_list_render_buffered_billboards.hpp>

namespace qpp {

  namespace cad {

    enum ws_atoms_list_render_type {
      ball_and_stick,
      dynamic_lines,
      xatom_lines,
      billboards,
      buffered_billboards
    };

    enum ws_atoms_list_color_mode {
      color_from_ptable,
      color_from_anim
    };

    const int ws_atoms_list_xgeom_ccr = 8;
    const int ws_atoms_list_xgeom_ccg = 9;
    const int ws_atoms_list_xgeom_ccb = 10;

    /// \brief The ws_atom_list_t class
    class ws_atoms_list_t : public ws_item_t {

      public:

        std::unique_ptr<xgeometry<float, periodic_cell<float> > >                    m_geom{};
        std::unique_ptr<ws_atoms_list_anim_subsys_t>                                 m_anim{};
        std::unique_ptr<ws_atoms_list_measurement_subsys_t>                          m_measure{};
        //std::unique_ptr<ws_atoms_list_labels_subsys_t>                               m_labels{};
        std::unique_ptr<ws_atoms_list_lat_planes_subsys_t>                           m_lat_planes{};
        std::unique_ptr<bonding_table<float> >                                       m_bt{};
        std::unique_ptr<tws_tree_t<float, periodic_cell<float> > >                   m_tws_tr{};
        std::unique_ptr<extents_observer_t<float, periodic_cell<float> > >           m_ext_obs{};
        std::set<uint16_t>                                                           m_atom_sel;
        std::unordered_set<atom_index_set_key, atom_index_set_key_hash>              m_atom_idx_sel;
        std::unique_ptr<ws_atoms_list_render_buffered_billboards_t> m_bs{};
        std::set<uint16_t>  m_atom_type_to_hide;

        float m_shading_specular_power{12.0f};
        float m_atom_scale_factor{0.3f};
        float m_bond_scale_factor{0.09f};
        vector3<float> m_cell_color{0.1f, 0.1f, 0.1f};
        vector3<float> m_gizmo_barycenter;
        vector3<float> m_new_atom_pos;

        ws_atoms_list_render_type m_cur_render_type{ws_atoms_list_render_type::ball_and_stick};
        vector3<int> m_subcells_range{1,1,1};

        ws_atoms_list_color_mode m_color_mode{ws_atoms_list_color_mode::color_from_ptable};
        bool m_show_imaginary_atoms{true};
        bool m_show_imaginary_bonds{true};
        bool m_draw_specular{true};
        bool m_show_bonds{true};
        bool m_show_atoms{true};
        bool m_draw_line_in_dist_measurement{false};
        bool m_bonding_table_show_disabled_record{true};
        bool m_has_animations;
        bool m_draw_subcells{false};

        ws_atoms_list_t();

        void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                   vector3<float> &vOutLookAt) override ;

        /// \brief geometry_changed
        void geometry_changed();
        void render() override;

//        void render_ui() override;
//        void render_overlay() override;
//        void render_work_panel_ui() override;
//        void td_context_menu_edit_item() override;
//        void td_context_menu_edit_content() override;

        bool mouse_click(ray_t<float> *click_ray) override;

        void select_atoms(bool all);
        bool select_atom(int atom_id);
        void select_by_type(const int item_type_to_select);
        void invert_selected_atoms();
        void insert_atom(const int atom_type, const vector3<float> &pos);
        void insert_atom(const std::string &atom_name, const vector3<float> &pos);
        void update_atom(const int at_id, const vector3<float> &pos);
        void update_atom(const int at_id, const std::string &at_name);
        void translate_selected(const vector3<float> &t_vec);
        void delete_selected_atoms();
        void make_super_cell(const int a_steps = 1, const int b_steps = 1, const int c_steps = 1);
        void apply_axial_scale(const float scale_a, const float scale_b, const float scale_c);
        void move_selected_atoms_to_home(bool ignore_selection = false);

        std::string compose_item_name() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;
        bool is_bb_visible() override;

        uint32_t get_amount_of_selected_content() override;
        size_t get_content_count() override;

        void on_begin_content_gizmo_translate() override;
        void apply_intermediate_translate_content(const vector3<float> &new_pos) override;
        void on_end_content_gizmo_translate() override;
        void recalc_gizmo_barycenter();
        const vector3<float> get_gizmo_content_barycenter() override;

        /// \brief shift
        /// \param vShift
        void shift(const vector3<float> shift);

        /// \brief load_from_file
        /// \param eFileFormat
        /// \param sFileName
        /// \param bAutoCenter
        void load_from_file (qc_file_fmt file_format, std::string file_name,
                            bool auto_center = false);
        void save_to_file (qc_file_fmt file_format, std::string file_name);

        std::string get_ws_item_class_name() override ;
        void write_to_json(json &data) override;
        void read_from_json(json &data) override;

        void dialog_save_to_file(qc_file_fmt file_format);

    };

  }

}

#endif
