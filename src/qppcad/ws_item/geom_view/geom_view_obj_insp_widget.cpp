#include <qppcad/ws_item/geom_view/geom_view_obj_insp_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_labels_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_sel_groups_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_measurement_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_type_summary_popup.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

#include <QColorDialog>

using namespace qpp;
using namespace qpp::cad;

void geom_view_obj_insp_widget_t::construct_general_tab() {

  app_state_t *astate = app_state_t::get_inst();

  tg_geom_summary_widget = new qspoiler_widget_t(tr("Geometry Summary"));
  tg_geom_summary_lt = new QFormLayout;
  tg_geom_summary_widget->add_content_layout(tg_geom_summary_lt);

  tg_geom_summary_total_atoms = new QLabel;
  tg_geom_summary_total_types = new QLabel;

  tg_geom_summary_lt->addRow(tr("Total atoms"), tg_geom_summary_total_atoms);
  tg_geom_summary_lt->addRow(tr("Total types"), tg_geom_summary_total_types);
  init_form_lt(tg_geom_summary_lt);

  tg_type_summary_widget = new qspoiler_widget_t(tr("Type Summary"),
                                                 this, true, 0, 360, false, 0);
  tg_type_summary_lt = new QVBoxLayout;
  tg_type_summary_widget->add_content_layout(tg_type_summary_lt);
  tg_type_summary_tbl = new QTableWidget;
  tg_type_summary_tbl->setFixedWidth(astate->size_guide.obj_insp_table_w());
  tg_type_summary_tbl->setColumnCount(3);

  type_summary_clear_tclr_override = new QPushButton;
  type_summary_clear_tclr_override->setFixedSize(QSize(astate->size_guide.ext_editor_btn_h(),
                                                       astate->size_guide.ext_editor_btn_h()));
  type_summary_clear_tclr_override->setIcon(QIcon("://images/outline-refresh-24px.svg"));
  type_summary_clear_tclr_override->setFlat(true);
  type_summary_clear_tclr_override->setToolTip("Clear the color override");

  connect(type_summary_clear_tclr_override,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::clear_color_override_button_clicked);

  tg_type_summary_widget->hbox_frm->insertWidget(3, type_summary_clear_tclr_override);

  QStringList table_hdr_cell;
  table_hdr_cell.push_back("X");
  table_hdr_cell.push_back("Y");
  table_hdr_cell.push_back("Z");

  tg_gb_cell = new qspoiler_widget_t(tr("Lattice Parameters"), this, true, 0, 360, false, 0);
  tg_gb_cell_lt = new QVBoxLayout;
  tg_gb_cell->add_content_layout(tg_gb_cell_lt);
  tg_gb_cell_tbl = new QTableWidget;

  tg_gb_cell_tbl->setColumnCount(3);
  tg_gb_cell_tbl->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  tg_gb_cell_tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tg_gb_cell_tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tg_gb_cell_tbl->verticalHeader()->setVisible(false);
  tg_gb_cell_tbl->setHorizontalHeaderLabels(table_hdr_cell);
  tg_gb_cell_tbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  tg_gb_cell_tbl->setFixedWidth(astate->size_guide.obj_insp_table_w());
  tg_gb_cell_tbl->setShowGrid(false);
  tg_gb_cell_lt->addWidget(tg_gb_cell_tbl);

  QStringList table_hdr;
  table_hdr.push_back("Name");
  table_hdr.push_back("Count");
  table_hdr.push_back("Color");

  tg_type_summary_tbl->setHorizontalHeaderLabels(table_hdr);
  tg_type_summary_tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tg_type_summary_tbl->verticalHeader()->setVisible(false);
  tg_type_summary_tbl->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  tg_type_summary_tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tg_type_summary_tbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  tg_type_summary_tbl->setShowGrid(false);
  tg_type_summary_lt->addWidget(tg_type_summary_tbl);

  connect(tg_type_summary_tbl,
          &QTableView::clicked,
          this,
          &geom_view_obj_insp_widget_t::type_summary_clicked);

  tab_general->tab_inner_widget_lt->addWidget(tg_geom_summary_widget);
  tab_general->tab_inner_widget_lt->addWidget(tg_type_summary_widget);
  tab_general->tab_inner_widget_lt->addWidget(tg_gb_cell);

  tab_general->tab_inner_widget_lt->addStretch();

}

void geom_view_obj_insp_widget_t::construct_display_tab() {

  app_state_t *astate = app_state_t::get_inst();

  gb_disp_s = new qspoiler_widget_t(tr("Draw Settings"));
  gb_disp_s_lt = new QFormLayout;
  gb_disp_s->add_content_layout(gb_disp_s_lt);

  disp_s_draw_atoms = new qbinded_checkbox_t;
  disp_s_draw_bonds = new qbinded_checkbox_t;
  disp_s_draw_img_atoms = new qbinded_checkbox_t;
  disp_s_draw_img_bonds = new qbinded_checkbox_t;

  disp_s_atom_scale = new qbinded_float_spinbox_t;
  disp_s_atom_scale->set_min_max_step(0.01, 3.0, 0.01);
  disp_s_bond_scale = new qbinded_float_spinbox_t;
  disp_s_bond_scale->set_min_max_step(0.01, 3.0, 0.01);

  disp_s_render_style = new qbinded_combobox_t;
  disp_s_render_style->addItem("Balls and sticks");
  disp_s_render_style->addItem("Suprematic atoms");
  disp_s_render_style->addItem("Dynamic lines");
  disp_s_render_style->addItem("X-atom lines");
  disp_s_render_style->addItem("Billboards");
  disp_s_render_style->addItem("Buffered billboards");

  disp_s_color_mode = new qbinded_combobox_t;
  disp_s_color_mode->addItem(tr("Color from ptable"));
  disp_s_color_mode->addItem(tr("Color from xgeom"));

  // ******************  Periodic related render *****************************
  gb_periodic_related_render = new qspoiler_widget_t(tr("Periodic Related Settings"));
  gb_periodic_related_render_lt = new QFormLayout;
  gb_periodic_related_render->add_content_layout(gb_periodic_related_render_lt);

  periodic_draw_cell_v = new qbinded_checkbox_t;
  periodic_cell_v_ratio = new qbinded_float_spinbox_t;
  periodic_cell_v_ratio->set_min_max_step(-5, 5, 0.01);

  periodic_draw_subcells = new qbinded_checkbox_t;
  periodic_subcells_idx = new qbinded_int3_input_t;
  periodic_subcells_idx_lbl = new QLabel(tr("Subcells range"));
  periodic_subcells_idx->set_min_max_step(1, 10, 1);
  periodic_subcells_clr = new qbinded_color3_input_t();
  periodic_subcells_color_lbl = new QLabel(tr("Subcell color"));

  init_form_lt_lbl(periodic_subcells_idx_lbl);
  init_form_lt_lbl(periodic_subcells_color_lbl);

  connect(periodic_draw_subcells,
          &qbinded_checkbox_t::stateChanged,
          this,
          &geom_view_obj_insp_widget_t::draw_subcells_changed);

  disp_s_sel_vis = new qbinded_checkbox_t;
  disp_s_sel_vis_affect_bonds = new qbinded_checkbox_t;

  periodic_draw_cell = new qbinded_checkbox_t;
  periodic_cell_offset = new qbinded_float3_input_t;
  periodic_cell_offset->set_min_max_step(-1000, 1000, 0.01);
  periodic_cell_color = new qbinded_color3_input_t();
  periodic_cell_vectors_color = new qbinded_color3_input_t;

  gb_periodic_related_render_lt->addRow(tr("Draw cell"), periodic_draw_cell);
  gb_periodic_related_render_lt->addRow(tr("Draw vectors"), periodic_draw_cell_v);
  gb_periodic_related_render_lt->addRow(tr("Vectors ratio"), periodic_cell_v_ratio);
  gb_periodic_related_render_lt->addRow(tr("Vectors offset"), periodic_cell_offset);
  gb_periodic_related_render_lt->addRow(tr("Vectors color"), periodic_cell_vectors_color);
  gb_periodic_related_render_lt->addRow(tr("Cell color"), periodic_cell_color);
  gb_periodic_related_render_lt->addRow(tr("Draw subcells"), periodic_draw_subcells);

  init_form_lt(gb_periodic_related_render_lt);
  // ******************  End of Periodic related render *********************************************

  gb_disp_s_lt->addRow(tr("Draw style"), disp_s_render_style);
  gb_disp_s_lt->addRow(tr("Color style"), disp_s_color_mode);
  gb_disp_s_lt->addRow(tr("Draw atoms"), disp_s_draw_atoms);
  gb_disp_s_lt->addRow(tr("Draw bonds"), disp_s_draw_bonds);
  gb_disp_s_lt->addRow(tr("Draw im. atoms"), disp_s_draw_img_atoms);
  gb_disp_s_lt->addRow(tr("Draw im. bonds"), disp_s_draw_img_bonds);
  gb_disp_s_lt->addRow(tr("Atom scale"), disp_s_atom_scale);
  gb_disp_s_lt->addRow(tr("Bond scale"), disp_s_bond_scale);
  gb_disp_s_lt->addRow(tr("Sel. vis."), disp_s_sel_vis);
  gb_disp_s_lt->addRow(tr("Sel. vis. bonds"), disp_s_sel_vis_affect_bonds);
  init_form_lt(gb_disp_s_lt);

  gb_disp_labels = new qspoiler_widget_t(tr("Labels"));
  gb_disp_labels_lt = new QFormLayout;
  gb_disp_labels->add_content_layout(gb_disp_labels_lt);

  disp_labels_style = new qbinded_combobox_t;
  disp_labels_style->addItem("None");
  disp_labels_style->addItem("Id");
  disp_labels_style->addItem("Type");
  disp_labels_style->addItem("Type & Id");
  disp_labels_style->addItem("Charge");
  disp_labels_style->addItem("Charge, Type");
  disp_labels_style->addItem("Charge, Type & Id");
  disp_labels_style->addItem("Custom");

  disp_labels_size = new qbinded_int_spinbox_t;
  disp_labels_size->set_min_max_step(5, 35, 1);
  disp_inplace_labels = new qbinded_checkbox_t;
  disp_inplace_offset = new qbinded_float2_input_t;
  disp_inplace_offset->set_min_max_step(0, 1, 0.01);
  disp_sl_labels = new qbinded_checkbox_t;
  disp_labels_screen_scale = new qbinded_checkbox_t;
  disp_labels_draw_outline = new qbinded_checkbox_t;
  disp_labels_outline_size = new qbinded_float_spinbox_t;
  disp_labels_outline_size->set_min_max_step(1, 20, 0.1);

  gb_disp_labels_lt->addRow(tr("Labels style"), disp_labels_style);
  gb_disp_labels_lt->addRow(tr("Labels size"), disp_labels_size);
  gb_disp_labels_lt->addRow(tr("Draw outline"), disp_labels_draw_outline);
  gb_disp_labels_lt->addRow(tr("Outline size"), disp_labels_outline_size);
  gb_disp_labels_lt->addRow(tr("Inplace labels"), disp_inplace_labels);
  gb_disp_labels_lt->addRow(tr("Inplace offset"), disp_inplace_offset);
  gb_disp_labels_lt->addRow(tr("Selective vis."), disp_sl_labels);
  gb_disp_labels_lt->addRow(tr("Scr. spc. scale"), disp_labels_screen_scale);
  init_form_lt(gb_disp_labels_lt);

  //display - shading tab initialization
  gb_disp_shading = new qspoiler_widget_t(tr("Shading Settings"));
  gb_disp_shading_lt = new QFormLayout;
  gb_disp_shading->add_content_layout(gb_disp_shading_lt);

  disp_shading_spec = new qbinded_checkbox_t;
  disp_shading_spec_value = new qbinded_float_spinbox_t;
  disp_shading_spec_value->set_min_max_step(0.25, 50, 0.25);
  gb_disp_shading_lt->addRow(tr("Enable specular"), disp_shading_spec);
  gb_disp_shading_lt->addRow(tr("Specular power"), disp_shading_spec_value);
  init_form_lt(gb_disp_shading_lt);

  //display - type specific rendering
  gb_disp_type_spec_rend = new qspoiler_widget_t(tr("Type Specific Rendering"),
                                                 this, true, 0, 360, false, 0);
  gb_disp_type_spec_rend_lt = new QVBoxLayout;
  gb_disp_type_spec_rend->add_content_layout(gb_disp_type_spec_rend_lt);

  disp_type_spec_tv = new QTableView;
  gb_disp_type_spec_rend_lt->addWidget(disp_type_spec_tv);

  disp_type_spec_tv->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  disp_type_spec_tv->verticalHeader()->hide();
  disp_type_spec_tv->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  disp_type_spec_tv->setFocusPolicy(Qt::NoFocus);
  disp_type_spec_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  disp_type_spec_tv->setFixedWidth(astate->size_guide.obj_insp_table_w());
  disp_type_spec_tv->setShowGrid(false);
  disp_type_spec_mdl = new qtype_specific_rendering_model_t;
  disp_type_spec_tv->setModel(disp_type_spec_mdl);

  //display - bonding table
  gb_disp_bt = new qspoiler_widget_t(tr("Bonding Table"), this, true, 0, 360, false, 0);
  gb_disp_bt->setContentsMargins(0,0,0,0);
  disp_bt_lt = new QVBoxLayout;
  gb_disp_bt->add_content_layout(disp_bt_lt);

  disp_bt_rebond_btn = new QPushButton;
  disp_bt_rebond_btn->setFixedSize(QSize(astate->size_guide.ext_editor_btn_h(),
                                         astate->size_guide.ext_editor_btn_h()));
  disp_bt_rebond_btn->setIcon(QIcon("://images/outline-refresh-24px.svg"));
  disp_bt_rebond_btn->setFlat(true);
  disp_bt_rebond_btn->setToolTip("Rebond the structure");

  connect(disp_bt_rebond_btn,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::rebond_button_clicked);

  gb_disp_bt->hbox_frm->insertWidget(3, disp_bt_rebond_btn);

  bt_mdl = new qbonding_table_model_t;
  disp_bt = new QTableView;

  disp_bt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  disp_bt->verticalHeader()->hide();
  disp_bt->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  disp_bt->setFocusPolicy(Qt::NoFocus);
  disp_bt->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  disp_bt->setFixedWidth(astate->size_guide.obj_insp_table_w());
  disp_bt->setShowGrid(false);
  disp_bt->setModel(bt_mdl);
  disp_bt->setLocale(QLocale::C);
  disp_bt_lt->addWidget(disp_bt);
  bt_dist_delegate = new qrealspinbox_delegate_t;
  bt_dist_delegate->set_min_max_step(0.1, 5.0, 0.01);
  disp_bt->setItemDelegateForColumn(2, bt_dist_delegate);

  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_s);
  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_shading);
  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_labels);
  tab_disp->tab_inner_widget_lt->addWidget(gb_periodic_related_render);
  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_type_spec_rend);
  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_bt);

  tab_disp->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_anim_tab() {

  app_state_t *astate = app_state_t::get_inst();

  gb_anim_summary = new qspoiler_widget_t(tr("Summary"));
  gb_anim_summary_lt = new QFormLayout;
  gb_anim_summary->add_content_layout(gb_anim_summary_lt);

  //gb_anim_summary->setMaximumHeight(250);
  gb_anim_total_anims = new QLabel;
  gb_rebuild_bonds = new qbinded_checkbox_t;
  gb_play_cyclic = new qbinded_checkbox_t;
  gb_anim_speed = new qbinded_float_spinbox_t;
  gb_anim_speed->set_min_max_step(0.01,10.0,0.01);
  gb_anim_total_frames_in_anim = new QLabel;
  gb_anim_cur_frame = new QLabel;
  gb_current_anim = new QComboBox;
  gb_current_anim->setFixedWidth(astate->size_guide.obj_insp_combo_max_w_v2());
  //gb_current_anim->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

  connect(gb_current_anim,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::cur_anim_index_changed);

  anim_act_lt = new QHBoxLayout;
  anim_act_lt->setContentsMargins(5, 0, 5, 0);
  anim_act_ren = new QPushButton(tr("Rename"));
  connect(anim_act_ren,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_act_ren_clicked);

  anim_act_del = new QPushButton(tr("Delete"));
  connect(anim_act_del,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_act_del_clicked);

  anim_act_lt->addWidget(anim_act_ren);
  anim_act_lt->addWidget(anim_act_del);

  gb_anim_summary_lt->addRow(tr("Num. of anim."), gb_anim_total_anims);
  gb_anim_summary_lt->addRow(tr("Rebuild bonds"), gb_rebuild_bonds);
  gb_anim_summary_lt->addRow(tr("Play in cycle"), gb_play_cyclic);
  gb_anim_summary_lt->addRow(tr("Current anim."), gb_current_anim);
  gb_anim_summary_lt->addRow(tr("Frame time "), gb_anim_speed);
  gb_anim_summary_lt->addRow(tr("Num frm."), gb_anim_total_frames_in_anim);
  gb_anim_summary_lt->addRow(tr("Current frame:"), gb_anim_cur_frame);
  gb_anim_summary_lt->addRow(tr("Actions"), anim_act_lt);
  init_form_lt(gb_anim_summary_lt);

  gb_anim_timeline = new qspoiler_widget_t(tr("Timeline"));
  gb_anim_timeline->setMaximumWidth(astate->size_guide.obj_insp_anim_w());
  gb_anim_timeline_lt = new QHBoxLayout;
  gb_anim_timeline->add_content_layout(gb_anim_timeline_lt);

  gb_anim_timeline_slider = new QSlider(Qt::Orientation::Horizontal);
  gb_anim_timeline_slider->setTickPosition(QSlider::TicksBothSides);
  gb_anim_timeline_slider->setTickInterval(10);

  connect(gb_anim_timeline_slider,
          &QSlider::valueChanged,
          this,
          &geom_view_obj_insp_widget_t::anim_timeline_slider_value_changed);

  //gb_anim_timeline_layout->addWidget(gb_anim_timeline_cur_frame, 1);
  gb_anim_timeline_lt->addWidget(gb_anim_timeline_slider, 1);

  gb_anim_buttons = new qspoiler_widget_t(tr("Control"));
  gb_anim_buttons_lt = new QHBoxLayout;
  gb_anim_buttons_lt->setContentsMargins(5, 0, 5, 0);
  gb_anim_buttons->add_content_layout(gb_anim_buttons_lt);
  gb_anim_buttons->setMaximumWidth(astate->size_guide.obj_insp_anim_w());
  //gb_anim_buttons->setMaximumHeight(90);

  anim_play = new QPushButton(tr("PLAY"));
  anim_play->setCheckable(true);

  connect(anim_play,
          &QPushButton::toggled,
          this,
          &geom_view_obj_insp_widget_t::play_anim_button_toggle);

  anim_to_start = new QPushButton(tr("<<"));

  connect(anim_to_start,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_button_begin_clicked);

  anim_to_end = new QPushButton(tr(">>"));

  connect(anim_to_end,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_button_end_clicked);

  anim_frame_forward = new QPushButton(tr("+F"));

  connect(anim_frame_forward,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_button_frame_move_forward_clicked);

  anim_frame_backward = new QPushButton(tr("-F"));

  connect(anim_frame_backward,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_button_frame_move_backward_clicked);

  gb_anim_buttons_lt->addWidget(anim_play, 1);
  gb_anim_buttons_lt->addWidget(anim_to_start, 1);
  gb_anim_buttons_lt->addWidget(anim_to_end, 1);
  gb_anim_buttons_lt->addWidget(anim_frame_forward, 1);
  gb_anim_buttons_lt->addWidget(anim_frame_backward, 1);

  tab_anim->tab_inner_widget_lt->addWidget(gb_anim_summary);
  tab_anim->tab_inner_widget_lt->addWidget(gb_anim_timeline);
  tab_anim->tab_inner_widget_lt->addWidget(gb_anim_buttons);

  tab_anim->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_measure_tab() {

  app_state_t *astate = app_state_t::get_inst();

  tms_common_settings_gb = new qspoiler_widget_t(tr("Common Settings"));
  tms_common_settings_gb_lt = new QFormLayout;
  tms_common_settings_gb->add_content_layout(tms_common_settings_gb_lt);

  tms_render_dist = new qbinded_checkbox_t;
  tms_render_angle = new qbinded_checkbox_t;
  tms_render_dist_legend = new qbinded_checkbox_t;
  tms_render_angle_legend = new qbinded_checkbox_t;

  tms_common_settings_gb_lt->addRow(tr("Draw dists"), tms_render_dist);
  tms_common_settings_gb_lt->addRow(tr("Draw angles"), tms_render_angle);
  tms_common_settings_gb_lt->addRow(tr("Dists legend"), tms_render_dist_legend);
  tms_common_settings_gb_lt->addRow(tr("Angles legend"), tms_render_angle_legend);

  init_form_lt(tms_common_settings_gb_lt);

  tms_pair_dist_gb = new qspoiler_widget_t(tr("Interatomic Distance"));
  tms_pair_dist_gb_lt = new QFormLayout;
  tms_pair_dist_gb->add_content_layout(tms_pair_dist_gb_lt);

  tms_pair_cur_msr = new QComboBox;
  tms_pair_cur_msr->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());

  connect(tms_pair_cur_msr,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::msr_pair_cur_idx_changed);

  tms_pair_at1_info = new QLabel();
  tms_pair_at2_info = new QLabel();
  tms_pair_dist = new QLabel();

  tms_pair_dist_color = new qbinded_color3_input_t;
  tms_pair_enabled = new qbinded_checkbox_t;
  tms_pair_label_enabled = new qbinded_checkbox_t;

  tms_pair_line_size = new qbinded_int_spinbox_t;
  tms_pair_line_size->set_min_max_step(1, 20, 1);

  tms_font_screen_size = new qbinded_int_spinbox_t;
  tms_font_screen_size->set_min_max_step(5, 80, 1);

  tms_pair_line_style = new qbinded_combobox_t;
  tms_pair_line_style->addItem("Solid");
  tms_pair_line_style->addItem("Dotted");
  tms_pair_line_style->addItem("Dashed");
  tms_pair_line_style->addItem("Dash-Dot");
  tms_pair_line_style->addItem("Dash-Dot-Dot");

  tms_pair_label_style = new qbinded_combobox_t;
  tms_pair_label_style->addItem("Standart");
  tms_pair_label_style->addItem("Outline");
  tms_pair_label_style->addItem("Border");

  tms_pair_action_sel = new QPushButton(tr("SEL"));

  connect(tms_pair_action_sel,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::msr_pair_select_clicked);

  tms_pair_action_copy = new QPushButton(tr("COPY"));

  connect(tms_pair_action_copy,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::msr_pair_copy_clicked);

  tms_pair_action_del = new QPushButton(tr("DEL"));

  connect(tms_pair_action_del,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::msr_pair_delete_clicked);

  tms_pair_action_lt = new QHBoxLayout;
  tms_pair_action_lt->setContentsMargins(4, 0, 8, 0);
  tms_pair_action_lt->addWidget(tms_pair_action_sel);
  tms_pair_action_lt->addWidget(tms_pair_action_copy);
  tms_pair_action_lt->addWidget(tms_pair_action_del);

  tms_pair_custom_text_edit = new qbinded_line_edit_t;
  tms_pair_custom_text_enabled = new qbinded_checkbox_t;
  tms_pair_delta_angle = new qbinded_float_spinbox_t;
  tms_pair_delta_angle->set_min_max_step(-90, 90, 0.1);

  tms_pair_dist_gb_lt->addRow(tr("Current"), tms_pair_cur_msr);
  tms_pair_dist_gb_lt->addRow(tr("Atom №1"), tms_pair_at1_info);
  tms_pair_dist_gb_lt->addRow(tr("Atom №2"), tms_pair_at2_info);
  tms_pair_dist_gb_lt->addRow(tr("Line style"), tms_pair_line_style);
  tms_pair_dist_gb_lt->addRow(tr("Line size"), tms_pair_line_size);
  tms_pair_dist_gb_lt->addRow(tr("Color"), tms_pair_dist_color);
  tms_pair_dist_gb_lt->addRow(tr("Enabled"), tms_pair_enabled);
  tms_pair_dist_gb_lt->addRow(tr("Label enabled"), tms_pair_label_enabled);
  tms_pair_dist_gb_lt->addRow(tr("Label style"), tms_pair_label_style);
  tms_pair_dist_gb_lt->addRow(tr("Custom lbl."), tms_pair_custom_text_enabled);
  tms_pair_dist_gb_lt->addRow(tr("Custom lbl. text"), tms_pair_custom_text_edit);
  tms_pair_dist_gb_lt->addRow(tr("Font size(pt)"), tms_font_screen_size);
  tms_pair_dist_gb_lt->addRow(tr("Delta angle"), tms_pair_delta_angle);
  tms_pair_dist_gb_lt->addRow(tr("Actions"), tms_pair_action_lt);
  init_form_lt(tms_pair_dist_gb_lt);

  tms_angle_gb = new qspoiler_widget_t(tr("Interatomic Angle"));
  tms_angle_gb_lt = new QFormLayout;
  tms_angle_gb->add_content_layout(tms_angle_gb_lt);
  tms_angle_cur_msr = new QComboBox;
  tms_angle_cur_msr->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w_v2());

  connect(tms_angle_cur_msr,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::msr_angle_cur_idx_changed);

  tms_angle_enabled = new qbinded_checkbox_t;
  tms_angle_order = new qbinded_int_spinbox_t;
  tms_angle_order->set_min_max_step(0, 10, 1);

  tms_angle_at1_info = new QLabel;
  tms_angle_at2_info = new QLabel;
  tms_angle_at3_info = new QLabel;
  tms_angle_gb_lt->addRow(tr("Current"), tms_angle_cur_msr);
  tms_angle_gb_lt->addRow(tr("Atom №1"), tms_angle_at1_info);
  tms_angle_gb_lt->addRow(tr("Atom №2"), tms_angle_at2_info);
  tms_angle_gb_lt->addRow(tr("Atom №3"), tms_angle_at3_info);
  tms_angle_gb_lt->addRow(tr("Enabled"), tms_angle_enabled);
  tms_angle_gb_lt->addRow(tr("Order"), tms_angle_order);
  init_form_lt(tms_angle_gb_lt);

  tab_measurement->tab_inner_widget_lt->addWidget(tms_common_settings_gb);
  tab_measurement->tab_inner_widget_lt->addWidget(tms_pair_dist_gb);
  tab_measurement->tab_inner_widget_lt->addWidget(tms_angle_gb);
  tab_measurement->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_modify_tab() {

  app_state_t *astate = app_state_t::get_inst();

  tm_gb_add_atom = new qspoiler_widget_t(tr("Add Atom"));
  tm_gb_add_atom_lt = new QFormLayout;
  tm_gb_add_atom->add_content_layout(tm_gb_add_atom_lt);

  tm_add_atom_combo = new QComboBox;
  tm_add_atom_combo->setEditable(true);
  tm_add_atom_combo->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  tm_add_atom_vec3 = new qbinded_float3_input_t;
  tm_add_atom_vec3->set_min_max_step(-1000, 1000, 0.01);

  tm_add_atom_button = new QPushButton(tr("Add"));
  tm_add_atom_button->setMaximumWidth(astate->size_guide.obj_insp_button_w());
  connect(tm_add_atom_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_add_atom_button_clicked);

  tm_gb_add_atom_lt->addRow(tr("Atom name"), tm_add_atom_combo);
  tm_gb_add_atom_lt->addRow(tr("Atom pos.[%1]").arg(astate->m_spatial_suffix), tm_add_atom_vec3);
  tm_gb_add_atom_lt->addRow("", tm_add_atom_button);
  init_form_lt(tm_gb_add_atom_lt);

  tm_gb_single_atom = new qspoiler_widget_t(tr("Modify Single Atom"));
  tm_gb_single_atom_lt = new QFormLayout;
  tm_gb_single_atom_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_single_atom->add_content_layout(tm_gb_single_atom_lt);
  tm_single_atom_combo = new QComboBox;
  tm_single_atom_combo->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  tm_single_atom_combo->setEditable(true);
  tm_single_atom_vec3 = new qbinded_float3_input_t;
  tm_single_atom_vec3->set_min_max_step(-10000, 10000, 0.01);
  tm_single_atom_info = new QLabel;

  tm_single_atom_commit = new QPushButton(tr("Commit"));
  tm_single_atom_commit->setMaximumWidth(astate->size_guide.obj_insp_button_w());
  connect(tm_single_atom_commit,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_single_atom_button_clicked);

  tm_single_atom_delete = new QPushButton(tr("Delete"));
  tm_single_atom_delete->setMaximumWidth(astate->size_guide.obj_insp_button_w());
  connect(tm_single_atom_delete,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_single_atom_delete_button_clicked);

  tm_single_atom_btn_lt = new QHBoxLayout;
  tm_single_atom_btn_lt->addWidget(tm_single_atom_commit);
  tm_single_atom_btn_lt->addWidget(tm_single_atom_delete);

  tm_gb_single_atom_lt->addRow(tr("Atom name"), tm_single_atom_combo);
  tm_gb_single_atom_lt->addRow(tr("Atom info"), tm_single_atom_info);
  tm_gb_single_atom_lt->addRow(tr("Atom pos.[%1]").arg(astate->m_spatial_suffix),
                               tm_single_atom_vec3);
  tm_gb_single_atom_lt->addRow("", tm_single_atom_btn_lt);
  init_form_lt(tm_gb_single_atom_lt);

  tm_gb_override_atom = new qspoiler_widget_t(tr("Override Atom"));
  tm_gb_override_atom_lt = new QFormLayout;
  tm_gb_override_atom->add_content_layout(tm_gb_override_atom_lt);
  tm_override_atom_info = new QLabel;
  tm_override_atom_color = new qbinded_xgeom_color3_input_t(this);
  tm_override_atom_radii = new qbinded_xgeom_float_spinbox_t(this);
  tm_override_atom_radii->set_min_max_step(0.01, 5, 0.1);
  tm_gb_override_atom_lt->addRow(tr("Atom info"), tm_override_atom_info);
  tm_gb_override_atom_lt->addRow(tr("Atom color"), tm_override_atom_color);
  tm_gb_override_atom_lt->addRow(tr("Atom radius"), tm_override_atom_radii);
  init_form_lt(tm_gb_override_atom_lt);

  tm_gb_pair_dist = new qspoiler_widget_t(tr("Pair Distance"));
  tm_gb_pair_dist_lt = new QFormLayout;
  tm_gb_pair_dist_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_pair_dist->add_content_layout(tm_gb_pair_dist_lt);
  tm_pair_dist_atom1 = new QLabel;
  tm_pair_dist_atom2 = new QLabel;
  tm_pair_dist_spinbox = new QDoubleSpinBox;
  tm_pair_dist_spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
  tm_pair_dist_note_label = new QLabel;
  tm_pair_dist_t_mode = new QComboBox;
  tm_pair_dist_t_mode->addItem("Transform both");
  tm_pair_dist_t_mode->addItem("Fix first");
  tm_pair_dist_t_mode->addItem("Fix second");
  tm_pair_dist_t_mode->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());

  tm_pair_dist_note_label->setText(tr("Distance"));
  tm_pair_dist_spinbox->setMinimum(0.0);
  tm_pair_dist_spinbox->setMaximum(10);
  tm_pair_dist_spinbox->setSingleStep(0.005);
  tm_pair_dist_spinbox->setDecimals(3);
  tm_pair_dist_spinbox->setLocale(QLocale::C);
  tm_pair_dist_spinbox->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  tm_pair_dist_swap_atoms = new QPushButton(tr("Swap"));
  tm_pair_dist_swap_atoms->setMaximumWidth(astate->size_guide.obj_insp_button_w());

  connect(tm_pair_dist_swap_atoms,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_pair_dist_swap_button_clicked);

  tm_pair_dist_cmb_lt = new QHBoxLayout;
  tm_pair_dist_cmb_lt->addWidget(tm_pair_dist_spinbox);
  tm_pair_dist_cmb_lt->addWidget(tm_pair_dist_swap_atoms);

  tm_gb_pair_dist_lt->addRow(tr("Atom №1"), tm_pair_dist_atom1);
  tm_gb_pair_dist_lt->addRow(tr("Atom №2"), tm_pair_dist_atom2);
  tm_gb_pair_dist_lt->addRow(tr("Trnsf. mode"), tm_pair_dist_t_mode);
  tm_gb_pair_dist_lt->addRow(tm_pair_dist_note_label, tm_pair_dist_cmb_lt);
  init_form_lt(tm_gb_pair_dist_lt);

  tm_gb_pair_creation = new qspoiler_widget_t(tr("Insert Atom Between"));
  tm_gb_pair_creation_lt = new QFormLayout;
  tm_gb_pair_creation_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_pair_creation->add_content_layout(tm_gb_pair_creation_lt);
  tm_pair_creation_combo = new QComboBox;
  tm_pair_creation_combo->setEditable(true);
  tm_pair_creation_combo->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  tm_pair_creation_button = new QPushButton("Append");
  tm_pair_creation_button->setAutoRepeat(true);
  tm_pair_creation_button->setAutoRepeatDelay(10000);
  tm_pair_creation_button->setAutoRepeatInterval(10000);
  tm_pair_creation_button->setMaximumWidth(astate->size_guide.obj_insp_button_w());

  tm_pair_creation_combo_btn_lt = new QHBoxLayout;
  tm_pair_creation_combo_btn_lt->addWidget(tm_pair_creation_combo);
  tm_pair_creation_combo_btn_lt->addWidget(tm_pair_creation_button);
  tm_gb_pair_creation_lt->addRow(tr("New atom"), tm_pair_creation_combo_btn_lt);
  init_form_lt(tm_gb_pair_creation_lt);

  connect(tm_pair_creation_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_add_atom_between_pair);

  connect(tm_pair_dist_spinbox,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &geom_view_obj_insp_widget_t::modify_pair_dist_spinbox_value_changed);

  tm_gb_u_scale = new qspoiler_widget_t(tr("Barycentric Scaling"));
  tm_gb_u_scale_lt = new QFormLayout;
  tm_gb_u_scale_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_u_scale->add_content_layout(tm_gb_u_scale_lt);

  tm_u_scale_vec = new qbinded_float3_input_t;
  tm_u_scale_vec->set_min_max_step(0.01, 10, 0.01);
  tm_u_scale_vec->bind_value(&tm_u_scale_vec_val);

  tm_u_apply_scale_button = new QPushButton(tr("Apply"));
  tm_u_apply_scale_button->setMaximumWidth(astate->size_guide.obj_insp_button_w());

  connect(tm_u_apply_scale_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_barycentric_scale_button_clicked);

  tm_gb_u_scale_lt->addRow("Scale ", tm_u_scale_vec);
  tm_gb_u_scale_lt->addRow("", tm_u_apply_scale_button);
  init_form_lt(tm_gb_u_scale_lt);

  tm_gb_translate = new qspoiler_widget_t(tr("Translate Selected Atoms"));
  tm_gb_translate_lt = new QFormLayout;
  tm_gb_translate_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_translate->add_content_layout(tm_gb_translate_lt);
  tm_translate_vec3 = new qbinded_float3_input_t;
  tm_translate_vec3->set_min_max_step(-10000, 10000, 0.01);

  tm_translate_apply_button = new QPushButton(tr("Apply"));
  tm_translate_apply_button->setMaximumWidth(astate->size_guide.obj_insp_button_w());

  tm_translate_coord_type = new QComboBox;
  tm_translate_coord_type->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  tm_translate_coord_type->addItem("Cart.");
  tm_translate_coord_type->addItem("Frac.");

  tm_translate_combo_btn_lt = new QHBoxLayout;
  tm_translate_combo_btn_lt->addWidget(tm_translate_coord_type);
  tm_translate_combo_btn_lt->addWidget(tm_translate_apply_button);

  tm_gb_translate_lt->addRow(tr("Tr. vector"), tm_translate_vec3);
  tm_gb_translate_lt->addRow("", tm_translate_combo_btn_lt);
  init_form_lt(tm_gb_translate_lt);

  connect(tm_translate_coord_type,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::modify_translate_coord_type_changed);

  connect(tm_translate_apply_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_translate_selected_atoms_clicked);

  tm_gb_bc_rot = new qspoiler_widget_t(tr("Rotate Selected Atoms"));
  tm_gb_bc_rot_lt = new QFormLayout;
  tm_gb_bc_rot_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_bc_rot->add_content_layout(tm_gb_bc_rot_lt);

  tm_bc_rot_axis = new QComboBox;
  tm_bc_rot_axis->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  tm_bc_rot_axis->addItem("X");
  tm_bc_rot_axis->addItem("Y");
  tm_bc_rot_axis->addItem("Z");
  tm_bc_rot_axis->setItemData(0, QBrush(Qt::red), Qt::TextColorRole);
  tm_bc_rot_axis->setItemData(1, QBrush(Qt::green), Qt::TextColorRole);
  tm_bc_rot_axis->setItemData(2, QBrush(Qt::blue), Qt::TextColorRole);

  tm_bc_rot_angle = new QDoubleSpinBox;
  tm_bc_rot_angle->setButtonSymbols(QAbstractSpinBox::NoButtons);
  tm_bc_rot_angle->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  tm_bc_rot_angle->setMinimum(-1000);
  tm_bc_rot_angle->setMaximum(1000);
  tm_bc_rot_angle->setLocale(QLocale::C);

  tm_bc_rot_angle_type = new QComboBox;
  tm_bc_rot_angle_type->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  connect(tm_bc_rot_angle_type,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::modify_bc_rot_angle_type_change);

  tm_bc_rot_angle_type->addItem("Degr.");
  tm_bc_rot_angle_type->addItem("Rad.");
  tm_bc_rot_apply = new QPushButton(tr("Apply"));
  tm_bc_rot_apply->setMaximumWidth(astate->size_guide.obj_insp_button_w());
  connect(tm_bc_rot_apply,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_bc_rot_apply);

  tm_bc_rot_cmb_lt1 = new QHBoxLayout;
  tm_bc_rot_cmb_lt1->addWidget(tm_bc_rot_angle);
  tm_bc_rot_cmb_lt1->addWidget(tm_bc_rot_angle_type);
  tm_bc_rot_cmb_lt2 = new QHBoxLayout;
  tm_bc_rot_cmb_lt2->addWidget(tm_bc_rot_axis);
  tm_bc_rot_cmb_lt2->addWidget(tm_bc_rot_apply);

  tm_gb_bc_rot_lt->addRow(tr("Angle"), tm_bc_rot_cmb_lt1);
  tm_gb_bc_rot_lt->addRow("", tm_bc_rot_cmb_lt2);
  init_form_lt(tm_gb_bc_rot_lt);

  tm_gb_group_op = new qspoiler_widget_t(tr("Group Operations"));
  tm_group_op_lt = new QGridLayout;
  tm_group_op_lt->setContentsMargins(5, 0, 5, 0);
  tm_gb_group_op->add_content_layout(tm_group_op_lt);

  tm_group_op_sv_show = new QPushButton(tr("SV:SHOW"));
  connect(tm_group_op_sv_show,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_group_op_sv_show);

  tm_group_op_sv_hide = new QPushButton(tr("SV:HIDE"));
  connect(tm_group_op_sv_hide,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_group_op_sv_hide);

  tm_group_op_sv_hide_invert = new QPushButton(tr("SV:HIDE INV"));
  connect(tm_group_op_sv_hide_invert,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_group_op_sv_hide_invert);

  tm_group_op_flip_a_p = new QPushButton(tr("FLIP:+A"));
  connect(tm_group_op_flip_a_p,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(0, 1);});

  tm_group_op_flip_b_p = new QPushButton(tr("FLIP:+B"));
  connect(tm_group_op_flip_b_p,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(1, 1);});

  tm_group_op_flip_c_p = new QPushButton(tr("FLIP:+C"));
  connect(tm_group_op_flip_c_p,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(2, 1);});

  tm_group_op_flip_a_n = new QPushButton(tr("FLIP:-A"));
  connect(tm_group_op_flip_a_n,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(0, -1);});

  tm_group_op_flip_b_n = new QPushButton(tr("FLIP:-B"));
  connect(tm_group_op_flip_b_n,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(1, -1);});

  tm_group_op_flip_c_n = new QPushButton(tr("FLIP:-C"));
  connect(tm_group_op_flip_c_n,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(2, -1);});

  tm_group_op_sel_ngbs = new QPushButton(tr("SEL:NGB"));

  connect(tm_group_op_sel_ngbs,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_group_op_sel_ngbs);

  tm_group_op_del_sel = new QPushButton(tr("SEL:DEL"));

  connect(tm_group_op_del_sel,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_group_op_del_sel);

  tm_group_make_animable = new QPushButton(tr("MAKE STC"));
  connect(tm_group_make_animable,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_group_op_make_static_anim);

  tm_group_op_lt->addWidget(tm_group_op_sv_show,        0, 0, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_sv_hide,        0, 1, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_sv_hide_invert, 0, 2, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_sel_ngbs,       1, 0, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_del_sel,        1, 1, 1, 1);
  tm_group_op_lt->addWidget(tm_group_make_animable,     1, 2, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_flip_a_p,       2, 0, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_flip_b_p,       2, 1, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_flip_c_p,       2, 2, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_flip_a_n,       3, 0, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_flip_b_n,       3, 1, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_flip_c_n,       3, 2, 1, 1);

  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_add_atom);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_override_atom);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_single_atom);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_pair_dist);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_pair_creation);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_u_scale);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_translate);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_bc_rot);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_group_op);

  tab_modify->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_select_tab() {

  app_state_t *astate = app_state_t::get_inst();

  // general settings
  ts_gb_general = new qspoiler_widget_t(tr("Selection Groups"));
  ts_gb_general_lt = new QFormLayout;
  ts_gb_general->add_content_layout(ts_gb_general_lt);

  ts_total_groups = new QLabel;
  ts_auto_apply = new qbinded_checkbox_t;
  ts_cur_sel_grp = new QComboBox;
  ts_cur_sel_grp->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());

  ts_gb_general_lt->addRow(tr("Total groups"), ts_total_groups);
  ts_gb_general_lt->addRow(tr("Auto apply"), ts_auto_apply);
  ts_gb_general_lt->addRow(tr("Current group"), ts_cur_sel_grp);

  init_form_lt(ts_gb_general_lt);

  // detail settings
  ts_gb_sel_grp_details = new qspoiler_widget_t(tr("Selection Group - Details"));
  ts_gb_sel_grp_details_lt = new QFormLayout;
  ts_gb_sel_grp_details->add_content_layout(ts_gb_sel_grp_details_lt);

  tab_select->tab_inner_widget_lt->addWidget(ts_gb_general);
  tab_select->tab_inner_widget_lt->addWidget(ts_gb_sel_grp_details);
  tab_select->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  auto _tmp = _binding_item->cast_as<geom_view_t>();

  if (_tmp) {
      b_al = _tmp;
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void geom_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_al) {

      //General tab
      tg_geom_summary_total_atoms->setText(
            QString::fromStdString(fmt::format("{}", b_al->m_geom->nat())));
      tg_geom_summary_total_types->setText(
            QString::fromStdString(fmt::format("{}", b_al->m_geom->n_atom_types())));

      //update type table
      tg_type_summary_tbl->clearContents();
      tg_type_summary_tbl->setRowCount(b_al->m_geom->n_atom_types());

      for (int i = 0; i < b_al->m_geom->n_atom_types(); i++) {

          QString n_name_str = QString::fromStdString(b_al->m_geom->atom_of_type(i));
          QTableWidgetItem *n_name = new QTableWidgetItem(n_name_str);
          n_name->setTextAlignment(Qt::AlignCenter);
          tg_type_summary_tbl->setItem(i, 0, n_name);

          QString n_c_str = tr("%1").arg(b_al->m_geom->get_atom_count_by_type(i));
          QTableWidgetItem *n_c = new QTableWidgetItem(n_c_str);
          n_c->setTextAlignment(Qt::AlignCenter);
          tg_type_summary_tbl->setItem(i, 1, n_c);

          QString n_clr_str = "";
          QTableWidgetItem *n_clr = new QTableWidgetItem(n_clr_str);
          n_clr->setTextAlignment(Qt::AlignCenter);

          auto ap_idx = ptable::number_by_symbol(b_al->m_geom->atom_of_type(i));
          vector3<float> bc(0.0, 0.0, 1.0);
          if (ap_idx) {bc = ptable::get_inst()->arecs[*ap_idx-1].m_color_jmol;}

          QColor color_bck;

          auto it = b_al->m_type_color_override.find(i);
          if (it != b_al->m_type_color_override.end())
            color_bck.setRgbF(it->second[0], it->second[1], it->second[2]);
          else color_bck.setRgbF(bc[0], bc[1], bc[2]);

          n_clr->setBackgroundColor(color_bck);
          tg_type_summary_tbl->setItem(i, 2, n_clr);

        }

      //resize type table view
      qt_hlp::vrt_resize_tv_to_cnt(tg_type_summary_tbl);

      //update cell
      cell_changed();
      qt_hlp::vrt_resize_tv_to_cnt(tg_gb_cell_tbl);

      //display tab
      disp_s_draw_atoms->bind_value(&b_al->m_draw_atoms);
      disp_s_draw_bonds->bind_value(&b_al->m_draw_bonds);
      disp_s_draw_img_atoms->bind_value(&b_al->m_draw_img_atoms);
      disp_s_draw_img_bonds->bind_value(&b_al->m_draw_img_bonds);
      disp_s_atom_scale->bind_value(&b_al->m_atom_scale_factor);
      disp_s_bond_scale->bind_value(&b_al->m_bond_scale_factor);
      disp_s_render_style->bind_value(reinterpret_cast<int*>(&b_al->m_render_style));
      disp_s_color_mode->bind_value(reinterpret_cast<int*>(&b_al->m_color_mode));

      disp_labels_style->bind_value(reinterpret_cast<int*>(&b_al->m_labels->m_style));
      disp_labels_size->bind_value(&b_al->m_labels->m_lbl_font_size);
      disp_inplace_labels->bind_value(&b_al->m_labels->m_render_inplace_hud);
      disp_inplace_offset->bind_value(&b_al->m_labels->m_inplace_offset);
      disp_sl_labels->bind_value(&b_al->m_labels->m_selective_lbl);
      disp_labels_screen_scale->bind_value(&b_al->m_labels->m_screen_scale);
      disp_labels_draw_outline->bind_value(&b_al->m_labels->m_render_outlines);
      disp_labels_outline_size->bind_value(&b_al->m_labels->m_outline_size);

      disp_shading_spec->bind_value(&b_al->m_draw_specular);
      disp_shading_spec_value->bind_value(&b_al->m_shading_specular_power);

      disp_s_sel_vis->bind_value(&b_al->m_sel_vis);
      disp_s_sel_vis_affect_bonds->bind_value(&b_al->m_sel_vis_affect_bonds);

      periodic_draw_cell_v->bind_value(&b_al->m_draw_cell_vectors);
      periodic_cell_offset->bind_value(&b_al->m_cell_vector_offset);
      periodic_cell_v_ratio->bind_value(&b_al->m_cell_vectors_ratio);
      periodic_cell_vectors_color->bind_value(&b_al->m_cell_vector_color);

      periodic_draw_cell->bind_value(&b_al->m_draw_cell);
      periodic_draw_subcells->bind_value(&b_al->m_draw_subcells);
      periodic_subcells_idx->bind_value(&b_al->m_subcells_range);
      periodic_cell_color->bind_value(&b_al->m_cell_color);
      periodic_subcells_clr->bind_value(&b_al->m_subcell_color);

      // 3d geom section
      bool _al3d = b_al->m_geom->DIM == 3;
      gb_periodic_related_render->setVisible(_al3d);
      // end 3d geom section

      //anim bindings
      update_animate_section_status();

      gb_anim_total_anims->setText(tr("%1").arg(b_al->m_anim->get_total_anims()));
      gb_rebuild_bonds->bind_value(&b_al->m_anim->m_rebuild_bonds_in_anim);
      gb_play_cyclic->bind_value(&b_al->m_anim->m_play_cyclic);
      gb_anim_speed->bind_value(&b_al->m_anim->m_anim_frame_time);

      //binding current anim combobox
      gb_current_anim->blockSignals(true);
      gb_current_anim->clear();

      for (size_t i = 0; i < b_al->m_anim->get_total_anims(); i++)
        gb_current_anim->addItem(
              QString::fromStdString(fmt::format("[{}] {}",
                                                 i,
                                                 b_al->m_anim->m_anim_data[i].m_anim_name))
              );

      gb_current_anim->setCurrentIndex(b_al->m_anim->m_cur_anim);
      gb_current_anim->blockSignals(false);
      update_anim_tab();

      anim_play->blockSignals(true);
      anim_play->setChecked(b_al->m_anim->m_play_anim);
      anim_play->blockSignals(false);

      disp_type_spec_tv->setModel(nullptr);
      disp_type_spec_mdl->bind(b_al);
      disp_type_spec_tv->setModel(disp_type_spec_mdl);
      disp_type_spec_tv->update();
      qt_hlp::vrt_resize_tv_to_cnt(disp_type_spec_tv);

      disp_bt->setModel(nullptr);
      bt_mdl->bind(b_al);
      disp_bt->setModel(bt_mdl);
      disp_bt->update();
      qt_hlp::vrt_resize_tv_to_cnt(disp_bt);

      //bind tab measurement common settings
      tms_render_dist->bind_value(&b_al->m_measure->m_render_dist);
      tms_render_angle->bind_value(&b_al->m_measure->m_render_angle);
      //end of bind tab measurement common settings

      update_modify_tab();
      update_measurement_tab();
      update_select_tab();

    }

}

void geom_view_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();

  disp_s_draw_atoms->unbind_value();
  disp_s_draw_bonds->unbind_value();
  disp_s_draw_img_atoms->unbind_value();
  disp_s_draw_img_bonds->unbind_value();
  disp_s_atom_scale->unbind_value();
  disp_s_bond_scale->unbind_value();
  disp_s_render_style->unbind_value();
  disp_s_color_mode->unbind_value();

  disp_labels_style->unbind_value();
  disp_labels_size->unbind_value();
  disp_inplace_labels->unbind_value();
  disp_inplace_offset->unbind_value();
  disp_sl_labels->unbind_value();
  disp_labels_screen_scale->unbind_value();
  disp_labels_draw_outline->unbind_value();
  disp_labels_outline_size->unbind_value();

  disp_shading_spec->unbind_value();
  disp_shading_spec_value->unbind_value();
  disp_s_sel_vis->unbind_value();
  disp_s_sel_vis_affect_bonds->unbind_value();

  periodic_draw_cell_v->unbind_value();
  periodic_cell_offset->unbind_value();
  periodic_cell_v_ratio->unbind_value();
  periodic_cell_vectors_color->unbind_value();
  periodic_draw_cell->unbind_value();
  periodic_cell_color->unbind_value();
  periodic_draw_subcells->unbind_value();
  periodic_subcells_idx->unbind_value();
  periodic_subcells_clr->unbind_value();

  gb_rebuild_bonds->unbind_value();
  gb_play_cyclic->unbind_value();
  gb_anim_speed->unbind_value();

  //tab modify spec
  tm_override_atom_color->unbind_value();
  tm_override_atom_radii->unbind_value();
  //end tab modify spec

  tms_render_dist->unbind_value();
  tms_render_angle->unbind_value();

  disp_type_spec_mdl->unbind();
  bt_mdl->unbind();

  unbind_dist_measure_tab();
  unbind_select_tab();

  b_al = nullptr;

}

void geom_view_obj_insp_widget_t::bind_dist_measure_tab() {

  if (b_al) {

      int cur_msr = b_al->m_measure->m_cur_dist_rec_ui - 1;
      if (cur_msr < b_al->m_measure->m_dist_recs.size() &&
          !b_al->m_measure->m_dist_recs.empty()) {

          auto &rec = b_al->m_measure->m_dist_recs[cur_msr];
          tms_pair_dist_color->bind_value(&rec.m_bond_color);
          tms_pair_enabled->bind_value(&rec.m_show);
          tms_pair_label_enabled->bind_value(&rec.m_show_label);
          tms_pair_line_size->bind_value(&rec.m_line_size);
          tms_font_screen_size->bind_value(&rec.m_font_size);
          tms_pair_line_style->bind_value(reinterpret_cast<int*>(&rec.m_line_render_style));
          tms_pair_label_style->bind_value(reinterpret_cast<int*>(&rec.m_label_render_style));

          tms_pair_custom_text_edit->bind_value(&rec.m_custom_label_text);
          tms_pair_custom_text_enabled->bind_value(&rec.m_show_custom_label);

          tms_pair_delta_angle->bind_value(&rec.m_delta_angle);

          tms_pair_enabled->setEnabled(true);
          tms_pair_dist_color->setEnabled(true);
          tms_pair_line_style->setEnabled(true);
          tms_pair_line_size->setEnabled(true);
          tms_font_screen_size->setEnabled(true);
          tms_pair_label_style->setEnabled(true);
          tms_pair_label_enabled->setEnabled(true);
          tms_pair_delta_angle->setEnabled(true);

          tms_pair_action_sel->setEnabled(true);
          tms_pair_action_del->setEnabled(true);
          tms_pair_action_copy->setEnabled(true);

          tms_pair_custom_text_edit->setEnabled(true);
          tms_pair_custom_text_enabled->setEnabled(true);

        } else {
          unbind_dist_measure_tab();
        }

    }

}

void geom_view_obj_insp_widget_t::unbind_dist_measure_tab() {

  tms_pair_dist_color->unbind_value();
  tms_pair_enabled->unbind_value();
  tms_pair_line_style->unbind_value();
  tms_pair_line_size->unbind_value();
  tms_font_screen_size->unbind_value();
  tms_pair_label_style->unbind_value();
  tms_pair_label_enabled->unbind_value();
  tms_pair_delta_angle->unbind_value();

  tms_pair_custom_text_enabled->unbind_value();
  tms_pair_custom_text_edit->unbind_value();

  tms_pair_line_style->setEnabled(false);
  tms_pair_dist_color->setEnabled(false);
  tms_pair_line_size->setEnabled(false);
  tms_pair_enabled->setEnabled(false);
  tms_pair_enabled->setChecked(false);
  tms_font_screen_size->setEnabled(false);
  tms_pair_label_style->setEnabled(false);
  tms_pair_label_enabled->setEnabled(false);
  tms_pair_delta_angle->setEnabled(false);

  tms_pair_action_sel->setEnabled(false);
  tms_pair_action_del->setEnabled(false);
  tms_pair_action_copy->setEnabled(false);

  tms_pair_custom_text_edit->setEnabled(false);
  tms_pair_custom_text_enabled->setEnabled(false);

}

void geom_view_obj_insp_widget_t::bind_angle_measure_tab() {

  if (b_al) {

      int cur_msr = b_al->m_measure->m_cur_angle_rec_ui - 1;
      if (cur_msr < b_al->m_measure->m_angle_recs.size() &&
          !b_al->m_measure->m_angle_recs.empty()) {
          auto &rec = b_al->m_measure->m_angle_recs[cur_msr];

          tms_angle_enabled->bind_value(&rec.m_show);
          tms_angle_enabled->setEnabled(true);

          tms_angle_order->bind_value(&rec.m_order);
          tms_angle_order->setEnabled(true);

        } else {
          unbind_angle_measure_tab();
        }

    }

}

void geom_view_obj_insp_widget_t::unbind_angle_measure_tab() {

  tms_angle_enabled->unbind_value();
  tms_angle_enabled->setEnabled(false);

  tms_angle_order->unbind_value();
  tms_angle_order->setEnabled(false);

}

void geom_view_obj_insp_widget_t::bind_select_tab() {

  ts_auto_apply->bind_value(&b_al->m_selg->m_auto_apply);

}

void geom_view_obj_insp_widget_t::unbind_select_tab() {

  ts_auto_apply->unbind_value();

}

void geom_view_obj_insp_widget_t::update_anim_tab() {

  if (b_al) {
      cur_anim_index_changed(b_al->m_anim->m_cur_anim);
      update_anim_tab_visibility();
    }

}

void geom_view_obj_insp_widget_t::update_anim_tab_visibility() {

  if (b_al) {

      auto cur_anim = b_al->m_anim->get_current_anim();
      if (cur_anim) {
          gb_anim_buttons->show();
          gb_anim_timeline->show();
        }
      else {
          gb_anim_timeline->hide();
          gb_anim_buttons->hide();
        }
    }

}

void geom_view_obj_insp_widget_t::update_modify_tab() {

  if (b_al) {

      if (b_al->m_parent_ws &&
          b_al->m_parent_ws->m_edit_type == ws_edit_e::edit_content) {

          tm_group_op_flip_a_p->setEnabled(b_al->m_geom->DIM > 0);
          tm_group_op_flip_b_p->setEnabled(b_al->m_geom->DIM > 1);
          tm_group_op_flip_c_p->setEnabled(b_al->m_geom->DIM > 2);
          tm_group_op_flip_a_n->setEnabled(b_al->m_geom->DIM > 0);
          tm_group_op_flip_b_n->setEnabled(b_al->m_geom->DIM > 1);
          tm_group_op_flip_c_n->setEnabled(b_al->m_geom->DIM > 2);

          set_tab_enabled(tab_modify, true);

          tm_override_atom_color->unbind_value();
          tm_override_atom_radii->unbind_value();

          if (b_al->m_atom_idx_sel.empty()) {
              tm_gb_single_atom->hide();
              tm_gb_add_atom->show();
              tm_gb_pair_dist->hide();
              tm_gb_u_scale->hide();
              tm_gb_pair_creation->hide();
              tm_gb_translate->hide();
              tm_gb_bc_rot->hide();
              tm_gb_group_op->hide();
              tm_gb_override_atom->hide();

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
              tm_gb_bc_rot->hide();
              tm_gb_group_op->show();
              tm_gb_override_atom->hide();

              fill_combo_with_atom_types(tm_single_atom_combo, b_al);

              auto it = b_al->m_atom_idx_sel.begin();
              if (it != b_al->m_atom_idx_sel.end()) {

                  tm_single_atom_info->setText(
                        QString::fromStdString(fmt::format("№{} {}", it->m_atm, it->m_idx)));

                  tm_single_atom_combo->setCurrentText(
                        QString::fromStdString(b_al->m_geom->atom_name(it->m_atm)));

                  tm_single_atom_vec3->sb_x->setValue(double(b_al->m_geom->pos(it->m_atm)[0]));
                  tm_single_atom_vec3->sb_y->setValue(double(b_al->m_geom->pos(it->m_atm)[1]));
                  tm_single_atom_vec3->sb_z->setValue(double(b_al->m_geom->pos(it->m_atm)[2]));

                  if (b_al->m_geom->xfield<bool>(xgeom_override, it->m_atm)) {

                      tm_override_atom_info->setText(
                            QString::fromStdString(fmt::format("{}{}",
                                                               b_al->m_geom->atom_name(it->m_atm),
                                                               it->m_atm)));

                      tm_gb_override_atom->show();
                      tm_override_atom_color->bind_value(b_al->m_geom.get(),
                      {xgeom_ccr, xgeom_ccg, xgeom_ccb},
                                                         it->m_atm);
                      tm_override_atom_radii->bind_value(b_al->m_geom.get(),
                                                         xgeom_atom_r,
                                                         it->m_atm);

                    }

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
              tm_gb_bc_rot->show();
              tm_gb_group_op->show();
              tm_gb_override_atom->hide();

              auto it1 = b_al->m_atom_idx_sel.begin();
              auto it2 = it1++;
              if (it2 != b_al->m_atom_idx_sel.end()) {

                  tm_pair_dist_atom1->setText(
                        QString::fromStdString(fmt::format("{}{} {}",
                                                           b_al->m_geom->atom_name(it1->m_atm),
                                                           it1->m_atm,
                                                           it1->m_idx)));
                  tm_pair_dist_atom2->setText(
                        QString::fromStdString(fmt::format("{}{} {}",
                                                           b_al->m_geom->atom_name(it2->m_atm),
                                                           it2->m_atm,
                                                           it1->m_idx)));

                  if (it1->m_idx == index::D(b_al->m_geom->DIM).all(0) &&
                      it2->m_idx == index::D(b_al->m_geom->DIM).all(0)) {
                      tm_pair_dist_spinbox->show();
                      float dist_btw = (b_al->m_geom->pos(it1->m_atm, it1->m_idx) -
                                        b_al->m_geom->pos(it2->m_atm, it2->m_idx)).norm();

                      tm_pair_dist_spinbox->blockSignals(true);
                      tm_pair_dist_spinbox->setValue(double(dist_btw));
                      tm_pair_dist_spinbox->blockSignals(false);
                      tm_pair_dist_note_label->show();
                      tm_pair_dist_spinbox->show();
                      tm_pair_dist_swap_atoms->show();
                    } else {
                      tm_pair_dist_note_label->hide();
                      tm_pair_dist_spinbox->hide();
                      tm_pair_dist_swap_atoms->hide();
                    }

                  //atom between
                  fill_combo_with_atom_types(tm_pair_creation_combo, b_al);
                }
            }

          if (b_al->m_atom_idx_sel.size() > 2) {

              tm_gb_add_atom->hide();
              tm_gb_single_atom->hide();
              tm_gb_pair_creation->hide();
              tm_gb_pair_dist->hide();
              tm_gb_u_scale->show();
              tm_gb_translate->show();
              tm_gb_bc_rot->show();
              tm_gb_group_op->show();
              tm_gb_override_atom->hide();

            }

          if (b_al->m_atom_idx_sel.size() > 0) {
              if (b_al->m_geom->DIM == 3) {
                  //tm_translate_coord_type_label->show();
                  tm_translate_coord_type->show();
                } else {
                  tm_translate_coord_type->setCurrentIndex(0);
                  tm_translate_coord_type->hide();
                  //tm_translate_coord_type_label->hide();
                }
            }

        } else {

          set_tab_enabled(tab_modify, false);

          tm_gb_add_atom->hide();
          tm_gb_pair_creation->hide();
          tm_gb_single_atom->hide();
          tm_gb_pair_dist->hide();
          tm_gb_u_scale->hide();
          tm_gb_translate->hide();
          tm_gb_bc_rot->hide();
          tm_gb_group_op->hide();
          tm_gb_override_atom->hide();

        }

    }

}

void geom_view_obj_insp_widget_t::update_measurement_tab() {

  if (b_al) {

      tms_pair_cur_msr->blockSignals(true);
      tms_pair_cur_msr->clear();
      tms_pair_cur_msr->addItem(tr("None"));
      index zero = index::D(b_al->m_geom->DIM).all(0);
      for (size_t i = 0; i < b_al->m_measure->m_dist_recs.size(); i++) {
          //compose dist msr name
          int at1 = b_al->m_measure->m_dist_recs[i].m_at1;
          int at2 = b_al->m_measure->m_dist_recs[i].m_at2;
          bool at1_img = b_al->m_measure->m_dist_recs[i].m_idx1 != zero;
          bool at2_img = b_al->m_measure->m_dist_recs[i].m_idx2 != zero;
          std::string item_name = fmt::format("[{}] {}{}{} - {}{}{}",
                                              i,
                                              b_al->m_geom->atom_name(at1),
                                              at1,
                                              at1_img ? "*" : "",
                                              b_al->m_geom->atom_name(at2),
                                              at2,
                                              at2_img ? "*" : "");
          tms_pair_cur_msr->addItem(QString::fromStdString(item_name));
        }

      tms_pair_cur_msr->setCurrentIndex(b_al->m_measure->m_cur_dist_rec_ui);
      update_dist_measurement_tab_info();
      tms_pair_cur_msr->blockSignals(false);

      //angle recs
      tms_angle_cur_msr->blockSignals(true);
      tms_angle_cur_msr->clear();
      tms_angle_cur_msr->addItem(tr("None"));
      for (size_t i = 0; i < b_al->m_measure->m_angle_recs.size(); i++) {

          int at1 = b_al->m_measure->m_angle_recs[i].m_at1;
          int at2 = b_al->m_measure->m_angle_recs[i].m_at2;
          int at3 = b_al->m_measure->m_angle_recs[i].m_at3;

          bool at1_img = b_al->m_measure->m_angle_recs[i].m_idx1 != zero;
          bool at2_img = b_al->m_measure->m_angle_recs[i].m_idx2 != zero;
          bool at3_img = b_al->m_measure->m_angle_recs[i].m_idx2 != zero;

          std::string item_name = fmt::format("[{}] {}{}{} - {}{}{} - {}{}{}",
                                              i,
                                              b_al->m_geom->atom_name(at1),
                                              at1,
                                              at1_img ? "*" : "",
                                              b_al->m_geom->atom_name(at2),
                                              at2,
                                              at2_img ? "*" : "",
                                              b_al->m_geom->atom_name(at3),
                                              at3,
                                              at3_img ? "*" : "");

          tms_angle_cur_msr->addItem(QString::fromStdString(item_name));

        }


      tms_angle_cur_msr->setCurrentIndex(b_al->m_measure->m_cur_angle_rec_ui);
      update_angle_measurement_tab_info();
      tms_angle_cur_msr->blockSignals(false);

    }

}

void geom_view_obj_insp_widget_t::update_dist_measurement_tab_info() {

  if (b_al) {

      //transform dist msr id
      int cur_dist_msr = b_al->m_measure->m_cur_dist_rec_ui - 1;

      if (cur_dist_msr < 0 || b_al->m_measure->m_dist_recs.empty()) {

          const QString empty_label = "-";
          tms_pair_at1_info->setText(empty_label);
          tms_pair_at2_info->setText(empty_label);
          unbind_dist_measure_tab();

        } else {

          auto &rec = b_al->m_measure->m_dist_recs[cur_dist_msr];

          QString atom1_info = tr("%1 [%2] %3")
                               .arg(QString::fromStdString(b_al->m_geom->atom_name(rec.m_at1)))
                               .arg(rec.m_at1)
                               .arg(QString::fromStdString(fmt::format("{}",rec.m_idx1)));
          tms_pair_at1_info->setText(atom1_info);

          QString atom2_info = tr("%1 [%2] %3")
                               .arg(QString::fromStdString(b_al->m_geom->atom_name(rec.m_at2)))
                               .arg(rec.m_at2)
                               .arg(QString::fromStdString(fmt::format("{}",rec.m_idx2)));
          tms_pair_at2_info->setText(atom2_info);

          bind_dist_measure_tab();

        }

    }

}

void geom_view_obj_insp_widget_t::update_angle_measurement_tab_info()  {

  if (b_al) {
      //transform dist msr id
      int cur_angle_msr = b_al->m_measure->m_cur_angle_rec_ui - 1;

      if (cur_angle_msr < 0 || b_al->m_measure->m_angle_recs.empty()) {

          const QString empty_label = "-";
          tms_angle_at1_info->setText(empty_label);
          tms_angle_at2_info->setText(empty_label);
          tms_angle_at3_info->setText(empty_label);
          unbind_angle_measure_tab();

        } else {

          auto &rec = b_al->m_measure->m_angle_recs[cur_angle_msr];

          QString atom1_info = tr("%1 [%2] %3")
                               .arg(QString::fromStdString(b_al->m_geom->atom_name(rec.m_at1)))
                               .arg(rec.m_at1)
                               .arg(QString::fromStdString(fmt::format("{}",rec.m_idx1)));
          tms_angle_at1_info->setText(atom1_info);

          QString atom2_info = tr("%1 [%2] %3")
                               .arg(QString::fromStdString(b_al->m_geom->atom_name(rec.m_at2)))
                               .arg(rec.m_at2)
                               .arg(QString::fromStdString(fmt::format("{}",rec.m_idx2)));
          tms_angle_at2_info->setText(atom2_info);

          QString atom3_info = tr("%1 [%2] %3")
                               .arg(QString::fromStdString(b_al->m_geom->atom_name(rec.m_at3)))
                               .arg(rec.m_at3)
                               .arg(QString::fromStdString(fmt::format("{}",rec.m_idx3)));
          tms_angle_at3_info->setText(atom3_info);

          bind_angle_measure_tab();

        }

    }

}

void geom_view_obj_insp_widget_t::update_select_tab() {

  if (b_al) {

      bind_select_tab();

      ts_total_groups->setText(QString("%1").arg(b_al->m_selg->m_sel_grps.size()));
      ts_cur_sel_grp->blockSignals(true);
      ts_cur_sel_grp->clear();
      ts_cur_sel_grp->addItem(tr("None"));
      for (size_t i = 0 ; i < b_al->m_selg->m_sel_grps.size(); i++)
        ts_cur_sel_grp->addItem(QString("[%1] %2")
                                .arg(i)
                                .arg(QString::fromStdString(b_al->m_selg->m_sel_grps[i].m_name)));
      ts_cur_sel_grp->blockSignals(false);

      if (b_al->m_parent_ws &&
          b_al->m_parent_ws->m_edit_type == ws_edit_e::edit_content) {
          set_tab_enabled(tab_select, true);
        } else {
          set_tab_enabled(tab_select, false);
        }

    } else {

      unbind_select_tab();

    }

}

void geom_view_obj_insp_widget_t::fill_combo_with_atom_types(QComboBox *combo,
                                                             geom_view_t *_al) {
  if (_al && combo) {
      combo->clear();
      for (auto i = 0 ; i < _al->m_geom->n_atom_types(); i++)
        combo->addItem(QString::fromStdString(_al->m_geom->atom_of_type(i)));
    }

}

void geom_view_obj_insp_widget_t::tab_modify_flip_cell_clicked(size_t flip_dim, float flip_magn) {

  if (b_al) geom_view_tools_t::flip_sel_atoms_in_cell(b_al, flip_dim, flip_magn);

}

geom_view_obj_insp_widget_t::geom_view_obj_insp_widget_t() : ws_item_obj_insp_widget_t() {

  tab_disp = def_tab(tr("Display settings"), "://images/monitor.svg");
  tab_anim = def_tab(tr("Animation"), "://images/film.svg", "://images/film_d.svg");

  tab_modify = def_tab(tr("Modify"),
                       "://images/outline-build-24px.svg",
                       "://images/outline-build-24px_d.svg");

  tab_measurement = def_tab(tr("Measurement"), "://images/outline-straighten-24px.svg");

  tab_select = def_tab(tr("Atom selection groups"),
                       "://images/outline-select_all-24px.svg",
                       "://images/outline-select_all-24px_d.svg");

  construct_general_tab();
  construct_display_tab();
  construct_anim_tab();
  construct_measure_tab();
  construct_modify_tab();
  construct_select_tab();

  app_state_t *astate = app_state_t::get_inst();

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_frame_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::cur_ws_sel_item_frame_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_cell_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::cell_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_selection_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::cur_it_list_selection_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_edit_type_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::cur_ws_edit_mode_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_selected_content_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::cur_it_selected_content_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_measurements_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::update_measurement_tab);

}

void geom_view_obj_insp_widget_t::cur_anim_index_changed(int index) {

  if (b_al) {

      if (index < int(b_al->m_anim->get_total_anims())) {

          app_state_t* astate = app_state_t::get_inst();

          if (b_al->m_anim->m_cur_anim != index) {
              b_al->m_anim->m_cur_anim = index;
              b_al->m_anim->m_cur_anim_time = 0.0f;
              b_al->m_anim->update_geom_to_anim();
            }

          auto cur_anim = b_al->m_anim->get_current_anim();

          if (cur_anim && cur_anim->m_anim_type != geom_anim_t::anim_static) {
              anim_play->blockSignals(true);
              anim_play->setChecked(b_al->m_anim->m_play_anim);
              anim_play->blockSignals(false);
            } else {
              anim_play->blockSignals(true);
              anim_play->setChecked(false);
              anim_play->blockSignals(false);
            }

          gb_anim_total_frames_in_anim->setText(tr("%1").arg(cur_anim->frames.size()));


          gb_anim_timeline_slider->setMinimum(0);
          gb_anim_timeline_slider->setMaximum(cur_anim->frames.size()-1);
          anim_updated_external();

        }

      update_anim_tab_visibility();

    }

}

void geom_view_obj_insp_widget_t::play_anim_button_toggle(bool value) {

  if (b_al) {

      b_al->m_anim->m_play_anim = value;

    }

}

void geom_view_obj_insp_widget_t::anim_updated_external() {

  if (b_al) {
      int current_frame_truncated = b_al->m_anim->current_frame_truncated();
      // remap frame id from 0..size-1 1..size
      gb_anim_cur_frame->setText(tr("%1").arg(current_frame_truncated+1));
      gb_anim_timeline_slider->blockSignals(true);
      gb_anim_timeline_slider->setValue(current_frame_truncated);
      gb_anim_timeline_slider->blockSignals(false);

    }

}

void geom_view_obj_insp_widget_t::cur_ws_sel_item_frame_changed() {
  anim_updated_external();
}

void geom_view_obj_insp_widget_t::anim_timeline_slider_value_changed(int value) {

  if (b_al) {
      auto cur_anim = b_al->m_anim->get_current_anim();
      if (cur_anim && value < cur_anim->frames.size()) {
          b_al->m_anim->update_and_set_anim(b_al->m_anim->m_cur_anim, value);
        }
    }

}

void geom_view_obj_insp_widget_t::anim_button_begin_clicked() {
  if (b_al) b_al->m_anim->update_current_frame_to_begin();
}

void geom_view_obj_insp_widget_t::anim_button_end_clicked() {
  if (b_al) b_al->m_anim->update_current_frame_to_end();
}

void geom_view_obj_insp_widget_t::anim_button_frame_move_forward_clicked() {
  if (b_al) b_al->m_anim->manual_frame_manipulate(1);
}

void geom_view_obj_insp_widget_t::anim_button_frame_move_backward_clicked() {
  if (b_al) b_al->m_anim->manual_frame_manipulate(-1);
}

void geom_view_obj_insp_widget_t::anim_act_ren_clicked() {

  if (!b_al || !b_al->m_anim->animable()) return;

  auto cur_anim = b_al->m_anim->get_current_anim();
  if (!cur_anim) return;

  QString cur_name = QString::fromStdString(cur_anim->m_anim_name);

  bool ok;
  QString new_name = QInputDialog::getText(this,
                                           tr("QInputDialog::getText()"),
                                           tr("New animation name:"),
                                           QLineEdit::Normal,
                                           cur_name,
                                           &ok);

  if (ok && !new_name.isEmpty()) {
      cur_anim->m_anim_name = new_name.toStdString();
      update_from_ws_item();
    }

}

void geom_view_obj_insp_widget_t::anim_act_del_clicked() {

  if (!b_al || !b_al->m_anim->animable()) return;

  if (b_al->m_anim->get_total_anims() == 1) {
      QMessageBox::warning(nullptr,
                           "Warning",
                           "Cannot delete the last animation. \n"
                           "Please make geom. view non animable first!");
      return;
    }

  size_t cur_anim_idx = b_al->m_anim->m_cur_anim;
  if (cur_anim_idx >= b_al->m_anim->get_total_anims()) {
      return;
    }

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, tr("Delete animation"),
                                tr("Do you really want to delete the current animation?"),
                                QMessageBox::Yes | QMessageBox::No);

  if (reply == QMessageBox::No) {
      return;
    }

  b_al->m_anim->m_anim_data.erase(b_al->m_anim->m_anim_data.begin() + cur_anim_idx);

  int new_anim_idx =
      std::clamp<int>(cur_anim_idx - 1, 0, b_al->m_anim->m_anim_data.size());
  b_al->m_anim->m_cur_anim = new_anim_idx;
  update_from_ws_item();

}

void geom_view_obj_insp_widget_t::cell_changed() {

  if (b_al) {

      if (b_al->m_geom->DIM > 0) {

          tg_gb_cell_tbl->setRowCount(b_al->m_geom->DIM);
          QStringList table_hdr_cell_v;
          table_hdr_cell_v.push_back("a");
          table_hdr_cell_v.push_back("b");
          table_hdr_cell_v.push_back("c");
          tg_gb_cell_tbl->setVerticalHeaderLabels(table_hdr_cell_v);

          for (int c = 0; c < b_al->m_geom->DIM; c++)
            for (int i = 0; i < 3; i++) {
                float cell_amp = b_al->m_geom->cell.v[c][i];
                QTableWidgetItem *n_ax = new QTableWidgetItem(tr("%1").arg(double(cell_amp)));
                tg_gb_cell_tbl->setItem(c, i, n_ax);
              }

          tg_gb_cell_tbl->resizeRowsToContents();
          tg_gb_cell->show();
        } else {
          tg_gb_cell->hide();
        }
    }

}

void geom_view_obj_insp_widget_t::draw_subcells_changed(int state) {

  qt_hlp::form_lt_ctrl_visibility(state == Qt::Checked,
                                  gb_periodic_related_render_lt,
                                  periodic_draw_subcells,
                                  periodic_subcells_idx_lbl,
                                  periodic_subcells_idx);

  qt_hlp::form_lt_ctrl_visibility(state == Qt::Checked,
                                  gb_periodic_related_render_lt,
                                  periodic_subcells_idx,
                                  periodic_subcells_color_lbl,
                                  periodic_subcells_clr);

}

void geom_view_obj_insp_widget_t::update_animate_section_status() {

  set_tab_enabled(tab_anim, b_al->m_anim->animable());

}

void geom_view_obj_insp_widget_t::cur_it_list_selection_changed() {

  if (b_al && b_al->is_selected()) {
      update_modify_tab();
    }

}

void geom_view_obj_insp_widget_t::modify_add_atom_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      vector3<float> new_atom_pos{
        float(tm_add_atom_vec3->sb_x->value()),
            float(tm_add_atom_vec3->sb_y->value()),
            float(tm_add_atom_vec3->sb_z->value())
      };
      std::string new_atom_name = tm_add_atom_combo->currentText().toStdString();
      b_al->ins_atom(new_atom_name, new_atom_pos);
      update_animate_section_status();
      astate->make_viewport_dirty();
    }
}

void geom_view_obj_insp_widget_t::modify_single_atom_button_clicked() {

  if (b_al && b_al->m_atom_idx_sel.size() == 1) {
      auto it = b_al->m_atom_idx_sel.begin();
      if (it != b_al->m_atom_idx_sel.end()) {
          auto itv = *it;
          b_al->upd_atom(itv.m_atm, tm_single_atom_combo->currentText().toStdString(),
                         vector3<float>(float(tm_single_atom_vec3->sb_x->value()),
                                        float(tm_single_atom_vec3->sb_y->value()),
                                        float(tm_single_atom_vec3->sb_z->value())));
          update_animate_section_status();
        }
    }
}

void geom_view_obj_insp_widget_t::modify_single_atom_delete_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      b_al->delete_selected_atoms();
      astate->make_viewport_dirty();
      update_animate_section_status();
    }
}

void geom_view_obj_insp_widget_t::modify_pair_dist_spinbox_value_changed(double newval) {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      auto it1 = b_al->m_atom_idx_sel.begin();
      auto it2 = it1++;
      if (it2 != b_al->m_atom_idx_sel.end() &&
          it1->m_idx == index::D(b_al->m_geom->DIM).all(0) &&
          it2->m_idx == index::D(b_al->m_geom->DIM).all(0)) {

          pair_dist_mode_e mode;
          mode = static_cast<pair_dist_mode_e>(tm_pair_dist_t_mode->currentIndex());
          b_al->update_inter_atomic_dist(float(newval), it1->m_atm, it2->m_atm, mode);

          astate->make_viewport_dirty();
        }
    }
}

void geom_view_obj_insp_widget_t::modify_pair_dist_swap_button_clicked() {

  if (b_al && b_al->m_atom_idx_sel.size() == 2) {
      auto it1 = b_al->m_atom_idx_sel.begin();
      auto it2 = it1++;
      if (it1->m_idx == index::D(b_al->m_geom->DIM).all(0) &&
          it2->m_idx == index::D(b_al->m_geom->DIM).all(0))
        b_al->swap_atoms(it1->m_atm, it2->m_atm);
    }

}

void geom_view_obj_insp_widget_t::modify_add_atom_between_pair() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      auto it1 = b_al->m_atom_idx_sel.begin();
      auto it2 = it1++;
      vector3<float> r_btw{0.0, 0.0, 0.0};

      if (it1 != b_al->m_atom_idx_sel.end() && it2 != b_al->m_atom_idx_sel.end())
        r_btw = (b_al->m_geom->pos(it1->m_atm, it1->m_idx) +
                 b_al->m_geom->pos(it2->m_atm, it2->m_idx))*0.5f;

      std::string new_atom_name = tm_pair_creation_combo->currentText().toStdString();
      b_al->ins_atom(new_atom_name, r_btw);
      update_animate_section_status();
      astate->make_viewport_dirty();
    }

}

void geom_view_obj_insp_widget_t::modify_barycentric_scale_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      vector3<float> center{0.0f, 0.0f, 0.0f};
      for (auto &rec : b_al->m_atom_idx_sel) center += b_al->m_geom->pos(rec.m_atm);
      center /= b_al->m_atom_idx_sel.size();

      for (auto &rec : b_al->m_atom_idx_sel) {

          vector3<float> new_pos_dist = center - b_al->m_geom->pos(rec.m_atm);
          vector3<float> new_pos = b_al->m_geom->pos(rec.m_atm);

          new_pos[0] +=  (1-tm_u_scale_vec_val[0]) * new_pos_dist[0];
          new_pos[1] +=  (1-tm_u_scale_vec_val[1]) * new_pos_dist[1];
          new_pos[2] +=  (1-tm_u_scale_vec_val[2]) * new_pos_dist[2] ;

          b_al->upd_atom(rec.m_atm, new_pos);

        }

      update_animate_section_status();
      astate->make_viewport_dirty();

    }

}

void geom_view_obj_insp_widget_t::modify_translate_selected_atoms_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      vector3<float> tr_vec(float(tm_translate_vec3->sb_x->value()),
                            float(tm_translate_vec3->sb_y->value()),
                            float(tm_translate_vec3->sb_z->value()));

      if (tm_translate_coord_type->currentIndex() == 1 && b_al->m_geom->DIM == 3) {
          vector3<float> tr_vec_c = tr_vec;
          tr_vec = b_al->m_geom->cell.frac2cart(tr_vec_c);
        }

      b_al->translate_selected(tr_vec);

    }

  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::modify_translate_coord_type_changed(int coord_type) {

  if (coord_type == 0) tm_translate_vec3->set_min_max_step(-100, 100, 0.01);
  else tm_translate_vec3->set_min_max_step(-1.0, 1.0, 0.01);

}

void geom_view_obj_insp_widget_t::modify_bc_rot_angle_type_change(int new_angle_type) {

  if (new_angle_type == 0) tm_bc_rot_angle->setSingleStep(0.5);
  else tm_bc_rot_angle->setSingleStep(0.01);
  tm_bc_rot_angle->setValue(0.0);

}

void geom_view_obj_insp_widget_t::modify_bc_rot_apply() {

  if (b_al && b_al->m_parent_ws && b_al->m_parent_ws->m_edit_type == ws_edit_e::edit_content) {

      float angle;

      if (tm_bc_rot_angle_type->currentIndex() == 0)
        angle = float(tm_bc_rot_angle->value() * qpp::pi) / 180.0f;
      else //bypass input value
        angle = float(tm_bc_rot_angle->value());

      vector3<float> rot_center = b_al->get_gizmo_content_barycenter();

      vector3<float> rot_axis{vector3<float>::Zero()};

      switch (tm_bc_rot_axis->currentIndex()) {
        case 0: {
            rot_axis = {1, 0, 0};
            break;
          }
        case 1: {
            rot_axis = {0, 1, 0};
            break;
          }
        default: {
            rot_axis = {0, 0, 1};
            break;
          }
        }

      Eigen::Affine3f t;
      Eigen::AngleAxisf rot(angle, rot_axis);
      Eigen::Translation<float,3> tb(-rot_center);
      Eigen::Translation<float,3> ta(rot_center);
      t = ta * rot * tb;

      matrix4<float> tm = t.matrix();

      b_al->transform_sel(tm);
      app_state_t *astate = app_state_t::get_inst();
      astate->make_viewport_dirty();

    }

}

void geom_view_obj_insp_widget_t::modify_group_op_sv_show() {

  if (b_al) {
      b_al->sv_modify_selected(false);
      update_from_ws_item();
    }

}

void geom_view_obj_insp_widget_t::modify_group_op_sv_hide() {

  if (b_al) {
      b_al->sv_modify_selected(true);
      update_from_ws_item();
    }

}

void geom_view_obj_insp_widget_t::modify_group_op_sv_hide_invert() {

  if (b_al) {
      b_al->sv_hide_invert_selected();
      update_from_ws_item();
    }

}

void geom_view_obj_insp_widget_t::modify_group_op_sel_ngbs() {

  app_state_t *astate = app_state_t::get_inst();
  if (b_al) b_al->sel_selected_atoms_ngbs();
  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::modify_group_op_del_sel() {

  app_state_t *astate = app_state_t::get_inst();
  if (b_al) b_al->delete_selected_atoms();
  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::modify_group_op_make_static_anim() {

  if (!b_al) return;

  b_al->m_anim->make_animable();

  std::string _anim_name = fmt::format("static_{}", b_al->m_anim->get_total_anims());
  b_al->m_anim->make_anim(_anim_name, geom_anim_t::anim_static, 1);

}

void geom_view_obj_insp_widget_t::msr_pair_cur_idx_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      b_al->m_measure->m_cur_dist_rec_ui = index;
      update_dist_measurement_tab_info();
    }

  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::msr_angle_cur_idx_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      b_al->m_measure->m_cur_angle_rec_ui = index;
      update_angle_measurement_tab_info();
    }

  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::msr_pair_select_clicked() {
  if (b_al) b_al->m_measure->dist_select_selected_atoms();
}

void geom_view_obj_insp_widget_t::msr_pair_delete_clicked() {
  if (b_al) b_al->m_measure->dist_delete_selected();
}

void geom_view_obj_insp_widget_t::msr_pair_copy_clicked() {
  if (b_al) b_al->m_measure->dist_copy_selected();
}

void geom_view_obj_insp_widget_t::cur_ws_edit_mode_changed() {

  update_modify_tab();
  update_measurement_tab();
  update_select_tab();

}

void geom_view_obj_insp_widget_t::cur_it_selected_content_changed() {

  if (b_al) {
      update_modify_tab();
      update_measurement_tab();
      update_select_tab();
    }

}

void geom_view_obj_insp_widget_t::type_summary_clicked(const QModelIndex &index) {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      int atom_type_idx = index.row();
      int col_idx = index.column();

      //type is valid
      if (atom_type_idx < b_al->m_geom->n_types() && col_idx == 2) {

          auto it = b_al->m_type_color_override.find(atom_type_idx);
          auto ap_idx = ptable::number_by_symbol(b_al->m_geom->atom_of_type(atom_type_idx));

          QColor _stored_color = Qt::black;

          //first load a color from ptable
          if (ap_idx && *ap_idx > 0 && *ap_idx <100) {
              _stored_color = QColor::fromRgbF(
                                ptable::get_inst()->arecs[*ap_idx-1].m_color_jmol[0],
                  ptable::get_inst()->arecs[*ap_idx-1].m_color_jmol[1],
                  ptable::get_inst()->arecs[*ap_idx-1].m_color_jmol[2]);
            }

          if (it != b_al->m_type_color_override.end())
            _stored_color = QColor::fromRgbF(it->second[0], it->second[1], it->second[2]);

          const QColor clr = QColorDialog::getColor(_stored_color, this, "Select Color");
          if (clr.isValid()) {
              if (it != b_al->m_type_color_override.end()) b_al->m_type_color_override.erase(it);
              auto new_c = std::make_pair(size_t(atom_type_idx),
                                          vector3<float>(clr.redF(), clr.greenF(), clr.blueF()));
              b_al->m_type_color_override.insert(new_c);
              update_from_ws_item();
              astate->make_viewport_dirty();
            }

        }

    }

}

void geom_view_obj_insp_widget_t::rebond_button_clicked() {

  if (!b_al) return;
  b_al->rebond();

}

void geom_view_obj_insp_widget_t::clear_color_override_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();
  if (!b_al) return;
  b_al->m_type_color_override.clear();
  update_from_ws_item();
  astate->make_viewport_dirty();

}

