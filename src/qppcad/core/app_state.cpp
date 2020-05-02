#include <qppcad/core/app_state.hpp>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <data/ptable.hpp>
#include <thread>
#include <qppcad/core/hotkey_manager.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

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
      m_utility_thread_count = std::thread::hardware_concurrency();

    }

    void app_state_t::init_glapi(){
      glapi = new glapi_t();
      dp = new draw_pipeline_t();
    }

    void app_state_t::init_shaders() {

      sp_default    = shader_generators::gen_sp_default();
      sp_default_suprematic = shader_generators::gen_sp_default_suprematic();
      sp_unit_line  = shader_generators::gen_sp_unit_line();
      sp_line_mesh  = shader_generators::gen_sp_line_mesh();
      sp_mvp_ssl    = shader_generators::gen_sp_mv_screen_space_lighting();
      sp_mvap_ssl   = shader_generators::gen_sp_mva_screen_space_lighting();
      sp_fbo_quad   = shader_generators::gen_sp_fbo_quad();
      sp_unit_line_styled = shader_generators::gen_sp_unit_line_styled();
      sp_bs_sphere = shader_generators::gen_sp_bs_sphere();
      sp_buf_bs = shader_generators::gen_sp_buf_bs_sphere();
      sp_2c_cylinder = shader_generators::gen_sp_2c_cylinder();
      sp_2c_cylinder_suprematic = shader_generators::gen_sp_2c_cylinder_suprematic();

    }

    void app_state_t::init_meshes() {

      mesh_spheres.push_back(mesh_generators::sphere(m_sphere_quality, m_sphere_quality));
      mesh_cylinder = mesh_generators::cylinder_mk2(2, m_cylinder_quality, 1.0f, 1.0f);
      mesh_unit_line = mesh_generators::unit_line();
      mesh_grid_xz = mesh_generators::xz_plane(20, 0.5, 20, 0.5);
      mesh_unit_cube = mesh_generators::unit_cube();
      mesh_unit_cone = mesh_generators::cone(1.0f, 2.0f, 1, 16);
      mesh_fbo_quad = mesh_generators::quad();
      mesh_zup_quad = mesh_generators::quad_zup();
      mesh_xline_mesh = mesh_generators::cross_line_atom();
      mesh_zl_plane = mesh_generators::plane_zl();

    }

    void app_state_t::init_managers() {

      hash_reg   = std::make_unique<string_hash_register_t>();
      ws_mgr     = std::make_shared<workspace_manager_t>(this);
      py_mgr     = std::make_unique<python_manager_t>();
      hotkey_mgr = std::make_shared<hotkey_manager_t>();

    }

    void app_state_t::init_styles() {

      icons.icon_arrow_up = new QIcon("://images/outline-arrow_upward-24px.svg");
      icons.icon_arrow_down = new QIcon("://images/outline-arrow_downward-24px.svg");

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

      settings.beginGroup("general_settings");
      m_console_font_size = settings.value("console_font_size", 16).toInt();

      //loading fixtures dir
      QString fixtures_dir = settings.value("fixtures_dir").toString();
      if (fixtures_dir.size() != 0) {
          auto fixtures_line = fixtures_dir.toStdString();
          split(fixtures_line, m_fixtures_dirs, ";");
          m_fixtures_dir_is_set = true;
        }

      settings.endGroup();

      settings.beginGroup("render");
      int load_num_samples = settings.value("num_samples", 6).toInt();
      if (load_num_samples >= 0 && load_num_samples < 32) m_num_samples = load_num_samples;
      settings.endGroup();

      settings.beginGroup("periodic_table");
      int ovr_size = settings.beginReadArray("mod");
      ptable *table = ptable::get_inst();

      for(int i = 0; i < ovr_size; i++) {
          settings.setArrayIndex(i);

          bool ok_number{false};
          int number = settings.value("number").toInt(&ok_number);

          bool ok_c_r{false};
          float c_r = settings.value("c_r").toFloat(&ok_c_r);

          bool ok_c_g{false};
          float c_g = settings.value("c_g").toFloat(&ok_c_g);

          bool ok_c_b{false};
          float c_b = settings.value("c_b").toFloat(&ok_c_b);

          bool ok_rad{false};
          float rad = settings.value("r").toFloat(&ok_rad);

          bool ok_cov_rad{false};
          float cov_rad = settings.value("covrad").toFloat(&ok_cov_rad);

          if (ok_number && number > 0 && number < 100) {

              if (ok_c_r) table->arecs[number-1].m_color_jmol[0] = c_r;
              if (ok_c_g) table->arecs[number-1].m_color_jmol[1] = c_g;
              if (ok_c_b) table->arecs[number-1].m_color_jmol[2] = c_b;
              if (ok_rad) table->arecs[number-1].m_radius = rad;
              if (ok_cov_rad) table->arecs[number-1].m_covrad_slater = cov_rad;
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

      int cmd_size = settings.beginReadArray("pyconsole_history");
      QString last_pycommand = "";
      for(int i = 0; i < cmd_size; i++) {
          settings.setArrayIndex(i);
          QString cmd = settings.value("cmd").toString();
          QString cmd_trimmed = cmd.trimmed();
          if (!cmd_trimmed.isEmpty() && last_pycommand != cmd_trimmed) {
              last_pycommand = cmd;
              py_mgr->m_commands.push_back(cmd);
            }
        }

      settings.endArray();

      int recent_file_size = settings.beginReadArray("recent_files");
      for (int i = 0; i < recent_file_size; i++) {
          settings.setArrayIndex(i);
          QString rc_filename = settings.value("filename").toString();
          //size_t _loaded_fft = settings.value("ff",0).toUInt();
          std::string ff_name = settings.value("ff").toString().toStdString();
          auto ff_hsh = ws_mgr->m_bhv_mgr->get_ff_by_short_name(ff_name);
          bool _loaded_is_native = settings.value("isnat", false).toBool();
          if (ff_hsh && !_loaded_is_native)
            add_recent_file(rc_filename.toStdString(), _loaded_is_native, *ff_hsh);
          if (_loaded_is_native)
            add_recent_file(rc_filename.toStdString(), _loaded_is_native, 0);
        }

      settings.endArray();

      settings.beginGroup("cache_float");
      QStringList keys_f = settings.childKeys();
      for (auto &key : keys_f)
        m_env_float[key.toStdString()] = settings.value(key).toDouble();
      settings.endGroup();

      settings.beginGroup("cache_int");
      QStringList keys_i = settings.childKeys();
      for (auto &key : keys_i)
        m_env_int[key.toStdString()] = settings.value(key).toInt();
      settings.endGroup();

      settings.beginGroup("cache_bool");
      QStringList keys_b = settings.childKeys();
      for (auto &key : keys_b)
        m_env_bool[key.toStdString()] = settings.value(key).toBool();
      settings.endGroup();

      settings.beginGroup("cache_string");
      QStringList keys_s = settings.childKeys();
      for (auto &key : keys_s)
        m_env_str[key.toStdString()] = settings.value(key).toString().toStdString();
      settings.endGroup();

      settings.beginGroup("cache_vector");
      QStringList keys_v = settings.childKeys();
      for (auto &key : keys_v) {
          QStringList _sl = settings.value(key).toStringList();
          if (_sl.size() == 3)
            m_env_vec3[key.toStdString()] =
                vector3<float>(_sl[0].toDouble(), _sl[1].toDouble(), _sl[2].toDouble());
        }
      settings.endGroup();

      settings.beginGroup("hotkey_manager");
      hotkey_mgr->load_from_settings(settings);
      settings.endGroup();

    }

    void app_state_t::save_settings() {

      QSettings settings;
      settings.setValue("test", 68);

      // general settings
      settings.beginGroup("general_settings");
      settings.setValue("console_font_size", m_console_font_size);

      if (m_fixtures_dir_is_set) {

          // join fixtures dir
          std::string fd_joined;
          std::for_each(m_fixtures_dirs.begin(), m_fixtures_dirs.end(),
                        [&fd_joined](const std::string &piece){ fd_joined += piece + ";"; });

          settings.setValue("fixtures_dir", QString::fromStdString(fd_joined));

        }
      // end of general settings

      settings.endGroup();

      settings.beginGroup("render");
      settings.setValue("num_samples", m_num_samples);
      settings.endGroup();

      settings.beginGroup("periodic_table");
      ptable *table = ptable::get_inst();

      settings.beginWriteArray("mod");
      int i = 0;

      for (auto &rec : table->arecs)
        if (rec.m_redefined) {

            settings.setArrayIndex(i);
            settings.setValue("number", int(rec.m_number));
            settings.setValue("c_r", double(rec.m_color_jmol[0]));
            settings.setValue("c_g", double(rec.m_color_jmol[1]));
            settings.setValue("c_b", double(rec.m_color_jmol[2]));
            settings.setValue("r", double(rec.m_radius));
            settings.setValue("covrad", double(rec.m_covrad_slater));
            i+=1;

          }

      settings.endArray();
      settings.endGroup();

      settings.beginGroup("mesh_generators");
      settings.setValue("sphere_res", m_sphere_quality);
      settings.setValue("cylinder_res", m_cylinder_quality);
      settings.setValue("default_camera_projection", static_cast<int>(m_default_cam_proj));
      settings.endGroup();

      settings.beginWriteArray("pyconsole_history");

      for (int q = 0; q < py_mgr->m_commands.size(); q++) {
          settings.setArrayIndex(q);
          settings.setValue("cmd", py_mgr->m_commands[q]);
        }

      settings.endArray();

      settings.beginWriteArray("recent_files");
      for (int q = 0; q < static_cast<int>(m_recent_files.size()); q++) {

          settings.setArrayIndex(q);
          settings.setValue("filename", QString::fromStdString(m_recent_files[q].m_file_name));
          std::string ff_name =
              ws_mgr->m_bhv_mgr->get_ff_short_name(m_recent_files[q].m_ff_id);
          settings.setValue("ff", QString::fromStdString(ff_name));
          settings.setValue("isnat", m_recent_files[q].m_native);

        }

      settings.endArray();

      settings.beginGroup("cache_float");
      for (auto &rec : m_env_float)
        settings.setValue(QString::fromStdString(rec.first), rec.second);
      settings.endGroup();

      settings.beginGroup("cache_int");
      for (auto &rec : m_env_int)
        settings.setValue(QString::fromStdString(rec.first), rec.second);
      settings.endGroup();

      settings.beginGroup("cache_bool");
      for (auto &rec : m_env_bool)
        settings.setValue(QString::fromStdString(rec.first), rec.second);
      settings.endGroup();

      settings.beginGroup("cache_string");
      for (auto &rec : m_env_str)
        settings.setValue(QString::fromStdString(rec.first), QString::fromStdString(rec.second));
      settings.endGroup();

      settings.beginGroup("cache_vector");
      for (auto &rec : m_env_vec3) {

          QStringList vecl;
          vecl.reserve(3);
          for (size_t q = 0; q < 3; q++) vecl.push_back(QString("%1").arg(rec.second[q]));
          settings.setValue(QString::fromStdString(rec.first), vecl);

        }

      settings.endGroup();

      settings.beginGroup("hotkey_manager");
      hotkey_mgr->save_to_settings(settings);
      settings.endGroup();

    }

    void app_state_t::log(std::string logstr, bool flush) {

      std::setlocale(LC_ALL, "C");
      std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::string ts( ctime( &t) );
      std::cout << fmt::format("[{}] {}\n", ts.substr( 0, ts.length() -1  ), logstr);
      if (flush) std::cout << std::flush;

    }

    void app_state_t::pylog(std::string logstr) {

      if (py_mgr) py_mgr->m_output_buffer += "\n" + logstr;

    }

    void app_state_t::add_recent_file(const std::string &file_name,
                                      const bool is_native,
                                      const size_t ff_id) {

      QFileInfo file_info(QString::fromStdString(file_name));
      m_last_dir =  file_info.absoluteDir().path();
      tlog("M_LAST_DIR= {}", m_last_dir.toStdString());

      if (m_recent_files.size() >= max_recent_files)
        m_recent_files.erase(m_recent_files.begin(),
                             m_recent_files.begin() + 1 +
                             (m_recent_files.size() - max_recent_files));

      for (auto it = m_recent_files.begin(); it != m_recent_files.end(); ++it) {
          if ((*it).m_file_name.find(file_name) != std::string::npos) {
              auto x = std::move(*it);
              m_recent_files.erase(it);
              m_recent_files.insert(m_recent_files.end(), std::move(x));
              return;
            }
        }

      //log(fmt::format("RECENT FILES ADD: {} {} {}", file_name, bhv_id, is_native));
      if (QFileInfo::exists(QString::fromStdString(file_name)) &&
          QFileInfo(QString::fromStdString(file_name)).isFile())
        m_recent_files.emplace_back(file_name, ff_id, is_native);
    }

    void app_state_t::init_fixtures() {

      // try to use ${USER}/.qppcad
      if (!m_fixtures_dir_is_set) {

          QString home_path = QDir::homePath();
          QString qc_home_path = QString("%1/.qppcad").arg(home_path);
          QDir si_qc_home_path(qc_home_path);
          m_fixtures_dirs.push_back(si_qc_home_path.path().toStdString());
          m_fixtures_dir_is_set = true;

        }

      if (!m_fixtures_dir_is_set) {
          tlog("Fixture dir is not set!");
          return;
        } else {
          tlog("Fixtures dir size = {}", m_fixtures_dirs.size());
          ws_mgr->m_bhv_mgr->load_fixtures_from_path(m_fixtures_dirs);
        }

    }

    app_state_t* app_state_t::g_inst = nullptr;

  } // namespace qpp

} // namespace cad
