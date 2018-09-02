#include <qppcad/gizmo.hpp>
#include <qppcad/app.hpp>

using namespace qpp;

gizmo_t::gizmo_t(){
  gizmo_box_size = 0.35f;
  gizmo_shift_magnitude = 2.0f;
  is_active = true;
  pos = vector3<float>(0.0f, 0.0f, 0.0f);
  attached_item = nullptr;
  interact_at_the_moment = false;
  for (uint8_t i = 0; i < 3; i++)
    bx[i].fill_guess_with_shift(gizmo_box_size, gizmo_axis[i] * gizmo_shift_magnitude);
  //update_gizmo();
}

void gizmo_t::render(){
  app_state_t* astate = &(c_app::get_state());
  astate->dp->begin_render_cube();

  vector3<float> _v_scale = vector3<float>::Ones()*gizmo_box_size;

  if (!interact_at_the_moment){
      astate->dp->render_cube(pos, _v_scale * 0.9f, clr_gray);
      astate->dp->render_cube(pos + gizmo_axis[0] * gizmo_shift_magnitude,
          _v_scale, gizmo_color[0]);
      astate->dp->render_cube(pos + gizmo_axis[1] * gizmo_shift_magnitude,
          _v_scale, gizmo_color[1]);
      astate->dp->render_cube(pos + gizmo_axis[2] * gizmo_shift_magnitude,
          _v_scale, gizmo_color[2]);

      astate->dp->render_cube(pos + gizmo_axis[0] * (gizmo_shift_magnitude-gizmo_box_size)*0.5f,
          vector3<float>((gizmo_shift_magnitude/2-gizmo_box_size)+_v_scale[0],
          _v_scale[1] * 0.5f,
          _v_scale[2] * 0.5f ),
          gizmo_color[0]);

      astate->dp->render_cube(pos + gizmo_axis[1] * (gizmo_shift_magnitude-gizmo_box_size)*0.5f,
          vector3<float>(_v_scale[0] * 0.5f,
          (gizmo_shift_magnitude/2-gizmo_box_size)+_v_scale[1],
          _v_scale[2] * 0.5f),
          gizmo_color[1]);

      astate->dp->render_cube(pos + gizmo_axis[2] * (gizmo_shift_magnitude-gizmo_box_size)*0.5f,
          vector3<float>(_v_scale[0] * 0.5f,
          _v_scale[1] * 0.5f,
          (gizmo_shift_magnitude/2-gizmo_box_size)+ _v_scale[2]),
          gizmo_color[2]);

      astate->dp->end_render_cube();

    }

  else {
      vector3<float> vec_small_aliasing(0.05f, 0.05f, 0.05f);
      for (uint8_t i = 0; i < 3; i++)
        if (bx_touched[i]){
            astate->dp->begin_render_line();
            astate->dp->render_line(gizmo_color[i],
                                    pos - gizmo_axis[i] * 120 + vec_small_aliasing,
                                    pos + gizmo_axis[i] * 120 + vec_small_aliasing,
                                    4.0f);
            astate->dp->end_render_line();
          }
    }

}

void gizmo_t::transform_attached_object(float delta_time){
  app_state_t *astate = &(c_app::get_state());
  if (attached_item){

      vector3<float> unproj_mouse_hit_old =
          astate->camera->unproject(astate->mouse_x_old, astate->mouse_y_old);

      vector3<float> unproj_mouse_hit     =
          astate->camera->unproject(astate->mouse_x, astate->mouse_y);

      vector3<float> d_unproj = unproj_mouse_hit - unproj_mouse_hit_old;

      float mouse_delta = astate->mouse_x_old - astate->mouse_x+
                          astate->mouse_y_old - astate->mouse_y;


      if (fabs(mouse_delta) > 1.5f) attached_item->pos +=
          gizmo_axis[touched_axis] * delta_time * d_unproj[touched_axis];
    }
}

void gizmo_t::clear_selected_axis(){
  for (int i = 0; i < 3; i++) bx_touched[i] = false;
  interact_at_the_moment = false;
}

void gizmo_t::update_gizmo(float delta_time){

  app_state_t *astate = &(c_app::get_state());

  if (attached_item) pos = attached_item->aabb.min+attached_item->pos;

  if (attached_item && astate->mouse_lb_pressed && touched_axis < 4){
      interact_at_the_moment = true;
      transform_attached_object(delta_time);
    }
  else {
      interact_at_the_moment = false;
      clear_selected_axis();
    }




}
