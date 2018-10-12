#include <qppcad/camera.hpp>
#include <qppcad/app.hpp>

using namespace qpp;
using namespace qpp::cad;

camera_t::camera_t () {
  reset_camera();
}

void camera_t::orthogonalize_gs () {
  m_view_dir = m_look_at - m_view_point;
  m_stored_dist = m_view_dir.norm();
  vector3<float> view_dir_new = m_view_dir.normalized();

  if (m_view_dir.isMuchSmallerThan(camera_t::norm_eps)) m_look_up = vector3<float>(0.0, 0.0, 1.0);
  else {
      m_look_up -= view_dir_new * (view_dir_new.dot(m_look_up));
      m_look_up.normalize();
    }

  m_right = -1.0*(m_look_up.cross(m_view_dir));
  if (m_right.isMuchSmallerThan(camera_t::norm_eps)) m_right = vector3<float>(1.0, 0.0, 0.0);
  else m_right.normalize();
}

void camera_t::rotate_camera_around_origin (const matrix3<float> &mat_rot,
                                            const vector3<float> origin) {
  translate_camera(-origin);
  m_view_point = mat_rot * m_view_point;
  m_look_at    = mat_rot * m_look_at;
  m_look_up    = mat_rot * m_look_up;
  translate_camera(origin);
  orthogonalize_gs();
}

void camera_t::rotate_camera_around_axis (const float angle, const vector3<float> axis) {
  Eigen::Matrix<float, 3, 3> mr = Eigen::Matrix<float, 3, 3>::Identity();
  mr = Eigen::AngleAxisf(angle, axis);
  rotate_camera_around_origin(mr, m_look_at);
}

void camera_t::rotate_camera_orbit_yaw (const float yaw) {
  rotate_camera_around_axis(yaw, vector3<float>(0.0, 0.0, 1.0));
}

void camera_t::rotate_camera_orbit_pitch (const float pitch) {
  rotate_camera_around_axis(pitch, m_right);
}

void camera_t::translate_camera_forward (const float amount) {
  vector3<float> view_dir_new = m_view_dir.normalized();
  m_view_point += view_dir_new * amount;
  m_look_at    += view_dir_new * amount;
}

void camera_t::translate_camera_right (const float amount) {
  vector3<float> _tmp_tr = m_right * amount;
  m_view_point += _tmp_tr;
  m_look_at += _tmp_tr;
}

void camera_t::translate_camera_up (const float amount) {
  vector3<float> _tmp_tr = m_look_up * amount;
  m_view_point += _tmp_tr;
  m_look_at += _tmp_tr;
}

void camera_t::translate_camera (const vector3<float> shift) {
  m_view_point += shift;
  m_look_at    += shift;
}

void camera_t::reset_camera () {
  //
  m_view_point = vector3<float>(0.0, 9.0, 0.0);
  m_look_at    = vector3<float>(0.0, 0.0, 0.0);
  m_look_up    = vector3<float>(0.0, 0.0, 1.0);
  orthogonalize_gs();

  cur_proj = cam_proj_type::CAMERA_PROJ_PERSP;
}

void camera_t::update_camera () {

  app_state_t* astate = &(c_app::get_state());

  float x_dt = astate->mouse_x - astate->mouse_x_old;
  float y_dt = astate->mouse_y - astate->mouse_y_old;

  if (m_move_camera){
      float move_right = -x_dt / camera_t::nav_div_step_translation;
      float move_up = y_dt / camera_t::nav_div_step_translation;

      if (fabs(move_right) > camera_t::nav_thresh) {
          translate_camera_right(move_right);
          astate->make_viewport_dirty();
        }

      if (fabs(move_up) > camera_t::nav_thresh) {
          translate_camera_up(move_up);
          astate->make_viewport_dirty();
        }

    }

  if (m_rotate_camera){
      float rot_angle_x = y_dt / camera_t::nav_div_step_rotation;
      float rot_angle_y = x_dt / camera_t::nav_div_step_rotation;

      if (fabs(rot_angle_y) > camera_t::nav_thresh) {
          rotate_camera_orbit_yaw(rot_angle_y);
          astate->make_viewport_dirty();
        }

      if (fabs(rot_angle_x) > camera_t::nav_thresh) {
          rotate_camera_orbit_pitch(rot_angle_x);
          astate->make_viewport_dirty();
        }

    }

  m_mat_view = look_at<float>(m_view_point, m_look_at, m_look_up);

  if (cur_proj == cam_proj_type::CAMERA_PROJ_PERSP)
    m_mat_proj = perspective<float>(m_fov,
                                    astate->viewport_size_c(0) / astate->viewport_size_c(1),
                                    m_znear_persp, m_zfar_persp);
  else {
      float width   = astate->viewport_size_c(0);
      float height  = astate->viewport_size_c(1);
      float x_scale = 1.0f;
      float y_scale = 1.0f;

      if (width > height) {
          x_scale = width / (height );
          y_scale = 1;
        }

      else {
          x_scale = 1;
          y_scale = height / (width );
        }

      float left   = - x_scale * (m_ortho_scale);
      float right  =   x_scale * (m_ortho_scale);
      float bottom = -  (m_ortho_scale);
      float top    =   (m_ortho_scale);
      //std::cout<<"ortho_scale"<<m_ortho_scale<<std::endl;
      m_mat_proj = ortho<float>(left, right, bottom, top , m_znear_ortho, m_zfar_ortho);
    }

  m_proj_view = m_mat_proj *  m_mat_view ;
  m_view_inv_tr = mat4_to_mat3<float>((m_mat_view.transpose()).inverse());
  m3_proj_view = mat4_to_mat3<float>(m_proj_view);

}

void camera_t::update_camera_zoom (const float dist) {

  app_state_t* astate = &(c_app::get_state());

  if (cur_proj == cam_proj_type::CAMERA_PROJ_PERSP){

      vector3<float> m_view_dir_n = - m_view_point + m_look_at;
      float f_dist = m_view_dir_n.norm();
      m_stored_dist = f_dist;
      m_view_dir_n.normalize();
      float f_dist_delta = dist * m_mouse_whell_camera_step;
      //bool bCanZoom = true;
      if (f_dist + f_dist_delta > m_mouse_zoom_min_distance || f_dist_delta < 0.0f)
        m_view_point += m_view_dir_n * f_dist_delta;

    } else {
      m_ortho_scale += dist;
      m_ortho_scale = clamp(m_ortho_scale, 1.0f, 150.0f);
    }

  astate->make_viewport_dirty();

}

void camera_t::update_camera_translation (const bool move_camera) {
  m_move_camera = move_camera;
}

void camera_t::update_camera_rotation (bool rotate_camera) {
  m_rotate_camera = rotate_camera;
}

void camera_t::set_projection (cam_proj_type _proj_to_set) {

  if (cur_proj != _proj_to_set){
      reset_camera();
      cur_proj = _proj_to_set;
    }

}

float camera_t::distance(const vector3<float> &point) {
  return (m_mat_view * vector4<float>(point(0), point(1), point(2), 1.0f)).norm();
}

vector3<float> camera_t::unproject (const float _x, const float _y) {
  //app_state_t* astate = &(c_app::get_state());
  matrix4<float> mat_mvp_inv = (m_mat_proj * m_mat_view).inverse();
  vector4<float> invec4(_x, _y, 0.5f, 1.0f);
  vector4<float> rvec4 = vector4<float>::Zero();

  /*if (cur_proj == app_camera_proj_type::CAMERA_PROJ_ORTHO){
      vector4<float> v_near_up = mat_mvp_inv * vector4<float>(_x, _y, m_znear_ortho, 1.0f);
      vector4<float> v_far_up = mat_mvp_inv * vector4<float>(_x, _y, m_zfar_ortho, 1.0f);
      rvec4 =   v_near_up;
    } else*/
  rvec4 = mat_mvp_inv * invec4;

  rvec4(3) = 1.0f / rvec4(3);
  rvec4(0) = rvec4(0) * rvec4(3);
  rvec4(1) = rvec4(1) * rvec4(3);
  rvec4(2) = rvec4(2) * rvec4(3);
  return vector3<float>(rvec4(0), rvec4(1), rvec4(2));

}

std::optional<vector2<float> > camera_t::project (const vector3<float> point) {

  app_state_t* astate = &(c_app::get_state());

  vector4<float> tmpv = m_proj_view * vector4<float>(point[0], point[1], point[2], 1.0f);

  if (std::fabs(tmpv[3]) < 0.00001f) return std::nullopt;

  tmpv[0] /= tmpv[3];
  tmpv[1] /= tmpv[3];
  tmpv[2] /= tmpv[3];

  tmpv[0] = (tmpv[0] + 1.0f) / 2.0f;
  tmpv[1] = (-tmpv[1] + 1.0f) / 2.0f;
  tmpv[2] = (tmpv[2] + 1.0f) / 2.0f;

  tmpv[0] = tmpv[0] * astate->viewport_size_c[0];
  tmpv[1] = (tmpv[1] * astate->viewport_size_c[1]) + astate->viewport_xy_c[1];

  return std::optional<vector2<float> >(vector2<float>(tmpv[0], tmpv[1]));

}
