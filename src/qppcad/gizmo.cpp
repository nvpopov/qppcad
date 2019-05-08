#include <qppcad/gizmo.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

gizmo_t::gizmo_t () {

  m_box_size = 0.65f;
  m_shift_magn = 2.0f;
  m_is_active = true;
  m_pos = vector3<float>::Zero();
  m_acc_tr = vector3<float>::Zero();
  attached_item = nullptr;
  m_is_interacting = false;

  for (uint8_t i = 0; i < 3; i++)
    m_bx[i].fill_guess_with_shift(m_box_size, gizmo_axis[i] * m_shift_magn);
  clear_selected_axis();
  //update_gizmo();
}

void gizmo_t::render () {

  if (!m_is_visible) return;

  app_state_t* astate = app_state_t::get_inst();
  ws_edit_e cur_edit_type = astate->ws_mgr->get_cur_ws()->m_edit_type;

  //prevent showing gizmo when no content selected
  if (attached_item && attached_item->get_num_cnt_selected() == 0 &&
      cur_edit_type == ws_edit_e::edit_content) return;

  astate->dp->begin_render_general_mesh();

  vector3<float> _v_scale = vector3<float>::Ones()*m_box_size*0.15;
  vector3<float> _v_one = vector3<float>::Ones();

  if ( !m_is_interacting &&
       (cur_edit_type == ws_edit_e::edit_item ||
        (cur_edit_type == ws_edit_e::edit_content && attached_item->get_num_cnt_selected() > 0))
       ) {

      astate->dp->render_cube(m_pos, _v_scale * 1.2f, clr_gray);

      astate->dp->render_general_mesh(
            m_pos + gizmo_axis[0] * m_shift_magn,
          _v_one * 0.35f,
          vector3<float>( 0.0f, float(pi) / 2, 0.0f ),
          gizmo_color[0],
          astate->mesh_unit_cone);

      astate->dp->render_general_mesh(
            m_pos + gizmo_axis[1] * m_shift_magn,
          _v_one * 0.35f,
          vector3<float>( 0.0f, 0.0f, -float(pi) / 2 ),
          gizmo_color[1],
          astate->mesh_unit_cone);

      astate->dp->render_general_mesh(
            m_pos + gizmo_axis[2] * m_shift_magn,
          _v_one * 0.35f,
          vector3<float>( 0.0f, 0.0f, 0.0f),
          gizmo_color[2],
          astate->mesh_unit_cone);

      astate->dp->render_general_mesh(
            m_pos + gizmo_axis[0] * (m_shift_magn-m_box_size-1.25f),
          vector3<float>( _v_scale[0] , _v_scale[1] ,
          (m_shift_magn/2-m_box_size) + 1.75f),
          vector3<float>( 0.0f, float(pi) / 2.0,  0.0f),
          gizmo_color[0],
          astate->mesh_cylinder);

      astate->dp->render_general_mesh(
            m_pos + gizmo_axis[1] * (m_shift_magn-m_box_size+0.75),
          vector3<float>( _v_scale[0] , _v_scale[1] ,
          (m_shift_magn/2-m_box_size) + 1.75f),
          vector3<float>( 0.0f, 0.0f,  float(pi) / 2.0),
          gizmo_color[1],
          astate->mesh_cylinder);

      astate->dp->render_general_mesh(
            m_pos + gizmo_axis[2] * (m_shift_magn-m_box_size-1.25f),
          vector3<float>( _v_scale[0] , _v_scale[1] ,
          (m_shift_magn/2-m_box_size) + 1.75f),
          vector3<float>( 0.0f, 0.0f,  0.0f),
          gizmo_color[2],
          astate->mesh_cylinder);

      astate->dp->end_render_general_mesh();

    } else {

      vector3<float> vec_small_aliasing(0.05f, 0.05f, 0.05f);
      astate->dp->begin_render_line();

      for (uint8_t i = 0; i < 3; i++)
        if (m_bx_touched[i])
          astate->dp->render_line(gizmo_color[i],
                                  m_pos - gizmo_axis[i] * 120 + vec_small_aliasing,
                                  m_pos + gizmo_axis[i] * 120 + vec_small_aliasing);
        else astate->dp->render_line(gizmo_color[i],
                                     m_pos - gizmo_axis[i] * 2 + vec_small_aliasing,
                                     m_pos + gizmo_axis[i] * 2 + vec_small_aliasing);
      astate->dp->end_render_line();

    }

}

void gizmo_t::translate_attached(float delta_time) {

  app_state_t* astate = app_state_t::get_inst();

  if (!astate->mouse_lb_pressed) {
      m_is_interacting = false;
      clear_selected_axis();
      return;
    }

  if (!astate->is_mouse_moving || astate->mouse_distance_pp < 1) {
      m_acc_tr = vector3<float>::Zero();
      return;
    }

  if (attached_item) {

      ws_edit_e cur_edit_type = astate->ws_mgr->get_cur_ws()->m_edit_type;

      vector3<float> unproj_mouse_hit_old =
          astate->camera->unproject(astate->mouse_x_dc_old, astate->mouse_y_dc_old);

      vector3<float> unproj_mouse_hit     =
          astate->camera->unproject(astate->mouse_x_dc, astate->mouse_y_dc);

      vector3<float> d_unproj = unproj_mouse_hit - unproj_mouse_hit_old;

      if (m_touched_axis >= 0 && m_touched_axis < 4) {
          //TODO: Magic numbers
          if (fabs(d_unproj[m_touched_axis]) > 0.00005f &&
              astate->is_mouse_moving &&
              astate->mouse_distance_pp > 0.001f) {

              float proj_dep_mod = 1500.0f;

              if (astate->camera->m_cur_proj == cam_proj_t::proj_ortho) proj_dep_mod = 350.0f;

              vector3<float> new_transform = gizmo_axis[m_touched_axis] * delta_time *
                                             d_unproj[m_touched_axis] * proj_dep_mod;

              m_acc_tr += new_transform;

              const float transform_amplitude = 0.2f;
              m_acc_tr[0] = std::clamp(m_acc_tr[0], -transform_amplitude, transform_amplitude);
              m_acc_tr[1] = std::clamp(m_acc_tr[1], -transform_amplitude, transform_amplitude);
              m_acc_tr[2] = std::clamp(m_acc_tr[2], -transform_amplitude, transform_amplitude);

              //std::cout << "M_ACC_TR" << m_acc_tr << std::endl;

              if (cur_edit_type == ws_edit_e::edit_item) attached_item->translate(m_acc_tr);
              else attached_item->apply_intermediate_translate_content(new_transform);

            }
          else {
              m_acc_tr = vector3<float>::Zero();
            }

        }

    } // if (attached_item)

}

void gizmo_t::clear_selected_axis () {
  for (int i = 0; i < 3; i++) m_bx_touched[i] = false;
  m_is_interacting = false;
}

void gizmo_t::update_gizmo (float delta_time, bool force_repaint) {

  app_state_t* astate = app_state_t::get_inst();
  ws_edit_e cur_edit_type = astate->ws_mgr->get_cur_ws()->m_edit_type;

  // update gizmo position according to the current workspace`s edit type value
  // if we are in node edit mode - snap to aabb min
  if (attached_item && cur_edit_type == ws_edit_e::edit_item) {
      m_pos = attached_item->m_pos;
      //m_is_visible = true;
      m_is_active = true;
      if (force_repaint) astate->make_viewport_dirty();
    }

  // if we are in the content edit mode - snap to calculated barycenter, provided by node
  if (attached_item && cur_edit_type == ws_edit_e::edit_content) {
      m_pos = attached_item->m_pos + attached_item->get_gizmo_content_barycenter();
      //m_is_visible = true;
      m_is_active = true;
      if (force_repaint) astate->make_viewport_dirty();
    }

  if (!astate->mouse_lb_pressed) {
      m_is_interacting = false;
      m_acc_tr = vector3<float>::Zero();
      clear_selected_axis();
      return;
    }

  // Transform in node mode
  // start interacting - run event
  if (attached_item && astate->mouse_lb_pressed && m_touched_axis >= 0 && m_touched_axis < 4 &&
      cur_edit_type == ws_edit_e::edit_item && !m_is_interacting){
      m_is_interacting = true;
      m_acc_tr = vector3<float>::Zero();
      attached_item->on_begin_node_gizmo_translate();
      astate->make_viewport_dirty();
    }

  // interacting - event already fired, start dragging object
  if (attached_item && astate->mouse_lb_pressed && m_touched_axis >= 0 && m_touched_axis < 4 &&
      cur_edit_type == ws_edit_e::edit_item && m_is_interacting) {
      translate_attached(delta_time);
      astate->make_viewport_dirty();
    }

  // we release left mouse button - fire event(on_end_content_translate)
  if (attached_item && !astate->mouse_lb_pressed &&
      cur_edit_type == ws_edit_e::edit_item && m_is_interacting) {
      m_is_interacting = false;
      attached_item->on_end_node_gizmo_translate();
      clear_selected_axis();
      astate->make_viewport_dirty();
    }
  // End transform in node edit mode

  // Transform in node mode
  // start interacting - run event
  if (attached_item && astate->mouse_lb_pressed &&  m_touched_axis >= 0 && m_touched_axis < 4 &&
      cur_edit_type == ws_edit_e::edit_content && !m_is_interacting) {
      m_is_interacting = true;
      m_acc_tr = vector3<float>::Zero();
      attached_item->on_begin_content_gizmo_translate();
      astate->make_viewport_dirty();
    }

  // interacting - event already fired, start dragging object
  if (attached_item && astate->mouse_lb_pressed &&  m_touched_axis >= 0 && m_touched_axis < 4 &&
      cur_edit_type == ws_edit_e::edit_content && m_is_interacting) {
      translate_attached(delta_time);
      astate->make_viewport_dirty();
    }

  // we we release left mouse button - fire event(on_end_content_translate)
  if (attached_item && !astate->mouse_lb_pressed &&
      cur_edit_type == ws_edit_e::edit_content && m_is_interacting) {
      m_is_interacting = false;
      attached_item->on_end_content_gizmo_translate();
      clear_selected_axis();
      astate->make_viewport_dirty();
    }
  // end transform in node edit mode

}
