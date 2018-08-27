#include <qppcad/camera.hpp>
#include <qppcad/app.hpp>

using namespace qpp;

camera::camera(){
  fFOV = 60.0;
  fZNearPersp = 0.1;
  fZFarPersp  = 600;
  fZNearOrtho = -100;
  fZFarOrtho  = 100;

  fMouseWhellCameraStep = 2.0f;
  fMouseZoomMinDistance = 4.0f;
  bRotateCamera = false;
  bMoveCamera = false;

  reset_camera();
}

void camera::orthogonalize_gs(){
  vViewDir = vLookAt - vViewPoint;
  fStoredDist = vViewDir.norm();
  vector3<float> vViewDir_n = vViewDir.normalized();

  if (vViewDir.isMuchSmallerThan(camera::fNormEps))
    vLookUp = vector3<float>(0.0, 0.0, 1.0);
  else {
      vLookUp -= vViewDir_n * (vViewDir_n.dot(vLookUp));
      vLookUp.normalize();
    }

  vRight = -1.0*(vLookUp.cross(vViewDir));
  if (vRight.isMuchSmallerThan(camera::fNormEps))
    vRight = vector3<float>(-1.0, 0.0, 0.0);
  else vRight.normalize();
}

void camera::rotate_camera_around_origin(matrix3<float> mRot,
                                         vector3<float> vOrigin){
  translate_camera(-vOrigin);
  vViewPoint = mRot * vViewPoint;
  vLookAt    = mRot * vLookAt;
  vLookUp    = mRot * vLookUp;
  translate_camera(vOrigin);
  orthogonalize_gs();
}

void camera::rotate_camera_around_axis(const float fAngle,
                                       vector3<float> vAxis){
  Eigen::Matrix<float, 3, 3> mr = Eigen::Matrix<float, 3, 3>::Identity();
  mr = Eigen::AngleAxisf(fAngle, vAxis);
  rotate_camera_around_origin(mr, vLookAt);
}

void camera::rotate_camera_orbit_yaw(const float fYawAngle){
  rotate_camera_around_axis(fYawAngle, vector3<float>(0.0, 0.0, 1.0));
}

void camera::rotate_camera_orbit_pitch(const float fPitchAngle){
  rotate_camera_around_axis(fPitchAngle, vRight);
}

void camera::translate_camera_forward(const float fAmount){
  vector3<float> vViewDir_n = vViewDir.normalized();
  vViewPoint += vViewDir_n * fAmount;
  vLookAt    += vViewDir_n * fAmount;
}

void camera::translate_camera_right(const float fAmount){
  vector3<float> _tmp_tr = vRight * fAmount;
  vViewPoint += _tmp_tr;
  vLookAt += _tmp_tr;
}

void camera::translate_camera_up(const float fAmount){
  vector3<float> _tmp_tr = vLookUp * fAmount;
  vViewPoint += _tmp_tr;
  vLookAt += _tmp_tr;
}

void camera::translate_camera(vector3<float> vShift){
  vViewPoint += vShift;
  vLookAt    += vShift;
  orthogonalize_gs();
}

void camera::reset_camera(){
  //
  vViewPoint = vector3<float>(0.0, 9.0, 0.0);
  vLookAt    = vector3<float>(0.0, 0.0, 0.0);
  vLookUp    = vector3<float>(0.0, 0.0, 1.0);
  orthogonalize_gs();

  cur_proj = app_camera_proj_type::CAMERA_PROJ_PERSP;
}

void camera::update_camera(){
  app_state* astate = &(c_app::get_state());

  float xDt = astate->MouseX - astate->MouseOldX;
  float yDt = astate->MouseY - astate->MouseOldY;

  astate->MouseOldX = astate->MouseX;
  astate->MouseOldY = astate->MouseY;

  if (bMoveCamera){
      float fMoveRight = -xDt/camera::iNavDivStepTranslation;
      float fMoveUp = yDt/camera::iNavDivStepTranslation;

      if (fabs(fMoveRight) > camera::fNavTresh)
        translate_camera_right(fMoveRight);
      if (fabs(fMoveUp) > camera::fNavTresh)
        translate_camera_up(fMoveUp);
    }

  if (bRotateCamera){
      float fRotAngleX = yDt/camera::iNavDivStepRotation;
      float fRotAngleY = xDt/camera::iNavDivStepRotation;

      if (fabs(fRotAngleY) > camera::fNavTresh)
        rotate_camera_orbit_yaw(fRotAngleY);

      if (fabs(fRotAngleX) > camera::fNavTresh)
        rotate_camera_orbit_pitch(fRotAngleX);
    }


  mView = look_at<float>(vViewPoint, vLookAt, vLookUp);

  if (cur_proj == app_camera_proj_type::CAMERA_PROJ_PERSP)
    mProjection = perspective<float>(fFOV,
                                     astate->vViewportWidthHeight(0) /
                                     astate->vViewportWidthHeight(1),
                                     fZNearPersp, fZFarPersp);
  else {
      float a = astate->vViewportWidthHeight(0);
      a /= astate->vViewportWidthHeight(1);
      float r = (vViewPoint-vLookAt).norm();
      if (a > 1.0)
        mProjection = ortho<float>(-r*a, r*a, -r, r, fZNearOrtho, fZFarOrtho);
      else
        mProjection = ortho<float>(-r, r, -r/a, r/a, fZNearOrtho, fZFarOrtho);
    }

  mViewProjection = mProjection *  mView ;
  mViewInvTr = mat4_to_mat3<float>((mView.transpose()).inverse());
}

void camera::update_camera_zoom(const double _dist){
  vector3<float> vViewDir_n = - vViewPoint + vLookAt;
  float fDist = vViewDir_n.norm();
  fStoredDist = fDist;
  vViewDir_n.normalize();
  float fDistDelta = _dist * fMouseWhellCameraStep;
  bool bCanZoom = true;
  if ((fDist + fDistDelta > fMouseZoomMinDistance) || (fDistDelta < 0.0))
    vViewPoint += vViewDir_n * fDistDelta;
}

void camera::update_camera_translation(const bool _bMoveCamera){
  bMoveCamera = _bMoveCamera;
}

void camera::update_camera_rotation(bool _bRotateCamera){
  bRotateCamera = _bRotateCamera;
}

void camera::set_projection(app_camera_proj_type _proj_to_set){
  if (cur_proj != _proj_to_set){
      reset_camera();
      cur_proj = _proj_to_set;
    }

}

vector3<float> camera::unproject(const float _x, const float _y){
  app_state* astate = &(c_app::get_state());
  matrix4<float> mMVP_Inv = (mProjection * mView).inverse();
  vector4<float> invec4(_x, _y, 0.5, 1.0);
  vector4<float> rvec4 = mMVP_Inv * invec4;
  rvec4(3) = 1.0f / rvec4(3);
  rvec4(0) = rvec4(0) * rvec4(3);
  rvec4(1) = rvec4(1) * rvec4(3);
  rvec4(2) = rvec4(2) * rvec4(3);
  return vector3<float>(rvec4(0), rvec4(1), rvec4(2));
}
