#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/ws_atoms_list_anim_subsys.hpp>
#include <qppcad/app_state.hpp>

namespace qpp {

  namespace cad {

    ws_atoms_list_anim_subsys_t::ws_atoms_list_anim_subsys_t(ws_atoms_list_t &_p_owner) {
      p_owner = &_p_owner;
    }

    void ws_atoms_list_anim_subsys_t::update_geom_to_anim(const int anim_id,
                                                          const float current_frame) {
      float start_frame = int(current_frame);
      float end_frame   = std::ceil(current_frame);
      float frame_delta = 1 - (current_frame - start_frame);
      int start_frame_n = int(start_frame);
      int end_frame_n   = int(end_frame);

      //TODO: throw
      if (anim_id > m_anim_data.size()) return;

      if (!m_rebuild_bonds_in_anim) p_owner->m_tws_tr->do_action(act_lock);
      else if (p_owner->m_geom->DIM > 0) p_owner->m_tws_tr->do_action(act_lock_img);

      size_t nat = p_owner->m_geom->nat();
      for (auto i = 0; i < p_owner->m_geom->nat(); i++){

          if (m_anim_data[anim_id].frames[start_frame_n].atom_pos.size() != nat) {
              m_force_non_animable = true;
              return;
            }

          vector3<float> new_pos =
              m_anim_data[anim_id].frames[start_frame_n].atom_pos[i] * (frame_delta) +
              m_anim_data[anim_id].frames[end_frame_n].atom_pos[i] * (1-frame_delta);

          p_owner->m_geom->change_pos(i, new_pos);

          if (p_owner->m_color_mode == ws_atoms_list_color_mode::color_from_xgeom) {
              // check the colors in frame_data are avaiable
              if (m_anim_data[anim_id].frames[start_frame_n].atom_color.size() == nat &&
                  m_anim_data[anim_id].frames[end_frame_n].atom_color.size() == nat) {
                  vector3<float> new_color =
                      m_anim_data[anim_id].frames[start_frame_n].atom_color[i] * (frame_delta) +
                      m_anim_data[anim_id].frames[end_frame_n].atom_color[i] * (1-frame_delta);
                  p_owner->m_geom->xfield<float>(xgeom_ccr, i) = new_color[0];
                  p_owner->m_geom->xfield<float>(xgeom_ccg, i) = new_color[1];
                  p_owner->m_geom->xfield<float>(xgeom_ccb, i) = new_color[2];
                }
              // otherwise - load default colors
              else {
                  p_owner->m_geom->xfield<float>(xgeom_ccr, i) = 0.0f;
                  p_owner->m_geom->xfield<float>(xgeom_ccg, i) = 0.0f;
                  p_owner->m_geom->xfield<float>(xgeom_ccb, i) = 0.0f;
                }
            }

          app_state_t* astate = app_state_t::get_inst();
          astate->make_viewport_dirty();

          if (p_owner->is_selected())
            astate->astate_evd->cur_ws_selected_item_frame_changed();
        }

      if (!m_rebuild_bonds_in_anim) p_owner->m_tws_tr->do_action(act_unlock);
      else if (p_owner->m_geom->DIM > 0) p_owner->m_tws_tr->do_action(act_unlock_img);
    }

    void ws_atoms_list_anim_subsys_t::update_and_set_anim(const int anim_id,
                                                          const float current_frame) {
      m_cur_anim = anim_id;
      m_cur_anim_time = current_frame;
      update_geom_to_anim(anim_id, current_frame);
    }

    void ws_atoms_list_anim_subsys_t::update_geom_to_anim() {
      update_geom_to_anim(m_cur_anim, m_cur_anim_time);
    }

    void ws_atoms_list_anim_subsys_t::update_current_frame_to(const int new_frame) {
      if (!animable()) return;
      if (m_cur_anim >= m_anim_data.size()) return;

      m_cur_anim_time = new_frame;
      update_geom_to_anim();
    }

    void ws_atoms_list_anim_subsys_t::update_current_frame_to_begin() {
      update_current_frame_to(0);
    }

    void ws_atoms_list_anim_subsys_t::update_current_frame_to_end() {
      update_current_frame_to(m_anim_data[m_cur_anim].frames.size()-1);
    }

    void ws_atoms_list_anim_subsys_t::update(const float delta_time) {

      app_state_t* astate = app_state_t::get_inst();

      if (m_cur_anim >= m_anim_data.size()) return; // wrong animation index
      if (m_anim_data[m_cur_anim].frames.empty()) return;

      //if (m_anim[m_cur_anim].frame_data[0].si)
      if (m_play_anim && animable()) {

          //astate->log("ANIM");

          m_cur_anim_time += 1 / (m_anim_frame_time * 60);
          if (m_cur_anim_time > m_anim_data[m_cur_anim].frames.size() - 1) {
              if (m_play_cyclic) m_cur_anim_time = 0.0f;
              else {
                  m_play_anim = false;
                  m_cur_anim_time = m_anim_data[m_cur_anim].frames.size() - 1;
                }
            } else {
              update_geom_to_anim(m_cur_anim, m_cur_anim_time);
            }

          //if current anim type equals static -> update to static and switch m_cur_anim_time = 0
          if (m_anim_data[m_cur_anim].m_anim_type == geom_anim_t::anim_static){
              m_cur_anim_time = 0.0f;
              m_play_anim = false;
              //m_play_cyclic = false;
            }
          //astate->make_viewport_dirty();
        }

    }

    bool ws_atoms_list_anim_subsys_t::animable() const {

      if (m_force_non_animable) return false;
      if (m_anim_data.empty()) return false;
      if (m_anim_data.size() == 1)
        if (m_anim_data[0].m_anim_type == geom_anim_t::anim_static) return false;
      for (auto &anim : m_anim_data)
        if (anim.frames.empty()) return false;
      return true;

    }

    void ws_atoms_list_anim_subsys_t::next_anim() {
      traverse_anim(1);
    }

    void ws_atoms_list_anim_subsys_t::prev_anim() {
      traverse_anim(-1);
    }

    void ws_atoms_list_anim_subsys_t::traverse_anim(int travel_dir) {

      int target_anim = m_cur_anim + travel_dir;
      int locked_target_anim = target_anim;
      bool need_to_update_anim = false;

      if (target_anim < 0 && m_anim_data.size() > 0) {
          locked_target_anim = int(m_anim_data.size()) - 1;
          need_to_update_anim = true;
        }

      if (target_anim >= m_anim_data.size() && m_anim_data.size() > 0 && !need_to_update_anim) {
          locked_target_anim = 0;
          need_to_update_anim = true;
        }

      if (target_anim >= 0 && target_anim < m_anim_data.size() && !need_to_update_anim) {
          locked_target_anim = target_anim;
          need_to_update_anim = true;
        }

      if (need_to_update_anim) {
          //          c_app::log(fmt::format("TRAVERSE ANIM ta={} lta={} asz={}",
          //                                 target_anim, locked_target_anim, get_total_anims()));

          m_cur_anim = locked_target_anim;
          m_cur_anim_time = 0.0f;
          update_geom_to_anim();
        }

    }


    void ws_atoms_list_anim_subsys_t::manual_frame_manipulate(const int frame_mod) {

      if (!animable()) return;
      if (m_cur_anim >= m_anim_data.size()) return;

      m_cur_anim_time += frame_mod;
      m_cur_anim_time = std::clamp(m_cur_anim_time,
                                   0.0f, float(m_anim_data[m_cur_anim].frames.size()- 1) );
      update_geom_to_anim();

    }

    geom_anim_record_t<float> *ws_atoms_list_anim_subsys_t::get_current_anim() {
      if (m_cur_anim < m_anim_data.size()) return &m_anim_data[m_cur_anim];
      else return nullptr;
    }

    geom_anim_t ws_atoms_list_anim_subsys_t::get_cur_anim_type() const {
      return m_anim_data[m_cur_anim].m_anim_type;
    }

    size_t ws_atoms_list_anim_subsys_t::get_total_anims() const {
      return m_anim_data.size();
    }

    size_t ws_atoms_list_anim_subsys_t::frame_count(const size_t anim_id) const {
      return m_anim_data[anim_id].frames.size();
    }

    size_t ws_atoms_list_anim_subsys_t::current_frame_count() const {
      return frame_count(m_cur_anim);
    }

  }

}
