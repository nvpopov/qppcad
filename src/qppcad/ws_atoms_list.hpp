#ifndef QPP_WS_ATOM_LIST_H
#define QPP_WS_ATOM_LIST_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/xgeom.hpp>
#include <geom/geom_anim.hpp>
#include <geom/ngbr.hpp>
#include <geom/extents_observer.hpp>
#include <geom/tws_tree.hpp>
#include <data/color.hpp>
#include <symm/index_set.hpp>
#include <qppcad/ws_item.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/file_formats.hpp>
#include <qppcad/ws_atoms_list_anim_subsys.hpp>

namespace qpp::cad {

  enum ws_atoms_list_render_type {
    ball_and_stick,
    dynamic_lines,
    xatom_lines,
    static_lines
  };

  /// \brief The ws_atom_list_t class
  class ws_atoms_list_t : public ws_item_t {

    public:

      unique_ptr<xgeometry<float, periodic_cell<float> > >             m_geom;
      unique_ptr<ws_atoms_list_anim_subsys_t<float, ws_atoms_list_t> > m_anim;
      unique_ptr<bonding_table<float> >                                m_bt;
      unique_ptr<tws_tree_t<float, periodic_cell<float> > >            m_tws_tr;
      unique_ptr<extents_observer_t<float, periodic_cell<float> > >    m_ext_obs;
      set<uint16_t>                                                    m_atom_sel;
      unordered_set<atom_index_set_key, atom_index_set_key_hash>       m_atom_idx_sel;

      set<uint16_t>  m_atom_type_to_hide;

      float m_shading_specular_power{12.0f};
      float m_atom_scale_factor{0.3f};
      float m_bond_scale_factor{0.09f};
      vector3<float> m_cell_color{0.1f, 0.1f, 0.1f};
      vector3<float> m_gizmo_barycenter;
      vector3<float> m_new_atom_pos;

      ws_atoms_list_render_type m_cur_render_type{ws_atoms_list_render_type::ball_and_stick};

      bool m_show_imaginary_atoms{true};
      bool m_show_imaginary_bonds{true};
      bool m_draw_specular{true};
      bool m_show_bonds{true};
      bool m_show_atoms{true};
      bool m_draw_line_in_dist_measurement{false};
      bool m_bonding_table_show_disabled_record{true};
      bool m_has_animations;


      ws_atoms_list_t();

      void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                 vector3<float> &vOutLookAt) override ;

      /// \brief geometry_changed
      void geometry_changed();
      void render() override;

      void render_ui() override;
      void td_context_menu_edit_item() override;
      void td_context_menu_edit_content() override;

      bool mouse_click(ray_t<float> *click_ray) override;

      void select_atoms(bool all);
      void invert_selected_atoms();
      void insert_atom(const int atom_type, const vector3<float> &pos);
      void insert_atom(const string &atom_name, const vector3<float> &pos);
      void update_atom(const int at_id, const vector3<float> &pos);
      void update_atom(const int at_id, const string &at_name);
      void delete_selected_atoms();

      bool support_translation() override;
      bool support_rotation() override;
      bool support_scaling() override;
      bool support_content_editing() override;
      bool support_selection() override;
      bool support_rendering_bounding_box() override;

      std::string compose_item_name() override;
      void update(float delta_time) override;
      float get_bb_prescaller() override;

      uint32_t get_amount_of_selected_content() override;


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
      void load_from_file(qc_file_format file_format, std::string file_name,
                          bool auto_center = false);

      string get_ws_item_class_name() override ;
      void write_to_json(json &data) override;
      void read_from_json(json &data) override;

      ~ws_atoms_list_t() override {

      }

  };

}

#endif
