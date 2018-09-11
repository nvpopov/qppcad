#ifndef QPP_CAMERA_H
#define QPP_CAMERA_H
#include <geom/lace3d.hpp>
#include <qppcad/gl_math.hpp>

namespace qpp{
  enum app_camera_proj_type {
    CAMERA_PROJ_ORTHO,
    CAMERA_PROJ_PERSP
  };

  class camera_t{
  public:
    static constexpr float norm_eps = 0.00001f;
    static constexpr float nav_thresh = 0.01f;
    static constexpr int   nav_div_step_translation = 100;
    static constexpr int   nav_div_step_rotation = 100;

    vector3<float> m_view_point;
    vector3<float> m_view_dir;
    vector3<float> m_look_at;
    vector3<float> m_look_up;
    vector3<float> m_right;

    matrix4<float> m_mat_view;
    matrix4<float> m_mat_proj;
    matrix4<float> m_view_proj;
    matrix3<float> m_view_inv_tr;

    bool m_rotate_camera;
    bool m_move_camera;

    /// how much to move forward on mouse wheel
    float m_mouse_whell_camera_step;
    float m_mouse_zoom_min_distance;

    float m_fov;
    float m_znear_persp;
    float m_zfar_persp;
    float m_znear_ortho;
    float m_zfar_ortho;
    float m_stored_dist;

    app_camera_proj_type cur_proj;

    camera_t();
    void orthogonalize_gs();
    void rotate_camera_around_origin(matrix3<float> mat_rot, vector3<float> origin);
    void rotate_camera_around_axis(const float angle, const vector3<float> axis);
    void rotate_camera_orbit_yaw(const float yaw);
    void rotate_camera_orbit_pitch(const float pitch);
    void translate_camera_forward(const float amount);
    void translate_camera_right(const float amount);
    void translate_camera_up(const float amount);
    void translate_camera(vector3<float> shift);

    void reset_camera();
    void update_camera();
    void update_camera_zoom(const float dist);
    void update_camera_translation(const bool move_camera);
    void update_camera_rotation(const bool rotate_camera);

    void set_projection(app_camera_proj_type _proj_to_set);
    vector3<float> unproject(const float x, const float y);
  };
}

#endif
