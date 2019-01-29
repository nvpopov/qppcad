#include <qppcad/tools/supercell/supercell.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>

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
                      al->make_super_cell(rep_a, rep_b, rep_c);
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
