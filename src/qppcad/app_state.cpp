#include <qppcad/app_state.hpp>

namespace qpp {

  namespace cad {

    void app_state_t::init_inst() {
      g_inst = new app_state_t;
    }

    app_state_t *app_state_t::get_inst() {
      return g_inst;
    }

    void app_state_t::init_glapi(){
      glapi = new glapi_t();
      dp = new draw_pipeline_t();
    }

    void app_state_t::init_shaders() {

      default_program    = shader_generators::gen_default_program();
      unit_line_program  = shader_generators::gen_unit_line_program();
      line_mesh_program  = shader_generators::gen_line_mesh_program();
      mvp_ssl_program    = shader_generators::gen_mv_screen_space_lighting_program();
      fbo_quad_program   = shader_generators::gen_fbo_quad_program();
      unit_line_styled_program = shader_generators::gen_unit_line_styled_program();
      bs_sphere_program = shader_generators::gen_bs_sphere_program();
      buf_bs_program = shader_generators::gen_buf_bs_sphere_program();


    }

    void app_state_t::init_meshes() {

      _sph_meshes.push_back(mesh_generators::sphere(18, 18));
      cylinder_mesh = mesh_generators::cylinder_mk2(2, 14, 1.0f, 1.0f);
      unit_line     = mesh_generators::unit_line();
      gridXZ        = mesh_generators::xz_plane(20, 0.5, 20, 0.5);
      unit_cube     = mesh_generators::unit_cube();
      unit_cone     = mesh_generators::cone(1.0f, 2.0f, 1, 16);
      fbo_quad      = mesh_generators::quad();
      zup_quad      = mesh_generators::quad_zup();
      xline_mesh    = mesh_generators::cross_line_atom();

    }

    void app_state_t::init_managers() {
      ws_manager   = std::make_shared<workspace_manager_t>(this);
      ws_manager->init_default();
    }

    void app_state_t::make_viewport_dirty() {
      m_viewport_dirty = true;
    }

    //    void app_state_t::write_app_settings() {

    //    }

    //    void app_state_t::read_app_settings() {

    //    }

    //    void app_state_t::make_viewport_dirty() {
    //      m_viewport_dirty = true;
    //      m_workspace_changed = true;
    //    }

    //    void app_state_t::update_viewport_cache() {

    //      viewport_xy_c = viewport_xy;
    //      viewport_xy_c[1] = ui_manager->work_panel_height + ui_manager->work_panel_yoffset ;
    //      viewport_size_c = viewport_size;
    //      viewport_size_c[1] = viewport_size[1] -
    //                           (ui_manager->work_panel_height + ui_manager->work_panel_yoffset);

    //      if (show_object_inspector) viewport_size_c[0] -= ui_manager->obj_insp_width;

    //    }



    //    void app_state_t::update_mouse_coord(const float _mcx, const float _mcy) {

    //      //3d area frame
    //      mouse_x_ws_frame = mouse_x;
    //      mouse_y_ws_frame = mouse_y - ui_manager->work_panel_height
    //                         - ui_manager->work_panel_yoffset;


    //      mouse_in_3d_area = mouse_x_ws_frame > 0 &&
    //                         mouse_x_ws_frame < viewport_size_c(0)&&
    //                         mouse_y_ws_frame > 0 &&
    //                         mouse_y_ws_frame < viewport_size_c(1);

    //      mouse_x_ws_frame = (mouse_x_ws_frame / viewport_size_c(0)-0.5)*2.0;
    //      mouse_y_ws_frame = (0.5 - mouse_y_ws_frame / viewport_size_c(1))*2.0;

    //      mouse_x_old = mouse_x;
    //      mouse_y_old = mouse_y;
    //      mouse_x = _mcx;
    //      mouse_y = _mcy;

    //      double delta = std::sqrt(
    //                       std::pow(mouse_x_old - mouse_x, 2) + std::pow(mouse_y_old - mouse_y, 2));

    //      if (delta < 2.5 && mouse_in_3d_area)
    //        m_trigger_3d_popup_counter += 1;
    //      else {
    //          m_trigger_3d_popup = 0;
    //          m_trigger_3d_popup = false;
    //        }

    //      if (m_trigger_3d_popup_counter > m_trigger_3d_popup_counter_max) {
    //          m_trigger_3d_popup = true;
    //          m_trigger_3d_popup_counter = 0;
    //        }
    //    }

    //    void app_state_t::mark_viewport_change() {
    //      viewport_changed = true;
    //    }

    //    void app_state_t::update(float delta_time) {

    //      update_viewport_cache();

    //      if (viewport_changed){
    //          frame_buffer->resize(viewport_size_c(0), viewport_size_c(1));
    //          viewport_changed = false;
    //          make_viewport_dirty();
    //        }

    //      if (mouse_lb_pressed) make_viewport_dirty();

    //      if (camera != nullptr){
    //          camera->update_camera();
    //          light_pos_tr = mat4_to_mat3<float>(camera->m_mat_view) * light_pos;
    //        }

    //      if (!ws_manager->has_wss()) {
    //          if (show_object_inspector) {
    //              viewport_changed = true;
    //            }
    //          show_object_inspector = false;
    //        }

    //      auto cur_ws = ws_manager->get_current();
    //      if (cur_ws) cur_ws->update(delta_time);

    //    }

    //    app_state_t::app_state_t() {

    //      camera = nullptr;

    //      glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &c_max_texture_buffer_size);

    //      dp = new draw_pipeline_t();

    //      m_color_maps["black_and_white"] = {{0.0f, clr_black}, {1.0, clr_white}};
    //      m_color_maps["semapwhore"] = {{0.0f, clr_green}, {0.5, clr_yellow}, {1.0, clr_red}};

    //      //default meshes
    //      _sph_meshes.push_back(mesh_generators::sphere(18, 18));
    //      cylinder_mesh = mesh_generators::cylinder_mk2(2, 14, 1.0f, 1.0f);
    //      unit_line     = mesh_generators::unit_line();
    //      gridXZ        = mesh_generators::xz_plane(20, 0.5, 20, 0.5);
    //      unit_cube     = mesh_generators::unit_cube();
    //      unit_cone     = mesh_generators::cone(1.0f, 2.0f, 1, 16);
    //      fbo_quad      = mesh_generators::quad();
    //      zup_quad      = mesh_generators::quad_zup();
    //      xline_mesh    = mesh_generators::cross_line_atom();



    //      kb_manager   = std::make_unique<keyboard_manager_t>();
    //      //frame_buffer = std::make_unique<frame_buffer_t<frame_buffer_opengl_provider> >(false);
    //
    //      ui_manager   = std::make_shared<ui_manager_t>(this);
    //      fd_manager   = std::make_shared<file_dialog_manager_t>();
    //      sq_manager   = std::make_unique<simple_query_manager_t>();


    //      make_viewport_dirty();
    //      update_viewport_cache();
    //    }

    app_state_t* app_state_t::g_inst = nullptr;

  }

}
