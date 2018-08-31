#ifndef QPP_WORKSPACE_ITEM_H
#define QPP_WORKSPACE_ITEM_H

#include <geom/lace3d.hpp>
#include <geom/aabb.hpp>
#include <geom/ray.hpp>

#include <vector>
#include <iostream>


namespace qpp{
  class workspace_t;
  class app_state_t;

  class ws_item_t {
    protected:
      workspace_t* parent_ws;
    public:
      app_state_t *app_state_c;
      std::string name;
      aabb_3d<float> aabb;
      vector3<float> vPosition;
      vector3<float> vScale;
      vector3<float> vRotation;
      bool b_show;
      bool b_draw_cell;
      bool bSelected;
      ws_item_t(workspace_t* parent);
      virtual void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                         vector3<float> &vOutLookAt) = 0;
      void set_parent_workspace(workspace_t* _parent_ws);

      const std::string get_name();
      void set_name(const std::string _name);
      void set_name(const char * _name);
      virtual void render();
      virtual void render_ui();
      virtual bool mouse_click(ray<float> *ray) = 0;
      virtual void update();
      virtual bool support_translation() = 0;
      virtual bool support_rotation() = 0;
      virtual bool support_scaling() = 0;
      virtual bool support_content_editing() = 0;
      virtual bool support_selection() = 0;
      virtual bool support_rendering_bounding_box() = 0;
      virtual std::string compose_item_name() = 0;

      virtual float get_bb_prescaller();
      virtual ~ws_item_t(){ }
  };

}

#endif
