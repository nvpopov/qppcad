#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/ws_item/geom_view/geom_view_labels_subsys.hpp>
#include <qppcad/app_state.hpp>
#include <random>

using namespace qpp;
using namespace qpp::cad;

void geom_view_tools_t::shake_atoms(geom_view_t *gv, std::set<size_t> atoms_to_shake, float magn) {

  if (!gv) return;

  app_state_t* astate = app_state_t::get_inst();

  static std::default_random_engine e;
  static std::uniform_real_distribution<> dis(0, magn);

  for (auto &atom : atoms_to_shake)
    if (atom < gv->m_geom->nat()) {

        vector3<float> new_pos = gv->m_geom->coord(atom) +
                                 vector3<float>(magn / 2) -
                                 vector3<float>(dis(e), dis(e), dis(e));

        gv->m_geom->coord(atom) = new_pos;

      }

  astate->make_viewport_dirty();

}

void geom_view_tools_t::py_shake_atoms(geom_view_t *gv, py::list atoms_to_shake, float magn) {

  if (!gv) return;

  std::set<size_t> atoms;
  for (auto elem : atoms_to_shake)
    if (py::isinstance<py::int_>(elem)) atoms.insert(py::cast<int>(elem));

  geom_view_tools_t::shake_atoms(gv, atoms, magn);

}

void geom_view_tools_t::purify_boundary_atoms(geom_view_t *dst, geom_view_t *src) {

  if (!dst || !src) return;

  if (!src || src->m_geom->nat() != dst->m_geom->nat()) return;

  dst->begin_structure_change();

  for (size_t i = 0; i < dst->m_geom->nat(); i++) {

      float min_dist = 100.0f;
      auto goal_vector = dst->m_geom->pos(i);
      auto pos_in_src = src->m_geom->pos(i);
      for (iterator idx(index::D(dst->m_geom->DIM).all(-1),
                        index::D(dst->m_geom->DIM).all(1)); !idx.end(); idx++ ) {
          auto t_pos_cf = dst->m_geom->cell.transform(dst->m_geom->pos(i), idx);
          auto dist = (pos_in_src - t_pos_cf).norm();
          if (dist < min_dist) {
              min_dist = dist;
              //min_dist_index = i;
              goal_vector = t_pos_cf;
            }
        }

      dst->m_geom->coord(i) = goal_vector;

    }

  dst->end_structure_change();

}

vector3<float> geom_view_tools_t::dipole_moment(geom_view_t *gv) {

  if (!gv) return vector3<float>(std::numeric_limits<float>::max());

  vector3<float> accum_dm{0,0,0};
  for (int i = 0; i < gv->m_geom->nat(); i++)
    accum_dm += gv->m_geom->pos(i) * gv->m_geom->xfield<float>(xgeom_charge, i);
  return accum_dm;

}

float geom_view_tools_t::total_charge(geom_view_t *gv) {

  if (!gv) return std::numeric_limits<float>::max();

  float accum_charge{0};
  for (int i = 0; i < gv->m_geom->nat(); i++)
    accum_charge += gv->m_geom->xfield<float>(xgeom_charge, i);
  return accum_charge;

}

std::vector<scalar_partition_per_type_t<> > geom_view_tools_t::get_charge_partition(
    geom_view_t *gv) {

  std::vector<scalar_partition_per_type_t<>> retcp;
  if (!gv) return retcp;

  const auto ch_eps = 0.0001f;

  for (size_t i = 0; i < gv->m_geom->nat(); i++) {

      bool rec_founded{false};

      for (size_t q = 0; q < retcp.size(); q++)
        if (retcp[q].atype == gv->m_geom->type(i) &&
            (std::fabs(retcp[q].value - gv->m_geom->xfield<float>(xgeom_charge, i)) < ch_eps)) {
            rec_founded = true;
            retcp[q].count++;
          }

      if (!rec_founded) {
          scalar_partition_per_type_t<> tmp_rec;
          tmp_rec.atype = gv->m_geom->type(i);
          tmp_rec.value = gv->m_geom->xfield<float>(xgeom_charge, i);
          tmp_rec.count = 1;
          retcp.push_back(std::move(tmp_rec));
        }

    }

  return retcp;

}

py::list geom_view_tools_t::py_get_charge_partition(geom_view_t *gv) {

  py::list ret_list;
  if (!gv) return ret_list;

  auto charge_part =geom_view_tools_t::get_charge_partition(gv);

  for (auto &ch_rec : charge_part) {

      py::list ch_rec_list;
      ch_rec_list.append(ch_rec.atype);
      ch_rec_list.append(ch_rec.value);
      ch_rec_list.append(ch_rec.count);

      ret_list.append(ch_rec_list);

    }

  return ret_list;

}

void geom_view_tools_t::pertrub_via_normal_mode(geom_view_t *gv,
                                                generic_array_t<vector3<float>, float> &disp) {

  if (!gv) return;

  if (gv->m_geom->nat() != disp.size()) return;

  for (size_t i = 0; i < gv->m_geom->nat(); i++)
    gv->m_geom->coord(i) = gv->m_geom->coord(i) + disp[i];

}

std::string geom_view_tools_t::pretty_print_selected_atoms(geom_view_t *gv,
                                                           vector3<float> new_frame) {

  std::string ret;
  if (!gv) return ret;

  bool first = true;
  for (auto &rec : gv->m_atom_idx_sel) {

      vector3<float> pos_i = gv->m_geom->pos(rec.m_atm) - new_frame;

      geom_labels_style_e cur_style =
          gv->m_labels->m_style == geom_labels_style_e::show_none ?
            geom_labels_style_e::show_id_type :
            gv->m_labels->m_style;

      ret +=
          fmt::format(first ? "{} {:8.8f} {:8.8f} {:8.8f}" : "\n{} {:8.8f} {:8.8f} {:8.8f}",
                      geom_view_labels_subsys_t::label_gen_fn(gv, cur_style, rec.m_atm),
                      pos_i[0],
                      pos_i[1],
                      pos_i[2]
          );

      first = false;

    }

  return ret;

}
