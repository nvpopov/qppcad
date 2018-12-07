#include <qppcad/ws_atoms_list_obj_insp_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void ws_atoms_list_obj_insp_widget_t::construct_general_tab() {

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

  QStringList table_hdr_cell;
  table_hdr_cell.push_back("X");
  table_hdr_cell.push_back("Y");
  table_hdr_cell.push_back("Z");

  tg_gb_cell = new QGroupBox(tr("Lattice parameters"));
  tg_gb_cell_layout = new QVBoxLayout;
  tg_gb_cell->setLayout(tg_gb_cell_layout);
  tg_gb_cell_table = new QTableWidget;
  tg_gb_cell_table->setColumnCount(3);
  tg_gb_cell_table->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  tg_gb_cell_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tg_gb_cell_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tg_gb_cell_table->setHorizontalHeaderLabels(table_hdr_cell);
  tg_gb_cell_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  tg_gb_cell_layout->addWidget(tg_gb_cell_table);

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
  tab_general->tab_inner_widget_layout->addWidget(tg_gb_cell);

}

void ws_atoms_list_obj_insp_widget_t::construct_display_tab() {

  gb_disp_s = new QGroupBox(tr("Draw settings"));
  gb_disp_s_layout = new QFormLayout;
  gb_disp_s_layout->setLabelAlignment(Qt::AlignRight);
  gb_disp_s->setLayout(gb_disp_s_layout);

  tab_display->tab_inner_widget_layout->addWidget(gb_disp_s);
  disp_s_draw_atoms = new qbinded_checkbox;
  disp_s_draw_bonds = new qbinded_checkbox;
  disp_s_draw_img_atoms = new qbinded_checkbox;
  disp_s_draw_img_bonds = new qbinded_checkbox;

  disp_s_atom_scale = new qbinded_float_spinbox;
  disp_s_atom_scale->set_min_max_step(0.01, 3.0, 0.01);
  disp_s_bond_scale = new qbinded_float_spinbox;
  disp_s_bond_scale->set_min_max_step(0.01, 3.0, 0.01);

  disp_s_render_style = new qbinded_combobox;
  disp_s_render_style->addItem("Balls and sticks");
  disp_s_render_style->addItem("Dynamic lines");
  disp_s_render_style->addItem("X-atom lines");
  disp_s_render_style->addItem("Billboards");
  disp_s_render_style->addItem("Buffered billboards");

  disp_s_labels_style = new qbinded_combobox;
  disp_s_labels_style->addItem("None");
  disp_s_labels_style->addItem("Id");
  disp_s_labels_style->addItem("Type");
  disp_s_labels_style->addItem("Type and Id");

  disp_s_draw_subcells = new qbinded_checkbox;
  disp_s_draw_subcells_label = new QLabel(tr("Draw subcells"));
  disp_s_subcells_idx = new qbinded_int3_input;
  disp_s_subcells_idx_label = new QLabel(tr("Subcells range"));

  disp_s_subcells_idx->set_min_max_step(1, 10, 1);
  connect(disp_s_draw_subcells, &qbinded_checkbox::stateChanged,
          this, &ws_atoms_list_obj_insp_widget_t::draw_subcells_changed);

  gb_disp_s_layout->addRow(tr("Draw style"), disp_s_render_style);
  gb_disp_s_layout->addRow(tr("Draw atoms"), disp_s_draw_atoms);
  gb_disp_s_layout->addRow(tr("Draw bonds"), disp_s_draw_bonds);
  gb_disp_s_layout->addRow(tr("Draw img. atoms"), disp_s_draw_img_atoms);
  gb_disp_s_layout->addRow(tr("Draw img. bonds"), disp_s_draw_img_bonds);
  gb_disp_s_layout->addRow(tr("Atom scale"), disp_s_atom_scale);
  gb_disp_s_layout->addRow(tr("Bond scale"), disp_s_bond_scale);
  gb_disp_s_layout->addRow(tr("Labels style"), disp_s_labels_style);
  gb_disp_s_layout->addRow(disp_s_draw_subcells_label, disp_s_draw_subcells);
  gb_disp_s_layout->addRow(disp_s_subcells_idx_label, disp_s_subcells_idx);

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
  display_bt->setFocusPolicy(Qt::NoFocus);
  display_bt->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  display_bt->setModel(bt_model);
  display_bt_layout->addWidget(display_bt);
  bt_dist_delegate = new qrealspinbox_delegate_t;
  bt_dist_delegate->set_min_max_step(0.1, 5.0, 0.01);
  display_bt->setItemDelegateForColumn(2, bt_dist_delegate);


  tab_display->tab_inner_widget_layout->addWidget(gb_display_shading);
  tab_display->tab_inner_widget_layout->addWidget(gb_display_bt);

}

void ws_atoms_list_obj_insp_widget_t::construct_anim_tab() {

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
          this, SLOT(cur_anim_index_changed(int)));

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

}

void ws_atoms_list_obj_insp_widget_t::construct_measure_tab() {

}

void ws_atoms_list_obj_insp_widget_t::construct_modify_tab() {

  tm_gb_add_atom = new QGroupBox(tr("Add atom"));
  tm_gb_add_atom_layout = new QFormLayout;
  tm_gb_add_atom->setLayout(tm_gb_add_atom_layout);

  tm_gb_single_atom = new QGroupBox(tr("Modify single atom"));
  tm_gb_single_atom_layout = new QFormLayout;
  tm_gb_single_atom->setLayout(tm_gb_single_atom_layout);
  tm_single_atom_combo = new QComboBox;
  tm_single_atom_combo->setEditable(true);
  tm_single_atom_vec3 = new qbinded_float3_input;
  tm_single_atom_vec3->set_min_max_step(-10000, 10000, 0.01);
  tm_single_atom_idx = new QLabel;

  tm_single_atom_commit = new QPushButton(tr("Commit changes"));
  tm_single_atom_delete = new QPushButton(tr("Delete atom"));
  connect(tm_single_atom_delete, &QPushButton::pressed,
          this, &ws_atoms_list_obj_insp_widget_t::modify_single_atom_delete_button_clicked);

  tm_gb_single_atom_layout->addRow(tr("Atom name"), tm_single_atom_combo);
  tm_gb_single_atom_layout->addRow(tr("Atom idx"), tm_single_atom_idx);
  tm_gb_single_atom_layout->addRow(tr("Atom position"), tm_single_atom_vec3);
  tm_gb_single_atom_layout->addRow("", tm_single_atom_commit);
  tm_gb_single_atom_layout->addRow("", tm_single_atom_delete);

  connect(tm_single_atom_commit, &QPushButton::pressed,
          this, &ws_atoms_list_obj_insp_widget_t::modify_single_atom_button_clicked);

  tm_add_atom_combo = new QComboBox;
  tm_add_atom_combo->setEditable(true);
  tm_add_atom_vec3 = new qbinded_float3_input;
  tm_add_atom_vec3->set_min_max_step(-1000, 1000, 0.01);
  tm_add_atom_button = new QPushButton(tr("Add atom"));

  connect(tm_add_atom_button, &QPushButton::pressed,
          this, &ws_atoms_list_obj_insp_widget_t::modify_add_atom_button_clicked);

  tm_gb_add_atom_layout->addRow(tr("Atom name"), tm_add_atom_combo);
  tm_gb_add_atom_layout->addRow(tr("Atom posisition"), tm_add_atom_vec3);
  tm_gb_add_atom_layout->addRow("", tm_add_atom_button);

  tm_gb_pair_dist = new QGroupBox(tr("Pair distance"));
  tm_gb_pair_dist_layout = new QFormLayout;
  tm_gb_pair_dist->setLayout(tm_gb_pair_dist_layout);
  tm_pair_dist_atom1 = new QLabel;
  tm_pair_dist_atom2 = new QLabel;
  tm_pair_dist_atom1_idx = new QLabel;
  tm_pair_dist_atom2_idx = new QLabel;
  tm_pair_dist_spinbox = new QDoubleSpinBox;
  tm_pair_dist_note_label = new QLabel;
  tm_pair_dist_note_label->setText(tr("Distance between"));
  tm_pair_dist_spinbox->setMinimum(0.0);
  tm_pair_dist_spinbox->setMaximum(10);
  tm_pair_dist_spinbox->setSingleStep(0.01);
  tm_gb_pair_dist_layout->addRow(tr("Atom №1"), tm_pair_dist_atom1);
  tm_gb_pair_dist_layout->addRow(tr("Atom №2"), tm_pair_dist_atom2);
  tm_gb_pair_dist_layout->addRow(tr("Atom №1 idx"), tm_pair_dist_atom1_idx);
  tm_gb_pair_dist_layout->addRow(tr("Atom №2 idx"), tm_pair_dist_atom2_idx);
  tm_gb_pair_dist_layout->addRow(tm_pair_dist_note_label, tm_pair_dist_spinbox);

  tm_gb_pair_creation = new QGroupBox(tr("Insert atom between"));
  tm_gb_pair_creation_layout = new QFormLayout;
  tm_gb_pair_creation->setLayout(tm_gb_pair_creation_layout);
  tm_pair_creation_combo = new QComboBox;
  tm_pair_creation_combo->setEditable(true);
  tm_pair_creation_button = new QPushButton("Append");
  tm_gb_pair_creation_layout->addRow(tr("New atom type"), tm_pair_creation_combo);
  tm_gb_pair_creation_layout->addRow("", tm_pair_creation_button);

  connect(tm_pair_creation_button, &QPushButton::pressed,
          this, &ws_atoms_list_obj_insp_widget_t::modify_add_atom_between_pair);

  connect(tm_pair_dist_spinbox,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this, &ws_atoms_list_obj_insp_widget_t::modify_pair_dist_spinbox_value_changed);

  tm_gb_u_scale = new QGroupBox(tr("Barycentric scaling"));
  tm_gb_u_scale_layout = new QFormLayout;
  tm_gb_u_scale->setLayout(tm_gb_u_scale_layout);

  tm_u_scale_sb_x = new QDoubleSpinBox;
  tm_u_scale_sb_x->setMinimum(0.01);
  tm_u_scale_sb_x->setMaximum(2.0);
  tm_u_scale_sb_x->setSingleStep(0.01);
  tm_u_scale_sb_x->setValue(1.0);

  tm_u_scale_sb_y = new QDoubleSpinBox;
  tm_u_scale_sb_y->setMinimum(0.01);
  tm_u_scale_sb_y->setMaximum(2.0);
  tm_u_scale_sb_y->setSingleStep(0.01);
  tm_u_scale_sb_y->setValue(1.0);

  tm_u_scale_sb_z = new QDoubleSpinBox;
  tm_u_scale_sb_z->setMinimum(0.01);
  tm_u_scale_sb_z->setMaximum(2.0);
  tm_u_scale_sb_z->setSingleStep(0.01);
  tm_u_scale_sb_z->setValue(1.0);

  tm_u_scale_x_enabled = new QCheckBox;
  tm_u_scale_x_enabled->setChecked(true);

  tm_u_scale_y_enabled = new QCheckBox;
  tm_u_scale_y_enabled->setChecked(true);

  tm_u_scale_z_enabled = new QCheckBox;
  tm_u_scale_z_enabled->setChecked(true);

  tm_u_apply_scale_button = new QPushButton(tr("Apply scale"));
  connect(tm_u_apply_scale_button, &QPushButton::pressed,
          this, &ws_atoms_list_obj_insp_widget_t::modify_barycentric_scale_button_clicked);

  tm_gb_u_scale_layout->addRow("Scaling value for X-axis", tm_u_scale_sb_x);
  tm_gb_u_scale_layout->addRow("Scaling value for Y-axis", tm_u_scale_sb_y);
  tm_gb_u_scale_layout->addRow("Scaling value for Z-axis", tm_u_scale_sb_z);
  tm_gb_u_scale_layout->addRow("Enable scaling for X-axis", tm_u_scale_x_enabled);
  tm_gb_u_scale_layout->addRow("Enable scaling for Y-axis", tm_u_scale_y_enabled);
  tm_gb_u_scale_layout->addRow("Enable scaling for Z-axis", tm_u_scale_z_enabled);

  tm_gb_u_scale_layout->addRow("", tm_u_apply_scale_button);

  tm_gb_translate = new QGroupBox(tr("Translate selected atoms"));
  tm_gb_translate_layout = new QFormLayout;
  tm_gb_translate->setLayout(tm_gb_translate_layout);
  tm_translate_vec3 = new qbinded_float3_input;
  tm_translate_vec3->set_min_max_step(-10000, 10000, 0.01);
  tm_translate_apply_button = new QPushButton(tr("Apply translate"));
  tm_translate_coord_type_label = new QLabel("Coord. type");
  tm_translate_coord_type = new QComboBox;
  tm_translate_coord_type->addItem("Cartesian");
  tm_translate_coord_type->addItem("Fractional");

  tm_gb_translate_layout->addRow(tm_translate_coord_type_label, tm_translate_coord_type);
  tm_gb_translate_layout->addRow(tr("Tr. vector"), tm_translate_vec3);
  tm_gb_translate_layout->addRow("", tm_translate_apply_button);

  connect(tm_translate_coord_type,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &ws_atoms_list_obj_insp_widget_t::modify_translate_coord_type_changed);

  connect(tm_translate_apply_button, &QPushButton::pressed,
          this, &ws_atoms_list_obj_insp_widget_t::modify_translate_selected_atoms_clicked);

  tab_modify->tab_inner_widget_layout->addWidget(tm_gb_add_atom);
  tab_modify->tab_inner_widget_layout->addWidget(tm_gb_single_atom);
  tab_modify->tab_inner_widget_layout->addWidget(tm_gb_pair_dist);
  tab_modify->tab_inner_widget_layout->addWidget(tm_gb_pair_creation);
  tab_modify->tab_inner_widget_layout->addWidget(tm_gb_u_scale);
  tab_modify->tab_inner_widget_layout->addWidget(tm_gb_translate);

  tab_modify->tab_inner_widget_layout->addStretch(0);

}

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

      tg_type_summary_table->resizeRowsToContents();

      //update cell

      cell_changed();

      //display tab

      disp_s_draw_atoms->bind_value(&b_al->m_draw_atoms);
      disp_s_draw_bonds->bind_value(&b_al->m_draw_bonds);
      disp_s_draw_img_atoms->bind_value(&b_al->m_draw_imaginary_atoms);
      disp_s_draw_img_bonds->bind_value(&b_al->m_draw_imaginary_bonds);
      disp_s_atom_scale->bind_value(&b_al->m_atom_scale_factor);
      disp_s_bond_scale->bind_value(&b_al->m_bond_scale_factor);
      disp_s_render_style->bind_value(
            reinterpret_cast<int*>(&b_al->m_cur_render_type));
      disp_s_labels_style->bind_value(
            reinterpret_cast<int*>(&b_al->m_labels->m_style));
      display_shading_spec->bind_value(&b_al->m_draw_specular);
      display_shading_spec_value->bind_value(&b_al->m_shading_specular_power);

      disp_s_draw_subcells->bind_value(&b_al->m_draw_subcells);
      disp_s_subcells_idx->bind_value(&b_al->m_subcells_range);
      draw_subcells_changed(0);
      disp_s_draw_subcells->setVisible(b_al->m_geom->DIM == 3);
      disp_s_draw_subcells_label->setVisible(b_al->m_geom->DIM == 3);
      disp_s_subcells_idx->setVisible(b_al->m_geom->DIM == 3 && b_al->m_draw_subcells);
      disp_s_subcells_idx_label->setVisible(b_al->m_geom->DIM == 3 && b_al->m_draw_subcells);

      //anim bindings
      update_animate_section_status();

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

      display_bt->setModel(nullptr);
      bt_model->bind(b_al);
      display_bt->setModel(bt_model);
      display_bt->update();

      update_modify_tab();
    }
}

void ws_atoms_list_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();
  disp_s_draw_atoms->unbind_value();
  disp_s_draw_bonds->unbind_value();
  disp_s_draw_img_atoms->unbind_value();
  disp_s_draw_img_bonds->unbind_value();
  disp_s_atom_scale->unbind_value();
  disp_s_bond_scale->unbind_value();
  disp_s_render_style->unbind_value();
  disp_s_labels_style->unbind_value();
  display_shading_spec->unbind_value();
  display_shading_spec_value->unbind_value();
  disp_s_draw_subcells->unbind_value();
  disp_s_subcells_idx->unbind_value();

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

void ws_atoms_list_obj_insp_widget_t::update_modify_tab() {

  if (b_al) {

      if (b_al->m_parent_ws &&
          b_al->m_parent_ws->m_edit_type == ws_edit_t::edit_content) {

          tabBar()->setTabEnabled(3, true);

          if (b_al->m_atom_sel.empty()) {
              tm_gb_single_atom->hide();
              tm_gb_add_atom->show();
              tm_gb_pair_dist->hide();
              tm_gb_u_scale->hide();
              tm_gb_pair_creation->hide();
              tm_gb_translate->hide();
              //update atom names combobox
              fill_combo_with_atom_types(tm_add_atom_combo, b_al);
            }

          if (b_al->m_atom_idx_sel.size() == 1) {
              tm_gb_single_atom->show();
              tm_gb_add_atom->hide();
              tm_gb_pair_dist->hide();
              tm_gb_u_scale->hide();
              tm_gb_pair_creation->hide();
              tm_gb_translate->show();

              fill_combo_with_atom_types(tm_single_atom_combo, b_al);

              auto it = b_al->m_atom_idx_sel.begin();
              if (it != b_al->m_atom_idx_sel.end()) {

                  tm_single_atom_idx->setText(
                        QString::fromStdString(fmt::format("{}", it->m_idx)));

                  tm_single_atom_combo->setCurrentText(
                        QString::fromStdString(b_al->m_geom->atom_name(it->m_atm)));
                  tm_single_atom_vec3->sb_x->setValue(b_al->m_geom->pos(it->m_atm)[0]);
                  tm_single_atom_vec3->sb_y->setValue(b_al->m_geom->pos(it->m_atm)[1]);
                  tm_single_atom_vec3->sb_z->setValue(b_al->m_geom->pos(it->m_atm)[2]);

                } else {

                }

            }

          if (b_al->m_atom_idx_sel.size() == 2) {
              tm_gb_add_atom->hide();
              tm_gb_single_atom->hide();
              tm_gb_pair_dist->show();
              tm_gb_u_scale->hide();
              tm_gb_pair_creation->show();
              tm_gb_translate->show();

              auto it1 = b_al->m_atom_idx_sel.begin();
              auto it2 = it1++;
              if (it2 != b_al->m_atom_idx_sel.end()) {

                  tm_pair_dist_atom1->setText(
                        QString::fromStdString(fmt::format("{}{}",
                                                           b_al->m_geom->atom_name(it1->m_atm),
                                                           it1->m_atm)));
                  tm_pair_dist_atom2->setText(
                        QString::fromStdString(fmt::format("{}{}",
                                                           b_al->m_geom->atom_name(it2->m_atm),
                                                           it2->m_atm)));
                  tm_pair_dist_atom1_idx->setText(
                        QString::fromStdString(fmt::format("{}", it1->m_idx)));
                  tm_pair_dist_atom2_idx->setText(
                        QString::fromStdString(fmt::format("{}", it2->m_idx)));

                  if (it1->m_idx == index::D(b_al->m_geom->DIM).all(0) &&
                      it2->m_idx == index::D(b_al->m_geom->DIM).all(0)) {
                      tm_pair_dist_spinbox->show();
                      float dist_btw = (b_al->m_geom->pos(it1->m_atm, it1->m_idx) -
                                        b_al->m_geom->pos(it2->m_atm, it2->m_idx)).norm();

                      tm_pair_dist_spinbox->blockSignals(true);
                      tm_pair_dist_spinbox->setValue(dist_btw);
                      tm_pair_dist_spinbox->blockSignals(false);
                      tm_pair_dist_note_label->show();
                      tm_pair_dist_spinbox->show();
                    } else {
                      tm_pair_dist_note_label->hide();
                      tm_pair_dist_spinbox->hide();
                    }

                  //atom between
                  fill_combo_with_atom_types(tm_pair_creation_combo, b_al);
                }
            }

          if (b_al->m_atom_sel.size() > 2) {
              tm_gb_add_atom->hide();
              tm_gb_single_atom->hide();
              tm_gb_pair_creation->hide();
              tm_gb_pair_dist->hide();
              tm_gb_u_scale->show();
              tm_gb_translate->show();

            }

          if (b_al->m_atom_sel.size() > 0) {
              if (b_al->m_geom->DIM == 3) {
                  tm_translate_coord_type_label->show();
                  tm_translate_coord_type->show();
                } else {
                  tm_translate_coord_type->setCurrentIndex(0);
                  tm_translate_coord_type->hide();
                  tm_translate_coord_type_label->hide();
                }
            }

        } else {

          tabBar()->setTabEnabled(3, false);
          tm_gb_add_atom->hide();
          tm_gb_pair_creation->hide();
          tm_gb_single_atom->hide();
          tm_gb_pair_dist->hide();
          tm_gb_u_scale->hide();
          tm_gb_translate->hide();

        }

    }

}

void ws_atoms_list_obj_insp_widget_t::fill_combo_with_atom_types(QComboBox *combo,
                                                                 ws_atoms_list_t *_al) {
  if (_al && combo) {
      combo->clear();
      for (auto i = 0 ; i < _al->m_geom->n_atom_types(); i++)
        combo->addItem(QString::fromStdString(_al->m_geom->atom_of_type(i)));
    }
}

ws_atoms_list_obj_insp_widget_t::ws_atoms_list_obj_insp_widget_t() : ws_item_obj_insp_widget_t() {

  tab_display = define_tab(tr("Display"));
  tab_animation = define_tab(tr("Animation"));
  tab_modify = define_tab(tr("Modify"));
  tab_measurement = define_tab(tr("Measurement"));

  construct_general_tab();
  construct_display_tab();
  construct_anim_tab();
  construct_measure_tab();
  construct_modify_tab();

  app_state_t *astate = app_state_t::get_inst();

  connect(astate->astate_evd, SIGNAL(cur_ws_selected_item_frame_changed_signal()),
          this, SLOT(cur_ws_selected_item_frame_changed()));

  connect(astate->astate_evd, SIGNAL(cur_ws_selected_atoms_list_cell_changed_signal()),
          this, SLOT(cell_changed()));

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_selection_changed_signal,
          this,
          &ws_atoms_list_obj_insp_widget_t::cur_it_list_selection_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_edit_type_changed_signal,
          this,
          &ws_atoms_list_obj_insp_widget_t::cur_ws_edit_mode_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_selected_content_changed_signal,
          this,
          &ws_atoms_list_obj_insp_widget_t::cur_it_selected_content_changed);

}

void ws_atoms_list_obj_insp_widget_t::cur_anim_index_changed(int index) {

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

void ws_atoms_list_obj_insp_widget_t::anim_updated_external() {

  if (b_al) {
      int current_frame_truncated = std::floor(b_al->m_anim->m_cur_anim_time);

      // remap frame id from 0..size-1 1..size
      gb_anim_cur_frame->setText(tr("%1").arg(current_frame_truncated+1));
      gb_anim_timeline_slider->blockSignals(true);
      gb_anim_timeline_slider->setValue(current_frame_truncated);
      gb_anim_timeline_slider->blockSignals(false);
    }

}

void ws_atoms_list_obj_insp_widget_t::cur_ws_selected_item_frame_changed() {
  anim_updated_external();
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

void ws_atoms_list_obj_insp_widget_t::cell_changed() {

  if (b_al) {

      if (b_al->m_geom->DIM > 0) {

          tg_gb_cell_table->setRowCount(b_al->m_geom->DIM);
          tg_gb_cell_table->setMinimumHeight(100);
          QStringList table_hdr_cell_v;
          table_hdr_cell_v.push_back("a");
          table_hdr_cell_v.push_back("b");
          table_hdr_cell_v.push_back("c");
          tg_gb_cell_table->setVerticalHeaderLabels(table_hdr_cell_v);

          for (int c = 0; c < b_al->m_geom->DIM; c++)
            for (int i = 0; i < 3; i++) {
                float cell_amp = b_al->m_geom->cell.v[c][i];
                QTableWidgetItem *n_ax = new QTableWidgetItem(tr("%1").arg(cell_amp));
                tg_gb_cell_table->setItem(c, i, n_ax);
              }

          tg_gb_cell_table->resizeRowsToContents();
          tg_gb_cell->show();
        } else {
          tg_gb_cell->hide();
        }
    }

}

void ws_atoms_list_obj_insp_widget_t::draw_subcells_changed(int state) {

  if (b_al && b_al->m_draw_subcells ) {
      disp_s_subcells_idx_label->show();
      disp_s_subcells_idx->show();

    } else {
      disp_s_subcells_idx_label->hide();
      disp_s_subcells_idx->hide();
    }

}

void ws_atoms_list_obj_insp_widget_t::update_animate_section_status() {
  tabBar()->setTabEnabled(2, b_al->m_anim->animable());
}

void ws_atoms_list_obj_insp_widget_t::cur_it_list_selection_changed() {

  std::cout<<"SEL"<<std::endl;

  if (b_al && b_al->is_selected()) {

      update_modify_tab();
    }

}

void ws_atoms_list_obj_insp_widget_t::modify_add_atom_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      vector3<float> new_atom_pos{tm_add_atom_vec3->sb_x->value(),
            tm_add_atom_vec3->sb_y->value(),
            tm_add_atom_vec3->sb_z->value()};
      std::string new_atom_name = tm_add_atom_combo->currentText().toStdString();
      b_al->insert_atom(new_atom_name, new_atom_pos);
      update_animate_section_status();
      astate->make_viewport_dirty();
    }
}

void ws_atoms_list_obj_insp_widget_t::modify_single_atom_button_clicked() {

  if (b_al && b_al->m_atom_sel.size() == 1) {
      auto it = b_al->m_atom_sel.begin();
      if (it != b_al->m_atom_sel.end()) {
          int itv = *it;
          b_al->update_atom(itv, tm_single_atom_combo->currentText().toStdString(),
                            vector3<float>(tm_single_atom_vec3->sb_x->value(),
                                           tm_single_atom_vec3->sb_y->value(),
                                           tm_single_atom_vec3->sb_z->value()));
          update_animate_section_status();
        }
    }
}

void ws_atoms_list_obj_insp_widget_t::modify_single_atom_delete_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      b_al->delete_selected_atoms();
      astate->make_viewport_dirty();
      update_animate_section_status();
    }
}

void ws_atoms_list_obj_insp_widget_t::modify_pair_dist_spinbox_value_changed(double newval) {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      auto it1 = b_al->m_atom_idx_sel.begin();
      auto it2 = it1++;
      if (it2 != b_al->m_atom_idx_sel.end() &&
          it1->m_idx == index::D(b_al->m_geom->DIM).all(0) &&
          it2->m_idx == index::D(b_al->m_geom->DIM).all(0)) {

          b_al->update_inter_atomic_dist(newval, it1->m_atm, it2->m_atm);

          astate->make_viewport_dirty();
        }
    }
}

void ws_atoms_list_obj_insp_widget_t::modify_add_atom_between_pair() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      auto it1 = b_al->m_atom_idx_sel.begin();
      auto it2 = it1++;
      vector3<float> r_btw{0.0, 0.0, 0.0};

      if (it1 != b_al->m_atom_idx_sel.end() && it2 != b_al->m_atom_idx_sel.end())
        r_btw = (b_al->m_geom->pos(it1->m_atm, it1->m_idx) +
                 b_al->m_geom->pos(it2->m_atm, it2->m_idx))*0.5f;

      std::string new_atom_name = tm_pair_creation_combo->currentText().toStdString();
      b_al->insert_atom(new_atom_name, r_btw);
      update_animate_section_status();
      astate->make_viewport_dirty();
    }

}

void ws_atoms_list_obj_insp_widget_t::modify_barycentric_scale_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      vector3<float> center{0.0f, 0.0f, 0.0f};
      for (auto &rec : b_al->m_atom_sel) center += b_al->m_geom->pos(rec);
      center /= b_al->m_atom_sel.size();

      for (auto &rec : b_al->m_atom_sel) {

          float scale_mod_x = 0.0f;
          float scale_mod_y = 0.0f;
          float scale_mod_z = 0.0f;

          if (tm_u_scale_x_enabled->checkState() == Qt::Checked) scale_mod_x = 1.0f;
          if (tm_u_scale_y_enabled->checkState() == Qt::Checked) scale_mod_y = 1.0f;
          if (tm_u_scale_z_enabled->checkState() == Qt::Checked) scale_mod_z = 1.0f;

          vector3<float> new_pos_dist = center - b_al->m_geom->pos(rec);
          vector3<float> new_pos = b_al->m_geom->pos(rec);

          new_pos[0] +=  (1-float(tm_u_scale_sb_x->value())) * new_pos_dist[0] * scale_mod_x;
          new_pos[1] +=  (1-float(tm_u_scale_sb_y->value())) * new_pos_dist[1] * scale_mod_y;
          new_pos[2] +=  (1-float(tm_u_scale_sb_z->value())) * new_pos_dist[2] * scale_mod_z;

          b_al->update_atom(rec, new_pos);
        }

      update_animate_section_status();
      astate->make_viewport_dirty();
    }
}

void ws_atoms_list_obj_insp_widget_t::modify_translate_selected_atoms_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      vector3<float> tr_vec(tm_translate_vec3->sb_x->value(),
                            tm_translate_vec3->sb_y->value(),
                            tm_translate_vec3->sb_z->value());

      if (tm_translate_coord_type->currentIndex() == 1 && b_al->m_geom->DIM == 3) {
          vector3<float> tr_vec_c = tr_vec;
          tr_vec = b_al->m_geom->cell.frac2cart(tr_vec_c);
        }

      for (auto &rec : b_al->m_atom_sel) {
          b_al->update_atom(rec, b_al->m_geom->pos(rec) + tr_vec);
        }

    }

  astate->make_viewport_dirty();
}

void ws_atoms_list_obj_insp_widget_t::modify_translate_coord_type_changed(int coord_type) {
  if (coord_type == 0) {
      tm_translate_vec3->set_min_max_step(-10000, 10000, 0.01);
    } else {
      tm_translate_vec3->set_min_max_step(-1.0, 1.0, 0.01);
    }
}

void ws_atoms_list_obj_insp_widget_t::cur_ws_edit_mode_changed() {
  update_modify_tab();
}

void ws_atoms_list_obj_insp_widget_t::cur_it_selected_content_changed() {
  update_modify_tab();
}
