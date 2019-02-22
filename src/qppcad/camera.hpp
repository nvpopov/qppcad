#ifndef QPP_CAMERA_H
#define QPP_CAMERA_H
#include <geom/lace3d.hpp>
#include <qppcad/gl_math.hpp>

namespace qpp {

  namespace cad {

    enum cam_target_view_t : int {
      tv_x_axis,
      tv_y_axis,
      tv_z_axis,
      tv_cart_center,
      tv_a_axis,
      tv_b_axis,
      tv_c_axis,
      tv_a_star_axis,
      tv_b_star_axis,
      tv_c_star_axis
    };

    enum cam_proj_t {
      proj_ortho,
      proj_persp
    };

    class camera_t{

      public:

        static constexpr float norm_eps = 0.00001f;
        static constexpr float nav_thresh = 0.00001f;
        static constexpr int   nav_div_step_translation = 13;
        static constexpr int   nav_div_step_rotation = 43;

        vector3<float> m_view_point;
        vector3<float> m_view_dir;
        vector3<float> m_look_at;
        vector3<float> m_look_up;
        vector3<float> m_right;
        vector3<float> m_forward;

        matrix4<float> m_mat_view;
        matrix4<float> m_mat_proj;
        matrix4<float> m_proj_view;
        matrix3<float> m_view_inv_tr;
        matrix3<float> m3_proj_view;

        bool m_rotate_camera{false};
        bool m_move_camera{false};
        bool m_rotate_over{false};

        float m_mouse_whell_camera_step{2.0f};
        float m_mouse_zoom_min_distance{4.0f};

        float m_ortho_scale{10.0f};
        float m_fov{50.0};
        float m_znear_persp{0.1f};
        float m_zfar_persp{800};
        float m_znear_ortho{-1000};
        float m_zfar_ortho{1000};
        float m_stored_dist;

        cam_proj_t m_cur_proj{cam_proj_t::proj_ortho};

        camera_t();
        void orthogonalize_gs();
        void rotate_camera_around_origin(const matrix3<float> &mat_rot,
                                         const vector3<float> origin);
        void rotate_camera_around_axis(const float angle, const vector3<float> axis);
        void rotate_camera_orbit_yaw(const float yaw);
        void rotate_camera_orbit_pitch(const float pitch);
        void rotate_camera_orbit_roll(const float roll);
        void translate_camera_forward(const float amount);
        void translate_camera_right(const float amount);
        void translate_camera_up(const float amount);
        void translate_camera(const vector3<float> shift);

        void copy_from_camera(const camera_t &another);

        void reset_camera();
        void update_camera();
        void update_camera_zoom(const float dist);
        void update_camera_translation(const bool move_camera);
        void update_camera_rotation(const bool rotate_camera);

        void set_projection(cam_proj_t _proj_to_set);
        float distance(const vector3<float> &point);
        vector3<float> unproject(const float x, const float y, const float _z = 0.5f);
        std::optional<vector2<float> > project(const vector3<float> point);
    };

  }
}
#endif
