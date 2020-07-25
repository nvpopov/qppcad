#ifndef QPPCAD_TOOLBAR_ELEMENT
#define QPPCAD_TOOLBAR_ELEMENT

#include <qppcad/core/qppcad.hpp>
#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QToolButton>

namespace qpp {

namespace cad {

enum class toolbar_element_style_e {
  toolbar_element_style_button,
  toolbar_element_style_dropdown,
  toolbar_element_style_group
};

class toolbar_element_t;
class app_state_t;
class ws_item_t;
class workspace_t;

struct toolbar_element_info_t {
  std::string m_full_name;
  std::function<std::shared_ptr<toolbar_element_t>()> m_fabric;
};

class toolbar_element_t {

private:

  // ui stuff
  QWidget *m_parent{nullptr};
  QButtonGroup *m_btn_grp{nullptr};
  QToolButton *m_tool_btn{nullptr};

  std::vector<QPushButton*> m_btns;

  // data stuff
  toolbar_element_style_e m_style{toolbar_element_style_e::toolbar_element_style_button};
  std::set<size_t> m_applicable_types;
  bool m_show_permanent{true};

public:

  // methods
  explicit toolbar_element_t();

  void init_element(QWidget *parent = nullptr);

  virtual QString get_tooltip_for(size_t btn_id = 0);
  virtual QString get_icon_for(size_t btn_id = 0);
  virtual size_t get_btn_cnt();
  virtual std::tuple<size_t, size_t> get_size(size_t btn_id = 0);
  virtual void clicked(size_t btn_id = 0);
  virtual void on_wss_changed(app_state_t *astate);
  virtual void on_cur_ws_changed(app_state_t *astate, workspace_t *cur_ws);
  virtual void on_cur_ws_sel_itm_changed(app_state_t *astate, ws_item_t *cur_item);
  virtual void on_cur_ws_prop_changed(app_state_t *astate, workspace_t *cur_ws);
  virtual void on_cur_ws_sel_cnt_changed(app_state_t *astate, ws_item_t *cur_item);

};

} // namespace qpp::cad

} // namespace qpp

#endif
