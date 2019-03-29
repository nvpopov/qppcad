#include <qppcad/tools/colorize_by_dist/colorize_by_dist.hpp>
#include <qppcad/ws_item/geom_view/geom_view_colorizer.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

void colorize_by_dist_tool_t::exec(ws_item_t *item) {

  app_state_t *astate = app_state_t::get_inst();

  if (!item) {
      return;
    }

  auto gv_ptr = item->cast_as<geom_view_t>();

  if (!gv_ptr) {
      return;
    }

  colorize_by_dist_widget_t cw;
  cw.b_gv = gv_ptr;
  cw.populate_atom_type_cmb();

  int res = cw.exec();

  if (res == QDialog::Accepted) {
      geom_view_colorizer_helper::colorize_by_distance(
                      cw.b_gv,
                      cw.atom_dist_max->value(),
                      cw.clr_low,
                      cw.clr_low,
                      true, true,
                      cw.atom1_type->currentText().toStdString(),
                      cw.atom2_type->currentText().toStdString());
      cw.b_gv->m_color_mode = geom_view_color_e::color_from_xgeom;
      astate->make_viewport_dirty();
    }

}

void colorize_by_dist_widget_t::populate_atom_type_cmb() {

  if (!b_gv) return;

  for (size_t i = 0; i < b_gv->m_geom->n_atom_types(); i++) {
      QString atom_type = QString::fromStdString(b_gv->m_geom->atom_of_type(i));
      atom1_type->addItem(atom_type);
      atom2_type->addItem(atom_type);
    }

}

colorize_by_dist_widget_t::colorize_by_dist_widget_t() {

  main_lt = new QVBoxLayout;
  setLayout(main_lt);

  main_gb = new QGroupBox(tr("Colorize by distance : parameters"));
  main_gb_lt = new QFormLayout;
  main_gb->setLayout(main_gb_lt);

  clr_input = new qbinded_color3_input;
  clr_input->bind_value(&clr_low);

  atom1_type = new QComboBox;
  atom2_type = new QComboBox;
  atom_dist_max = new QDoubleSpinBox;
  atom_dist_max->setMinimum(0);
  atom_dist_max->setMaximum(30);
  atom_dist_max->setSingleStep(0.1);

  main_gb_lt->addRow(tr("Atom №1 type"), atom1_type);
  main_gb_lt->addRow(tr("Atom №2 type"), atom2_type);
  main_gb_lt->addRow(tr("Max. dist"), atom_dist_max);
  main_gb_lt->addRow(tr("Color"), clr_input);

  app_state_t *astate = app_state_t::get_inst();
  qt_helpers::resize_form_lt_lbls(main_gb_lt, astate->size_guide.obj_insp_lbl_w());

  buttons_lt = new QHBoxLayout;
  button_apply = new QPushButton(tr("Apply"));
  button_cancel = new QPushButton((tr("Cancel")));


  buttons_lt->addWidget(button_apply);
  buttons_lt->addWidget(button_cancel);

  main_lt->addWidget(main_gb);
  main_lt->addLayout(buttons_lt);

  connect(button_apply,
          &QPushButton::clicked,
          this,
          &colorize_by_dist_widget_t::accept);

  connect(button_cancel,
          &QPushButton::clicked,
          this,
          &colorize_by_dist_widget_t::reject);

}
