#include <qppcad/embedded_cluster_tools.hpp>
#include <geom/builders.hpp>
#include <geom/shape.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void embedded_cluster_tools::find_high_symmetry_qm_cluster(geom_view_t *uc,
                                                           vector3<float> sphere_center,
                                                           float qm_r_start_scan,
                                                           float qm_r_end_scan,
                                                           float symm_tolerance,
                                                           size_t total_steps) {

  if (qm_r_start_scan > qm_r_end_scan) {
      throw std::runtime_error("qm_r_start_scan > qm_r_end_scan!");
      return;
    }

  if (!uc) {
      throw std::runtime_error("uc == nullptr");
      return;
    }

  float dr = (qm_r_end_scan - qm_r_start_scan) / float(total_steps);

  for (size_t i = 0; i < total_steps; i++) {

      xgeometry<float, periodic_cell<float> > g_all_m(0);
      g_all_m.set_format({"charge"},{type_real});
      g_all_m.additive(4) = true;
      g_all_m.tol_geom = 0.01f;

      float cluster_r = qm_r_start_scan + dr * i;
      shape_sphere<float> sp(cluster_r, sphere_center);
      qpp::fill(g_all_m, *uc->m_geom, sp, crowd_ignore | fill_cells);

      py::print(fmt::format("find_hs: i={} r={} nat={}", i, cluster_r, g_all_m.nat()));

    }

}

void embedded_cluster_tools::gen_spherical_cluster(geom_view_t *uc,
                                                   vector3<float> displ,
                                                   float cluster_r,
                                                   float cls_r,
                                                   bool generate_qm ,
                                                   float qm_r,
                                                   bool do_legacy) {

    if (!uc || uc->m_role != geom_view_role_e::r_uc) {
        //throw py::error_already_set();
        throw std::runtime_error("uc || uc->m_role != geom_view_role_t::role_uc");
        return;
    }

    if (cls_r > cluster_r) {
        //throw py::error_already_set();
        throw std::runtime_error("cls_r > cluster_r");
        return;
    }

    //try to find charges, classic and quantum geom_view`s

    std::shared_ptr<geom_view_t> ws_chg{nullptr};
    std::shared_ptr<geom_view_t> ws_cls{nullptr};
    std::shared_ptr<geom_view_t> ws_qm{nullptr};

    for (auto elem : uc->m_connected_items) {
        std::shared_ptr<geom_view_t> as_al = std::dynamic_pointer_cast<geom_view_t>(elem);
        if (as_al) {
            if (as_al->m_role == geom_view_role_e::r_embc_chg) ws_chg = as_al;
            if (as_al->m_role == geom_view_role_e::r_embc_cls) ws_cls = as_al;
            if (as_al->m_role == geom_view_role_e::r_embc_qm)  ws_qm  = as_al;
        }
    }

    uc->m_is_visible = false;

    if (ws_chg == nullptr) {
        ws_chg = std::make_shared<geom_view_t>();
        uc->m_parent_ws->add_item_to_ws(ws_chg);
    } else {
        ws_chg->m_tws_tr->do_action(act_lock);
        ws_chg->m_tws_tr->do_action(act_clear_all);
        ws_chg->m_geom->clear();
    }

    ws_chg->m_tws_tr->do_action(act_lock);
    ws_chg->m_role = geom_view_role_e::r_embc_chg;
    ws_chg->m_name = fmt::format("{}_chg", uc->m_name);
    ws_chg->m_draw_bonds = false;

    if (ws_cls == nullptr) {
        ws_cls = std::make_shared<geom_view_t>();
        uc->m_parent_ws->add_item_to_ws(ws_cls);
    } else {
        ws_cls->m_tws_tr->do_action(act_lock);
        ws_cls->m_tws_tr->do_action(act_clear_all);
        ws_cls->m_geom->clear();
    }

    ws_cls->m_tws_tr->do_action(act_lock);
    ws_cls->m_role = geom_view_role_e::r_embc_cls;
    ws_cls->m_name = fmt::format("{}_cls", uc->m_name);

    if (ws_qm == nullptr) {
        ws_qm = std::make_shared<geom_view_t>();
        uc->m_parent_ws->add_item_to_ws(ws_qm);
    } else {
        ws_qm->m_tws_tr->do_action(act_lock);
        ws_qm->m_tws_tr->do_action(act_clear_all);
        ws_qm->m_geom->clear();
    }

    ws_qm->m_tws_tr->do_action(act_lock);
    ws_qm->m_name = fmt::format("{}_qm", uc->m_name);
    ws_qm->m_role = geom_view_role_e::r_embc_qm;

    shape_sphere<float> sp(cluster_r, displ);

    //copy uc to intermediate
    xgeometry<float, periodic_cell<float> > gd_uc(3);
    gd_uc.set_format({"charge"},{type_real});
    gd_uc.DIM = 3;
    gd_uc.cell.DIM = 3;
    gd_uc.cell.v[0] = uc->m_geom->cell.v[0];
    gd_uc.cell.v[1] = uc->m_geom->cell.v[1];
    gd_uc.cell.v[2] = uc->m_geom->cell.v[2];

    for (int i = 0 ; i < uc->m_geom->nat(); i++) {
        gd_uc.add(uc->m_geom->atom(i), uc->m_geom->pos(i));
        gd_uc.xfield<float>(4, i) = uc->m_geom->xfield<float>(4, i);
    }

    //initialize result xgeometries
    xgeometry<float, periodic_cell<float> > gd_chg(0);

    //initialize intermidiates for charge counting
    xgeometry<float, periodic_cell<float> > g_all_m(0);

    g_all_m.set_format({"charge"},{type_real});
    g_all_m.additive(4) = true;
    g_all_m.tol_geom = 0.01f;

    gd_chg.set_format({"charge"},{type_real});
    gd_chg.additive(4) = true;
    gd_chg.tol_geom = 0.01f;

    int mode_m = crowd_ignore | fill_cells;

    if (do_legacy) {
        mode_m = mode_m | legacy_fill;
    }

    qpp::fill(g_all_m, gd_uc, sp, mode_m);

    //translate intermediates to zero
    for (int i = 0 ; i < g_all_m.nat(); i++) g_all_m.coord(i) -= displ;

    //performing charge addition
    tws_tree_t<float, periodic_cell<float> > sum_tree(g_all_m);
    sum_tree.do_action(act_unlock | act_rebuild_tree);

    const float equality_dist = 0.01f;

    for (int i = 0; i < g_all_m.nat(); i++) {

        std::vector<tws_node_content_t<float> > res;
        sum_tree.query_sphere(equality_dist, g_all_m.pos(i), res);
        float accum_chg = 0;

        std::set<int> num_occur;

        for (auto &elem : res) {
            accum_chg += g_all_m.xfield<float>(4, elem.m_atm);
            num_occur.insert(elem.m_atm);
        }

        if (!num_occur.empty() && i == *num_occur.begin()) {
            gd_chg.add(g_all_m.atom(i), g_all_m.pos(i));
            gd_chg.xfield<float>(4, gd_chg.nat()-1) = accum_chg;
        }

    }

    //time to assign atoms to clusters
    auto add_atom_to_xgeom = [](xgeometry<float, periodic_cell<float> > &g1,
            xgeometry<float, periodic_cell<float> > &g2, int atom_id ) {
        g2.add(g1.atom(atom_id), g1.pos(atom_id));
        g2.xfield<float>(xgeom_charge, g2.nat()-1) = g1.xfield<float>(xgeom_charge, atom_id);
    };

    for (int i = 0; i < gd_chg.nat(); i++) {

        float r = gd_chg.pos(i).norm();

        if (r >= cls_r) add_atom_to_xgeom(gd_chg, *ws_chg->m_geom, i);

        if (generate_qm) {
            if (r > -0.01f && r < qm_r) add_atom_to_xgeom(gd_chg, *ws_qm->m_geom, i);
            if (r > qm_r + 0.01f && r < cls_r) add_atom_to_xgeom(gd_chg, *ws_cls->m_geom, i);
        } else {
            if (r > -0.01f && r < cls_r) add_atom_to_xgeom(gd_chg, *ws_cls->m_geom, i);
        }
    }


    if (generate_qm) if (ws_qm->m_geom->nat() > 0)
        ws_qm->m_tws_tr->do_action(act_unlock | act_rebuild_all);

    if (ws_chg->m_geom->nat() > 0) ws_chg->m_tws_tr->do_action(act_unlock | act_rebuild_all);
    if (ws_cls->m_geom->nat() > 0) ws_cls->m_tws_tr->do_action(act_unlock | act_rebuild_all);

    //if (generate_qm && )

    if (ws_chg->m_geom->nat() > 1800)
      ws_chg->m_render_style = geom_view_render_style_e::xatom_lines;
    //qm->m_tws_tr->do_action(act_unlock | act_rebuild_all);

    //add connection info
    ws_chg->add_connected_item(ws_cls);
    ws_chg->add_connected_item(ws_qm);
    ws_chg->add_connected_item(uc->shared_from_this());

    ws_cls->add_connected_item(ws_chg);
    ws_cls->add_connected_item(ws_qm);
    ws_cls->add_connected_item(uc->shared_from_this());

    ws_qm->add_connected_item(ws_chg);
    ws_qm->add_connected_item(ws_cls);
    ws_qm->add_connected_item(uc->shared_from_this());

    uc->add_connected_item(ws_cls);
    uc->add_connected_item(ws_qm);
    uc->add_connected_item(ws_chg);

    app_state_t *astate = app_state_t::get_inst();
    astate->astate_evd->cur_ws_changed();

    //time to print dipole moments

    //auto dm_chg = embedded_cluster_tools::calc_dipole_moment();

}

//embc.gen_sph(pq.vector3f(0,0,0), 16, 9)
void embedded_cluster_tools::gen_spherical_cluster_cur(vector3<float> displ,
                                                       float cluster_r,
                                                       float cls_r,
                                                       bool do_legacy) {
    app_state_t *astate = app_state_t::get_inst();

    auto [cur_ws, cur_it, as_al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>();

    if (!ok) {
      return;
    }

    gen_spherical_cluster(as_al, displ, cluster_r, cls_r, do_legacy);

}

void embedded_cluster_tools::gen_spherical_cluster_cur_qm(vector3<float> displ,
                                                          float cluster_r,
                                                          float cls_r,
                                                          float qm_r,
                                                          bool do_legacy) {
    app_state_t *astate = app_state_t::get_inst();

    auto [cur_ws, cur_it, as_al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>();

    if (!ok) {
      return;
    }


    bool succes{false};

    if (as_al && as_al->m_role == geom_view_role_e::r_uc) {
        succes = true;
        gen_spherical_cluster(as_al, displ, cluster_r, cls_r, true, qm_r, do_legacy);
        return;
    }

    //try to deduce uc from connected items
    if (as_al && (as_al->m_role == geom_view_role_e::r_embc_qm ||
                  as_al->m_role == geom_view_role_e::r_embc_chg ||
                  as_al->m_role == geom_view_role_e::r_embc_cls ))

        for (auto elem : as_al->m_connected_items) {
            auto con_al = elem->cast_as<geom_view_t>();
            if (con_al && con_al->m_role == geom_view_role_e::r_uc) {
                succes = true;
                gen_spherical_cluster(con_al, displ, cluster_r, cls_r,
                                      true, qm_r, do_legacy);
                return;
            }
        }

    if (!succes) {
        throw std::runtime_error("Cannot deduce context for embedded cluster generation");
        return;
    }

}

void embedded_cluster_tools::set_qm_cluster_r(std::shared_ptr<geom_view_t> qm,
                                              std::shared_ptr<geom_view_t> cls,
                                              float new_r) {

    if (!qm || !cls) {
        throw std::runtime_error("!qm || !cls");
        return;
    }

    //phase 1 : move atoms from cls to qm

    std::vector<tws_node_content_t<float> > redu_cls;

    cls->m_tws_tr->query_sphere(new_r, vector3<float>(0), redu_cls);
    std::set<int> redu_cls_set; //set for fast search

    for (auto &elem : redu_cls) redu_cls_set.insert(elem.m_atm);

    for (int i = 0; i < cls->m_geom->nat(); i++)
        if (redu_cls_set.find(i) != redu_cls_set.end())
            qm->ins_atom(cls->m_geom->atom(i), cls->m_geom->pos(i));

    //delete atoms from cls
    cls->delete_atoms(redu_cls_set);

    //phase 2 : move atoms from qm to cls
    std::vector<tws_node_content_t<float> > redu_qm;
    std::set<int> redu_qm_inside, redu_qm_outside;

    //construct direct set - atoms inside sphere with r = new_r
    qm->m_tws_tr->query_sphere(new_r, vector3<float>(0), redu_qm);
    for (auto &elem : redu_qm) redu_qm_inside.insert(elem.m_atm);

    // redu_qm_outside = set(ALL) - set(INSIDE)
    for (int i = 0; i < qm->m_geom->nat(); i++)
        if (redu_qm_inside.find(i) == redu_qm_inside.end()) {
            cls->ins_atom(qm->m_geom->atom(i), qm->m_geom->pos(i));
            redu_qm_outside.insert(i);
        }

    qm->delete_atoms(redu_qm_outside);

}

void embedded_cluster_tools::move_sel_from_qm_to_cls(std::shared_ptr<geom_view_t> qm,
                                                     std::shared_ptr<geom_view_t> cls) {
    if ( !cls || !qm) {
        throw std::runtime_error("!chg || !cls || !qm");
        return;
    } else {
        for (auto &elem : qm->m_atom_idx_sel)
            cls->ins_atom(qm->m_geom->atom(elem.m_atm), qm->m_geom->pos(elem.m_atm));
        qm->delete_selected_atoms();
    }
}

void embedded_cluster_tools::move_sel_from_qm_to_cls_cur() {

    app_state_t *astate = app_state_t::get_inst();

    if (astate->ws_mgr->has_wss()) {

        auto cur_ws = astate->ws_mgr->get_cur_ws();

        if (cur_ws) {
            auto cur_it_al = std::static_pointer_cast<geom_view_t>(cur_ws->get_selected_sp());

            std::shared_ptr<geom_view_t> uc{nullptr};
            std::shared_ptr<geom_view_t> chg{nullptr};
            std::shared_ptr<geom_view_t> cls{nullptr};
            std::shared_ptr<geom_view_t> qm{nullptr};

            deduce_embedding_context(uc, chg, cls, qm);

            if (!chg || !cls || !qm) {
                throw std::runtime_error("!chg || !cls || !qm");
                return;
            } else {
                move_sel_from_qm_to_cls(qm, cls);
            }
        }
    }
}

void embedded_cluster_tools::set_qm_cluster_r_cur(float new_r) {

    app_state_t *astate = app_state_t::get_inst();

    if (astate->ws_mgr->has_wss()) {

        auto cur_ws = astate->ws_mgr->get_cur_ws();

        if (cur_ws) {
            auto cur_it_al = std::static_pointer_cast<geom_view_t>(cur_ws->get_selected_sp());

            std::shared_ptr<geom_view_t> uc{nullptr};
            std::shared_ptr<geom_view_t> chg{nullptr};
            std::shared_ptr<geom_view_t> cls{nullptr};
            std::shared_ptr<geom_view_t> qm{nullptr};

            deduce_embedding_context(uc, chg, cls, qm);

            if (!chg || !cls || !qm) {
                throw std::runtime_error("!chg || !cls || !qm");
                return;
            } else {
                set_qm_cluster_r(qm, cls, new_r);
            }
        }
    }

}

void embedded_cluster_tools::deduce_embedding_context(std::shared_ptr<geom_view_t> &uc,
                                                      std::shared_ptr<geom_view_t> &chg,
                                                      std::shared_ptr<geom_view_t> &cls,
                                                      std::shared_ptr<geom_view_t> &qm) {
    app_state_t *astate = app_state_t::get_inst();

    if (astate->ws_mgr->has_wss()) {

        auto cur_ws = astate->ws_mgr->get_cur_ws();

        if (cur_ws) {

            auto cur_it_al = std::static_pointer_cast<geom_view_t>(cur_ws->get_selected_sp());

            if (cur_it_al) {

                if (cur_it_al->m_role == geom_view_role_e::r_embc_qm) qm = cur_it_al;
                if (cur_it_al->m_role == geom_view_role_e::r_embc_chg) chg = cur_it_al;
                if (cur_it_al->m_role == geom_view_role_e::r_embc_cls) cls = cur_it_al;
                if (cur_it_al->m_role == geom_view_role_e::r_uc) uc = cur_it_al;

                for (auto elem : cur_it_al->m_connected_items) {
                    auto elem_al = std::static_pointer_cast<geom_view_t>(elem);
                    if (elem_al && elem_al->m_role == geom_view_role_e::r_embc_qm)
                      qm = elem_al;
                    if (elem_al && elem_al->m_role == geom_view_role_e::r_embc_chg)
                      chg = elem_al;
                    if (elem_al && elem_al->m_role == geom_view_role_e::r_embc_cls)
                      cls = elem_al;
                    if (elem_al && elem_al->m_role == geom_view_role_e::r_uc) uc = elem_al;
                }

            }

        }

    }

}

vector3<float> embedded_cluster_tools::calc_dipole_moment() {

    app_state_t *astate = app_state_t::get_inst();

    vector3<float> accum_dm{0,0,0};

    if (astate->ws_mgr->has_wss()) {

        auto cur_ws = astate->ws_mgr->get_cur_ws();

        if (cur_ws) {
            auto cur_it_al = dynamic_cast<geom_view_t*>(cur_ws->get_selected());
            if (cur_it_al) {
                for (int i = 0; i < cur_it_al->m_geom->nat(); i++)
                    accum_dm += cur_it_al->m_geom->pos(i) *
                            cur_it_al->m_geom->xfield<float>(4, i);
            }
        }
    }

    return accum_dm;

}

void embedded_cluster_tools::generate_molcas_embc_sp_input(std::string outdir) {

    app_state_t *astate = app_state_t::get_inst();

    if (!astate->ws_mgr->has_wss()) return;

    auto cur_ws = astate->ws_mgr->get_cur_ws();
    if (!cur_ws) return;

    std::shared_ptr<geom_view_t> uc{nullptr};
    std::shared_ptr<geom_view_t> chg{nullptr};
    std::shared_ptr<geom_view_t> cls{nullptr};
    std::shared_ptr<geom_view_t> qm{nullptr};

    deduce_embedding_context(uc, chg, cls, qm);

    if (!chg || !cls || !qm) return;

    std::ofstream embc_inp(fmt::format("{}/inp", outdir));
    std::ofstream embc_chg(fmt::format("{}/embc_chg", outdir));

    //generate main input file
    fmt::print(embc_inp, "&GATEWAY\n");

    //printing qm atoms
    for (int i = 0; i < qm->m_geom->n_types(); i++) {
        fmt::print(embc_inp, "Basis set\n");
        fmt::print(embc_inp, "{}.ANO-RCC-VDZP.\n", qm->m_geom->atom_of_type(i));
        int local_type_c = 0;
        for (int q = 0; q < qm->m_geom->nat(); q++)
            if (qm->m_geom->type_table(q) == i) {
                local_type_c +=1 ;
                fmt::print(
                           embc_inp, "{}{} {} {} {}\n",
                           qm->m_geom->atom_of_type(i),
                           local_type_c,
                           qm->m_geom->pos(q)[0],
                           qm->m_geom->pos(q)[1],
                           qm->m_geom->pos(q)[2]
                         );
            }
        fmt::print(embc_inp, "End Of Basis\n");
    }

    //printing mm atoms
    std::array<std::string, 7> map_type_to_sn = {"X", "Y", "L", "J", "M", "T", "E"};

    for (int i = 0; i < cls->m_geom->n_types(); i++) {
        fmt::print(embc_inp, "Basis set\n");
        fmt::print(embc_inp, "{}.ECP.Pascual.0s.0s.0e-AIMP-CaF2.\n", cls->m_geom->atom_of_type(i));
        int local_type_c = 0;
        for (int q = 0; q < cls->m_geom->nat(); q++)
            if (cls->m_geom->type_table(q) == i) {
                local_type_c +=1 ;
                fmt::print(
                            embc_inp, "{}{} {} {} {}\n",
                            map_type_to_sn[i],
                            local_type_c,
                            cls->m_geom->pos(q)[0],
                        cls->m_geom->pos(q)[1],
                        cls->m_geom->pos(q)[2]
                        );
            }
        fmt::print(embc_inp, "End Of Basis\n");
    }

    //printing charges
    //x y z  c  0. 0. 0.
    for (int i = 0; i < chg->m_geom->nat(); i++)
        fmt::print(
                    embc_chg, "{} {} {} {} 0. 0. 0.\n",
                    chg->m_geom->pos(i)[0],
                chg->m_geom->pos(i)[1],
                chg->m_geom->pos(i)[2],
                chg->m_geom->xfield<float>(xgeom_charge, i)
                );

    fmt::print(embc_inp, "XField\n");
    fmt::print(embc_inp, "{}\n", chg->m_geom->nat());
    fmt::print(embc_inp, ">>INCLUDE embc_chg\n");
    fmt::print(embc_inp, "&SEWARD\n");
    fmt::print(embc_inp, "EXPERT\n");
    fmt::print(embc_inp, "AMFI\n");
    fmt::print(embc_inp, "&SCF\n");
    fmt::print(embc_inp, "CHARGE=1\n");

}

void embedded_cluster_tools::generate_orca_embc_sp_input(std::string outdir,
                                                         std::vector<std::string> anion_list,
                                                         bool merge_cls_and_chg) {

    app_state_t *astate = app_state_t::get_inst();

    if (!astate->ws_mgr->has_wss()) return;

    auto cur_ws = astate->ws_mgr->get_cur_ws();
    if (!cur_ws) return;

    std::shared_ptr<geom_view_t> uc{nullptr};
    std::shared_ptr<geom_view_t> chg{nullptr};
    std::shared_ptr<geom_view_t> cls{nullptr};
    std::shared_ptr<geom_view_t> qm{nullptr};

    deduce_embedding_context(uc, chg, cls, qm);

    if (!chg || !cls || !qm) return;

    std::ofstream embc_inp(fmt::format("{}/orca.inp", outdir));
    std::ofstream embc_pc(fmt::format("{}/pointcharges.pc", outdir));

    fmt::print(embc_inp, "#Embedded cluster single-point calculation \n");
    fmt::print(embc_inp, "! hf ano-rcc \n");
    fmt::print(embc_inp, "\n");
    fmt::print(embc_inp, "% pointcharges \"pointcharges.pc\"\n");
    fmt::print(embc_inp, "\n");
    fmt::print(embc_inp, "* xyz 0 1\n");

    //printing qm atoms N x y z
    for (int i = 0; i < qm->m_geom->nat(); i++)
        fmt::print(
                   embc_inp, "{0} {1} {2} {3}\n",
                   qm->m_geom->atom_name(i),
                   qm->m_geom->pos(i)[0],
                   qm->m_geom->pos(i)[1],
                   qm->m_geom->pos(i)[2]
                 );

    //printing cls atoms N> q x y z
    if (merge_cls_and_chg) {
        //fmt::print(embc_inp, "{}\n", cls->m_geom->nat());
        for (int i = 0; i < cls->m_geom->nat(); i++) {
            bool add_ecp = cls->m_geom->atom_name(i).find("F") == std::string::npos;
            fmt::print(
                        embc_inp,
                        add_ecp ? "{0}> {1} {2} {3} {4} NewECP \"{0}_emb\" end\n" :
                                  "{0}> {1} {2} {3} {4} \n",
                        cls->m_geom->atom_name(i),
                        cls->m_geom->xfield<float>(xgeom_charge, i),
                        cls->m_geom->pos(i)[0],
                        cls->m_geom->pos(i)[1],
                        cls->m_geom->pos(i)[2]
                    );
        }

        //printing charges
        //q x y z
        fmt::print(embc_pc, "{}\n", chg->m_geom->nat());
        for (int i = 0; i < chg->m_geom->nat(); i++)
            fmt::print(
                       embc_pc, "{} {} {} {}\n",
                       chg->m_geom->xfield<float>(xgeom_charge, i),
                       chg->m_geom->pos(i)[0],
                       chg->m_geom->pos(i)[1],
                       chg->m_geom->pos(i)[2]
                    );
    } else {
        int total_charges = cls->m_geom->nat() + chg->m_geom->nat();
        fmt::print(embc_pc, "{}\n", total_charges);
        for (int i = 0; i < cls->m_geom->nat(); i++)
            fmt::print(
                       embc_pc, "{} {} {} {}\n",
                       cls->m_geom->xfield<float>(xgeom_charge, i),
                       cls->m_geom->pos(i)[0],
                       cls->m_geom->pos(i)[1],
                       cls->m_geom->pos(i)[2]
                    );
        for (int i = 0; i < chg->m_geom->nat(); i++)
            fmt::print(
                       embc_pc, "{} {} {} {}\n",
                       chg->m_geom->xfield<float>(xgeom_charge, i),
                       chg->m_geom->pos(i)[0],
                       chg->m_geom->pos(i)[1],
                       chg->m_geom->pos(i)[2]
                    );
    }

    fmt::print(embc_inp, "end\n");

}
