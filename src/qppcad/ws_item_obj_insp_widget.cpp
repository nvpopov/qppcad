#include <qppcad/ws_item_obj_insp_widget.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_item_tab_widget_t *ws_item_obj_insp_widget_t::define_tab(QString tab_name) {

  ws_item_tab_widget_t *tmp = new ws_item_tab_widget_t;
  tmp->tab_scroll = new QScrollArea;
  tmp->tab_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  tmp->tab_scroll->setWidgetResizable(true);

  tmp->tab_inner_widget = new QWidget(tmp->tab_scroll);
  tmp->tab_inner_widget->setProperty("s_class", "tab_inner_widget");
  tmp->tab_scroll->setWidget(tmp->tab_inner_widget);

  tmp->tab_inner_widget_layout = new QVBoxLayout;
  tmp->tab_inner_widget_layout->setContentsMargins(3,3,3,3);
  tmp->tab_inner_widget_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
  tmp->tab_inner_widget->setLayout(tmp->tab_inner_widget_layout);

  addTab(tmp->tab_scroll, tab_name);

  return tmp;
}

void ws_item_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  m_binded_item = _binding_item;

  if (m_binded_item) {

      int target_index = -1;
      if (m_binded_item->m_last_tab >= 0) target_index = m_binded_item->m_last_tab;
      else target_index = 0;

      auto *widget_to_focus = widget(target_index);

      if (widget_to_focus) {
          widget_to_focus->setFocus();
          setCurrentIndex(target_index);
        }

      if (m_binded_item->get_flags() & ws_item_flags_support_tr) {
          ws_item_pos->bind_value(&m_binded_item->m_pos, m_binded_item);
          ws_item_pos->show();
          ws_item_pos_label->show();
        } else {
          ws_item_pos->hide();
          ws_item_pos_label->hide();
        }

      if (m_binded_item->get_flags() & ws_item_flags_support_render_bb) {
          ws_item_bb_visible->bind_value(&m_binded_item->m_show_bb);
          ws_item_bb_visible->show();
          ws_item_bb_visible_label->show();
        } else {
          ws_item_bb_visible->hide();
          ws_item_bb_visible_label->hide();
        }

    }

  update_from_ws_item();

}

void ws_item_obj_insp_widget_t::unbind_item() {

  if (m_binded_item) {
      m_binded_item->m_last_tab = currentIndex();
    }

  m_binded_item = nullptr;
  ws_item_is_visible->unbind_value();
  ws_item_pos->unbind_value();
  ws_item_bb_visible->unbind_value();

}

void ws_item_obj_insp_widget_t::update_from_ws_item() {

  if (m_binded_item) {
      ws_item_name->setText(QString::fromStdString(m_binded_item->get_name()));
      ws_item_type->setText(QString::fromStdString(m_binded_item->compose_item_name()));
      ws_item_is_visible->bind_value(&m_binded_item->m_is_visible);
    }

}

void ws_item_obj_insp_widget_t::pre_init_group_box(QGroupBox *gb, QFormLayout *gb_lt) {
  gb->setLayout(gb_lt);
}

void ws_item_obj_insp_widget_t::post_init_group_box(QGroupBox *gb, QFormLayout *gb_lt) {
  qt_helpers::resize_form_lt_labels(gb_lt, def_label_width);
}

ws_item_obj_insp_widget_t::ws_item_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  tab_general = define_tab(tr("General"));

  //begin group box Item information
  tg_info_widget = new QGroupBox(tr("Item information"));
  tg_form_layout = new QFormLayout;
  pre_init_group_box(tg_info_widget, tg_form_layout);

  ws_item_name = new QLabel;
  ws_item_type = new QLabel;
  ws_item_is_visible_label = new QLabel(tr("Is visible"));
  ws_item_is_visible = new qbinded_checkbox;

  ws_item_bb_visible_label = new QLabel(tr("Show bbox"));
  ws_item_bb_visible = new qbinded_checkbox;

  ws_item_pos_label = new QLabel(tr("Position"));
  ws_item_pos = new qbinded_float3_input;
  ws_item_pos->set_default_suffix();
  ws_item_pos->set_min_max_step(-10000, 10000, 0.01);

  tg_form_layout->addRow(tr("Name"), ws_item_name);
  tg_form_layout->addRow(tr("Type"), ws_item_type);
  tg_form_layout->addRow(ws_item_is_visible_label, ws_item_is_visible);
  tg_form_layout->addRow(ws_item_bb_visible_label, ws_item_bb_visible);
  tg_form_layout->addRow(ws_item_pos_label, ws_item_pos);

  post_init_group_box(tg_info_widget, tg_form_layout);
  //end group box Item information

  //Begin group box Item actions
  tg_actions = new QGroupBox(tr("Item actions"));
  tg_actions_layout = new QHBoxLayout;
  tg_actions->setLayout(tg_actions_layout);
  tg_actions_delete = new QPushButton(tr("Delete"));
  connect(tg_actions_delete, SIGNAL(pressed()), this, SLOT(delete_current_item()));

  tg_actions_rename = new QPushButton(tr("Rename"));
  connect(tg_actions_rename, SIGNAL(pressed()), this, SLOT(rename_current_item()));

  tg_actions_clone = new QPushButton(tr("Clone"));
  tg_actions_layout->addWidget(tg_actions_delete);
  tg_actions_layout->addWidget(tg_actions_rename);
  tg_actions_layout->addWidget(tg_actions_clone);
  tg_actions->setMaximumHeight(90);
  //end group box Item actions

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_position_changed_signal,
          this, &ws_item_obj_insp_widget_t::cur_ws_selected_item_position_changed);

  tab_general->tab_inner_widget_layout->addWidget(tg_info_widget);
  tab_general->tab_inner_widget_layout->addWidget(tg_actions);

  connect(this, &ws_item_obj_insp_widget_t::currentChanged,
          this, &ws_item_obj_insp_widget_t::cur_tab_changed);
}

void ws_item_obj_insp_widget_t::cur_ws_selected_item_position_changed() {
  if (m_binded_item) {
      if (m_binded_item->get_flags() & ws_item_flags_support_tr) {
          ws_item_pos->load_value();
        }
    }
}

void ws_item_obj_insp_widget_t::rename_current_item() {

  if (m_binded_item) {
      app_state_t* astate = app_state_t::get_inst();
      bool ok;
      QString text = QInputDialog::getText(this, tr("Workspace item-> Rename"),
                                           tr("User name:"), QLineEdit::Normal,
                                           QString::fromStdString(m_binded_item->m_name), &ok);
      if (ok && text != "") {
          m_binded_item->m_name = text.toStdString();
          astate->astate_evd->cur_ws_selected_item_changed();
        }
    }
}

void ws_item_obj_insp_widget_t::delete_current_item() {

  if (m_binded_item) {
      app_state_t* astate = app_state_t::get_inst();
      m_binded_item->m_marked_for_deletion = true;
      unbind_item();
      astate->ws_manager->get_cur_ws()->unselect_all();
      astate->astate_evd->cur_ws_changed();
      astate->astate_evd->wss_changed();
    }
}

void ws_item_obj_insp_widget_t::cur_tab_changed(int index) {
  if (m_binded_item) {
      m_binded_item->m_last_tab = index;
    }
}
