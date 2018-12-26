#include <qppcad/app_state.hpp>
#include <QSettings>
#include <data/ptable.hpp>

namespace qpp {

  namespace cad {

    void app_state_t::init_inst() {
      g_inst = new app_state_t;
    }

    app_state_t *app_state_t::get_inst() {
      return g_inst;
    }

    app_state_t::app_state_t() {
      astate_evd = new app_state_event_disp_t;
    }

    void app_state_t::init_glapi(){
      glapi = new glapi_t();
      dp = new draw_pipeline_t();
    }

    void app_state_t::init_shaders() {

      sp_default    = shader_generators::gen_sp_default();
      sp_unit_line  = shader_generators::gen_sp_unit_line();
      sp_line_mesh  = shader_generators::gen_sp_line_mesh();
      sp_mvp_ssl    = shader_generators::gen_sp_mv_screen_space_lighting();
      sp_fbo_quad   = shader_generators::gen_sp_fbo_quad();
      sp_unit_line_styled = shader_generators::gen_sp_unit_line_styled();
      sp_bs_sphere = shader_generators::gen_sp_bs_sphere();
      sp_buf_bs = shader_generators::gen_sp_buf_bs_sphere();
      sp_2c_cylinder = shader_generators::gen_sp_2c_cylinder();

    }

    void app_state_t::init_meshes() {

      mesh_spheres.push_back(mesh_generators::sphere(m_sphere_quality, m_sphere_quality));
      mesh_cylinder = mesh_generators::cylinder_mk2(2, m_cylinder_quality, 1.0f, 1.0f);
      mesh_unit_line     = mesh_generators::unit_line();
      mesh_grid_xz        = mesh_generators::xz_plane(20, 0.5, 20, 0.5);
      mesh_unit_cube     = mesh_generators::unit_cube();
      mesh_unit_cone     = mesh_generators::cone(1.0f, 2.0f, 1, 16);
      mesh_fbo_quad      = mesh_generators::quad();
      mesh_zup_quad      = mesh_generators::quad_zup();
      mesh_xline_mesh    = mesh_generators::cross_line_atom();

    }

    void app_state_t::init_managers() {
      ws_manager   = std::make_shared<workspace_manager_t>(this);
      //ws_manager->init_default();

      py_manager = std::make_unique<python_manager_t>();
//      auto result = py_manager->sys.attr("copyright").cast<std::string>();
//      std::cout<< "IM HERE " << result << std::endl;
    }

    void app_state_t::make_viewport_dirty() {
      m_viewport_dirty = true;
    }

    void app_state_t::disable_app() {
      m_disable_app = true;
    }

    void app_state_t::enable_app() {
      m_disable_app = false;
    }


    bool app_state_t::is_viewport_dirty() {
      return m_viewport_dirty;
    }

    void app_state_t::cleanup_viewport() {

      if (m_viewport_dirty) {
          m_viewport_dirty = false;
        }

    }

    void app_state_t::load_settings() {

      QSettings settings;

      settings.beginGroup("periodic_table");
      int ovr_size = settings.beginReadArray("mod");
      ptable *table = ptable::get_inst();

      for(int i=0; i < ovr_size; i++) {
         settings.setArrayIndex(i);
          int number = settings.value("number").toInt();
          float c_r = settings.value("c_r").toFloat();
          float c_g = settings.value("c_g").toFloat();
          float c_b = settings.value("c_b").toFloat();
          float rad = settings.value("r").toFloat();
          if (number > 0 && number < 100) {
              table->arecs[number-1].m_color_jmol[0] = c_r;
              table->arecs[number-1].m_color_jmol[1] = c_g;
              table->arecs[number-1].m_color_jmol[2] = c_b;
              table->arecs[number-1].m_radius = rad;
              table->arecs[number-1].m_redefined = true;
            }
        }

      settings.endArray();
      settings.endGroup();

      settings.beginGroup("mesh_generators");
      m_sphere_quality = settings.value("sphere_res", 18).toInt();
      m_cylinder_quality = settings.value("cylinder_res", 16).toInt();
      int m_loaded_default_cam_proj = settings.value("default_camera_projection", 0).toInt();
      m_default_cam_proj = static_cast<cam_proj_t>(m_loaded_default_cam_proj);

      settings.endGroup();

    }

    void app_state_t::save_settings() {
      QSettings settings;
      settings.setValue("test", 68);

      settings.beginGroup("periodic_table");
      ptable *table = ptable::get_inst();
      settings.beginWriteArray("mod");
      int i = 0;
      for (auto &rec : table->arecs)
        if (rec.m_redefined) {
            settings.setArrayIndex(i);
            settings.setValue("number", rec.m_number);
            settings.setValue("c_r", double(rec.m_color_jmol[0]));
            settings.setValue("c_g", double(rec.m_color_jmol[1]));
            settings.setValue("c_b", double(rec.m_color_jmol[2]));
            settings.setValue("r", double(rec.m_radius));
            i+=1;
          }
      settings.endArray();
      settings.endGroup();

      settings.beginGroup("mesh_generators");
      settings.setValue("sphere_res", m_sphere_quality);
      settings.setValue("cylinder_res", m_cylinder_quality);
      settings.setValue("default_camera_projection", static_cast<int>(m_default_cam_proj));
      settings.endGroup();
    }

    void app_state_t::log(std::string logstr, bool flush) {
      std::setlocale(LC_ALL, "C");
      std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::string ts( ctime( &t) );
      std::cout << fmt::format("[{}] {}\n", ts.substr( 0, ts.length() -1  ), logstr);
      if (flush) std::cout << std::flush;
    }

    app_state_t* app_state_t::g_inst = nullptr;

  }

}
