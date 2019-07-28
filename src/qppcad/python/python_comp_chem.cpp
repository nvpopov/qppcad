#include <qppcad/python/python_comp_chem.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void cp2k_helper_t::save_neb_data(std::shared_ptr<ws_item_t> g_start,
                                  std::shared_ptr<ws_item_t> g_end,
                                  std::string path) {

}

std::string cp2k_helper_t::gen_fixed_atoms_section(py::list &fixed_atoms) {

  std::string ret;
  ret  = "&FIXED_ATOMS\nCOMPONENTS_TO_FIX XYZ\n";

  const auto items_per_section = 14;
  size_t num_sections = fixed_atoms.size() / items_per_section;

  for (size_t sec = 0; sec <= num_sections; sec++) {
      ret += "LIST ";
      for (size_t itm_c = 0; itm_c < items_per_section; itm_c++) {
          size_t idx_in_list = sec * items_per_section + itm_c;
          if (idx_in_list < fixed_atoms.size()) {
              auto value = py::cast<int>(fixed_atoms[idx_in_list]);
              ret += " " + std::to_string(value);
            }
        }
      ret += "\n";
    }

  ret += "&END FIXED_ATOMS\n";
  return ret;

}

void generic_qc_helper_t::generate_pair_dist_calc_data(std::shared_ptr<geom_view_t> gv,
                                                       size_t at1, size_t at2, size_t num_frames,
                                                       float start_r, float end_r) {

  app_state_t* astate = app_state_t::get_inst();

  if (!gv) return;
  if (at1 >= gv->m_geom->nat() || at2 >= gv->m_geom->nat() || at1 == at2) return;
  if (start_r > end_r || start_r < 0 || end_r < 0) return;

  //compose transformation data
  auto cnt = (gv->m_geom->pos(at1) + gv->m_geom->pos(at2)) * 0.5f;
  auto dir1 = (cnt - gv->m_geom->pos(at1)).normalized();
  auto dir2 = (cnt - gv->m_geom->pos(at2)).normalized();
  auto dr = 0.5f * (end_r - start_r)/(num_frames-1);

  //create static anim for current state
  gv->m_anim->make_static_anim();

  //compose new anim
  geom_anim_record_t<float> new_anim;
  new_anim.m_anim_name = fmt::format("pair_dist{}", gv->m_anim->get_total_anims());
  new_anim.m_anim_type = geom_anim_t::anim_generic;

  new_anim.frames.resize(num_frames);

  size_t tot_at = gv->m_geom->nat();

  for (size_t i = 0; i < num_frames; i++) {

      new_anim.frames[i].atom_pos.resize(tot_at);

      for (size_t q = 0; q < tot_at; q++)
        new_anim.frames[i].atom_pos[q] = gv->m_geom->pos(q);

      new_anim.frames[i].atom_pos[at1] = cnt - dir1 * (start_r * 0.5f + dr * i);
      new_anim.frames[i].atom_pos[at2] = cnt - dir2 * (start_r * 0.5f + dr * i);

    }

  gv->m_anim->m_anim_data.emplace_back(std::move(new_anim));

  if (gv->is_selected())
    astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();

}
