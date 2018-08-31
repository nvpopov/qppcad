#ifndef QPP_GIZMO_H
#define QPP_GIZMO_H
#include <geom/lace3d.hpp>
#include <geom/aabb.hpp>
#include <geom/ray.hpp>
#include <geom/primitive_intersections.hpp>

namespace qpp {

  const vector3<float> gizmo_axis[3] = {
    vector3<float>(1, 0, 0),
    vector3<float>(0, 1, 0),
    vector3<float>(0, 0, 1)
  };

  class gizmo_t {
    public:
      vector3<float> pos;
      float gizmo_box_size;
      float gizmo_shift_magnitude;
      bool is_active;
      std::array<aabb_3d<float>,3> bx;
      std::array<bool, 3> bx_touched;

      template<typename REAL>
      void process_ray(ray<REAL> *ray);
      void update_gizmo();
      void render();

      gizmo_t();
  };
}
#endif
