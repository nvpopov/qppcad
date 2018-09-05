#ifndef QPP_WORKSPACE_ITEM_H
#define QPP_WORKSPACE_ITEM_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/aabb.hpp>
#include <geom/ray.hpp>

namespace qpp{

  class workspace_t;
  class app_state_t;

  ///
  /// \brief The ws_item_t class
  ///
  class ws_item_t {
    protected:
      workspace_t *parent_ws;
    public:
      app_state_t *app_state_c;
      std::string name;
      aabb_3d<float> aabb;

      vector3<float> pos; ///
      vector3<float> scale; ///
      vector3<float> rotation; ///
      bool b_show; ///
      bool b_draw_cell; ///
      bool is_selected; ///

      ///
      /// \brief ws_item_t
      /// \param parent
      ///
      ws_item_t(workspace_t* parent);

      ///
      /// \brief vote_for_view_vectors
      /// \param vOutLookPos
      /// \param vOutLookAt
      ///
      virtual void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                         vector3<float> &vOutLookAt) = 0;

      ///
      /// \brief set_parent_workspace
      /// \param _parent_ws
      ///
      void set_parent_workspace(workspace_t* _parent_ws);

      ///
      /// \brief get_name
      /// \return
      ///
      const std::string get_name();

      ///
      /// \brief set_name
      /// \param _name
      ///
      void set_name(const std::string _name);

      ///
      /// \brief set_name
      /// \param _name
      ///
      void set_name(const char * _name);

      ///
      /// \brief render
      ///
      virtual void render();

      ///
      /// \brief render_ui
      ///
      virtual void render_ui();

      ///
      /// \brief mouse_click
      /// \param ray
      /// \return
      ///
      virtual bool mouse_click(ray<float> *ray) = 0;

      ///
      /// \brief support_translation
      /// \return
      ///
      virtual bool support_translation() = 0;

      ///
      /// \brief support_rotation
      /// \return
      ///
      virtual bool support_rotation() = 0;

      ///
      /// \brief support_scaling
      /// \return
      ///
      virtual bool support_scaling() = 0;

      ///
      /// \brief support_content_editing
      /// \return
      ///
      virtual bool support_content_editing() = 0;

      ///
      /// \brief support_selection
      /// \return
      ///
      virtual bool support_selection() = 0;

      ///
      /// \brief support_rendering_bounding_box
      /// \return
      ///
      virtual bool support_rendering_bounding_box() = 0;

      ///
      /// \brief compose_item_name
      /// \return
      ///
      virtual std::string compose_item_name() = 0;

      ///
      /// \brief update
      /// \param delta_time
      ///
      virtual void update(float delta_time);

      ///
      /// \brief get_bb_prescaller
      /// \return
      ///
      virtual float get_bb_prescaller();

      ///
      /// \brief get_gizmo_content_barycenter
      /// \return
      ///
      virtual const vector3<float> get_gizmo_content_barycenter() = 0;

      ///
      /// \brief ~ws_item_t
      ///
      virtual ~ws_item_t(){ }
  };

}

#endif
