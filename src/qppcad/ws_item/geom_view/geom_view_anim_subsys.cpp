#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

geom_view_anim_subsys_t::geom_view_anim_subsys_t(geom_view_t &_p_owner) {
  p_owner = &_p_owner;
  m_anim_frame_time.set_value(0.25f); add_hs_child(&m_anim_frame_time);
  m_rebuild_bonds_in_anim.set_value(true); add_hs_child(&m_rebuild_bonds_in_anim);
  m_play_cyclic.set_value(true); add_hs_child(&m_play_cyclic);
  m_play_anim.set_value(false); add_hs_child(&m_play_anim);
  m_interpolate_anim.set_value(true); add_hs_child(&m_interpolate_anim);
}

void geom_view_anim_subsys_t::update_geom_to_anim(const int anim_id, const float current_frame) {
  float start_frame = int(current_frame);
  float end_frame   = std::ceil(current_frame);
  float frame_delta = 1 - (current_frame - start_frame);
  int start_frame_n = int(start_frame);
  int end_frame_n   = int(end_frame);

  app_state_t* astate = app_state_t::get_inst();

  //TODO: throw
  if (anim_id > m_anim_data.size()) return;

  bool is_variable_cell_anim =
      m_anim_data[anim_id].m_variable_cell_anim && p_owner->m_geom->get_DIM() > 0;

  // tws_tree setup before modification the geometry
  if (is_variable_cell_anim) {
    p_owner->m_tws_tr->do_action(act_lock | act_clear_all | act_clear_img);
    // update cell
    for (size_t vc_i = 0; vc_i < p_owner->m_geom->get_DIM(); vc_i++)
      p_owner->m_geom->cell.v[vc_i] =
          m_anim_data[anim_id].frames[start_frame_n].m_cell[vc_i] * (frame_delta) +
          m_anim_data[anim_id].frames[end_frame_n].m_cell[vc_i]  * (1-frame_delta);
  } else {
    if (!m_rebuild_bonds_in_anim.get_value())
      p_owner->m_tws_tr->do_action(act_lock);
    else if (p_owner->m_geom->get_DIM() > 0)
      p_owner->m_tws_tr->do_action(act_lock_img);
  }

  size_t nat = p_owner->m_geom->nat();
  if (m_anim_data[anim_id].frames[start_frame_n].atom_pos.size() != nat
      || m_anim_data[anim_id].frames[end_frame_n].atom_pos.size() != nat) {
    m_force_non_animable = true;
    return;
  }

  for (auto i = 0; i < p_owner->m_geom->nat(); i++) { // update atom data
    vector3<float> new_pos =
        m_interpolate_anim.get_value() ?
          m_anim_data[anim_id].frames[start_frame_n].atom_pos[i] * (frame_delta) +
          m_anim_data[anim_id].frames[end_frame_n].atom_pos[i] * (1-frame_delta) :
          m_anim_data[anim_id].frames[start_frame_n].atom_pos[i];

    p_owner->m_geom->change_pos(i, new_pos);

    if (p_owner->m_color_mode.get_value() == geom_view_color_e::color_from_xgeom) {
      // check that colors in frame_data are avaiable
      if (m_anim_data[anim_id].frames[start_frame_n].atom_color.size() == nat &&
          m_anim_data[anim_id].frames[end_frame_n].atom_color.size() == nat) {
        vector3<float> new_color =
            m_anim_data[anim_id].frames[start_frame_n].atom_color[i] * (frame_delta) +
            m_anim_data[anim_id].frames[end_frame_n].atom_color[i] * (1-frame_delta);
        p_owner->m_geom->xfield<float>(xg_ccr, i) = new_color[0];
        p_owner->m_geom->xfield<float>(xg_ccg, i) = new_color[1];
        p_owner->m_geom->xfield<float>(xg_ccb, i) = new_color[2];
      }
      // otherwise - load default colors
      else {
        p_owner->m_geom->xfield<float>(xg_ccr, i) = 0.0f;
        p_owner->m_geom->xfield<float>(xg_ccg, i) = 0.0f;
        p_owner->m_geom->xfield<float>(xg_ccb, i) = 0.0f;
      }
    }
  }  // end of update atom data

  if (is_variable_cell_anim) {
    p_owner->m_tws_tr->do_action(act_unlock | act_rebuild_tree | act_unlock_img);
    p_owner->m_tws_tr->do_action(act_rebuild_all);
  } else {
    if (!m_rebuild_bonds_in_anim.get_value())
      p_owner->m_tws_tr->do_action(act_unlock);
    else if (p_owner->m_geom->get_DIM() > 0)
      p_owner->m_tws_tr->do_action(act_unlock_img);
  }

  p_owner->call_followers();
  astate->make_viewport_dirty();

  if (p_owner->is_selected())
    astate->astate_evd->cur_ws_selected_item_frame_changed();
}

void geom_view_anim_subsys_t::update_and_set_anim(const int anim_id,
                                                  const float current_frame) {
  m_cur_anim = anim_id;
  m_cur_anim_time = current_frame;
  update_geom_to_anim(anim_id, current_frame);
}

void geom_view_anim_subsys_t::update_geom_to_anim() {
  update_geom_to_anim(m_cur_anim, m_cur_anim_time);
}

void geom_view_anim_subsys_t::update_current_frame_to(const int new_frame) {
  if (!animable())
    return;
  if (m_cur_anim >= m_anim_data.size())
    return;
  if (get_current_anim()->m_anim_type == geom_anim_e::anim_static)
    return;

  m_cur_anim_time = new_frame;
  update_geom_to_anim();
}

void geom_view_anim_subsys_t::update_current_frame_to_begin() {
  update_current_frame_to(0);
}

void geom_view_anim_subsys_t::update_current_frame_to_end() {
  update_current_frame_to(m_anim_data[m_cur_anim].frames.size()-1);
}

void geom_view_anim_subsys_t::update(const float delta_time) {
  ws_item_subsystem_t::update(delta_time);

  if (m_cur_anim >= m_anim_data.size())
    return; // wrong animation index
  if (m_anim_data[m_cur_anim].frames.empty())
    return;
  if (!animable())
    return;
  if (!m_play_anim.get_value())
    return;

  m_cur_anim_time += 1 / (m_anim_frame_time.get_value() * 60);
  if (m_cur_anim_time > m_anim_data[m_cur_anim].frames.size() - 1) {
    if (m_play_cyclic.get_value()) {
      m_cur_anim_time = 0.0f;
    } else {
      m_play_anim.set_value(false);
      m_cur_anim_time = m_anim_data[m_cur_anim].frames.size() - 1;
    }
  } else {
    update_geom_to_anim(m_cur_anim, m_cur_anim_time);
  }

  //if current anim type equals static -> update to static and switch m_cur_anim_time = 0
  if (m_anim_data[m_cur_anim].m_anim_type == geom_anim_e::anim_static) {
    m_cur_anim_time = 0.0f;
    m_play_anim.set_value(false);
    //m_play_cyclic = false;
  }
  //astate->make_viewport_dirty();
}

bool geom_view_anim_subsys_t::animable() const {
  if (m_force_non_animable)
    return false;
  if (m_anim_data.empty())
    return false;

  //  TODO: check everything is good
  //  if (m_anim_data.size() == 1)
  //    if (m_anim_data[0].m_anim_type == geom_anim_t::anim_static) return false;

  bool has_empty_anims = std::any_of(m_anim_data.cbegin(), m_anim_data.cend(),
                                     [](const geom_anim_record_t<float> &rec) -> bool
  {return rec.frames.empty();});
  return !has_empty_anims;
}

void geom_view_anim_subsys_t::next_anim() {
  traverse_anim(1);
}

void geom_view_anim_subsys_t::prev_anim() {
  traverse_anim(-1);
}

void geom_view_anim_subsys_t::traverse_anim(int travel_dir) {
  app_state_t* astate = app_state_t::get_inst();

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

  if (p_owner->m_selected)
    astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();
}

void geom_view_anim_subsys_t::make_interpolated_anim(std::string new_anim_name,
                                                     size_t num_frames,
                                                     size_t anim1_id,
                                                     size_t anim1_frm,
                                                     size_t anim2_id,
                                                     size_t anim2_frm,
                                                     bool bounce) {
  app_state_t* astate = app_state_t::get_inst();

  auto validate_anim_idx =
      [this](size_t _anim_n_id, size_t _anim_n_frm, size_t _order) {

    if (_anim_n_id >= this->m_anim_data.size()) {
      throw std::out_of_range(fmt::format("invalid anim{} index={}", _order, _anim_n_id));
    }

    if (_anim_n_frm >= this->m_anim_data[_anim_n_id].frames.size())
      throw std::out_of_range(fmt::format("invalid anim{} index={} frame={} of total={}",
                                          _order,
                                          _anim_n_id,
                                          _anim_n_frm,
                                          this->m_anim_data[_anim_n_id].frames.size())
                              );

    size_t _frame_data_size =
        this->m_anim_data[_anim_n_id].frames[_anim_n_frm].atom_pos.size();

    if (_frame_data_size != this->p_owner->m_geom->nat())
      throw std::out_of_range(fmt::format("invalid anim{} index={} frame={} framedata={}",
                                          _order,
                                          _anim_n_id,
                                          _anim_n_frm,
                                          _frame_data_size)
                              );

  };

  validate_anim_idx(anim1_id, anim1_frm, 1);
  validate_anim_idx(anim2_id, anim2_frm, 2);

  if (num_frames <= 1) {
    throw std::out_of_range("invalid number of frames!");
  }

  // validate target frames
  auto &rec1 = m_anim_data[anim1_id];
  auto &rec2 = m_anim_data[anim2_id];

  auto &r_fr1 = rec1.frames[anim1_frm];
  auto &r_fr2 = rec2.frames[anim2_frm];

  geom_anim_record_t<float> new_anim;
  new_anim.m_anim_name = new_anim_name;
  new_anim.m_anim_type = geom_anim_e::anim_generic;

  new_anim.frames.resize(num_frames);

  size_t tot_at = p_owner->m_geom->nat();

  for (size_t i = 0; i < num_frames; i++) {
    new_anim.frames[i].atom_pos.resize(tot_at);
    float alpha = 1 - ((i) / float(num_frames-1));
    for (size_t q = 0; q < tot_at; q++) {
      vector3<float> s_fr = r_fr1.atom_pos[q];
      vector3<float> e_fr = r_fr2.atom_pos[q];
      new_anim.frames[i].atom_pos[q] = s_fr * (alpha) + e_fr * (1-alpha);
    }
  }

  m_anim_data.emplace_back(std::move(new_anim));

  if (p_owner->is_selected())
    astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();
}

void geom_view_anim_subsys_t::make_animable() {
  app_state_t* astate = app_state_t::get_inst();

  if (m_anim_data.empty()) {
    geom_anim_record_t<float> new_static_rec;
    new_static_rec.m_anim_type = geom_anim_e::anim_generic;
    new_static_rec.m_anim_name = "static";
    m_anim_data.emplace_back(std::move(new_static_rec));
  }

  //iterate over all anims and delete wrong anims
  for (auto &anim : m_anim_data) {
    if (anim.frames.empty())
      anim.frames.resize(1);
    for (auto &frame : anim.frames) {
      if (frame.atom_pos.size() != static_cast<size_t>(p_owner->m_geom->nat())) {
        frame.atom_pos.resize(p_owner->m_geom->nat());
        for (size_t i = 0; i < static_cast<size_t>(p_owner->m_geom->nat()); i++)
          frame.atom_pos[i] = p_owner->m_geom->coord(i);
      }
    }
  }

  m_force_non_animable = false;

  astate->make_viewport_dirty();
  astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();
}

void geom_view_anim_subsys_t::make_anim(const std::string &anim_name,
                                        const geom_anim_e anim_type,
                                        const size_t num_frames) {
  geom_anim_record_t<float> new_anim_rec;
  new_anim_rec.m_anim_name = anim_name;
  new_anim_rec.m_anim_type = anim_type;
  new_anim_rec.frames.resize(num_frames);
  m_anim_data.emplace_back(std::move(new_anim_rec));
  make_animable();
}

void geom_view_anim_subsys_t::make_static_anim(bool do_it_anyway) {
  auto static_it = std::find_if(std::cbegin(m_anim_data), std::cend(m_anim_data),
                                [](const geom_anim_record_t<float> &rec)
                                 {return rec.m_anim_type == geom_anim_e::anim_static;});

  if (static_it != std::cend(m_anim_data) && !do_it_anyway)
    return;

  geom_anim_record_t<float> new_anim_rec;
  new_anim_rec.m_anim_name = fmt::format("static{}", m_anim_data.size());
  new_anim_rec.m_anim_type = geom_anim_e::anim_static;

  new_anim_rec.frames.resize(1);
  new_anim_rec.frames[0].atom_pos.resize(p_owner->m_geom->nat());

  for (size_t i = 0; i < static_cast<size_t>(p_owner->m_geom->nat()); i++)
    new_anim_rec.frames[0].atom_pos[i] = p_owner->m_geom->pos(i);

  m_anim_data.emplace_back(std::move(new_anim_rec));

  make_animable();
}

int geom_view_anim_subsys_t::current_frame_truncated() {
  return std::floor(p_owner->m_anim->m_cur_anim_time);
}

void geom_view_anim_subsys_t::commit_atom_pos(size_t atom_id,
                                              bool lerp_from_start,
                                              bool propagate_to_the_end) {
  if (!animable()) {
    throw std::runtime_error("Object is not animable!");
  }

  if (m_cur_anim >= m_anim_data.size()) {
    throw std::out_of_range(fmt::format("Invalid anim m_cur_anim={}, m_anim_data.size={}",
                                        m_cur_anim,
                                        m_anim_data.size()));
  }

  auto cur_anim = get_current_anim();

  if (!cur_anim) {
    throw std::runtime_error("cur_anim is not valid!");
  }

  int current_frame = current_frame_truncated();

  if (current_frame >= cur_anim->frames.size()) {
    throw std::out_of_range(fmt::format("Invalid anim current_frame={}, "
                                        "cur_anim->frames.size={}",
                                        m_cur_anim,
                                        m_anim_data.size()));
  }

  int c_start_frame = current_frame;
  int c_end_frame = propagate_to_the_end ? cur_anim->frames.size() : current_frame + 1;

  for (int i = c_start_frame; i < c_end_frame; i++) {
    if (cur_anim->frames[i].atom_pos.size() <= atom_id)
      throw std::out_of_range(fmt::format("cur_anim->frames[{}].atom_pos not populated!", i));
    cur_anim->frames[i].atom_pos[atom_id] = p_owner->m_geom->coord(atom_id);
  }

  for (int i = 0; i < c_start_frame; i++) {
    if (cur_anim->frames[i].atom_pos.size() <= atom_id)
      throw std::out_of_range(fmt::format("cur_anim->frames[{}].atom_pos not populated!", i));
    if (cur_anim->frames[c_start_frame].atom_pos.size() <= atom_id)
      throw std::out_of_range(fmt::format("cur_anim->frames[{}].atom_pos not populated!",
                                          c_start_frame));
    float i_c = i / (c_start_frame - i);
    cur_anim->frames[i].atom_pos[atom_id] = cur_anim->frames[i].atom_pos[atom_id] +
                                            (cur_anim->frames[i].atom_pos[atom_id]
                                            - cur_anim->frames[i].atom_pos[atom_id]) * i_c;
  }
}

void geom_view_anim_subsys_t::manual_frame_manipulate(const int frame_mod) {
  if (!animable()) {
    throw std::runtime_error("Object is not animable!");
  }
  if (m_cur_anim >= m_anim_data.size())
    return;
  m_cur_anim_time += frame_mod;
  m_cur_anim_time = std::clamp(m_cur_anim_time,
                               0.0f,
                               float(m_anim_data[m_cur_anim].frames.size()- 1));
  update_geom_to_anim();
}

geom_anim_record_t<float> *geom_view_anim_subsys_t::get_current_anim() {
  if (m_cur_anim < m_anim_data.size())
    return &m_anim_data[m_cur_anim];
  else
    return nullptr;
}

geom_anim_e geom_view_anim_subsys_t::get_cur_anim_type() const {
  return m_anim_data[m_cur_anim].m_anim_type;
}

geom_anim_e geom_view_anim_subsys_t::get_anim_type_by_idx(size_t anim_idx) const {
  if (anim_idx < m_anim_data.size()) {
    return m_anim_data[anim_idx].m_anim_type;
  } else {
    throw std::out_of_range("invalid animation index");
    return geom_anim_e::anim_generic;
  }
}

size_t geom_view_anim_subsys_t::get_total_anims() const {
  return m_anim_data.size();
}

size_t geom_view_anim_subsys_t::frame_count(const size_t anim_id) const {
  return m_anim_data[anim_id].frames.size();
}

size_t geom_view_anim_subsys_t::current_frame_count() const {
  return frame_count(m_cur_anim);
}

bool geom_view_anim_subsys_t::is_cell_animable(size_t anim_id, size_t frame_id) {

  if (anim_id >= m_anim_data.size())
    return false;
  if (frame_id >= m_anim_data[anim_id].frames.size())
    return false;
  return m_anim_data[anim_id].frames[frame_id].m_cell_is_animable;

}

vector3<float> geom_view_anim_subsys_t::get_cell_vectors(size_t anim_id,
                                                         size_t frame_id,
                                                         size_t cell_id) {
  if (is_cell_animable(anim_id, frame_id))
    return m_anim_data[anim_id].frames[frame_id].m_cell[cell_id];

  if (p_owner->m_geom->get_DIM() >= cell_id)
    return p_owner->m_geom->cell.v[cell_id];

  return vector3<float>{0};
}

vector3<float> geom_view_anim_subsys_t::get_atom_pos(size_t anim_id,
                                                     size_t frame_id,
                                                     size_t atom_id) {
  if (anim_id >= m_anim_data.size())
    return vector3<float>{0};

  if (frame_id >= m_anim_data[anim_id].frames.size())
    return vector3<float>{0};

  if (atom_id >= m_anim_data[anim_id].frames[frame_id].atom_pos.size())
    return vector3<float>{0};

  return m_anim_data[anim_id].frames[frame_id].atom_pos[atom_id];
}
