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

void geom_view_tools_t::flip_atom_in_cell(geom_view_t *gv,
                                          size_t at_id,
                                          size_t dim_id,
                                          float flip_magn,
                                          bool rebuild_tree) {

  if (!gv) return;

  app_state_t *astate = app_state_t::get_inst();

  if (at_id >= gv->m_geom->nat() || dim_id > gv->m_geom->DIM) return;

  if (rebuild_tree) gv->begin_structure_change();

  auto as_frac = gv->m_geom->cell.cart2frac(gv->m_geom->pos(at_id));
  as_frac[dim_id] = as_frac[dim_id] + flip_magn;
  auto as_cart = gv->m_geom->cell.frac2cart(as_frac);
  gv->m_geom->coord(at_id) = as_cart;

  if (rebuild_tree) gv->end_structure_change();

  astate->make_viewport_dirty();

}

void geom_view_tools_t::flip_sel_atoms_in_cell(geom_view_t *gv, size_t dim_id, float flip_magn) {

  if (!gv) return;

  gv->begin_structure_change();

  index zero = index::D(gv->m_geom->DIM).all(0);

  for (auto &sel : gv->m_atom_idx_sel)
    if (sel.m_idx == zero)
      geom_view_tools_t::flip_atom_in_cell(gv, sel.m_atm, dim_id, flip_magn, false);

  gv->end_structure_change();

}

void geom_view_tools_t::align_atoms_to_point(geom_view_t *gv, vector3<float> fpoint) {

  if (!gv) return;

  gv->begin_structure_change();

  for (size_t i = 0; i < gv->m_geom->nat(); i++) {

      float min_dist = 100.0f;
      auto goal_vector = gv->m_geom->pos(i);
      for (iterator idx(index::D(gv->m_geom->DIM).all(-1),
                        index::D(gv->m_geom->DIM).all(1)); !idx.end(); idx++ ) {
          auto t_pos_cf = gv->m_geom->cell.transform(gv->m_geom->pos(i), idx);
          auto dist = (fpoint - t_pos_cf).norm();
          if (dist < min_dist && gv->m_geom->cell.within(t_pos_cf)) {
              min_dist = dist;
              //min_dist_index = i;
              goal_vector = t_pos_cf;
            }
        }

      gv->m_geom->coord(i) = goal_vector;

    }

  gv->end_structure_change();

}

std::vector<size_t> geom_view_tools_t::get_atoms_cn(geom_view_t *gv) {

  std::vector<size_t> retv;

  if (!gv) return retv;

  retv.resize(gv->m_geom->nat(), 0);
  tws_tree_t<float, periodic_cell<float> > l_tws_tree(*gv->m_geom);

  l_tws_tree.m_cell_within_eps = 0.3f;

  l_tws_tree.do_action(act_unlock | act_rebuild_tree);
  l_tws_tree.do_action(act_rebuild_ntable);

  for (size_t i = 0; i < gv->m_geom->nat(); i++) retv[i] = l_tws_tree.n(i);

  return retv;

}

std::vector<size_t> geom_view_tools_t::get_atoms_sublattices(geom_view_t *gv, float score_eps) {

  std::vector<size_t> retv, sfv;

  if (!gv) return retv;

  retv.resize(gv->m_geom->nat(), 0);
  sfv.resize(gv->m_geom->nat(), 0);

  tws_tree_t<float, periodic_cell<float> > l_tws_tree(*gv->m_geom);

  l_tws_tree.m_cell_within_eps = 0.3f;

  l_tws_tree.do_action(act_unlock | act_rebuild_tree);
  l_tws_tree.do_action(act_rebuild_ntable);

  for (size_t i = 0; i < gv->m_geom->nat(); i++) sfv[i] = l_tws_tree.n(i);
  auto max_sfv = std::max_element(sfv.begin(), sfv.end());

  std::vector<Eigen::VectorXf> dl_subspace;
  std::vector<Eigen::VectorXf> dl_uniq_subspace;
  dl_subspace.resize(gv->m_geom->nat(), Eigen::VectorXf::Zero(*max_sfv + 1));

  for (size_t i = 0; i < gv->m_geom->nat(); i++) {
      auto pos_i = gv->m_geom->pos(i);
      dl_subspace[i](0) = gv->m_geom->type_table(i);
      for (size_t q = 0; q < l_tws_tree.n(i); q++) {
          auto pos_q = gv->m_geom->pos(l_tws_tree.table_atm(i,q), l_tws_tree.table_idx(i,q));
          dl_subspace[i](q + 1) = (pos_q - pos_i).norm();
        }
    }

  //sort by descending order
  for (size_t i = 0; i < gv->m_geom->nat(); i++)
    std::sort(dl_subspace[i].data() + 1,
              dl_subspace[i].data() + *max_sfv + 1,
              std::greater<float>());

  //build unique
  for (size_t i = 0; i < gv->m_geom->nat(); i++) {

      int best_uniq_id = -1;

      for (size_t q = 0; q < dl_uniq_subspace.size(); q++) {
          auto score = (dl_uniq_subspace[q] - dl_subspace[i]).norm();
          if (score < score_eps) best_uniq_id = q;
        }

      if (best_uniq_id == -1 ) {
          dl_uniq_subspace.push_back(dl_subspace[i]);
          retv[i] = dl_uniq_subspace.size() - 1;
        } else {
          retv[i] = best_uniq_id;
        }

    }

  //debug print unique
  py::print(fmt::format("Total unique types: {}", dl_uniq_subspace.size()));

  //debug print to python console
  for (size_t i = 0; i < gv->m_geom->nat(); i++) {

      std::string dp_str = "";

      for (size_t q = 0; q < *max_sfv+1; q++)
        dp_str += fmt::format("{}, ", dl_subspace[i][q]);

      py::print(dp_str);

    }

  return retv;

}

void geom_view_tools_t::clamp_atoms_to_cell(geom_view_t *gv, bool ignore_selection) {

  if (!gv) return;

  for (int i = 0; i < gv->m_geom->nat(); i++)
    if (gv->m_atom_idx_sel.find(atom_index_set_key(i, index::D(gv->m_geom->DIM).all(0)))
        != gv->m_atom_idx_sel.end() || ignore_selection) {

        vector3<float> pos = gv->m_geom->pos(i);
        gv->m_geom->change_pos(i, gv->m_geom->cell.reduce(pos));

      }

}

vector3<float> geom_view_tools_t::center_cell_on(geom_view_t *gv,
                                                 vector3<float> new_cnt,
                                                 bool clamp_atoms) {

  //compute cell center
  vector3<float> cell_cnt{0};
  if (!gv) return cell_cnt;

  app_state_t *astate = app_state_t::get_inst();

  for (size_t i = 0 ; i < gv->m_geom->DIM; i++) cell_cnt += gv->m_geom->cell.v[i] * 0.5f;

  vector3<float> delta = cell_cnt - new_cnt;
  for (size_t i = 0; i < gv->m_geom->nat(); i++) gv->m_geom->coord(i) += delta;

  if (clamp_atoms) geom_view_tools_t::clamp_atoms_to_cell(gv);

  astate->make_viewport_dirty();

  return cell_cnt;

}
