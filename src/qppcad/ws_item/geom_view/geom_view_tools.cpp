#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/ws_item/geom_view/geom_view_labels_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/ws_item/compl_list_view/compl_list_view.hpp>
#include <qppcad/core/app_state.hpp>
#include <random>

using namespace qpp;
using namespace qpp::cad;

void geom_view_tools_t::shake_atoms(geom_view_t *gv,
                                    std::set<size_t> atoms_to_shake,
                                    float magn) {

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

void geom_view_tools_t::name_sel_atoms_by_order(geom_view_t *gv) {

  if (!gv) return;

  gv->m_labels->m_style = geom_labels_style_e::show_custom;

  size_t atom_ord_c{0};

  for (auto &rec : gv->m_atom_idx_sel) {

      gv->m_geom->xfield<std::string>(xgeom_label_text, rec.m_atm) = std::to_string(atom_ord_c);
      gv->m_geom->xfield<bool>(xgeom_label_show, rec.m_atm) = true;
      atom_ord_c++;

    }

}

void geom_view_tools_t::name_sel_atoms_by_dist_to_point(geom_view_t *gv, vector3<float> point) {


  if (!gv) return;

  gv->m_labels->m_style = geom_labels_style_e::show_custom;

  std::vector<std::tuple<size_t, float> > tmp_dists;

  for (auto &rec : gv->m_atom_idx_sel) {

      float dist = (gv->m_geom->pos(rec.m_atm, rec.m_idx) - point).norm();
      std::tuple<size_t, float> tmp_dist_rec{rec.m_atm, dist};
      tmp_dists.push_back(std::move(tmp_dist_rec));

    }

  std::sort(std::begin(tmp_dists),
            std::end(tmp_dists),
            [](const auto & a, const auto & b) -> bool
  {return std::get<1>(a) < std::get<1>(b);});

  for (size_t i = 0; i < std::size(tmp_dists); i++) {

      gv->m_geom->xfield<bool>(xgeom_label_show, std::get<0>(tmp_dists[i])) = true;
      gv->m_geom->xfield<std::string>(xgeom_label_text, std::get<0>(tmp_dists[i])) =
          std::to_string(i);

    }

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

void geom_view_tools_t::flip_sel_atoms_in_cell(geom_view_t *gv,
                                               size_t dim_id,
                                               float flip_magn) {

  if (!gv) return;

  gv->begin_structure_change();

  index zero = index::D(gv->m_geom->DIM).all(0);

  for (auto &sel : gv->m_atom_idx_sel)
    if (sel.m_idx == zero)
      geom_view_tools_t::flip_atom_in_cell(gv, sel.m_atm, dim_id, flip_magn, false);

  gv->end_structure_change();

}

void geom_view_tools_t::align_atoms_to_point(geom_view_t *gv,
                                             vector3<float> fpoint) {

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

std::vector<size_t> geom_view_tools_t::get_atoms_sublattices(geom_view_t *gv,
                                                             float score_eps) {

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

void geom_view_tools_t::clamp_atoms_to_cell(geom_view_t *gv,
                                            bool ignore_selection,
                                            bool affect_anim) {

  if (!gv) return;

  //update geometry
  for (int i = 0; i < gv->m_geom->nat(); i++)
    if (gv->m_atom_idx_sel.find(atom_index_set_key(i, index::D(gv->m_geom->DIM).all(0)))
        != gv->m_atom_idx_sel.end() || ignore_selection) {

        vector3<float> pos = gv->m_geom->pos(i);
        gv->m_geom->change_pos(i, gv->m_geom->cell.reduce(pos));

      }

  //update animations
  if (affect_anim)
    for (auto &anim_rec : gv->m_anim->m_anim_data)
      for (auto &frame : anim_rec.frames)
        for (auto &atom_pos : frame.atom_pos) {
            auto reduced = gv->m_geom->cell.reduce(atom_pos);
            atom_pos = reduced;
          }

}

void geom_view_tools_t::translate_atoms_in_cell(geom_view_t *gv, vector3<float> tr_vec,
                                                bool clamp_atoms, bool affect_anim) {

  if (!gv) return;

  app_state_t *astate = app_state_t::get_inst();

  for (size_t i = 0; i < gv->m_geom->nat(); i++) gv->m_geom->coord(i) += tr_vec;
  if (affect_anim)
    for (auto &anim_rec : gv->m_anim->m_anim_data)
      for (auto &frame : anim_rec.frames)
        for (auto &atom_pos : frame.atom_pos)
          atom_pos += tr_vec;

  if (clamp_atoms) geom_view_tools_t::clamp_atoms_to_cell(gv, true, affect_anim);

  astate->make_viewport_dirty();

}

vector3<float> geom_view_tools_t::center_cell_on(geom_view_t *gv,
                                                 vector3<float> new_cnt,
                                                 bool clamp_atoms,
                                                 bool affect_anim) {

  //compute cell center
  vector3<float> cell_cnt{0};
  if (!gv) return cell_cnt;

  for (size_t i = 0 ; i < gv->m_geom->DIM; i++) cell_cnt += gv->m_geom->cell.v[i] * 0.5f;

  vector3<float> delta = cell_cnt - new_cnt;
  geom_view_tools_t::translate_atoms_in_cell(gv, delta, clamp_atoms, affect_anim);

  return cell_cnt;

}

void geom_view_tools_t::tr_align_geoms(geom_view_t *what_gv,
                                       geom_view_t *to_gv,
                                       vector3<float> start_offset,
                                       vector3<float> axis_steps,
                                       size_t total_steps) {

  if (!what_gv || !to_gv) return;

  vector3<float> cur_offset{0};
  float cur_score{0};

  vector3<float> best_offset{0};
  float best_score{0};

  vector3<float> atom_pos{0};

  const auto sphere_query_eps = 0.1f;

  for (size_t ix = 0; ix < total_steps; ix++)
    for (size_t iy = 0; iy < total_steps; iy++)
      for (size_t iz = 0; iz < total_steps; iz++) {

          cur_score = 0;

          cur_offset[0] = start_offset[0] + axis_steps[0] * ix;
          cur_offset[1] = start_offset[1] + axis_steps[1] * iy;
          cur_offset[2] = start_offset[2] + axis_steps[2] * iz;

          for (size_t q = 0; q < what_gv->m_geom->nat(); q++) {

              atom_pos = what_gv->m_geom->pos(q) + cur_offset;
              std::vector<tws_node_content_t<float, size_t> > res;
              to_gv->m_tws_tr->query_sphere(sphere_query_eps, atom_pos, res);

              for (auto &rec : res)
                if (to_gv->m_geom->atom_name(rec.m_atm) == what_gv->m_geom->atom_name(q)) {
                    cur_score += 100;
                  }

            }

          if (best_score > cur_score) {
              best_score = cur_score;
              best_offset = cur_offset;
            }

        }

  py::print(fmt::format("Best score = {}, Best offset = ({}, {}, {})",
                        best_score, best_offset[0], best_offset[1], best_offset[2]));

}

void geom_view_tools_t::compose_gv_from_images(pybind11::list gvs) {

  app_state_t *astate = app_state_t::get_inst();
  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws();

      if (!ok) throw std::invalid_argument("invalid ws");

      //bool succes{false};

      auto new_gv = astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(geom_view_t::get_type_static());
      new_gv->m_name = fmt::format("composed_{}", cur_ws->m_ws_items.size());
      cur_ws->add_item_to_ws(new_gv);

      auto new_as_gv = new_gv->cast_as<geom_view_t>();
      new_as_gv->begin_structure_change();

      int img_idx = 0;
      geom_anim_record_t<float> new_anim;
      new_anim.frames.resize(gvs.size());
      new_anim.m_anim_name = "composed0";
      new_anim.m_anim_type = geom_anim_t::anim_geo_opt;

      for (auto list_itm : gvs)
      if (py::isinstance<geom_view_t &>(list_itm)) {

      geom_view_t &gv_src = py::cast<geom_view_t &>(list_itm);
      //py::print(fmt::format("{} fff", gv_src.m_name));

      if (img_idx == 0) {
          gv_src.copy_to_xgeom(*new_as_gv->m_geom);
        }

      new_anim.frames[img_idx].atom_pos.resize(new_as_gv->m_geom->nat());

      for (size_t i = 0; i < new_as_gv->m_geom->nat(); i++)
        new_anim.frames[img_idx].atom_pos[i] = gv_src.m_geom->pos(i);

      img_idx++;

      gv_src.m_is_visible = false;

    }

  new_as_gv->m_anim->m_anim_data.emplace_back(std::move(new_anim));

  new_as_gv->end_structure_change();

  astate->make_viewport_dirty();

}

std::shared_ptr<geom_view_t> geom_view_tools_t::generate_ncells(geom_view_t *gv,
                                                                int s_a, int e_a,
                                                                int s_b, int e_b,
                                                                int s_c, int e_c) {

  if (!gv) return nullptr;
  if (gv->m_geom->DIM != 3) return nullptr;

  std::shared_ptr<geom_view_t> sc_al = std::make_shared<geom_view_t>();
  sc_al->m_geom->DIM = 0;
  sc_al->m_geom->cell.DIM = 0;
  sc_al->m_name = fmt::format("composed_{}", gv->m_parent_ws->m_ws_items.size());

  sc_al->begin_structure_change();

  for (auto i = 0; i < gv->m_geom->nat(); i++)
    for (iterator i_it(index({s_a, s_b, s_c}), index({e_a, e_b, e_c})); !i_it.end(); i_it++ ) {
        vector3<float> new_atom_pos = gv->m_geom->pos(i, i_it);
        sc_al->m_geom->add(gv->m_geom->atom(i), new_atom_pos);
        sc_al->m_geom->xfield<float>(xgeom_charge, sc_al->m_geom->nat()-1) =
            gv->m_geom->xfield<float>(xgeom_charge, i);
      }

  sc_al->end_structure_change();

  gv->m_parent_ws->add_item_to_ws(sc_al);

  return sc_al;

}

void geom_view_tools_t::generate_supercell(geometry<float, periodic_cell<float> > *src,
                                           geometry<float, periodic_cell<float> > *dst,
                                           index sc_dim,
                                           std::optional<geom_view_role_e> role) {

  //app_state_t::get_inst()->tlog("@SUPERCELL IDX {}", sc_dim);

  if (!src || !dst /*|| sc_dim != src->m_geom->DIM*/) {
      return;
    }

  if (src->DIM != dst->DIM) {
      return;
    }

  for (auto i = 0; i < sc_dim.DIM; i++)
    dst->cell.v[i] = src->cell.v[i] * (sc_dim(i) + 1);

  xgeometry<float, periodic_cell<float> > *xsrc = nullptr;
  xgeometry<float, periodic_cell<float> > *xdst = nullptr;

  if (src->is_xgeometry()) xsrc = (xgeometry<float, periodic_cell<float> > *)(&src);
  if (dst->is_xgeometry()) xdst = (xgeometry<float, periodic_cell<float> > *)(&dst);

  for (auto i = 0; i < src->nat(); i++)
    for (iterator idx_it(index::D(src->DIM).all(0), sc_dim); !idx_it.end(); idx_it++ ) {
        vector3<float> new_atom_pos = src->pos(i, idx_it);
        //        app_state_t::get_inst()->tlog("@SUPERCELL {} {} {} {}",
        //                                      i, new_atom_pos[0], new_atom_pos[1], new_atom_pos[2]);
        dst->add(src->atom(i), new_atom_pos);

        if (xsrc && xdst && role && *role == geom_view_role_e::r_uc)
          xdst->xfield<float>(xgeom_charge, src->nat()-1) = xsrc->xfield<float>(xgeom_charge, i);
      }

}

void geom_view_tools_t::gen_pair_dist_anim(geom_view_t *gv,
                                           size_t at1,
                                           size_t at2,
                                           size_t num_frames,
                                           float start_r,
                                           float end_r) {

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

void geom_view_tools_t::filter_uniq(geom_view_t *gv) {

}

void geom_view_tools_t::set_charge_for_type(geom_view_t *gv,
                                            std::map<std::string, float> &map_t_chg) {

  if (!gv) return;

  for (size_t i = 0; i < gv->m_geom->nat(); i++)
    if (auto chg_rec_ex = map_t_chg.find(gv->m_geom->atom_name(i));
        chg_rec_ex != map_t_chg.end()) gv->m_geom->charge(i) = chg_rec_ex->second;

}

void geom_view_tools_t::purify_atom_names_from_numbers(geom_view_t *gv) {

  app_state_t *astate = app_state_t::get_inst();

  if (!gv) return;

  for (size_t i = 0; i < gv->m_geom->nat(); i++) {

      auto &atom_name = gv->m_geom->atom(i);
      atom_name.erase(remove_if(atom_name.begin(),
                                atom_name.end(),
                                [](char c) {return !isalpha(c);} ),
                      atom_name.end());

    }

  gv->m_geom->build_types();
  gv->rebond();

  if (gv->m_selected) astate->astate_evd->cur_ws_selected_item_changed();

}

void geom_view_tools_t::cut_selected_as_new_gv(geom_view_t *gv, bool cut_selected) {

  if (!gv) return;
  if (gv->m_atom_idx_sel.empty()) return;

  std::shared_ptr<geom_view_t> ret_gv = std::make_shared<geom_view_t>();
  ret_gv->copy_cell(*gv, false);

  ret_gv->begin_structure_change();

  index zero_gv = index::D(gv->m_geom->DIM).all(0);
  for (auto &rec : gv->m_atom_idx_sel)
    if (rec.m_idx == zero_gv) {
        ret_gv->m_geom->add(gv->m_geom->atom(rec.m_atm), gv->m_geom->pos(rec.m_atm, rec.m_idx));
        std::vector<datum> v;
        gv->m_geom->get_fields(rec.m_atm, v);
        ret_gv->m_geom->set_fields(ret_gv->m_geom->nat()-1, v);
      }

  ret_gv->end_structure_change();

  if (cut_selected) gv->delete_selected_atoms();

  ret_gv->m_name = fmt::format("{}{}", gv->m_name, gv->m_parent_ws->m_ws_items.size());
  gv->m_parent_ws->add_item_to_ws(ret_gv);

}

std::map<std::string, size_t> geom_view_tools_t::get_sel_types(geom_view_t *gv) {

  std::map<std::string, size_t> retv;

  if (!gv) return retv;

  std::vector<size_t> tmp_tc;
  tmp_tc.resize(gv->m_geom->n_types());

  index zero = index::D(gv->m_geom->DIM).all(0);
  for (auto &rec : gv->m_atom_idx_sel)
    if (rec.m_idx == zero)
      tmp_tc[gv->m_geom->type_table(rec.m_atm)]++;

  for (size_t i = 0; i < gv->m_geom->n_types(); i++)
    retv[gv->m_geom->atom_of_type(i)] = tmp_tc[i];

  return retv;

}

void geom_view_tools_t::naive_project_displ(geom_view_t *src,
                                            geom_view_t *dst,
                                            float eps_dist,
                                            bool check_run) {

  if (!src || !dst || eps_dist < 0) return;

  py::print(fmt::format("Check run = {}", check_run));

  for (size_t i = 0; i < dst->m_geom->nat(); i++) {

      std::vector<tws_node_content_t<float> > qs_res;
      src->m_tws_tr->query_sphere(eps_dist, dst->m_geom->pos(i), qs_res);

      if (qs_res.empty()) continue;

      auto pos_src = src->m_geom->pos(qs_res.front().m_atm);
      auto pos_dst = dst->m_geom->pos(i);

      py::print(
            fmt::format("Hit: src_i = {}, src_p = {}, dst_i = {}, dst_p = {}, dr = {}",
                        qs_res.front().m_atm,
                        pos_src,
                        i,
                        pos_dst,
                        (pos_src-pos_dst).norm())
            );

      if (!check_run) dst->m_geom->coord(i) = pos_src;

    }

}

void geom_view_tools_t::naive_fit_str(geom_view_t *model,
                                      geom_view_t *target,
                                      std::vector<size_t> &model_idx,
                                      std::vector<size_t> &target_idx) {

  if (!model || !target || (model_idx.size() != target_idx.size())) return;

  std::vector<vector3<float> > model_pos;
  std::vector<vector3<float> > target_pos;

  std::transform(model_idx.begin(),
                 model_idx.end(),
                 std::back_inserter(model_pos),
                 [model](auto atm_idx){return model->m_geom->pos(atm_idx);});

  std::transform(target_idx.begin(),
                 target_idx.end(),
                 std::back_inserter(target_pos),
                 [target](auto atm_idx){return target->m_geom->pos(atm_idx);});

  std::vector<vector3<float> > diff_pos;
  std::transform(target_pos.begin(),
                 target_pos.end(),
                 model_pos.begin(),
                 std::back_inserter(diff_pos),
                 [](auto &t1, auto &t2){ return t1 - t2;});

  vector3<float> aver_pos =
      std::accumulate(diff_pos.begin(), diff_pos.end(), vector3<float>{0}) / diff_pos.size();

  model->m_pos = aver_pos;

}

std::vector<std::tuple<size_t, size_t> > geom_view_tools_t::gen_geoms_compl_list(
    geom_view_t *model,
    geom_view_t *target,
    float compl_eps,
    bool only_affect_visible_atoms) {

  std::vector<std::tuple<size_t, size_t> > retv;
  if (!model || !target) return retv;

  for (size_t i = 0; i < target->m_geom->nat(); i++)
    if (!only_affect_visible_atoms ||
        (only_affect_visible_atoms && !target->m_geom->xfield<bool>(xgeom_sel_vis_hide, i))) {

        // accuire atom pos
        auto atom_pos = target->m_geom->pos(i);

        // transform to world frame
        atom_pos += target->m_pos;

        // transform from world to model frame
        atom_pos -= model->m_pos;

        std::vector<tws_node_content_t<float> > qs_res;
        model->m_tws_tr->query_sphere(compl_eps, atom_pos, qs_res);

        if (!qs_res.empty() && qs_res.front().m_atm < model->m_geom->nat() &&
            !model->m_geom->xfield<bool>(xgeom_sel_vis_hide, qs_res.front().m_atm))
          retv.push_back({qs_res.front().m_atm, i});

      }

  return retv;

}

void geom_view_tools_t::construct_compl_list_view(geom_view_t *model,
                                                  geom_view_t *target,
                                                  float compl_eps,
                                                  bool only_affect_visible_atoms) {

  std::vector<std::tuple<size_t, size_t> > res =
      geom_view_tools_t::gen_geoms_compl_list(model, target, compl_eps, only_affect_visible_atoms);

  std::shared_ptr<compl_list_view_t> clv = std::make_shared<compl_list_view_t>();
  clv->m_name = fmt::format("clv_{}", model->m_name);

  clv->m_compl_list = std::move(res);

  model->m_parent_ws->add_item_to_ws(clv);

}

void geom_view_tools_t::displ_geom_by_comp_list(
    geom_view_t *target,
    geom_view_t *d_start,
    geom_view_t *d_end,
    std::vector<std::tuple<size_t, size_t> > &comp_l,
    int d_start_anim,
    int d_end_anim,
    int d_start_frame,
    int d_end_frame) {

  if (!d_start || !d_end || !target || comp_l.empty()) return ;

  //first anim invalid
  if (d_start_anim > -1 && d_start_anim >= d_start->m_anim->m_anim_data.size() &&
      !d_start->m_anim->m_anim_data[d_start_anim].frames.empty() &&
      d_start_frame < d_start->m_anim->m_anim_data[d_start_anim].frames.size())
    return;

  //first anim invalid
  if (d_end_anim > -1 && d_end_anim >= d_end->m_anim->m_anim_data.size() &&
      !d_end->m_anim->m_anim_data[d_end_anim].frames.empty() &&
      d_end_frame < d_end->m_anim->m_anim_data[d_end_anim].frames.size())
    return;

  for (auto const &rec : comp_l) {

      // unpack tuple - { model idx , target idx }
      auto model_idx = std::get<0>(rec);
      auto target_idx = std::get<1>(rec);

      if (model_idx >= d_start->m_geom->nat() || target_idx >= target->m_geom->nat())
        continue;

      // process displacements

      vector3<float> &target_coord = target->m_geom->coord(target_idx);

      vector3<float> start_coord = d_start_anim == -1 ?
            d_start->m_geom->pos(model_idx) :
            d_start->m_anim->m_anim_data[d_start_anim].frames[d_start_frame].atom_pos[model_idx];

      vector3<float> end_coord = d_end_anim == -1 ?
            d_end->m_geom->pos(model_idx) :
            d_end->m_anim->m_anim_data[d_end_anim].frames[d_end_frame].atom_pos[model_idx];

      target_coord += (end_coord - start_coord);

    }


}

void geom_view_tools_t::change_atom_type(const std::string &src,
                                         const std::string &dst,
                                         geom_view_t *gv) {

  app_state_t *astate = app_state_t::get_inst();

  auto tmp_gv = gv;

  if (!tmp_gv) {
     auto [cur_ws, cur_it, al] = astate->ws_mgr->get_sel_tpl_itm<geom_view_t>(error_ctx_throw);
     if (al) tmp_gv = al;
  }

  int src_id = tmp_gv->m_geom->type_of_atom(src);
  //int dst_id = gv->m_geom->type_of_atom(dst);

  if (src_id == -1) {
   return;
  }

  tmp_gv->begin_structure_change();

  for (size_t i = 0; i < tmp_gv->m_geom->nat(); i++)
    if (tmp_gv->m_geom->type_table(i) == src_id)
      tmp_gv->m_geom->change(i, dst, tmp_gv->m_geom->pos(i));

  tmp_gv->end_structure_change();

}

void geom_view_tools_t::merge_gv(geom_view_t *gv_src1,
                                 geom_view_t *gv_src2,
                                 geom_view_t *gv_dst) {

  app_state_t *astate = app_state_t::get_inst();

  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (!cur_ws) return;

  if (!gv_src1 || !gv_src2) {
      return;
    }

  geom_view_t *tmp_dst = gv_dst;

  if (!tmp_dst) {

      std::shared_ptr<geom_view_t> sc_al = std::make_shared<geom_view_t>();

      gv_src1->m_parent_ws->add_item_to_ws(sc_al);
      tmp_dst = sc_al.get();

    }

  if (!tmp_dst) {
      return;
    }

  tmp_dst->begin_structure_change();

  //make decision about cell
  if (gv_src1->m_geom->DIM == gv_src2->m_geom->DIM) {

      int DIM = gv_src1->m_geom->DIM;
      tmp_dst->m_geom->DIM = DIM;
      tmp_dst->m_geom->cell.DIM = DIM;

      if (DIM == 1) {
          if (gv_src1->m_geom->cell.v[0].norm() > gv_src2->m_geom->cell.v[0].norm())
            tmp_dst->copy_cell(*gv_src1, false);
          else
            tmp_dst->copy_cell(*gv_src2, false);
        }

      if (DIM == 2) {

          float sq1 = (gv_src1->m_geom->cell.v[0].cross(gv_src1->m_geom->cell.v[1])).norm();
          float sq2 = (gv_src2->m_geom->cell.v[0].cross(gv_src2->m_geom->cell.v[1])).norm();

          if (sq1 > sq2) tmp_dst->copy_cell(*gv_src1, false);
          else tmp_dst->copy_cell(*gv_src2, false);

        }

      if (DIM == 3) {

          float vol1 = (gv_src1->m_geom->cell.v[0].cross(gv_src1->m_geom->cell.v[1])).dot(
                gv_src1->m_geom->cell.v[2]);
          float vol2 = (gv_src2->m_geom->cell.v[0].cross(gv_src2->m_geom->cell.v[1])).dot(
                gv_src2->m_geom->cell.v[2]);

          if (vol1 > vol2) tmp_dst->copy_cell(*gv_src1, false);
          else tmp_dst->copy_cell(*gv_src2, false);

        }

    } else if (gv_src1->m_geom->DIM > gv_src2->m_geom->DIM) {

      tmp_dst->copy_cell(*gv_src1, false);

    } else if (gv_src1->m_geom->DIM < gv_src2->m_geom->DIM) {

      tmp_dst->copy_cell(*gv_src2, false);

    }

  astate->tlog("MERGE DIM {}", tmp_dst->m_geom->DIM);
  astate->tlog("MERGE CELL DIM {}", tmp_dst->m_geom->cell.DIM);

  for (size_t i = 0 ; i < tmp_dst->m_geom->DIM; i++)
    astate->tlog("MERGE VEC {} {}", i, tmp_dst->m_geom->cell.v[i]);

  for (auto elem : {gv_src1, gv_src2})
    if (elem != tmp_dst)
      for (size_t i = 0; i < elem->m_geom->nat(); i++) {
          tmp_dst->m_geom->add(elem->m_geom->atom(i), elem->m_geom->pos(i));
          tmp_dst->m_geom->xfield<float>(xgeom_charge, tmp_dst->m_geom->nat()-1) =
              elem->m_geom->xfield<float>(xgeom_charge, i);
        }

  tmp_dst->end_structure_change();

  if (tmp_dst->m_name.empty())
    tmp_dst->set_name(fmt::format("merged_{}", gv_src1->m_parent_ws->m_ws_items.size()));

  astate->astate_evd->cur_ws_changed();

}

void geom_view_tools_t::sort_gv_by_point(geom_view_t *gv, vector3<float> point) {

  if (!gv) return;

  auto sort_func =
      [&point](const geometry<float, periodic_cell<float>> &geom, int idx) -> float {
      return (point-geom.pos(idx)).norm();
    };

  gv->begin_structure_change();
  gv->m_geom->sort(sort_func);
  gv->end_structure_change();

}

void geom_view_tools_t::sort_gv(
    geom_view_t *gv,
    const std::function<float (const geom_view_tools_t::geometry_t &, int)> &key) {

  if (!gv) return;
  gv->begin_structure_change();
  gv->m_geom->sort(key);
  gv->end_structure_change();

}

void geom_view_tools_t::change_cell_keep_atoms(geom_view_t *gv,
                                               vector3<float> new_a,
                                               vector3<float> new_b,
                                               vector3<float> new_c) {

  if (!gv) return;

  gv->begin_structure_change();

  periodic_cell<float> new_cell(3);
  new_cell.v[0] = new_a;
  new_cell.v[1] = new_b;
  new_cell.v[2] = new_c;

  for (auto i = 0; i < gv->m_geom->nat(); i++) {
      vector3<float> frac_in_old_cell = gv->m_geom->cell.cart2frac(gv->m_geom->pos(i));
      gv->m_geom->change_pos(i, new_cell.frac2cart(frac_in_old_cell));
    }

  gv->m_geom->cell.v[0] = new_cell.v[0];
  gv->m_geom->cell.v[1] = new_cell.v[1];
  gv->m_geom->cell.v[2] = new_cell.v[2];

  gv->end_structure_change();

}
