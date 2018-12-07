#include <qppcad/python_simple_query.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_atoms_list.hpp>

using namespace qpp;
using namespace qpp::cad;

void simple_query::select_ws(int ws_idx) {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->ws_manager->has_wss()) {
      astate->ws_manager->set_cur_id(ws_idx);
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
//              for (auto itm : sel_list)
//                if (py::isinstance<py::int_>(itm)) cur_it_al->select_atom(py::cast<int>(itm));
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
//              for (auto itm : sel_list)
//                if (py::isinstance<py::int_>(itm)) cur_it_al->select_atom(py::cast<int>(itm));
            }
        }

      astate->make_viewport_dirty();

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
