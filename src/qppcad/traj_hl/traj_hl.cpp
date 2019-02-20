#include <qppcad/traj_hl/traj_hl.hpp>
#include <qppcad/geom_view/geom_view.hpp>
#include <qppcad/app_state.hpp>

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
  if (m_is_visible && !m_need_to_rebuild && m_leader) {
      astate->dp->begin_render_line_mesh();
      astate->sp_line_mesh->set_u(sp_u_name::v_translate, (GLfloat*)m_leader->m_pos.data());
      astate->sp_line_mesh->set_u(sp_u_name::v_color, (GLfloat*)m_traj_color.data());
      m_line_mesh->render();
      astate->dp->end_render_line_mesh();
    }
}

bool traj_hl_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string traj_hl_t::compose_item_name() {
  return "traj_hl";
}

void traj_hl_t::update(float delta_time) {
  //do nothing
}

float traj_hl_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t traj_hl_t::get_amount_of_selected_content() {
  return 0;
}

size_t traj_hl_t::get_content_count() {
  return 0;
}

void traj_hl_t::on_leader_changed() {
   app_state_t* astate = app_state_t::get_inst();
   astate->log(fmt::format("{} ::on_leader_changed()", m_name));

   if (m_leader->get_type() == geom_view_t::get_type_static()) {
         m_need_to_rebuild = true;
         b_al = m_leader->cast_as<geom_view_t>();
     }
}

void traj_hl_t::on_leader_call() {
//  app_state_t* astate = app_state_t::get_inst();
//  astate->log(fmt::format("{} ::on_leader_call()", m_name));
}

void traj_hl_t::rebuild_line_mesh() {

  app_state_t* astate = app_state_t::get_inst();

  if (!b_al) return;
  if (b_al->m_anim->get_total_anims() < 1) return;

  int anm_idx = m_anim_id;
  int atm_idx = m_atm_id;

  m_line_mesh->vertecies.clear();
  m_line_mesh->normals.clear();
  m_line_mesh->indices.clear();

  for (int frame_id = 0; frame_id < b_al->m_anim->m_anim_data[anm_idx].frames.size(); frame_id++) {
      if (b_al->m_anim->m_anim_data[anm_idx].frames[frame_id].atom_pos.size() < atm_idx) return;
      vector3<float> atm_pos =
          b_al->m_anim->m_anim_data[anm_idx].frames[frame_id].atom_pos[atm_idx];
      m_line_mesh->vertecies.push_back(atm_pos[0]);
      m_line_mesh->vertecies.push_back(atm_pos[1]);
      m_line_mesh->vertecies.push_back(atm_pos[2]);
      m_line_mesh->normals.push_back(0);
      m_line_mesh->normals.push_back(0);
      m_line_mesh->normals.push_back(1);
      m_line_mesh->indices.push_back(frame_id);
    }

  astate->log(fmt::format("traj_hler_t total points : {}", m_line_mesh->indices.size()));
  m_line_mesh->mesh_rt = GL_LINES;
  m_line_mesh->num_primitives = m_line_mesh->indices.size();
  m_line_mesh->bind_data();
}

void traj_hl_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void traj_hl_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

void traj_hl_t::updated_internally() {
}

