#include <qppcad/object_inspector_widget.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/ws_comp_chem_data/ws_comp_chem_data.hpp>

using namespace qpp;
using namespace qpp::cad;

object_inspector_widget_t::object_inspector_widget_t() {

  app_state_t* astate = app_state_t::get_inst();

  obj_insp_layout = new QVBoxLayout;
  setLayout(obj_insp_layout);
  ws_items_label = new QLabel;
  ws_items_label->setText(tr("Workspace items:"));

  ws_items_list = new QListWidget;
  ws_items_list->setMaximumHeight(250);
  ws_items_list->setStyleSheet(" QListWidget::item { margin: 6px; }");
  ws_items_list->setFocusPolicy(Qt::NoFocus);

  none_item_placeholder = new QWidget;
  none_item_placeholder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  obj_insp_layout->addWidget(ws_items_label);
  obj_insp_layout->addWidget(ws_items_list);
  obj_insp_layout->addWidget(none_item_placeholder);
  obj_insp_layout->setContentsMargins(0,0,15,15);

  connect(astate->astate_evd,
          SIGNAL(cur_ws_selected_item_changed_signal()),
          this, SLOT(cur_ws_selected_item_changed()));

  connect(astate->astate_evd,
          SIGNAL(cur_ws_changed_signal()),
          this, SLOT(cur_ws_changed()));

  connect(ws_items_list, &QListWidget::itemSelectionChanged,
          this, &object_inspector_widget_t::ui_cur_ws_selected_item_changed);

  //ws_item_view = new ws_item_obj_insp_widget_t;
  ws_atoms_list_view = new ws_atoms_list_obj_insp_widget_t;
  ws_comp_chem_data_view = new ws_comp_chem_data_obj_insp_widget_t;
  ws_volume_data_view = new ws_volume_data_obj_insp_widget_t;

  ws_current_view = none_item_placeholder;

  cur_ws_changed();
  ui_cur_ws_selected_item_changed();
  ws_items_list->clearSelection();

}

void object_inspector_widget_t::update_ws_items_view_widget() {

  app_state_t* astate = app_state_t::get_inst();

  if (ws_current_view) {
      obj_insp_layout->removeWidget(ws_current_view);
      ws_current_view->setParent(nullptr);
      ws_current_view = none_item_placeholder;
      obj_insp_layout->addWidget(none_item_placeholder);
      none_item_placeholder->show();
      ws_comp_chem_data_view->unbind_item();
      ws_comp_chem_data_view->hide();
      ws_atoms_list_view->unbind_item();
      ws_atoms_list_view->hide();
      ws_volume_data_view->unbind_item();
      ws_volume_data_view->hide();
    }

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {

          if (dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected())) {

              if (ws_current_view)  {
                  obj_insp_layout->removeWidget(ws_current_view);
                  ws_current_view->setParent(nullptr);
                }

              none_item_placeholder->hide();
              obj_insp_layout->addWidget(ws_atoms_list_view);
              ws_current_view = ws_atoms_list_view;

              ws_atoms_list_view->bind_to_item(cur_ws->get_selected());
              ws_atoms_list_view->show();

              ws_comp_chem_data_view->unbind_item();
              ws_volume_data_view->unbind_item();

              astate->log("DEBUG: ws_current_view = ws_atoms_list_view;");
              return;
            }

          if (dynamic_cast<ws_comp_chem_data_t*>(cur_ws->get_selected())) {

              if (ws_current_view)  {
                  obj_insp_layout->removeWidget(ws_current_view);
                  ws_current_view->setParent(nullptr);
                }

              none_item_placeholder->hide();
              obj_insp_layout->addWidget(ws_comp_chem_data_view);
              ws_current_view = ws_comp_chem_data_view;

              ws_comp_chem_data_view->bind_to_item(cur_ws->get_selected());
              ws_comp_chem_data_view->show();

              ws_atoms_list_view->unbind_item();
              ws_volume_data_view->unbind_item();

              astate->log("DEBUG: ws_current_view = ws_comp_chem_data_view;");
              return;
            }

          if (dynamic_cast<ws_volume_data_t*>(cur_ws->get_selected())) {

              if (ws_current_view)  {
                  obj_insp_layout->removeWidget(ws_current_view);
                  ws_current_view->setParent(nullptr);
                }

              none_item_placeholder->hide();
              obj_insp_layout->addWidget(ws_volume_data_view);
              ws_current_view = ws_volume_data_view;

              ws_volume_data_view->bind_to_item(cur_ws->get_selected());
              ws_volume_data_view->show();

              ws_atoms_list_view->unbind_item();
              ws_comp_chem_data_view->unbind_item();

              astate->log("DEBUG: ws_current_view = ws_comp_chem_data_view;");
              return;
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
              ws_items_list->addItem(QString::fromStdString(cur_ws->m_ws_items[i]->m_name));
            }
        }
    }

  cur_ws_selected_item_changed();
  ws_items_list->blockSignals(false);

}

void object_inspector_widget_t::cur_ws_selected_item_changed() {

  app_state_t* astate = app_state_t::get_inst();

  astate->log("DEBUG: object_inspector_widget_t::cur_ws_selected_item_changed");

  ws_items_list->blockSignals(true);

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
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
          cur_ws->set_selected_item(ws_items_list->currentRow(), true);

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
