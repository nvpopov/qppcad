#include <qppcad/workspace.hpp>
#include <qppcad/app_state.hpp>
#include <data/ptable.hpp>
#include <mathf/math.hpp>
#include <qppcad/register_all_things.hpp>
#include <io/parsing_exceptions.hpp>
#include <qppcad/ui/qrich_error_message_box.hpp>
#include <qppcad/json_helpers.hpp>

#include <QDir>

using namespace qpp;
using namespace qpp::cad;

opt<size_t> workspace_t::get_selected_idx () {
  for (size_t i = 0; i < m_ws_items.size(); i++)
    if (m_ws_items[i]->m_selected) return opt<size_t>(i);
  return std::nullopt;
}

ws_item_t *workspace_t::get_selected () {
  std::optional<size_t> sel_idx = get_selected_idx();
  if (sel_idx) return m_ws_items[*sel_idx].get();
  else return nullptr;
}

std::shared_ptr<ws_item_t> workspace_t::get_selected_sp(){
  std::optional<size_t> sel_idx = get_selected_idx();
  if (sel_idx) return m_ws_items[*sel_idx];
  else return nullptr;
}

std::shared_ptr<ws_item_t> workspace_t::get_by_name(std::string _name) {
  for (auto item : m_ws_items)
    if (item->m_name == _name) return item;
  return nullptr;
}

bool workspace_t::set_selected_item (const size_t sel_idx, bool emit_signal) {

  app_state_t* astate = app_state_t::get_inst();

  unselect_all();

  astate->log(fmt::format("workspace_t::set_selected_item ({} {})", sel_idx, emit_signal));

  if (sel_idx < m_ws_items.size() && !m_ws_items.empty()) {
      m_ws_items[sel_idx]->m_selected = true;
      if (m_ws_items[sel_idx]->get_flags() & ws_item_flags_support_tr) {
          m_gizmo->attached_item = m_ws_items[sel_idx].get();
          m_gizmo->update_gizmo(0.1f, true);
          astate->make_viewport_dirty();
        }
      else {
          m_gizmo->attached_item = nullptr;
          m_gizmo->update_gizmo(0.1f, true);
        }
      //astate->make_viewport_dirty();
      if (emit_signal) astate->astate_evd->cur_ws_selected_item_changed();
      return true;
    }

  //astate->make_viewport_dirty();
  if (emit_signal) astate->astate_evd->cur_ws_selected_item_changed();
  return false;

}

void workspace_t::unselect_all (bool emit_signal) {

  for (auto &ws_item : m_ws_items) ws_item->m_selected = false;
  app_state_t* astate = app_state_t::get_inst();
  if (emit_signal) astate->astate_evd->cur_ws_selected_item_changed();

}

void workspace_t::toggle_edit_mode () {

  app_state_t* astate = app_state_t::get_inst();

  if (m_edit_type == ws_edit_e::edit_item) m_edit_type = ws_edit_e::edit_content;
  else m_edit_type = ws_edit_e::edit_item;

  astate->astate_evd->cur_ws_edit_type_changed();

}

void workspace_t::ws_changed () {

}

void workspace_t::reset_camera () {

  m_camera->reset_camera();
  set_best_view();

}

void workspace_t::set_best_view () {

  if (m_ws_items.size() == 0) {
      m_camera->reset_camera();
      m_camera->update_camera();
      return;
    }

  vector3<float> vec_look_at  = vector3<float>(0.0, 0.0, 0.0);
  vector3<float> vec_look_pos = vector3<float>(0.0, 0.0, 0.0);

  size_t total_voters = 0;
  for (auto &ws_item : m_ws_items)
    if (ws_item->get_flags() & ws_item_flags_support_view_voting) {
        total_voters+=1;
        ws_item->vote_for_view_vectors(vec_look_pos, vec_look_at);
      }

  total_voters = std::clamp<size_t>(total_voters, 1, 20);

  vec_look_at  /= total_voters;
  vec_look_pos /= total_voters;

  m_camera->m_look_at = vec_look_at;
  m_camera->m_view_point = vec_look_pos;

  m_camera->orthogonalize_gs();
  m_camera->update_camera();

  if ((m_camera->m_look_at-m_camera->m_view_point).norm() < 0.4f || vec_look_at == vec_look_pos)
    m_camera->reset_camera();

}

void workspace_t::render() {

  app_state_t* astate = app_state_t::get_inst();

  if (m_gizmo->m_is_active && m_gizmo->attached_item) m_gizmo->render();

  if (astate->m_debug_show_selection_ray){
      astate->dp->begin_render_line();
      astate->dp->render_line(vector3<float>(1.0, 1.0, 0.0), m_ray.start,
                              m_ray.start + m_ray.dir * 155.0);
      astate->dp->end_render_line();
    }

  if (astate->dp) {

      ///// Draw axis /////
      if (astate->m_show_axis) {
          vector3<float> vScrTW = astate->camera->unproject(-0.95f, -0.90f);
          float axis_magn = 0.07f *astate->camera->m_stored_dist;
          if (astate->camera->m_cur_proj == cam_proj_t::proj_persp) axis_magn = 0.015f;
          else axis_magn = m_camera->m_ortho_scale * 0.1f;

          astate->dp->begin_render_line();
          astate->dp->
              render_line(vector3<float>(1.0, 0.0, 0.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(axis_magn, 0.0, 0.0) + vScrTW);

          astate->dp->
              render_line(vector3<float>(0.0, 1.0, 0.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(0.0, axis_magn, 0.0) + vScrTW);

          astate->dp->
              render_line(vector3<float>(0.0, 0.0, 1.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(0.0, 0.0, axis_magn) + vScrTW);
          astate->dp->end_render_line();

        }
      ///// Draw axis end /////
    }

  for (auto &ws_item : m_ws_items) ws_item->render();

}

void workspace_t::render_overlay(QPainter &painter) {
  for (auto &ws_item : m_ws_items)
    if (ws_item->m_is_visible) ws_item->render_overlay(painter);
}

void workspace_t::mouse_click (const float mouse_x, const float mouse_y) {

  app_state_t* astate = app_state_t::get_inst();

  if (m_camera->m_cur_proj == cam_proj_t::proj_persp) {
      m_ray.start = m_camera->m_view_point;
      m_ray.dir = (m_camera->unproject(mouse_x, mouse_y) - m_camera->m_view_point).normalized();
    } else {
      float z_p =  (m_camera->m_znear_ortho + m_camera->m_zfar_ortho) /
                   (m_camera->m_znear_ortho - m_camera->m_zfar_ortho);

      m_ray.start = m_camera->unproject(mouse_x, mouse_y, z_p);
      m_ray.dir = (m_camera->m_look_at - m_camera->m_view_point).normalized();

    }

  if (m_gizmo->m_is_visible && m_gizmo->attached_item)
    if (m_gizmo->process_ray(&m_ray)) {
        astate->log("gizmo clicked");
        return;
      }

  bool hit_any = false;

  if (m_edit_type != ws_edit_e::edit_content) {
      for (auto &ws_item : m_ws_items) ws_item->m_selected = false;
      m_gizmo->attached_item = nullptr;
    }

  for (auto &ws_item : m_ws_items) {
      bool is_hit = ws_item->mouse_click(&m_ray);
      hit_any = hit_any || is_hit;
      if (is_hit && m_edit_type == ws_edit_e::edit_item && ws_item->m_is_visible &&
          (ws_item->get_flags() & ws_item_flags_support_sel)) {
          m_gizmo->attached_item = ws_item.get();
          auto it = std::find(m_ws_items.begin(), m_ws_items.end(), ws_item);
          if (it != m_ws_items.end()) {
              auto index = std::distance(m_ws_items.begin(), it);
              set_selected_item(index);
              break;
            }
        }
    }

  if (m_edit_type != ws_edit_e::edit_content && !hit_any) {
      m_gizmo->attached_item = nullptr;
      unselect_all();
    }

}

void workspace_t::add_item_to_ws (const std::shared_ptr<ws_item_t> item_to_add) {

  item_to_add->set_parent_workspace(this);
  m_ws_items.push_back(item_to_add);
  app_state_t::get_inst()->astate_evd->cur_ws_changed();

}

void workspace_t::clear_connected_items(std::shared_ptr<ws_item_t> item_to_delete) {

  for (auto elem : m_ws_items) {
      auto it = std::find(elem->m_connected_items.begin(),
                          elem->m_connected_items.end(),
                          item_to_delete);
      if (it != elem->m_connected_items.end())
        elem->m_connected_items.erase(it);
    }

}

void workspace_t::save_ws_to_json (const std::string filename) {

  app_state_t* astate = app_state_t::get_inst();

  std::ofstream out_file(filename);
  json data;

  data[JSON_QPPCAD_VERSION] = "1.0-aa";

  json_helper::save_var(JSON_WS_NAME, m_ws_name, data);
  json_helper::save_vec3(JSON_BG_CLR, m_background_color, data);

  json camera_data;
  m_camera->save_to_json(camera_data);
  data[JSON_WS_CAMERA] = camera_data;

  json ws_objects = json::array({});
  for (const auto &ws_item : m_ws_items)
    if (ws_item->can_be_written_to_json()) {
        json ws_object;
        ws_item->save_to_json(ws_object);
        ws_objects.push_back(ws_object);
      }

  data[JSON_OBJECTS] = ws_objects;

  out_file << data.dump(2);

  astate->astate_evd->new_file_loaded(filename, 0, true);

}

void workspace_t::load_ws_from_json (const std::string filename) {

  app_state_t* astate = app_state_t::get_inst();

  std::fstream ifile(filename);
  json data;

  try {

    data = json::parse(ifile);

    json_helper::load_var(JSON_WS_NAME, m_ws_name, data);
    json_helper::load_vec3(JSON_BG_CLR, m_background_color, data);

    auto data_camera = data.find(JSON_WS_CAMERA);
    if (data_camera != data.end()) m_camera->load_from_json(data_camera.value());

    if (data.find(JSON_OBJECTS) != data.end()){
        json objects = data[JSON_OBJECTS];
        for (auto &object : objects)
          if (object.find(JSON_WS_ITEM_TYPE) != object.end()) {
              std::string obj_type = object[JSON_WS_ITEM_TYPE];
              size_t obj_hash = astate->hash_reg->calc_hash(obj_type);
              std::shared_ptr<ws_item_t> obj =
                  astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(obj_hash);
              if (obj) {
                  obj->load_from_json(object);
                  add_item_to_ws(obj);
                }
            } else {
              astate->log(
                    fmt::format("WARNING: Cannot find type for object \"{}\" in file \"{}\"!",
                                object[JSON_WS_ITEM_NAME].get<std::string>(), filename)
                    );
            }
      }

    m_fs_path = filename;

  } catch (json::parse_error &e) {
    std::cerr << e.what() << " " << std::endl;
  }

}

void workspace_t::update (float delta_time) {

  if (m_first_render) {
      if (!m_camera->m_already_loaded) set_best_view();
      m_first_render = false;
    }

  m_gizmo->update_gizmo(delta_time);

  //handle deletion

  bool need_to_emit_ws_changed{false};

  for (auto it = m_ws_items.begin(); it != m_ws_items.end(); ) {
      if ((*it)->m_marked_for_deletion) {

          if (it->get() == m_gizmo->attached_item)
            m_gizmo->attached_item = nullptr;

          if (it->get()->m_selected) unselect_all(true);

          clear_connected_items(*it);
          it->get()->m_parent_ws = nullptr;
          it->get()->m_connected_items.clear();
          it = m_ws_items.erase(it);
          //it->reset();
          need_to_emit_ws_changed = true;
          app_state_t* astate = app_state_t::get_inst();
          astate->astate_evd->cur_ws_changed();
        }
      else {
          ++it;
        }
    }

  ws_changed();

  //update cycle
  for (auto &ws_item : m_ws_items) ws_item->update(delta_time);

}

void workspace_t::set_edit_type (const ws_edit_e new_edit_type) {

  app_state_t* astate = app_state_t::get_inst();
  m_edit_type = new_edit_type;
  astate->astate_evd->cur_ws_edit_type_changed();

}

std::string workspace_t::py_get_repr() {
  return fmt::format("[workspace, name=\"{}\"]", m_ws_name);
}

std::shared_ptr<ws_item_t> workspace_t::py_construct_item(std::string class_name,
                                                          std::string item_name) {
  app_state_t* astate = app_state_t::get_inst();
  if (!m_owner) {
      astate->log("ERROR: workspace_t::py_construct_item -> no ws mgr");
      return nullptr;
    }

  auto type_hash = astate->hash_reg->calc_hash(class_name);

  if (!type_hash) {
      astate->log("ERROR: workspace_t::py_construct_item -> invalid hash");
      return nullptr;
    }

  auto new_item = m_owner->m_bhv_mgr->fbr_ws_item_by_type(type_hash);

  if (!new_item) {
      astate->log("ERROR: workspace_t::py_construct_item -> fabric error");
      return nullptr;
    }

  new_item->m_name = item_name;
  add_item_to_ws(new_item);

  return new_item;

}

workspace_manager_t::workspace_manager_t (app_state_t *_astate) {

  m_cur_ws_id = 0;
  cached_astate = _astate;

}

std::shared_ptr<workspace_t> workspace_manager_t::get_cur_ws () {

  if (!m_cur_ws_id) return nullptr;
  if (*m_cur_ws_id >= m_ws.size()) return nullptr;
  return m_ws[*m_cur_ws_id];

}

std::shared_ptr<workspace_t> workspace_manager_t::get_by_name(std::string target_name) {

  for (auto item : m_ws)
    if (item->m_ws_name == target_name) return item;

  return nullptr;

}

opt<size_t> workspace_manager_t::get_cur_id () {

  if (!m_ws.empty()) return m_cur_ws_id;
  return std::nullopt;

}

bool workspace_manager_t::set_cur_id (const opt<size_t> ws_index) {

  //c_app::log("set current called");
  app_state_t* astate = app_state_t::get_inst();

  if (ws_index && *ws_index < m_ws.size() && has_wss()) {
      m_cur_ws_id = opt<size_t>(ws_index);
      //update_window_title();
      astate->camera = m_ws[*ws_index]->m_camera.get();
      astate->camera->update_camera();
      astate->astate_evd->cur_ws_changed();
      return true;
    }

  astate->camera = nullptr;
  astate->astate_evd->cur_ws_changed();
  return false;

}

std::shared_ptr<workspace_t> workspace_manager_t::get_ws(int id) {

  if (!m_ws.empty() && id >= 0 && id < m_ws.size()) return m_ws[id];
  else return nullptr;

}

void workspace_manager_t::init_default () {

  app_state_t* astate = app_state_t::get_inst();

  std::ifstream test_in_dev_env("../data/refs/laf3_p3.vasp");
  if (!test_in_dev_env.good()) return;

  QStringList files = {
    "../data/refs/POSCAR.mp-558947_SiO2",
    "../deps/qpp/examples/io/ref_data/xyz/nanotube.xyz",
    "../data/refs/mp-971662_Si.vasp"
  };

  for (auto &file : files)
    load_from_file_autodeduce(QDir::toNativeSeparators(file).toStdString());

  auto new_node_book = m_bhv_mgr->fbr_ws_item_by_name("node_book_t");
  new_node_book->m_name = "test_nb1";
  m_ws.back()->add_item_to_ws(new_node_book);

}

void workspace_manager_t::render_cur_ws () {

  app_state_t* astate = app_state_t::get_inst();

  if (has_wss()) {

      if (m_cur_ws_id && *m_cur_ws_id < m_ws.size()) {

          astate->glapi->glClearColor(m_ws[*m_cur_ws_id]->m_background_color[0],
              m_ws[*m_cur_ws_id]->m_background_color[1],
              m_ws[*m_cur_ws_id]->m_background_color[2], 1);
          astate->glapi->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          m_ws[*m_cur_ws_id]->render();
          return ;

        }

    }

  astate->glapi->glClearColor(0.4f, 0.4f, 0.4f, 1);
  astate->glapi->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void workspace_manager_t::render_cur_ws_overlay(QPainter &painter) {

  if (!has_wss()) return;

  if (m_cur_ws_id && *m_cur_ws_id < m_ws.size()) {
      m_ws[*m_cur_ws_id]->render_overlay(painter);
    }

}


void workspace_manager_t::mouse_click () {

  app_state_t* astate = app_state_t::get_inst();

  astate->log(fmt::format("Mouse click {} {}", astate->mouse_x, astate->mouse_y));
  astate->log(fmt::format("Mouse click in ws {} {}", astate->mouse_x_dc, astate->mouse_y_dc));

  if (has_wss()) {
      get_cur_ws()->mouse_click(astate->mouse_x_dc, astate->mouse_y_dc);
      astate->make_viewport_dirty();
    }

}

void workspace_manager_t::ws_mgr_changed() {

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->wss_changed();

}

void workspace_manager_t::add_ws (const std::shared_ptr<workspace_t> &ws_to_add) {

  ws_to_add->m_owner = this;
  m_ws.push_back(ws_to_add);
  ws_to_add->ws_changed();
  ws_mgr_changed();

}

void workspace_manager_t::init_ws_item_bhv_mgr() {

  m_bhv_mgr = std::make_unique<ws_item_behaviour_manager_t>();
  registration_helper_t::reg_ws_item_fbr(m_bhv_mgr.get());
  registration_helper_t::reg_ws_item_io_bhv(m_bhv_mgr.get());
  registration_helper_t::reg_ws_item_obj_insp(m_bhv_mgr.get());
  registration_helper_t::reg_ws_item_tools(m_bhv_mgr.get());
  registration_helper_t::reg_ws_item_ext_edt(m_bhv_mgr.get());

}


void workspace_manager_t::load_from_file(const std::string &fname,
                                         bool override) {

  app_state_t* astate = app_state_t::get_inst();

  if (!override)
    for (int i = 0; i < m_ws.size(); i++)
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
                                           bool need_to_create_new_ws) {

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

      std::shared_ptr<ws_item_t> p_new_item{nullptr};

      try {
        p_new_item = m_bhv_mgr->load_ws_itm_from_file(fname, bhv_id, exec_ws.get());
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

        qrich_error_message_box_t err_msg(msg_box_title,
                                          error_message,
                                          error_detail);
        err_msg.exec();

      }
      catch (...) {
        loading_is_succesfull = false;
        QString error_message =
            QObject::tr("An error has occured while parsing the file\n");
        QMessageBox::critical(nullptr, "Error", error_message);
      }

      astate->astate_evd->cur_ws_changed();

      if (loading_is_succesfull) {

          if (need_to_create_new_ws && p_new_item) {
              exec_ws->m_ws_name = p_new_item->m_name;
              if (!exec_ws->m_camera->m_already_loaded) exec_ws->set_best_view();
            }

          astate->astate_evd->new_file_loaded(
                fname,
                m_bhv_mgr->m_ws_item_io[bhv_id]->m_accepted_file_format,
                false);

        } else {
          if (need_to_create_new_ws && exec_ws) exec_ws->m_marked_for_deletion = true;
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

void workspace_manager_t::load_from_file_autodeduce(const std::string file_name,
                                                    const std::string file_format,
                                                    bool create_new_ws) {

  if (file_name.find("json") != std::string::npos ||
      file_format.find("json") != std::string::npos) {
      load_from_file(file_name, false);
    } else {
      //do autodeduce magic
      if (!file_format.empty()) {
          auto ff = m_bhv_mgr->get_ff_by_short_name(file_format);
          if (ff) {
              auto bhv_id = m_bhv_mgr->get_io_bhv_by_file_format(*ff);
              if (bhv_id) import_from_file(file_name, *bhv_id, create_new_ws);
            }
        } else {
          auto ff = m_bhv_mgr->get_ff_by_finger_print(file_name);
          if (ff) {
              auto bhv_id = m_bhv_mgr->get_io_bhv_by_file_format(*ff);
              if (bhv_id) import_from_file(file_name, *bhv_id, create_new_ws);
            }
        }
    }

}

void workspace_manager_t::create_new_ws(bool switch_to_new_workspace) {

  auto new_ws = std::make_shared<workspace_t>();
  new_ws->m_ws_name = fmt::format("new_workspace{}", m_ws.size());
  new_ws->m_owner = this;
  m_ws.push_back(new_ws);

  if (switch_to_new_workspace) set_cur_id(m_ws.size()-1);
  ws_mgr_changed();

}

std::shared_ptr<ws_item_t> workspace_manager_t::get_sel_itm_sp() {

  auto cur_ws = get_cur_ws();
  if (!cur_ws) return nullptr;

  auto cur_it = cur_ws->get_selected_sp();
  if (!cur_it) return nullptr;

  return cur_it;
}

void workspace_manager_t::utility_event_loop() {

  app_state_t* astate = app_state_t::get_inst();

  for (auto it = m_ws.begin(); it != m_ws.end(); ) {
      if ((*it)->m_marked_for_deletion) {
          auto cur_ws_idx = get_cur_id();
          if (cur_ws_idx) {
              //last?
              if (m_ws.size() == 1) m_cur_ws_id = std::nullopt;
              else if (int(*cur_ws_idx) - 1 < 0) m_cur_ws_id = opt<size_t>(0);
              else m_cur_ws_id = opt<size_t>(std::clamp<size_t>(int(*cur_ws_idx) - 1, 0, 100));
            }
          it = m_ws.erase(it);
          set_cur_id(m_cur_ws_id);
          astate->astate_evd->cur_ws_changed();
        }
      else {
          ++it;
        }
    }

}


