#include <qppcad/tools/supercell/supercell.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void supercell_tool_t::exec(ws_item_t *item) {

  app_state_t *astate = app_state_t::get_inst();
  astate->log("Supercell tools::exec()");

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (al) {
              if (al->m_geom->DIM == 3) {
                  super_cell_widget_t scw;
                  int ret_code = scw.exec();
                  int rep_a = scw.get_replication_coeff(0);
                  int rep_b = scw.get_replication_coeff(1);
                  int rep_c = scw.get_replication_coeff(2);

                  if (ret_code == QDialog::Accepted && (rep_a + rep_b + rep_c > 3)) {
                      make_super_cell(al, rep_a, rep_b, rep_c);
                      astate->make_viewport_dirty();
                    }

                } else QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                                            QObject::tr("m_geom.DIM !=3"));

            } else QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                                        QObject::tr("ws_item.type != ws_atoms_list"));

        } else QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                                    QObject::tr("Workspace not select"));

    }

}

void supercell_tool_t::make_super_cell(ws_atoms_list_t *al,
                                       const int a_steps,
                                       const int b_steps,
                                       const int c_steps) {
  if (!al->m_parent_ws) return;
  if (al->m_geom->DIM != 3) return;

  std::shared_ptr<ws_atoms_list_t> sc_al = std::make_shared<ws_atoms_list_t>();
  sc_al->m_geom->DIM = 3;
  sc_al->m_geom->cell.DIM = 3;

  //sc_al->set_parent_workspace(parent_ws);
  sc_al->m_tws_tr->do_action(act_lock | act_clear_all);

  sc_al->m_geom->cell.v[0] = al->m_geom->cell.v[0] * (a_steps);
  sc_al->m_geom->cell.v[1] = al->m_geom->cell.v[1] * (b_steps);
  sc_al->m_geom->cell.v[2] = al->m_geom->cell.v[2] * (c_steps);

  if (al->m_role == ws_atoms_list_role_t::r_uc) {
      sc_al->m_draw_img_atoms = false;
      sc_al->m_draw_img_bonds = false;
    }

  for (auto i = 0; i < al->m_geom->nat(); i++)
    for (iterator idx_it(index::D(al->m_geom->DIM).all(0),
                         index({a_steps-1, b_steps-1, c_steps-1}));
         !idx_it.end(); idx_it++ ) {
        vector3<float> new_atom_pos = al->m_geom->pos(i, idx_it);
        sc_al->m_geom->add(al->m_geom->atom(i), new_atom_pos);
        if (al->m_role == ws_atoms_list_role_t::r_uc)
          sc_al->m_geom->xfield<float>(xgeom_charge, sc_al->m_geom->nat()-1) =
              al->m_geom->xfield<float>(xgeom_charge, i);
      }

  sc_al->m_pos = al->m_pos + al->m_geom->cell.v[0] * 1.4f;

  sc_al->m_name = al->m_name + fmt::format("_sc_{}_{}_{}", a_steps, b_steps, c_steps);

  al->m_parent_ws->add_item_to_ws(sc_al);

  sc_al->m_tws_tr->do_action(act_unlock | act_rebuild_all);
  sc_al->geometry_changed();

  //perform purification
  if (al->m_role == ws_atoms_list_role_t::r_uc) {

      sc_al->m_tws_tr->do_action(act_lock);
      //intermediage xgeom
      xgeometry<float, periodic_cell<float> > g(3);
      g.set_format({"charge"},{type_real});
      g.DIM = 3;
      g.cell.DIM = 3;
      g.cell.v[0] = sc_al->m_geom->cell.v[0];
      g.cell.v[1] = sc_al->m_geom->cell.v[1];
      g.cell.v[2] = sc_al->m_geom->cell.v[2];
      // tws_tree_t<float, periodic_cell<float> > sum_tree(g);
      const float equality_dist = 0.01f;
      for (int i = 0; i < sc_al->m_geom->nat(); i++) {
          std::vector<tws_node_content_t<float> > res;
          sc_al->m_tws_tr->query_sphere(equality_dist, sc_al->m_geom->pos(i), res);
          float accum_chg = 0;

          bool need_to_add{true};
          for (auto &elem : res)
            if (elem.m_idx == index::D(sc_al->m_geom->DIM).all(0)) {
                accum_chg +=  sc_al->m_geom->xfield<float>(xgeom_charge, elem.m_atm);
                if (i > elem.m_atm) need_to_add = false;
              }

          if (need_to_add) {
              g.add(sc_al->m_geom->atom(i), sc_al->m_geom->pos(i));
              g.xfield<float>(xgeom_charge, g.nat()-1) = accum_chg;
            }
        }

      sc_al->m_geom->clear();
      sc_al->copy_from_xgeometry(g);
      sc_al->m_tws_tr->do_action(act_unlock | act_rebuild_all);

    }

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_changed();

}

int super_cell_widget_t::get_replication_coeff(int dim_num) {

  switch (dim_num) {
    case 0:
      return sp_rep_a->value();
      break;
    case 1:
      return sp_rep_b->value();
      break;
    case 2:
      return sp_rep_c->value();
      break;
    default:
      return 0;
      break;
    }

}

super_cell_widget_t::super_cell_widget_t () : QDialog () {

  setWindowTitle(tr("Supercell generation"));
  dialog_layout = new QVBoxLayout;
  setLayout(dialog_layout);

  gb_rep_par = new QGroupBox(tr("Parameters of supercell generation"));
  gb_rep_par_layout = new QFormLayout;
  gb_rep_par->setLayout(gb_rep_par_layout);

  sp_rep_a = new QSpinBox;
  sp_rep_a->setMinimum(1);
  sp_rep_a->setMaximum(10);

  sp_rep_b = new QSpinBox;
  sp_rep_b->setMinimum(1);
  sp_rep_b->setMaximum(10);

  sp_rep_c = new QSpinBox;
  sp_rep_c->setMinimum(1);
  sp_rep_c->setMaximum(10);

  gb_rep_par_layout->addRow(tr("Replicate along a-axis"), sp_rep_a);
  gb_rep_par_layout->addRow(tr("Replicate along b-axis"), sp_rep_b);
  gb_rep_par_layout->addRow(tr("Replicate along c-axis"), sp_rep_c);

  dialog_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(dialog_bb, SIGNAL(accepted()), this, SLOT(accept()));
  connect(dialog_bb, SIGNAL(rejected()), this, SLOT(reject()));
  dialog_layout->addWidget(gb_rep_par);
  dialog_layout->addWidget(dialog_bb);

}
