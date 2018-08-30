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
    static constexpr float fNormEps = 0.00001f;
    static constexpr float fNavTresh = 0.01f;
    static constexpr int   iNavDivStepTranslation = 100;
    static constexpr int   iNavDivStepRotation = 100;

    vector3<float> vViewPoint;
    vector3<float> vViewDir;
    vector3<float> vLookAt;
    vector3<float> vLookUp;
    vector3<float> vRight;

    matrix4<float> mView;
    matrix4<float> mProjection;
    matrix4<float> mViewProjection;
    matrix3<float> mViewInvTr;

    bool bRotateCamera;
    bool bMoveCamera;

    /// how much to move forward on mouse wheel
    float fMouseWhellCameraStep;
    float fMouseZoomMinDistance;

    float fFOV;
    float fZNearPersp;
    float fZFarPersp;
    float fZNearOrtho;
    float fZFarOrtho;
    float fStoredDist;

    app_camera_proj_type cur_proj;

    camera_t();
    void orthogonalize_gs();
    void rotate_camera_around_origin(matrix3<float> mRot,
                                     vector3<float> vOrigin);
    void rotate_camera_around_axis(const float fAngle,
                                     vector3<float> vAxis);
    void rotate_camera_orbit_yaw(const float fYawAngle);
    void rotate_camera_orbit_pitch(const float fPitchAngle);
    void translate_camera_forward(const float fAmount);
    void translate_camera_right(const float fAmount);
    void translate_camera_up(const float fAmount);
    void translate_camera(vector3<float> vShift);

    void reset_camera();
    void update_camera();
    void update_camera_zoom(const double _dist);
    void update_camera_translation(const bool _bMoveCamera);
    void update_camera_rotation(bool _bRotateCamera);

    void set_projection(app_camera_proj_type _proj_to_set);
    vector3<float> unproject(const float _x, const float _y);
  };
}

#endif
