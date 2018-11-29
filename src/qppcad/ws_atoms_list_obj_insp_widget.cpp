#include <qppcad/ws_atoms_list_obj_insp_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void ws_atoms_list_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  auto _tmp = dynamic_cast<ws_atoms_list_t*>(_binding_item);

  if (_tmp) {
      b_al = _tmp;
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
}

void ws_atoms_list_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_al) {

      //General tab
      tg_geom_summary_total_atoms->setText(
            QString::fromStdString(fmt::format("{}", b_al->m_geom->nat())));
      tg_geom_summary_total_types->setText(
            QString::fromStdString(fmt::format("{}", b_al->m_geom->n_atom_types())));

      //update type table
      tg_type_summary_table->clearContents();
      tg_type_summary_table->setRowCount(b_al->m_geom->n_atom_types());
      for (int i = 0; i < b_al->m_geom->n_atom_types(); i++) {

          QString n_name_str = QString::fromStdString(b_al->m_geom->atom_of_type(i));
          QTableWidgetItem *n_name = new QTableWidgetItem(n_name_str);
          n_name->setTextAlignment(Qt::AlignCenter);
          tg_type_summary_table->setItem(i,0, n_name);

          QString n_c_str = tr("%1").arg(b_al->m_geom->get_atom_count_by_type(i));
          QTableWidgetItem *n_c = new QTableWidgetItem(n_c_str);
          n_c->setTextAlignment(Qt::AlignCenter);
          tg_type_summary_table->setItem(i,1, n_c);

          QString n_clr_str = "";
          QTableWidgetItem *n_clr = new QTableWidgetItem(n_clr_str);
          n_clr->setTextAlignment(Qt::AlignCenter);

          auto ap_idx = ptable::number_by_symbol(b_al->m_geom->atom_of_type(i));
          vector3<float> bc(0.0, 0.0, 1.0);
          if (ap_idx) {bc = ptable::get_inst()->arecs[*ap_idx-1].aColorJmol;}
          QColor color_bck;
          color_bck.setRgbF(bc[0], bc[1], bc[2]);
          n_clr->setBackgroundColor(color_bck);
          tg_type_summary_table->setItem(i,2, n_clr);

        }

      //display tab

      display_settings_draw_atoms->bind_value(&b_al->m_show_atoms);
      display_settings_draw_bonds->bind_value(&b_al->m_show_bonds);
      display_settings_draw_img_atoms->bind_value(&b_al->m_show_imaginary_atoms);
      display_settings_draw_img_bonds->bind_value(&b_al->m_show_imaginary_bonds);
      display_settings_atom_scale->bind_value(&b_al->m_atom_scale_factor);
      display_settings_bond_scale->bind_value(&b_al->m_bond_scale_factor);
      display_settings_render_style->bind_value(
            reinterpret_cast<int*>(&b_al->m_cur_render_type));
      display_settings_labels_style->bind_value(
            reinterpret_cast<int*>(&b_al->m_labels->m_style));
      display_shading_spec->bind_value(&b_al->m_draw_specular);
      display_shading_spec_value->bind_value(&b_al->m_shading_specular_power);

      //anim bindings
      tabBar()->setTabEnabled(2, b_al->m_anim->animable());

      gb_anim_total_anims->setText(tr("%1").arg(b_al->m_anim->get_total_anims()));
      gb_rebuild_bonds->bind_value(&b_al->m_anim->m_rebuild_bonds_in_anim);
      gb_play_cyclic->bind_value(&b_al->m_anim->m_play_cyclic);
      gb_anim_speed->bind_value(&b_al->m_anim->m_anim_frame_time);

      //binding current anim combobox
      gb_current_anim->blockSignals(true);
      gb_current_anim->clear();

      for (int i = 0; i < b_al->m_anim->get_total_anims(); i++) {
          gb_current_anim->addItem(
                QString::fromStdString(b_al->m_anim->m_anim_data[i].m_anim_name));
        }

      gb_current_anim->setCurrentIndex(b_al->m_anim->m_cur_anim);
      gb_current_anim->blockSignals(false);
      update_anim_tab_gb();

      anim_play->blockSignals(true);
      anim_play->setChecked(b_al->m_anim->m_play_anim);
      anim_play->blockSignals(false);

      bt_model->bind(b_al);
    }
}

void ws_atoms_list_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();
  display_settings_draw_atoms->unbind_value();
  display_settings_draw_bonds->unbind_value();
  display_settings_draw_img_atoms->unbind_value();
  display_settings_draw_img_bonds->unbind_value();
  display_settings_atom_scale->unbind_value();
  display_settings_bond_scale->unbind_value();
  display_settings_render_style->unbind_value();
  display_settings_labels_style->unbind_value();
  display_shading_spec->unbind_value();
  display_shading_spec_value->unbind_value();
  gb_rebuild_bonds->unbind_value();
  gb_play_cyclic->unbind_value();
  gb_anim_speed->unbind_value();

  bt_model->unbind();

}

void ws_atoms_list_obj_insp_widget_t::update_anim_tab_gb() {

  if (b_al) {
      auto cur_anim = b_al->m_anim->get_current_anim();
      if (cur_anim && cur_anim->m_anim_type == geom_anim_type::anim_static) {
          gb_anim_buttons->hide();
          gb_anim_timeline->hide();
        }
      else {
          gb_anim_timeline->show();
          gb_anim_buttons->show();
        }
    }

}

ws_atoms_list_obj_insp_widget_t::ws_atoms_list_obj_insp_widget_t()
  : ws_item_obj_insp_widget_t() {

  //general tab

  tab_display = define_tab(tr("Display"));
  tab_animation = define_tab(tr("Animation"));
  tab_measurement = define_tab(tr("Measurement"));

  tg_geom_summary_widget = new QGroupBox;
  tg_geom_summary_layout = new QFormLayout;
  tg_geom_summary_layout->setLabelAlignment(Qt::AlignRight);
  tg_geom_summary_widget->setLayout(tg_geom_summary_layout);
  tg_geom_summary_widget->setTitle("Geometry summary");

  tg_geom_summary_total_atoms = new QLabel;
  tg_geom_summary_total_types = new QLabel;

  tg_geom_summary_layout->addRow(tr("Total atoms:"), tg_geom_summary_total_atoms);
  tg_geom_summary_layout->addRow(tr("Total atom types:"), tg_geom_summary_total_types);

  tg_type_summary_widget = new QGroupBox(tr("Type Summary"));
  tg_type_summary_layout = new QVBoxLayout;
  tg_type_summary_widget->setLayout(tg_type_summary_layout);
  tg_type_summary_table = new QTableWidget;
  tg_type_summary_table->setColumnCount(3);

  QStringList table_hdr;
  table_hdr.push_back("Name");
  table_hdr.push_back("Count");
  table_hdr.push_back("Color");

  tg_type_summary_table->setHorizontalHeaderLabels(table_hdr);
  tg_type_summary_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tg_type_summary_table->verticalHeader()->setVisible(false);
  tg_type_summary_table->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  tg_type_summary_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tg_type_summary_layout->addWidget(tg_type_summary_table);

  tab_general->tab_inner_widget_layout->addWidget(tg_geom_summary_widget);
  tab_general->tab_inner_widget_layout->addWidget(tg_type_summary_widget);

  //display tab
  gb_display_settings = new QGroupBox(tr("Draw settings"));
  gb_display_settings_layout = new QFormLayout;
  gb_display_settings_layout->setLabelAlignment(Qt::AlignRight);
  gb_display_settings->setLayout(gb_display_settings_layout);

  tab_display->tab_inner_widget_layout->addWidget(gb_display_settings);
  display_settings_draw_atoms = new qbinded_checkbox;
  display_settings_draw_bonds = new qbinded_checkbox;
  display_settings_draw_img_atoms = new qbinded_checkbox;
  display_settings_draw_img_bonds = new qbinded_checkbox;

  display_settings_atom_scale = new qbinded_float_spinbox;
  display_settings_atom_scale->set_min_max_step(0.01, 3.0, 0.01);
  display_settings_bond_scale = new qbinded_float_spinbox;
  display_settings_bond_scale->set_min_max_step(0.01, 3.0, 0.01);

  display_settings_render_style = new qbinded_combobox;
  display_settings_render_style->addItem("Balls and sticks");
  display_settings_render_style->addItem("Dynamic lines");
  display_settings_render_style->addItem("X-atom lines");
  display_settings_render_style->addItem("Billboards");
  display_settings_render_style->addItem("Buffered billboards");

  display_settings_labels_style = new qbinded_combobox;
  display_settings_labels_style->addItem("None");
  display_settings_labels_style->addItem("Id");
  display_settings_labels_style->addItem("Type");
  display_settings_labels_style->addItem("Type and Id");

  gb_display_settings_layout->addRow(tr("Draw style"), display_settings_render_style);
  gb_display_settings_layout->addRow(tr("Draw atoms"), display_settings_draw_atoms);
  gb_display_settings_layout->addRow(tr("Draw bonds"), display_settings_draw_bonds);
  gb_display_settings_layout->addRow(tr("Draw img. atoms"), display_settings_draw_img_atoms);
  gb_display_settings_layout->addRow(tr("Draw img. bonds"), display_settings_draw_img_bonds);
  gb_display_settings_layout->addRow(tr("Atom scale"), display_settings_atom_scale);
  gb_display_settings_layout->addRow(tr("Bond scale"), display_settings_bond_scale);
  gb_display_settings_layout->addRow(tr("Labels style"), display_settings_labels_style);

  //display - shading tab initialization
  gb_display_shading = new QGroupBox(tr("Shading settings"));
  gb_display_shading_layout = new QFormLayout;
  gb_display_shading->setLayout(gb_display_shading_layout);
  display_shading_spec = new qbinded_checkbox;
  display_shading_spec_value = new qbinded_float_spinbox;
  display_shading_spec_value->set_min_max_step(0.25, 50, 0.25);
  gb_display_shading_layout->addRow(tr("Enable specular"), display_shading_spec);
  gb_display_shading_layout->addRow(tr("Specular power"), display_shading_spec_value);

  //display - bonding table
  gb_display_bt = new QGroupBox(tr("Bonding table"));
  display_bt_layout = new QVBoxLayout;
  gb_display_bt->setLayout(display_bt_layout);
  bt_model = new qbonding_table_model_t;
  display_bt = new QTableView;
  display_bt->verticalHeader()->hide();
  display_bt->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  display_bt->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  display_bt->setModel(bt_model);
  display_bt_layout->addWidget(display_bt);

  tab_display->tab_inner_widget_layout->addWidget(gb_display_shading);
  tab_display->tab_inner_widget_layout->addWidget(gb_display_bt);

  //Anim tab
  gb_anim_summary = new QGroupBox(tr("Summary"));
  gb_anim_summary_layout = new QFormLayout;
  gb_anim_summary->setLayout(gb_anim_summary_layout);
  gb_anim_total_anims = new QLabel;
  gb_rebuild_bonds = new qbinded_checkbox;
  gb_play_cyclic = new qbinded_checkbox;
  gb_anim_speed = new qbinded_float_spinbox;
  gb_anim_speed->set_min_max_step(0.01,1.0,0.01);
  gb_anim_total_frames_in_anim = new QLabel;
  gb_anim_cur_frame = new QLabel;
  gb_current_anim = new QComboBox;

  connect(gb_current_anim, SIGNAL(currentIndexChanged(int)),
          this, SLOT(current_anim_index_changed(int)));

  gb_anim_summary_layout->addRow(tr("Number of animations"), gb_anim_total_anims);
  gb_anim_summary_layout->addRow(tr("Rebuild bonds"), gb_rebuild_bonds);
  gb_anim_summary_layout->addRow(tr("Play in cycle"), gb_play_cyclic);
  gb_anim_summary_layout->addRow(tr("Current animation"), gb_current_anim);
  gb_anim_summary_layout->addRow(tr("Frame time"), gb_anim_speed);
  gb_anim_summary_layout->addRow(tr("Total frames in anim."), gb_anim_total_frames_in_anim);
  gb_anim_summary_layout->addRow(tr("Current frame:"), gb_anim_cur_frame);

  gb_anim_timeline = new QGroupBox(tr("Timeline"));
  gb_anim_timeline_layout = new QHBoxLayout;
  gb_anim_timeline->setLayout(gb_anim_timeline_layout);


  gb_anim_timeline_slider = new QSlider(Qt::Orientation::Horizontal);
  gb_anim_timeline_slider->setTickPosition(QSlider::TicksBothSides);
  gb_anim_timeline_slider->setTickInterval(10);
  connect(gb_anim_timeline_slider, SIGNAL(valueChanged(int)),
          this, SLOT(anim_timeline_slider_value_changed(int)));
  //gb_anim_timeline_layout->addWidget(gb_anim_timeline_cur_frame, 1);
  gb_anim_timeline_layout->addWidget(gb_anim_timeline_slider, 1);


  gb_anim_buttons = new QGroupBox(tr("Control"));
  gb_anim_buttons_layout = new QHBoxLayout;
  gb_anim_buttons->setLayout(gb_anim_buttons_layout);
  gb_anim_buttons->setMaximumWidth(373);
  gb_anim_buttons->setMaximumHeight(90);

  anim_play = new QPushButton(tr("PLAY"));
  anim_play->setCheckable(true);
  connect(anim_play, SIGNAL(toggled(bool)), this, SLOT(play_anim_button_toggle(bool)));

  anim_to_start = new QPushButton(tr("<<"));
  connect(anim_to_start, SIGNAL(clicked()),
          this, SLOT(anim_button_begin_clicked()));

  anim_to_end = new QPushButton(tr(">>"));
  connect(anim_to_end, SIGNAL(clicked()),
          this, SLOT(anim_button_end_clicked()));

  anim_frame_forward = new QPushButton(tr("+F"));
  connect(anim_frame_forward, SIGNAL(clicked()),
          this, SLOT(anim_button_frame_move_forward_clicked()));

  anim_frame_backward = new QPushButton(tr("-F"));
  connect(anim_frame_backward, SIGNAL(clicked()),
          this, SLOT(anim_button_frame_move_backward_clicked()));

  gb_anim_buttons_layout->addWidget(anim_play, 1);
  gb_anim_buttons_layout->addWidget(anim_to_start, 1);
  gb_anim_buttons_layout->addWidget(anim_to_end, 1);
  gb_anim_buttons_layout->addWidget(anim_frame_forward, 1);
  gb_anim_buttons_layout->addWidget(anim_frame_backward, 1);

  tab_animation->tab_inner_widget_layout->addWidget(gb_anim_summary);
  tab_animation->tab_inner_widget_layout->addWidget(gb_anim_timeline);
  tab_animation->tab_inner_widget_layout->addWidget(gb_anim_buttons);

  tab_general->tab_inner_widget_layout->addStretch(0);
  tab_animation->tab_inner_widget_layout->addStretch(0);

  //connect astate dispatcher signals
  //current_workspace_selected_item_frame_changed
  app_state_t *astate = app_state_t::get_inst();

  connect(astate->astate_evd, SIGNAL(current_workspace_selected_item_frame_changed_signal()),
          this, SLOT(current_workspace_selected_item_frame_changed()));
}

void ws_atoms_list_obj_insp_widget_t::current_anim_index_changed(int index) {

  if (b_al) {

      if (index < b_al->m_anim->get_total_anims()) {
          //          fmt::print(std::cout, "DEBUG CHANGE CURRENT ANIM {} {}\n", index,
          //                     b_al->m_anim->m_cur_anim);
          b_al->m_anim->m_cur_anim = index;
          b_al->m_anim->m_cur_anim_time = 0.0f;
          b_al->m_anim->update_geom_to_anim();

          auto cur_anim = b_al->m_anim->get_current_anim();
          if (cur_anim && cur_anim->m_anim_type != geom_anim_type::anim_static) {
              anim_play->blockSignals(true);
              anim_play->setChecked(b_al->m_anim->m_play_anim);
              anim_play->blockSignals(false);
            } else {
              anim_play->blockSignals(true);
              anim_play->setChecked(false);
              anim_play->blockSignals(false);
            }

          update_anim_tab_gb();
          gb_anim_total_frames_in_anim->setText(tr("%1").arg(cur_anim->frame_data.size()));
          gb_anim_timeline_slider->setMinimum(0);
          gb_anim_timeline_slider->setMaximum(cur_anim->frame_data.size()-1);
        }

    }
}

void ws_atoms_list_obj_insp_widget_t::play_anim_button_toggle(bool value) {
  if (b_al) {
      b_al->m_anim->m_play_anim = value;
    }
}

void ws_atoms_list_obj_insp_widget_t::animation_updated_external() {
  if (b_al) {
      int current_frame_truncated = std::floor(b_al->m_anim->m_cur_anim_time);

      // remap frame id from 0..size-1 1..size
      gb_anim_cur_frame->setText(tr("%1").arg(current_frame_truncated+1));
      gb_anim_timeline_slider->blockSignals(true);
      gb_anim_timeline_slider->setValue(current_frame_truncated);
      gb_anim_timeline_slider->blockSignals(false);
    }
}

void ws_atoms_list_obj_insp_widget_t::current_workspace_selected_item_frame_changed() {
  animation_updated_external();
}

void ws_atoms_list_obj_insp_widget_t::anim_timeline_slider_value_changed(int value) {
  if (b_al) {
      auto cur_anim = b_al->m_anim->get_current_anim();
      if (cur_anim && value < cur_anim->frame_data.size()) {
          // gb_anim_timeline_slider->setValue(value);
          b_al->m_anim->update_and_set_anim(b_al->m_anim->m_cur_anim, value);
          //animation_updated_external();
        }
    }
}

void ws_atoms_list_obj_insp_widget_t::anim_button_begin_clicked() {
  if (b_al) b_al->m_anim->update_current_frame_to_begin();
}

void ws_atoms_list_obj_insp_widget_t::anim_button_end_clicked() {
  if (b_al) b_al->m_anim->update_current_frame_to_end();
}

void ws_atoms_list_obj_insp_widget_t::anim_button_frame_move_forward_clicked() {
  if (b_al) b_al->m_anim->manual_frame_manipulate(1);
}

void ws_atoms_list_obj_insp_widget_t::anim_button_frame_move_backward_clicked() {
  if (b_al) b_al->m_anim->manual_frame_manipulate(-1);
}
