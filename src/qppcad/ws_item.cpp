#include <qppcad/ws_item.hpp>
#include <qppcad/app.hpp>
#include <data/color.hpp>
#include <qppcad/workspace.hpp>

using namespace qpp;

ws_item_t::ws_item_t(workspace_t* parent){
  m_pos         = vector3<float>(0.0f, 0.0f, 0.0f);
  m_scale       = vector3<float>(1.0f, 1.0f, 1.0f);
  m_rotation    = vector3<float>(0.0f, 0.0f, 0.0f);
  m_selected    = false;
  parent_ws     = parent;
  m_is_visible  = true;
  m_draw_cell   = true;
}

void ws_item_t::set_parent_workspace(workspace_t *_parent_ws){
  parent_ws = _parent_ws;
}

const std::string ws_item_t::get_name(){

}

void ws_item_t::set_name(const std::string _name){
  if (m_name != _name){
      m_name = _name;
      parent_ws->workspace_changed();
    }
}

void ws_item_t::set_name(const char *_name){
  if (m_name != _name){
      m_name = std::string(_name);
      parent_ws->workspace_changed();
    }
}

void ws_item_t::render(){
  app_state_c = &(c_app::get_state());
  if (app_state_c->dp != nullptr){
      if (m_selected && support_selection() && !support_rendering_bounding_box()){
          app_state_c->dp->begin_render_aabb();
          if (parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM)
            app_state_c->dp->render_aabb(clr_fuchsia, m_pos + m_aabb.min , m_pos + m_aabb.max  );
          else
            app_state_c->dp->render_aabb_segmented(clr_olive, m_pos + m_aabb.min , m_pos + m_aabb.max);
          app_state_c->dp->end_render_aabb();
        }
    }

}

void ws_item_t::render_ui(){
  ImGui::Spacing();
  if (ImGui::CollapsingHeader("General properties", ImGuiTreeNodeFlags_DefaultOpen)){
      char * s_item_name = new char[60];
      strcpy(s_item_name, m_name.c_str());
      ImGui::InputText("Item name", s_item_name, 60);
      ImGui::Checkbox("Draw workspace item", &m_is_visible);
      ImGui::Spacing();
      if (m_name != s_item_name) set_name(s_item_name);
      delete[] s_item_name;
    }
}


void ws_item_t::update(float delta_time){

}

float ws_item_t::get_bb_prescaller(){
  return 1.0f;
}

void ws_item_t::on_begin_node_gizmo_translate(){
  m_pos_old = m_pos;
  c_app::log(fmt::format("Start of translation of node [{}], pos = {}", m_name, m_pos.to_string_vec()));
}

void ws_item_t::on_end_node_gizmo_translate(){
  c_app::log(fmt::format("End of translation of node [{}], pos = {}", m_name, m_pos.to_string_vec()));
}
