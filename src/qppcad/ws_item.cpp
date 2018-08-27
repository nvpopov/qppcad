#include <qppcad/ws_item.hpp>
#include <qppcad/app.hpp>
#include <data/color.hpp>
#include <qppcad/workspace.hpp>

using namespace qpp;

ws_item::ws_item(workspace* parent){
  vPosition = vector3<float>(0.0f, 0.0f, 0.0f);
  vScale    = vector3<float>(1.0f, 1.0f, 1.0f);
  vRotation = vector3<float>(0.0f, 0.0f, 0.0f);
  bSelected = false;
  parent_ws = parent;
}

void ws_item::set_parent_workspace(workspace *_parent_ws){
  parent_ws = _parent_ws;
}

void ws_item::render(){
  app_state* astate = &(c_app::get_state());

  if (astate->dp != nullptr){
      if (bSelected && support_selection()){
          astate->dp->begin_render_aabb();
          if (parent_ws->cur_edit_type == ws_edit_type::EDIT_WS_ITEM)
            astate->dp->render_aabb(clr_fuchsia,
                                    aabb.min ,
                                    aabb.max  );
          else astate->dp->render_aabb_segmented(clr_olive,
                                              aabb.min ,
                                              aabb.max  );
          astate->dp->end_render_aabb();
        }
    }

}

void ws_item::update(){

}

float ws_item::get_bb_prescaller(){
  return 1.0f;
}
