#include <qppcad/gizmo.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

gizmo_t::gizmo_t () {

  gizmo_box_size         = 0.65f;
  gizmo_shift_magnitude  = 2.0f;
  is_active              = true;
  pos                    = vector3<float>::Zero();
  accum_translate        = vector3<float>::Zero();
  attached_item          = nullptr;
  interact_at_the_moment = false;

  for (uint8_t i = 0; i < 3; i++)
    bx[i].fill_guess_with_shift(gizmo_box_size, gizmo_axis[i] * gizmo_shift_magnitude);
  clear_selected_axis();
  //update_gizmo();
}

void gizmo_t::render () {

  if (!m_is_visible) return;

  app_state_t* astate = app_state_t::get_inst();
  ws_edit_t cur_edit_type = astate->ws_manager->get_cur_ws()->m_edit_type;

  //prevent showing gizmo when no content selected
  if (attached_item && attached_item->get_amount_of_selected_content() == 0 &&
      cur_edit_type == ws_edit_t::edit_content) return;

  astate->dp->begin_render_general_mesh();

  vector3<float> _v_scale = vector3<float>::Ones()*gizmo_box_size*0.15;
  vector3<float> _v_one = vector3<float>::Ones();

  if ( !interact_at_the_moment &&
       (cur_edit_type == ws_edit_t::edit_item ||
        (cur_edit_type == ws_edit_t::edit_content &&
         attached_item->get_amount_of_selected_content() > 0)) ){

      astate->dp->render_cube(pos, _v_scale * 1.2f, clr_gray);

      astate->dp->render_general_mesh(
            pos + gizmo_axis[0] * gizmo_shift_magnitude,
          _v_one * 0.35f,
          vector3<float>( -pi / 2, 0.0f, 0.0f ),
          gizmo_color[0],
          astate->mesh_unit_cone);

      astate->dp->render_general_mesh(
            pos + gizmo_axis[1] * gizmo_shift_magnitude,
          _v_one * 0.35f,
          vector3<float>( 0.0f, 0.0f, 0.0f ),
          gizmo_color[1],
          astate->mesh_unit_cone);

      astate->dp->render_general_mesh(
            pos + gizmo_axis[2] * gizmo_shift_magnitude,
          _v_one * 0.35f,
          vector3<float>( 0.0f, 0.0f,  pi / 2),
          gizmo_color[2],
          astate->mesh_unit_cone);

      astate->dp->render_general_mesh(
            pos + gizmo_axis[0] * (gizmo_shift_magnitude-gizmo_box_size-1.25f),
          vector3<float>( _v_scale[0] , _v_scale[1] ,
          (gizmo_shift_magnitude/2-gizmo_box_size)+1.75f),
          vector3<float>( 0.0f, pi / 2.0,  0.0f),
          gizmo_color[0],
          astate->mesh_cylinder);

      astate->dp->render_general_mesh(
            pos + gizmo_axis[1] * (gizmo_shift_magnitude-gizmo_box_size+0.75),
          vector3<float>( _v_scale[0] , _v_scale[1] ,
          (gizmo_shift_magnitude/2-gizmo_box_size)+1.75f),
          vector3<float>( 0.0f, 0.0f,   pi / 2.0),
          gizmo_color[1],
          astate->mesh_cylinder);

      astate->dp->render_general_mesh(
            pos + gizmo_axis[2] * (gizmo_shift_magnitude-gizmo_box_size-1.25f),
          vector3<float>( _v_scale[0] , _v_scale[1] ,
          (gizmo_shift_magnitude/2-gizmo_box_size)+1.75f),
          vector3<float>( 0.0f, 0.0f,  0.0f),
          gizmo_color[2],
          astate->mesh_cylinder);

      astate->dp->end_render_general_mesh();

    } else {
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

void gizmo_t::translate_attached(float delta_time){

  app_state_t* astate = app_state_t::get_inst();

  if (!astate->mouse_lb_pressed) {
      interact_at_the_moment = false;
      clear_selected_axis();
      return;
    }

  if (!astate->is_mouse_moving || astate->mouse_distance_pp < 15) {
      accum_translate = vector3<float>::Zero();
      return;
    }

  if (attached_item) {

      ws_edit_t cur_edit_type = astate->ws_manager->get_cur_ws()->m_edit_type;

      vector3<float> unproj_mouse_hit_old =
          astate->camera->unproject(astate->mouse_x_dc_old, astate->mouse_y_dc_old);

      vector3<float> unproj_mouse_hit     =
          astate->camera->unproject(astate->mouse_x_dc, astate->mouse_y_dc);

      vector3<float> d_unproj = unproj_mouse_hit - unproj_mouse_hit_old;


      if (fabs(d_unproj[touched_axis]) > 0.00025f &&
          astate->is_mouse_moving &&
          astate->mouse_distance_pp > 0.2) {

          vector3<float> new_transform =
              gizmo_axis[touched_axis] * delta_time * d_unproj[touched_axis] * 3500.0f;

          accum_translate += new_transform;

          if (cur_edit_type == ws_edit_t::edit_item) {
              attached_item->m_pos += new_transform;
              if (attached_item && attached_item->is_selected())
                astate->astate_evd->cur_ws_selected_item_position_changed();
            }
          else attached_item->apply_intermediate_translate_content(new_transform);
        }
      else {
          accum_translate = vector3<float>::Zero();
        }

    }
}

void gizmo_t::clear_selected_axis () {
  for (int i = 0; i < 3; i++) bx_touched[i] = false;
  interact_at_the_moment = false;
}

void gizmo_t::update_gizmo (float delta_time, bool force_repaint) {

  app_state_t* astate = app_state_t::get_inst();
  ws_edit_t cur_edit_type = astate->ws_manager->get_cur_ws()->m_edit_type;

  //update gizmo position according to current workspace edit type value
  //if we are in node edit mode - snap to aabb min
  if (attached_item && cur_edit_type == ws_edit_t::edit_item) {
      pos = attached_item->m_pos;
      //m_is_visible = true;
      is_active = true;
      if (force_repaint) astate->make_viewport_dirty();
    }

  //if we are in the content edit mode - snap to calculated barycenter, provided by node
  if (attached_item && cur_edit_type == ws_edit_t::edit_content) {
      pos = attached_item->m_pos + attached_item->get_gizmo_content_barycenter();
      //m_is_visible = true;
      is_active = true;
      if (force_repaint) astate->make_viewport_dirty();
    }

  if (!astate->mouse_lb_pressed) {
      interact_at_the_moment = false;
      accum_translate = vector3<float>::Zero();
      clear_selected_axis();
      return;
    }

  //Transform in node mode
  //start interacting - run event
  if (attached_item && astate->mouse_lb_pressed && touched_axis < 4 &&
      cur_edit_type == ws_edit_t::edit_item && !interact_at_the_moment){
      interact_at_the_moment = true;
      accum_translate = vector3<float>::Zero();
      attached_item->on_begin_node_gizmo_translate();
      astate->make_viewport_dirty();
    }

  //interacting - event already fired, start dragging object
  if (attached_item && astate->mouse_lb_pressed && touched_axis < 4 &&
      cur_edit_type == ws_edit_t::edit_item && interact_at_the_moment) {
      translate_attached(delta_time);
      astate->make_viewport_dirty();
    }

  //we we release left mouse button - fire event(on_end_content_translate)
  if (attached_item && !astate->mouse_lb_pressed &&
      cur_edit_type == ws_edit_t::edit_item && interact_at_the_moment) {
      interact_at_the_moment = false;
      attached_item->on_end_node_gizmo_translate();
      clear_selected_axis();
      astate->make_viewport_dirty();
    }
  //End transform in node edit mode

  //Transform in node mode
  //start interacting - run event
  if (attached_item && astate->mouse_lb_pressed && touched_axis < 4 &&
      cur_edit_type == ws_edit_t::edit_content && !interact_at_the_moment) {
      interact_at_the_moment = true;
      accum_translate = vector3<float>::Zero();
      attached_item->on_begin_content_gizmo_translate();
      astate->make_viewport_dirty();
    }

  //interacting - event already fired, start dragging object
  if (attached_item && astate->mouse_lb_pressed && touched_axis < 4 &&
      cur_edit_type == ws_edit_t::edit_content && interact_at_the_moment) {
      translate_attached(delta_time);
      astate->make_viewport_dirty();
    }

  //we we release left mouse button - fire event(on_end_content_translate)
  if (attached_item && !astate->mouse_lb_pressed &&
      cur_edit_type == ws_edit_t::edit_content && interact_at_the_moment) {
      interact_at_the_moment = false;
      attached_item->on_end_content_gizmo_translate();
      clear_selected_axis();
      astate->make_viewport_dirty();
    }
  //End transform in node edit mode


}
