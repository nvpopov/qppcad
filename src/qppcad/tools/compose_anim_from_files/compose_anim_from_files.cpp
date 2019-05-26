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

}
