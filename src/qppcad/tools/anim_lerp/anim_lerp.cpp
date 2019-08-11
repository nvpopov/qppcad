#include <qppcad/tools/anim_lerp/anim_lerp.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void anim_lerp_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!item) {
      QMessageBox::warning(nullptr, QObject::tr("anim_lerp_tool"),
                           QObject::tr("ws_item == nullptr"));
      return;
    }

  auto al = item->cast_as<geom_view_t>();
  if (!al) {
      QMessageBox::warning(nullptr, QObject::tr("anim_lerp_tool"),
                           QObject::tr("ws_item.cast<geom_view_t>() == nullptr"));
      return;
    }

  if (!al->m_anim->animable()) {
      QMessageBox::warning(nullptr, QObject::tr("anim_lerp_tool"),
                           QObject::tr("ws_item.animable ? == False"));
      return;
    }

  anim_lerp_widget_t alw;
  alw.bind_geom_view(al);
  int ret_code = alw.exec();

  if (ret_code == QDialog::Accepted) {
      al->m_anim->make_interpolated_anim(fmt::format("new_anim{}", al->m_anim->get_total_anims()),
                                         alw.alerp_total_fr->value(),
                                         alw.alerp_anim1->currentIndex(),
                                         alw.alerp_anim1_frame->currentIndex(),
                                         alw.alerp_anim2->currentIndex(),
                                         alw.alerp_anim2_frame->currentIndex()
                                         );
    }

}

anim_lerp_widget_t::anim_lerp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  setWindowTitle(tr("Anim. lerp"));
  dialog_layout = new QVBoxLayout;
  setLayout(dialog_layout);

  gb_alerp_par = new QGroupBox(tr("Parameters"));
  gb_alerp_par_lt = new QFormLayout;
  gb_alerp_par->setLayout(gb_alerp_par_lt);

  alerp_anim1 = new QComboBox;
  connect(alerp_anim1,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &anim_lerp_widget_t::cmb_anim1_changed);

  alerp_anim2 = new QComboBox;
  connect(alerp_anim2,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &anim_lerp_widget_t::cmb_anim2_changed);

  alerp_anim1_frame = new QComboBox;
  alerp_anim2_frame = new QComboBox;

  alerp_total_fr = new QSpinBox;
  alerp_total_fr->setMinimum(2);
  alerp_total_fr->setAlignment(Qt::AlignCenter);
  alerp_total_fr->setButtonSymbols(QAbstractSpinBox::NoButtons);

  gb_alerp_par_lt->addRow(tr("First anim"), alerp_anim1);
  gb_alerp_par_lt->addRow(tr("Second anim"), alerp_anim2);
  gb_alerp_par_lt->addRow(tr("First anim frame"), alerp_anim1_frame);
  gb_alerp_par_lt->addRow(tr("Second anim frame"), alerp_anim2_frame);
  gb_alerp_par_lt->addRow(tr("Frames for new anim."), alerp_total_fr);

  dialog_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  for (auto btn : dialog_bb->buttons())
    btn->setFixedWidth(astate->size_guide.common_button_fixed_w());

  connect(dialog_bb,
          &QDialogButtonBox::accepted,
          this,
          &anim_lerp_widget_t::accept);

  connect(dialog_bb,
          &QDialogButtonBox::rejected,
          this,
          &anim_lerp_widget_t::reject);

  dialog_layout->addWidget(gb_alerp_par);
  dialog_layout->addWidget(dialog_bb);

}

void anim_lerp_widget_t::bind_geom_view(geom_view_t *_gv) {

  if (!_gv) return;

  m_gv = _gv;
  populate_anims_cmbs();

}

void anim_lerp_widget_t::populate_anims_cmbs() {

  alerp_anim1->blockSignals(true);
  alerp_anim2->blockSignals(true);

  alerp_anim1->clear();
  alerp_anim2->clear();

  for (size_t i = 0; i < m_gv->m_anim->get_total_anims(); i++) {
      QString anim_rec = QString("[%1] %2")
                         .arg(i)
                         .arg(QString::fromStdString(m_gv->m_anim->m_anim_data[i].m_anim_name));
      alerp_anim1->addItem(anim_rec);
      alerp_anim2->addItem(anim_rec);
    }

  alerp_anim1->blockSignals(false);
  alerp_anim2->blockSignals(false);

  alerp_anim1->setCurrentIndex(0);
  alerp_anim2->setCurrentIndex(0);
  cmb_anim1_changed(0);
  cmb_anim2_changed(0);

}

void anim_lerp_widget_t::cmb_anim_changed_marshall(QComboBox *cmb, QComboBox *frm, int cmb_idx) {

  if (!cmb || !frm || !m_gv || cmb_idx < 0) return;

  frm->clear();

  for (size_t i = 0; i < m_gv->m_anim->m_anim_data[cmb_idx].frames.size(); i++)
    frm->addItem(QString("%1").arg(i));

}

void anim_lerp_widget_t::cmb_anim1_changed(int index) {
  cmb_anim_changed_marshall(alerp_anim1, alerp_anim1_frame, index);
}

void anim_lerp_widget_t::cmb_anim2_changed(int index) {
  cmb_anim_changed_marshall(alerp_anim2, alerp_anim2_frame, index);
}

