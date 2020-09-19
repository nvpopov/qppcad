#include <qppcad/core/workspace.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/core/app_state.hpp>
#include <data/ptable.hpp>
#include <mathf/math.hpp>
#include <qppcad/core/register_all_things.hpp>
#include <qppcad/ws_item/sflow_register.hpp>
#include <io/parsing_exceptions.hpp>
#include <qppcad/ui/qrich_error_message_box.hpp>
#include <qppcad/core/json_helpers.hpp>

#include <QDir>

#include <symm/shoenflis.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/node_book/node_book.hpp>

using namespace qpp;
using namespace qpp::cad;

workspace_t::workspace_t(std::string _ws_name) {

  m_ws_name = _ws_name;
  m_camera = std::make_unique<camera_t>();
  m_camera->reset_camera();
  m_gizmo = std::make_unique<gizmo_t>();

  //m_cur_itm.set_value(-1);

  begin_recording(hs_doc_rec_type_e::hs_doc_rec_init);
  add_hs_child(&m_ws_items);
  add_hs_child(&m_cur_itm);
  m_cur_itm.set_value(-1);
  end_recording();
  //m_cur_itm.set_commit_exclusive_on_change(true);

}

bool workspace_t::is_current() {
  return p_mgr ? (p_mgr->get_cur_rptr() == this ? true : false) : false;
}

void workspace_t::set_mgr(workspace_manager_t *new_wsm) {
  p_mgr = new_wsm;
}

workspace_manager_t *workspace_t::get_mgr() {
  return p_mgr;
}

std::optional<size_t> workspace_t::get_sel_idx() {

  for (size_t i = 0; i < num_items(); i++) {
    auto ws_item = m_ws_items.get_hs_child_as_array(i).get();
    if (!ws_item)
      continue;
    if (ws_item->m_selected)
      return std::optional<size_t>(i);
  }

  return std::nullopt;

}

ws_item_t *workspace_t::get_sel() {
  std::optional<size_t> sel_idx = get_sel_idx();
  if (sel_idx)
    return m_ws_items.get_hs_child_as_array(*sel_idx).get();
  else
    return nullptr;
}

std::shared_ptr<ws_item_t> workspace_t::get_sel_sp() {
  std::optional<size_t> sel_idx = get_sel_idx();
  if (sel_idx)
    return m_ws_items.get_hs_child_as_array(*sel_idx);
  else
    return nullptr;
}

std::shared_ptr<ws_item_t> workspace_t::get_by_name(std::string name) {

  for (size_t i = 0; i < num_items(); i++) {
    auto ws_item = m_ws_items.get_hs_child_as_array(i);
    if (!ws_item)
      continue;
    if (ws_item->m_name.get_value() == name)
      return ws_item;
  }

  return nullptr;

}

std::optional<size_t> workspace_t::get_item_idx(ws_item_t *item) {

  for (size_t i = 0; i < num_items(); i++) {
    auto ws_item = m_ws_items.get_hs_child_as_array(i).get();
    if (!ws_item)
      continue;
    if (ws_item == item)
      return std::optional{i};
  }

  return std::nullopt;

}

bool workspace_t::set_sel_item(const size_t sel_idx, bool emit_signal, bool emit_hs_event) {

  app_state_t* astate = app_state_t::get_inst();
  p_inside_selection_event = true;

  unsel_all();

  astate->tlog("\n Setting selected item in ws \"{0}\" to {1}\n"
               " emit_signal = {2}, emit_hs_event={3}",
               m_ws_name, sel_idx, emit_signal, emit_hs_event);

  if (sel_idx < num_items() && num_items() != 0) {
    auto ws_item = m_ws_items.get_hs_child_as_array(sel_idx).get();
    if (!ws_item) {
      p_inside_selection_event = false;
      return false;
    }
    ws_item->m_selected = true;
    if (emit_hs_event) {
      assert(m_cur_itm.commit_value_exclusive(sel_idx) == hs_result_e::hs_success);
      astate->tlog("@@@ DEBUG: sel_idx = {}, m_cur_itm.val == {}",
                   sel_idx,
                   m_cur_itm.get_value());
    }
    if (ws_item->get_flags() & ws_item_flags_support_tr) {
      m_gizmo->attached_item = ws_item;
      m_gizmo->update_gizmo(0.1f, true);
      astate->make_viewport_dirty();
    } else {
      m_gizmo->attached_item = nullptr;
      m_gizmo->update_gizmo(0.1f, true);
    }
    if (emit_signal)
      astate->astate_evd->cur_ws_selected_item_changed();
    update_overview(ws_item->compose_overview());
    p_inside_selection_event = false;
    return true;
  }

  //nothing selected
  update_overview("");

  if (emit_signal)
    astate->astate_evd->cur_ws_selected_item_changed();

  if (emit_hs_event)
    m_cur_itm.commit_value_exclusive(-1);

  p_inside_selection_event = false;
  return false;

}

bool workspace_t::set_sel_item(ws_item_t *item, bool emit_signal, bool emit_hs_event) {

  auto itm_idx = get_item_idx(item);
  if (!itm_idx)
    return false;

  return set_sel_item(*itm_idx, emit_signal, emit_hs_event);

}

void workspace_t::next_item() {
  size_t target_id = get_sel_idx().value_or(0) + 1;
  if (target_id >= num_items())
    target_id = 0;
  set_sel_item(target_id);
}

void workspace_t::prev_item() {
  int target_id = get_sel_idx().value_or(0) - 1;
  if (target_id < 0)
    target_id = num_items() - 1;
  set_sel_item(target_id);
}

void workspace_t::unsel_all(bool emit_signal) {

  for (size_t i = 0; i < num_items(); i++) {
    auto ws_item = m_ws_items.get_hs_child_as_array(i).get();
    if (!ws_item)
      continue;
    ws_item->m_selected = false;
  }

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->request_update_overview("");

  if (emit_signal)
    astate->astate_evd->cur_ws_selected_item_changed();

}

void workspace_t::toggle_edit_mode() {

  app_state_t* astate = app_state_t::get_inst();

  if (p_edit_type == ws_edit_e::edit_item) {
    p_edit_type = ws_edit_e::edit_content;
  } else {
    p_edit_type = ws_edit_e::edit_item;
  }

  astate->astate_evd->cur_ws_edit_type_changed();

}

size_t workspace_t::num_items() {
  return m_ws_items.get_hs_children_count();
}

void workspace_t::reset_cam() {
  m_camera->reset_camera();
  set_best_view();
}

void workspace_t::set_best_view() {

  if (num_items() == 0) {
    m_camera->reset_camera();
    m_camera->update_camera();
    return;
  }

  vector3<float> vec_look_at{0.0, 0.0, 0.0};
  vector3<float> vec_look_pos{0.0, 0.0, 0.0};
  vector3<float> vec_look_up{0.0, 0.0, 0.0};

  // special case : there is only one element in the workspace that supports cam_target_view
  int num_items_tv = 0;
  bool need_to_update_camera{false};
  bool cam_staged{false};

  std::shared_ptr<ws_item_t> acc_item{nullptr};

  for (size_t i = 0; i < num_items(); i++) {
    auto ws_item = m_ws_items.get_hs_child_as_array(i);
    if (!ws_item) continue;
    if (ws_item->get_flags() & ws_item_flags_cam_target_view) {
      num_items_tv++;
      acc_item = ws_item;
    }
  }

  if (num_items_tv == 1 && acc_item) {

    acc_item->target_view(cam_tv_e::tv_auto,
                          vec_look_pos,
                          vec_look_at,
                          vec_look_up,
                          need_to_update_camera);

    if (need_to_update_camera) cam_staged = true;

  }

  // if special case failed fallback to vote view
  if (!cam_staged) {
    size_t total_voters = 0;
    for (size_t i = 0; i < num_items(); i++) {
      auto ws_item = m_ws_items.get_hs_child_as_array(i);
      if (!ws_item) continue;
      if (ws_item->get_flags() & ws_item_flags_support_view_voting) {
        total_voters+=1;
        ws_item->vote_for_view_vectors(vec_look_pos, vec_look_at);
      }
    }

    total_voters = std::clamp<size_t>(total_voters, 1, 20);

    vec_look_at  /= total_voters;
    vec_look_pos /= total_voters;

  }

  m_camera->m_cam_state.m_look_at = vec_look_at;
  m_camera->m_cam_state.m_view_point = vec_look_pos;

  m_camera->orthogonalize_gs();
  m_camera->update_camera();

  if ((m_camera->m_cam_state.m_look_at-m_camera->m_cam_state.m_view_point).norm() < 0.4f
      || vec_look_at == vec_look_pos)
    m_camera->reset_camera();

}

hs_result_e workspace_t::on_epoch_changed(hs_doc_base_t::epoch_t prev_epoch) {

  app_state_t* astate = app_state_t::get_inst();
  astate->make_viewport_dirty();

  bool affected{false};

  size_t alive_cnt_before{0};
  size_t alive_cnt_after{0};

  epoch_t cur_epoch = get_cur_epoch();

  auto cur_ws = astate->ws_mgr->get_cur_ws();

  if (cur_ws && cur_ws.get() == this) {

    for (size_t i = 0; i < m_ws_items.get_hs_children_count(); i++) {

      auto itm = m_ws_items.get_hs_child_as_array(i);
      if (!itm)
        continue;

      if (itm->is_selected())
        affected = true;

      if (m_ws_items.is_child_alive(prev_epoch, itm) == hs_result_e::hs_alive)
        alive_cnt_before++;

      if (m_ws_items.is_child_alive(cur_epoch, itm)  == hs_result_e::hs_alive)
        alive_cnt_after++;

    }

    astate->tlog("\n Epoch changed in workspace {0}\n"
                 " alive_cnt_bef = {1}, alive_cnt_aft = {2}, prev_epoch = {3}, cur_epoch = {4}",
                 m_ws_name, alive_cnt_before, alive_cnt_after, prev_epoch, cur_epoch);

    m_cur_itm.set_commit_exclusive_on_change(false);
    astate->tlog("@@@ OnEPChanged, m_cur_itm = {}", m_cur_itm.get_value());
    if (!p_inside_selection_event) {
      if (m_cur_itm.get_value() == -1) {
        unsel_all(true);
      } else {
        astate->tlog("Epoch changed in workspace {} -> set_sel_item {}",
                     m_ws_name, m_cur_itm.get_value());
        set_sel_item(m_cur_itm.get_value(), true, false);
      }
    }
    m_cur_itm.set_commit_exclusive_on_change(true);

    if (alive_cnt_after != alive_cnt_before)
      astate->astate_evd->cur_ws_content_changed_signal();
    if (affected)
      astate->astate_evd->cur_ws_selected_item_changed();

  }

  return hs_result_e::hs_success;

}

void workspace_t::render() {

  app_state_t* astate = app_state_t::get_inst();

  if (m_gizmo->m_is_active && m_gizmo->attached_item)
    m_gizmo->render();

  if (astate->m_debug_show_selection_ray) {
    astate->dp->begin_render_line();
    astate->dp->render_line({1.0, 1.0, 0.0}, m_ray.start, m_ray.start + m_ray.dir * 155.0);
    astate->dp->end_render_line();
  }

  if (astate->dp) {

    if (astate->m_show_axis) { // Draw axis

      vector3<float> vScrTW = astate->camera->unproject(-0.92f, -0.90f);
      float axis_magn =
          astate->camera->m_cur_proj ==
          cam_proj_t::proj_persp ? 0.07f *astate->camera->m_cam_state.m_stored_dist
                                 : m_camera->m_cam_state.m_ortho_scale * 0.1f;

      astate->dp->begin_render_line();

      astate->dp->render_line(vector3<float>(1.0, 0.0, 0.0),
                              vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                              vector3<float>(axis_magn, 0.0, 0.0) + vScrTW);

      astate->dp->render_line(vector3<float>(0.0, 1.0, 0.0),
                              vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                              vector3<float>(0.0, axis_magn, 0.0) + vScrTW);

      astate->dp->render_line(vector3<float>(0.0, 0.0, 1.0),
                              vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                              vector3<float>(0.0, 0.0, axis_magn) + vScrTW);

      astate->dp->end_render_line();

    } // Draw axis end

  }

  for (size_t i = 0; i < num_items(); i++) {
    auto ws_item = m_ws_items.get_hs_child_as_array(i);
    if (!ws_item)
      continue;
    ws_item->render();
  }

}

void workspace_t::render_overlay(QPainter &painter) {
  for (size_t i = 0; i < num_items(); i++) {
    auto ws_item = m_ws_items.get_hs_child_as_array(i);
    if (!ws_item)
      continue;
    if (ws_item->m_is_visible.get_value())
      ws_item->render_overlay(painter);
  }
}

void workspace_t::mouse_click(const float mouse_x, const float mouse_y) {

  app_state_t* astate = app_state_t::get_inst();

  if (m_camera->m_cur_proj == cam_proj_t::proj_persp) {
    m_ray.start = m_camera->m_cam_state.m_view_point;
    m_ray.dir =
        (m_camera->unproject(mouse_x, mouse_y) - m_camera->m_cam_state.m_view_point).normalized();
  } else {
    float z_p = (m_camera->m_cam_state.m_znear_ortho + m_camera->m_cam_state.m_zfar_ortho)
                / (m_camera->m_cam_state.m_znear_ortho - m_camera->m_cam_state.m_zfar_ortho);

    m_ray.start = m_camera->unproject(mouse_x, mouse_y, z_p);
    m_ray.dir = (m_camera->m_cam_state.m_look_at - m_camera->m_cam_state.m_view_point).normalized();
  }

  if (m_gizmo->m_is_visible && m_gizmo->attached_item && m_gizmo->process_ray(&m_ray)) {
    astate->tlog("[UI] gizmo clicked");
    return;
  }

  bool hit_any = false;

  int sel_idx{-1};
  if (p_edit_type != ws_edit_e::edit_content) {
    for (size_t i = 0; i < num_items(); i++) {
      auto ws_item = m_ws_items.get_hs_child_as_array(i);
      if (!ws_item)
        continue;
      if (ws_item->m_selected)
        sel_idx = i;
      ws_item->m_selected = false;
    }
    m_gizmo->attached_item = nullptr;
  }

  for (size_t i = 0; i < num_items(); i++) {

    auto ws_item = m_ws_items.get_hs_child_as_array(i);
    if (!ws_item)
      continue;

    bool is_hit = ws_item->mouse_click(&m_ray);
    hit_any = hit_any || is_hit;

    if (is_hit
        && p_edit_type == ws_edit_e::edit_item
        && ws_item->m_is_visible.get_value()
        && (ws_item->get_flags() & ws_item_flags_support_sel)) {

      m_gizmo->attached_item = ws_item.get();
      set_sel_item(i);
      break;

    }

  } // end of for (auto &ws_item : m_ws_items)

  if (p_edit_type != ws_edit_e::edit_content && !hit_any) {
    m_gizmo->attached_item = nullptr;
    unsel_all();
  }

  //the item was unselected, we need to commit -1 to m_cur_itm proxy
  if (sel_idx != -1 && !hit_any) {
    m_cur_itm.commit_value_exclusive(-1);
  }

}

void workspace_t::mouse_double_click(const float mouse_x, const float mouse_y) {

  auto cur_it = get_sel();
  if (!cur_it)
    return;

  if (p_edit_type == ws_edit_e::edit_item) {
    cur_it->mouse_double_click(nullptr);
    return;
  }

  if (cur_it->get_num_cnt_selected() == 0 && p_edit_type == ws_edit_e::edit_content) {
    set_edit_type(ws_edit_e::edit_item);
  }

}

void workspace_t::add_item_to_ws(std::shared_ptr<ws_item_t> item_to_add, bool add_new_epoch) {

  app_state_t *astate = app_state_t::get_inst();
  item_to_add->m_parent_ws = this;
  m_ws_items.add_hs_child_as_array(item_to_add, add_new_epoch);

  astate->tlog("Adding item \"{}\" to workspace \"{}\", type = {}, add_new_epoch = {}",
               item_to_add ? item_to_add->m_name.get_value() : "None",
               m_ws_name,
               item_to_add ? item_to_add->get_type_name() : "None",
               add_new_epoch);

  astate->astate_evd->cur_ws_changed();

}

void workspace_t::update_overview(const std::string &overview_text) {

  app_state_t *astate = app_state_t::get_inst();
  astate->astate_evd->request_update_overview(overview_text);

}

void workspace_t::clear_connected_items(std::shared_ptr<ws_item_t> item_to_delete) {
  for (size_t i = 0; i < num_items(); i++) {
    auto ws_item = m_ws_items.get_hs_child_as_array(i);
    if (!ws_item)
      continue;
    auto it = std::find(begin(ws_item->m_connected_items),
                        end(ws_item->m_connected_items),
                        item_to_delete);
    if (it != end(ws_item->m_connected_items))
      ws_item->m_connected_items.erase(it);
  }
}

void workspace_t::save_ws_to_json(const std::string filename) {

  app_state_t* astate = app_state_t::get_inst();

  std::ofstream out_file(filename);
  json data;

  data[JSON_QPPCAD_VERSION] = "1.0-aa";

  json_io::save_var(JSON_WS_NAME, m_ws_name, data);
  json_io::save_vec3(JSON_BG_CLR, m_bg_color, data);

  json camera_data;
  m_camera->save_to_json(camera_data);
  data[JSON_WS_CAMERA] = camera_data;

  json ws_objects = json::array({});

  for (size_t i = 0; i < num_items(); i++) {

    auto ws_item = m_ws_items.get_hs_child_as_array(i);
    if (!ws_item)
      continue;

    json ws_object;
    ws_item->save_to_json(ws_object);
    ws_objects.push_back(ws_object);

  }

  data[JSON_OBJECTS] = ws_objects;

  out_file << data.dump(2);

  astate->astate_evd->new_file_loaded(filename, 0, true);

}

void workspace_t::load_ws_from_json(const std::string filename) {

  app_state_t* astate = app_state_t::get_inst();

  std::fstream ifile(filename);
  json data;

  try {

    repair_connection_info_t rep_info;

    data = json::parse(ifile);

    json_io::load_var(JSON_WS_NAME, m_ws_name, data);
    json_io::load_vec3(JSON_BG_CLR, m_bg_color, data);

    auto data_camera = data.find(JSON_WS_CAMERA);
    //    if (data_camera != data.end()) m_camera->load_from_json(data_camera.value());

    if (data.find(JSON_OBJECTS) != data.end()) {

      json objects = data[JSON_OBJECTS];
      for (auto &object : objects)
        if (object.find(JSON_WS_ITEM_TYPE) != object.end()) {

          std::string obj_type = object[JSON_WS_ITEM_TYPE];
          size_t obj_hash = astate->hash_reg->calc_hash_ub(obj_type);
          std::shared_ptr<ws_item_t> obj =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(obj_hash);

          if (obj) {
            obj->begin_recording(hs_doc_rec_type_e::hs_doc_rec_init);
            obj->load_from_json(object, rep_info);
            obj->end_recording();
            add_item_to_ws(obj);
          }

        } else {

          astate->tlog("WARNING: Cannot find type for object \"{}\" in file \"{}\"!",
                       object[JSON_WS_ITEM_NAME].get<std::string>(), filename);

        }

    }

    //revive connections
    for (auto &rec : rep_info.m_connected_items) {

      auto cur_obj = get_by_name(rec.first);
      if (cur_obj)
        for (auto &rec_values : rec.second) {
          auto con_obj = get_by_name(rec_values);
          if (con_obj)
            cur_obj->m_connected_items.push_back(con_obj);
        }

    }
    //end of revive connections

    //revive ws_item_t class fields
    for (auto &rec : rep_info.m_fields) {
      //astate->tlog("revive class field {}", rec.m_field_name);
      auto target_obj = get_by_name(rec.m_field_name);
      if (target_obj)
        *rec.m_field = target_obj;
    }

    //end of revive ws_item_t class fields

    for (size_t i = 0; i < num_items(); i++) {
      auto ws_item = m_ws_items.get_hs_child_as_array(i);
      if (!ws_item)
        continue;
      ws_item->updated_externally();
    }

    m_fs_path = filename;

  } catch (json::parse_error &e) {
    std::cerr << e.what() << " " << std::endl;
  }

}

void workspace_t::update(float delta_time) {

  app_state_t* astate = app_state_t::get_inst();

  if (m_first_render) {
    if (!m_camera->m_already_loaded)
      set_best_view();
    m_first_render = false;
  }

  m_gizmo->update_gizmo(delta_time);

  //scenic camera rotation
  if (m_scenic_rotation) {
    m_camera->rotate_camera_orbit_roll(m_scenic_rotation_speed[0] * delta_time);
    m_camera->rotate_camera_orbit_pitch(m_scenic_rotation_speed[1] * delta_time);
    m_camera->rotate_camera_orbit_yaw(m_scenic_rotation_speed[2] * delta_time);
    m_camera->update_camera();
    astate->make_viewport_dirty();
  }

  //update cycle
  for (size_t i = 0; i < num_items(); i++) {
    auto ws_item = m_ws_items.get_hs_child_as_array(i);
    if (!ws_item)
      continue;
    ws_item->update(delta_time);
  }

}

void workspace_t::set_edit_type (const ws_edit_e new_edit_type) {
  app_state_t* astate = app_state_t::get_inst();
  p_edit_type = new_edit_type;
  astate->astate_evd->cur_ws_edit_type_changed();
}

ws_edit_e workspace_t::get_edit_type() {
  return p_edit_type;
}

void workspace_t::copy_cam(std::shared_ptr<workspace_t> source) {

  if (!source)
    return;

  if (!source->m_camera)
    return;

  if (!m_camera)
    return;

  m_first_render = false;
  m_camera->update_camera();
  m_camera->copy_from_camera(*source->m_camera);

}

void workspace_t::push_cam_state() {
  if (m_camera)
    m_camera->push_cam_state();
}

void workspace_t::pop_cam_state() {
  if (m_camera)
    m_camera->pop_cam_state();
}

void workspace_t::del_item_by_index(size_t idx) {
  if (idx < num_items()) {
    auto ws_item = m_ws_items.get_hs_child_as_array(idx);
    if (!ws_item)
      return;
    ws_item->hs_delete();
  }
}

void workspace_t::make_overview_dirty() {
  m_need_to_update_overview = true;
}

std::string workspace_t::py_get_repr() {
  return fmt::format("[workspace, name=\"{}\"]", m_ws_name);
}

std::shared_ptr<ws_item_t> workspace_t::py_construct_item(std::string class_name,
                                                          std::string item_name) {

  app_state_t* astate = app_state_t::get_inst();

  if (!p_mgr) {
    astate->tlog("ERROR: workspace_t::py_construct_item -> no ws mgr");
    return nullptr;
  }

  auto type_hash = astate->hash_reg->calc_hash_ub(class_name);

  if (!type_hash) {
    astate->tlog("ERROR: workspace_t::py_construct_item -> invalid hash");
    return nullptr;
  }

  auto new_item = p_mgr->m_bhv_mgr->fbr_ws_item_by_type(type_hash);

  if (!new_item) {
    astate->tlog("ERROR: workspace_t::py_construct_item -> fabric error");
    return nullptr;
  }

  new_item->m_name.set_value(std::move(item_name));
  add_item_to_ws(new_item);

  return new_item;

}

workspace_manager_t::workspace_manager_t (app_state_t *_astate) {
  m_cur_ws_id = 0;
  cached_astate = _astate;
}

std::shared_ptr<workspace_t> workspace_manager_t::get_cur_ws () {

  if (!m_cur_ws_id)
    return nullptr;

  if (*m_cur_ws_id >= m_ws.size())
    return nullptr;

  return m_ws[*m_cur_ws_id];

}

workspace_t *workspace_manager_t::get_cur_rptr() {

  if (!m_cur_ws_id)
    return nullptr;

  if (*m_cur_ws_id >= m_ws.size())
    return nullptr;

  return m_ws[*m_cur_ws_id].get();

}

std::shared_ptr<workspace_t> workspace_manager_t::get_by_name(std::string target_name) {

  auto result = std::find_if(m_ws.begin(), m_ws.end(),
                             [&target_name](std::shared_ptr<workspace_t> src)
  {return src->m_ws_name == target_name;}
                );
  return *result;

}

std::optional<size_t> workspace_manager_t::get_cur_id() {
  if (!m_ws.empty())
    return m_cur_ws_id;
  return std::nullopt;
}

bool workspace_manager_t::set_cur_id(const std::optional<size_t> ws_index) {

  app_state_t* astate = app_state_t::get_inst();

  if (has_wss()) {
    if (ws_index && *ws_index < m_ws.size()) {
      m_cur_ws_id = std::optional<size_t>(ws_index);
      //update_window_title();
      astate->camera = m_ws[*ws_index]->m_camera.get();
      astate->camera->update_camera();
      astate->wlog("========================================================"
                   "\n    Workspace changed: {}\n"
                   "========================================================",
                   m_ws[*ws_index]->m_ws_name);
      astate->astate_evd->cur_ws_changed();
      return true;
    }
  } else {
    m_cur_ws_id = std::nullopt;
    astate->camera = nullptr;
    astate->astate_evd->cur_ws_changed();
    return false;
  }

  return false;

}

std::shared_ptr<workspace_t> workspace_manager_t::get_ws(int id) {
  if (!m_ws.empty() && id >= 0 && id < m_ws.size())
    return m_ws[id];
  else
    return nullptr;
}

void workspace_manager_t::next_ws() {
  size_t target_id = get_cur_id().value_or(0) + 1;
  if (target_id >= m_ws.size())
    target_id = 0;
  set_cur_id(std::optional<size_t>(target_id));
}

void workspace_manager_t::prev_ws() {
  int target_id = get_cur_id().value_or(0) - 1;
  if (target_id < 0)
    target_id = m_ws.size() - 1;
  set_cur_id(std::optional<size_t>(target_id));
}

void workspace_manager_t::cur_ws_next_item() {
  auto [ok, cur_ws] = get_sel_tuple_ws(error_ctx_ignore);
      if (ok)
      cur_ws->next_item();
}

      void workspace_manager_t::cur_ws_prev_item() {
    auto [ok, cur_ws] = get_sel_tuple_ws(error_ctx_ignore);
        if (ok)
        cur_ws->prev_item();
  }

        void workspace_manager_t::init_default () {

      QStringList files = {
        "../data/refs/POSCAR.mp-558947_SiO2",
        "../data/refs/mp-971662_Si.vasp",
        "../deps/qpp/examples/io/ref_data/xyz/nanotube.xyz"
      };

      for (auto &file : files)
        load_from_file_autodeduce(QDir::toNativeSeparators(file).toStdString(),
                                  "",   /* for autodeduce */
                                  true, /* create new ws */
                                  true  /* squash hs */
                                  );

      create_new_ws();

      auto g1 = m_bhv_mgr->fbr_ws_item_by_name("geom_view_t");
      auto g1_gv = g1->cast_as<geom_view_t>();
      g1_gv->m_xgeom_proxy.set_ignore_changes(true);
      g1_gv->ins_atom("Si", vector3<float>(0, -2, 0));
      g1_gv->ins_atom("Si", vector3<float>(0,  2, 0));
      g1_gv->m_xgeom_proxy.set_ignore_changes(false);
      g1_gv->m_xgeom_proxy.init_base_epoch();
      g1->m_name.set_value("g1_src");
      m_ws.back()->add_item_to_ws(g1);

      auto nb1 = m_bhv_mgr->fbr_ws_item_by_name("node_book_t");
      auto nb1_c = nb1->cast_as<node_book_t>();
      nb1_c->m_name.set_value("nodebook1");
      m_ws.back()->add_item_to_ws(nb1);
      m_ws.back()->squash();

    }

    void workspace_manager_t::render_cur_ws () {

      app_state_t* astate = app_state_t::get_inst();

      if (has_wss()) {
        if (m_cur_ws_id && *m_cur_ws_id < m_ws.size()) {
          astate->glapi->glClearColor(m_ws[*m_cur_ws_id]->m_bg_color[0],
              m_ws[*m_cur_ws_id]->m_bg_color[1],
              m_ws[*m_cur_ws_id]->m_bg_color[2],
              1);
          astate->glapi->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          m_ws[*m_cur_ws_id]->render();
          return ;
        }
      }

      astate->glapi->glClearColor(0.4f, 0.4f, 0.4f, 1);
      astate->glapi->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }

    void workspace_manager_t::render_cur_ws_overlay(QPainter &painter) {

      app_state_t *astate = app_state_t::get_inst();

      if (!has_wss())
        return;

      if (m_cur_ws_id && *m_cur_ws_id < m_ws.size()) {
        m_ws[*m_cur_ws_id]->render_overlay(painter);
      }

    }


    void workspace_manager_t::mouse_click () {

      app_state_t* astate = app_state_t::get_inst();

      astate->tlog("Mouse click {} {}", astate->mouse_x, astate->mouse_y);
      astate->tlog("Mouse click in ws {} {}", astate->mouse_x_dc, astate->mouse_y_dc);

      if (has_wss()) {
        get_cur_ws()->mouse_click(astate->mouse_x_dc, astate->mouse_y_dc);
        astate->make_viewport_dirty();
      }

    }

    void workspace_manager_t::mouse_double_click() {

      app_state_t* astate = app_state_t::get_inst();

      if (has_wss()) {
        get_cur_ws()->mouse_double_click(astate->mouse_x_dc, astate->mouse_y_dc);
        astate->make_viewport_dirty();
      }

    }

    void workspace_manager_t::ws_mgr_changed() {
      app_state_t* astate = app_state_t::get_inst();
      astate->astate_evd->wss_changed();
    }

    void workspace_manager_t::add_ws (const std::shared_ptr<workspace_t> &ws_to_add) {
      ws_to_add->set_mgr(this);
      m_ws.push_back(ws_to_add);
      ws_mgr_changed();
    }

    void workspace_manager_t::init_ws_item_bhv_mgr() {

      m_bhv_mgr = std::make_shared<ws_item_behaviour_manager_t>();

      registration_helper_t::reg_ws_item_fbr(m_bhv_mgr.get());
      registration_helper_t::reg_ws_item_io_bhv(m_bhv_mgr.get());
      registration_helper_t::reg_ws_item_obj_insp(m_bhv_mgr.get());
      registration_helper_t::reg_ws_item_tools(m_bhv_mgr.get());
      registration_helper_t::reg_ws_item_ext_edt(m_bhv_mgr.get());
      registration_helper_t::reg_toolbar_elements_bhv(m_bhv_mgr.get());

      sf_node_reg_hlp_t::reg_sf_info(m_bhv_mgr.get());

    }

    void workspace_manager_t::move_ws(size_t from, size_t to) {

      app_state_t* astate = app_state_t::get_inst();

      if (from == to || from >= m_ws.size() || to >= m_ws.size())
        return;

      std::swap(m_ws[from], m_ws[to]);
      set_cur_id(std::optional<size_t>(to));

      astate->astate_evd->wss_changed();

    }


    void workspace_manager_t::load_from_file(const std::string &fname, bool override) {

      app_state_t* astate = app_state_t::get_inst();

      if (!override)
        for (size_t i = 0; i < m_ws.size(); i++)
          if (m_ws[i]->m_fs_path.find(fname) != std::string::npos) {
            set_cur_id(i);
            astate->astate_evd->cur_ws_changed();
            return;
          }

      auto new_ws = std::make_shared<workspace_t>();

      std::string file_name_extr = qpp::extract_base_name(fname);
      new_ws->m_ws_name = file_name_extr;
      new_ws->m_fs_path = fname;
      new_ws->load_ws_from_json(fname);

      add_ws(new_ws);
      set_cur_id(m_ws.size()-1);

      astate->astate_evd->new_file_loaded(fname, 0, true);

    }

    void workspace_manager_t::import_from_file(const std::string &fname,
                                               size_t bhv_id,
                                               bool need_to_create_new_ws,
                                               bool need_to_squash_hs) {

      app_state_t* astate = app_state_t::get_inst();

      if (!need_to_create_new_ws && !has_wss()) return;

      std::shared_ptr<workspace_t> exec_ws{nullptr};

      bool loading_is_succesfull{true};

      if (need_to_create_new_ws) {
        create_new_ws(true);
        exec_ws = m_ws[m_ws.size()-1];
      } else {
        exec_ws = get_cur_ws();
      }

      if (exec_ws) {

        std::shared_ptr<ws_item_t> p_new_itm{nullptr};

        try {

          auto rec_type = need_to_squash_hs ? hs_doc_rec_type_e::hs_doc_rec_init :
                                              hs_doc_rec_type_e::hs_doc_rec_as_new_epoch;

          p_new_itm = m_bhv_mgr->load_ws_itm_from_file(fname, bhv_id, exec_ws.get(), rec_type);

          if (need_to_squash_hs)
            exec_ws.get()->squash();

        } catch (const qpp::parsing_error_t &exc) {

          loading_is_succesfull = false;

          //compose error message
          QString msg_box_title = "Error";
          QString error_message = QObject::tr("An error has occured while parsing the file:\n"
                                              "%1").arg(QString::fromStdString(fname));
          QString error_detail =
              QObject::tr("File name : \n"
                          "\"%1\"\n"
                          "\n"
                          "Line number : \n"
                          "%2\n"
                          "\n"
                          "Line : \"%3\"\n"
                          "\n"
                          "Message : \n"
                          "\"%4\"")
              .arg(QString::fromStdString(fname))
              .arg(exc.m_line_num)
              .arg(QString::fromStdString(exc.m_exception_src))
              .arg(QString::fromStdString(exc.m_exception_msg));

          qrich_error_message_box_t err_msg(msg_box_title, error_message, error_detail);
          err_msg.exec();

        } catch (...) {
          loading_is_succesfull = false;
          QString error_message =
              QObject::tr("An error has occured while parsing the file\n");
          QMessageBox::critical(nullptr, "Error", error_message);
        }

        astate->astate_evd->cur_ws_changed();

        if (loading_is_succesfull) {

          if (need_to_create_new_ws && p_new_itm) {
            exec_ws->m_ws_name = p_new_itm->m_name.get_value();
            if (!exec_ws->m_camera->m_already_loaded)
              exec_ws->set_best_view();
          }

          astate->astate_evd->new_file_loaded(fname,
                                              m_bhv_mgr->m_ws_item_io[bhv_id]->m_accepted_file_format,
                                              false);

        } else {
          if (need_to_create_new_ws && exec_ws)
            exec_ws->m_marked_for_deletion = true;
        }

        astate->astate_evd->wss_changed();

      }

    }

    void workspace_manager_t::save_ws_item_to_file(std::string &file_name,
                                                   std::shared_ptr<ws_item_t> ws_item,
                                                   size_t bhv_id) {

      std::string message;
      bool save_res = m_bhv_mgr->save_ws_itm_to_file(file_name, ws_item, bhv_id, message);

      if (!save_res) {
        QMessageBox::critical(nullptr, "Error", QString::fromStdString(message));
      }

    }

    void workspace_manager_t::load_from_file_autodeduce(const std::string &file_name,
                                                        const std::string &file_format,
                                                        bool new_ws,
                                                        bool squash_hs) {

      app_state_t* astate = app_state_t::get_inst();

      //restore file`s absolute path
      QFileInfo file_info(QString::fromStdString(file_name));
      QString absolute_file_name = file_info.absoluteFilePath();
      auto absolute_file_name_native = absolute_file_name.toStdString();

      astate->tlog("workspace_manager_t::load_from_file_autodeduce, {}",
                   absolute_file_name.toStdString());

      if (!QFileInfo(absolute_file_name).exists()) {
        astate->tlog("Error while opening file \"{}\" - invalid name of the file", file_name);
        return;
      }

      if (file_name.find("json") != std::string::npos
          ||file_format.find("json") != std::string::npos) {
        load_from_file(absolute_file_name_native, false);
      } else {
        //do autodeduce magic
        if (!file_format.empty()) {

          auto ff = m_bhv_mgr->get_ff_by_short_name(file_format);
          if (ff) {
            auto bhv_id = m_bhv_mgr->get_io_bhv_by_file_format(*ff);
            if (bhv_id)
              import_from_file(absolute_file_name_native, *bhv_id, new_ws, squash_hs);
          }

        } else {

          auto ff = m_bhv_mgr->get_ff_by_finger_print(file_name);
          if (ff) {
            auto bhv_id = m_bhv_mgr->get_io_bhv_by_file_format(*ff);
            if (bhv_id)
              import_from_file(absolute_file_name_native, *bhv_id, new_ws, squash_hs);
          }

        }
      }

    }

    void workspace_manager_t::create_new_ws(bool switch_to_new_workspace) {

      auto new_ws = std::make_shared<workspace_t>();
      new_ws->m_ws_name = fmt::format("new_workspace{}", m_ws.size());
      new_ws->set_mgr(this);
      m_ws.push_back(new_ws);

      if (switch_to_new_workspace)
        set_cur_id(m_ws.size()-1);
      ws_mgr_changed();

    }

    std::shared_ptr<ws_item_t> workspace_manager_t::get_sel_itm_sp() {

      auto cur_ws = get_cur_ws();
      if (!cur_ws)
        return nullptr;

      auto cur_it = cur_ws->get_sel_sp();
      if (!cur_it)
        return nullptr;

      return cur_it;

    }

    void workspace_manager_t::utility_event_loop() {

      app_state_t* astate = app_state_t::get_inst();

      bool has_been_deleted{false};

      for (auto it = m_ws.begin(); it != m_ws.end(); ) {

        if ((*it)->m_marked_for_deletion) {

          has_been_deleted = true;
          auto cur_ws_idx = get_cur_id();

          if (cur_ws_idx) {

            //last?
            if (m_ws.size() == 1)
              m_cur_ws_id = std::nullopt;
            else if (int(*cur_ws_idx) - 1 < 0)
              m_cur_ws_id = std::optional<size_t>(0);
            else
              m_cur_ws_id = std::optional<size_t>(std::clamp<size_t>(int(*cur_ws_idx) - 1, 0, 100));

          }

          it = m_ws.erase(it);
          set_cur_id(m_cur_ws_id);
          astate->astate_evd->cur_ws_changed();

        } else {
          ++it;
        }

      }

      if (has_been_deleted)
        astate->astate_evd->wss_changed();

    }


