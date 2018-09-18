#ifndef QPP_UI_MANAGER_H
#define QPP_UI_MANAGER_H
#pragma GCC diagnostic ignored "-Wformat-security"

#include <qppcad/qppcad.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <qppcad/imgui_addons.hpp>
#include <qppcad/console.hpp>

namespace qpp{
  class ui_manager_t{
  public:
    uint32_t m_rename_ws_id;
    char s_rename_workspace_name[60];
    bool show_rename_workspace_dialog;
    int iWorkPanelHeight;
    int iWorkPanelYOffset;
    int iObjInspWidth;
    unique_ptr<console_widget_t> console_widget;
    ui_manager_t();
    void setup_style();
    void render_ui();
    void render_main_menu();
    void render_work_panel();
    void render_task_panel();
    void render_ws_tabs();
    void render_object_inspector();
    void render_3d_viewport_context_menu();
    void render_mtable_big();
  };
}

#endif
