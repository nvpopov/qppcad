#include <qppcad/ui/toolbar_element.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/core/workspace.hpp>

using namespace qpp;
using namespace qpp::cad;

toolbar_element_t::toolbar_element_t(toolbar_element_style_e style,
                                     std::set<size_t> &&applicable_types,
                                     QWidget *parent) {

  m_parent = parent;
  m_style = style;
  m_applicable_types = std::move(applicable_types);

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
