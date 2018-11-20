#ifndef QPP_APP_STATE_H
#define QPP_APP_STATE_H

#include <qppcad/qppcad.hpp>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <qppcad/shader_generators.hpp>
#include <qppcad/mesh_generators.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/glapi.hpp>
#include <qppcad/draw_pipeline.hpp>
#include <qppcad/workspace.hpp>
#include <qppcad/app_state_event_disp.hpp>

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
        void log(std::string logstr, bool flush = true);

        app_state_event_disp_t *astate_evd;

        glapi_t *glapi;
        camera_t *camera;

        draw_pipeline_t*               dp;

        shader_program_t*              default_program;
        shader_program_t*              unit_line_program;
        shader_program_t*              unit_line_styled_program;
        shader_program_t*              line_mesh_program;
        shader_program_t*              mvp_ssl_program;
        shader_program_t*              fbo_quad_program;
        shader_program_t*              bs_sphere_program;
        shader_program_t*              buf_bs_program;

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

        std::shared_ptr<workspace_manager_t>                           ws_manager;

        // //       std::map<std::string, color_map_t> m_color_maps;

        vector2<float> viewport_xy;
        vector2<float> viewport_size;
        vector2<float> viewport_xy_c;
        vector2<float> viewport_size_c;

        //        vector3<float> light_pos{0, 1.0f, 1.0f};
        //        vector3<float> light_color; // @unused
        //        vector3<float> light_pos_tr{0, 0, 0};

        //        int c_max_texture_buffer_size;
        //        int current_fps{60};
        //        int max_fps{60};
        //        uint m_viewport_ms_level{0};

        float mouse_x_dc;
        float mouse_y_dc;
        float mouse_x;
        float mouse_y;
        float mouse_x_old;
        float mouse_y_old;
        float mouse_distance_pp{0.0f};
        bool mouse_lb_pressed{false};
        bool mouse_rb_pressed{false};
        bool is_mouse_moving{false};
        //        float mouse_x_ws_frame;
        //        float mouse_y_ws_frame;

        ////        app_task_type cur_task{app_task_type::TASK_WORKSPACE_EDITOR};
        ////        app_render_mode m_render_mode{app_render_mode::buffered_multi_sampling};

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

        bool m_viewport_dirty{true};
        //        bool m_trigger_3d_popup{false};
        //        int  m_trigger_3d_popup_counter{0};
        //        int  m_trigger_3d_popup_counter_max{65};

        //        bool m_realtime{false};
        //
        //        bool m_workspace_changed{true};

        //        void write_app_settings ();

        //        void read_app_settings ();



        //        void update_viewport_cache ();

        //        /// \brief update_mouse_coord
        //        /// \param _mcx
        //        /// \param _mcy
        //        void update_mouse_coord (const float _mcx, const float _mcy);

        //        //TODO: implement via vote pool
        //        void mark_viewport_change ();

        //        /// \brief update
        //        void update (float delta_time);

        //        /// \brief app_state
        //        app_state_t ();
    };

  }

}

#endif
