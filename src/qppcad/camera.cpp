#include <qppcad/camera.hpp>
#include <qppcad/app.hpp>

using namespace qpp;

camera_t::camera_t(){
  m_fov = 50.0;
  m_znear_persp = 0.1f;
  m_zfar_persp  = 600;
  m_znear_ortho = -100;
  m_zfar_ortho  = 100;

  m_mouse_whell_camera_step = 2.0f;
  m_mouse_zoom_min_distance = 4.0f;
  m_rotate_camera           = false;
  m_move_camera             = false;

  reset_camera();
}

void camera_t::orthogonalize_gs(){
  m_view_dir = m_look_at - m_view_point;
  m_stored_dist = m_view_dir.norm();
  vector3<float> view_dir_new = m_view_dir.normalized();

  if (m_view_dir.isMuchSmallerThan(camera_t::norm_eps)) m_look_up = vector3<float>(0.0, 0.0, 1.0);
  else {
      m_look_up -= view_dir_new * (view_dir_new.dot(m_look_up));
      m_look_up.normalize();
    }

  m_right = -1.0*(m_look_up.cross(m_view_dir));
  if (m_right.isMuchSmallerThan(camera_t::norm_eps)) m_right = vector3<float>(-1.0, 0.0, 0.0);
  else m_right.normalize();
}

void camera_t::rotate_camera_around_origin(matrix3<float> mat_rot, vector3<float> origin){
  translate_camera(-origin);
  m_view_point = mat_rot * m_view_point;
  m_look_at    = mat_rot * m_look_at;
  m_look_up    = mat_rot * m_look_up;
  translate_camera(origin);
  orthogonalize_gs();
}

void camera_t::rotate_camera_around_axis(const float angle, const vector3<float> axis){
  Eigen::Matrix<float, 3, 3> mr = Eigen::Matrix<float, 3, 3>::Identity();
  mr = Eigen::AngleAxisf(angle, axis);
  rotate_camera_around_origin(mr, m_look_at);
}

void camera_t::rotate_camera_orbit_yaw(const float yaw){
  rotate_camera_around_axis(yaw, vector3<float>(0.0, 0.0, 1.0));
}

void camera_t::rotate_camera_orbit_pitch(const float pitch){
  rotate_camera_around_axis(pitch, m_right);
}

void camera_t::translate_camera_forward(const float amount){
  vector3<float> view_dir_new = m_view_dir.normalized();
  m_view_point += view_dir_new * amount;
  m_look_at    += view_dir_new * amount;
}

void camera_t::translate_camera_right(const float amount){
  vector3<float> _tmp_tr = m_right * amount;
  m_view_point += _tmp_tr;
  m_look_at += _tmp_tr;
}

void camera_t::translate_camera_up(const float amount){
  vector3<float> _tmp_tr = m_look_up * amount;
  m_view_point += _tmp_tr;
  m_look_at += _tmp_tr;
}

void camera_t::translate_camera(vector3<float> shift){
  m_view_point += shift;
  m_look_at    += shift;
  orthogonalize_gs();
}

void camera_t::reset_camera(){
  //
  m_view_point = vector3<float>(0.0, 9.0, 0.0);
  m_look_at    = vector3<float>(0.0, 0.0, 0.0);
  m_look_up    = vector3<float>(0.0, 0.0, 1.0);
  orthogonalize_gs();

  cur_proj = app_camera_proj_type::CAMERA_PROJ_PERSP;
}

void camera_t::update_camera(){
  app_state_t* astate = &(c_app::get_state());

  float x_dt = astate->mouse_x - astate->mouse_x_old;
  float y_dt = astate->mouse_y - astate->mouse_y_old;


  if (m_move_camera && !astate->disable_mouse_camera_control){
      float move_right = -x_dt / camera_t::nav_div_step_translation;
      float move_up = y_dt / camera_t::nav_div_step_translation;
      if (fabs(move_right) > camera_t::nav_thresh) translate_camera_right(move_right);
      if (fabs(move_up) > camera_t::nav_thresh) translate_camera_up(move_up);
    }

  if (m_rotate_camera && !astate->disable_mouse_camera_control){
      float rot_angle_x = y_dt / camera_t::nav_div_step_rotation;
      float rot_angle_y = x_dt / camera_t::nav_div_step_rotation;
      if (fabs(rot_angle_y) > camera_t::nav_thresh) rotate_camera_orbit_yaw(rot_angle_y);
      if (fabs(rot_angle_x) > camera_t::nav_thresh) rotate_camera_orbit_pitch(rot_angle_x);
    }


  m_mat_view = look_at<float>(m_view_point, m_look_at, m_look_up);

  float _viewport_w = 0.0f;
  if (!astate->show_object_inspector) _viewport_w = astate->ui_manager->iObjInspWidth;

  if (cur_proj == app_camera_proj_type::CAMERA_PROJ_PERSP)
    m_mat_proj = perspective<float>(m_fov,
                                    (astate->vViewportWidthHeight(0) + _viewport_w)/
                                     astate->vViewportWidthHeight(1),
                                     m_znear_persp, m_zfar_persp);
  else {
      float a = astate->vViewportWidthHeight(0) + _viewport_w;
      a /= astate->vViewportWidthHeight(1);
      float r = (m_view_point-m_look_at).norm();
      if (a < 1.0f)
        m_mat_proj = ortho<float>(-r*a, r*a, -r, r, m_znear_ortho, m_zfar_ortho);
      else
        m_mat_proj = ortho<float>(-r, r, -r/a, r/a, m_znear_ortho, m_zfar_ortho);
    }

  m_view_proj = m_mat_proj *  m_mat_view ;
  m_view_inv_tr = mat4_to_mat3<float>((m_mat_view.transpose()).inverse());
}

void camera_t::update_camera_zoom(const float dist){
  vector3<float> m_view_dir_n = - m_view_point + m_look_at;
  float f_dist = m_view_dir_n.norm();
  m_stored_dist = f_dist;
  m_view_dir_n.normalize();
  float f_dist_delta = dist * m_mouse_whell_camera_step;
  //bool bCanZoom = true;
  if (f_dist + f_dist_delta > m_mouse_zoom_min_distance || f_dist_delta < 0.0f)
    m_view_point += m_view_dir_n * f_dist_delta;
}

void camera_t::update_camera_translation(const bool move_camera){
  m_move_camera = move_camera;
}

void camera_t::update_camera_rotation(bool rotate_camera){
  m_rotate_camera = rotate_camera;
}

void camera_t::set_projection(app_camera_proj_type _proj_to_set){
  if (cur_proj != _proj_to_set){
      reset_camera();
      cur_proj = _proj_to_set;
    }
}

vector3<float> camera_t::unproject(const float _x, const float _y){
  app_state_t* astate = &(c_app::get_state());
  matrix4<float> mMVP_Inv = (m_mat_proj * m_mat_view).inverse();
  vector4<float> invec4(_x, _y, 0.5, 1.0);
  vector4<float> rvec4 = mMVP_Inv * invec4;
  rvec4(3) = 1.0f / rvec4(3);
  rvec4(0) = rvec4(0) * rvec4(3);
  rvec4(1) = rvec4(1) * rvec4(3);
  rvec4(2) = rvec4(2) * rvec4(3);
  return vector3<float>(rvec4(0), rvec4(1), rvec4(2));
}
