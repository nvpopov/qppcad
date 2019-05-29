#include <qppcad/tools/axial_scale/axial_scale.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void axial_scale_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t* astate = app_state_t::get_inst();
  astate->log("axial_scale_tool_t::exec()");

  auto [cur_ws, cur_it, al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>(_error_ctx);

  if (!ok) return;

  if (al->m_geom->DIM != 3) {
      QMessageBox::warning(nullptr,
                           QObject::tr("Axial scale"),
                           QObject::tr("Structure`s dimension is not equal to 3")
                           );
      return;
    }

  axial_scale_widget_t asw;
  int ret_code = asw.exec();
  double sc_a = asw.get_scale_value(0);
  double sc_b = asw.get_scale_value(1);
  double sc_c = asw.get_scale_value(2);

  if (ret_code == QDialog::Accepted) {
      apply_axial_scale(al, float(sc_a), float(sc_b), float(sc_c));
      astate->make_viewport_dirty();
    }

}

void axial_scale_tool_t::apply_axial_scale(geom_view_t *al,
                                           const float scale_a,
                                           const float scale_b,
                                           const float scale_c) {
  if (al->m_geom->DIM != 3) return;
  al->begin_structure_change();

  periodic_cell<float> new_cell(3);
  new_cell.v[0] = al->m_geom->cell.v[0] * scale_a;
  new_cell.v[1] = al->m_geom->cell.v[1] * scale_b;
  new_cell.v[2] = al->m_geom->cell.v[2] * scale_c;

  for (auto i = 0; i < al->m_geom->nat(); i++) {
      vector3<float> frac_in_old_cell = al->m_geom->cell.cart2frac(al->m_geom->pos(i));
      al->m_geom->change_pos(i, new_cell.frac2cart(frac_in_old_cell));
    }

  al->m_geom->cell.v[0] = new_cell.v[0];
  al->m_geom->cell.v[1] = new_cell.v[1];
  al->m_geom->cell.v[2] = new_cell.v[2];

  al->end_structure_change();

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_selected_atoms_list_cell_changed();

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

  auto make_spinbox = [](){
      auto ret = new QDoubleSpinBox;
      ret->setMinimum(0.001);
      ret->setMaximum(5.00);
      ret->setValue(1.0);
      ret->setSingleStep(0.00001);
      ret->setDecimals(10);
      ret->setAlignment(Qt::AlignCenter);
      ret->setButtonSymbols(QAbstractSpinBox::NoButtons);
      ret->setLocale(QLocale::C);
      return ret;
    };

  sb_sc_a = make_spinbox();
  sb_sc_b = make_spinbox();
  sb_sc_c = make_spinbox();

  gb_sc_par_layout->addRow(tr("Scale a-axis"), sb_sc_a);
  gb_sc_par_layout->addRow(tr("Scale b-axis"), sb_sc_b);
  gb_sc_par_layout->addRow(tr("Scale c-axis"), sb_sc_c);

  dialog_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(dialog_bb,
          &QDialogButtonBox::accepted,
          this,
          &axial_scale_widget_t::accept);

  connect(dialog_bb,
          &QDialogButtonBox::rejected,
          this,
          &axial_scale_widget_t::reject);

  dialog_layout->addWidget(gb_sc_par);
  dialog_layout->addWidget(dialog_bb);

}
