#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_item_tab_widget_t *ws_item_obj_insp_widget_t::def_tab(QString tab_name,
                                                         QString icon_name_enabled,
                                                         QString icon_name_disabled) {

  ws_item_tab_widget_t *tmp = new ws_item_tab_widget_t;

  tmp->tab_top_wdgt = new QWidget(nullptr);
  tmp->tab_top_wdgt_lt = new QVBoxLayout;
  tmp->tab_top_wdgt->setLayout(tmp->tab_top_wdgt_lt);
  tmp->tab_top_wdgt_lt->setContentsMargins(0, 0, 0, 3);
  tmp->tab_top_wdgt_lt->setSpacing(0);

  tmp->tab_scroll = new QScrollArea;
  tmp->tab_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  tmp->tab_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tmp->tab_scroll->setWidgetResizable(true);
  // todo: move to qss
  tmp->tab_scroll->setStyleSheet("QScrollArea {border:0;}");

  tmp->tab_inner_widget = new QFrame(tmp->tab_scroll);
  tmp->tab_scroll->setWidget(tmp->tab_inner_widget);

  tmp->tab_inner_widget_lt = new QVBoxLayout;
  tmp->tab_inner_widget_lt->setContentsMargins(0, 0, 0, 3);
  tmp->tab_inner_widget_lt->setSizeConstraint(QLayout::SetMinAndMaxSize);
  tmp->tab_inner_widget_lt->setSpacing(0);
  tmp->tab_inner_widget->setLayout(tmp->tab_inner_widget_lt);

  tmp->tab_top_wdgt_lt->addWidget(tmp->tab_scroll);

  tmp->tab_id = addTab(tmp->tab_top_wdgt, "");

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

      m_ws_item_pos->bind_value(&m_binded_item->m_pos, m_binded_item);
      m_ws_item_show_item_bb->bind_value({&m_binded_item->m_is_visible, &m_binded_item->m_show_bb});

      qt_hlp::form_lt_ctrl_visibility(m_binded_item->get_flags() & ws_item_flags_support_tr,
                                      m_tg_form_lt,
                                      m_ws_item_type,
                                      m_ws_item_pos_label,
                                      m_ws_item_pos);

    }

  update_from_ws_item();

}

void ws_item_obj_insp_widget_t::unbind_item() {

  if (m_binded_item) {
      m_binded_item->m_last_tab = currentIndex();
    }

  m_binded_item = nullptr;
  m_ws_item_show_item_bb->unbind_value();
  m_ws_item_pos->unbind_value();

}

void ws_item_obj_insp_widget_t::update_from_ws_item() {

  const int trc_name = 25;
  const int trc_type = 25;

  if (m_binded_item) {

      auto name_fstr = QString::fromStdString(m_binded_item->get_name());
      auto name_qstr = qt_hlp::clamp_string(name_fstr, trc_name);
      m_ws_item_name->setText(name_fstr);
      m_ws_item_name->setToolTip(name_qstr);

      auto name_tstr =
          qt_hlp::clamp_string(QString::fromStdString(m_binded_item->compose_type_descr()),
                               trc_type);
      m_ws_item_type->setText(name_tstr);

      m_ws_item_show_item_bb->load_value();
      m_ws_item_pos->load_value_ex();

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
  qt_hlp::resize_form_lt_lbls(gb_lt, astate->size_guide.obj_insp_lbl_w());

}

void ws_item_obj_insp_widget_t::init_form_lt(QFormLayout *frm_lt) {

  app_state_t *astate = app_state_t::get_inst();
  qt_hlp::resize_form_lt_lbls(frm_lt, astate->size_guide.obj_insp_lbl_w());
  frm_lt->setSpacing(1);

}

void ws_item_obj_insp_widget_t::init_form_lt_lbl(QLabel *_label) {

  app_state_t *astate = app_state_t::get_inst();
  qt_hlp::resize_form_lt_lbl(_label, astate->size_guide.obj_insp_lbl_w());

}

ws_item_obj_insp_widget_t::ws_item_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setIconSize(QSize(26,26));
  m_tab_general = def_tab(tr("General settings of workspace item"),
                        "://images/settings.svg");

  //begin group box Item information
  m_sp_info_wdgt = new qspoiler_widget_t(tr("Item Information"));
  m_tg_form_lt = new QFormLayout;

  //pre_init_group_box(tg_info_widget, tg_form_layout);
  m_sp_info_wdgt->add_content_layout(m_tg_form_lt);

  m_ws_item_name = new QLabel;
  m_ws_item_type = new QLabel;

  m_ws_item_show_item_bb = new qbinded_bool_named_vector_t({tr("Item"), tr("BBox")});

  m_ws_item_pos_label = new QLabel(tr("Position[%1]").arg(astate->m_spatial_suffix));
  m_ws_item_pos = new qbinded_float3_input_t;
  m_ws_item_pos->m_updated_externally_event = true;
  m_ws_item_pos->m_upd_flag = ws_item_updf_pos_changed;

  m_ws_item_pos->set_min_max_step(-10000, 10000, 0.01);

  m_tg_form_lt->addRow(tr("Name"), m_ws_item_name);
  m_tg_form_lt->addRow(tr("Type"), m_ws_item_type);
  m_tg_form_lt->addRow(m_ws_item_pos_label, m_ws_item_pos);
  m_tg_form_lt->addRow(tr("Show"), m_ws_item_show_item_bb);

  init_form_lt(m_tg_form_lt);

  //end group box Item information

  //Begin group box Item actions
  m_tg_acts = new qspoiler_widget_t(tr("Item Actions"));

  m_tg_acts_layout = new QGridLayout;
  m_tg_acts_layout->setContentsMargins(5, 0, 5, 0);
  m_tg_acts->add_content_layout(m_tg_acts_layout);

  m_tg_acts_delete = new QPushButton(tr("Delete"));
  connect(m_tg_acts_delete,
          &QPushButton::pressed,
          this,
          &ws_item_obj_insp_widget_t::delete_current_item);

  m_tg_acts_rename = new QPushButton(tr("Rename"));
  connect(m_tg_acts_rename,
          &QPushButton::pressed,
          this,
          &ws_item_obj_insp_widget_t::rename_current_item);

  m_tg_acts_clone = new QPushButton(tr("Clone"));
  m_tg_acts_clone->setEnabled(false);

  m_tg_acts_layout->addWidget(m_tg_acts_delete, 0, 0);
  m_tg_acts_layout->addWidget(m_tg_acts_rename, 0, 1);
  m_tg_acts_layout->addWidget(m_tg_acts_clone,  0, 2);

  //tg_actions->setMaximumHeight(90);
  //end group box Item actions

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_position_changed_signal,
          this,
          &ws_item_obj_insp_widget_t::cur_ws_selected_item_position_changed);

  m_tab_general->tab_inner_widget_lt->addWidget(m_sp_info_wdgt);
  m_tab_general->tab_inner_widget_lt->addWidget(m_tg_acts);

  connect(this,
          &ws_item_obj_insp_widget_t::currentChanged,
          this,
          &ws_item_obj_insp_widget_t::cur_tab_changed);

}

void ws_item_obj_insp_widget_t::cur_ws_selected_item_position_changed() {

  if (m_binded_item) {
      if (m_binded_item->get_flags() & ws_item_flags_support_tr) {
          m_ws_item_pos->load_value_ex();
        }
    }

}

void ws_item_obj_insp_widget_t::rename_current_item() {

  if (m_binded_item) {

      app_state_t* astate = app_state_t::get_inst();
      bool ok;
      QString text = QInputDialog::getText(this,
                                           tr("Workspace item -> Rename"),
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

      astate->ws_mgr->get_cur_ws()->unsel_all();
      _binded_item->m_marked_for_deletion = true;

      astate->astate_evd->cur_ws_changed();
      astate->astate_evd->wss_changed();

    } else {

      astate->ws_mgr->get_cur_ws()->unsel_all();
      astate->astate_evd->cur_ws_changed();
      astate->astate_evd->wss_changed();

    }

}

void ws_item_obj_insp_widget_t::cur_tab_changed(int index) {

  if (m_binded_item) {
      m_binded_item->m_last_tab = index;
    }

}

