#include <qppcad/super_cell_widget.hpp>

using namespace qpp::cad;

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
  sp_rep_a->setMinimum(0);
  sp_rep_a->setMaximum(10);

  sp_rep_b = new QSpinBox;
  sp_rep_b->setMinimum(0);
  sp_rep_b->setMaximum(10);

  sp_rep_c = new QSpinBox;
  sp_rep_c->setMinimum(0);
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
