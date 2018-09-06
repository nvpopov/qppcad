#include <qppcad/gizmo.hpp>
#include <qppcad/app.hpp>

using namespace qpp;

gizmo_t::gizmo_t(){
  gizmo_box_size = 0.65f;
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
  astate->dp->begin_render_general_mesh();

  vector3<float> _v_scale = vector3<float>::Ones()*gizmo_box_size*0.25;
  vector3<float> _v_one = vector3<float>::Ones();
  if (!interact_at_the_moment){
      astate->dp->render_cube(pos, _v_scale * 1.2f, clr_gray);
//      astate->dp->render_cube(pos + gizmo_axis[0] * gizmo_shift_magnitude,
//          _v_scale, gizmo_color[0]);
//      astate->dp->render_cube(pos + gizmo_axis[1] * gizmo_shift_magnitude,
//          _v_scale, gizmo_color[1]);

      astate->dp->render_general_mesh(
            pos + gizmo_axis[0] * gizmo_shift_magnitude,
          _v_one * 0.5f,
          vector3<float>( -pi / 2, 0.0f, 0.0f ),
          gizmo_color[0],
          astate->unit_cone);

      astate->dp->render_general_mesh(
            pos + gizmo_axis[1] * gizmo_shift_magnitude,
          _v_one * 0.5f,
          vector3<float>( 0.0f, 0.0f, 0.0f ),
          gizmo_color[1],
          astate->unit_cone);

      astate->dp->render_general_mesh(
            pos + gizmo_axis[2] * gizmo_shift_magnitude,
          _v_one * 0.5f,
          vector3<float>( 0.0f, 0.0f,  pi / 2),
          gizmo_color[2],
          astate->unit_cone);



//      astate->dp->render_cube(pos + gizmo_axis[0] * (gizmo_shift_magnitude-gizmo_box_size)*0.5f,
//          vector3<float>((gizmo_shift_magnitude/2-gizmo_box_size)+_v_scale[0],
//          _v_scale[1] * 0.5f,
//          _v_scale[2] * 0.5f ),
//          gizmo_color[0]);

      astate->dp->render_general_mesh(
          pos + gizmo_axis[0] * (gizmo_shift_magnitude-gizmo_box_size-1.25f),
          vector3<float>( _v_scale[0] , _v_scale[1] ,
          (gizmo_shift_magnitude/2-gizmo_box_size)+1.75f),
          vector3<float>( 0.0f, pi / 2.0,  0.0f),
          gizmo_color[0],
          astate->cylinder_mesh);

      astate->dp->render_general_mesh(
          pos + gizmo_axis[1] * (gizmo_shift_magnitude-gizmo_box_size+0.75),
          vector3<float>( _v_scale[0] , _v_scale[1] ,
          (gizmo_shift_magnitude/2-gizmo_box_size)+1.75f),
          vector3<float>( 0.0f, 0.0f,   pi / 2.0),
          gizmo_color[1],
          astate->cylinder_mesh);

      astate->dp->render_general_mesh(
          pos + gizmo_axis[2] * (gizmo_shift_magnitude-gizmo_box_size-1.25f),
          vector3<float>( _v_scale[0] , _v_scale[1] ,
          (gizmo_shift_magnitude/2-gizmo_box_size)+1.75f),
          vector3<float>( 0.0f, 0.0f,  0.0f),
          gizmo_color[2],
          astate->cylinder_mesh);

//      astate->dp->render_cube(pos + gizmo_axis[1] * (gizmo_shift_magnitude-gizmo_box_size)*0.5f,
//          vector3<float>(_v_scale[0] * 0.5f,
//          (gizmo_shift_magnitude/2-gizmo_box_size)+_v_scale[1],
//          _v_scale[2] * 0.5f),
//          gizmo_color[1]);

//      astate->dp->render_cube(pos + gizmo_axis[2] * (gizmo_shift_magnitude-gizmo_box_size)*0.5f,
//          vector3<float>(_v_scale[0] * 0.5f,
//          _v_scale[1] * 0.5f,
//          (gizmo_shift_magnitude/2-gizmo_box_size)+ _v_scale[2]),
//          gizmo_color[2]);

      astate->dp->end_render_general_mesh();

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


      if (fabs(d_unproj[touched_axis]) > 0.01f) attached_item->pos +=
          gizmo_axis[touched_axis] * delta_time * d_unproj[touched_axis] * 0.5f;
    }
}

void gizmo_t::clear_selected_axis(){
  for (int i = 0; i < 3; i++) bx_touched[i] = false;
  interact_at_the_moment = false;
}

void gizmo_t::update_gizmo(float delta_time){

  app_state_t *astate = &(c_app::get_state());
  ws_edit_type cur_edit_type = astate->workspace_manager->get_current_workspace()->cur_edit_type;

  if (attached_item && cur_edit_type== ws_edit_type::EDIT_WS_ITEM)
    pos = attached_item->aabb.min+attached_item->pos;

  if (attached_item && cur_edit_type== ws_edit_type::EDIT_WS_ITEM_CONTENT)
    pos = attached_item->pos + attached_item->get_gizmo_content_barycenter();


  if (attached_item && astate->mouse_lb_pressed && touched_axis < 4 &&
      cur_edit_type== ws_edit_type::EDIT_WS_ITEM){
      interact_at_the_moment = true;
      transform_attached_object(delta_time);
    }
  else {
      interact_at_the_moment = false;
      clear_selected_axis();
    }




}
