#include <qppcad/tools/compose_anim_from_files/compose_anim_from_files.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void compose_anim_from_files_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  astate->log("compose_anim_from_files_tool_t::exec()");

  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws();

  if (!ok) {
      QMessageBox::warning(nullptr, QObject::tr("compose_anim_from_files_tool_t"),
                           QObject::tr("cur_ws == nullptr"));
      return;
    }

  compose_anim_from_files_widget_t caw;
  int ret_code = caw.exec();

  if (ret_code == QDialog::Accepted) {
      astate->make_viewport_dirty();
    }

}

compose_anim_from_files_widget_t::compose_anim_from_files_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  setWindowTitle(tr("Compose anim from files"));

  main_lt = new QVBoxLayout;
  setLayout(main_lt);

  gv_selector = new qgeom_view_selector_widget_t;
  dialog_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  for (auto btn : dialog_bb->buttons())
    btn->setFixedWidth(astate->size_guide.common_button_fixed_w());

  connect(dialog_bb,
          &QDialogButtonBox::accepted,
          this,
          &compose_anim_from_files_widget_t::accept);

  connect(dialog_bb,
          &QDialogButtonBox::rejected,
          this,
          &compose_anim_from_files_widget_t::reject);

  main_lt->addWidget(gv_selector);
  main_lt->addWidget(dialog_bb);

}
