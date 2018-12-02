#ifndef QPP_GIZMO_H
#define QPP_GIZMO_H
#include <geom/lace3d.hpp>
#include <geom/aabb.hpp>
#include <geom/ray.hpp>
#include <geom/primitive_intersections.hpp>
#include <qppcad/ws_item.hpp>
#include <data/color.hpp>

namespace qpp {

  namespace cad {

    enum gizmo_transform_type {
      translation,
      rotation
    };

    const vector3<float> gizmo_axis[3] = {
      vector3<float>(1, 0, 0),
      vector3<float>(0, 1, 0),
      vector3<float>(0, 0, 1)
    };

    const vector3<float> gizmo_color[3] = {
      clr_red,
      clr_green,
      clr_navy
    };

    class gizmo_t {

      public:

        vector3<float> pos;
        vector3<float> accum_translate;
        float gizmo_box_size;
        float gizmo_shift_magnitude;
        bool is_active{false};
        bool interact_at_the_moment{false};
        bool m_is_visible{true};
        gizmo_transform_type m_cur_ttype{gizmo_transform_type::translation};

        uint8_t touched_axis{0};
        std::array<aabb_3d_t<float>,3> bx;
        std::array<bool, 3> bx_touched;
        ws_item_t *attached_item;

        template<typename REAL>
        bool process_ray (ray_t<REAL> *ray) {

          bool _gizmo_touched = false;

          if (ray) {
              touched_axis = 4;
              for(uint8_t i = 0; i < 3; i++){
                  aabb_3d_t<float> aabb_in_world_frame = bx[i].shifted(pos);
                  if (ray_aabb_test(*ray, aabb_in_world_frame)){
                      touched_axis = i;
                      bx_touched[i] = true;
                      _gizmo_touched = true;
                    }
                  else
                    bx_touched[i] = false;
                }
            }

          return _gizmo_touched;

        }

        void translate_attached (float delta_time);
        void clear_selected_axis ();
        void update_gizmo (float delta_time, bool force_repaint = false);
        void render ();

        gizmo_t ();
    };

  }

}
#endif
