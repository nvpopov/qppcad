#include <qppcad/embedded_cluster_tools.hpp>
#include <geom/builders.hpp>
#include <geom/shape.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void embedded_cluster_tools::gen_spherical_cluster(ws_atoms_list_t *uc,
                                                   vector3<float> displ,
                                                   float cluster_r,
                                                   float cls_r,
                                                   bool generate_qm ,
                                                   float qm_r) {

  if (!uc || uc->m_role != ws_atoms_list_role_t::role_uc) return;
  if (cls_r > cluster_r) return;

  //try to find charges, classic and quantum ws_atoms_list`s
  ws_atoms_list_t *chg{nullptr};
  ws_atoms_list_t *cls{nullptr};
  ws_atoms_list_t *qm{nullptr};

  for (auto elem : uc->m_connected_items) {
      auto as_al = dynamic_cast<ws_atoms_list_t*>(elem.get());
      if (as_al) {
          if (as_al->m_role == ws_atoms_list_role_t::role_embc_chg) chg = as_al;
          if (as_al->m_role == ws_atoms_list_role_t::role_embc_cls) cls = as_al;
          if (as_al->m_role == ws_atoms_list_role_t::role_embc_qm)  qm  = as_al;
        }
    }

  if (chg || cls || qm) return;

  uc->m_is_visible = false;

  auto ws_chg = std::make_shared<ws_atoms_list_t>();
  auto ws_cls = std::make_shared<ws_atoms_list_t>();
  auto ws_qm = std::make_shared<ws_atoms_list_t>();

  chg = ws_chg.get();
  chg->m_tws_tr->do_action(act_lock);
  chg->m_role = ws_atoms_list_role_t::role_embc_chg;
  chg->m_name = fmt::format("{}_chg", uc->m_name);
  chg->m_draw_bonds = false;
  uc->m_parent_ws->add_item_to_ws(ws_chg);

  cls = ws_cls.get();
  cls->m_tws_tr->do_action(act_lock);
  cls->m_role = ws_atoms_list_role_t::role_embc_cls;
  cls->m_name = fmt::format("{}_cls", uc->m_name);
  uc->m_parent_ws->add_item_to_ws(ws_cls);

  qm = ws_qm.get();
  qm->m_tws_tr->do_action(act_lock);
  qm->m_name = fmt::format("{}_qm", uc->m_name);
  qm->m_role = ws_atoms_list_role_t::role_embc_qm;
  uc->m_parent_ws->add_item_to_ws(ws_qm);

  shape_sphere<float> sp(cluster_r, displ);
  shape_sphere<float> sp_cls(cls_r, displ);
  shape_sphere<float> sp_qm(qm_r, displ);
  shape<float> & sh_chg = sp - sp_cls;
  shape<float> & sh_cls_w_qm = sp_cls - sp_qm;

  xgeometry<float, periodic_cell<float> > gd_uc(3);
  gd_uc.set_format({"charge"},{type_real});
  gd_uc.DIM = 3;
  gd_uc.cell.DIM = 3;
  gd_uc.cell.v[0] = uc->m_geom->cell.v[0];
  gd_uc.cell.v[1] = uc->m_geom->cell.v[1];
  gd_uc.cell.v[2] = uc->m_geom->cell.v[2];

  for (int i = 0 ; i < uc->m_geom->nat(); i++) {
      gd_uc.add(uc->m_geom->atom(i), uc->m_geom->pos(i) + displ);
      gd_uc.xfield<float>(4, i) = uc->m_geom->xfield<float>(4, i);
    }

  xgeometry<float, periodic_cell<float> > gd_cls(0);
  xgeometry<float, periodic_cell<float> > gd_chg(0);
  xgeometry<float, periodic_cell<float> > gd_qm(0);

  xgeometry<float, periodic_cell<float> > g_all_m(0);
  xgeometry<float, periodic_cell<float> > g_all_e(0);

  gd_cls.set_format({"charge"},{type_real});
  gd_cls.additive(4) = true;

  gd_chg.set_format({"charge"},{type_real});
  gd_chg.additive(4) = true;

  gd_qm.set_format({"charge"},{type_real});
  gd_qm.additive(4) = true;


  qpp::fill(g_all_m, gd_uc, sp, crowd_merge | fill_cells);
  qpp::fill(g_all_e, gd_uc, sp, crowd_exclude | fill_cells);

  //translate intermediates to zero
//  for (int i = 0 ; i < g_all_m.nat(); i++) g_all_m.coord(i) -= displ;
//  for (int i = 0 ; i < g_all_e.nat(); i++) g_all_e.coord(i) -= displ;

  //performing charge addition
  tws_tree_t<float, periodic_cell<float> > sum_tree(g_all_m);
  sum_tree.do_action(act_unlock | act_rebuild_tree);
  const float equality_dist = 0.01f;

  for (int i = 0; i < g_all_e.nat(); i++) {
      std::vector<tws_node_content_t<float> > res;
      sum_tree.query_sphere(equality_dist, g_all_e.pos(i), res);
      float accum_chg = 0;
      for (auto &elem : res) accum_chg += g_all_m.xfield<float>(4, elem.m_atm);
      gd_chg.add(g_all_e.atom(i), g_all_e.pos(i));
      gd_chg.xfield<float>(4, i) = accum_chg ;
    }

  if (generate_qm) {
    //  qpp::fill(gd_cls, *uc->m_geom, sh_cls_w_qm, crowd_merge | fill_atoms);
    //  qpp::fill(gd_qm, *uc->m_geom, sp_qm, crowd_merge | fill_cells);
    }
  else {
    //  qpp::fill(gd_cls, *uc->m_geom, sp_cls, crowd_merge | fill_cells);
    }

  //std::cout << "DEBUG UC " << std::endl;

  chg->copy_from_xgeometry(gd_chg);
  //cls->copy_from_xgeometry(gd_cls);

  if (generate_qm) {
      qm->copy_from_xgeometry(gd_qm);
      if (qm->m_geom->nat() > 0) qm->m_tws_tr->do_action(act_unlock | act_rebuild_all);
    }

  if (chg->m_geom->nat() > 0) chg->m_tws_tr->do_action(act_unlock | act_rebuild_all);
  if (cls->m_geom->nat() > 0) cls->m_tws_tr->do_action(act_unlock | act_rebuild_all);

  //if (generate_qm && )

  if (chg->m_geom->nat() > 1800) chg->m_cur_render_type = ws_atoms_list_render_t::xatom_lines;
  //qm->m_tws_tr->do_action(act_unlock | act_rebuild_all);

  //add connection info
  ws_chg->add_connected_item(ws_cls);
  ws_chg->add_connected_item(ws_qm);
  ws_cls->add_connected_item(ws_chg);
  ws_cls->add_connected_item(ws_qm);
  ws_qm->add_connected_item(ws_chg);
  ws_qm->add_connected_item(ws_cls);

  app_state_t *astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_changed();
}

//embc.gen_sph(pq.vector3f(0,0,0), 16, 9)
void embedded_cluster_tools::gen_spherical_cluster_cur(vector3<float> displ,
                                                       float cluster_r,
                                                       float cls_r) {
  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) gen_spherical_cluster(cur_it_al, displ, cluster_r, cls_r);
        }
    }
}

void embedded_cluster_tools::gen_spherical_cluster_cur_qm(vector3<float> displ,
                                                          float cluster_r,
                                                          float cls_r,
                                                          float qm_r) {
  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) gen_spherical_cluster(cur_it_al, displ, cluster_r, cls_r, true, qm_r);
        }
    }
}

vector3<float> embedded_cluster_tools::calc_dipole_moment() {

  app_state_t *astate = app_state_t::get_inst();

  vector3<float> accum_dm{0,0,0};

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              for (int i = 0; i < cur_it_al->m_geom->nat(); i++)
                accum_dm += cur_it_al->m_geom->pos(i) *
                            cur_it_al->m_geom->xfield<float>(4, i);
            }
        }
    }

  return accum_dm;

}
