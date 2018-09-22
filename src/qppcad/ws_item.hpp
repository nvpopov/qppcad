#ifndef QPP_WORKSPACE_ITEM_H
#define QPP_WORKSPACE_ITEM_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/aabb.hpp>
#include <geom/ray.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stl.h>

namespace qpp::cad{

  class workspace_t;
  class app_state_t;

  ///
  /// \brief The ws_item_t class
  ///
//  struct behaviour_disable{
//      static constexpr bool value  = false;
//      static constexpr bool custom = false;
//  };

//  struct behaviour_enable{
//      static constexpr bool value  = true;
//      static constexpr bool custom = false;
//  };

//  struct behaviour_custom{
//      static constexpr bool value  = true;
//      static constexpr bool custom = true;
//  };

//  template<typename support_translation_behaviour = behaviour_enable,
//           typename support_rotation_behaviour    = behaviour_disable,
//           typename support_scale_behaviour       = behaviour_disable>
  class ws_item_t  {
    protected:
      shared_ptr<workspace_t> parent_ws{};
    public:
      app_state_t *app_state_c;
      std::string      m_name;
      aabb_3d_t<float> m_aabb;
      vector3<float>   m_pos_old;
      vector3<float>   m_pos; ///
      vector3<float>   m_scale; ///
      vector3<float>   m_rotation; ///
      vector3<float>   explicit_translation;
      bool             m_is_visible; ///
      bool             m_draw_cell; ///
      bool             m_selected; ///
      bool             m_hide_gizmo_trigger;
      ///
      /// \brief ws_item_t
      /// \param parent
      ///
      ws_item_t();

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
      void set_parent_workspace(const shared_ptr<workspace_t> _parent_ws);

      ///
      /// \brief get_name
      /// \return
      ///
      const std::string get_name();

      ///
      /// \brief set_name
      /// \param _name
      ///
      void set_name(const std::string &_name);

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
      virtual void td_context_menu_edit_item();
      virtual void td_context_menu_edit_content();
      ///
      /// \brief mouse_click
      /// \param ray
      /// \return
      ///
      virtual bool mouse_click(ray_t<float> *ray) = 0;

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
      /// \brief get_amount_of_selected_content
      /// \return
      ///
      virtual uint32_t get_amount_of_selected_content() = 0;

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

      virtual void on_begin_node_gizmo_translate();
      virtual void on_end_node_gizmo_translate();
      virtual void on_begin_content_gizmo_translate() = 0;
      virtual void apply_intermediate_translate_content(const vector3<float> &new_pos) = 0;
      virtual void on_end_content_gizmo_translate() = 0;
  };

}

#endif
