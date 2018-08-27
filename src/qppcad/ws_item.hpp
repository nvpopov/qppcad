#ifndef QPP_WORKSPACE_ITEM_H
#define QPP_WORKSPACE_ITEM_H

#include <geom/lace3d.hpp>
#include <geom/aabb.hpp>
#include <geom/ray.hpp>
#include <vector>
#include <iostream>


namespace qpp{
  class workspace;

  class ws_item {
    protected:
      workspace* parent_ws;
    public:
      std::string name;
      aabb_3d<float> aabb;
      vector3<float> vPosition;
      vector3<float> vScale;
      vector3<float> vRotation;
      bool bSelected;
      ws_item(workspace* parent);
      virtual void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                         vector3<float> &vOutLookAt) = 0;
      void set_parent_workspace(workspace* _parent_ws);

      virtual void render();
      virtual void render_ui() = 0;
      virtual bool mouse_click(ray<float> *ray) = 0;
      virtual void update();
      virtual bool support_translation() = 0;
      virtual bool support_rotation() = 0;
      virtual bool support_scaling() = 0;
      virtual bool support_content_editing() = 0;
      virtual bool support_selection() = 0;
      virtual bool support_rendering_bounding_box() = 0;

      virtual float get_bb_prescaller();
      virtual ~ws_item(){ }
  };

}

#endif
