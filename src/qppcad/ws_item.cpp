#include <qppcad/ws_item.hpp>
#include <qppcad/app_state.hpp>
#include <data/color.hpp>
#include <qppcad/workspace.hpp>

using namespace qpp;
using namespace qpp::cad;

void ws_item_t::set_parent_workspace(workspace_t *_parent_ws){
  m_parent_ws = _parent_ws;
}

const std::string ws_item_t::get_name(){
  return m_name;
}

void ws_item_t::set_name(const std::string &_name){
  if (m_name != _name){
      m_name = _name;
      m_parent_ws->ws_changed();
    }
}

void ws_item_t::set_name(const char *_name){
  if (m_name != _name){
      m_name = std::string(_name);
      m_parent_ws->ws_changed();
    }
}

bool ws_item_t::is_selected() {

  if (m_parent_ws) {
      return m_parent_ws->get_selected() == this;
    }
  else {
      return false;
    }

}

void ws_item_t::render () {

  app_state_t* astate = app_state_t::get_inst();

  if (m_selected && (get_flags() & ws_item_flags_support_selection) &&
      (get_flags() & ws_item_flags_support_rendering_bb) && is_bb_visible()) {
      astate->dp->begin_render_aabb();
      if (m_parent_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM)
        astate->dp->render_aabb(clr_fuchsia,
                                     m_pos + m_aabb.min,
                                     m_pos + m_aabb.max  );
      else
        astate->dp->render_aabb_segmented(clr_olive,
                                               m_pos + m_aabb.min,
                                               m_pos + m_aabb.max);
      astate->dp->end_render_aabb();
    }
}

void ws_item_t::render_overlay(QPainter *painter) {

}

//void ws_item_t::render_ui () {

//  ImGui::Spacing();

//  if (ImGui::CollapsingHeader("General properties", ImGuiTreeNodeFlags_DefaultOpen)){
//      char * s_item_name = new char[60];
//      strcpy(s_item_name, m_name.c_str());
//      ImGui::InputText("Item name", s_item_name, 60);

//      if (get_flags() & ws_item_flags_support_rendering)
//        ImGui::Checkbox("Draw workspace item", &m_is_visible);

//      ImGui::Spacing();

//      ImGui::Separator();

//      if ( get_flags() & ws_item_flags_support_actions) {
//          ImGui::Text("Actions:");

//          if (get_flags() & ws_item_flags_support_delete) {
//              if (ImGui::Button("Delete")) m_marked_for_deletion = true;
//              ImGui::SameLine();
//            }

//          if (get_flags() & ws_item_flags_support_clone) {
//              ImGui::Button("Clone");
//              ImGui::SameLine();
//            }

//          if (get_flags() & ws_item_flags_support_moveto) {
//              ImGui::Button("Move to");
//            }
//          ImGui::Spacing();
//          ImGui::Separator();
//          ImGui::Spacing();
//        }

//      if (m_name != s_item_name) set_name(s_item_name);
//      delete[] s_item_name;
//    }
//}

//void ws_item_t::td_context_menu_edit_item () {
//  //  if (support_translation())
//  //    if (ImGui::BeginMenu("Translate Node")){

//  //        ImGui::SliderFloat("X", &(explicit_translation[0]), -10.0, 10.0);
//  //        ImGui::SameLine();
//  //        ImGui::Button("Apply X");

//  //        ImGui::SliderFloat("Y", &(explicit_translation[1]), -10.0, 10.0);
//  //        ImGui::SameLine();
//  //        ImGui::Button("Apply Y");

//  //        ImGui::SliderFloat("Z", &(explicit_translation[2]), -10.0, 10.0);
//  //        ImGui::SameLine();
//  //        ImGui::Button("Apply Z");
//  //        ImGui::EndMenu();
//  //      }
//}

//void ws_item_t::td_context_menu_edit_content () {

//}

void ws_item_t::set_default_flags(uint32_t flags){
  p_flags = flags;
}

uint32_t ws_item_t::get_flags() const {
  return p_flags;
}


void ws_item_t::update (float delta_time) {

}

float ws_item_t::get_bb_prescaller(){
  return 1.0f;
}

bool ws_item_t::is_bb_visible() {
  return true;
}

const vector3<float> ws_item_t::get_gizmo_content_barycenter() {
  return vector3<float>::Zero();
}

//ws_item_t::~ws_item_t(){

//}

void ws_item_t::on_begin_node_gizmo_translate(){
  m_pos_old = m_pos;
//  c_app::log(fmt::format("Start of translation of node [{}], pos = {}",
//                         m_name, m_pos.to_string_vec()));
}

void ws_item_t::on_end_node_gizmo_translate(){
//  c_app::log(fmt::format("End of translation of node [{}], pos = {}",
//                         m_name, m_pos.to_string_vec()));
}

void ws_item_t::on_begin_content_gizmo_translate() {

}

void ws_item_t::apply_intermediate_translate_content(const vector3<float> &new_pos){

}

void ws_item_t::on_end_content_gizmo_translate() {

}

void ws_item_t::write_to_json(json &data) {
  data[JSON_WS_ITEM_NAME] = m_name;
  data[JSON_WS_ITEM_TYPE] = get_ws_item_class_name();
  data[JSON_IS_VISIBLE] = m_is_visible;
  json coord = json::array({m_pos[0], m_pos[1], m_pos[2]});
  data[JSON_POS] = coord;
}

void ws_item_t::read_from_json(json &data) {
  //read m_name & is_visible & pos
  if (data.find(JSON_WS_ITEM_NAME) != data.end()) m_name = data[JSON_WS_ITEM_NAME];
  if (data.find(JSON_IS_VISIBLE) != data.end()) m_is_visible = data[JSON_IS_VISIBLE];

  if (get_flags() | ws_item_flags_support_translation)
    if (data.find(JSON_POS) != data.end()) {
        vector3<float> pos_rd = vector3<float>::Zero();
        for(uint8_t i = 0; i < 3; i++)
          pos_rd[i] = data[JSON_POS][i].get<float>();
        m_pos = pos_rd;
      }
}
