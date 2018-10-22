#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/ws_atoms_list_anim_subsys.hpp>

namespace qpp {

  namespace cad {

    template <typename DATA, typename REAL>
    ws_atoms_list_anim_subsys_t<DATA, REAL>::ws_atoms_list_anim_subsys_t(DATA &_p_owner) {
      p_owner = &_p_owner;
    }

    template <typename DATA, typename REAL>
    void ws_atoms_list_anim_subsys_t<DATA, REAL>::update_geom_to_anim(const int anim_id,
                                                                      const REAL current_frame) {
      float start_frame = int(current_frame);
      float end_frame   = std::ceil(current_frame);
      float frame_delta = 1 - (current_frame - start_frame);
      int start_frame_n = int(start_frame);
      int end_frame_n   = int(end_frame);

      //TODO: throw
      if (anim_id > m_anim_data.size()) return;

      if (!m_rebuild_bonds_in_anim) p_owner->m_tws_tr->do_action(act_lock);
      else if (p_owner->m_geom->DIM > 0) p_owner->m_tws_tr->do_action(act_lock_img);

      for (auto i = 0; i < p_owner->m_geom->nat(); i++){

          if (m_anim_data[anim_id].frame_data[start_frame_n].size() != p_owner->m_geom->nat()){
              m_force_non_animable = true;
              return;
            }

          vector3<float> new_pos =
              m_anim_data[anim_id].frame_data[start_frame_n][i] * (frame_delta) +
              m_anim_data[anim_id].frame_data[end_frame_n][i] * (1-frame_delta);

          p_owner->m_geom->change_pos(i, new_pos);
        }

      if (!m_rebuild_bonds_in_anim) p_owner->m_tws_tr->do_action(act_unlock);
      else if (p_owner->m_geom->DIM > 0) p_owner->m_tws_tr->do_action(act_unlock_img);
    }

    template <typename DATA, typename REAL>
    void ws_atoms_list_anim_subsys_t<DATA, REAL>::update_geom_to_anim() {
      update_geom_to_anim(m_cur_anim, m_cur_anim_time);
    }

    template <typename DATA, typename REAL>
    void ws_atoms_list_anim_subsys_t<DATA, REAL>::update(const REAL delta_time) {

      app_state_t* astate = &(c_app::get_state());

      if (m_cur_anim >= m_anim_data.size()) return; // wrong animation index
      if (m_anim_data[m_cur_anim].frame_data.empty()) return;

      //if (m_anim[m_cur_anim].frame_data[0].si)
      if (m_play_anim && animable()) {

          m_cur_anim_time += 1 / (m_anim_frame_time * astate->current_fps);
          if (m_cur_anim_time > m_anim_data[m_cur_anim].frame_data.size() - 1) {
              if (m_play_cyclic) m_cur_anim_time = 0.0f;
              else {
                  m_play_anim = false;
                  m_cur_anim_time = m_anim_data[m_cur_anim].frame_data.size() - 1;
                }
            } else {
              update_geom_to_anim(m_cur_anim, m_cur_anim_time);
            }

          //if current anim type equals static -> update to static and switch m_cur_anim_time = 0
          if (m_anim_data[m_cur_anim].m_anim_type == geom_anim_type::anim_static){
              m_cur_anim_time = 0.0f;
              m_play_anim = false;
              m_play_cyclic = false;
            }
          astate->make_viewport_dirty();
        }

    }

    template <typename DATA, typename REAL>
    bool ws_atoms_list_anim_subsys_t<DATA, REAL>::animable() const {

      if (m_force_non_animable) return false;
      if (m_anim_data.empty()) return false;
      for (auto &anim : m_anim_data)
        if (anim.frame_data.empty()) return false;
      return true;

    }

    template <typename DATA, typename REAL>
    void ws_atoms_list_anim_subsys_t<DATA, REAL>::manual_frame_manipulate(const int frame_mod){

      if (!animable()) return;
      if (m_cur_anim >= m_anim_data.size()) return;

      m_cur_anim_time += frame_mod;
      m_cur_anim_time = std::clamp(m_cur_anim_time,
                                   0.0f, float(m_anim_data[m_cur_anim].frame_data.size()- 1) );
      update_geom_to_anim();

    }

    template <typename DATA, typename REAL>
    geom_anim_type ws_atoms_list_anim_subsys_t<DATA, REAL>::get_cur_anim_type() const {
      return m_anim_data[m_cur_anim].m_anim_type;
    }

    template <typename DATA, typename REAL>
    size_t ws_atoms_list_anim_subsys_t<DATA, REAL>::get_total_anims() const {
      return m_anim_data.size();
    }

    template <typename DATA, typename REAL>
    size_t ws_atoms_list_anim_subsys_t<DATA, REAL>::frame_count(const size_t anim_id) const {
      return m_anim_data[anim_id].frame_data.size();
    }

    template <typename DATA, typename REAL>
    size_t ws_atoms_list_anim_subsys_t<DATA, REAL>::current_frame_count() const {
      return frame_count(m_cur_anim);
    }


    template class ws_atoms_list_anim_subsys_t<ws_atoms_list_t, float>;
  }

}
