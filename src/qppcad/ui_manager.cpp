#include <qppcad/ui_manager.hpp>
#include <qppcad/app.hpp>
#include <qppcad/uniq_id.hpp>

using namespace qpp;
using namespace qpp::cad;

ui_manager_t::ui_manager_t(app_state_t *init_app_state){
  // app_state_t *astate = &(c_app::get_state());
  console_widget = make_unique<console_widget_t>(init_app_state);
  m_rename_ws_id = get_uniq_id();

  show_rename_workspace_dialog = false;

  iObjInspWidth = 350;
  iWorkPanelHeight = 38;
  iWorkPanelYOffset = 28;
  setup_style();

  init_app_state->kb_manager->connect("edit_mode_toggle", this, &ui_manager_t::toggle_edit_mode);

}

void ui_manager_t::toggle_edit_mode(){
  app_state_t* astate = &(c_app::get_state());
  if (astate->ws_manager->has_wss()) astate->ws_manager->get_current()->toggle_edit_mode();
}

void ui_manager_t::setup_style(){
  ImGuiStyle * style = &ImGui::GetStyle();
  style->FrameRounding = 3.0f;
  //style->
  style->ScrollbarRounding = 2.0f;
  style->FrameBorderSize = 1.0f;
  style->Colors[ImGuiCol_Text]                  = {0.99333335f, 0.99333335f, 0.99333335f, 1.00f};
  style->Colors[ImGuiCol_TextDisabled]          = {0.34509805f, 0.34509805f, 0.34509805f, 1.00f};
  style->Colors[ImGuiCol_WindowBg]              = {0.23529413f, 0.24705884f, 0.25490198f, 0.94f};
  style->Colors[ImGuiCol_ChildBg]               = {0.23529413f, 0.24705884f, 0.25490198f, 0.9f};
  style->Colors[ImGuiCol_PopupBg]               = {0.23529413f, 0.24705884f, 0.25490198f, 0.94f};
  style->Colors[ImGuiCol_Border]                = {0.33333334f, 0.33333334f, 0.33333334f, 0.90f};
  style->Colors[ImGuiCol_BorderShadow]          = {0.15686275f, 0.15686275f, 0.15686275f, 0.00f};
  style->Colors[ImGuiCol_FrameBg]               = {0.16862746f, 0.16862746f, 0.16862746f, 0.98f};
  style->Colors[ImGuiCol_FrameBgHovered]        = {0.453125f, 0.67578125f, 0.99609375f, 0.97f};
  style->Colors[ImGuiCol_FrameBgActive]         = {0.47058827f, 0.47058827f, 0.47058827f, 0.97f};
  style->Colors[ImGuiCol_TitleBg]               = {0.04f, 0.04f, 0.04f, 1.00f};
  style->Colors[ImGuiCol_TitleBgCollapsed]      = {0.16f, 0.29f, 0.48f, 1.00f};
  style->Colors[ImGuiCol_TitleBgActive]         = {0.00f, 0.00f, 0.00f, 0.96f};
  style->Colors[ImGuiCol_MenuBarBg]             = {0.21058825f, 0.21627452f, 0.2101961f, 0.990f};
  style->Colors[ImGuiCol_ScrollbarBg]           = {0.27058825f, 0.28627452f, 0.2901961f, 0.90f};
  style->Colors[ImGuiCol_ScrollbarGrab]         = {0.21960786f, 0.30980393f, 0.41960788f, 0.91f};
  style->Colors[ImGuiCol_ScrollbarGrabHovered]  = {0.21960786f, 0.30980393f, 0.41960788f, 1.00f};
  style->Colors[ImGuiCol_ScrollbarGrabActive]   = {0.13725491f, 0.19215688f, 0.2627451f, 0.91f};
  // style->Colors[ImGuiCol_ComboBg]               = {0.1f, 0.1f, 0.1f, 0.99f};
  style->Colors[ImGuiCol_CheckMark]             = {0.90f, 0.90f, 0.90f, 0.83f};
  style->Colors[ImGuiCol_SliderGrab]            = {0.70f, 0.70f, 0.70f, 0.62f};
  style->Colors[ImGuiCol_SliderGrabActive]      = {0.30f, 0.30f, 0.30f, 0.84f};
  style->Colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
  style->Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
  style->Colors[ImGuiCol_ButtonActive]           = ImVec4(0.334f, 0.681f, 0.044f, 1.000f);
  style->Colors[ImGuiCol_Header]                = ImVec4(0.44f, 0.44f, 0.44f, 0.90f);
  style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
  style->Colors[ImGuiCol_HeaderActive]          = {0.47058827f, 0.47058827f, 0.47058827f, 0.67f};
  style->Colors[ImGuiCol_Separator]             = {0.31640625f, 0.31640625f, 0.31640625f, 1.00f};
  style->Colors[ImGuiCol_SeparatorHovered]      = {0.31640625f, 0.31640625f, 0.31640625f, 1.00f};
  style->Colors[ImGuiCol_SeparatorActive]       = {0.31640625f, 0.31640625f, 0.31640625f, 1.00f};
  style->Colors[ImGuiCol_ResizeGrip]            = {1.00f, 1.00f, 1.00f, 0.85f};
  style->Colors[ImGuiCol_ResizeGripHovered]     = {1.00f, 1.00f, 1.00f, 0.60f};
  style->Colors[ImGuiCol_ResizeGripActive]      = {1.00f, 1.00f, 1.00f, 0.90f};
  style->Colors[ImGuiCol_PlotLines]             = {0.61f, 0.61f, 0.61f, 1.00f};
  style->Colors[ImGuiCol_PlotLinesHovered]      = {1.00f, 0.43f, 0.35f, 1.00f};
  style->Colors[ImGuiCol_PlotHistogram]         = {0.90f, 0.70f, 0.00f, 1.00f};
  style->Colors[ImGuiCol_PlotHistogramHovered]  = {1.00f, 0.60f, 0.00f, 1.00f};
  style->Colors[ImGuiCol_TextSelectedBg]        = {0.18431373f, 0.39607847f, 0.79215693f, 0.90f};
}

void ui_manager_t::render_ui(){
  render_main_menu();
  render_task_panel();
  if(c_app::get_state().cur_task == app_task_type::TASK_WORKSPACE_EDITOR){
      render_work_panel();
      render_ws_tabs();
      render_3d_viewport_context_menu();
      if (c_app::get_state().show_object_inspector) render_object_inspector();
    }

  if(c_app::get_state().cur_task == app_task_type::TASK_MENDELEY_TABLE){
      render_mtable_big();
    }

  console_widget->render();


}

void ui_manager_t::render_main_menu(){
  bool bShowExitDialog = false;

  app_state_t* astate = &(c_app::get_state());
  //
  //ImGui::PushStyleVar(, ImVec2(0.85, 2.85));
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8,8));

  if (ImGui::BeginMainMenuBar()){

      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6,6));
      if (ImGui::BeginMenu("File")){
          if (ImGui::MenuItem("New", "Ctrl + N")){
              astate->ws_manager->query_create_new_workspace();
            }

          if (ImGui::MenuItem("Open", "Ctrl + O")){
              astate->ws_manager->dialog_load_workspace();
            }

          ImGui::Separator();
          if (ImGui::BeginMenu("Import")){

              if (ImGui::BeginMenu("XYZ")){
                  if (ImGui::MenuItem("Standart XYZ(0D)")){
                      astate->ws_manager->
                          query_import_file_as_new_workspace(qc_file_format::format_standart_xyz);
                    }
                  if (ImGui::MenuItem("Multiframe XYZ")){
                      astate->ws_manager->
                          query_import_file_as_new_workspace(qc_file_format::format_multi_frame_xyz);
                    }
                  ImGui::EndMenu();
                }


              if (ImGui::BeginMenu("VASP")){
                  if (ImGui::MenuItem("VASP POSCAR(CONTCAR)")){
                      astate->ws_manager->
                          query_import_file_as_new_workspace(qc_file_format::format_vasp_poscar);
                    }
                  if (ImGui::MenuItem("VASP OUTCAR MD")){
                      astate->ws_manager->
                          query_import_file_as_new_workspace(qc_file_format::format_vasp_outcar_md);
                    }
                  ImGui::EndMenu();
                }
              ImGui::EndMenu();
            }


          ImGui::Separator();

          if (ImGui::MenuItem("Save", "Ctrl + S")){
              astate->ws_manager->dialog_save_current_workspace(false);
            }

          if (ImGui::MenuItem("Save as", "Alt + S")){
              astate->ws_manager->dialog_save_current_workspace(true);
            }

          ImGui::Separator();
          if (ImGui::MenuItem("Exit")){
              qpp::cad::c_app::log("Menu -> File -> Exit clicked");
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
          if (c_app::get_state().ws_manager->has_wss()){
              if(ImGui::BeginMenu("Camera")){
                  int _cp_t = c_app::get_state().camera->cur_proj;
                  ImGui::RadioButton("Ortho", &_cp_t, int(app_camera_proj_type::CAMERA_PROJ_ORTHO));
                  ImGui::SameLine();
                  ImGui::RadioButton("Perspective", &_cp_t, int(app_camera_proj_type::CAMERA_PROJ_PERSP));
                  ImGui::SameLine();
                  if(_cp_t != c_app::get_state().camera->cur_proj){
                      c_app::get_state().camera->set_projection(app_camera_proj_type(_cp_t));
                      c_app::get_state().ws_manager->get_current()->set_best_view();
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

          //          ImGui::SliderFloat("Atom size scale",
          //                             &(c_app::get_state().atom_radius_scale_factor),
          //                             0.25f, 2.0f, "%.4f", 1);
          //          ImGui::SliderFloat("Bond size scale",
          //                             &(c_app::get_state().bond_radius_scale_factor),
          //                             0.02f, 2.0f, "%.4f", 1);

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

      c_app::get_state().cur_task = app_task_type(e_task);
      ImGui::Separator();
      ImGui::SameLine();
      ImGui::PopStyleVar();

      //
      optional<size_t> ui_cur_ws = astate->ws_manager->get_current_id();
      int ui_cur_ws_exact = -1;
      if (ui_cur_ws) ui_cur_ws_exact = *ui_cur_ws;

      //TODO We need to reinvent the wheel vec<string> - > vec<char*>
      std::vector<std::string>  vStr;
      vStr.reserve(20);
      std::vector<char*>  vChar;
      for (size_t i = 0; i < astate->ws_manager->m_ws.size(); i++)
        vStr.push_back(astate->ws_manager->m_ws[i]->m_ws_name);
      std::transform(vStr.begin(), vStr.end(),
                     std::back_inserter(vChar),
                     vec_str_to_char);

      ImGui::PushItemWidth(170);
      ImGui::PushID(1);
      ImGui::Combo("", &ui_cur_ws_exact, vChar.data(), astate->ws_manager->m_ws.size());
      ImGui::PopItemWidth();
      ImGui::PopID();

      for ( size_t i = 0 ; i < vChar.size() ; i++ ) delete [] vChar[i];
      if (ui_cur_ws_exact != *astate->ws_manager->get_current_id())
        astate->ws_manager->set_current(ui_cur_ws_exact);
      //

      if (ImGui::Button("+")){
          astate->ws_manager->query_create_new_workspace();
        }

      if (ImGui::Button("-")){

        }

      if (ImGui::Button("R")){
          show_rename_workspace_dialog = true;
          if (astate->ws_manager->has_wss())
            strcpy(s_rename_workspace_name, astate->ws_manager->get_current()->m_ws_name.c_str());
        }

      ImGui::Separator();
      ImGui::SameLine();
      ImGui::Text(fmt::format("| FPS : {} |", c_app::get_state().FPS).c_str());

      ImGui::EndMainMenuBar();

    }

  ImGui::PopStyleVar();

  if (bShowExitDialog) ImGui::OpenPopup("Quit?");
  if (ImGui::BeginPopupModal("Quit?", nullptr, ImGuiWindowFlags_NoResize)){
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


  astate->config_vote_pool.vote_state(DISABLE_MOUSE_CONTROL_IN_WORKSPACE,
                                      m_rename_ws_id,
                                      show_rename_workspace_dialog);

  if (show_rename_workspace_dialog && astate->ws_manager->has_wss()){
      ImGui::OpenPopup("Renaming workspace");
      ImGui::SetNextWindowSize(ImVec2(400, 120));
      ImGui::SetNextWindowPos(ImVec2(650, 20));

    }

  if (ImGui::BeginPopupModal("Renaming workspace", &show_rename_workspace_dialog,
                             ImGuiWindowFlags_NoResize)){
      ImGui::Columns(2);
      ImGui::Text("Current name");
      ImGui::Text("New name");
      ImGui::NextColumn();
      ImGui::Text(astate->ws_manager->get_current()->m_ws_name.c_str());


      ImGui::PushID("input1");
      ImGui::PushItemWidth(180);
      ImGui::InputText("", s_rename_workspace_name, 180);
      ImGui::PushItemWidth(0);
      ImGui::PopID();
      ImGui::Columns(1);

      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::Dummy(ImVec2(117, 0)); ImGui::SameLine();
      if (ImGui::Button("Rename")){
          astate->ws_manager->get_current()->m_ws_name = string(s_rename_workspace_name);
          ImGui::CloseCurrentPopup();
          show_rename_workspace_dialog = false;
        }

      ImGui::SameLine();
      if (ImGui::Button("Close")){
          show_rename_workspace_dialog = false;
          ImGui::CloseCurrentPopup();
        }

      ImGui::EndPopup();

    }
  //
}

void ui_manager_t::render_work_panel(){

  app_state_t* astate = &(c_app::get_state());

  ImGui::SetNextWindowSize(ImVec2( c_app::get_state().viewport_size(0), iWorkPanelHeight));
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

  ImGui::ToggleButton("~" , &console_widget->m_active, ImVec2(20,24));
  ImGui::Separator();
  //  ImGui::Text("View:");
  //  ImGui::Button("a" , ImVec2(20,24));
  //  ImGui::Button("b" , ImVec2(20,24));
  //  ImGui::Button("c" , ImVec2(20,24));
  //  ImGui::Separator();

  if (c_app::get_state().ws_manager->has_wss()){
      ImGui::Text("Edit:");
      int edit_mode = int(c_app::get_state().ws_manager->get_current()->m_edit_type);

      ImGui::BeginTabs("newtab", 2, edit_mode, 40 );
      if (ImGui::AddTab( "ITM")) {
          c_app::get_state().ws_manager->get_current()->m_edit_type = ws_edit_type::EDIT_WS_ITEM;
        }

      if (ImGui::AddTab( "CNT")) {
          c_app::get_state().ws_manager->get_current()->m_edit_type =
              ws_edit_type::EDIT_WS_ITEM_CONTENT;
        }

      ImGui::EndTabs();
      c_app::get_state().ws_manager->get_current()->m_edit_type = ws_edit_type(edit_mode);

      ImGui::Spacing();
      ImGui::Separator();

      ImGui::Button("Undo" , ImVec2(50,24));
      ImGui::Button("Redo" , ImVec2(50,24));
      ImGui::Separator();

      ImGui::Button("RULER" , ImVec2(64,24));
      ImGui::Separator();

      if(ImGui::ToggleButton("INSP", &(c_app::get_state().show_object_inspector), ImVec2(50,24))){
          c_app::get_state().mark_viewport_change();
        }
      ImGui::Separator();

      if (astate->ws_manager->has_wss())
        ImGui::ToggleButton("GIZMO", &(astate->ws_manager->get_current()->m_gizmo->m_is_visible),
                             ImVec2(60, 24));
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
                                  astate->viewport_size(1)-(iWorkPanelYOffset +
                                                            iWorkPanelHeight)
                                  ));
  ImGui::SetNextWindowPos(ImVec2(astate->viewport_size(0) - iObjInspWidth,
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
              if (astate->ws_manager->has_wss()) astate->ws_manager->get_current()->
                  dialog_add_geom_from_file(qc_file_format::format_standart_xyz);
            }

          if (ImGui::BeginMenu("VASP")){
              if (ImGui::MenuItem("POSCAR/CONTCAR")){
                  if (astate->ws_manager->has_wss()) astate->ws_manager->get_current()->
                      dialog_add_geom_from_file(qc_file_format::format_vasp_poscar);
                }

              if (ImGui::MenuItem("OUTCAR")){

                }

              ImGui::EndMenu();
            }

          ImGui::EndMenu();
        }

      ImGui::EndMenuBar();
    }

  ImGui::Text("Workspace items:");
  ImGui::Spacing();


  ImGui::PushItemWidth(iObjInspWidth-10);

  auto cur_ws = astate->ws_manager->get_current();
  if (cur_ws){

      optional<uint16_t> ws_itm_cur = cur_ws->get_selected_item();

      int ws_itm_cur_val = -1;
      if (ws_itm_cur) ws_itm_cur_val = *ws_itm_cur;
      int ws_itm_cur_val_stored = ws_itm_cur_val;

      ImGui::PushID(1);
      ImGui::ListBox_stl("", &ws_itm_cur_val, cur_ws->m_ws_names_c, 8);
      ImGui::PopID();

      if (ws_itm_cur_val != ws_itm_cur_val_stored && ws_itm_cur_val != -1)
        cur_ws->set_selected_item(ws_itm_cur_val);

      ImGui::PopItemWidth();
      ImGui::Spacing();
      ImGui::Separator();

      if (ws_itm_cur_val != -1){
          ImGui::Text(cur_ws->get_selected()->compose_item_name().c_str());
          ImGui::Separator();
          ImGui::Spacing();
          cur_ws->get_selected()->render_ui();
        }
      else {
          if (ImGui::CollapsingHeader("Workspace settings")){
              ImGui::ColorEdit3("Background", cur_ws->m_background_color.data());
            }
          //ImGui::Separator();
        }
    }

  ImGui::End();
  ImGui::PopStyleVar(1);

}

void ui_manager_t::render_3d_viewport_context_menu(){
  app_state_t* astate = &(c_app::get_state());

  if (ImGui::BeginPopupContextVoid("3dAreaContext")){
      ImGui::BulletText("Context menu");
      ImGui::Separator();
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws){
          auto cur_it = cur_ws->get_selected();

          if (cur_it){
              //if we in edit-item-mode show node related menu
              if (cur_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM)
                cur_it->td_context_menu_edit_item();
              else cur_it->td_context_menu_edit_content();

            } else {
              ImGui::BulletText("Nothing selected");
            }
        }
      ImGui::EndPopup();
    }

}

void ui_manager_t::render_mtable_big(){
  float mendFrm = 0.85f;

  ImGui::SetNextWindowSize(ImVec2(c_app::get_state().viewport_size(0) * mendFrm ,
                                  c_app::get_state().viewport_size(1) * mendFrm));

  ImGui::SetNextWindowPos(ImVec2(c_app::get_state().viewport_size(0) * (1-mendFrm) * 0.5f,
                                 c_app::get_state().viewport_size(1) * (1-mendFrm) * 0.5f));

  ImGui::Begin("Mendeley table", nullptr,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_NoCollapse);

  ImGui::End();
}

