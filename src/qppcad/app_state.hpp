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
#include <geom/lace3d.hpp>
#include <qppcad/vote_pool.hpp>

namespace qpp {

  ///
  /// \brief The app_task_type enum
  ///
  enum app_task_type {
    TASK_WORKSPACE_EDITOR = 0,
    TASK_NODE_EDITOR = 1,
    TASK_MENDELEY_TABLE = 2
  };

  // vote candidats
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

    std::shared_ptr<workspace_manager_t>   ws_manager;
    std::shared_ptr<ui_manager_t>          ui_manager;
    std::shared_ptr<file_dialog_manager_t> fd_manager;

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

    int FPS;

    int wWidth;
    int wHeight;

    float atom_radius_scale_factor;
    float bond_radius_scale_factor;

    vector2<float> vViewportXY;
    vector2<float> vViewportWidthHeight;

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

    ///
    /// \brief update_mouse_coord
    /// \param _mcx
    /// \param _mcy
    ///
    void update_mouse_coord(const float _mcx, const float _mcy){
      //3d area frame
      mouse_x_ws_frame = mouse_x;
      mouse_y_ws_frame = mouse_y - ui_manager->iWorkPanelHeight
                        - ui_manager->iWorkPanelYOffset;

      float vw_delta = 0.0f;
      if (!show_object_inspector) vw_delta = ui_manager->iObjInspWidth;

      mouse_in_3d_area = mouse_x_ws_frame > 0 &&
                         mouse_x_ws_frame < (vViewportWidthHeight(0) + vw_delta) &&
                         mouse_y_ws_frame > 0 &&
                         mouse_y_ws_frame < vViewportWidthHeight(1);

      mouse_x_ws_frame = (mouse_x_ws_frame / vViewportWidthHeight(0)-0.5)*2.0;
      mouse_y_ws_frame = (mouse_y_ws_frame / vViewportWidthHeight(1)-0.5)*-2.0;

      mouse_x_old = mouse_x;
      mouse_y_old = mouse_y;
      mouse_x = _mcx;
      mouse_y = _mcy;
      //std::cout << fmt::format("msx xo {} yo {} x {} y {}\n", mouse_x_old, mouse_y_old, mouse_x, mouse_y);
    }

    ///
    /// \brief update
    ///
    void update(float delta_time){

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

      wWidth  = 600;
      wHeight = 600;
      atom_radius_scale_factor = 0.3f;
      bond_radius_scale_factor = 0.09f;

      camera = nullptr;

      dp = new draw_pipeline_t();

      //default meshes
      _sph_meshes.push_back(mesh_t::generate_sphere_mesh(15, 15));
      cylinder_mesh = mesh_t::generate_cylinder_mk2(2, 10, 1.0f, 1.0f);
      unit_line     = mesh_t::generate_unit_line();
      gridXZ        = mesh_t::generate_xz_plane(20, 0.5, 20, 0.5);
      unit_cube     = mesh_t::generate_unit_cube();
      unit_cone     = mesh_t::generate_cone_mesh(1.0f, 2.0f, 1, 16);

      default_program       = gen_default_program();
      unit_line_program = gen_unit_line_program();
      line_mesh_program   = gen_line_mesh_program();
      mvp_ssl_program  = gen_mv_screen_space_lighting_program();

      ws_manager = std::make_shared<workspace_manager_t>();
      ws_manager->init_default();
      ui_manager = std::make_shared<ui_manager_t>();
      ui_manager->setup_style();

      fd_manager = std::make_shared<file_dialog_manager_t>();
    }

  };
}

#endif
