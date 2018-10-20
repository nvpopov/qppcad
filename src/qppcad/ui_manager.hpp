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
#include <qppcad/super_cell_widget.hpp>

namespace qpp {

  namespace cad {

    class app_state_t;

    class ui_manager_t{

      public:

        vector3<float> explicit_translation;
        std::unique_ptr<console_widget_t> console_widget;
        std::unique_ptr<super_cell_widget_t> sc_widget;
        int work_panel_height{38};
        int work_panel_yoffset{28};
        int obj_insp_width{350};
        uint32_t m_rename_ws_id;
        char s_rename_workspace_name[60];
        bool show_rename_workspace_dialog{false};

        ui_manager_t(app_state_t *astate);

        void toggle_edit_mode();
        void setup_style();
        void render_ui();
        void render_main_menu();
        void render_work_panel();
        void render_object_inspector();
        void render_3d_viewport_context_menu();
        void render_mtable_big();
        void render_start_page();

    };

  }

}

#endif
