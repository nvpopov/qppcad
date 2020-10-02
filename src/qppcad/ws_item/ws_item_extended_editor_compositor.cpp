#include <qppcad/ws_item/ws_item_extended_editor_compositor.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_item_extended_editor_compositor_t::ws_item_extended_editor_compositor_t(QWidget *parent) :
  qembed_window_t (parent) {

  app_state_t *astate = app_state_t::get_inst();

  setMinimumWidth(300);
  setObjectName("ext_editor_compositor");
  setProperty("border_class", "border_left");

  m_ew_header->setText(tr("EXTENDED EDITOR"));

  m_ext_edt_plch = new QWidget;
  m_main_lt->addWidget(m_ext_edt_plch);
  m_main_lt->setSpacing(0);

  m_top_btn_close = new QPushButton;
  m_top_btn_close->setFixedSize(QSize(astate->size_guide.spoiler_button_h(),
                                      astate->size_guide.spoiler_button_h()));
  m_top_btn_close->setIconSize(QSize(astate->size_guide.spoiler_button_icon_h(),
                                     astate->size_guide.spoiler_button_icon_h()));
  m_top_btn_close->setIcon(QIcon("://images/outline-close-24px.svg"));
  m_top_btn_close->setFlat(true);

  connect(m_top_btn_close,
          &QPushButton::clicked,
          this,
          &ws_item_extended_editor_compositor_t::close_ext_editor_btn_clicked);

  m_top_btn_refresh = new QPushButton;
  m_top_btn_refresh->setFixedSize(QSize(astate->size_guide.spoiler_button_h(),
                                        astate->size_guide.spoiler_button_h()));
  m_top_btn_refresh->setIconSize(QSize(astate->size_guide.spoiler_button_icon_h(),
                                       astate->size_guide.spoiler_button_icon_h()));
  m_top_btn_refresh->setIcon(QIcon("://images/outline-refresh-24px.svg"));
  m_top_btn_refresh->setFlat(true);

  connect(m_top_btn_refresh,
          &QPushButton::clicked,
          this,
          &ws_item_extended_editor_compositor_t::refresh_ext_editor_btn_clicked);

  m_header_lt->insertWidget(0, m_top_btn_refresh);
  m_header_lt->insertWidget(0, m_top_btn_close);

  connect(astate->astate_evd,
          &app_state_event_disp_t::extended_editor_open_requested_signal,
          this,
          &ws_item_extended_editor_compositor_t::open_requested);

  connect(astate->astate_evd,
          &app_state_event_disp_t::extended_editor_open_requested_with_order_signal,
          this,
          &ws_item_extended_editor_compositor_t::open_requested_with_order);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_changed_signal,
          this,
          &ws_item_extended_editor_compositor_t::cur_ws_selected_item_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_changed_signal,
          this,
          &ws_item_extended_editor_compositor_t::cur_ws_changed);

}

ws_item_extended_editor_compositor_t::~ws_item_extended_editor_compositor_t() {
  if (m_cur_ext_editor_widget) {
    m_cur_ext_editor_widget->unbind_item();
    m_main_lt->removeWidget(m_cur_ext_editor_widget.get());
    m_cur_ext_editor_widget->setParent(nullptr);
    m_cur_ext_editor_widget = nullptr;
  }
}

void ws_item_extended_editor_compositor_t::open_extended_editor(size_t editor_id) {

  app_state_t *astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_mgr->m_bhv_mgr.get();

  bool need_to_hide_compositor = true;

  if (m_cur_ext_editor_widget) {
    m_cur_ext_editor_widget->unbind_item();
    auto coiw_ptr = m_cur_ext_editor_widget.get();
    if (coiw_ptr)
      m_main_lt->removeWidget(coiw_ptr);
    m_cur_ext_editor_widget->setParent(nullptr);
    m_cur_ext_editor_widget = nullptr;
  }

  if (astate->ws_mgr->has_wss()) {
    auto cur_ws = astate->ws_mgr->get_cur_ws();
    if (cur_ws) {
      auto cur_it = cur_ws->get_sel();
      if (cur_it) {
        size_t thash = cur_it->get_type();
        auto ext_editor_w = bhv_mgr->get_ext_editor_widget_sp(thash, editor_id);
        if (ext_editor_w && ext_editor_w->can_be_binded_to(cur_it)) {
          need_to_hide_compositor = false;
          ext_editor_w->bind_to_item(cur_it);
          cur_it->m_ext_editor_id = editor_id;
          m_cur_ext_editor_widget = ext_editor_w;
          m_main_lt->addWidget(ext_editor_w.get());
          m_ew_header->setText(tr("%1 [%2/%3]")
                               .arg(ext_editor_w->header_name_hint())
                               .arg(QString::fromStdString(cur_ws->m_ws_name))
                               .arg(QString::fromStdString(cur_it->m_name.get_value())));
          show();
          cur_it->m_ext_editor_opened = true;
        }
      }
      else {

      }
    }
  }

  if (need_to_hide_compositor)
    hide();

}

void ws_item_extended_editor_compositor_t::open_requested() {
  open_extended_editor();
}

void ws_item_extended_editor_compositor_t::open_requested_with_order(size_t editor_id) {
  open_extended_editor(editor_id);
}

void ws_item_extended_editor_compositor_t::cur_ws_selected_item_changed() {

  app_state_t* astate = app_state_t::get_inst();

  if (m_cur_ext_editor_widget)
    m_cur_ext_editor_widget->unbind_item();

  if (astate->ws_mgr->has_wss()) {
    auto cur_ws = astate->ws_mgr->get_cur_ws();
    if (cur_ws) {
      auto cur_it = cur_ws->get_sel();
      if (cur_it && cur_it->m_ext_editor_opened) {
        open_extended_editor(cur_it->m_ext_editor_id);
      }
      else {
        hide();
      }
    }
  }

}

void ws_item_extended_editor_compositor_t::cur_ws_changed() {

  cur_ws_selected_item_changed();

}

void ws_item_extended_editor_compositor_t::close_ext_editor_btn_clicked() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_mgr->has_wss()) {
    auto cur_ws = astate->ws_mgr->get_cur_ws();
    if (cur_ws) {
      auto cur_it = cur_ws->get_sel();
      if (cur_it && cur_it->m_ext_editor_opened) {
        cur_it->m_ext_editor_opened = false;
      }
    }
  }

  hide();

}

void ws_item_extended_editor_compositor_t::refresh_ext_editor_btn_clicked() {
  if (m_cur_ext_editor_widget)
    m_cur_ext_editor_widget->update_from_ws_item();
}
