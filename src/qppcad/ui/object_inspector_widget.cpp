#include <qppcad/ui/object_inspector_widget.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view.hpp>

using namespace qpp;
using namespace qpp::cad;

object_inspector_widget_t::object_inspector_widget_t(QWidget *parent) : qembed_window_t(parent) {

  app_state_t* astate = app_state_t::get_inst();

  ew_header->setText(tr("Object inspector"));

  setProperty("s_class", "obj_insp");

  ws_items_label = new QLabel;
  ws_items_label->setText(tr("Workspace items:"));

  ws_items_list = new QListWidget;

  ws_items_list->setMaximumHeight(astate->size_guide.obj_insp_item_list_max_h());
  ws_items_list->setProperty("s_class", "ws_items_list");
  ws_items_list->setFocusPolicy(Qt::NoFocus);

  none_item_placeholder = new QWidget;
  none_item_placeholder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  main_lt->addWidget(ws_items_label);
  main_lt->addWidget(ws_items_list);
  main_lt->addWidget(none_item_placeholder);
  //main_lt->setContentsMargins(10, 4, 0, 0);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_changed_signal,
          this,
          &object_inspector_widget_t::cur_ws_selected_item_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_changed_signal,
          this,
          &object_inspector_widget_t::cur_ws_changed);

  connect(ws_items_list,
          &QListWidget::itemSelectionChanged,
          this,
          &object_inspector_widget_t::ui_cur_ws_selected_item_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_need_to_update_obj_insp_signal,
          this,
          &object_inspector_widget_t::need_to_update_obj_insp_received);

  connect(ws_items_list,
          &QListWidget::itemDoubleClicked,
          this,
          &object_inspector_widget_t::ws_item_list_double_clicked);

  cur_ws_changed();
  ui_cur_ws_selected_item_changed();
  ws_items_list->clearSelection();

}

object_inspector_widget_t::~object_inspector_widget_t() {

  if (m_cur_obj_insp_widget) {
      m_cur_obj_insp_widget->unbind_item();
      main_lt->removeWidget(m_cur_obj_insp_widget.get());
      m_cur_obj_insp_widget->setParent(nullptr);
      m_cur_obj_insp_widget = nullptr;
    }

}

void object_inspector_widget_t::update_ws_items_view_widget() {

  app_state_t* astate = app_state_t::get_inst();
  ws_item_behaviour_manager_t *bhv_mgr = astate->ws_manager->m_bhv_mgr.get();

  if (!bhv_mgr) return;

  if (m_cur_obj_insp_widget) {
      m_cur_obj_insp_widget->unbind_item();
      auto coiw_ptr = m_cur_obj_insp_widget.get();
      if (coiw_ptr) main_lt->removeWidget(coiw_ptr);
      m_cur_obj_insp_widget->setParent(nullptr);
      m_cur_obj_insp_widget = nullptr;
      none_item_placeholder->show();
    }

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();

          if (cur_it) {
              size_t thash = cur_it->get_type();
              auto obj_insp_w = bhv_mgr->get_obj_insp_widget_sp(thash);
              if (obj_insp_w) {

                  // a dirty hack for preventing object inspectors widgets to be unbounded
                  for (auto elem : bhv_mgr->m_obj_insp_widgets) elem.second->setVisible(false);

                  none_item_placeholder->hide();
                  main_lt->addWidget(obj_insp_w.get());
                  m_cur_obj_insp_widget = obj_insp_w;
                  obj_insp_w->bind_to_item(cur_it);
                  obj_insp_w->show();
                }
            }

        }

    }
}

void object_inspector_widget_t::cur_ws_changed() {

  app_state_t* astate = app_state_t::get_inst();

  astate->log("DEBUG: object_inspector_widget_t::cur_ws_changed");

  ws_items_list->blockSignals(true);

  ws_items_list->clear();
  ws_items_list->clearSelection();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          for (size_t i = 0; i < cur_ws->m_ws_items.size(); i++) {
              ws_items_list->addItem(
                    QString::fromStdString(fmt::format("[{}] {} ",
                                                       i, cur_ws->m_ws_items[i]->m_name))
                    );
            }
        }
    }

  cur_ws_selected_item_changed();
  ws_items_list->blockSignals(false);

}

void object_inspector_widget_t::cur_ws_selected_item_changed() {

  app_state_t* astate = app_state_t::get_inst();

  astate->log("DEBUG: obj_insp_widget_t::cur_ws_selected_item_changed");

  ws_items_list->blockSignals(true);

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {

          astate->log(fmt::format("DEBUG ::cur_ws_selected_item_changed(), "
                                  "[num_wsi = {}]", cur_ws->m_ws_items.size()));
          auto cur_id = cur_ws->get_selected_idx();
          if (cur_id) {
              ws_items_list->item(*cur_id)->setSelected(true);
            }
          else {
              ws_items_list->clearSelection();
            }
        }

    }

  update_ws_items_view_widget();
  ws_items_list->blockSignals(false);

}

void object_inspector_widget_t::ui_cur_ws_selected_item_changed() {

  app_state_t* astate = app_state_t::get_inst();

  astate->log("DEBUG: object_inspector_widget_t::ui_cur_ws_selected_item_changed");

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          cur_ws->set_selected_item(size_t(ws_items_list->currentRow()), true);

          if (ws_items_list->currentRow() != -1) {
              ws_items_list->blockSignals(true);
              ws_items_list->item(ws_items_list->currentRow())->setSelected(true);
              ws_items_list->blockSignals(false);
            }

          astate->make_viewport_dirty();
        }

    }

  update_ws_items_view_widget();

}

void object_inspector_widget_t::need_to_update_obj_insp_received() {

  if (m_cur_obj_insp_widget) {
      m_cur_obj_insp_widget->update_from_ws_item();
    }

}

void object_inspector_widget_t::ws_item_list_double_clicked(QListWidgetItem *item) {

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->extended_editor_open_requested();

}
