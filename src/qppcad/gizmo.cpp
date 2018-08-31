#include <qppcad/gizmo.hpp>
#include <qppcad/app.hpp>

using namespace qpp;

gizmo_t::gizmo_t(){
  gizmo_box_size = 0.65f;
  gizmo_shift_magnitude = 5.5f;
  is_active = true;
  pos = vector3<float>(0.0f, 0.0f, 0.0f);
}

void gizmo_t::render(){
  app_state_t* astate = &(c_app::get_state());
  astate->dp->begin_render_cube();

  vector3<float> _v_scale = vector3<float>::Ones()*gizmo_box_size;
  //draw center cube
  astate->dp->render_cube(pos, _v_scale * 0.8f, clr_gray);
  astate->dp->render_cube(pos+gizmo_axis[0]*gizmo_shift_magnitude, _v_scale, clr_red);
  astate->dp->render_cube(pos+gizmo_axis[1]*gizmo_shift_magnitude, _v_scale, clr_green);
  astate->dp->render_cube(pos+gizmo_axis[2]*gizmo_shift_magnitude, _v_scale, clr_navy);


  astate->dp->end_render_cube();

}

template<typename REAL>
void gizmo_t::process_ray(ray<REAL> *ray){
  if (ray) {
      for(uint8_t i = 0; i < 3; i++)
        if (ray_aabb_test(ray, &bx[i]))
          bx_touched[i] = true;
        else
          bx_touched[i] = false;
    }

}

void gizmo_t::update_gizmo(){
  for (uint8_t i = 0; i < 3; i++)
    bx[i].fill_guess_with_shift(gizmo_box_size, gizmo_axis[i] * gizmo_shift_magnitude);
}
