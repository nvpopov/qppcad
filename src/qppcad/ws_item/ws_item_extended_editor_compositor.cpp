#include <qppcad/ws_item/ws_item_extended_editor_compositor.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


ws_item_extended_editor_compositor_t::ws_item_extended_editor_compositor_t(QWidget *parent) :
  qembed_window_t (parent) {

  app_state_t *astate = app_state_t::get_inst();

  setMinimumWidth(300);
  setObjectName("ext_editor_compositor");
  ew_header->setText(tr("EXTENDED EDITOR"));

  ext_edt_plch = new QWidget;
  main_lt->addWidget(ext_edt_plch);

  top_btn_close = new QPushButton;
  top_btn_close->setFixedSize(QSize(astate->size_guide.ext_editor_btn_h(),
                                    astate->size_guide.ext_editor_btn_h()));
  top_btn_close->setIcon(QIcon("://images/outline-close-24px.svg"));

  connect(top_btn_close,
          &QPushButton::clicked,
          this,
          &ws_item_extended_editor_compositor_t::close_ext_editor_btn_clicked);

  header_lt->insertWidget(0, top_btn_close);

  connect(astate->astate_evd,
          &app_state_event_disp_t::extended_editor_open_requested_signal,
          this,
          &ws_item_extended_editor_compositor_t::open_requested);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_changed_signal,
          this,
          &ws_item_extended_editor_compositor_t::cur_ws_selected_item_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_changed_signal,
          this,
          &ws_item_extended_editor_compositor_t::cur_ws_changed);

}

void ws_item_extended_editor_compositor_t::open_requested() {

  app_state_t *astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_manager->m_bhv_mgr.get();

  bool need_to_hide_compositor = true;

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();

          if (cur_it) {
              size_t thash = cur_it->get_type();
              auto ext_editor_w = bhv_mgr->get_ext_editor_widget_sp(thash);
              //              for (auto elem : bhv_mgr->m_ext_editors)
              // elem.second->setVisible(false);
              //              QLayoutItem *item;
              //              while ((item = layout()->takeAt(0)))
              //                item->widget()->setParent(nullptr);
              if (ext_editor_w) {
                  // layout()->addWidget(ext_editor_w.get());
                  need_to_hide_compositor = false;
                  show();
                  cur_it->m_extended_editor_opened = true;
                }
            }
          else {

            }

        }

    }

  if (need_to_hide_compositor) hide();

}

void ws_item_extended_editor_compositor_t::cur_ws_selected_item_changed() {

  app_state_t* astate = app_state_t::get_inst();


  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();

          if (cur_it && cur_it->m_extended_editor_opened) {
              show();
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


  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {

          auto cur_it = cur_ws->get_selected();

          if (cur_it && cur_it->m_extended_editor_opened) {
              cur_it->m_extended_editor_opened = false;
            }

        }

    }

  hide();

}
