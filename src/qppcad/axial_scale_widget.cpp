#include <qppcad/axial_scale_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

double axial_scale_widget_t::get_scale_value(int dim_id) {

  switch (dim_id) {
    case 0:
      return sb_sc_a->value();
      break;
    case 1:
      return sb_sc_b->value();
      break;
    case 2:
      return sb_sc_c->value();
      break;
    default:
      return 1.0;
      break;
    }

}

axial_scale_widget_t::axial_scale_widget_t() : QDialog () {

  setWindowTitle("Axial scale");
  dialog_layout = new QVBoxLayout;
  setLayout(dialog_layout);

  gb_sc_par = new QGroupBox(tr("Parameters of axial scaling"));
  gb_sc_par_layout = new QFormLayout;
  gb_sc_par->setLayout(gb_sc_par_layout);

  sb_sc_a = new QDoubleSpinBox;
  sb_sc_a->setMinimum(0.01);
  sb_sc_a->setMaximum(2.00);
  sb_sc_a->setValue(1.0);
  sb_sc_a->setSingleStep(0.01);

  sb_sc_b= new QDoubleSpinBox;
  sb_sc_b->setMinimum(0.01);
  sb_sc_b->setMaximum(2.00);
  sb_sc_b->setValue(1.0);
  sb_sc_b->setSingleStep(0.01);

  sb_sc_c = new QDoubleSpinBox;
  sb_sc_c->setMinimum(0.01);
  sb_sc_c->setMaximum(2.00);
  sb_sc_c->setValue(1.0);
  sb_sc_c->setSingleStep(0.01);

  gb_sc_par_layout->addRow(tr("Scale a-axis"), sb_sc_a);
  gb_sc_par_layout->addRow(tr("Scale b-axis"), sb_sc_b);
  gb_sc_par_layout->addRow(tr("Scale c-axis"), sb_sc_c);

  dialog_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(dialog_bb, SIGNAL(accepted()), this, SLOT(accept()));
  connect(dialog_bb, SIGNAL(rejected()), this, SLOT(reject()));
  dialog_layout->addWidget(gb_sc_par);
  dialog_layout->addWidget(dialog_bb);
}
