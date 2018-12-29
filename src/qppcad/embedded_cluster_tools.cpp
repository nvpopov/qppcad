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

  xgeometry<float, periodic_cell<float> > gd_cls(0);
  xgeometry<float, periodic_cell<float> > gd_chg(0);
  xgeometry<float, periodic_cell<float> > gd_qm(0);

  if (generate_qm) {
      qpp::fill(gd_cls, *uc->m_geom, sh_cls_w_qm, crowd_merge);
      qpp::fill(gd_qm, *uc->m_geom, sp_qm, crowd_merge);
    }
  else {
      qpp::fill(gd_cls, *uc->m_geom, sp_cls, crowd_merge);
    }

  qpp::fill(gd_chg, *uc->m_geom, sh_chg, crowd_merge);

  //std::cout << "DEBUG UC " << std::endl;

  chg->copy_from_xgeometry(gd_chg);
  cls->copy_from_xgeometry(gd_cls);

  if (generate_qm) {
      qm->copy_from_xgeometry(gd_qm);
      if (qm->m_geom->nat() > 0) qm->m_tws_tr->do_action(act_unlock | act_rebuild_all);
    }

  chg->m_tws_tr->do_action(act_unlock | act_rebuild_all);
  cls->m_tws_tr->do_action(act_unlock | act_rebuild_all);

  //if (generate_qm && )

  if (chg->m_geom->nat() > 1000) chg->m_cur_render_type = ws_atoms_list_render_t::xatom_lines;
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
