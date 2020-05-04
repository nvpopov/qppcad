#include <qppcad/ui/ptable_rich_widget.hpp>
#include <data/ptable.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

ptable_rich_widget_t::ptable_rich_widget_t(QWidget *parent) : QDialog(parent) {

  setWindowTitle(tr("Periodic Table"));

  m_btn_grp = new QButtonGroup;

  m_main_lt = new QHBoxLayout;
  m_elem_lt = new QGridLayout;
  m_main_lt->addLayout(m_elem_lt);
  setLayout(m_main_lt);

  construct_widgets();

  connect(m_btn_grp,
          QOverload<int>::of(&QButtonGroup::buttonClicked),
          this,
          &ptable_rich_widget_t::btn_grp_clicked);

}

void ptable_rich_widget_t::construct_widgets() {

  ptable *pt_inst = ptable::get_inst();

  for (size_t i = 0; i < PTABLE_ELEM_N; i++) {

      std::optional<std::tuple<int, int> > elem_pos = get_ptable_x_y(i);

      if (!elem_pos) continue;

      QPushButton *tst = new QPushButton;
      m_btn_grp->addButton(tst, i);
      colorize_element_btn(i);

      tst->setText(QString::fromStdString(pt_inst->arecs[i].m_symbol));
      m_elem_lt->addWidget(tst, std::get<1>(*elem_pos), std::get<0>(*elem_pos));

    }



}

void ptable_rich_widget_t::colorize_element_btn(int btn_id) {

  QAbstractButton *btn = m_btn_grp->button(btn_id);
  if (!btn) return;

  ptable *pt_inst = ptable::get_inst();

  btn->setStyleSheet(QString("QPushButton {"
                             "background-color: rgb(%1, %2, %3);"
                             "}")
                        .arg(int(pt_inst->arecs[btn_id].m_color_jmol[0]*255))
                        .arg(int(pt_inst->arecs[btn_id].m_color_jmol[1]*255))
                        .arg(int(pt_inst->arecs[btn_id].m_color_jmol[2]*255)));

}

std::optional<std::tuple<int, int> > ptable_rich_widget_t::get_ptable_x_y(size_t elem_id) {

  auto elem = elem_id + 1;

  using pair_int = std::optional<std::tuple<int, int> >;

  if (elem == 1) return pair_int({0, 0});
  if (elem == 2) return pair_int({17, 0});

  if (elem == 3) return pair_int({0, 1});
  if (elem == 4) return pair_int({1, 1});
  if (elem >= 5 && elem <= 10) return pair_int({elem + 7, 1});

  if (elem == 11) return pair_int({0, 2});
  if (elem == 12) return pair_int({1, 2});
  if (elem >= 13 && elem <= 18) return pair_int({elem - 1, 2});
  if (elem >= 19 && elem <= 36) return pair_int({elem - 19, 3});
  if (elem >= 37 && elem <= 54) return pair_int({elem - 37, 4});

  if (elem >= 55 && elem <= 56) return pair_int({elem - 55, 5});
  if (elem >= 72 && elem <= 86) return pair_int({elem - 69, 5});

  if (elem >= 87 && elem <= 88) return pair_int({elem - 87, 6});
  if (elem >= 104 && elem <= 118) return pair_int({elem - 101, 6});

  /* lanthanides */
  if (elem >= 57 && elem <= 71) return pair_int({elem - 55, 8});

  /* actinides */
  if (elem >= 89 && elem <= 103) return pair_int({elem - 87, 9});

  return std::nullopt;

}

void ptable_rich_widget_t::btn_grp_clicked(int id) {

  ptable_element_editor_t elem_edtr(id);

  if (elem_edtr.exec() == QDialog::Accepted) {

      elem_edtr.update_element_data();
      colorize_element_btn(elem_edtr.m_elem_id);
      app_state_t::get_inst()->make_viewport_dirty();

    }

}

ptable_element_editor_t::ptable_element_editor_t(int elem_id, QWidget *parent) : QDialog (parent) {

  const auto widget_width = 280;

  app_state_t *astate = app_state_t::get_inst();
  ptable *pt_inst = ptable::get_inst();

  m_elem_id = elem_id;

  setFixedWidth(widget_width+8);

  setWindowTitle(
        QString("Edit element - %1").arg(QString::fromStdString(pt_inst->arecs[elem_id].m_symbol))
        );

  m_main_lt = new QVBoxLayout;
  m_main_lt->setContentsMargins(4, 2, 4, 5);
  setLayout(m_main_lt);

  m_main_splr = new qspoiler_widget_t(tr("Element properties"), nullptr, false, 6, widget_width);
  m_main_splr->set_top_border(false);
  m_splr_lt = new QFormLayout;
  m_main_splr->add_content_layout(m_splr_lt);

  m_radius = new qbinded_float_spinbox_t;
  m_radius->set_min_max_step(-1, 20, 0.01);

  m_ionic_rad = new qbinded_float_spinbox_t;
  m_ionic_rad->set_min_max_step(0.0, 50, 0.01);

  m_cov_rad = new qbinded_float_spinbox_t;
  m_cov_rad->set_min_max_step(0.0, 50, 0.01);

  m_color = new qbinded_color3_input_t;

  m_splr_lt->addRow(tr("Radius"), m_radius);
  m_splr_lt->addRow(tr("Ionic Radius"), m_ionic_rad);
  m_splr_lt->addRow(tr("Covalent Radius"), m_cov_rad);
  m_splr_lt->addRow(tr("Color"), m_color);

  qt_hlp::resize_form_lt_lbls(m_splr_lt, astate->size_guide.obj_insp_lbl_w());

  m_dialog_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(m_dialog_bb,
          &QDialogButtonBox::accepted,
          this,
          &ptable_element_editor_t::accept);

  connect(m_dialog_bb,
          &QDialogButtonBox::rejected,
          this,
          &ptable_element_editor_t::reject);

  m_main_lt->addWidget(m_main_splr);
  m_main_lt->addWidget(m_dialog_bb);

  /* load data from ptable and then bind inputs*/
  m_binded_radius.set_cvalue(pt_inst->arecs[m_elem_id].m_radius);
  m_binded_ionic_rad.set_cvalue(pt_inst->arecs[m_elem_id].m_ionic_radius);
  m_binded_cov_rad.set_cvalue(pt_inst->arecs[m_elem_id].m_covrad_slater);
  m_binded_color.set_cvalue(pt_inst->arecs[m_elem_id].m_color_jmol);

  m_radius->bind_value(&m_binded_radius);
  m_ionic_rad->bind_value(&m_binded_ionic_rad);
  m_cov_rad->bind_value(&m_binded_cov_rad);
  m_color->bind_value(&m_binded_color);

}

void ptable_element_editor_t::update_element_data() {

  if (m_elem_id == -1) return;

  ptable *pt_inst = ptable::get_inst();

  pt_inst->arecs[m_elem_id].m_radius = m_binded_radius.get_value();
  pt_inst->arecs[m_elem_id].m_ionic_radius = m_binded_ionic_rad.get_value();
  pt_inst->arecs[m_elem_id].m_covrad_slater = m_binded_cov_rad.get_value();
  pt_inst->arecs[m_elem_id].m_color_jmol = m_binded_color.get_value();
  pt_inst->arecs[m_elem_id].m_redefined = true;

}
