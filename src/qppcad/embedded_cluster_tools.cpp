#include <qppcad/embedded_cluster_tools.hpp>
#include <geom/builders.hpp>
#include <geom/shape.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void embedded_cluster_tools::gen_spherical_cluster(ws_atoms_list_t *uc,
                                                   vector3<float> displ,
                                                   float cluster_r,
                                                   float cls_r) {

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
  chg->m_name = fmt::format("{}_chg", uc->m_name);
  uc->m_parent_ws->add_item_to_ws(ws_chg);

  cls = ws_cls.get();
  cls->m_tws_tr->do_action(act_lock);
  cls->m_name = fmt::format("{}_cls", uc->m_name);
  uc->m_parent_ws->add_item_to_ws(ws_cls);

  qm = ws_qm.get();
  qm->m_tws_tr->do_action(act_lock);
  qm->m_name = fmt::format("{}_qm", uc->m_name);
  uc->m_parent_ws->add_item_to_ws(ws_qm);

  shape_sphere<float> sp(cluster_r, displ);
  shape_sphere<float> sp_q(cls_r, displ);
  shape<float> & sh = sp - sp_q;

  xgeometry<float, periodic_cell<float> > gd_cls(0);
  xgeometry<float, periodic_cell<float> > gd_chg(0);

  qpp::fill(gd_cls, *uc->m_geom, sp_q, crowd_merge | fill_atoms);
  qpp::fill(gd_chg, *uc->m_geom, sh, crowd_merge | fill_atoms);

  std::cout << "DEBUG UC " << std::endl;

  chg->copy_from_xgeometry(gd_chg);
  cls->copy_from_xgeometry(gd_cls);

  chg->m_tws_tr->do_action(act_unlock | act_rebuild_all);
  cls->m_tws_tr->do_action(act_unlock | act_rebuild_all);

  if (chg->m_geom->nat() > 1000) chg->m_cur_render_type = ws_atoms_list_render_t::xatom_lines;
  //qm->m_tws_tr->do_action(act_unlock | act_rebuild_all);

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
