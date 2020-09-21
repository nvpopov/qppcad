#include <qppcad/tools/axial_scale/axial_scale.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

void axial_scale_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t* astate = app_state_t::get_inst();
  astate->tlog("axial_scale_tool_t::exec()");

  auto [cur_ws, cur_it, al, ok] = astate->ws_mgr->get_sel_tpl_itmc<geom_view_t>(_error_ctx);

  if (!ok) return;

  if (al->m_geom->get_DIM() != 3) {
    QMessageBox::warning(nullptr,
                         QObject::tr("Axial scale"),
                         QObject::tr("Structure's dimension is not equals to 3"));
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
  if (al->m_geom->get_DIM() != 3)
    return;

  std::array<vector3<float>, 3> new_cell;
  new_cell[0] = al->m_geom->cell.v[0] * scale_a;
  new_cell[1] = al->m_geom->cell.v[1] * scale_b;
  new_cell[2] = al->m_geom->cell.v[2] * scale_c;

  al->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
  geom_view_tools_t::change_cell_keep_atoms(al, new_cell[0], new_cell[1], new_cell[2]);
  al->end_recording();

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_selected_atoms_list_cell_changed();

}

double axial_scale_widget_t::get_scale_value(int dim_id) {
  switch (dim_id) {
  case 0:
    return m_sb_sc_a->value();
    break;
  case 1:
    return m_sb_sc_b->value();
    break;
  case 2:
    return m_sb_sc_c->value();
    break;
  default:
    return 1.0;
    break;
  }
}

axial_scale_widget_t::axial_scale_widget_t() : QDialog () {

  app_state_t *astate = app_state_t::get_inst();

  setWindowTitle("Axial scale");
  m_dialog_lt = new QVBoxLayout;
  setLayout(m_dialog_lt);

  m_gb_sc_par = new qspoiler_widget_t(tr("Parameters of axial scaling"), nullptr, false, 6, 300);
  m_gb_sc_par_lt = new QFormLayout;
  m_gb_sc_par->add_content_layout(m_gb_sc_par_lt);

  setFixedWidth(320);
  setFixedHeight(212);

  auto make_spinbox = [astate]() {
    auto ret = new QDoubleSpinBox;
    ret->setMinimum(0.001);
    ret->setMaximum(5.00);
    ret->setValue(1.0);
    ret->setSingleStep(0.00001);
    ret->setDecimals(8);
    ret->setAlignment(Qt::AlignCenter);
    ret->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ret->setLocale(QLocale::C);
    ret->setFixedWidth(astate->size_guide.obj_insp_list_wdgt_h());
    return ret;
  };

  m_sb_sc_a = make_spinbox();
  m_sb_sc_b = make_spinbox();
  m_sb_sc_c = make_spinbox();

  m_gb_sc_par_lt->addRow(tr("Scale a-axis"), m_sb_sc_a);
  m_gb_sc_par_lt->addRow(tr("Scale b-axis"), m_sb_sc_b);
  m_gb_sc_par_lt->addRow(tr("Scale c-axis"), m_sb_sc_c);

  qt_hlp::resize_form_lt_lbls(m_gb_sc_par_lt, 130);

  m_dialog_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(m_dialog_bb,
          &QDialogButtonBox::accepted,
          this,
          &axial_scale_widget_t::accept);

  connect(m_dialog_bb,
          &QDialogButtonBox::rejected,
          this,
          &axial_scale_widget_t::reject);

  m_dialog_lt->addWidget(m_gb_sc_par);
  m_dialog_lt->addStretch(1);
  m_dialog_lt->addWidget(m_dialog_bb);

}

bool axial_scale_can_apply_helper_t::can_apply(ws_item_t *item) {

  if (!item)
    return false;

  auto as_gv = item->cast_as<geom_view_t>();
  if (!as_gv)
    return false;

  return as_gv->m_geom->get_DIM() == 3;

}
