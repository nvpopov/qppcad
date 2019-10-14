#include <qppcad/tools/colorize_by_dist/colorize_by_dist.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools_colorize.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

void colorize_by_dist_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

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
  cw.init_data();

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

      //cache tool data
      astate->m_env_float["T_CLRD_DIST"] = cw.atom_dist_max->value();
      astate->m_env_str["T_CLRD_T1"] = cw.atom1_type->currentText().toStdString();
      astate->m_env_str["T_CLRD_T2"] = cw.atom2_type->currentText().toStdString();
      astate->m_env_vec3["T_CLRD_CLR"] = cw.clr_low;

      astate->make_viewport_dirty();

    }

}

void colorize_by_dist_widget_t::init_data() {

  app_state_t *astate = app_state_t::get_inst();

  if (!b_gv) return;

  for (size_t i = 0; i < b_gv->m_geom->n_atom_types(); i++) {
      QString atom_type = QString::fromStdString(b_gv->m_geom->atom_of_type(i));
      atom1_type->addItem(atom_type);
      atom2_type->addItem(atom_type);
    }

  auto it_d = astate->m_env_float.find("T_CLRD_DIST");
  if (it_d != astate->m_env_float.end()) atom_dist_max->setValue(it_d->second);

  auto it_clr = astate->m_env_vec3.find("T_CLRD_CLR");
  if (it_clr != astate->m_env_vec3.end()) {
      clr_low = it_clr->second;
      clr_input->load_value_ex();
    }

  auto it_t1 = astate->m_env_str.find("T_CLRD_T1");
  auto it_t2 = astate->m_env_str.find("T_CLRD_T2");

  if (it_t1 != astate->m_env_str.end()) {
      QString type1 = QString::fromStdString(it_t1->second);
      int type1_idx = atom1_type->findText(type1);
      if (type1_idx >= 0) atom1_type->setCurrentIndex(type1_idx);
    }

  if (it_t2 != astate->m_env_str.end()) {
      QString type2 = QString::fromStdString(it_t2->second);
      int type2_idx = atom1_type->findText(type2);
      if (type2_idx >= 0) atom2_type->setCurrentIndex(type2_idx);
    }

}

colorize_by_dist_widget_t::colorize_by_dist_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  main_lt = new QVBoxLayout;
  setLayout(main_lt);
  setFixedWidth(320);

  main_gb = new qspoiler_widget_t(tr("Colorize by distance : parameters"), nullptr, false, 6, 300);
  main_gb_lt = new QFormLayout;
  main_gb->add_content_layout(main_gb_lt);

  clr_input = new qbinded_color3_input_t;
  clr_input->bind_value(&clr_low);

  atom1_type = new QComboBox;
  atom1_type->setFixedWidth(astate->size_guide.obj_insp_ctrl_max_w());

  atom2_type = new QComboBox;
  atom2_type->setFixedWidth(astate->size_guide.obj_insp_ctrl_max_w());

  atom_dist_max = new QDoubleSpinBox;
  atom_dist_max->setFixedWidth(astate->size_guide.obj_insp_ctrl_max_w());
  atom_dist_max->setMinimum(0);
  atom_dist_max->setMaximum(30);
  atom_dist_max->setSingleStep(0.1);
  atom_dist_max->setAlignment(Qt::AlignCenter);
  atom_dist_max->setLocale(QLocale::C);
  atom_dist_max->setButtonSymbols(QAbstractSpinBox::NoButtons);

  main_gb_lt->addRow(tr("Atom №1 type"), atom1_type);
  main_gb_lt->addRow(tr("Atom №2 type"), atom2_type);
  main_gb_lt->addRow(tr("Max. dist"), atom_dist_max);
  main_gb_lt->addRow(tr("Color"), clr_input);

  qt_hlp::resize_form_lt_lbls(main_gb_lt, 170);

  buttons_lt = new QHBoxLayout;
  button_apply = new QPushButton(tr("Apply"));
  button_cancel = new QPushButton((tr("Cancel")));

  buttons_lt->addSpacing(0);
  buttons_lt->addWidget(button_apply);
  buttons_lt->addWidget(button_cancel);
  buttons_lt->addSpacing(0);

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
