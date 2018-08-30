#ifndef QPP_UI_MANAGER_H
#define QPP_UI_MANAGER_H
#pragma GCC diagnostic ignored "-Wformat-security"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <qppcad/imgui_addons.hpp>
#include <io/strfun.hpp>
#include <cstdlib>

namespace qpp{
  class ui_manager_t{
  public:
    int iWorkPanelHeight;
    int iWorkPanelYOffset;
    int iObjInspWidth;
    ui_manager_t();
    void render_ui();
    void render_main_menu();
    void render_work_panel();
    void render_task_panel();
    void render_ws_tabs();
    void render_object_inspector();
    void render_mtable_big();
  };
}
#endif
