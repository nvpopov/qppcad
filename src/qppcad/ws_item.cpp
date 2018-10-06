#include <qppcad/ws_item.hpp>
#include <qppcad/app.hpp>
#include <data/color.hpp>
#include <qppcad/workspace.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_item_t::ws_item_t(){
  m_pos                = vector3<float>(0.0f, 0.0f, 0.0f);
  m_scale              = vector3<float>(1.0f, 1.0f, 1.0f);
  m_rotation           = vector3<float>(0.0f, 0.0f, 0.0f);
  m_selected           = false;
  m_is_visible         = true;
  m_draw_cell          = true;
  m_hide_gizmo_trigger = false;
}

void ws_item_t::set_parent_workspace(const std::shared_ptr<workspace_t> _parent_ws){
  parent_ws = _parent_ws;
}

const std::string ws_item_t::get_name(){
  return m_name;
}

void ws_item_t::set_name(const std::string &_name){
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

void ws_item_t::td_context_menu_edit_item(){
  //  if (support_translation())
  //    if (ImGui::BeginMenu("Translate Node")){

  //        ImGui::SliderFloat("X", &(explicit_translation[0]), -10.0, 10.0);
  //        ImGui::SameLine();
  //        ImGui::Button("Apply X");

  //        ImGui::SliderFloat("Y", &(explicit_translation[1]), -10.0, 10.0);
  //        ImGui::SameLine();
  //        ImGui::Button("Apply Y");

  //        ImGui::SliderFloat("Z", &(explicit_translation[2]), -10.0, 10.0);
  //        ImGui::SameLine();
  //        ImGui::Button("Apply Z");
  //        ImGui::EndMenu();
  //      }
}

void ws_item_t::td_context_menu_edit_content(){

}


void ws_item_t::update(float delta_time){

}

float ws_item_t::get_bb_prescaller(){
  return 1.0f;
}

void ws_item_t::on_begin_node_gizmo_translate(){
  m_pos_old = m_pos;
  c_app::log(fmt::format("Start of translation of node [{}], pos = {}",
                         m_name, m_pos.to_string_vec()));
}

void ws_item_t::on_end_node_gizmo_translate(){
  c_app::log(fmt::format("End of translation of node [{}], pos = {}",
                         m_name, m_pos.to_string_vec()));
}

void ws_item_t::write_to_json(json &data){
  data[JSON_WS_ITEM_NAME] = m_name;
  data[JSON_WS_ITEM_TYPE] = get_ws_item_class_name();
  data[JSON_IS_VISIBLE] = m_is_visible;
  json coord = json::array({m_pos[0], m_pos[1], m_pos[2]});
  data[JSON_POS] = coord;
}

void ws_item_t::read_from_json(json &data){
  //read m_name & is_visible & pos
  if (data.find(JSON_WS_ITEM_NAME) != data.end()) m_name = data[JSON_WS_ITEM_NAME];
  if (data.find(JSON_IS_VISIBLE) != data.end()) m_is_visible = data[JSON_IS_VISIBLE];

  if (support_translation())
    if (data.find(JSON_POS) != data.end()) {
        vector3<float> pos_rd = vector3<float>::Zero();
        for(uint8_t i = 0; i < 3; i++)
          pos_rd[i] = data[JSON_POS][i].get<float>();
        m_pos = pos_rd;
      }


}
