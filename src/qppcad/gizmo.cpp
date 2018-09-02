#include <qppcad/gizmo.hpp>
#include <qppcad/app.hpp>

using namespace qpp;

gizmo_t::gizmo_t(){
  gizmo_box_size = 0.35f;
  gizmo_shift_magnitude = 2.0f;
  is_active = true;
  pos = vector3<float>(0.0f, 0.0f, 0.0f);
  attached_item = nullptr;
  for (uint8_t i = 0; i < 3; i++)
    bx[i].fill_guess_with_shift(gizmo_box_size, gizmo_axis[i] * gizmo_shift_magnitude);
  //update_gizmo();
}

void gizmo_t::render(){
  app_state_t* astate = &(c_app::get_state());
  astate->dp->begin_render_cube();

  vector3<float> _v_scale = vector3<float>::Ones()*gizmo_box_size;

  //draw center cube
  astate->dp->render_cube(pos, _v_scale * 0.9f, clr_gray);

  vector3<float> x_gizmo_color = clr_red;
  if (bx_touched[0]) x_gizmo_color = clr_yellow;

  vector3<float> y_gizmo_color = clr_green;
  if (bx_touched[1]) y_gizmo_color = clr_yellow;

  vector3<float> z_gizmo_color = clr_navy;
  if (bx_touched[2]) z_gizmo_color = clr_yellow;

  astate->dp->render_cube(pos + gizmo_axis[0] * gizmo_shift_magnitude, _v_scale, x_gizmo_color);
  astate->dp->render_cube(pos + gizmo_axis[1] * gizmo_shift_magnitude, _v_scale, y_gizmo_color);
  astate->dp->render_cube(pos + gizmo_axis[2] * gizmo_shift_magnitude, _v_scale, z_gizmo_color);

  astate->dp->render_cube(pos + gizmo_axis[0] * (gizmo_shift_magnitude-gizmo_box_size)*0.5f,
      vector3<float>((gizmo_shift_magnitude/2-gizmo_box_size)+_v_scale[0],
      _v_scale[1] * 0.5f,
      _v_scale[2] * 0.5f ),
      x_gizmo_color);

  astate->dp->render_cube(pos + gizmo_axis[1] * (gizmo_shift_magnitude-gizmo_box_size)*0.5f,
      vector3<float>(_v_scale[0] * 0.5f,
      (gizmo_shift_magnitude/2-gizmo_box_size)+_v_scale[1],
      _v_scale[2] * 0.5f),
      y_gizmo_color);

  astate->dp->render_cube(pos + gizmo_axis[2] * (gizmo_shift_magnitude-gizmo_box_size)*0.5f,
      vector3<float>(_v_scale[0] * 0.5f,
      _v_scale[1] * 0.5f,
      (gizmo_shift_magnitude/2-gizmo_box_size)+ _v_scale[2]),
      z_gizmo_color);

  astate->dp->end_render_cube();

}

void gizmo_t::transform_attached_object(float delta_time){
  app_state_t *astate = &(c_app::get_state());
  if (attached_item){
      float mouse_delta = astate->mouse_x_old - astate->mouse_x+
                          astate->mouse_y_old - astate->mouse_y;
      if (fabs(mouse_delta) > 0.1f) attached_item->pos +=
        gizmo_axis[touched_axis] * delta_time * (mouse_delta) * 0.1;
    }
}

void gizmo_t::clear_selected_axis(){
  for (int i = 0; i < 3; i++) bx_touched[i] = false;
}

void gizmo_t::update_gizmo(float delta_time){
  app_state_t *astate = &(c_app::get_state());
  if (attached_item){
      pos = attached_item->aabb.min+attached_item->pos;
      if (astate->mouse_lb_pressed && touched_axis < 4) transform_attached_object(delta_time);
      else clear_selected_axis();
    }


}
