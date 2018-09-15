#include <qppcad/workspace.hpp>
#include <qppcad/app.hpp>
#include <data/ptable.hpp>
#include <io/geomio.hpp>
#include <mathf/math.hpp>

using namespace qpp;

int16_t workspace_t::get_selected_item(){
  for (uint16_t i = 0; i < m_ws_items.size(); i++)
    if (m_ws_items[i]->m_selected) return i;
  return -1;
}

ws_item_t *workspace_t::get_selected(){
  int16_t sel_idx = get_selected_item();
  if (sel_idx != -1) return m_ws_items[sel_idx].get();
  else return nullptr;
}

void workspace_t::set_selected_item(const int16_t sel_idx){
  unselect_all();
  if (sel_idx >= 0 && sel_idx < m_ws_items.size() && m_ws_items.size() > 0){
      m_ws_items[sel_idx]->m_selected = true;
      m_gizmo->attached_item = m_ws_items[sel_idx].get();;
    }
}

void workspace_t::unselect_all(){
  for (auto &ws_item : m_ws_items) ws_item->m_selected = false;
}

void workspace_t::workspace_changed(){
  //rebuild ws items
  m_ws_names_c.clear();
  for(uint8_t i = 0; i < m_ws_items.size(); i++)
    m_ws_names_c.push_back(fmt::format("[{}] {}", i, m_ws_items[i]->m_name));
}

void workspace_t::reset_camera(){
  m_camera->reset_camera();
  set_best_view();
}

void workspace_t::set_best_view(){

  if (m_ws_items.size() == 0) {
      m_camera->reset_camera();
      return;
    }

  vector3<float> vec_look_at  = vector3<float>(0.0, 0.0, 0.0);
  vector3<float> vec_look_pos = vector3<float>(0.0, 0.0, 0.0);

  for (auto &ws_item : m_ws_items)
    ws_item->vote_for_view_vectors(vec_look_pos, vec_look_at);

  vec_look_at  /= m_ws_items.size();
  vec_look_pos /= m_ws_items.size();

  m_camera->m_look_at = vec_look_at;
  m_camera->m_view_point = vec_look_pos;

  //  std::cout << "set bv " << _vLookAt << std::endl << _vLookPos << std::endl
  //            << "end bv " << std::endl;
  m_camera->orthogonalize_gs();

  if ((m_camera->m_look_at-m_camera->m_view_point).norm() < 0.4f)
    m_camera->reset_camera();
}

void workspace_t::render(){

  if (m_first_render) {
      set_best_view();
      m_first_render = false;
    }

  app_state_t* astate = &(c_app::get_state());

  if (m_gizmo->is_active && m_gizmo->attached_item) m_gizmo->render();

  if (astate->debug_show_selection_ray){
      astate->dp->begin_render_line();
      astate->dp->render_line(vector3<float>(1.0, 1.0, 0.0), m_ray_debug.start,
                              m_ray_debug.start + m_ray_debug.dir * 155.0);
      astate->dp->end_render_line();
    }

  if (astate->dp != nullptr){

      ///// Draw grid /////
      if (astate->show_grid){
          astate->line_mesh_program->begin_shader_program();
          vector3<float> color(0.75, 0.75, 0.75);
          astate->line_mesh_program->set_u(sp_u_name::m_model_view_proj,
                                           astate->camera->m_view_proj.data());
          astate->line_mesh_program->set_u(sp_u_name::m_model_view, astate->camera->m_mat_view.data());
          astate->line_mesh_program->set_u(sp_u_name::v_color, (GLfloat*)color.data());

          for (int dx = -4; dx <= 4; dx++)
            for (int dz = -4; dz <= 4; dz++){
                vector3<float> vTr(dx * (20.0f * 0.5f), dz * (20.0f * 0.5f), 0.0f );
                astate->line_mesh_program->set_u(sp_u_name::v_translate, (GLfloat*)vTr.data());
                astate->gridXZ->render();

              }
          astate->line_mesh_program->end_shader_program();
        }
      ///// Draw grid /////

      ///// Draw axis /////
      if (astate->show_axis){
          vector3<float> vScrTW = astate->camera->unproject(-0.95f, -0.90f);
          float fAxisLen = 0.07f *astate->camera->m_stored_dist;
          if (astate->camera->cur_proj ==
              app_camera_proj_type::CAMERA_PROJ_PERSP)
            fAxisLen = 0.015f;

          astate->dp->begin_render_line();
          astate->dp->
              render_line(vector3<float>(1.0, 0.0, 0.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(fAxisLen, 0.0, 0.0) + vScrTW);

          astate->dp->
              render_line(vector3<float>(0.0, 1.0, 0.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(0.0, fAxisLen, 0.0) + vScrTW);

          astate->dp->
              render_line(vector3<float>(0.0, 0.0, 1.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(0.0, 0.0, fAxisLen) + vScrTW);
          astate->dp->end_render_line();

        }
      ///// Draw axis end /////
    }

  for (auto &ws_item : m_ws_items) ws_item->render();

}

void workspace_t::mouse_click(const double mouse_x, const double mouse_y){
  m_ray_debug.dir = (m_camera->unproject(mouse_x, mouse_y) - m_camera->m_view_point).normalized();


  m_ray_debug.start = m_camera->m_view_point;
//  if (m_camera->cur_proj == app_camera_proj_type::CAMERA_PROJ_ORTHO) m_ray_debug.start *= m_camera->m_ortho_scale;

  if (m_gizmo->process_ray(&m_ray_debug)){
      c_app::log("gizmo clicked");
      return;
    }

  bool hit_any = false;

  if (m_edit_type != ws_edit_type::EDIT_WS_ITEM_CONTENT){
      for (auto &ws_item : m_ws_items) ws_item->m_selected = false;
      m_gizmo->attached_item = nullptr;
    }

  for (auto &ws_item : m_ws_items){
      bool is_hit = ws_item->mouse_click(&m_ray_debug);
      hit_any = hit_any || is_hit;
      if (is_hit && m_edit_type == ws_edit_type::EDIT_WS_ITEM && ws_item->support_selection()){
          m_gizmo->attached_item = ws_item.get();
          ws_item->m_selected = true;
        }
    }

  if (m_edit_type != ws_edit_type::EDIT_WS_ITEM_CONTENT && !hit_any){
      m_gizmo->attached_item = nullptr;
      for (auto &ws_item : m_ws_items) ws_item->m_selected = false;
    }

}

void workspace_t::add_item_to_workspace(const shared_ptr<ws_item_t> &item_to_add){

  item_to_add->set_parent_workspace(shared_from_this());
  m_ws_items.push_back(item_to_add);
  workspace_changed();
  c_app::log(fmt::format("New workspace {} size = {}", m_ws_name, m_ws_items.size()));

}

void workspace_t::dialog_add_geom_from_file(qc_file_format file_format){

  app_state_t *astate = &(c_app::get_state());

  bool succes = false;
  string filter = "*";
  string file_name_fd = astate->fd_manager->request_open_file(filter, succes);

  if (succes){
      auto wsl = make_shared<ws_atoms_list_t>();
      add_item_to_workspace(wsl);
      wsl->m_name = "test1";
      wsl->load_from_file(file_format, file_name_fd, false);
    }

}

void workspace_t::update(float delta_time){

  m_gizmo->update_gizmo(delta_time);
  for (auto &ws_item : m_ws_items) ws_item->update(delta_time);

}

shared_ptr<workspace_t> workspace_manager_t::get_current(){

  if (m_current_workspace_id >= m_ws.size())
    return nullptr;
  return m_ws[m_current_workspace_id];

}

optional<uint8_t> workspace_manager_t::get_current_id(){

  if (m_ws.size() != 0) return optional<uint8_t>(m_current_workspace_id);
  return nullopt;

}

bool workspace_manager_t::set_current(const uint8_t ws_index){

  bool succes = false;
  if (ws_index < m_ws.size()){
      m_current_workspace_id = ws_index;
      succes = true;
    }
  return succes;

}

void workspace_manager_t::init_default(){

  auto _ws2 = make_shared<workspace_t>();
  _ws2->m_ws_name = "d2";

  auto _ws3 = make_shared<workspace_t>();
  _ws3->m_ws_name = "d1";

  auto _wsl2 = make_shared<ws_atoms_list_t>();
  _ws3->add_item_to_workspace(_wsl2);
  _wsl2->load_from_file(qc_file_format::format_vasp_poscar, "../data/refs/laf3_p3.vasp",
                        false);

  auto _wsl3 = make_shared<ws_atoms_list_t>();
  _ws2->add_item_to_workspace(_wsl3);
  _wsl3->load_from_file(qc_file_format::format_vasp_poscar, "../data/refs/POSCAR.mp-558947_SiO2",
                        false);

  auto _wsl32 = make_shared<ws_atoms_list_t>();
  _ws2->add_item_to_workspace(_wsl32);
  _wsl32->load_from_file(qc_file_format::format_standart_xyz,
                         "../deps/qpp/examples/io/ref_data/nanotube.xyz",
                         true);

  auto _wsl33 = make_shared<ws_atoms_list_t>();
  _ws2->add_item_to_workspace(_wsl33);
  _wsl33->load_from_file(qc_file_format::format_vasp_poscar, "../data/refs/mp-971662_Si.vasp",
                         false);

  _wsl3->m_name = "zeolite1";
  _wsl32->m_name = "nanotube1";
  _wsl32->m_pos = vector3<float>(0.0f, 0.0f, 14.0f);
  _wsl33->m_name = "ss1";
  _wsl33->m_pos = vector3<float>(0.0f, 22.0f, 2.0f);


  add_workspace(_ws3);
  add_workspace(_ws2);

  m_current_workspace_id = 1;
}

void workspace_manager_t::render_current_workspace(){

  if (has_wss())
    if (m_current_workspace_id < m_ws.size()){
        c_app::get_state().camera = m_ws[m_current_workspace_id]->m_camera.get();
        m_ws[m_current_workspace_id]->render();
      }
}

void workspace_manager_t::mouse_click(){

  app_state_t* astate =  &(c_app::get_state());

  //transform from window frame to viewport frame
  float new_mouse_x = astate->mouse_x;
  float new_mouse_y = astate->mouse_y - astate->ui_manager->iWorkPanelHeight
                    - astate->ui_manager->iWorkPanelYOffset;
  c_app::log(fmt::format("Mouse click {} {}", astate->mouse_x, astate->mouse_y));

  if (new_mouse_x > 0 && new_mouse_x < astate->vViewportWidthHeight(0) &&
      new_mouse_y > 0 && new_mouse_y < astate->vViewportWidthHeight(1)){

      new_mouse_x = (new_mouse_x/astate->vViewportWidthHeight(0)-0.5)*2.0;
      new_mouse_y = (new_mouse_y/astate->vViewportWidthHeight(1)-0.5)*-2.0;

      c_app::log(fmt::format("Mouse click in ws {} {}", new_mouse_x, new_mouse_y));

      if(has_wss()) get_current()->mouse_click(new_mouse_x, new_mouse_y);
    }
}

void workspace_manager_t::add_workspace(const shared_ptr<workspace_t> &ws_to_add){
  m_ws.push_back(ws_to_add);
}

void workspace_manager_t::query_import_file_as_new_workspace(qc_file_format file_format){
  app_state_t *astate = &(c_app::get_state());
  bool succes = false;
  std::string filter = "*";
  std::string file_name_fd =
      astate->fd_manager->request_open_file(filter, succes);
  if (succes){
      auto _ws2 = make_shared<workspace_t>();
      std::string file_name_extr = qpp::extract_base_name(file_name_fd);
      _ws2->m_ws_name = file_name_extr;
      auto _wsl2 = make_shared<ws_atoms_list_t>();
      _ws2->add_item_to_workspace(_wsl2);
      _wsl2->load_from_file(file_format, file_name_fd, false);
      add_workspace(_ws2);
      set_current(m_ws.size()-1);
    }
}

