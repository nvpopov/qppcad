#ifndef QPP_WS_ATOM_LIST_H
#define QPP_WS_ATOM_LIST_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/geom.hpp>
#include <geom/xgeom.hpp>
#include <geom/ngbr.hpp>
#include <geom/extents_observer.hpp>
#include <geom/tws_tree.hpp>
#include <data/color.hpp>
#include <qppcad/ws_item.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/file_formats.hpp>


namespace qpp{

  ///
  /// \brief The ws_atom_list_t class
  ///
  class ws_atoms_list_t : public ws_item_t {
    public:

      bool need_to_rebuild_nbt;

      ///
      /// \brief display_imaginary_atoms
      ///
      bool show_imaginary_atoms;
      bool show_imaginary_bonds;
      bool show_bonds;
      bool show_atoms;

      unique_ptr<xgeometry<float, periodic_cell<float> > >          geom;
      unique_ptr<bonding_table<float> >                             bt;
      unique_ptr<neighbours_table<float> >                          nt;
      unique_ptr<tws_tree_t<float, periodic_cell<float> > >         tws_tr;
      unique_ptr<extents_observer_t<float, periodic_cell<float> > > ext_obs;
      unordered_set<uint16_t>                     atom_selection;

      vector3<float> gizmo_barycenter;

      ws_atoms_list_t(workspace_t* parent);

      void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                 vector3<float> &vOutLookAt) override ;
      ///
      /// \brief geometry_changed
      ///
      void geometry_changed();
      void render() override;

      ///
      /// \brief render_atom
      /// \param atNum
      /// \param atIndex
      ///
      void render_atom(const uint16_t atNum, const index &atIndex);

      ///
      /// \brief render_bond
      /// \param atNum1
      /// \param atIndex1
      /// \param atNum2
      /// \param atIndex2
      ///
      void render_bond(const uint16_t atNum1, const index &atIndex1,
                       const uint16_t atNum2, const index &atIndex2);

      void render_ui() override;
      bool mouse_click(ray_t<float> *click_ray) override;


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

      ///
      void on_begin_content_gizmo_translate() override;
      void apply_intermediate_translate_content(const vector3<float> &new_pos) override;
      void on_end_content_gizmo_translate() override;
      void recalc_gizmo_barycenter();
      const vector3<float> get_gizmo_content_barycenter() override;

      ///
      /// \brief shift
      /// \param vShift
      ///
      void shift(const vector3<float> vShift);

      ///
      /// \brief load_from_file
      /// \param eFileFormat
      /// \param sFileName
      /// \param bAutoCenter
      ///
      void load_from_file(qc_file_format eFileFormat, std::string sFileName,
                          bool bAutoCenter = false);

      void rebuild_ngbt();

      ~ws_atoms_list_t() override {

      }

  };

}

#endif
