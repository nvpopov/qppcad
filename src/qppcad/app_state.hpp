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

        draw_pipeline_t*               dp;

        shader_program_t*              default_program;
        shader_program_t*              unit_line_program;
        shader_program_t*              unit_line_styled_program;
        shader_program_t*              line_mesh_program;
        shader_program_t*              mvp_ssl_program;
        shader_program_t*              fbo_quad_program;

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

        bool m_realtime{false};
        bool m_viewport_dirty{true};
        bool m_workspace_changed{true};

        void write_app_settings () {

        }

        void read_app_settings () {

        }

        void make_viewport_dirty () {
          m_viewport_dirty = true;
          m_workspace_changed = true;
        }

        void update_viewport_cache () {

          viewport_xy_c = viewport_xy;
          viewport_xy_c[1] = ui_manager->work_panel_height + ui_manager->work_panel_yoffset ;
          viewport_size_c = viewport_size;
          viewport_size_c[1] = viewport_size[1] -
                               (ui_manager->work_panel_height + ui_manager->work_panel_yoffset);

          if (show_object_inspector) viewport_size_c[0] -= ui_manager->obj_insp_width;

        }

        /// \brief update_mouse_coord
        /// \param _mcx
        /// \param _mcy
        void update_mouse_coord (const float _mcx, const float _mcy) {

          //3d area frame
          mouse_x_ws_frame = mouse_x;
          mouse_y_ws_frame = mouse_y - ui_manager->work_panel_height
                             - ui_manager->work_panel_yoffset;


          mouse_in_3d_area = mouse_x_ws_frame > 0 &&
                             mouse_x_ws_frame < viewport_size_c(0)&&
                             mouse_y_ws_frame > 0 &&
                             mouse_y_ws_frame < viewport_size_c(1);

          mouse_x_ws_frame = (mouse_x_ws_frame / viewport_size_c(0)-0.5)*2.0;
          mouse_y_ws_frame = (0.5 - mouse_y_ws_frame / viewport_size_c(1))*2.0;

          mouse_x_old = mouse_x;
          mouse_y_old = mouse_y;
          mouse_x = _mcx;
          mouse_y = _mcy;

        }

        //TODO: implement via vote pool
        void mark_viewport_change () {
          viewport_changed = true;
        }

        /// \brief update
        void update (float delta_time) {

          update_viewport_cache();

          if (viewport_changed){
              frame_buffer->resize(viewport_size_c(0), viewport_size_c(1));
              viewport_changed = false;
              make_viewport_dirty();
            }

          if (mouse_lb_pressed) make_viewport_dirty();

          if (camera != nullptr){
              camera->update_camera();
              light_pos_tr = mat4_to_mat3<float>(camera->m_mat_view) * light_pos;
            }

          if (!ws_manager->has_wss()) {
              if (show_object_inspector) {
                  viewport_changed = true;
                }
              show_object_inspector = false;
            }

          auto cur_ws = ws_manager->get_current();
          if (cur_ws) cur_ws->update(delta_time);

        }

        /// \brief app_state
        app_state_t () {

          camera = nullptr;

          dp = new draw_pipeline_t();

          //default meshes
          _sph_meshes.push_back(mesh_generators::sphere(18, 18));
          cylinder_mesh = mesh_generators::cylinder_mk2(2, 14, 1.0f, 1.0f);
          unit_line     = mesh_generators::unit_line();
          gridXZ        = mesh_generators::xz_plane(20, 0.5, 20, 0.5);
          unit_cube     = mesh_generators::unit_cube();
          unit_cone     = mesh_generators::cone(1.0f, 2.0f, 1, 16);
          fbo_quad      = mesh_generators::quad();
          zup_quad      = mesh_generators::quad_zup();
          xline_mesh    = mesh_generators::cross_line_atom();

          default_program    = shader_generators::gen_default_program();
          unit_line_program  = shader_generators::gen_unit_line_program();
          line_mesh_program  = shader_generators::gen_line_mesh_program();
          mvp_ssl_program    = shader_generators::gen_mv_screen_space_lighting_program();
          fbo_quad_program   = shader_generators::gen_fbo_quad_program();
          unit_line_styled_program = shader_generators::gen_unit_line_styled_program();

          kb_manager   = std::make_unique<keyboard_manager_t>();
          //frame_buffer = std::make_unique<frame_buffer_t<frame_buffer_opengl_provider> >(false);
          ws_manager   = std::make_shared<workspace_manager_t>(this);
          ws_manager->init_default();
          ui_manager   = std::make_shared<ui_manager_t>(this);
          fd_manager   = std::make_shared<file_dialog_manager_t>();
          sq_manager   = std::make_unique<simple_query_manager_t>();

          make_viewport_dirty();
          update_viewport_cache();
        }

    };
  }
}

#endif
