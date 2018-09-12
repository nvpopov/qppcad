#include <qppcad/ui_manager.hpp>
#include <qppcad/app.hpp>

using namespace qpp;

ui_manager_t::ui_manager_t(){
  iObjInspWidth = 300;
  iWorkPanelHeight = 35;
  iWorkPanelYOffset = 28;

}

void ui_manager_t::setup_style(){
  ImGuiStyle * style = &ImGui::GetStyle();
  style->FrameRounding = 2.0f;
  style->ScrollbarRounding = 2.0f;
  style->FrameBorderSize = 1.0f;
  //style->Colors[ImGuiCol_Button] = ImVec4(0.40f, 0.39f, 0.12f, 1.00f);
}

void ui_manager_t::render_ui(){
  render_main_menu();
  render_task_panel();
  if(c_app::get_state().cur_task == app_task_type::TASK_WORKSPACE_EDITOR){
      render_work_panel();
      render_ws_tabs();
      if (c_app::get_state().show_object_inspector) render_object_inspector();
    }

  if(c_app::get_state().cur_task == app_task_type::TASK_MENDELEY_TABLE){
      render_mtable_big();
    }


}

void ui_manager_t::render_main_menu(){
  bool bShowExitDialog = false;
  bool show_rename_workspace_dialog = false;
  app_state_t* astate = &(c_app::get_state());
  //
  //ImGui::PushStyleVar(, ImVec2(0.85, 2.85));
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8,8));

  if (ImGui::BeginMainMenuBar()){

      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6,6));
      if (ImGui::BeginMenu("File")){
          ImGui::MenuItem("New");
          ImGui::MenuItem("Open");
          if (ImGui::BeginMenu("Import")){

              if (ImGui::MenuItem("Standart XYZ(0D)")){
                  astate->workspace_manager->
                      query_import_file_as_new_workspace(qc_file_format::format_standart_xyz);
                }

              if (ImGui::MenuItem("VASP POSCAR(CONTCAR)")){
                  astate->workspace_manager->
                      query_import_file_as_new_workspace(qc_file_format::format_vasp_poscar);
                }
              ImGui::EndMenu();
            }
          ImGui::MenuItem("Save");
          ImGui::MenuItem("Save as");

          if (ImGui::MenuItem("Exit")){
              qpp::c_app::log("Menu -> File -> Exit clicked");
              bShowExitDialog = true;
            }

          ImGui::EndMenu();
        }


      if (ImGui::BeginMenu("Edit")){
          ImGui::MenuItem("Undo");
          ImGui::MenuItem("Redo");
          ImGui::EndMenu();
        }

      if (ImGui::BeginMenu("View")){
          if (c_app::get_state().workspace_manager->has_wss()){
              if(ImGui::BeginMenu("Camera")){
                  int _cp_t = c_app::get_state().camera->cur_proj;
                  ImGui::RadioButton("Ortho", &_cp_t, int(app_camera_proj_type::CAMERA_PROJ_ORTHO));
                  ImGui::SameLine();
                  ImGui::RadioButton("Perspective", &_cp_t, int(app_camera_proj_type::CAMERA_PROJ_PERSP));
                  ImGui::SameLine();
                  if(_cp_t != c_app::get_state().camera->cur_proj){
                      c_app::get_state().camera->set_projection(app_camera_proj_type(_cp_t));
                      c_app::get_state().workspace_manager->get_current_workspace()->set_best_view();
                    }
                  ImGui::EndMenu();
                }
            }


          if(ImGui::BeginMenu("Debug")){
              ImGui::Checkbox("Show tws-Tree",
                              &(c_app::get_state().debug_show_tws_tree));
              ImGui::Checkbox("Show debug selection ray",
                              &(c_app::get_state().debug_show_selection_ray));
              ImGui::EndMenu();
            }

          ImGui::Separator();

          ImGui::Checkbox("Cartesian Axis" , &(c_app::get_state().show_axis));
          ImGui::Checkbox("Grid XY" , &(c_app::get_state().show_grid));

          ImGui::Separator();

          ImGui::SliderFloat("Atom size scale",
                             &(c_app::get_state().atom_radius_scale_factor),
                             0.25f, 2.0f, "%.4f", 1);
          ImGui::SliderFloat("Bond size scale",
                             &(c_app::get_state().bond_radius_scale_factor),
                             0.02f, 2.0f, "%.4f", 1);

          ImGui::EndMenu();
        }

      if (ImGui::BeginMenu("Tools")){
          ImGui::MenuItem("Tool1");
          ImGui::MenuItem("Tool2");
          ImGui::EndMenu();
        }

      if (ImGui::BeginMenu("Help")){
          ImGui::MenuItem("Help1");
          ImGui::MenuItem("Help2");
          ImGui::EndMenu();
        }

      ImGui::PopStyleVar();

      int e_task = c_app::get_state().cur_task;

      ImGui::Separator();
      ImGui::SameLine();
      //ImGui::Set
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5,15));
      ImGui::RadioButton("Workspace editor", &e_task,
                         int(app_task_type::TASK_WORKSPACE_EDITOR));
      ImGui::SameLine();
      ImGui::RadioButton("Node editor", &e_task,
                         int(app_task_type::TASK_NODE_EDITOR));
      ImGui::SameLine();
      ImGui::RadioButton("Mendeley table", &e_task,
                         int(app_task_type::TASK_MENDELEY_TABLE));
      c_app::get_state().cur_task = app_task_type(e_task);
      ImGui::Separator();
      ImGui::SameLine();
      ImGui::PopStyleVar();

      //
      int ui_current_workspace = astate->workspace_manager->get_current_workspace_id();

      std::vector<std::string>  vStr;
      std::vector<char*>  vChar;
      for (int i = 0; i < astate->workspace_manager->m_ws.size(); i++)
        vStr.push_back(astate->workspace_manager->m_ws[i]->m_ws_name);
      std::transform(vStr.begin(), vStr.end(),
                     std::back_inserter(vChar),
                     vec_str_to_char);

      ImGui::PushItemWidth(150);
      ImGui::Combo("Workspace", &ui_current_workspace, vChar.data(),
                   astate->workspace_manager->m_ws.size());
      ImGui::PopItemWidth();

      for ( size_t i = 0 ; i < vChar.size() ; i++ ) delete [] vChar[i];
      if (ui_current_workspace != astate->workspace_manager->get_current_workspace_id())
        astate->workspace_manager->set_current_workspace(ui_current_workspace);
      //

      if (ImGui::Button("New")){

        }

      if (ImGui::Button("Del")){

        }

      if (ImGui::Button("Ren")){
          show_rename_workspace_dialog = true;
        }

      ImGui::Separator();
      ImGui::SameLine();
      ImGui::Text(fmt::format("| FPS : {} |", c_app::get_state().FPS).c_str());

      ImGui::EndMainMenuBar();

    }

  ImGui::PopStyleVar();

  if (bShowExitDialog) ImGui::OpenPopup("Quit?");
  if (ImGui::BeginPopupModal("Quit?")){
      ImGui::Text("Do you want to quit?");
      if (ImGui::Button("Yes", ImVec2(120, 40))){
          //ImGui::CloseCurrentPopup();
          exit(0);
        }
      ImGui::SameLine();
      if (ImGui::Button("No", ImVec2(120, 40))){
          ImGui::CloseCurrentPopup();
        }
      ImGui::SetItemDefaultFocus();
      ImGui::EndPopup();
    }

  if (show_rename_workspace_dialog && astate->workspace_manager->has_wss()){
      ImGui::OpenPopup("Renaming workspace");
      ImGui::SetNextWindowSize(ImVec2(300, 110));
      ImGui::SetNextWindowPos(ImVec2(100, 20));
      strcpy(s_rename_workspace_name, astate->workspace_manager->get_current_workspace()->m_ws_name.c_str());
    }

  if (ImGui::BeginPopupModal("Renaming workspace", NULL, ImGuiWindowFlags_NoResize)){
      ImGui::Columns(2);
      ImGui::Text("Current name");
      ImGui::Text("New name");
      ImGui::NextColumn();
      ImGui::Text(astate->workspace_manager->get_current_workspace()->m_ws_name.c_str());


      ImGui::PushID("input1");
      ImGui::InputText("", s_rename_workspace_name, 60);
      ImGui::PopID();
      ImGui::Columns(1);

      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::Dummy(ImVec2(76, 0)); ImGui::SameLine();
      if (ImGui::Button("Rename")){
          astate->workspace_manager->get_current_workspace()->m_ws_name =
              std::string(s_rename_workspace_name);
          ImGui::CloseCurrentPopup();
        }
      ImGui::SameLine();
      if (ImGui::Button("Close")){
          ImGui::CloseCurrentPopup();
        }
      ImGui::EndPopup();

    }
  //
}

void ui_manager_t::render_work_panel(){

  app_state_t* astate = &(c_app::get_state());

  ImGui::SetNextWindowSize(ImVec2( c_app::get_state().wWidth, iWorkPanelHeight));
  ImGui::SetNextWindowPos(ImVec2(0, iWorkPanelYOffset));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.1f);
  ImGui::Begin("task_panel", nullptr,
               ImGuiWindowFlags_NoTitleBar |
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_NoScrollbar |
               ImGuiWindowFlags_NoBringToFrontOnFocus);
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  window->DC.LayoutType = ImGuiLayoutType_Horizontal;

  ImGui::ToggleButton("~" , &astate->show_console);
  ImGui::Separator();
  ImGui::Text("View:");
  ImGui::Button("a" , ImVec2(20,20));
  ImGui::Button("b" , ImVec2(20,20));
  ImGui::Button("c" , ImVec2(20,20));
  ImGui::Separator();


  ImGui::Text("T:");
  ImGui::Button("X" , ImVec2(20,20));
  ImGui::Button("Y" , ImVec2(20,20));
  ImGui::Button("Z" , ImVec2(20,20));
  ImGui::Separator();



  if (c_app::get_state().workspace_manager->has_wss()){
      ImGui::Text("Edit:");
      int edit_mode = int(c_app::get_state().workspace_manager->
                          get_current_workspace()->m_edit_type);

      ImGui::BeginTabs("newtab", 2, edit_mode, 35 );
      if (ImGui::AddTab( "ITM")) {
          c_app::get_state().workspace_manager->
              get_current_workspace()->m_edit_type = ws_edit_type::EDIT_WS_ITEM;
        }

      if (ImGui::AddTab( "CNT")) {
          c_app::get_state().workspace_manager->
              get_current_workspace()->m_edit_type = ws_edit_type::EDIT_WS_ITEM_CONTENT;
        }

      ImGui::EndTabs();
      c_app::get_state().workspace_manager->get_current_workspace()->
          m_edit_type = ws_edit_type(edit_mode);

      ImGui::Spacing();
      ImGui::Separator();

      ImGui::Button("Undo" , ImVec2(40,20));
      ImGui::Button("Redo" , ImVec2(40,20));
      ImGui::Separator();

      ImGui::Button("Tr. mode" , ImVec2(60,20));
      ImGui::Separator();

      ImGui::ToggleButton("INSP", &(c_app::get_state().show_object_inspector));
      ImGui::Separator();

    }



  ImGui::End();
  ImGui::PopStyleVar();
}

void ui_manager_t::render_task_panel(){

}

void ui_manager_t::render_ws_tabs(){
  //  ImGui::SetNextWindowSize(ImVec2(c_app::get_state().wWidth - iObjInspWidth ,
  //                                  35
  //                                  ));
  //  ImGui::SetNextWindowPos(ImVec2(0,
  //                                 iWorkPanelYOffset + iWorkPanelHeight));

  // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.f);

  //  if (ImGui::Begin("tanb", NULL,
  //               ImGuiWindowFlags_NoMove |
  //               ImGuiWindowFlags_NoResize |
  //               ImGuiWindowFlags_NoCollapse |
  //               ImGuiWindowFlags_NoNavFocus |
  //               ImGuiWindowFlags_NoTitleBar)){
  // // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
  //  ImGui::Button("workspace1");
  //  //ImGui::PopStyleVar();
  //    }
  //  ImGui::End();

  //  ImGui::PopStyleVar(2);

}

void ui_manager_t::render_object_inspector(){

  app_state_t* astate = &(c_app::get_state());

  ImGui::SetNextWindowSize(ImVec2(iObjInspWidth ,
                                  astate->wHeight-(iWorkPanelYOffset +
                                                   iWorkPanelHeight)
                                  ));
  ImGui::SetNextWindowPos(ImVec2(astate->wWidth - iObjInspWidth,
                                 iWorkPanelYOffset + iWorkPanelHeight));


  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

  ImGui::Begin("Object inspector", nullptr,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_MenuBar);

  if(ImGui::BeginMenuBar()){
      if(ImGui::BeginMenu("Add")){
          ImGui::MenuItem("Geometry");
          ImGui::MenuItem("Debug");
          ImGui::EndMenu();
        }

      if(ImGui::BeginMenu("Import")){
          if (ImGui::MenuItem("XYZ")) {
              if (astate->workspace_manager->has_wss())
                astate->workspace_manager->get_current_workspace()->
                    dialog_add_geom_from_file(qc_file_format::format_standart_xyz);
            }
          if (ImGui::MenuItem("VASP")){
              if (astate->workspace_manager->has_wss())
                astate->workspace_manager->get_current_workspace()->
                    dialog_add_geom_from_file(qc_file_format::format_vasp_poscar);
            }

          ImGui::EndMenu();
        }

      ImGui::EndMenuBar();
    }

  ImGui::Text("Workspace items:");
  ImGui::Spacing();
  auto iCurWs = astate->workspace_manager->get_current_workspace_id();

  ImGui::PushItemWidth(284);
  auto cur_ws = astate->workspace_manager->m_ws[iCurWs];
  if (cur_ws != nullptr){
      int ws_itm_cur = cur_ws->get_selected_item();
      ImGui::PushID(1);
      ImGui::ListBox_stl("", &ws_itm_cur, cur_ws->m_ws_names_c, 8);
      ImGui::PopID();
      if (ws_itm_cur != cur_ws->get_selected_item())
        cur_ws->set_selected_item(ws_itm_cur);

      ImGui::PopItemWidth();

      ImGui::Spacing();
      ImGui::Separator();
      if (cur_ws->get_selected_item() != -1){
          ImGui::Text(fmt::format("Selected: {}",
                                  cur_ws->get_selected()->compose_item_name()).c_str());
          ImGui::Separator();
          ImGui::Spacing();
          cur_ws->get_selected()->render_ui();
        }
      else {
          ImGui::Text("Selected: None");
          ImGui::Separator();
        }
    }

  ImGui::End();
  ImGui::PopStyleVar(1);

}

void ui_manager_t::render_mtable_big(){
  float mendFrm = 0.85f;
  ImGui::SetNextWindowSize(ImVec2(c_app::get_state().wWidth*mendFrm ,
                                  c_app::get_state().wHeight*mendFrm));
  ImGui::SetNextWindowPos(ImVec2(c_app::get_state().wWidth*(1-mendFrm)*0.5f,
                                 c_app::get_state().wHeight*(1-mendFrm)*0.5f));

  ImGui::Begin("Mendeley table", nullptr,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_NoCollapse);

  ImGui::End();
}

