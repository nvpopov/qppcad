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

namespace qpp::cad {

  ///
  /// \brief The app_task_type enum
  ///
  enum app_task_type {
    TASK_WORKSPACE_EDITOR = 0,
    TASK_NODE_EDITOR = 1,
    TASK_MENDELEY_TABLE = 2
  };

  // vote candidates
  const int DISABLE_MOUSE_CONTROL_IN_WORKSPACE = 0;

  ///
  /// \brief The app_state class
  ///
  class app_state_t {
    public:
      draw_pipeline_t*               dp;

      shader_program_t*              default_program;
      shader_program_t*              unit_line_program;
      shader_program_t*              line_mesh_program;
      shader_program_t*              mvp_ssl_program;
      shader_program_t*              fbo_quad_program;

      shared_ptr<workspace_manager_t>                           ws_manager;
      shared_ptr<ui_manager_t>                                  ui_manager;
      shared_ptr<file_dialog_manager_t>                         fd_manager;
      unique_ptr<frame_buffer_t<frame_buffer_opengl_provider> > frame_buffer;
      unique_ptr<keyboard_manager_t>                            kb_manager;
      camera_t*  camera;

      float mouse_x;
      float mouse_y;
      float mouse_x_old;
      float mouse_y_old;
      float mouse_x_ws_frame;
      float mouse_y_ws_frame;

      app_task_type cur_task;
      //app_edit_type cur_edit_type;

      // sphere mesh with differnet resolutions
      std::vector<mesh_t*> _sph_meshes;
      mesh_t *cylinder_mesh;
      mesh_t *unit_line;
      mesh_t *trm;
      mesh_t *gridXZ;
      mesh_t *unit_cube;
      mesh_t *unit_cone;
      mesh_t *fbo_quad;
      mesh_t *xline_mesh;

      int FPS;

      vector2<float> viewport_xy;
      vector2<float> viewport_size;
      vector2<float> viewport_xy_c;
      vector2<float> viewport_size_c;

      vector3<float> light_pos;
      vector3<float> light_color;
      vector3<float> light_pos_tr;

      bool mouse_lb_pressed;
      bool disable_mouse_camera_control;

      vote_pool_t<uint32_t> config_vote_pool;

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


      void make_viewport_dirty(){
        m_viewport_dirty = true;
      }

      void update_viewport_cache(){
        //      std::cout << fmt::format("s {} {} | xy {} {}\n",
        //                               viewport_size_c[0],
        //          viewport_size_c[1],
        //          viewport_xy_c[0],
        //          viewport_xy_c[1]);
        viewport_xy_c = viewport_xy;
        viewport_xy_c(1) = ui_manager->iWorkPanelHeight + ui_manager->iWorkPanelYOffset - 64;
        viewport_size_c = viewport_size;
        viewport_size_c(1) = viewport_size(1) -
                             (ui_manager->iWorkPanelHeight + ui_manager->iWorkPanelYOffset);
        if (show_object_inspector) viewport_size_c[0] -= ui_manager->iObjInspWidth;
      }

      ///
      /// \brief update_mouse_coord
      /// \param _mcx
      /// \param _mcy
      ///
      void update_mouse_coord(const float _mcx, const float _mcy){
        //3d area frame
        //std::cout<< fmt::format("b{}\n", mouse_in_3d_area);
        mouse_x_ws_frame = mouse_x;
        mouse_y_ws_frame = mouse_y - ui_manager->iWorkPanelHeight
                           - ui_manager->iWorkPanelYOffset;


        mouse_in_3d_area = mouse_x_ws_frame > 0 &&
                           mouse_x_ws_frame < viewport_size_c(0)&&
                           mouse_y_ws_frame > 0 &&
                           mouse_y_ws_frame < viewport_size_c(1);

        mouse_x_ws_frame = (mouse_x_ws_frame / viewport_size_c(0)-0.5)*2.0;
        mouse_y_ws_frame = (mouse_y_ws_frame / viewport_size_c(1)-0.5)*-2.0;

        mouse_x_old = mouse_x;
        mouse_y_old = mouse_y;
        mouse_x = _mcx;
        mouse_y = _mcy;

      }

      //TODO: implement via vote pool
      void mark_viewport_change(){
        viewport_changed = true;
      }

      ///
      /// \brief update
      ///
      void update(float delta_time){
        update_viewport_cache();

        if (viewport_changed){
            frame_buffer->resize(viewport_size_c(0),
                                 viewport_size_c(1));
            viewport_changed = false;
          }

        if (camera != nullptr){
            camera->update_camera();
            light_pos_tr = mat4_to_mat3<float>(camera->m_mat_view) * light_pos;
          }

        auto cur_ws = ws_manager->get_current();
        if (cur_ws) cur_ws->update(delta_time);

      }

      ///
      /// \brief app_state
      ///
      app_state_t(){

        FPS = 60;

        cur_task = app_task_type::TASK_WORKSPACE_EDITOR;
        //cur_edit_type = app_edit_type::EDIT_WS_ITEM_CONTENT;

        //      show_axis                    = true;
        //      show_grid                    = false;
        //      debug_show_tws_tree          = false;
        //      debug_show_selection_ray     = false;
        //      show_object_inspector        = true;
        //      mouse_lb_pressed             = false;
        //      disable_mouse_camera_control = false;
        //      show_console                 = false;

        light_pos    = vector3<float>(0, 1.0f, 1.0f);
        light_pos_tr = vector3<float>(0, 0, 0);

        camera = nullptr;

        dp = new draw_pipeline_t();

        //default meshes
        _sph_meshes.push_back(mesh_generators::sphere(15, 15));
        cylinder_mesh = mesh_generators::cylinder_mk2(2, 10, 1.0f, 1.0f);
        unit_line     = mesh_generators::unit_line();
        gridXZ        = mesh_generators::xz_plane(20, 0.5, 20, 0.5);
        unit_cube     = mesh_generators::unit_cube();
        unit_cone     = mesh_generators::cone(1.0f, 2.0f, 1, 16);
        fbo_quad      = mesh_generators::quad();
        xline_mesh    = mesh_generators::cross_line_atom();

        default_program    = shader_generators::gen_default_program();
        unit_line_program  = shader_generators::gen_unit_line_program();
        line_mesh_program  = shader_generators::gen_line_mesh_program();
        mvp_ssl_program    = shader_generators::gen_mv_screen_space_lighting_program();
        fbo_quad_program   = shader_generators::gen_fbo_quad_program();

        kb_manager   = make_unique<keyboard_manager_t>();
        frame_buffer = make_unique<frame_buffer_t<frame_buffer_opengl_provider> >();
        ws_manager   = make_shared<workspace_manager_t>(this);
        ws_manager->init_default();
        ui_manager   = make_shared<ui_manager_t>(this);
        fd_manager   = make_shared<file_dialog_manager_t>();

        update_viewport_cache();
      }

  };
}

#endif
