#ifndef QPPCAD_GIZMO_H
#define QPPCAD_GIZMO_H
#include <geom/lace3d.hpp>
#include <geom/aabb.hpp>
#include <geom/ray.hpp>
#include <geom/primitive_intersections.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <data/color.hpp>

namespace qpp {

  namespace cad {

    enum class gizmo_transform_type {
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

        vector3<float> m_pos; // gizmo pos in world coordinates
        vector3<float> m_acc_tr;

        std::array<vector2<float> , 3> m_proj_axes;
        vector2<float> m_proj_axes_cnt;

        float m_box_size;
        float m_shift_magn;
        bool m_is_active{false};
        bool m_is_interacting{false};
        bool m_is_visible{true};
        gizmo_transform_type m_cur_type{gizmo_transform_type::translation};

        int8_t m_touched_axis{-1};
        std::array<aabb_3d_t<float>,3> m_bx;
        std::array<bool, 3> m_bx_touched{false, false, false};
        ws_item_t *attached_item{nullptr};

        template<typename REAL>
        bool process_ray (ray_t<REAL> *ray) {

          bool _gizmo_touched = false;

          if (!check_attached_item_in_content_mode()) return false;

          if (ray) {
              m_touched_axis = 4;
              for(uint8_t i = 0; i < 3; i++){
                  aabb_3d_t<float> aabb_in_world_frame = m_bx[i].shifted(m_pos);
                  if (ray_aabb_test(*ray, aabb_in_world_frame)){
                      m_touched_axis = i;
                      m_bx_touched[i] = true;
                      _gizmo_touched = true;
                    }
                  else
                    m_bx_touched[i] = false;
                }
            }

          return _gizmo_touched;

        }

        vector2<float> get_axis_coord(size_t axis_id, float displ);
        bool check_attached_item_in_content_mode();
        void translate_attached (float delta_time);
        void clear_selected_axis ();
        void update_gizmo (float delta_time, bool force_repaint = false);
        void render ();
        bool is_any_axis_touched();

        gizmo_t ();
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
