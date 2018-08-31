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
#include <geom/lace3d.hpp>

namespace qpp {

  ///
  /// \brief The app_task_type enum
  ///
  enum app_task_type {
    TASK_WORKSPACE_EDITOR = 0,
    TASK_NODE_EDITOR = 1,
    TASK_MENDELEY_TABLE = 2
  };


  ///
  /// \brief The app_state class
  ///
  class app_state_t {
  public:
    draw_pipeline_t*     dp;
    shader_program_t*    default_program;
    shader_program_t*    unit_line_program;
    shader_program_t*    line_mesh_program;
    shader_program_t*    mvp_ssl_program;
    workspace_manager_t* workspace_manager;
    ui_manager_t*        ui_manager;
    camera_t*            camera;

    double MouseX;
    double MouseY;
    double MouseOldX;
    double MouseOldY;

    app_task_type cur_task;
    //app_edit_type cur_edit_type;

    // sphere mesh with differnet resolutions
    std::vector<mesh_t*> _sph_meshes;
    mesh_t* cylinder_mesh;
    mesh_t* unit_line;
    mesh_t* trm;
    mesh_t* gridXZ;
    mesh_t* unit_cube;

    int FPS;

    int wWidth;
    int wHeight;

    float fAtomRadiusScaleFactor;
    float fBondScaleFactor;

    vector2<float> vViewportXY;
    vector2<float> vViewportWidthHeight;

    vector3<float> vLigthPos;
    vector3<float> vLightColor;
    vector3<float> vLightPosTr;


    bool bDrawAxis;
    bool bDrawGrid;
    bool bDebugDrawRTree;
    bool bDebugDrawSelectionRay;

    ///
    /// \brief update_mouse_coord
    /// \param _mcx
    /// \param _mcy
    ///
    void update_mouse_coord(const double _mcx, const double _mcy){
      MouseX = _mcx; MouseY = _mcy;}

    ///
    /// \brief update
    ///
    void update(){
      if (camera != nullptr){
          camera->update_camera();
          vLightPosTr = mat4_to_mat3<float>(camera->mView) * vLigthPos;
        }
    }

    ///
    /// \brief app_state
    ///
    app_state_t(){

      FPS = 60;

      cur_task = app_task_type::TASK_WORKSPACE_EDITOR;
      //cur_edit_type = app_edit_type::EDIT_WS_ITEM_CONTENT;

      bDrawAxis = true;
      bDrawGrid = false;
      bDebugDrawRTree = false;
      bDebugDrawSelectionRay = false;

      vLigthPos = vector3<float>(0, 25.0, 25.0);
      vLightPosTr = vector3<float>(0, 0, 0);

      wWidth  = 600;
      wHeight = 600;
      fAtomRadiusScaleFactor = 0.3f;
      fBondScaleFactor = 0.09f;

      camera = nullptr;

      dp = new draw_pipeline_t();

      //default meshes
      _sph_meshes.push_back(mesh_t::generate_sphere_mesh(15, 15));
      cylinder_mesh = mesh_t::generate_cylinder_whole(12, 4);
      unit_line     = mesh_t::generate_unit_line();
      gridXZ        = mesh_t::generate_xz_plane(20, 0.5, 20, 0.5);
      unit_cube     = mesh_t::generate_unit_cube();

      default_program       = gen_default_program();
      unit_line_program = gen_unit_line_program();
      line_mesh_program   = gen_line_mesh_program();
      mvp_ssl_program  = gen_mv_screen_space_lighting_program();

      workspace_manager = new workspace_manager_t();
      workspace_manager->init_default_workspace();
      ui_manager = new ui_manager_t();
    }

  };
}

#endif
