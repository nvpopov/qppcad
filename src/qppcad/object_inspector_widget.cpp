#include <qppcad/object_inspector_widget.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/ws_comp_chem_data.hpp>

using namespace qpp;
using namespace qpp::cad;

object_inspector_widget_t::object_inspector_widget_t() {

  app_state_t* astate = app_state_t::get_inst();

  obj_insp_layout = new QVBoxLayout;
  setLayout(obj_insp_layout);
  ws_items_label = new QLabel;
  ws_items_label->setText(tr("Workspace items:"));
  //  ws_items_gb = new QGroupBox;
  //  ws_items_gb->setTitle(tr("Workspace items:"));
  //  //ws_items_gb->setMaximumHeight(340);
  //  ws_items_gb_layout = new QHBoxLayout;
  //  ws_items_gb->setStyleSheet("QGroupBox::title {"
  //                             "subcontrol-origin: margin;"
  //                             "subcontrol-position: top center;"
  //                             "padding-left: 10px;"
  //                             "padding-right: 10px;"
  //                             "padding-top: 0px;"
  //                             "margin-bottom:15px;}");
  //  ws_items_gb->setFlat(false);
  //  ws_items_gb->setLayout(ws_items_gb_layout);
  ws_items_list = new QListWidget;
  ws_items_list->setMaximumHeight(300);
  ws_items_list->setStyleSheet(" QListWidget::item { margin: 6px; }");

  none_item_placeholder = new QWidget;
  none_item_placeholder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //  property_label = new QLabel;
  //  property_label->setText(tr("Properties:"));

  obj_insp_layout->addWidget(ws_items_label);
  obj_insp_layout->addWidget(ws_items_list);
  obj_insp_layout->addWidget(none_item_placeholder);

  //obj_insp_layout->addWidget(property_label, 0, Qt::AlignTop);


  obj_insp_layout->setContentsMargins(0,0,15,15);

  connect(astate->astate_evd,
          SIGNAL(current_workspace_selected_item_changed_signal()),
          this, SLOT(current_workspace_selected_item_changed()));

  connect(astate->astate_evd,
          SIGNAL(current_workspace_changed_signal()),
          this, SLOT(current_workspace_changed()));

  connect(ws_items_list, &QListWidget::itemSelectionChanged,
          this, &object_inspector_widget_t::ui_current_workspace_selected_item_changed);

  //ws_item_view = new ws_item_obj_insp_widget_t;
  ws_atoms_list_view = new ws_atoms_list_obj_insp_widget_t;
  ws_comp_chem_data_view = new ws_comp_chem_data_obj_insp_widget_t;
  ws_current_view = none_item_placeholder;

  current_workspace_changed();
  ui_current_workspace_selected_item_changed();
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
    }

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {

          if (dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected())) {
              if (ws_current_view)  {
                  obj_insp_layout->removeWidget(ws_current_view);
                  ws_current_view->setParent(nullptr);
                }
              none_item_placeholder->hide();
              obj_insp_layout->addWidget(ws_atoms_list_view);
              ws_current_view = ws_atoms_list_view;
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
              astate->log("DEBUG: ws_current_view = ws_comp_chem_data_view;");
              return;
            }
        }
    }


}

void object_inspector_widget_t::current_workspace_changed() {

  app_state_t* astate = app_state_t::get_inst();
  astate->log("DEBUG: object_inspector_widget_t::current_workspace_changed");

  ws_items_list->blockSignals(true);

  ws_items_list->clear();
  ws_items_list->clearSelection();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          for (size_t i = 0; i < cur_ws->m_ws_items.size(); i++) {
              ws_items_list->addItem(QString::fromStdString(cur_ws->m_ws_items[i]->m_name));
            }
        }
    }

  current_workspace_selected_item_changed();
  ws_items_list->blockSignals(false);
}

void object_inspector_widget_t::current_workspace_selected_item_changed() {

  app_state_t* astate = app_state_t::get_inst();
  astate->log("DEBUG: object_inspector_widget_t::current_workspace_selected_item_changed");
  ws_items_list->blockSignals(true);

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
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

void object_inspector_widget_t::ui_current_workspace_selected_item_changed() {

  app_state_t* astate = app_state_t::get_inst();
  astate->log("DEBUG: object_inspector_widget_t::ui_current_workspace_selected_item_changed");
  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          cur_ws->set_selected_item(ws_items_list->currentRow(), false);

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
