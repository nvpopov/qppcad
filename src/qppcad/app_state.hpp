#ifndef QPP_APP_STATE_H
#define QPP_APP_STATE_H

#include <qppcad/qppcad.hpp>
#include <qppcad/render/shader_generators.hpp>
#include <qppcad/render/mesh_generators.hpp>
#include <qppcad/render/camera.hpp>
#include <qppcad/render/glapi.hpp>
#include <qppcad/render/draw_pipeline.hpp>
#include <qppcad/workspace.hpp>
#include <qppcad/app_state_event_disp.hpp>
#include <qppcad/python/python_manager.hpp>
#include <qppcad/string_hash_register.hpp>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QString>
#include <QPalette>
#include <qppcad/ui/size_guide.hpp>

namespace qpp {

  namespace cad {

    class app_state_t;

    class recent_file_record_t {

      public:

        std::string m_file_name;
        size_t m_ff_id;
        bool m_native;
        recent_file_record_t(){}
        recent_file_record_t(const std::string _file_name ,
                             const size_t _ff,
                             const bool _native) :
          m_file_name(_file_name), m_ff_id(_ff), m_native(_native){}

    };

    class icon_repository_t {
      public:
        QIcon *icon_arrow_down;
        QIcon *icon_arrow_up;
    };

    class app_state_t {

      private:

        static app_state_t *g_inst;

      public:

        static void init_inst();
        static app_state_t* get_inst();

        app_state_t();
        void init_glapi();
        void init_shaders();
        void init_meshes();
        void init_managers();
        void init_styles();
        void make_viewport_dirty();
        void disable_app();
        void enable_app();
        bool is_viewport_dirty();
        void cleanup_viewport();

        void load_settings();
        void save_settings();

        void log(std::string logstr,
                 bool flush = true);

        void pylog(std::string logstr);

        template <typename... Args>
        void tlog(const char *format, const Args & ... args) {

          std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
          std::string ts( ctime( &t) );
          std::string pf = fmt::format("[{}] ", ts.substr( 0, ts.length() - 1));
          std::string body = fmt::vformat(format, fmt::make_format_args(args...));

          std::cout << pf << body << "\n" << std::flush;

        }

        void add_recent_file(const std::string file_name,
                             const bool is_native,
                             const size_t bhv_id);

        app_state_event_disp_t *astate_evd;
        glapi_t *glapi;
        camera_t *camera{nullptr};
        draw_pipeline_t *dp;
        size_guide_t size_guide;

        std::unordered_map<std::string, float> m_cache_float;
        std::unordered_map<std::string, int> m_cache_int;
        std::unordered_map<std::string, bool> m_cache_bool;
        std::unordered_map<std::string, std::string> m_cache_string;
        std::unordered_map<std::string, vector3<float>> m_cache_vector;

        shader_program_t *sp_default;
        shader_program_t *sp_unit_line;
        shader_program_t *sp_unit_line_styled;
        shader_program_t *sp_line_mesh;
        shader_program_t *sp_mvp_ssl;
        shader_program_t *sp_mvap_ssl;
        shader_program_t *sp_fbo_quad;
        shader_program_t *sp_bs_sphere;
        shader_program_t *sp_buf_bs;
        shader_program_t *sp_2c_cylinder;

        std::vector<mesh_t*> mesh_spheres;
        mesh_t *mesh_cylinder;
        mesh_t *mesh_unit_line;
        mesh_t *mesh_grid_xz;
        mesh_t *mesh_unit_cube;
        mesh_t *mesh_unit_cone;
        mesh_t *mesh_fbo_quad;
        mesh_t *mesh_zup_quad;
        mesh_t *mesh_zl_plane;
        mesh_t *mesh_xline_mesh;

        std::shared_ptr<workspace_manager_t> ws_mgr;
        std::unique_ptr<python_manager_t> py_mgr;
        std::unique_ptr<string_hash_register_t> hash_reg;

        std::vector<recent_file_record_t> m_recent_files;
        // //       std::map<std::string, color_map_t> m_color_maps;
        QString m_last_dir;
        QString m_font_name;
        QString m_screen_shots_dir{"."};
        QString m_spatial_suffix{"Å"};
        QString m_degree_suffix{"°"};
        QPalette m_app_palette;
        QPalette m_bgfg_light_pal;
        QPalette m_bg_light_pal;
        QPalette m_bg_embwnd_pal;
        QPalette m_bgfg_embwnd_pal;

        icon_repository_t icons;

        vector2<float> viewport_xy;
        vector2<float> viewport_size;

        int64_t m_last_frame_time_cpu{0};
        int64_t m_last_frame_time_gpu{0};

        int m_num_samples{6};
        int m_console_font_size{16};

        bool m_show_debug_frame_stats{false};

        //Settings begin
        int m_sphere_quality{18};
        int m_cylinder_quality{16};
        cam_proj_t m_default_cam_proj{cam_proj_t::proj_ortho};
        //Settings end

        float m_middle_mb_translate_mode{1.25f};
        float mouse_x_dc;
        float mouse_y_dc;
        float mouse_x_dc_old;
        float mouse_y_dc_old;
        float mouse_x;
        float mouse_y;
        float mouse_x_old;
        float mouse_y_old;
        float mouse_distance_pp{0.0f};
        bool mouse_lb_pressed{false};
        bool mouse_rb_pressed{false};
        bool mouse_md_pressed{false};
        bool is_mouse_moving{false};

        bool m_show_axis{true};
        bool m_show_grid{false};
        bool m_debug_show_tws_tree{false};
        bool m_debug_show_sel_deque{false};
        bool m_debug_show_selection_ray{false};
        bool m_show_object_inspector{true};
        bool m_show_console{false};
        bool m_mouse_in_3d_area{false};
        bool m_viewport_changed{false};
        bool m_transform_pdb_atom_names{true};

        bool m_disable_app{false};
        bool m_viewport_dirty{true};

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
