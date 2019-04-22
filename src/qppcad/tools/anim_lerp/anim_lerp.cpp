#include <qppcad/tools/anim_lerp/anim_lerp.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void anim_lerp_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!item) {
      QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                           QObject::tr("ws_item == nullptr"));
      return;
    }

  auto al = item->cast_as<geom_view_t>();
  if (!al) {
      QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                           QObject::tr("ws_item.cast<geom_view_t>() == nullptr"));
      return;
    }

  anim_lerp_widget_t alw;
  int ret_code = alw.exec();

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
  alerp_anim2 = new QComboBox;
  alerp_anim1_frame = new QComboBox;
  alerp_anim2_frame = new QComboBox;
  alerp_total_fr = new QSpinBox;
  alerp_total_fr->setMinimum(0);

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
