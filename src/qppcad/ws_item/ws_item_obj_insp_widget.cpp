#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_item_tab_widget_t *ws_item_obj_insp_widget_t::def_tab(QString tab_name,
                                                            QString icon_name_enabled,
                                                            QString icon_name_disabled) {

  ws_item_tab_widget_t *tmp = new ws_item_tab_widget_t;
  tmp->tab_scroll = new QScrollArea;
  tmp->tab_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  tmp->tab_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tmp->tab_scroll->setWidgetResizable(true);
  tmp->tab_scroll->setStyleSheet("QScrollArea {border:0;}");

  tmp->tab_inner_widget = new QFrame(tmp->tab_scroll);
  //tmp->tab_inner_widget->setStyleSheet("QFrame {border: 0;}");
  tmp->tab_inner_widget->setProperty("s_class", "tab_inner_widget");
  tmp->tab_scroll->setWidget(tmp->tab_inner_widget);

  tmp->tab_inner_widget_lt = new QVBoxLayout;
  tmp->tab_inner_widget_lt->setContentsMargins(3,3,3,3);
  tmp->tab_inner_widget_lt->setSizeConstraint(QLayout::SetMinAndMaxSize);
  tmp->tab_inner_widget->setLayout(tmp->tab_inner_widget_lt);

  tmp->tab_id = addTab(tmp->tab_scroll, "");
  setTabToolTip(tmp->tab_id, tab_name);
  tmp->icon_enabled = new QIcon(icon_name_enabled);
  if (icon_name_disabled != "") tmp->icon_disabled = new QIcon(icon_name_disabled);
  setTabIcon(tmp->tab_id, *tmp->icon_enabled);

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

      ws_item_pos->bind_value(&m_binded_item->m_pos, m_binded_item);
      ws_item_bb_visible->bind_value(&m_binded_item->m_show_bb);
      ws_item_is_visible->bind_value(&m_binded_item->m_is_visible);

      qt_helpers::form_lt_ctrl_visibility(m_binded_item->get_flags() & ws_item_flags_support_tr,
                                          tg_form_layout,
                                          ws_item_type,
                                          ws_item_is_visible_label,
                                          ws_item_is_visible);

      qt_helpers::form_lt_ctrl_visibility(m_binded_item->get_flags() & ws_item_flags_support_tr,
                                          tg_form_layout,
                                          ws_item_is_visible,
                                          ws_item_bb_visible_label,
                                          ws_item_bb_visible);

      qt_helpers::form_lt_ctrl_visibility(m_binded_item->get_flags() & ws_item_flags_support_tr,
                                          tg_form_layout,
                                          ws_item_bb_visible,
                                          ws_item_pos_label,
                                          ws_item_pos);

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

  const int trc = 22;

  if (m_binded_item) {

      std::string name_str = m_binded_item->get_name();
      QString name_truncated = QString::fromStdString(name_str).left(trc);
      if (name_str.length() > trc) name_truncated += "...";
      ws_item_name->setText(name_truncated);

      std::string type_str = m_binded_item->compose_type_descr();
      QString type_truncated = QString::fromStdString(type_str).left(trc);
      if (type_str.length() > trc) type_truncated += "...";
      ws_item_type->setText(type_truncated);

      ws_item_is_visible->load_value_ex();
      ws_item_bb_visible->load_value_ex();
      ws_item_pos->load_value_ex();

    }

}

void ws_item_obj_insp_widget_t::set_tab_enabled(ws_item_tab_widget_t *tab,
                                                bool v_enabled) {
  if (!tab) return;
  setTabEnabled(tab->tab_id, v_enabled);

  if (!v_enabled && tab->icon_disabled) setTabIcon(tab->tab_id, *tab->icon_disabled);
  else setTabIcon(tab->tab_id, *tab->icon_enabled);

}

void ws_item_obj_insp_widget_t::pre_init_gb(QGroupBox *gb, QFormLayout *gb_lt) {
  gb->setLayout(gb_lt);
}

void ws_item_obj_insp_widget_t::post_init_gb(QGroupBox *gb, QFormLayout *gb_lt) {

  app_state_t *astate = app_state_t::get_inst();
  qt_helpers::resize_form_lt_lbls(gb_lt, astate->size_guide.obj_insp_lbl_w());

}

void ws_item_obj_insp_widget_t::init_form_lt(QFormLayout *frm_lt) {

  app_state_t *astate = app_state_t::get_inst();
  qt_helpers::resize_form_lt_lbls(frm_lt, astate->size_guide.obj_insp_lbl_w());

}

void ws_item_obj_insp_widget_t::init_form_lt_lbl(QLabel *_label) {

  app_state_t *astate = app_state_t::get_inst();
  qt_helpers::resize_form_lt_lbl(_label, astate->size_guide.obj_insp_lbl_w());

}

ws_item_obj_insp_widget_t::ws_item_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setIconSize(QSize(32,32));
  tab_general = def_tab(tr("General settings of workspace item"),
                           "://images/settings.svg");

  //begin group box Item information
  sp_info_widget = new qspoiler_widget_t(tr("Item information"));
  tg_form_layout = new QFormLayout;

  //pre_init_group_box(tg_info_widget, tg_form_layout);
  sp_info_widget->add_content_layout(tg_form_layout);

  ws_item_name = new QLabel;
  ws_item_type = new QLabel;
  ws_item_is_visible_label = new QLabel(tr("Is visible"));
  ws_item_is_visible = new qbinded_checkbox_t;

  ws_item_bb_visible_label = new QLabel(tr("Show bbox"));
  ws_item_bb_visible = new qbinded_checkbox_t;

  ws_item_pos_label = new QLabel(tr("Position[%1]").arg(astate->m_spatial_suffix));
  ws_item_pos = new qbinded_float3_input_t;
  ws_item_pos->m_updated_externally_event = true;
  ws_item_pos->m_upd_flag = ws_item_updf_pos_changed;

  ws_item_pos->set_min_max_step(-10000, 10000, 0.01);

  tg_form_layout->addRow(tr("Name"), ws_item_name);
  tg_form_layout->addRow(tr("Type"), ws_item_type);

  init_form_lt_lbl(ws_item_pos_label);
  init_form_lt_lbl(ws_item_bb_visible_label);
  init_form_lt_lbl(ws_item_is_visible_label);

  init_form_lt(tg_form_layout);

  //end group box Item information

  //Begin group box Item actions
  tg_actions = new qspoiler_widget_t(tr("Item actions"));

  tg_actions_layout = new QHBoxLayout;
  tg_actions_layout->setContentsMargins(5, 0, 5, 0);
  tg_actions->add_content_layout(tg_actions_layout);

  tg_actions_delete = new QPushButton(tr("Delete"));
  connect(tg_actions_delete,
          &QPushButton::pressed,
          this,
          &ws_item_obj_insp_widget_t::delete_current_item);

  tg_actions_rename = new QPushButton(tr("Rename"));
  connect(tg_actions_rename,
          &QPushButton::pressed,
          this,
          &ws_item_obj_insp_widget_t::rename_current_item);

  tg_actions_clone = new QPushButton(tr("Clone"));
  tg_actions_clone->setEnabled(false);

  tg_actions_layout->addWidget(tg_actions_delete);
  tg_actions_layout->addWidget(tg_actions_rename);
  tg_actions_layout->addWidget(tg_actions_clone);
  tg_actions->setMaximumHeight(90);
  //end group box Item actions

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_position_changed_signal,
          this,
          &ws_item_obj_insp_widget_t::cur_ws_selected_item_position_changed);

  tab_general->tab_inner_widget_lt->addWidget(sp_info_widget);
  tab_general->tab_inner_widget_lt->addWidget(tg_actions);

  connect(this,
          &ws_item_obj_insp_widget_t::currentChanged,
          this,
          &ws_item_obj_insp_widget_t::cur_tab_changed);

}

void ws_item_obj_insp_widget_t::cur_ws_selected_item_position_changed() {

  if (m_binded_item) {
      if (m_binded_item->get_flags() & ws_item_flags_support_tr) {
          ws_item_pos->load_value_ex();
        }
    }

}

void ws_item_obj_insp_widget_t::rename_current_item() {

  if (m_binded_item) {
      app_state_t* astate = app_state_t::get_inst();
      bool ok;
      QString text = QInputDialog::getText(this,
                                           tr("Workspace item-> Rename"),
                                           tr("User name:"),
                                           QLineEdit::Normal,
                                           QString::fromStdString(m_binded_item->m_name),
                                           &ok);
      if (ok && text != "") {
          m_binded_item->m_name = text.toStdString();
          astate->astate_evd->cur_ws_selected_item_changed();
          astate->astate_evd->cur_ws_changed();
        }
    }

}

void ws_item_obj_insp_widget_t::delete_current_item() {

  app_state_t* astate = app_state_t::get_inst();

  if (m_binded_item) {

      int ret = QMessageBox::warning(this,
                                     tr("Confirm the deletion"),
                                     tr("Are you sure?"),
                                     QMessageBox::Ok | QMessageBox::Cancel);

      if (ret == QMessageBox::Cancel) return;

      ws_item_t *_binded_item = m_binded_item;
      unbind_item();

      astate->ws_mgr->get_cur_ws()->unselect_all();
      _binded_item->m_marked_for_deletion = true;

      astate->astate_evd->cur_ws_changed();
      astate->astate_evd->wss_changed();

    } else {

      astate->ws_mgr->get_cur_ws()->unselect_all();
      astate->astate_evd->cur_ws_changed();
      astate->astate_evd->wss_changed();

    }

}

void ws_item_obj_insp_widget_t::cur_tab_changed(int index) {

  if (m_binded_item) {
      m_binded_item->m_last_tab = index;
    }

}

