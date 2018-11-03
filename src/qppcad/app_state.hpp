#ifndef QPP_APP_STATE_H
#define QPP_APP_STATE_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

#include <imgui.h>
#include <imgui_freetype.h>

#include <mathf/math.hpp>
#include <qppcad/draw_pipeline.hpp>
#include <qppcad/workspace.hpp>
#include <qppcad/mesh.hpp>
#include <qppcad/shader_program.hpp>
#include <qppcad/ui_manager.hpp>
#include <qppcad/gl_math.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/gizmo.hpp>
#include <qppcad/file_dialog.hpp>
#include <qppcad/vote_pool.hpp>
#include <qppcad/frame_buffer.hpp>
#include <qppcad/mesh_generators.hpp>
#include <qppcad/shader_generators.hpp>
#include <qppcad/keyboard_event.hpp>
#include <qppcad/simple_query.hpp>

namespace qpp {

  namespace cad {

    /// \brief The app_task_type enum
    enum app_task_type {
      TASK_WORKSPACE_EDITOR = 0,
      TASK_NODE_EDITOR = 1,
      TASK_MENDELEY_TABLE = 2
    };

    enum app_render_mode {
      direct,
      buffered_legacy,
      buffered_multi_sampling
    };

    // vote candidates
    const int DISABLE_MOUSE_CONTROL_IN_WORKSPACE = 0;
    const int NEED_TO_REDRAW_WS_VIEWPORT         = 1;

    ///
    /// \brief The app_state class
    ///
    class app_state_t {

      public:

        ImFont* fontn;
        ImFont* fontb;

        draw_pipeline_t*               dp;

        shader_program_t*              default_program;
        shader_program_t*              unit_line_program;
        shader_program_t*              unit_line_styled_program;
        shader_program_t*              line_mesh_program;
        shader_program_t*              mvp_ssl_program;
        shader_program_t*              fbo_quad_program;
        shader_program_t*              bs_sphere_program;
        shader_program_t*              buf_bs_program;

        std::shared_ptr<workspace_manager_t>                           ws_manager;
        std::shared_ptr<ui_manager_t>                                  ui_manager;
        std::shared_ptr<file_dialog_manager_t>                         fd_manager;
        std::unique_ptr<frame_buffer_t<frame_buffer_opengl_provider> > frame_buffer;
        std::unique_ptr<keyboard_manager_t>                            kb_manager;
        std::unique_ptr<simple_query_manager_t>                        sq_manager;

        camera_t*  camera;
        vote_pool_t<uint32_t> config_vote_pool;

        std::vector<mesh_t*> _sph_meshes;
        mesh_t *cylinder_mesh;
        mesh_t *unit_line;
        mesh_t *trm;
        mesh_t *gridXZ;
        mesh_t *unit_cube;
        mesh_t *unit_cone;
        mesh_t *fbo_quad;
        mesh_t *zup_quad;
        mesh_t *xline_mesh;


        vector2<float> viewport_xy;
        vector2<float> viewport_size;
        vector2<float> viewport_xy_c;
        vector2<float> viewport_size_c;

        vector3<float> light_pos{0, 1.0f, 1.0f};
        vector3<float> light_color; // @unused
        vector3<float> light_pos_tr{0, 0, 0};

        int c_max_texture_buffer_size;
        int current_fps{60};
        int max_fps{60};
        uint m_viewport_ms_level{0};

        float mouse_x;
        float mouse_y;
        float mouse_x_old;
        float mouse_y_old;
        float mouse_x_ws_frame;
        float mouse_y_ws_frame;

        app_task_type cur_task{app_task_type::TASK_WORKSPACE_EDITOR};
        app_render_mode m_render_mode{app_render_mode::buffered_multi_sampling};

        bool mouse_lb_pressed{false};
        bool disable_mouse_camera_control;

        bool show_axis{true};
        bool show_grid{false};
        bool debug_show_tws_tree{false};
        bool debug_show_selection_ray{false};
        bool show_object_inspector{true};
        bool show_console{false};
        bool mouse_in_3d_area{false};
        bool viewport_changed{false};
        bool m_transform_pdb_atom_names{true};

        bool m_realtime{false};
        bool m_viewport_dirty{true};
        bool m_workspace_changed{true};

        void write_app_settings ();

        void read_app_settings ();

        void make_viewport_dirty ();

        void update_viewport_cache ();

        /// \brief update_mouse_coord
        /// \param _mcx
        /// \param _mcy
        void update_mouse_coord (const float _mcx, const float _mcy);

        //TODO: implement via vote pool
        void mark_viewport_change ();

        /// \brief update
        void update (float delta_time);

        /// \brief app_state
        app_state_t ();

    };
  }
}

#endif
