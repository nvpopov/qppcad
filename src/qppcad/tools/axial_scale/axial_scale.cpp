#include <qppcad/tools/axial_scale/axial_scale.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void axial_scale_tool_t::exec(ws_item_t *item) {
  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (al) {
              if (al->m_geom->DIM == 3) {
                  axial_scale_widget_t asw;
                  int ret_code = asw.exec();
                  double sc_a = asw.get_scale_value(0);
                  double sc_b = asw.get_scale_value(1);
                  double sc_c = asw.get_scale_value(2);

                  if (ret_code == QDialog::Accepted) {
                      //  al->make_super_cell(rep_a + 1, rep_b + 1, rep_c + 1);
                      al->apply_axial_scale(float(sc_a), float(sc_b), float(sc_c));
                      astate->make_viewport_dirty();
                    }
                } else QMessageBox::warning(nullptr,
                                            QObject::tr("Axial scale"),
                                            QObject::tr("m_geom.DIM !=3"));
            }
          else { // is not an atoms list
              QMessageBox::warning(nullptr,
                                   QObject::tr("Axial scale"),
                                   QObject::tr("ws_item.type != ws_atoms_list"));
            }

        } else {
          QMessageBox::warning(nullptr,
                               QObject::tr("Axial scale"),
                               QObject::tr("Workspace not select"));
        }
    }
}

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
