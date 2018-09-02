#include <qppcad/ws_item.hpp>
#include <qppcad/app.hpp>
#include <data/color.hpp>
#include <qppcad/workspace.hpp>

using namespace qpp;

ws_item_t::ws_item_t(workspace_t* parent){
  pos = vector3<float>(0.0f, 0.0f, 0.0f);
  scale    = vector3<float>(1.0f, 1.0f, 1.0f);
  rotation = vector3<float>(0.0f, 0.0f, 0.0f);
  bSelected = false;
  parent_ws = parent;

  b_show = true;
  b_draw_cell = true;
}

void ws_item_t::set_parent_workspace(workspace_t *_parent_ws){
  parent_ws = _parent_ws;
}

const std::string ws_item_t::get_name(){

}

void ws_item_t::set_name(const std::string _name){
  if (name != _name){
      name = _name;
      parent_ws->workspace_changed();
    }
}

void ws_item_t::set_name(const char *_name){
  if (name != _name){
      name = std::string(_name);
      parent_ws->workspace_changed();
    }
}

void ws_item_t::render(){
  app_state_c = &(c_app::get_state());
  if (app_state_c->dp != nullptr){
      if (bSelected && support_selection() && !support_rendering_bounding_box()){
          app_state_c->dp->begin_render_aabb();
          if (parent_ws->cur_edit_type == ws_edit_type::EDIT_WS_ITEM)
            app_state_c->dp->render_aabb(clr_fuchsia,
                                    pos + aabb.min ,
                                    pos + aabb.max  );
          else app_state_c->dp->render_aabb_segmented(clr_olive,
                                              pos + aabb.min ,
                                              pos + aabb.max  );
          app_state_c->dp->end_render_aabb();
        }
    }

}

void ws_item_t::render_ui(){
  ImGui::Text("General properties:");
  ImGui::Spacing();
  char * s_item_name = new char[60];
  strcpy(s_item_name, name.c_str());
  ImGui::InputText("Item name", s_item_name, 60);
  ImGui::Checkbox("Draw workspace item", &b_show);
  if (name != s_item_name) set_name(s_item_name);
  delete[] s_item_name;
}


void ws_item_t::update(float delta_time){

}

float ws_item_t::get_bb_prescaller(){
  return 1.0f;
}
