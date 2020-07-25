#include <qppcad/ui/toolbar_element.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/core/workspace.hpp>

using namespace qpp;
using namespace qpp::cad;

toolbar_element_t::toolbar_element_t() {

  //  m_parent = parent;
  //  m_style = style;
  //  m_applicable_types = std::move(applicable_types);

}

void toolbar_element_t::init_element(QWidget *parent) {

}

QString toolbar_element_t::get_tooltip_for(size_t btn_id) {
  return QString("");
}

QString toolbar_element_t::get_icon_for(size_t btn_id) {
  return QString("");
}

size_t toolbar_element_t::get_btn_cnt() {
  return 1;
}

std::tuple<size_t, size_t> toolbar_element_t::get_size(size_t btn_id) {

  app_state_t* astate = app_state_t::get_inst();
  return {
      astate->size_guide.tool_panel_ctrl_w(),
      astate->size_guide.tool_panel_ctrl_h()
  };

}

void toolbar_element_t::clicked(size_t btn_id) {

}

void toolbar_element_t::on_wss_changed(app_state_t *astate) {

}

void toolbar_element_t::on_cur_ws_changed(app_state_t *astate, workspace_t *cur_ws) {

}

void toolbar_element_t::on_cur_ws_sel_itm_changed(app_state_t *astate, ws_item_t *cur_item) {

}

void toolbar_element_t::on_cur_ws_prop_changed(app_state_t *astate, workspace_t *cur_ws) {

}

void toolbar_element_t::on_cur_ws_sel_cnt_changed(app_state_t *astate, ws_item_t *cur_item) {

}
