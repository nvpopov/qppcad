#include <qppcad/python_simple_query.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <data/ptable.hpp>

using namespace qpp;
using namespace qpp::cad;

void simple_query::open_file(std::string file_name) {

  app_state_t *astate = app_state_t::get_inst();

  astate->ws_manager->import_file_autodeduce(file_name);
  astate->make_viewport_dirty();
  astate->astate_evd->python_console_focus_requested();

}

void simple_query::open_file_query(std::string file_name, qc_file_fmt file_format) {

  app_state_t *astate = app_state_t::get_inst();

  astate->ws_manager->import_file_generic(file_name, file_format);
  astate->make_viewport_dirty();
  astate->astate_evd->python_console_focus_requested();

}

void simple_query::make_super_cell(const int sc_a, const int sc_b, const int sc_c) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              cur_it_al->make_super_cell(sc_a+1, sc_b+1, sc_c+1);
            }
          cur_ws->set_selected_item(cur_ws->m_ws_items.size()-1);
          astate->astate_evd->python_console_focus_requested();
        }
      astate->make_viewport_dirty();

    }
}

void simple_query::select_ws(int ws_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      astate->ws_manager->set_cur_id(ws_idx);
      astate->astate_evd->cur_ws_changed();
      astate->make_viewport_dirty();
    }

}

void simple_query::select_itm(int itm_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          cur_ws->set_selected_item(itm_idx);
        }
      astate->make_viewport_dirty();
    }

  astate->astate_evd->python_console_focus_requested();

}

void simple_query::sel_cnt(int cnt_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              cur_it_al->select_atom(cnt_idx);
            }
        }
      astate->make_viewport_dirty();

    }
}

void simple_query::sel_cnt_parity() {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              index zero = index::D(cur_it_al->m_geom->DIM).all(0);
              for (auto &elem : cur_it_al->m_atom_idx_sel)
                if (elem.m_idx == zero) {
                    std::array<int, 2> parity_d{-1,1};
                    for (auto &p_x : parity_d)
                      for (auto &p_y : parity_d)
                        for (auto &p_z : parity_d) {
                            vector3<float> new_pos{0};
                            new_pos[0] = cur_it_al->m_geom->coord(elem.m_atm)[0] * p_x;
                            new_pos[1] = cur_it_al->m_geom->coord(elem.m_atm)[1] * p_y;
                            new_pos[2] = cur_it_al->m_geom->coord(elem.m_atm)[2] * p_z;
                            std::vector<tws_node_content_t<float> > res;
                            const float eps_dist = 0.01;
                            cur_it_al->m_tws_tr->query_sphere(eps_dist, new_pos, res);
                            for (auto &res_elem : res)
                              if (res_elem.m_idx == zero) cur_it_al->select_atom(res_elem.m_atm);
                          }
                  }
            }
        }
      astate->make_viewport_dirty();

    }
}

void simple_query::sel_invert() {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              cur_it_al->invert_selected_atoms();
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::sel_cnt_all() {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              cur_it_al->select_atoms(true);
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::sel_cnt_list(pybind11::list sel_list) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              for (auto itm : sel_list)
                if (py::isinstance<py::int_>(itm)) cur_it_al->select_atom(py::cast<int>(itm));
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::sel_cnt_type(pybind11::str sel_type) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              std::string type_name = py::cast<std::string>(sel_type);
              int type_id = cur_it_al->m_geom->type_of_atom(type_name);
              if (type_id != -1) cur_it_al->select_by_type(type_id);

            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::sel_cnt_sphere(vector3<float> sph_center, float sph_rad) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              std::vector<tws_node_content_t<float> > cnt;
              cur_it_al->m_tws_tr->query_sphere(sph_rad, sph_center, cnt);

              for (auto &item : cnt)
                if (item.m_idx == index::D(cur_it_al->m_geom->DIM).all(0))
                  cur_it_al->select_atom(item.m_atm);
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::sel_hemisphere(int coord_idx, bool positive) {

  app_state_t *astate = app_state_t::get_inst();

  if (coord_idx < 0 || coord_idx > 2) return;

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              for (int i = 0; i < cur_it_al->m_geom->nat(); i++)
                if (positive == cur_it_al->m_geom->pos(i)[coord_idx] > -0.01)
                  cur_it_al->select_atom(i);
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::edit_mode(int mode) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          if (mode == 0) cur_ws->set_edit_type(ws_edit_t::edit_item);
          else cur_ws->set_edit_type(ws_edit_t::edit_content);
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::unsel_cnt_all() {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              cur_it_al->select_atoms(true);
              cur_it_al->invert_selected_atoms();
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::unsel_cnt(int cnt_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              cur_it_al->unselect_atom(cnt_idx);
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::unsel_cnt_list(pybind11::list sel_list) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              for (auto itm : sel_list)
                if (py::isinstance<py::int_>(itm)) cur_it_al->unselect_atom(py::cast<int>(itm));
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::unsel_cnt_type(pybind11::str sel_type) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              std::string type_name = py::cast<std::string>(sel_type);
              int type_id = cur_it_al->m_geom->type_of_atom(type_name);
              if (type_id != -1) cur_it_al->unselect_by_type(type_id);
            }
        }

      astate->make_viewport_dirty();

    }
}

pybind11::list simple_query::get_sel() {

  app_state_t *astate = app_state_t::get_inst();
  py::list res;

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              for (auto &elem : cur_it_al->m_atom_idx_sel)
                if (elem.m_idx == index::D(cur_it_al->m_geom->DIM).all(0)) res.append(elem.m_atm);
            }
        }
    }

  return res;
}

vector3<float> simple_query::gizmo_pos() {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              return cur_it_al->get_gizmo_content_barycenter();
            }
        }
    }

  return vector3<float>::Zero();
}

void simple_query::rebond() {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              cur_it_al->m_tws_tr->m_bonding_table.init_default(cur_it_al->m_geom.get());
              cur_it_al->m_tws_tr->do_action(act_rebuild_ntable);
              astate->astate_evd->cur_ws_selected_item_changed();
            }
        }
    }
}

void simple_query::translate_selected(float tx, float ty, float tz) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {

          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

          if (cur_ws->m_edit_type == ws_edit_t::edit_item) {
              cur_it_al->translate(vector3<float>(tx, ty, tz));
            } else {
              if (cur_it_al) {
                  cur_it_al->translate_selected(vector3<float>(tx, ty, tz));
                }
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::set_charge(float charge) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {

          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

          if (cur_it_al && cur_ws->m_edit_type == ws_edit_t::edit_content) {
              index zero = index::D(cur_it_al->m_geom->DIM).all(0);
              for (auto &elem : cur_it_al->m_atom_idx_sel)
                if (elem.m_idx == zero)
                  cur_it_al->m_geom->xfield<float>(xgeom_charge, elem.m_atm) = charge;
            }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::set_ws_bg(vector3<float> bg) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          cur_ws->m_background_color = bg;
        }

    }
}

void simple_query::add_atoms_list_0d(std::string name) {
  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto new_al = std::make_shared<ws_atoms_list_t>();
          new_al->m_name = name;
          cur_ws->add_item_to_ws(new_al);
        }

    }
}

void simple_query::add_atoms_list_3d(std::string name, vector3<float> a,
                                     vector3<float> b,
                                     vector3<float> c) {
  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto new_al = std::make_shared<ws_atoms_list_t>();
          new_al->m_name = name;
          new_al->m_geom->DIM = 3;
          new_al->m_geom->cell.DIM = 3;
          new_al->m_geom->cell.v[0] = a;
          new_al->m_geom->cell.v[1] = b;
          new_al->m_geom->cell.v[2] = c;
          cur_ws->add_item_to_ws(new_al);
        }

    }
}

void simple_query::set_sel_color_vec(vector3<float> color) {
  set_sel_color(color[0], color[1], color[2]);
}

void simple_query::set_sel_color(float r, float g, float b) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {

          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

          if (cur_it_al)
            for (auto &elem : cur_it_al->m_atom_idx_sel) {
                cur_it_al->m_geom->xfield<float>(xgeom_ccr, elem.m_atm) = r;
                cur_it_al->m_geom->xfield<float>(xgeom_ccg, elem.m_atm) = g;
                cur_it_al->m_geom->xfield<float>(xgeom_ccb, elem.m_atm) = b;

                if (cur_it_al->m_anim->get_total_anims() > 0 &&
                    !cur_it_al->m_anim->m_anim_data[0].frames.empty() &&
                    cur_it_al->m_anim->m_anim_data[0].frames[0].atom_color.size() ==
                    cur_it_al->m_geom->nat()) {
                    cur_it_al->m_anim->m_anim_data[0].frames[0].atom_color[elem.m_atm][0] = r;
                    cur_it_al->m_anim->m_anim_data[0].frames[0].atom_color[elem.m_atm][1] = g;
                    cur_it_al->m_anim->m_anim_data[0].frames[0].atom_color[elem.m_atm][2] = b;
                  }

              }
        }

      astate->make_viewport_dirty();

    }
}

void simple_query::ptable_set_color_by_number(int num, float r, float g, float b) {

  app_state_t *astate = app_state_t::get_inst();

  if (num > 0 && num <100) {
      ptable_atom_record &rec = ptable::get_inst()->arecs[num-1];
      rec.m_color_jmol = vector3<float>(r, g, b);
      rec.m_redefined = true;
      astate->astate_evd->cur_ws_selected_item_changed();
      astate->astate_evd->python_console_focus_requested();
    }
}

void simple_query::ptable_set_color_by_name(std::string name, float r, float g, float b) {
  auto ap_idx = ptable::number_by_symbol(name);
  if (ap_idx) ptable_set_color_by_number(*ap_idx, r, g, b);
}

void simple_query::ptable_set_radius_by_number(int num, float r) {
  if (num > 0 && num <100) {
      ptable_atom_record &rec = ptable::get_inst()->arecs[num-1];
      rec.m_radius = r;
      rec.m_redefined = true;
    }
}

void simple_query::ptable_set_radius_by_name(std::string name, float r) {
  auto ap_idx = ptable::number_by_symbol(name);
  if (ap_idx) ptable_set_radius_by_number(*ap_idx, r);
}

vector3<float> simple_query::ptable_get_color_by_number(int num) {
  if (num > 0 && num <100) return ptable::get_inst()->arecs[num-1].m_color_jmol;
  else return vector3<float>::Zero();
}

vector3<float> simple_query::ptable_get_color_by_name(std::string name) {
  auto ap_idx = ptable::number_by_symbol(name);
  if (ap_idx) return ptable_get_color_by_number(*ap_idx);
  else return vector3<float>::Zero();
}

float simple_query::ptable_get_radius_by_number(int num) {
  if (num > 0 && num <100) return ptable::get_inst()->arecs[num-1].m_radius;
  else return 0.0f;
}

float simple_query::ptable_get_radius_by_name(std::string name) {
  auto ap_idx = ptable::number_by_symbol(name);
  if (ap_idx) return ptable_get_radius_by_number(*ap_idx);
  else return 0.0f;
}

void simple_query::camera_move(vector3<float> axis, float magnitude) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      astate->camera->translate_camera(axis * magnitude);
      astate->camera->update_camera();
    }
}

void simple_query::camera_rotate_yaw(float magnitude) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      astate->camera->rotate_camera_orbit_yaw(magnitude);
      astate->camera->update_camera();
    }

}

void simple_query::camera_rotate_pitch(float magnitude) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      astate->camera->rotate_camera_orbit_pitch(magnitude);
      astate->camera->update_camera();
    }

}

void simple_query::camera_zoom(float magnitude) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      astate->camera->update_camera_zoom(magnitude);
      astate->camera->update_camera();
    }

}

void simple_query::camera_mode(int mode) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->camera) {
      if (mode == 0) astate->camera->set_projection(cam_proj_t::proj_persp);
      else astate->camera->set_projection(cam_proj_t::proj_ortho);
      astate->camera->update_camera();
    }
}

void simple_query::copy_camera_from_ws(int ws_id) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->camera) {
      auto ws_ref = astate->ws_manager->get_ws(ws_id);
      if (ws_ref) astate->camera->copy_from_camera(*(ws_ref->m_camera));
    }
}

pybind11::list simple_query::sv_get() {

  py::list ret;

  app_state_t *astate = app_state_t::get_inst();
  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (_al) {
              for (int i = 0; i < _al->m_geom->nat(); i++)
                ret.append(_al->m_geom->xfield<bool>(xgeom_sel_vis, i));
            }
        }
      astate->make_viewport_dirty();
    }
  return ret;
}

void simple_query::sv_edit(int at, bool status) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (_al) {
              _al->m_geom->xfield<bool>(xgeom_sel_vis, at) = status;
            }
        }
      astate->make_viewport_dirty();
    }
}

void simple_query::sv_edit_list(pybind11::list at_list, bool status) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (_al) {
              for (auto elem : at_list)
                if (py::isinstance<py::int_>(elem))
                  _al->m_geom->xfield<bool>(xgeom_sel_vis, elem.cast<py::int_>()) =
                      status;
            }
        }
      astate->make_viewport_dirty();
    }
}

void simple_query::sv_edit_all(bool status) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (_al)
            for (int i = 0; i < _al->m_geom->nat(); i++)
              _al->m_geom->xfield<bool>(xgeom_sel_vis,i) = status;
        }
      astate->make_viewport_dirty();
    }
}

pybind11::list simple_query::cl_get() {

  py::list ret;

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (_al) {
              for (int i = 0; i < _al->m_geom->nat(); i++)
                ret.append(_al->m_geom->xfield<bool>(xgeom_label_state, i));
            }
        }
      astate->make_viewport_dirty();
    }
  return ret;

}

void simple_query::set_cl_state(int at, bool status) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (_al) {
              _al->m_geom->xfield<bool>(xgeom_label_state, at) = status;
            }
        }
      astate->make_viewport_dirty();
    }
}

void simple_query::set_cl_text(int at, std::string text) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto _al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (_al) {
              _al->m_geom->xfield<std::string>(xgeom_label_text, at) = text;
            }
        }
      astate->make_viewport_dirty();
    }
}
