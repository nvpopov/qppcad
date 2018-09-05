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
  class ws_atom_list_t : public ws_item_t {
    public:

      bool need_to_rebuild_nbt;

      ///
      /// \brief display_imaginary_atoms
      ///
      bool show_imaginary_atoms;
      bool show_imaginary_bonds;
      bool show_bonds;
      bool show_atoms;

      xgeometry<float, periodic_cell<float> > *geom;
      bonding_table<float> *bt;
      neighbours_table<float> *nt;
      tws_tree<float, periodic_cell<float> > *tws_tr;
      extents_observer<float, periodic_cell<float> > *ext_obs;
      std::unordered_set<uint16_t> atom_selection;

      vector3<float> gizmo_barycenter;

      ws_atom_list_t(workspace_t* parent);

      void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                 vector3<float> &vOutLookAt) override ;
      void geometry_changed();
      void render() override;
      void render_atom(const uint16_t atNum, const index &atIndex);
      void render_bond(const uint16_t atNum1, const index &atIndex1,
                       const uint16_t atNum2, const index &atIndex2);

      void render_ui() override;
      bool mouse_click(ray<float> *click_ray) override;

      bool support_translation() override;
      bool support_rotation() override;
      bool support_scaling() override;
      bool support_content_editing() override;
      bool support_selection() override;
      bool support_rendering_bounding_box() override;
      std::string compose_item_name() override;
      void update(float delta_time) override;
      float get_bb_prescaller() override;

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

      ~ws_atom_list_t() override {

      }

  };

}

#endif
