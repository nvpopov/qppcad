#include <qppcad/ws_item.hpp>
#include <qppcad/app.hpp>
#include <data/color.hpp>
#include <qppcad/workspace.hpp>

using namespace qpp;

ws_item_t::ws_item_t(workspace_t* parent){
  vPosition = vector3<float>(0.0f, 0.0f, 0.0f);
  vScale    = vector3<float>(1.0f, 1.0f, 1.0f);
  vRotation = vector3<float>(0.0f, 0.0f, 0.0f);
  bSelected = false;
  parent_ws = parent;
}

void ws_item_t::set_parent_workspace(workspace_t *_parent_ws){
  parent_ws = _parent_ws;
}

void ws_item_t::render(){
  app_state_c = &(c_app::get_state());
  if (app_state_c->dp != nullptr){
      if (bSelected && support_selection() && !support_rendering_bounding_box()){
          app_state_c->dp->begin_render_aabb();
          if (parent_ws->cur_edit_type == ws_edit_type::EDIT_WS_ITEM)
            app_state_c->dp->render_aabb(clr_fuchsia,
                                    aabb.min ,
                                    aabb.max  );
          else app_state_c->dp->render_aabb_segmented(clr_olive,
                                              aabb.min ,
                                              aabb.max  );
          app_state_c->dp->end_render_aabb();
        }
    }

}

void ws_item_t::update(){

}

float ws_item_t::get_bb_prescaller(){
  return 1.0f;
}
