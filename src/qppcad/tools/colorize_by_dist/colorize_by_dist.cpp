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
                      cw.m_atom_dist_max->value(),
                      cw.clr_low,
                      cw.clr_low,
                      true, true,
                      cw.m_atom1_type->currentText().toStdString(),
                      cw.m_atom2_type->currentText().toStdString());
      cw.b_gv->m_color_mode = geom_view_color_e::color_from_xgeom;

      //cache tool data
      astate->m_env_float["T_CLRD_DIST"] = cw.m_atom_dist_max->value();
      astate->m_env_str["T_CLRD_T1"] = cw.m_atom1_type->currentText().toStdString();
      astate->m_env_str["T_CLRD_T2"] = cw.m_atom2_type->currentText().toStdString();
      astate->m_env_vec3["T_CLRD_CLR"] = cw.clr_low;

      astate->make_viewport_dirty();

    }

}

void colorize_by_dist_widget_t::init_data() {

  app_state_t *astate = app_state_t::get_inst();

  if (!b_gv) return;

  for (size_t i = 0; i < b_gv->m_geom->n_atom_types(); i++) {
      QString atom_type = QString::fromStdString(b_gv->m_geom->atom_of_type(i));
      m_atom1_type->addItem(atom_type);
      m_atom2_type->addItem(atom_type);
    }

  auto it_d = astate->m_env_float.find("T_CLRD_DIST");
  if (it_d != astate->m_env_float.end()) m_atom_dist_max->setValue(it_d->second);

  auto it_clr = astate->m_env_vec3.find("T_CLRD_CLR");
  if (it_clr != astate->m_env_vec3.end()) {
      clr_low = it_clr->second;
      m_clr_input->load_value_ex();
    }

  auto it_t1 = astate->m_env_str.find("T_CLRD_T1");
  auto it_t2 = astate->m_env_str.find("T_CLRD_T2");

  if (it_t1 != astate->m_env_str.end()) {
      QString type1 = QString::fromStdString(it_t1->second);
      int type1_idx = m_atom1_type->findText(type1);
      if (type1_idx >= 0) m_atom1_type->setCurrentIndex(type1_idx);
    }

  if (it_t2 != astate->m_env_str.end()) {
      QString type2 = QString::fromStdString(it_t2->second);
      int type2_idx = m_atom1_type->findText(type2);
      if (type2_idx >= 0) m_atom2_type->setCurrentIndex(type2_idx);
    }

}

colorize_by_dist_widget_t::colorize_by_dist_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  m_main_lt = new QVBoxLayout;
  setLayout(m_main_lt);
  setFixedWidth(astate->size_guide.ws_item_modal_tool_small_width());

  m_main_gb = new qspoiler_widget_t(tr("Colorize by distance : parameters"), nullptr, false, 6,
                                    astate->size_guide.ws_item_modal_tool_small_gb_width());
  m_main_gb_lt = new QFormLayout;
  m_main_gb->add_content_layout(m_main_gb_lt);

  m_clr_input = new qbinded_color3_input_t;
  m_clr_input->bind_value(&clr_low);

  m_atom1_type = new QComboBox;
  m_atom1_type->setFixedWidth(astate->size_guide.obj_insp_ctrl_max_w());

  m_atom2_type = new QComboBox;
  m_atom2_type->setFixedWidth(astate->size_guide.obj_insp_ctrl_max_w());

  m_atom_dist_max = new QDoubleSpinBox;
  m_atom_dist_max->setFixedWidth(astate->size_guide.obj_insp_ctrl_max_w());
  m_atom_dist_max->setMinimum(0);
  m_atom_dist_max->setMaximum(30);
  m_atom_dist_max->setSingleStep(0.1);
  m_atom_dist_max->setAlignment(Qt::AlignCenter);
  m_atom_dist_max->setLocale(QLocale::C);
  m_atom_dist_max->setButtonSymbols(QAbstractSpinBox::NoButtons);

  m_main_gb_lt->addRow(tr("Atom №1 type"), m_atom1_type);
  m_main_gb_lt->addRow(tr("Atom №2 type"), m_atom2_type);
  m_main_gb_lt->addRow(tr("Max. dist"), m_atom_dist_max);
  m_main_gb_lt->addRow(tr("Color"), m_clr_input);

  qt_hlp::resize_form_lt_lbls(m_main_gb_lt,
                              astate->size_guide.ws_item_modal_tool_small_form_label_width());

  m_buttons_lt = new QHBoxLayout;
  m_button_apply = new QPushButton(tr("Apply"));
  m_button_cancel = new QPushButton((tr("Cancel")));

  m_buttons_lt->addSpacing(0);
  m_buttons_lt->addWidget(m_button_apply);
  m_buttons_lt->addWidget(m_button_cancel);
  m_buttons_lt->addSpacing(0);

  m_main_lt->addWidget(m_main_gb);
  m_main_lt->addLayout(m_buttons_lt);

  connect(m_button_apply,
          &QPushButton::clicked,
          this,
          &colorize_by_dist_widget_t::accept);

  connect(m_button_cancel,
          &QPushButton::clicked,
          this,
          &colorize_by_dist_widget_t::reject);

}
