#ifndef QPP_GLMATH_H
#define QPP_GLMATH_H
#include <Eigen/Eigen>
#include <geom/lace3d.hpp>
#include <mathf/math.hpp>

namespace qpp {

  namespace cad {

    template<typename REAL>
    matrix4<REAL> perspective( REAL fovy,
                               REAL aspect,
                               REAL zNear,
                               REAL zFar){
      //    assert(aspect > 0);
      //    assert(zFar > zNear);

      REAL radf = deg2rad(fovy);

      REAL tanHalfFovy = tan(radf / 2.0);
      matrix4<REAL> res = matrix4<REAL>::Zero();
      res(0,0) = 1.0 / (aspect * tanHalfFovy);
      res(1,1) = 1.0 / (tanHalfFovy);
      res(2,2) = - (zFar + zNear) / (zFar - zNear);
      res(3,2) = - 1.0;
      res(2,3) = - (2.0 * zFar * zNear) / (zFar - zNear);
      return res;
    }

    template<typename REAL>
    matrix4<REAL> ortho(REAL left,
                        REAL right,
                        REAL bottom,
                        REAL top,
                        REAL zNear,
                        REAL zFar){

      matrix4<REAL> res;

      res(0,0) = REAL(2) / (right-left);
      res(0,1) = 0;
      res(0,2) = 0;
      res(0,3) = -(right+left) / (right-left);

      res(1,0) = 0;
      res(1,1) = REAL(2) / (top-bottom);
      res(1,2) = 0;
      res(1,3) = -(top+bottom) / (top-bottom);

      res(2,0) = 0;
      res(2,1) = 0;
      res(2,2) = -REAL(2) / (zFar-zNear);
      res(2,3) = -(zFar+zNear) / (zFar-zNear);

      res(3,0) = 0;
      res(3,1) = 0;
      res(3,2) = 0;
      res(3,3) = REAL(1);

      return res;
    }

    template<typename REAL>
    matrix4<REAL> look_at(
        vector3<REAL> const & camPos,
        vector3<REAL> const & center,
        vector3<REAL> const & up
        ){

      vector3<REAL> f = (center - camPos).normalized();
      vector3<REAL> u = up.normalized();
      vector3<REAL> s = f.cross(u).normalized();
      u = s.cross(f);

      matrix4<REAL> res;
      res <<  s.x(),s.y(),s.z(),-s.dot(camPos),
          u.x(),u.y(),u.z(),-u.dot(camPos),
          -f.x(),-f.y(),-f.z(),f.dot(camPos),
          0,0,0,1;

      return res;
    }

    template<typename REAL>
    matrix3<REAL> align_vectors3(const vector3<REAL> v1, const vector3<REAL> v2) {
      matrix3<REAL> m_out = matrix4<REAL>::Identity();
      vector3<REAL> v3 = (v1.cross(v2)).normalized();
      vector3<REAL> v4 = (v3.cross(v1)).normalized();
      REAL cosv = v2.dot(v1);
      REAL sinv = v2.dot(v4);
      matrix3<REAL> m1 = matrix4<REAL>::Identity();
      matrix3<REAL> m2 = matrix4<REAL>::Identity();

      m1(0,0) = v1(0);
      m1(0,1) = v1(1);
      m1(0,2) = v1(2);

      m1(1,0) = v4(0);
      m1(1,1) = v4(1);
      m1(1,2) = v4(2);

      m1(2,0) = v3(0);
      m1(2,1) = v3(1);
      m1(2,2) = v3(2);

      m2(0,0) = cosv;
      m2(0,1) = sinv;

      m2(1,0) = -sinv;
      m2(1,1) = cosv;

      return m1.inverse() * m2 * m1;
    }

    template<typename REAL>
    matrix4<REAL> align_vectors4(const vector3<REAL> v1, const vector3<REAL> v2){
      //   V3 = normalize(cross(V1, V2)). (the normalization here is mandatory.)
      //   V4 = cross(V3, V1).

      //        [ V1 ]
      //   M1 = [ V4 ]
      //        [ V3 ]

      //   cos = dot(V2, V1), sin = dot(V2, V4)

      //        [ cos   sin    0 ]
      //   M2 = [ -sin  cos    0 ]
      //        [ 0     0      1 ]
      //   M1^-1 * M2 * M1

      matrix4<REAL> m_out = matrix4<REAL>::Identity();
      vector3<REAL> v3 = (v1.cross(v2)).normalized();
      vector3<REAL> v4 = (v3.cross(v1)).normalized();
      REAL cosv = v2.dot(v1);
      REAL sinv = v2.dot(v4);
      matrix4<REAL> m1 = matrix4<REAL>::Identity();
      matrix4<REAL> m2 = matrix4<REAL>::Identity();

      m1(0,0) = v1(0);
      m1(0,1) = v1(1);
      m1(0,2) = v1(2);

      m1(1,0) = v4(0);
      m1(1,1) = v4(1);
      m1(1,2) = v4(2);

      m1(2,0) = v3(0);
      m1(2,1) = v3(1);
      m1(2,2) = v3(2);

      m2(0,0) = cosv;
      m2(0,1) = sinv;

      m2(1,0) = -sinv;
      m2(1,1) = cosv;

      return m1.inverse() * m2 * m1;
    }

  } // namespace qpp::cad

} // namespace qpp

#endif
