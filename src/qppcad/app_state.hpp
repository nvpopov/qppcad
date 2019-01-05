#ifndef QPP_APP_STATE_H
#define QPP_APP_STATE_H

#include <qppcad/qppcad.hpp>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QString>
#include <qppcad/shader_generators.hpp>
#include <qppcad/mesh_generators.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/glapi.hpp>
#include <qppcad/draw_pipeline.hpp>
#include <qppcad/workspace.hpp>
#include <qppcad/app_state_event_disp.hpp>
#include <qppcad/python_manager.hpp>

namespace qpp {

  namespace cad {

    class app_state_t;

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
        void make_viewport_dirty();
        void disable_app();
        void enable_app();
        bool is_viewport_dirty();
        void cleanup_viewport();

        void load_settings();
        void save_settings();

        void log(std::string logstr, bool flush = true);
        void pylog(std::string logstr);

        app_state_event_disp_t *astate_evd;

        glapi_t *glapi;
        camera_t *camera{nullptr};

        draw_pipeline_t *dp;

        shader_program_t *sp_default;
        shader_program_t *sp_unit_line;
        shader_program_t *sp_unit_line_styled;
        shader_program_t *sp_line_mesh;
        shader_program_t *sp_mvp_ssl;
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
        mesh_t *mesh_xline_mesh;

        std::shared_ptr<workspace_manager_t> ws_manager;
        std::unique_ptr<python_manager_t> py_manager;
        // //       std::map<std::string, color_map_t> m_color_maps;

        vector2<float> viewport_xy;
        vector2<float> viewport_size;
        vector2<float> viewport_xy_c;
        vector2<float> viewport_size_c;

        int64_t m_last_frame_time_cpu{0};
        int64_t m_last_frame_time_gpu{0};

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

  }

}

#endif
