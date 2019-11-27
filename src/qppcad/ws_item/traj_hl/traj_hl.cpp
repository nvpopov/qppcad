#include <qppcad/ws_item/traj_hl/traj_hl.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

traj_hl_t::traj_hl_t() {
  set_default_flags(ws_item_flags_default);
}

void traj_hl_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                                vector3<float> &out_look_at) {
  //do nothing
}

void traj_hl_t::render() {

  app_state_t* astate = app_state_t::get_inst();

  //do nothing
  if (!m_line_mesh) {
      m_line_mesh = std::make_unique<mesh_t>();
    }

  if (m_need_to_rebuild) {
      rebuild_line_mesh();
      m_need_to_rebuild = false;
    }

  //render mesh
  if (m_is_visible && !m_need_to_rebuild && m_leader) {

      if (m_traj_style == traj_hl_style_points ||
          m_traj_style == traj_hl_style_lines ||
          m_traj_style == traj_hl_style_lines_loop) {

          astate->dp->begin_render_line_mesh();
          astate->sp_line_mesh->set_u(sp_u_name::v_translate, (GLfloat*)m_leader->m_pos.data());
          astate->sp_line_mesh->set_u(sp_u_name::v_color, (GLfloat*)m_traj_color.data());
          m_line_mesh->render_ex(GL_POINTS + m_traj_style);
          astate->dp->end_render_line_mesh();
        } else {

          if (m_traj_style == traj_hl_style_spheres && m_binded_gv &&
              m_anim_id < m_binded_gv->m_anim->get_total_anims()  &&
              m_atom_id < m_binded_gv->m_geom->nat()) {

              astate->dp->begin_atom_render(12, 1.0f);
              for (size_t i = 0; i < m_binded_gv->m_anim->m_anim_data[m_anim_id].frames.size(); i++)
                astate->dp->render_atom(
                      m_traj_color,
                      m_binded_gv->m_anim->m_anim_data[m_anim_id].frames[i].atom_pos[m_atom_id],
                      m_elem_size);
              astate->dp->end_atom_render();

            }

        }

    }

}

bool traj_hl_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string traj_hl_t::compose_type_descr() {
  return "traj_hl";
}

void traj_hl_t::update(float delta_time) {
  //do nothing
}

float traj_hl_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t traj_hl_t::get_num_cnt_selected() {
  return 0;
}

size_t traj_hl_t::get_content_count() {
  return 0;
}

void traj_hl_t::on_leader_changed() {

   app_state_t* astate = app_state_t::get_inst();
   //astate->tlog("{} ::on_leader_changed()", m_name);

   if (m_leader->get_type() == geom_view_t::get_type_static()) {
         m_need_to_rebuild = true;
         m_binded_gv = m_leader->cast_as<geom_view_t>();
     }

}

void traj_hl_t::on_leader_call() {


}

void traj_hl_t::rebuild_line_mesh() {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_binded_gv) return;
  if (m_binded_gv->m_anim->get_total_anims() < 1) return;

  int anm_idx = m_anim_id;
  int atm_idx = m_atom_id;

  m_line_mesh->vertecies.clear();
  m_line_mesh->normals.clear();
  m_line_mesh->indices.clear();

  for (int frame_id = 0; frame_id < m_binded_gv->m_anim->m_anim_data[anm_idx].frames.size(); frame_id++) {
      if (m_binded_gv->m_anim->m_anim_data[anm_idx].frames[frame_id].atom_pos.size() < atm_idx) return;
      vector3<float> atm_pos =
          m_binded_gv->m_anim->m_anim_data[anm_idx].frames[frame_id].atom_pos[atm_idx];
      m_line_mesh->vertecies.push_back(atm_pos[0]);
      m_line_mesh->vertecies.push_back(atm_pos[1]);
      m_line_mesh->vertecies.push_back(atm_pos[2]);
      m_line_mesh->normals.push_back(0);
      m_line_mesh->normals.push_back(0);
      m_line_mesh->normals.push_back(1);
      m_line_mesh->indices.push_back(frame_id);
    }

  astate->tlog("traj_hler_t total points : {}", m_line_mesh->indices.size());
  m_line_mesh->mesh_rt = GL_LINE_LOOP;
  m_line_mesh->num_primitives = m_line_mesh->indices.size();
  m_line_mesh->bind_data();

}

void traj_hl_t::save_to_json(json &data) {

  ws_item_t::save_to_json(data);

}

void traj_hl_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);

}

void traj_hl_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

  app_state_t *astate = app_state_t::get_inst();

  if (m_src) {
      auto _as_gv = m_src->cast_as<geom_view_t>();
      //if (_as_gv && _as_gv == m_binded_gv)
      if (_as_gv) m_binded_gv = _as_gv;
      else m_binded_gv = nullptr;
    } else {
      m_binded_gv = nullptr;
    }

}

