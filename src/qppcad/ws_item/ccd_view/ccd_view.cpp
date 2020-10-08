#include <qppcad/ws_item/ccd_view/ccd_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ccd_view_t::ccd_view_t() : ws_item_t() {

  set_default_flags(ws_item_flags_default);

  begin_recording(hs_doc_rec_type_e::hs_doc_rec_init);
  add_hs_child(&m_copy_charges);
  m_copy_charges.set_value(ccd_copy_charges_mode::do_not_copy_charges);
  end_recording();

}

void ccd_view_t::manual_step_update(const int dir) {

  auto old_step = m_cur_step;
  auto new_step = m_cur_step + dir;
  if (new_step >= 0 && new_step < m_ccd->m_steps.size()) m_cur_step = new_step;
  if (old_step != m_cur_step) update_joined_atoms_list_animation(m_cur_step);

}

void ccd_view_t::manual_update_vib() {

  for (auto &items : m_connected_items) {
    auto al = items->cast_as<geom_view_t>();
    if (al && al->m_anim->get_total_anims() == m_ccd->m_vibs.size() + 1)  {
      al->m_anim->m_cur_anim = m_cur_vib + 1;
      al->m_anim->m_cur_anim_time = 0.0f;
      //TODO batch changes
      al->m_anim->m_play_anim.set_value(true);
      al->m_anim->m_play_cyclic.set_value(true);
      al->m_anim->m_anim_frame_time.set_value(0.1f);
    }
  }

}

void ccd_view_t::fill_custom_colors_of_geom_anim(const std::string &color_map_name) {

  app_state_t* astate = app_state_t::get_inst();

}

void ccd_view_t::update_connected_items() {

  switch (m_ccd->m_run_t) {

  case comp_chem_program_run_e::rt_unknown : {
    break;
  }

  case comp_chem_program_run_e::rt_energy : {
    break;
  }

  case comp_chem_program_run_e::rt_geo_opt : {

    for (auto con_itm : m_connected_items)
      if (auto as_gv = con_itm->cast_as<geom_view_t>(); as_gv && as_gv->m_anim->animable()) {
        for (size_t i = 0; i < as_gv->m_anim->get_total_anims(); i++)
          if (as_gv->m_anim->m_anim_data[i].m_anim_type == geom_anim_e::anim_geo_opt) {
            as_gv->m_anim->update_and_set_anim(i, m_cur_step);
            as_gv->m_anim->m_play_anim.set_value(false);

            //copy charges
            if (m_copy_charges.get_value() != ccd_copy_charges_mode::do_not_copy_charges
                && m_cur_step < m_ccd->m_steps.size() && m_connected_items.size() == 1)
              update_charges(as_gv, 0, as_gv->m_geom->nat());
            //end copy charges
            break;
          }
      }

    break;

  }

  case comp_chem_program_run_e::rt_vib : case comp_chem_program_run_e::rt_raman : {
    for (auto con_itm : m_connected_items)
      if (auto as_gv = con_itm->cast_as<geom_view_t>();
          as_gv &&
          as_gv->m_anim->animable() &&
          as_gv->m_anim->get_total_anims() == m_ccd->m_vibs.size() + 1 &&
          as_gv->m_anim->m_anim_data[m_cur_vib+1].m_anim_type == geom_anim_e::anim_vib) {
        as_gv->m_anim->update_and_set_anim(m_cur_vib+1, 0);
        as_gv->m_anim->m_play_anim.set_value(true);
      }
    break;
  }

  default : {
    break;
  }

  }

}

void ccd_view_t::update_charges(geom_view_t *gv, size_t start_atom, size_t end_atom) {

  for (size_t c = start_atom; c < end_atom; c++) {

    bool succes{false};

    switch (m_copy_charges.get_value()) {

    case ccd_copy_charges_mode::copy_mulliken : {

      if (c < m_ccd->m_steps[m_cur_step].m_mulliken_pop_per_atom.size()) {
        gv->m_geom->xfield<float>(xg_charge, c) =
            m_ccd->m_steps[m_cur_step].m_mulliken_net_chg_per_atom[c];
        succes = true;
      }
      break;

    };

    case ccd_copy_charges_mode::copy_mulliken_spin : {

      if (c < m_ccd->m_steps[m_cur_step].m_mulliken_spin_pop_per_atom.size()) {
        gv->m_geom->xfield<float>(xg_charge, c) =
            m_ccd->m_steps[m_cur_step].m_mulliken_spin_pop_per_atom[c];
        succes = true;
      }
      break;

    };

    case ccd_copy_charges_mode::copy_lowdin : {

      if (c < m_ccd->m_steps[m_cur_step].m_lowdin_pop_per_atom.size()) {
        gv->m_geom->xfield<float>(xg_charge, c) =
            m_ccd->m_steps[m_cur_step].m_lowdin_net_chg_per_atom[c];
        succes = true;
      }
      break;

    };

    case ccd_copy_charges_mode::copy_lowdin_spin : {

      if (c < m_ccd->m_steps[m_cur_step].m_lowdin_pop_per_atom.size()) {
        gv->m_geom->xfield<float>(xg_charge, c) =
            m_ccd->m_steps[m_cur_step].m_lowdin_spin_pop_per_atom[c];
        succes = true;
      }
      break;

    };

    default: {
      break;
    };

    }

    if (!succes) gv->m_geom->xfield<float>(xg_charge, c) = 0;

  } // end for

}

void ccd_view_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
                                       vector3<float> &vOutLookAt) {
  //do nothing
}

void ccd_view_t::render() {

}

void ccd_view_t::update_joined_atoms_list_animation(size_t step_idx) {

  for (auto &items : m_connected_items) {
    auto al = items->cast_as<geom_view_t>();
    if (al && al->m_anim->get_total_anims() > 1
        && al->m_anim->m_anim_data[1].frames.size() == m_ccd->m_steps.size())  {
      //fmt::print(std::cout, "Hallelujiah\n");
      al->m_anim->update_and_set_anim(1, step_idx);
    }
  }

}


bool ccd_view_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}


std::string ccd_view_t::compose_type_descr() {
  return "comp. chem. data";
}

void ccd_view_t::update(float delta_time) {

}

float ccd_view_t::get_bb_prescaller() {
  return 1.0f;
}

void ccd_view_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

  switch (update_reason) {

  case ws_item_updf_regenerate_content: {
    update_connected_items();
    break;
  }

  default: {
    break;
  }

  }

}

uint32_t ccd_view_t::get_num_cnt_selected() {
  return 0;
}

size_t ccd_view_t::get_content_count() {
  return 0;
}

void ccd_view_t::save_to_json(json &data) {

  ws_item_t::save_to_json(data);

}

void ccd_view_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);

}

void ccd_view_t::traverse_step_manual(int delta_step) {

  m_cur_step += delta_step;
  m_cur_step = std::clamp<int>(m_cur_step, 0, m_ccd->m_steps.size() - 1);

}

void ccd_view_t::traverse_step_boundary(bool to_the_begin) {

  (to_the_begin ? m_cur_step = 0 : m_cur_step = m_ccd->m_steps.size() - 1);

}

std::vector<size_t> ccd_view_t::query_vib_by_atoms(std::vector<size_t> atoms, float gate) {

  std::vector<size_t> ret;

  for (size_t i = 0; i < m_ccd->m_vibs.size(); i++) {

    bool passed = true;

    for (size_t q = 0; q < m_ccd->m_tot_nat; q++)
      if (m_ccd->m_vibs[i].m_disp[q].norm() < gate) passed = false;

    if (passed) ret.push_back(i);

  }

  return ret;

}

py::list ccd_view_t::py_query_vib_by_atoms(py::list atoms, float gate) {

  py::list ret;

  std::vector<size_t> atomsl;
  for (auto &elem : atoms)
    if (py::isinstance<py::int_>(elem)) atomsl.push_back(py::cast<size_t>(elem));

  auto ret_q = query_vib_by_atoms(atomsl, gate);
  for (auto &rec : ret_q) ret.append(rec);

  return ret;

}
