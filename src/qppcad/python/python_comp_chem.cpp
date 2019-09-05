#include <qppcad/python/python_comp_chem.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/core/app_state.hpp>

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

std::string orca_helper_t::gen_multijob_from_anim(geom_view_t *gv, size_t anim_id) {

  //app_state_t* astate = app_state_t::get_inst();

  std::string rets;

  if (!gv) {
      throw std::invalid_argument("invalid gv!");
      return rets;
    }

  if (anim_id >= gv->m_anim->get_total_anims()) {
      throw std::invalid_argument(fmt::format("invalid anim id! ctx=[anim_id={}, tot_anim={}]",
                                              anim_id, gv->m_anim->get_total_anims()));
      return rets;
    }

  if (gv->m_anim->m_anim_data[anim_id].frames.size() == 0) {
      throw std::invalid_argument(
            fmt::format("gv->m_anim->m_anim_data[anim_id].frames.size == {}!",
            gv->m_anim->m_anim_data[anim_id].frames.size()));
      return rets;
    }

  for (size_t i = 0; i < gv->m_anim->m_anim_data[anim_id].frames.size(); i++) {

      rets += fmt::format("#step {}\n", i);
      if (i > 0) rets += "$new_job\n";
      rets += "! RHF aug-cc-pVQZ RIJK AutoAux VeryTightSCF DIRECT\n";
      rets += "#params\n";
      rets += "*xyz 0 1\n";

      for (size_t q = 0; q < gv->m_geom->nat(); q++) {
          rets += fmt::format("{} {:>16.5f} {:>16.5f} {:>16.5f}\n",
                              gv->m_geom->atom_name(q),
                              gv->m_anim->m_anim_data[anim_id].frames[i].atom_pos[q][0],
                              gv->m_anim->m_anim_data[anim_id].frames[i].atom_pos[q][1],
                              gv->m_anim->m_anim_data[anim_id].frames[i].atom_pos[q][2]);
        }

      rets += "*\n\n";

    }

  return rets;

}

std::string orca_helper_t::gen_coord_section(geom_view_t *gv,
                                             std::vector<std::string> &is_point_charge,
                                             std::vector<std::string> &basis_less_cnt,
                                             std::map<std::string, std::string> &ecp_name) {

  std::string rets;

  if (!gv) {
      throw std::invalid_argument("invalid gv!");
      return rets;
    }

  for (size_t q = 0; q < gv->m_geom->nat(); q++) {

      auto &pos = gv->m_geom->coord(q);
      auto &atom_name = gv->m_geom->atom(q);

      auto is_point_charge_itr =
          std::find(is_point_charge.begin(), is_point_charge.end(), atom_name);
      bool is_pc = is_point_charge_itr != is_point_charge.end();

      auto is_basis_less_cnt_itr =
          std::find(basis_less_cnt.begin(), basis_less_cnt.end(), atom_name);
      bool is_blc = is_basis_less_cnt_itr != basis_less_cnt.end();

      auto has_blc_ecp_it = ecp_name.find(atom_name);
      bool has_blc_ecp = has_blc_ecp_it != ecp_name.end();

      rets += fmt::format("{}{} {} {:>16.5f} {:>16.5f} {:>16.5f} {}\n",
                          is_pc ? "Q" : gv->m_geom->atom_name(q),
                          is_blc ? ">" : "",
                          !is_pc && !is_blc ? "" : "   "+std::to_string(gv->m_geom->charge(q)),
                          pos[0], pos[1], pos[2],
                          is_blc && has_blc_ecp ?
                            fmt::format("   NewECP \"{}\" end", has_blc_ecp_it->second) : ""
          );

    }

  return rets;

}

std::string generic_qc_helper_t::get_raw_coords_section(std::shared_ptr<geom_view_t> gv) {

  std::string retv;

  for (size_t i = 0; i < gv->m_geom->nat(); i++)
    retv+= fmt::format("{} {:15.8f} {:15.8f} {:15.8f}\n",
               gv->m_geom->atom(i),
               gv->m_geom->pos(i)[0], gv->m_geom->pos(i)[1], gv->m_geom->pos(i)[2]);

  return retv;

}
