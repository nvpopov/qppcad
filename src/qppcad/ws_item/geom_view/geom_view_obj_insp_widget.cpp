#include <qppcad/ws_item/geom_view/geom_view_obj_insp_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_labels_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_sel_groups_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_measurement_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_type_summary_popup.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>

#include <qppcad/ws_item/geom_view/qbonding_table_model.hpp>
#include <qppcad/ws_item/geom_view/qtype_specific_rendering_model.hpp>
#include <qppcad/ws_item/geom_view/qmeasurements_table_model.hpp>
#include <qppcad/ws_item/geom_view/xgeom_fields_model.hpp>

#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

#include <QColorDialog>

using namespace qpp;
using namespace qpp::cad;

void geom_view_obj_insp_widget_t::construct_general_tab() {

  app_state_t *astate = app_state_t::get_inst();

  m_tg_geom_sum_total_atoms = new QLabel;
  m_tg_geom_sum_total_atoms->setProperty("s_class", "thin_label");

  QLabel *tmp_lbl = new QLabel(tr("Total"));
  m_tg_form_lt->addRow(tmp_lbl, m_tg_geom_sum_total_atoms);
  qt_hlp::resize_form_lt_lbl(tmp_lbl, astate->size_guide.obj_insp_lbl_w());

  m_tg_type_sum_wdgt = new qspoiler_widget_t(tr("Type Summary"), this, true, 0,
                                             astate->size_guide.obj_insp_splr_w(), false, 0);
  m_tg_type_sum_lt = new QVBoxLayout;
  m_tg_type_sum_wdgt->add_content_layout(m_tg_type_sum_lt);
  m_tg_type_sum_tbl = new QTableWidget;
  m_tg_type_sum_tbl->setFixedWidth(astate->size_guide.obj_insp_table_w());
  m_tg_type_sum_tbl->setColumnCount(3);

  m_type_sum_clear_tclr_override = new QPushButton;
  m_type_sum_clear_tclr_override->setFixedSize(QSize(astate->size_guide.ext_editor_btn_h(),
                                                     astate->size_guide.ext_editor_btn_h()));
  m_type_sum_clear_tclr_override->setIcon(QIcon("://images/outline-refresh-24px.svg"));
  m_type_sum_clear_tclr_override->setFlat(true);
  m_type_sum_clear_tclr_override->setToolTip("Clear the color override");

  connect(m_type_sum_clear_tclr_override,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::clear_color_override_button_clicked);

  m_tg_type_sum_wdgt->m_hbox_frm->insertWidget(3, m_type_sum_clear_tclr_override);

  QStringList table_hdr_cell;
  table_hdr_cell.push_back("X");
  table_hdr_cell.push_back("Y");
  table_hdr_cell.push_back("Z");

  m_tg_gb_cell = new qspoiler_widget_t(tr("Lattice Parameters"), this, true, 0,
                                       astate->size_guide.obj_insp_splr_w(), false, 0);
  m_tg_gb_cell_lt = new QVBoxLayout;
  m_tg_gb_cell->add_content_layout(m_tg_gb_cell_lt);
  m_tg_gb_cell_tbl = new QTableWidget;

  m_tg_gb_cell_tbl->setColumnCount(3);
  m_tg_gb_cell_tbl->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  m_tg_gb_cell_tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_tg_gb_cell_tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_tg_gb_cell_tbl->verticalHeader()->setVisible(false);
  m_tg_gb_cell_tbl->setHorizontalHeaderLabels(table_hdr_cell);
  m_tg_gb_cell_tbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_tg_gb_cell_tbl->setFixedWidth(astate->size_guide.obj_insp_table_w());
  m_tg_gb_cell_tbl->setShowGrid(false);
  m_tg_gb_cell_lt->addWidget(m_tg_gb_cell_tbl);

  QStringList table_hdr;
  table_hdr.push_back("Name");
  table_hdr.push_back("Count");
  table_hdr.push_back("Color");

  m_tg_type_sum_tbl->setHorizontalHeaderLabels(table_hdr);
  m_tg_type_sum_tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_tg_type_sum_tbl->verticalHeader()->setVisible(false);
  m_tg_type_sum_tbl->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  m_tg_type_sum_tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_tg_type_sum_tbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_tg_type_sum_tbl->setShowGrid(false);
  m_tg_type_sum_lt->addWidget(m_tg_type_sum_tbl);

  connect(m_tg_type_sum_tbl,
          &QTableView::clicked,
          this,
          &geom_view_obj_insp_widget_t::type_summary_clicked);

  m_tab_general->tab_inner_widget_lt->addWidget(m_tg_type_sum_wdgt);
  m_tab_general->tab_inner_widget_lt->addWidget(m_tg_gb_cell);

  m_tab_general->tab_inner_widget_lt->addStretch();

}

void geom_view_obj_insp_widget_t::construct_disp_tab() {

  app_state_t *astate = app_state_t::get_inst();

  m_gb_disp_s = new qspoiler_widget_t(tr("Draw Settings"));
  m_gb_disp_s_lt = new QFormLayout;
  m_gb_disp_s->add_content_layout(m_gb_disp_s_lt);

  m_disp_s_scale = new qbinded_float_named_vector_t({"Atom", "Bond"});
  m_disp_s_scale->set_min_max_step(0.01, 3.0, 0.01);

  m_disp_s_render_style = new qbinded_combobox_t;
  m_disp_s_render_style->addItem("Balls and sticks");
  m_disp_s_render_style->addItem("Suprematic atoms");
  m_disp_s_render_style->addItem("Dynamic lines");
  m_disp_s_render_style->addItem("X-atom lines");
  m_disp_s_render_style->addItem("Billboards");
  m_disp_s_render_style->addItem("Buffered billboards");

  m_disp_s_color_mode = new qbinded_combobox_t;
  m_disp_s_color_mode->addItem(tr("Color from ptable"));
  m_disp_s_color_mode->addItem(tr("Color from xgeom"));

  m_disp_s_draw_atoms_bonds = new qbinded_bool_named_vector_t({"Atom", "Bond"});
  m_disp_s_draw_img_atoms_bonds = new qbinded_bool_named_vector_t({"Atom", "Bond"});

  // periodic related render
  m_gb_periodic_related_render = new qspoiler_widget_t(tr("Periodic Related Settings"));
  m_gb_periodic_related_render_lt = new QFormLayout;
  m_gb_periodic_related_render->add_content_layout(m_gb_periodic_related_render_lt);

  m_periodic_draw_settings = new qbinded_bool_named_vector_t({tr("Cell"), tr("Vectors")});
  m_periodic_draw_subcells = new qbinded_checkbox_t;

  m_periodic_cell_v_ratio = new qbinded_float_spinbox_t;
  m_periodic_cell_v_ratio->set_min_max_step(-5, 5, 0.01);

  m_periodic_subcells_idx = new qbinded_int3_input_t;
  m_periodic_subcells_idx_lbl = new QLabel(tr("Subcells range"));
  m_periodic_subcells_idx->set_min_max_step(1, 10, 1);
  m_periodic_subcells_clr = new qbinded_color3_input_t();
  m_periodic_subcells_color_lbl = new QLabel(tr("Subcell color"));

  init_form_lt_lbl(m_periodic_subcells_idx_lbl);
  init_form_lt_lbl(m_periodic_subcells_color_lbl);

  connect(m_periodic_draw_subcells,
          &qbinded_checkbox_t::stateChanged,
          this,
          &geom_view_obj_insp_widget_t::draw_subcells_changed);

  m_disp_s_sel_vis_p_affect_bonds = new qbinded_bool_named_vector_t({"Atom", "Bond"});

  //periodic_draw_cell = new qbinded_checkbox_t;
  m_periodic_cell_offset = new qbinded_float3_input_t;
  m_periodic_cell_offset->set_min_max_step_dec(-1000, 1000, 0.01);
  m_periodic_cell_color = new qbinded_color3_input_t();
  m_periodic_cell_vectors_color = new qbinded_color3_input_t;

  m_gb_periodic_related_render_lt->addRow(tr("Draw"), m_periodic_draw_settings);
  //gb_periodic_related_render_lt->addRow(tr("Draw vectors"), periodic_draw_cell_v);
  m_gb_periodic_related_render_lt->addRow(tr("Vectors ratio"), m_periodic_cell_v_ratio);
  m_gb_periodic_related_render_lt->addRow(tr("Vectors offset"), m_periodic_cell_offset);
  m_gb_periodic_related_render_lt->addRow(tr("Vectors color"), m_periodic_cell_vectors_color);
  m_gb_periodic_related_render_lt->addRow(tr("Cell color"), m_periodic_cell_color);
  m_gb_periodic_related_render_lt->addRow(tr("Draw subcells"), m_periodic_draw_subcells);

  init_form_lt(m_gb_periodic_related_render_lt);
  // end of periodic related render

  m_gb_disp_s_lt->addRow(tr("Draw style"), m_disp_s_render_style);
  m_gb_disp_s_lt->addRow(tr("Color style"), m_disp_s_color_mode);
  m_gb_disp_s_lt->addRow(tr("Draw real"), m_disp_s_draw_atoms_bonds);
  m_gb_disp_s_lt->addRow(tr("Draw img."), m_disp_s_draw_img_atoms_bonds);
  m_gb_disp_s_lt->addRow(tr("Sel. visibility"), m_disp_s_sel_vis_p_affect_bonds);
  m_gb_disp_s_lt->addRow(tr("Scale"), m_disp_s_scale);

  init_form_lt(m_gb_disp_s_lt);

  m_gb_disp_labels = new qspoiler_widget_t(tr("Labels"));
  m_gb_disp_labels_lt = new QFormLayout;
  m_gb_disp_labels->add_content_layout(m_gb_disp_labels_lt);

  m_disp_labels_style = new qbinded_combobox_t;
  m_disp_labels_style->addItem("None");
  m_disp_labels_style->addItem("Id");
  m_disp_labels_style->addItem("Type");
  m_disp_labels_style->addItem("Type & Id");
  m_disp_labels_style->addItem("Charge");
  m_disp_labels_style->addItem("Charge, Type");
  m_disp_labels_style->addItem("Charge, Type & Id");
  m_disp_labels_style->addItem("Custom");

  m_disp_labels_size = new qbinded_int_spinbox_t;
  m_disp_labels_size->set_min_max_step(5, 35, 1);
  m_disp_inplace_labels = new qbinded_checkbox_t;
  m_disp_inplace_offset = new qbinded_float2_input_t;
  m_disp_inplace_offset->set_min_max_step(0, 1, 0.01);
  m_disp_sl_labels = new qbinded_checkbox_t;
  m_disp_labels_screen_scale = new qbinded_checkbox_t;
  m_disp_labels_draw_outline = new qbinded_checkbox_t;
  m_disp_labels_outline_size = new qbinded_float_spinbox_t;
  m_disp_labels_outline_size->set_min_max_step(1, 20, 0.1);
  m_disp_labels_draw_axis_labels = new qbinded_checkbox_t;

  m_gb_disp_labels_lt->addRow(tr("Labels style"), m_disp_labels_style);
  m_gb_disp_labels_lt->addRow(tr("Labels size"), m_disp_labels_size);
  m_gb_disp_labels_lt->addRow(tr("Draw outline"), m_disp_labels_draw_outline);
  m_gb_disp_labels_lt->addRow(tr("Outline size"), m_disp_labels_outline_size);
  m_gb_disp_labels_lt->addRow(tr("Inplace labels"), m_disp_inplace_labels);
  m_gb_disp_labels_lt->addRow(tr("Inplace offset"), m_disp_inplace_offset);
  m_gb_disp_labels_lt->addRow(tr("Sel. visibility"), m_disp_sl_labels);
  m_gb_disp_labels_lt->addRow(tr("Screen scale"), m_disp_labels_screen_scale);
  m_gb_disp_labels_lt->addRow(tr("Axis labels"), m_disp_labels_draw_axis_labels);
  init_form_lt(m_gb_disp_labels_lt);

  //display - shading tab initialization
  m_gb_disp_shading = new qspoiler_widget_t(tr("Shading Settings"));
  m_gb_disp_shading_lt = new QFormLayout;
  m_gb_disp_shading->add_content_layout(m_gb_disp_shading_lt);

  m_disp_shading_spec = new qbinded_checkbox_t;
  m_disp_shading_spec_value = new qbinded_float_spinbox_t;
  m_disp_shading_spec_value->set_min_max_step(0.25, 50, 0.25);
  m_gb_disp_shading_lt->addRow(tr("Draw specular"), m_disp_shading_spec);
  m_gb_disp_shading_lt->addRow(tr("Spec. power"), m_disp_shading_spec_value);
  init_form_lt(m_gb_disp_shading_lt);

  //display - type specific rendering
  m_gb_disp_type_spec_rend = new qspoiler_widget_t(tr("Type Specific Rendering"),
                                                   this, true, 0,
                                                   astate->size_guide.obj_insp_splr_w(), false, 0);
  m_gb_disp_type_spec_rend_lt = new QVBoxLayout;
  m_gb_disp_type_spec_rend->add_content_layout(m_gb_disp_type_spec_rend_lt);

  m_disp_type_spec_tv = new QTableView;
  m_gb_disp_type_spec_rend_lt->addWidget(m_disp_type_spec_tv);

  m_disp_type_spec_tv->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  m_disp_type_spec_tv->verticalHeader()->hide();
  m_disp_type_spec_tv->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  m_disp_type_spec_tv->setFocusPolicy(Qt::NoFocus);
  m_disp_type_spec_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_disp_type_spec_tv->setFixedWidth(astate->size_guide.obj_insp_table_w());
  m_disp_type_spec_tv->setShowGrid(false);
  m_disp_type_spec_mdl = new qtype_specific_rendering_model_t;
  m_disp_type_spec_tv->setModel(m_disp_type_spec_mdl);

  //display - bonding table
  m_gb_disp_bt = new qspoiler_widget_t(tr("Bonding Table"), this, true, 0,
                                       astate->size_guide.obj_insp_splr_w(), false, 0);
  m_gb_disp_bt->setContentsMargins(0,0,0,0);
  m_disp_bt_lt = new QVBoxLayout;
  m_gb_disp_bt->add_content_layout(m_disp_bt_lt);

  m_disp_bt_rebond_btn = new QPushButton;
  m_disp_bt_rebond_btn->setFixedSize(QSize(astate->size_guide.ext_editor_btn_h(),
                                           astate->size_guide.ext_editor_btn_h()));
  m_disp_bt_rebond_btn->setIcon(QIcon("://images/outline-refresh-24px.svg"));
  m_disp_bt_rebond_btn->setFlat(true);
  m_disp_bt_rebond_btn->setToolTip("Rebond the structure");

  connect(m_disp_bt_rebond_btn,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::rebond_button_clicked);

  m_gb_disp_bt->m_hbox_frm->insertWidget(3, m_disp_bt_rebond_btn);

  m_bt_mdl = new qbonding_table_model_t;
  m_disp_bt = new QTableView;

  m_disp_bt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  m_disp_bt->verticalHeader()->hide();
  m_disp_bt->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  m_disp_bt->setFocusPolicy(Qt::NoFocus);
  m_disp_bt->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_disp_bt->setFixedWidth(astate->size_guide.obj_insp_table_w());
  m_disp_bt->setShowGrid(false);
  m_disp_bt->setModel(m_bt_mdl);
  m_disp_bt->setLocale(QLocale::C);
  m_disp_bt_lt->addWidget(m_disp_bt);
  m_bt_dist_delegate = new qrealspinbox_delegate_t;
  m_bt_dist_delegate->set_min_max_step(0.1, 5.0, 0.01);
  m_disp_bt->setItemDelegateForColumn(2, m_bt_dist_delegate);

  m_tab_disp_switch = new QButtonGroup;
  m_tab_disp_switch->setExclusive(true);

  connect(m_tab_disp_switch, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),
          this, &geom_view_obj_insp_widget_t::disp_switch_cur_changed);

  QHBoxLayout *tmp_lt = new QHBoxLayout;
  tmp_lt->setContentsMargins(4, 0, 4, 3);
  tmp_lt->setSpacing(2);

  QPushButton *tmp_b1 = new QPushButton(tr("Appearance"));
  QPushButton *tmp_b2 = new QPushButton(tr("Labels"));
  QPushButton *tmp_b3 = new QPushButton(tr("Bonding"));
  QPushButton *tmp_b4 = new QPushButton(tr("Misc"));

  int i = 0;
  for (auto btn : {tmp_b1, tmp_b2, tmp_b3, tmp_b4}) {
    tmp_lt->addWidget(btn);
    m_tab_disp_switch->addButton(btn, i);
    btn->setCheckable(true);
    i++;
  }

  m_tab_disp->tab_top_wdgt_lt->insertLayout(0, tmp_lt);

  m_tab_disp->tab_inner_widget_lt->addWidget(m_gb_disp_s);
  m_tab_disp->tab_inner_widget_lt->addWidget(m_gb_disp_shading);
  m_tab_disp->tab_inner_widget_lt->addWidget(m_gb_disp_labels);
  m_tab_disp->tab_inner_widget_lt->addWidget(m_gb_periodic_related_render);
  m_tab_disp->tab_inner_widget_lt->addWidget(m_gb_disp_type_spec_rend);
  m_tab_disp->tab_inner_widget_lt->addWidget(m_gb_disp_bt);

  m_tab_disp->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_anim_tab() {

  app_state_t *astate = app_state_t::get_inst();

  m_gb_anim_sum = new qspoiler_widget_t(tr("Summary"));
  m_gb_anim_summary_lt = new QFormLayout;
  m_gb_anim_sum->add_content_layout(m_gb_anim_summary_lt);

  m_anim_speed = new qbinded_float_spinbox_t;
  m_anim_settings = new qbinded_bool_named_vector_t({"Cycl.", "Intr.", "Rbnd."});
  m_anim_speed->set_min_max_step(0.01,10.0,0.01);

  m_anim_cur_frame = new QLabel;
  m_anim_cur_frame->setProperty("s_class", "thin_label");
  m_anim_current_anim = new QComboBox;
  m_anim_current_anim->setFixedWidth(astate->size_guide.obj_insp_combo_max_w_v2());

  connect(m_anim_current_anim,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::cur_anim_index_changed);

  m_anim_act_lt = new QHBoxLayout;
  m_anim_act_lt->setContentsMargins(5, 0, 5, 0);
  m_anim_act_ren = new QPushButton(tr("Rename"));
  connect(m_anim_act_ren,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_act_ren_clicked);

  m_anim_act_del = new QPushButton(tr("Delete"));
  connect(m_anim_act_del,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_act_del_clicked);

  m_anim_act_lt->addWidget(m_anim_act_ren);
  m_anim_act_lt->addWidget(m_anim_act_del);

  m_gb_anim_summary_lt->addRow(tr("Play Settings"), m_anim_settings);
  m_gb_anim_summary_lt->addRow(tr("Current Anim."), m_anim_current_anim);
  m_gb_anim_summary_lt->addRow(tr("Frame Time "), m_anim_speed);
  m_gb_anim_summary_lt->addRow(tr("Info"), m_anim_cur_frame);
  m_gb_anim_summary_lt->addRow(tr("Actions"), m_anim_act_lt);
  init_form_lt(m_gb_anim_summary_lt);

  m_gb_anim_timeline = new qspoiler_widget_t(tr("Timeline"));
  m_gb_anim_timeline->setMaximumWidth(astate->size_guide.obj_insp_anim_w());
  m_anim_timeline_lt = new QHBoxLayout;
  m_gb_anim_timeline->add_content_layout(m_anim_timeline_lt);

  m_anim_timeline_slider = new QSlider(Qt::Orientation::Horizontal);
  m_anim_timeline_slider->setTickPosition(QSlider::TicksBothSides);
  m_anim_timeline_slider->setTickInterval(10);

  connect(m_anim_timeline_slider,
          &QSlider::valueChanged,
          this,
          &geom_view_obj_insp_widget_t::anim_timeline_slider_value_changed);

  //gb_anim_timeline_layout->addWidget(gb_anim_timeline_cur_frame, 1);
  m_anim_timeline_lt->addWidget(m_anim_timeline_slider, 1);

  m_anim_buttons = new qspoiler_widget_t(tr("Control"));
  m_anim_buttons_lt = new QHBoxLayout;
  m_anim_buttons_lt->setContentsMargins(5, 0, 5, 0);
  m_anim_buttons->add_content_layout(m_anim_buttons_lt);
  m_anim_buttons->setMaximumWidth(astate->size_guide.obj_insp_anim_w());
  //gb_anim_buttons->setMaximumHeight(90);

  m_anim_play = new QPushButton(tr("PLAY"));
  m_anim_play->setCheckable(true);

  connect(m_anim_play,
          &QPushButton::toggled,
          this,
          &geom_view_obj_insp_widget_t::play_anim_button_toggle);

  m_anim_to_start = new QPushButton(tr("<<"));

  connect(m_anim_to_start,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_button_begin_clicked);

  m_anim_to_end = new QPushButton(tr(">>"));

  connect(m_anim_to_end,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_button_end_clicked);

  m_anim_frame_fwd = new QPushButton(tr("+F"));

  connect(m_anim_frame_fwd,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_button_frame_move_forward_clicked);

  m_anim_frame_bck = new QPushButton(tr("-F"));

  connect(m_anim_frame_bck,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::anim_button_frame_move_backward_clicked);

  m_anim_buttons_lt->addWidget(m_anim_play, 1);
  m_anim_buttons_lt->addWidget(m_anim_to_start, 1);
  m_anim_buttons_lt->addWidget(m_anim_to_end, 1);
  m_anim_buttons_lt->addWidget(m_anim_frame_fwd, 1);
  m_anim_buttons_lt->addWidget(m_anim_frame_bck, 1);

  m_tab_anim->tab_inner_widget_lt->addWidget(m_gb_anim_sum);
  m_tab_anim->tab_inner_widget_lt->addWidget(m_gb_anim_timeline);
  m_tab_anim->tab_inner_widget_lt->addWidget(m_anim_buttons);

  m_tab_anim->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_msr_tab() {

  app_state_t *astate = app_state_t::get_inst();

  m_tms_common_settings_gb = new qspoiler_widget_t(tr("Common Settings"));
  m_tms_common_settings_gb_lt = new QFormLayout;
  m_tms_common_settings_gb->add_content_layout(m_tms_common_settings_gb_lt);

  m_tms_render_dist_angle = new qbinded_bool_named_vector_t({tr("Dists"), tr("Angles")});
  m_tms_render_dist_angle_legend = new qbinded_bool_named_vector_t({tr("Dists"), tr("Angles")});

  m_tms_common_settings_gb_lt->addRow(tr("Draw"), m_tms_render_dist_angle);
  m_tms_common_settings_gb_lt->addRow(tr("Legend"), m_tms_render_dist_angle_legend);

  init_form_lt(m_tms_common_settings_gb_lt);

  m_tms_pair_dist_gb = new qspoiler_widget_t(tr("Interatomic Distance"));
  m_tms_pair_dist_gb_lt = new QFormLayout;
  m_tms_pair_dist_gb->add_content_layout(m_tms_pair_dist_gb_lt);

  m_tms_pair_cur_msr = new QComboBox;
  m_tms_pair_cur_msr->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());

  connect(m_tms_pair_cur_msr,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::msr_pair_cur_idx_changed);

  m_tms_pair_at1_info = new QLabel;
  m_tms_pair_at1_info->setProperty("s_class", "thin_label");
  m_tms_pair_at2_info = new QLabel;
  m_tms_pair_at2_info->setProperty("s_class", "thin_label");
  m_tms_pair_dist = new QLabel;
  m_tms_pair_dist->setProperty("s_class", "thin_label");

  m_tms_pair_dist_color = new qbinded_color3_input_t;
  m_tms_pair_show = new qbinded_checkbox_t;
  m_tms_pair_label_enabled = new qbinded_checkbox_t;

  m_tms_pair_line_size = new qbinded_int_spinbox_t;
  m_tms_pair_line_size->set_min_max_step(1, 20, 1);

  m_tms_font_screen_size = new qbinded_int_spinbox_t;
  m_tms_font_screen_size->set_min_max_step(5, 80, 1);

  m_tms_pair_line_style = new qbinded_combobox_t;
  m_tms_pair_line_style->addItem("Solid");
  m_tms_pair_line_style->addItem("Dotted");
  m_tms_pair_line_style->addItem("Dashed");
  m_tms_pair_line_style->addItem("Dash-Dot");
  m_tms_pair_line_style->addItem("Dash-Dot-Dot");

  m_tms_pair_label_style = new qbinded_combobox_t;
  m_tms_pair_label_style->addItem("Standart");
  m_tms_pair_label_style->addItem("Outline");
  m_tms_pair_label_style->addItem("Border");

  m_tms_pair_action_sel = new QPushButton(tr("SEL"));

  connect(m_tms_pair_action_sel,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::msr_pair_select_clicked);

  m_tms_pair_action_copy = new QPushButton(tr("COPY"));

  connect(m_tms_pair_action_copy,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::msr_pair_copy_clicked);

  m_tms_pair_action_del = new QPushButton(tr("DEL"));

  connect(m_tms_pair_action_del,
          &QPushButton::clicked,
          this,
          &geom_view_obj_insp_widget_t::msr_pair_delete_clicked);

  m_tms_pair_action_lt = new QHBoxLayout;
  m_tms_pair_action_lt->setContentsMargins(4, 0, 8, 0);
  m_tms_pair_action_lt->addWidget(m_tms_pair_action_sel);
  m_tms_pair_action_lt->addWidget(m_tms_pair_action_copy);
  m_tms_pair_action_lt->addWidget(m_tms_pair_action_del);

  m_tms_pair_custom_text_edit = new qbinded_line_edit_t;
  m_tms_pair_custom_text_enabled = new qbinded_checkbox_t;
  m_tms_pair_delta_angle = new qbinded_float_spinbox_t;
  m_tms_pair_delta_angle->set_min_max_step(-90, 90, 0.1);
  m_tms_pair_delta_offset = new qbinded_int2b_input_t;

  m_tms_pair_term_size = new qbinded_int_spinbox_t;
  m_tms_pair_term_size->set_min_max_step(0, 20, 1);

  m_tms_pair_term_style = new qbinded_combobox_t;
  m_tms_pair_term_style->addItem("None");
  m_tms_pair_term_style->addItem("First");
  m_tms_pair_term_style->addItem("Second");
  m_tms_pair_term_style->addItem("Both");

  m_tms_pair_dist_gb_lt->addRow(tr("Current"), m_tms_pair_cur_msr);
  m_tms_pair_dist_gb_lt->addRow(tr("Atom №1"), m_tms_pair_at1_info);
  m_tms_pair_dist_gb_lt->addRow(tr("Atom №2"), m_tms_pair_at2_info);
  m_tms_pair_dist_gb_lt->addRow(tr("Line style"), m_tms_pair_line_style);
  m_tms_pair_dist_gb_lt->addRow(tr("Line size"), m_tms_pair_line_size);
  m_tms_pair_dist_gb_lt->addRow(tr("Color"), m_tms_pair_dist_color);
  m_tms_pair_dist_gb_lt->addRow(tr("Enabled"), m_tms_pair_show);
  m_tms_pair_dist_gb_lt->addRow(tr("Label enabled"), m_tms_pair_label_enabled);
  m_tms_pair_dist_gb_lt->addRow(tr("Label style"), m_tms_pair_label_style);
  m_tms_pair_dist_gb_lt->addRow(tr("Custom label"), m_tms_pair_custom_text_enabled);
  m_tms_pair_dist_gb_lt->addRow(tr("Label text"), m_tms_pair_custom_text_edit);
  m_tms_pair_dist_gb_lt->addRow(tr("Font size(pt)"), m_tms_font_screen_size);
  m_tms_pair_dist_gb_lt->addRow(tr("Delta angle"), m_tms_pair_delta_angle);
  m_tms_pair_dist_gb_lt->addRow(tr("Delta offset"), m_tms_pair_delta_offset);
  m_tms_pair_dist_gb_lt->addRow(tr("Term. style"), m_tms_pair_term_style);
  m_tms_pair_dist_gb_lt->addRow(tr("Term. size"), m_tms_pair_term_size);
  m_tms_pair_dist_gb_lt->addRow(tr("Actions"), m_tms_pair_action_lt);
  init_form_lt(m_tms_pair_dist_gb_lt);

  m_tms_angle_gb = new qspoiler_widget_t(tr("Interatomic Angle"));
  m_tms_angle_gb_lt = new QFormLayout;
  m_tms_angle_gb->add_content_layout(m_tms_angle_gb_lt);
  m_tms_angle_cur_msr = new QComboBox;
  m_tms_angle_cur_msr->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w_v2());

  connect(m_tms_angle_cur_msr,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::msr_angle_cur_idx_changed);

  m_tms_angle_enabled = new qbinded_checkbox_t;
  m_tms_angle_order = new qbinded_int_spinbox_t;
  m_tms_angle_order->set_min_max_step(0, 10, 1);

  m_tms_angle_at1_info = new QLabel;
  m_tms_angle_at1_info->setProperty("s_class", "thin_label");
  m_tms_angle_at2_info = new QLabel;
  m_tms_angle_at2_info->setProperty("s_class", "thin_label");
  m_tms_angle_at3_info = new QLabel;
  m_tms_angle_at3_info->setProperty("s_class", "thin_label");

  m_tms_angle_gb_lt->addRow(tr("Current"), m_tms_angle_cur_msr);
  m_tms_angle_gb_lt->addRow(tr("Atom №1"), m_tms_angle_at1_info);
  m_tms_angle_gb_lt->addRow(tr("Atom №2"), m_tms_angle_at2_info);
  m_tms_angle_gb_lt->addRow(tr("Atom №3"), m_tms_angle_at3_info);
  m_tms_angle_gb_lt->addRow(tr("Enabled"), m_tms_angle_enabled);
  m_tms_angle_gb_lt->addRow(tr("Order"), m_tms_angle_order);
  init_form_lt(m_tms_angle_gb_lt);


  tms_switch = new QButtonGroup;
  tms_switch->setExclusive(true);

  connect(tms_switch, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),
          this, &geom_view_obj_insp_widget_t::msr_switch_current_changed);

  QHBoxLayout *tmp_lt = new QHBoxLayout;
  tmp_lt->setContentsMargins(4, 0, 4, 3);
  tmp_lt->setSpacing(2);

  QPushButton *tmp_b1 = new QPushButton(tr("General"));
  QPushButton *tmp_b2 = new QPushButton(tr("Distance"));
  QPushButton *tmp_b3 = new QPushButton(tr("Angle"));

  int i = 0;
  for (auto btn : {tmp_b1, tmp_b2, tmp_b3}) {
    tmp_lt->addWidget(btn);
    tms_switch->addButton(btn, i);
    btn->setCheckable(true);
    i++;
  }

  m_tab_measurement->tab_top_wdgt_lt->insertLayout(0, tmp_lt);
  m_tab_measurement->tab_inner_widget_lt->addWidget(m_tms_common_settings_gb);
  m_tab_measurement->tab_inner_widget_lt->addWidget(m_tms_pair_dist_gb);
  m_tab_measurement->tab_inner_widget_lt->addWidget(m_tms_angle_gb);
  m_tab_measurement->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_mod_tab() {

  app_state_t *astate = app_state_t::get_inst();

  m_tm_gb_add_atom = new qspoiler_widget_t(tr("Add Atom"));
  m_tm_gb_add_atom_lt = new QFormLayout;
  m_tm_gb_add_atom->add_content_layout(m_tm_gb_add_atom_lt);

  m_tm_add_atom_combo = new QComboBox;
  m_tm_add_atom_combo->setEditable(true);
  m_tm_add_atom_combo->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  m_tm_add_atom_vec3 = new qbinded_float3_input_t;
  m_tm_add_atom_vec3->set_min_max_step_dec(-1000, 1000, 0.01);

  m_tm_add_atom_button = new QPushButton(tr("Add"));
  m_tm_add_atom_button->setMaximumWidth(astate->size_guide.obj_insp_button_w());
  connect(m_tm_add_atom_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_add_atom_button_clicked);

  m_tm_gb_add_atom_lt->addRow(tr("Atom name"), m_tm_add_atom_combo);
  m_tm_gb_add_atom_lt->addRow(tr("Atom pos.[%1]").arg(astate->m_spatial_suffix), m_tm_add_atom_vec3);
  m_tm_gb_add_atom_lt->addRow("", m_tm_add_atom_button);
  init_form_lt(m_tm_gb_add_atom_lt);

  m_tm_gb_single_atom = new qspoiler_widget_t(tr("Modify Single Atom"));
  m_tm_gb_single_atom_lt = new QFormLayout;
  m_tm_gb_single_atom_lt->setLabelAlignment(Qt::AlignRight);
  m_tm_gb_single_atom->add_content_layout(m_tm_gb_single_atom_lt);
  m_tm_single_atom_combo = new QComboBox;
  m_tm_single_atom_combo->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  m_tm_single_atom_combo->setEditable(true);
  m_tm_single_atom_v3 = new qbinded_float3_input_t;
  m_tm_single_atom_v3->set_min_max_step_dec(-10000, 10000, 0.01);
  m_tm_single_atom_info = new QLabel;
  m_tm_single_atom_info->setProperty("s_class", "thin_label");

  m_tm_single_atom_commit = new QPushButton(tr("Apply"));
  m_tm_single_atom_commit->setMaximumWidth(astate->size_guide.obj_insp_button_w());
  connect(m_tm_single_atom_commit,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_single_atom_button_clicked);

  m_tm_single_atom_delete = new QPushButton(tr("Delete"));
  m_tm_single_atom_delete->setMaximumWidth(astate->size_guide.obj_insp_button_w());
  connect(m_tm_single_atom_delete,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_single_atom_delete_button_clicked);

  m_tm_single_atom_btn_lt = new QHBoxLayout;
  m_tm_single_atom_btn_lt->addWidget(m_tm_single_atom_commit);
  m_tm_single_atom_btn_lt->addWidget(m_tm_single_atom_delete);

  m_tm_gb_single_atom_lt->addRow(tr("Atom name"), m_tm_single_atom_combo);
  m_tm_gb_single_atom_lt->addRow(tr("Atom info"), m_tm_single_atom_info);
  m_tm_gb_single_atom_lt->addRow(tr("Atom pos.[%1]").arg(astate->m_spatial_suffix),
                                 m_tm_single_atom_v3);
  m_tm_gb_single_atom_lt->addRow("", m_tm_single_atom_btn_lt);
  init_form_lt(m_tm_gb_single_atom_lt);

  m_tm_gb_override_atom = new qspoiler_widget_t(tr("Override Atom"));
  m_tm_gb_override_atom_lt = new QFormLayout;
  m_tm_gb_override_atom->add_content_layout(m_tm_gb_override_atom_lt);
  m_tm_override_atom_color = new qbinded_xgeom_color3_input_t(this);
  m_tm_override_atom_radii = new qbinded_xgeom_float_spinbox_t(this);
  m_tm_override_atom_radii->set_min_max_step(0.01, 5, 0.1);
  m_tm_gb_override_atom_lt->addRow(tr("Atom color"), m_tm_override_atom_color);
  m_tm_gb_override_atom_lt->addRow(tr("Atom radius"), m_tm_override_atom_radii);
  init_form_lt(m_tm_gb_override_atom_lt);

  m_tm_gb_pair_dist = new qspoiler_widget_t(tr("Pair Distance"));
  m_tm_gb_pair_dist_lt = new QFormLayout;
  m_tm_gb_pair_dist_lt->setLabelAlignment(Qt::AlignRight);
  m_tm_gb_pair_dist->add_content_layout(m_tm_gb_pair_dist_lt);
  m_tm_pair_dist_atom1 = new QLabel;
  m_tm_pair_dist_atom1->setProperty("s_class", "thin_label");

  m_tm_pair_dist_atom2 = new QLabel;
  m_tm_pair_dist_atom2->setProperty("s_class", "thin_label");

  m_tm_pair_dist_spinbox = new QDoubleSpinBox;
  m_tm_pair_dist_spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_tm_pair_dist_note_label = new QLabel;
  m_tm_pair_dist_note_label->setProperty("s_class", "thin_label");

  m_tm_pair_dist_t_mode = new QComboBox;
  m_tm_pair_dist_t_mode->addItem("Transform both");
  m_tm_pair_dist_t_mode->addItem("Fix first");
  m_tm_pair_dist_t_mode->addItem("Fix second");
  m_tm_pair_dist_t_mode->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());

  m_tm_pair_dist_note_label->setText(tr("Distance"));
  m_tm_pair_dist_spinbox->setMinimum(0.0);
  m_tm_pair_dist_spinbox->setMaximum(10);
  m_tm_pair_dist_spinbox->setSingleStep(0.005);
  m_tm_pair_dist_spinbox->setDecimals(3);
  m_tm_pair_dist_spinbox->setLocale(QLocale::C);
  m_tm_pair_dist_spinbox->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  m_tm_pair_dist_swap_atoms = new QPushButton(tr("Swap"));
  m_tm_pair_dist_swap_atoms->setMaximumWidth(astate->size_guide.obj_insp_button_w());

  connect(m_tm_pair_dist_swap_atoms,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_pair_dist_swap_button_clicked);

  m_tm_pair_dist_cmb_lt = new QHBoxLayout;
  m_tm_pair_dist_cmb_lt->addWidget(m_tm_pair_dist_spinbox);
  m_tm_pair_dist_cmb_lt->addWidget(m_tm_pair_dist_swap_atoms);

  m_tm_gb_pair_dist_lt->addRow(tr("Atom №1"), m_tm_pair_dist_atom1);
  m_tm_gb_pair_dist_lt->addRow(tr("Atom №2"), m_tm_pair_dist_atom2);
  m_tm_gb_pair_dist_lt->addRow(tr("Trnsf. mode"), m_tm_pair_dist_t_mode);
  m_tm_gb_pair_dist_lt->addRow(m_tm_pair_dist_note_label, m_tm_pair_dist_cmb_lt);
  init_form_lt(m_tm_gb_pair_dist_lt);

  m_tm_gb_pair_creation = new qspoiler_widget_t(tr("Insert Atom Between"));
  m_tm_gb_pair_creation_lt = new QFormLayout;
  m_tm_gb_pair_creation_lt->setLabelAlignment(Qt::AlignRight);
  m_tm_gb_pair_creation->add_content_layout(m_tm_gb_pair_creation_lt);
  m_tm_pair_creation_combo = new QComboBox;
  m_tm_pair_creation_combo->setEditable(true);
  m_tm_pair_creation_combo->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  m_tm_pair_creation_button = new QPushButton("Append");
  m_tm_pair_creation_button->setAutoRepeat(true);
  m_tm_pair_creation_button->setAutoRepeatDelay(10000);
  m_tm_pair_creation_button->setAutoRepeatInterval(10000);
  m_tm_pair_creation_button->setMaximumWidth(astate->size_guide.obj_insp_button_w());

  m_tm_pair_creation_combo_btn_lt = new QHBoxLayout;
  m_tm_pair_creation_combo_btn_lt->addWidget(m_tm_pair_creation_combo);
  m_tm_pair_creation_combo_btn_lt->addWidget(m_tm_pair_creation_button);
  m_tm_gb_pair_creation_lt->addRow(tr("New atom"), m_tm_pair_creation_combo_btn_lt);
  init_form_lt(m_tm_gb_pair_creation_lt);

  connect(m_tm_pair_creation_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_add_atom_between_pair);

  connect(m_tm_pair_dist_spinbox,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &geom_view_obj_insp_widget_t::mod_pair_dist_spinbox_value_changed);

  m_tm_gb_u_scale = new qspoiler_widget_t(tr("Barycentric Scaling"));
  m_tm_gb_u_scale_lt = new QFormLayout;
  m_tm_gb_u_scale_lt->setLabelAlignment(Qt::AlignRight);
  m_tm_gb_u_scale->add_content_layout(m_tm_gb_u_scale_lt);

  m_tm_u_scale_v3 = new qbinded_float3_input_t;
  m_tm_u_scale_v3->set_min_max_step_dec(0.01, 10, 0.01);
  m_tm_u_scale_v3_val.set_value(vector3<float>{1, 1, 1});
  m_tm_u_scale_v3->bind_value(&m_tm_u_scale_v3_val);

  m_tm_u_apply_scale_button = new QPushButton(tr("Apply"));
  m_tm_u_apply_scale_button->setMaximumWidth(astate->size_guide.obj_insp_button_w());

  connect(m_tm_u_apply_scale_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_barycentric_scale_button_clicked);

  m_tm_gb_u_scale_lt->addRow("Scale ", m_tm_u_scale_v3);
  m_tm_gb_u_scale_lt->addRow("", m_tm_u_apply_scale_button);
  init_form_lt(m_tm_gb_u_scale_lt);

  m_tm_gb_translate = new qspoiler_widget_t(tr("Translate Selected Atoms"));
  m_tm_gb_translate_lt = new QFormLayout;
  m_tm_gb_translate_lt->setLabelAlignment(Qt::AlignRight);
  m_tm_gb_translate->add_content_layout(m_tm_gb_translate_lt);
  m_tm_translate_v3 = new qbinded_float3_input_t;
  m_tm_translate_v3->set_min_max_step_dec(-10000, 10000, 0.01);

  m_tm_translate_apply_button = new QPushButton(tr("Apply"));
  m_tm_translate_apply_button->setMaximumWidth(astate->size_guide.obj_insp_button_w());

  m_tm_translate_coord_type = new QComboBox;
  m_tm_translate_coord_type->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  m_tm_translate_coord_type->addItem("Cart.");
  m_tm_translate_coord_type->addItem("Frac.");

  m_tm_translate_combo_btn_lt = new QHBoxLayout;
  m_tm_translate_combo_btn_lt->addWidget(m_tm_translate_coord_type);
  m_tm_translate_combo_btn_lt->addWidget(m_tm_translate_apply_button);

  m_tm_gb_translate_lt->addRow(tr("Tr. vector"), m_tm_translate_v3);
  m_tm_gb_translate_lt->addRow("", m_tm_translate_combo_btn_lt);
  init_form_lt(m_tm_gb_translate_lt);

  connect(m_tm_translate_coord_type,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::mod_translate_coord_type_changed);

  connect(m_tm_translate_apply_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_translate_selected_atoms_clicked);

  m_tm_gb_bc_rot = new qspoiler_widget_t(tr("Rotate Selected Atoms"));
  m_tm_gb_bc_rot_lt = new QFormLayout;
  m_tm_gb_bc_rot_lt->setLabelAlignment(Qt::AlignRight);
  m_tm_gb_bc_rot->add_content_layout(m_tm_gb_bc_rot_lt);

  m_tm_bc_rot_axis = new QComboBox;
  m_tm_bc_rot_axis->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  m_tm_bc_rot_axis->addItem("X");
  m_tm_bc_rot_axis->addItem("Y");
  m_tm_bc_rot_axis->addItem("Z");
  m_tm_bc_rot_axis->setItemData(0, QBrush(Qt::red), Qt::TextColorRole);
  m_tm_bc_rot_axis->setItemData(1, QBrush(Qt::green), Qt::TextColorRole);
  m_tm_bc_rot_axis->setItemData(2, QBrush(Qt::blue), Qt::TextColorRole);

  m_tm_bc_rot_angle = new QDoubleSpinBox;
  m_tm_bc_rot_angle->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_tm_bc_rot_angle->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  m_tm_bc_rot_angle->setMinimum(-1000);
  m_tm_bc_rot_angle->setMaximum(1000);
  m_tm_bc_rot_angle->setLocale(QLocale::C);

  m_tm_bc_rot_angle_type = new QComboBox;
  m_tm_bc_rot_angle_type->setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  connect(m_tm_bc_rot_angle_type,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::mod_bc_rot_angle_type_change);

  m_tm_bc_rot_angle_type->addItem("Degr.");
  m_tm_bc_rot_angle_type->addItem("Rad.");
  m_tm_bc_rot_apply = new QPushButton(tr("Apply"));
  m_tm_bc_rot_apply->setMaximumWidth(astate->size_guide.obj_insp_button_w());
  connect(m_tm_bc_rot_apply,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_bc_rot_apply);

  m_tm_bc_rot_cmb_lt1 = new QHBoxLayout;
  m_tm_bc_rot_cmb_lt1->addWidget(m_tm_bc_rot_angle);
  m_tm_bc_rot_cmb_lt1->addWidget(m_tm_bc_rot_angle_type);
  m_tm_bc_rot_cmb_lt2 = new QHBoxLayout;
  m_tm_bc_rot_cmb_lt2->addWidget(m_tm_bc_rot_axis);
  m_tm_bc_rot_cmb_lt2->addWidget(m_tm_bc_rot_apply);

  m_tm_gb_bc_rot_lt->addRow(tr("Angle"), m_tm_bc_rot_cmb_lt1);
  m_tm_gb_bc_rot_lt->addRow("", m_tm_bc_rot_cmb_lt2);
  init_form_lt(m_tm_gb_bc_rot_lt);

  m_tm_gb_grp_op = new qspoiler_widget_t(tr("Group Operations"));
  m_tm_grp_op_lt = new QGridLayout;
  m_tm_grp_op_lt->setContentsMargins(15, 0, 15, 0);
  m_tm_gb_grp_op->add_content_layout(m_tm_grp_op_lt);

  m_tm_grp_op_sv_show = new QPushButton(tr("SV:SHOW"));
  connect(m_tm_grp_op_sv_show,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_group_op_sv_show);

  m_tm_grp_op_sv_hide = new QPushButton(tr("SV:HIDE"));
  connect(m_tm_grp_op_sv_hide,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_group_op_sv_hide);

  m_tm_grp_op_sv_hide_invert = new QPushButton(tr("SV:HIDE INV"));
  connect(m_tm_grp_op_sv_hide_invert,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_group_op_sv_hide_invert);

  m_tm_grp_op_flip_a_p = new QPushButton(tr("FLIP:+A"));
  connect(m_tm_grp_op_flip_a_p,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(0, 1);});

  m_tm_grp_op_flip_b_p = new QPushButton(tr("FLIP:+B"));
  connect(m_tm_grp_op_flip_b_p,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(1, 1);});

  m_tm_grp_op_flip_c_p = new QPushButton(tr("FLIP:+C"));
  connect(m_tm_grp_op_flip_c_p,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(2, 1);});

  m_tm_grp_op_flip_a_n = new QPushButton(tr("FLIP:-A"));
  connect(m_tm_grp_op_flip_a_n,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(0, -1);});

  m_tm_grp_op_flip_b_n = new QPushButton(tr("FLIP:-B"));
  connect(m_tm_grp_op_flip_b_n,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(1, -1);});

  m_tm_grp_op_flip_c_n = new QPushButton(tr("FLIP:-C"));
  connect(m_tm_grp_op_flip_c_n,
          &QPushButton::pressed,
          [this](){this->tab_modify_flip_cell_clicked(2, -1);});

  m_tm_grp_op_rebond_all = new QPushButton(tr("REBOND"));
  connect(m_tm_grp_op_rebond_all,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_group_op_rebond_all);

  m_tm_grp_op_sel_ngbs = new QPushButton(tr("SEL:NGB"));

  connect(m_tm_grp_op_sel_ngbs,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_group_op_sel_ngbs);

  m_tm_grp_op_del_sel = new QPushButton(tr("SEL:DEL"));

  connect(m_tm_grp_op_del_sel,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_group_op_del_sel);

  m_tm_grp_make_animable = new QPushButton(tr("MAKE STC"));
  connect(m_tm_grp_make_animable,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::mod_group_op_make_static_anim);

  m_tm_grp_op_lt->addWidget(m_tm_grp_op_sv_show,        0, 0, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_sv_hide,        0, 1, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_sv_hide_invert, 0, 2, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_sel_ngbs,       1, 0, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_del_sel,        1, 1, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_make_animable,     1, 2, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_flip_a_p,       2, 0, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_flip_b_p,       2, 1, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_flip_c_p,       2, 2, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_flip_a_n,       3, 0, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_flip_b_n,       3, 1, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_flip_c_n,       3, 2, 1, 1);
  m_tm_grp_op_lt->addWidget(m_tm_grp_op_rebond_all,     4, 0, 1, 1);

  m_tab_modify->tab_inner_widget_lt->addWidget(m_tm_gb_add_atom);
  m_tab_modify->tab_inner_widget_lt->addWidget(m_tm_gb_override_atom);
  m_tab_modify->tab_inner_widget_lt->addWidget(m_tm_gb_single_atom);
  m_tab_modify->tab_inner_widget_lt->addWidget(m_tm_gb_pair_dist);
  m_tab_modify->tab_inner_widget_lt->addWidget(m_tm_gb_pair_creation);
  m_tab_modify->tab_inner_widget_lt->addWidget(m_tm_gb_u_scale);
  m_tab_modify->tab_inner_widget_lt->addWidget(m_tm_gb_translate);
  m_tab_modify->tab_inner_widget_lt->addWidget(m_tm_gb_bc_rot);
  m_tab_modify->tab_inner_widget_lt->addWidget(m_tm_gb_grp_op);

  m_tab_modify->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_sel_tab() {

  app_state_t *astate = app_state_t::get_inst();

  // general settings
  m_ts_gb_general = new qspoiler_widget_t(tr("Selection Groups"));
  m_ts_gb_general_lt = new QFormLayout;
  m_ts_gb_general->add_content_layout(m_ts_gb_general_lt);

  m_ts_total_grps = new QLabel;
  m_ts_total_grps->setProperty("s_class", "thin_label");

  m_ts_auto_apply = new qbinded_checkbox_t;
  m_ts_cur_sel_grp = new QComboBox;
  m_ts_cur_sel_grp->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());

  m_ts_gb_general_lt->addRow(tr("Total groups"), m_ts_total_grps);
  m_ts_gb_general_lt->addRow(tr("Auto apply"), m_ts_auto_apply);
  m_ts_gb_general_lt->addRow(tr("Current group"), m_ts_cur_sel_grp);

  init_form_lt(m_ts_gb_general_lt);

  // detail settings
  m_ts_gb_sel_grp_details = new qspoiler_widget_t(tr("Selection Group - Details"));
  m_ts_gb_sel_grp_details_lt = new QFormLayout;
  m_ts_gb_sel_grp_details->add_content_layout(m_ts_gb_sel_grp_details_lt);

  m_tab_select->tab_inner_widget_lt->addWidget(m_ts_gb_general);
  m_tab_select->tab_inner_widget_lt->addWidget(m_ts_gb_sel_grp_details);
  m_tab_select->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_xgeom_tab() {

  app_state_t *astate = app_state_t::get_inst();

  m_txg_gb_info = new qspoiler_widget_t(tr("XGeometry Fields"));
  m_txg_gb_info_lt = new QVBoxLayout;
  m_txg_gb_info->add_content_layout(m_txg_gb_info_lt);

  m_txg_info_tv = new QTableView;
  m_txg_gb_info_lt->addWidget(m_txg_info_tv);

  m_txg_info_tmdl = new qxgeom_fields_model_t;
  m_txg_info_tv->setModel(m_txg_info_tmdl);

  //m_txg_gb_info->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_txg_info_tv->verticalHeader()->hide();
  m_txg_info_tv->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  m_txg_info_tv->setFocusPolicy(Qt::NoFocus);
  m_txg_info_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  //m_txg_info_tv->setFixedWidth(astate->size_guide.obj_insp_table_w());
  m_txg_info_tv->setShowGrid(false);
  //m_txg_info_tv->setMinimumHeight(400);

  m_txg_gb_comp_prop = new qspoiler_widget_t(tr("Computed Property"));
  m_txg_gb_comp_prop_lt = new QVBoxLayout;
  m_txg_gb_comp_prop->add_content_layout(m_txg_gb_comp_prop_lt);

  m_txg_switch = new QButtonGroup;
  m_txg_switch->setExclusive(true);

  connect(m_txg_switch,
          static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),
          this,
          &geom_view_obj_insp_widget_t::xgeom_switch_current_changed);

  QHBoxLayout *tmp_lt = new QHBoxLayout;
  tmp_lt->setContentsMargins(4, 0, 4, 3);
  tmp_lt->setSpacing(2);

  QPushButton *tmp_b1 = new QPushButton(tr("XGeometry Structure"));
  QPushButton *tmp_b2 = new QPushButton(tr("Computed Property"));

  int i = 0;
  for (auto btn : {tmp_b1, tmp_b2}) {
    tmp_lt->addWidget(btn);
    m_txg_switch->addButton(btn, i);
    btn->setCheckable(true);
    i++;
  }

  m_tab_xgeom->tab_top_wdgt_lt->insertLayout(0, tmp_lt);
  m_tab_xgeom->tab_inner_widget_lt->addWidget(m_txg_gb_info);
  m_tab_xgeom->tab_inner_widget_lt->addWidget(m_txg_gb_comp_prop);
  m_tab_xgeom->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {
  auto tmp = _binding_item->cast_as<geom_view_t>();
  if (tmp) {
    b_al = tmp;
  }
  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
}

void geom_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_al) {

    //General tab
    m_tg_geom_sum_total_atoms->setText(
        QString::fromStdString(fmt::format("[Atoms:{}], [Types:{}]",
                                           b_al->m_geom->nat(),
                                           b_al->m_geom->n_types())));

    //      m_tg_geom_sum_total_types->setText(
    //            QString::fromStdString(fmt::format("{}", b_al->m_geom->n_atom_types())));

    //update type table
    m_tg_type_sum_tbl->clearContents();
    m_tg_type_sum_tbl->setRowCount(b_al->m_geom->n_atom_types());

    for (int i = 0; i < b_al->m_geom->n_atom_types(); i++) {

      QString n_name_str = QString::fromStdString(b_al->m_geom->atom_of_type(i));
      QTableWidgetItem *n_name = new QTableWidgetItem(n_name_str);
      n_name->setTextAlignment(Qt::AlignCenter);
      m_tg_type_sum_tbl->setItem(i, 0, n_name);

      QString n_c_str = tr("%1").arg(b_al->m_geom->get_atom_count_by_type(i));
      QTableWidgetItem *n_c = new QTableWidgetItem(n_c_str);
      n_c->setTextAlignment(Qt::AlignCenter);
      m_tg_type_sum_tbl->setItem(i, 1, n_c);

      QString n_clr_str = "";
      QTableWidgetItem *n_clr = new QTableWidgetItem(n_clr_str);
      n_clr->setTextAlignment(Qt::AlignCenter);

      auto ap_idx = ptable::number_by_symbol(b_al->m_geom->atom_of_type(i));
      vector3<float> bc(0.0, 0.0, 1.0);
      if (ap_idx)
        bc = ptable::get_inst()->arecs[*ap_idx-1].m_color_jmol;

      QColor color_bck;

      auto it = b_al->m_type_color_override.find(i);
      if (it != b_al->m_type_color_override.end())
        color_bck.setRgbF(it->second[0], it->second[1], it->second[2]);
      else color_bck.setRgbF(bc[0], bc[1], bc[2]);

      n_clr->setBackgroundColor(color_bck);
      m_tg_type_sum_tbl->setItem(i, 2, n_clr);

    }

    //resize type table view
    qt_hlp::vrt_resize_tv_to_cnt(m_tg_type_sum_tbl);

    //update cell
    cell_changed();
    qt_hlp::vrt_resize_tv_to_cnt(m_tg_gb_cell_tbl);

    //display tab
    m_disp_s_draw_atoms_bonds->bind_value({&b_al->m_draw_atoms, &b_al->m_draw_bonds});
    m_disp_s_draw_img_atoms_bonds->bind_value({&b_al->m_draw_img_atoms, &b_al->m_draw_img_bonds});

    m_disp_s_scale->bind_value({&b_al->m_atom_scale_factor, &b_al->m_bond_scale_factor});
    m_disp_s_render_style->bind_value(&b_al->m_render_style);
    m_disp_s_color_mode->bind_value(&b_al->m_color_mode);

    m_disp_labels_style->bind_value(&b_al->m_labels->m_style);
    m_disp_labels_size->bind_value(&b_al->m_labels->m_lbl_font_size);
    m_disp_inplace_labels->bind_value(&b_al->m_labels->m_render_inplace_hud);
    m_disp_inplace_offset->bind_value(&b_al->m_labels->m_inplace_offset);
    m_disp_sl_labels->bind_value(&b_al->m_labels->m_selective_lbl);
    m_disp_labels_screen_scale->bind_value(&b_al->m_labels->m_screen_scale);
    m_disp_labels_draw_outline->bind_value(&b_al->m_labels->m_render_outlines);
    m_disp_labels_outline_size->bind_value(&b_al->m_labels->m_outline_size);
    m_disp_labels_draw_axis_labels->bind_value(&b_al->m_labels->m_render_axis_labels);

    m_disp_shading_spec->bind_value(&b_al->m_draw_specular);
    m_disp_shading_spec_value->bind_value(&b_al->m_shading_specular_power);

    m_disp_s_sel_vis_p_affect_bonds->bind_value({&b_al->m_sel_vis, &b_al->m_sel_vis_affect_bonds});

    m_periodic_draw_settings->bind_value({&b_al->m_draw_cell, &b_al->m_draw_cell_vectors});
    m_periodic_draw_subcells->bind_value(&b_al->m_draw_subcells);

    m_periodic_cell_offset->bind_value(&b_al->m_cell_vector_offset);
    m_periodic_cell_v_ratio->bind_value(&b_al->m_cell_vectors_ratio);
    m_periodic_cell_vectors_color->bind_value(&b_al->m_cell_vector_color);

    m_periodic_draw_subcells->bind_value(&b_al->m_draw_subcells);

    m_periodic_subcells_idx->bind_value(&b_al->m_subcells_range);
    m_periodic_cell_color->bind_value(&b_al->m_cell_color);
    m_periodic_subcells_clr->bind_value(&b_al->m_subcell_color);

    // 3d geom section
    bool _al3d = b_al->m_geom->get_DIM() == 3;
    m_gb_periodic_related_render->setVisible(_al3d && m_tab_disp_switch->button(0)->isChecked());
    // end 3d geom section

    //anim bindings
    update_anim_section_status();
    m_anim_settings->bind_value({&b_al->m_anim->m_play_cyclic,
                                 &b_al->m_anim->m_interpolate_anim,
                                 &b_al->m_anim->m_rebuild_bonds_in_anim});
    m_anim_speed->bind_value(&b_al->m_anim->m_anim_frame_time);

    //binding current anim combobox
    m_anim_current_anim->blockSignals(true);
    m_anim_current_anim->clear();

    for (size_t i = 0; i < b_al->m_anim->get_total_anims(); i++)
      m_anim_current_anim->addItem(
          QString::fromStdString(fmt::format("[{}] {}",
                                             i,
                                             b_al->m_anim->m_anim_data[i].m_anim_name)));

    m_anim_current_anim->setCurrentIndex(b_al->m_anim->m_cur_anim);
    m_anim_current_anim->blockSignals(false);
    update_anim_tab();

    m_anim_play->blockSignals(true);
    m_anim_play->setChecked(b_al->m_anim->m_play_anim.get_value());
    m_anim_play->blockSignals(false);

    m_disp_type_spec_tv->setModel(nullptr);
    m_disp_type_spec_mdl->bind(b_al);
    m_disp_type_spec_tv->setModel(m_disp_type_spec_mdl);
    m_disp_type_spec_tv->update();
    qt_hlp::vrt_resize_tv_to_cnt(m_disp_type_spec_tv);

    m_disp_bt->setModel(nullptr);
    m_bt_mdl->bind(b_al);
    m_disp_bt->setModel(m_bt_mdl);
    m_disp_bt->update();
    qt_hlp::vrt_resize_tv_to_cnt(m_disp_bt);

    //bind tab measurement common settings
    m_tms_render_dist_angle->bind_value({&b_al->m_measure->m_render_dist,
                                         &b_al->m_measure->m_render_angle});
    //end of bind tab measurement common settings

    m_txg_info_tv->setModel(nullptr);
    m_txg_info_tmdl->bind(b_al);
    m_txg_info_tv->setModel(m_txg_info_tmdl);
    m_txg_info_tv->update();
    //qt_hlp::vrt_resize_tv_to_cnt(txg_info_tv);

    update_mod_tab();
    update_msr_tab();
    update_sel_tab();
    update_xgeom_tab();

  }

}

void geom_view_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();

  m_disp_s_draw_atoms_bonds->unbind_value();
  m_disp_s_draw_img_atoms_bonds->unbind_value();

  m_disp_s_scale->unbind_value();
  m_disp_s_render_style->unbind_value();
  m_disp_s_color_mode->unbind_value();

  m_disp_labels_style->unbind_value();
  m_disp_labels_size->unbind_value();
  m_disp_inplace_labels->unbind_value();
  m_disp_inplace_offset->unbind_value();
  m_disp_sl_labels->unbind_value();
  m_disp_labels_screen_scale->unbind_value();
  m_disp_labels_draw_outline->unbind_value();
  m_disp_labels_outline_size->unbind_value();

  m_disp_shading_spec->unbind_value();
  m_disp_shading_spec_value->unbind_value();
  m_disp_s_sel_vis_p_affect_bonds->unbind_value();

  m_periodic_draw_settings->unbind_value();
  m_periodic_cell_offset->unbind_value();
  m_periodic_cell_v_ratio->unbind_value();
  m_periodic_cell_vectors_color->unbind_value();

  m_periodic_cell_color->unbind_value();
  m_periodic_draw_subcells->unbind_value();
  m_periodic_subcells_idx->unbind_value();
  m_periodic_subcells_clr->unbind_value();

  //  m_anim_rebuild_bonds->unbind_value();
  m_anim_settings->unbind_value();
  //  m_anim_play_cyclic->unbind_value();
  //  m_anim_interpolate_anim->unbind_value();

  m_anim_speed->unbind_value();

  //tab modify spec
  m_tm_override_atom_color->unbind_value();
  m_tm_override_atom_radii->unbind_value();
  //end tab modify spec

  m_tms_render_dist_angle->unbind_value();

  m_disp_type_spec_mdl->unbind();
  m_bt_mdl->unbind();

  unbind_dist_msr_tab();
  unbind_sel_tab();

  b_al = nullptr;

}

void geom_view_obj_insp_widget_t::bind_dist_msr_tab() {

  if (b_al) {

    int cur_msr = b_al->m_measure->m_cur_dist_rec_ui - 1;
    auto dmr = b_al->m_measure->m_dist_recs.get_hs_child_as_array(cur_msr);

    if (dmr) {

      m_tms_pair_dist_color->bind_value(&dmr->m_bond_color);
      m_tms_pair_show->bind_value(&dmr->m_show);
      m_tms_pair_label_enabled->bind_value(&dmr->m_show_label);
      m_tms_pair_line_size->bind_value(&dmr->m_line_size);
      m_tms_font_screen_size->bind_value(&dmr->m_font_size);
      m_tms_pair_line_style->bind_value(&dmr->m_line_render_style);
      m_tms_pair_label_style->bind_value(&dmr->m_label_render_style);
      m_tms_pair_term_size->bind_value(&dmr->m_pair_term_width);
      m_tms_pair_term_style->bind_value(&dmr->m_pair_term_style);

      m_tms_pair_custom_text_edit->bind_value(&dmr->m_custom_label_text);
      m_tms_pair_custom_text_enabled->bind_value(&dmr->m_show_custom_label);

      m_tms_pair_delta_angle->bind_value(&dmr->m_delta_angle);
      m_tms_pair_delta_offset->bind_value(&dmr->m_delta_offset);

      m_tms_pair_show->setEnabled(true);
      m_tms_pair_dist_color->setEnabled(true);
      m_tms_pair_line_style->setEnabled(true);
      m_tms_pair_line_size->setEnabled(true);
      m_tms_font_screen_size->setEnabled(true);
      m_tms_pair_label_style->setEnabled(true);
      m_tms_pair_label_enabled->setEnabled(true);
      m_tms_pair_delta_angle->setEnabled(true);
      m_tms_pair_delta_offset->setEnabled(true);

      m_tms_pair_term_size->setEnabled(true);
      m_tms_pair_term_style->setEnabled(true);

      m_tms_pair_action_sel->setEnabled(true);
      m_tms_pair_action_del->setEnabled(true);
      m_tms_pair_action_copy->setEnabled(true);

      m_tms_pair_custom_text_edit->setEnabled(true);
      m_tms_pair_custom_text_enabled->setEnabled(true);

    } else {
      unbind_dist_msr_tab();
    }

  }

}

void geom_view_obj_insp_widget_t::unbind_dist_msr_tab() {

  m_tms_pair_dist_color->unbind_value();
  m_tms_pair_show->unbind_value();
  m_tms_pair_line_style->unbind_value();
  m_tms_pair_line_size->unbind_value();
  m_tms_font_screen_size->unbind_value();
  m_tms_pair_label_style->unbind_value();
  m_tms_pair_label_enabled->unbind_value();
  m_tms_pair_delta_angle->unbind_value();
  m_tms_pair_delta_offset->unbind_value();

  m_tms_pair_term_size->unbind_value();
  m_tms_pair_term_style->unbind_value();

  m_tms_pair_custom_text_enabled->unbind_value();
  m_tms_pair_custom_text_edit->unbind_value();

  m_tms_pair_line_style->setEnabled(false);
  m_tms_pair_dist_color->setEnabled(false);
  m_tms_pair_line_size->setEnabled(false);
  m_tms_pair_show->setEnabled(false);
  m_tms_pair_show->setChecked(false);
  m_tms_font_screen_size->setEnabled(false);
  m_tms_pair_label_style->setEnabled(false);
  m_tms_pair_label_enabled->setEnabled(false);
  m_tms_pair_delta_angle->setEnabled(false);
  m_tms_pair_delta_offset->setEnabled(false);

  m_tms_pair_term_size->setEnabled(false);
  m_tms_pair_term_style->setEnabled(false);

  m_tms_pair_action_sel->setEnabled(false);
  m_tms_pair_action_del->setEnabled(false);
  m_tms_pair_action_copy->setEnabled(false);

  m_tms_pair_custom_text_edit->setEnabled(false);
  m_tms_pair_custom_text_enabled->setEnabled(false);

}

void geom_view_obj_insp_widget_t::bind_angle_msr_tab() {

  if (b_al) {

    int cur_msr = b_al->m_measure->m_cur_angle_rec_ui - 1;
    auto amr = b_al->m_measure->m_angle_recs.get_hs_child_as_array(cur_msr);

    if (amr) {

      m_tms_angle_enabled->bind_value(&amr->m_show);
      m_tms_angle_enabled->setEnabled(true);

      m_tms_angle_order->bind_value(&amr->m_order);
      m_tms_angle_order->setEnabled(true);

    } else {
      unbind_angle_msr_tab();
    }

  }

}

void geom_view_obj_insp_widget_t::unbind_angle_msr_tab() {
  m_tms_angle_enabled->unbind_value();
  m_tms_angle_enabled->setEnabled(false);
  m_tms_angle_order->unbind_value();
  m_tms_angle_order->setEnabled(false);
}

void geom_view_obj_insp_widget_t::bind_sel_tab() {
  m_ts_auto_apply->bind_value(&b_al->m_selg->m_auto_apply);
}

void geom_view_obj_insp_widget_t::unbind_sel_tab() {
  m_ts_auto_apply->unbind_value();
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
      m_anim_buttons->show();
      m_gb_anim_timeline->show();
    } else {
      m_gb_anim_timeline->hide();
      m_anim_buttons->hide();
    }
  }
}

void geom_view_obj_insp_widget_t::update_mod_tab() {
  if (b_al) {
    if (b_al->m_parent_ws
        && b_al->m_parent_ws->get_edit_type() == ws_edit_type_e::edit_content) {

      m_tm_grp_op_flip_a_p->setEnabled(b_al->m_geom->get_DIM() > 0);
      m_tm_grp_op_flip_b_p->setEnabled(b_al->m_geom->get_DIM() > 1);
      m_tm_grp_op_flip_c_p->setEnabled(b_al->m_geom->get_DIM() > 2);
      m_tm_grp_op_flip_a_n->setEnabled(b_al->m_geom->get_DIM() > 0);
      m_tm_grp_op_flip_b_n->setEnabled(b_al->m_geom->get_DIM() > 1);
      m_tm_grp_op_flip_c_n->setEnabled(b_al->m_geom->get_DIM() > 2);
      set_tab_enabled(m_tab_modify, true);
      m_tm_override_atom_color->unbind_value();
      m_tm_override_atom_radii->unbind_value();

      if (b_al->m_geom->no_aselected()) {
        m_tm_gb_single_atom->hide();
        m_tm_gb_add_atom->show();
        m_tm_gb_pair_dist->hide();
        m_tm_gb_u_scale->hide();
        m_tm_gb_pair_creation->hide();
        m_tm_gb_translate->hide();
        m_tm_gb_bc_rot->hide();
        m_tm_gb_grp_op->hide();
        m_tm_gb_override_atom->hide();
        //update atom names combobox
        fill_combo_with_atom_types(m_tm_add_atom_combo, b_al);
      }

      if (b_al->m_geom->num_aselected() == 1) {
        m_tm_gb_single_atom->show();
        m_tm_gb_add_atom->hide();
        m_tm_gb_pair_dist->hide();
        m_tm_gb_u_scale->hide();
        m_tm_gb_pair_creation->hide();
        m_tm_gb_translate->show();
        m_tm_gb_bc_rot->hide();
        m_tm_gb_grp_op->show();
        m_tm_gb_override_atom->hide();
        fill_combo_with_atom_types(m_tm_single_atom_combo, b_al);
        auto oval = b_al->m_geom->nth_aselected(0);
        if (oval) {
          auto val = *oval;
          m_tm_single_atom_info->setText(
              QString::fromStdString(fmt::format("№{} {}", val.m_atm, val.m_idx)));

          m_tm_single_atom_combo->setCurrentText(
              QString::fromStdString(b_al->m_geom->atom_name(val.m_atm)));

          m_tm_single_atom_v3->sb_x->setValue(double(b_al->m_geom->pos(val.m_atm)[0]));
          m_tm_single_atom_v3->sb_y->setValue(double(b_al->m_geom->pos(val.m_atm)[1]));
          m_tm_single_atom_v3->sb_z->setValue(double(b_al->m_geom->pos(val.m_atm)[2]));
        }
      }

      if (b_al->m_geom->num_aselected() == 2) {
        m_tm_gb_add_atom->hide();
        m_tm_gb_single_atom->hide();
        m_tm_gb_pair_dist->show();
        m_tm_gb_u_scale->hide();
        m_tm_gb_pair_creation->show();
        m_tm_gb_translate->show();
        m_tm_gb_bc_rot->show();
        m_tm_gb_grp_op->show();
        m_tm_gb_override_atom->hide();
        auto oval1 = b_al->m_geom->nth_aselected(0);
        auto oval2 = b_al->m_geom->nth_aselected(1);
        if (oval1 && oval2) {
          auto val1 = *oval1;
          auto val2 = *oval2;
          m_tm_pair_dist_atom1->setText(
              QString::fromStdString(fmt::format("{}{} {}",
                                                 b_al->m_geom->atom_name(val1.m_atm),
                                                 val1.m_atm,
                                                 val1.m_idx)));
          m_tm_pair_dist_atom2->setText(
              QString::fromStdString(fmt::format("{}{} {}",
                                                 b_al->m_geom->atom_name(val2.m_atm),
                                                 val2.m_atm,
                                                 val2.m_idx)));
          if (val1.m_idx.is_zero() && val2.m_idx.is_zero()) {
            m_tm_pair_dist_spinbox->show();
            float dist_btw = (b_al->m_geom->pos(val1.m_atm, val1.m_idx) -
                              b_al->m_geom->pos(val2.m_atm, val2.m_idx)).norm();
            m_tm_pair_dist_spinbox->blockSignals(true);
            m_tm_pair_dist_spinbox->setValue(double(dist_btw));
            m_tm_pair_dist_spinbox->blockSignals(false);
            m_tm_pair_dist_note_label->show();
            m_tm_pair_dist_spinbox->show();
            m_tm_pair_dist_swap_atoms->show();
          } else {
            m_tm_pair_dist_note_label->hide();
            m_tm_pair_dist_spinbox->hide();
            m_tm_pair_dist_swap_atoms->hide();
          }
          //atom between
          fill_combo_with_atom_types(m_tm_pair_creation_combo, b_al);
        }
      }

      if (b_al->m_geom->num_aselected() > 2) {
        m_tm_gb_add_atom->hide();
        m_tm_gb_single_atom->hide();
        m_tm_gb_pair_creation->hide();
        m_tm_gb_pair_dist->hide();
        m_tm_gb_u_scale->show();
        m_tm_gb_translate->show();
        m_tm_gb_bc_rot->show();
        m_tm_gb_grp_op->show();
        m_tm_gb_override_atom->hide();
      }

      if (b_al->m_geom->num_aselected() > 0) {
        if (b_al->m_geom->get_DIM() == 3) {
          //tm_translate_coord_type_label->show();
          m_tm_translate_coord_type->show();
        } else {
          m_tm_translate_coord_type->setCurrentIndex(0);
          m_tm_translate_coord_type->hide();
          //tm_translate_coord_type_label->hide();
        }
        /* begin atom override */
        if (!b_al->any_of_sel_xfield_equal<bool>(xg_override, false)) {
          std::set<size_t> atoms_id_to_bind;
          std::transform(
              b_al->m_geom->cbegin_selected(),
              b_al->m_geom->cend_selected(),
              std::inserter(atoms_id_to_bind, atoms_id_to_bind.begin()),
              [](auto &sel_rec){return sel_rec.m_atm;}
              );
          m_tm_gb_override_atom->show();
          m_tm_override_atom_color->bind_value(b_al->m_geom.get(),
                                               {xg_ccr, xg_ccg, xg_ccb},
                                               std::set<size_t>(atoms_id_to_bind)
                                               );
          m_tm_override_atom_radii->bind_value(b_al->m_geom.get(),
                                               xg_atom_r,
                                               std::move(atoms_id_to_bind)
                                               );
        }
        /* end atom override */
      }

    } else {
      set_tab_enabled(m_tab_modify, false);
      m_tm_gb_add_atom->hide();
      m_tm_gb_pair_creation->hide();
      m_tm_gb_single_atom->hide();
      m_tm_gb_pair_dist->hide();
      m_tm_gb_u_scale->hide();
      m_tm_gb_translate->hide();
      m_tm_gb_bc_rot->hide();
      m_tm_gb_grp_op->hide();
      m_tm_gb_override_atom->hide();
    }

  }

}

void geom_view_obj_insp_widget_t::update_msr_tab() {

  if (b_al) {

    m_tms_pair_cur_msr->blockSignals(true);
    m_tms_pair_cur_msr->clear();
    m_tms_pair_cur_msr->addItem(tr("None"));

    index zero = index::D(b_al->m_geom->get_DIM()).all(0);

    for (size_t i = 0; i < b_al->m_measure->m_dist_recs.get_hs_children_count(); i++) {
      auto dmr = b_al->m_measure->m_dist_recs.get_hs_child_as_array(i);
      if (!dmr)
        continue;
      //compose dist msr name
      int at1 = dmr->m_at1;
      int at2 = dmr->m_at2;
      bool at1_img = dmr->m_idx1 != zero;
      bool at2_img = dmr->m_idx2 != zero;
      std::string item_name = fmt::format("[{}] {}{}{} - {}{}{}",
                                          i,
                                          b_al->m_geom->atom_name(at1),
                                          at1,
                                          at1_img ? "*" : "",
                                          b_al->m_geom->atom_name(at2),
                                          at2,
                                          at2_img ? "*" : "");

      m_tms_pair_cur_msr->addItem(QString::fromStdString(item_name));
    }

    m_tms_pair_cur_msr->setCurrentIndex(b_al->m_measure->m_cur_dist_rec_ui);
    update_dist_msr_tab_info();
    m_tms_pair_cur_msr->blockSignals(false);

    //angle recs
    m_tms_angle_cur_msr->blockSignals(true);
    m_tms_angle_cur_msr->clear();
    m_tms_angle_cur_msr->addItem(tr("None"));
    for (size_t i = 0; i < b_al->m_measure->m_angle_recs.get_hs_children_count(); i++) {
      auto amr = b_al->m_measure->m_angle_recs.get_hs_child_as_array(i);
      if (!amr)
        continue;
      int at1 = amr->m_at1;
      int at2 = amr->m_at2;
      int at3 = amr->m_at3;
      bool at1_img = amr->m_idx1 != zero;
      bool at2_img = amr->m_idx2 != zero;
      bool at3_img = amr->m_idx2 != zero;
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
      m_tms_angle_cur_msr->addItem(QString::fromStdString(item_name));
    }
    m_tms_angle_cur_msr->setCurrentIndex(b_al->m_measure->m_cur_angle_rec_ui);
    update_angle_msr_tab_info();
    m_tms_angle_cur_msr->blockSignals(false);
  }

}

void geom_view_obj_insp_widget_t::update_dist_msr_tab_info() {
  if (b_al) {
    //transform dist msr id
    int cur_dist_msr = b_al->m_measure->m_cur_dist_rec_ui - 1;
    if (cur_dist_msr < 0 || b_al->m_measure->m_dist_recs.get_hs_children_count() == 0) {
      const QString empty_label = "-";
      m_tms_pair_at1_info->setText(empty_label);
      m_tms_pair_at2_info->setText(empty_label);
      unbind_dist_msr_tab();
    } else {
      auto rec = b_al->m_measure->m_dist_recs.get_hs_child_as_array(cur_dist_msr);
      if (rec) {
        QString atom1_info = tr("%1 [%2] %3")
                                 .arg(QString::fromStdString(b_al->m_geom->atom_name(rec->m_at1)))
                                 .arg(rec->m_at1)
                                 .arg(QString::fromStdString(fmt::format("{}",rec->m_idx1)));
        m_tms_pair_at1_info->setText(atom1_info);
        QString atom2_info = tr("%1 [%2] %3")
                                 .arg(QString::fromStdString(b_al->m_geom->atom_name(rec->m_at2)))
                                 .arg(rec->m_at2)
                                 .arg(QString::fromStdString(fmt::format("{}",rec->m_idx2)));
        m_tms_pair_at2_info->setText(atom2_info);

        bind_dist_msr_tab();
      }
    }
  }
}

void geom_view_obj_insp_widget_t::update_angle_msr_tab_info()  {
  if (b_al) {
    //transform dist msr id
    int cur_angle_msr = b_al->m_measure->m_cur_angle_rec_ui - 1;
    if (cur_angle_msr < 0 || b_al->m_measure->m_angle_recs.get_hs_children_count() == 0) {
      const QString empty_label = "-";
      m_tms_angle_at1_info->setText(empty_label);
      m_tms_angle_at2_info->setText(empty_label);
      m_tms_angle_at3_info->setText(empty_label);
      unbind_angle_msr_tab();
    } else {
      auto rec = b_al->m_measure->m_angle_recs.get_hs_child_as_array(cur_angle_msr);
      if (rec) {
        QString atom1_info = tr("%1 [%2] %3")
                                 .arg(QString::fromStdString(b_al->m_geom->atom_name(rec->m_at1)))
                                 .arg(rec->m_at1)
                                 .arg(QString::fromStdString(fmt::format("{}", rec->m_idx1)));
        m_tms_angle_at1_info->setText(atom1_info);
        QString atom2_info = tr("%1 [%2] %3")
                                 .arg(QString::fromStdString(b_al->m_geom->atom_name(rec->m_at2)))
                                 .arg(rec->m_at2)
                                 .arg(QString::fromStdString(fmt::format("{}", rec->m_idx2)));
        m_tms_angle_at2_info->setText(atom2_info);
        QString atom3_info = tr("%1 [%2] %3")
                                 .arg(QString::fromStdString(b_al->m_geom->atom_name(rec->m_at3)))
                                 .arg(rec->m_at3)
                                 .arg(QString::fromStdString(fmt::format("{}", rec->m_idx3)));
        m_tms_angle_at3_info->setText(atom3_info);
        bind_angle_msr_tab();
      }
    }
  }
}

void geom_view_obj_insp_widget_t::update_sel_tab() {
  if (b_al) {
    bind_sel_tab();
    m_ts_total_grps->setText(QString("%1").arg(b_al->m_selg->m_sel_grps.size()));
    m_ts_cur_sel_grp->blockSignals(true);
    m_ts_cur_sel_grp->clear();
    m_ts_cur_sel_grp->addItem(tr("None"));
    for (size_t i = 0 ; i < b_al->m_selg->m_sel_grps.size(); i++)
      m_ts_cur_sel_grp->addItem(QString("[%1] %2")
                                    .arg(i)
                                    .arg(QString::fromStdString(b_al->m_selg->m_sel_grps[i].m_name)));
    m_ts_cur_sel_grp->blockSignals(false);
    if (b_al->m_parent_ws
        && b_al->m_parent_ws->get_edit_type() == ws_edit_type_e::edit_content) {
      set_tab_enabled(m_tab_select, false);
    } else {
      set_tab_enabled(m_tab_select, false);
    }
  } else {
    unbind_sel_tab();
  }
}

void geom_view_obj_insp_widget_t::update_xgeom_tab() {
  if (b_al) {
    //tab xgeom
    qt_hlp::vrt_resize_tv_to_cnt(m_txg_info_tv);
  }
}

void geom_view_obj_insp_widget_t::fill_combo_with_atom_types(QComboBox *combo, geom_view_t *_al) {
  if (_al && combo) {
    combo->clear();
    for (auto i = 0 ; i < _al->m_geom->n_atom_types(); i++)
      combo->addItem(QString::fromStdString(_al->m_geom->atom_of_type(i)));
  }
}

void geom_view_obj_insp_widget_t::tab_modify_flip_cell_clicked(size_t flip_dim, float flip_magn) {
  if (b_al)
    geom_view_tools_t::flip_sel_atoms_in_cell(b_al, flip_dim, flip_magn);
}

void geom_view_obj_insp_widget_t::resizeEvent(QResizeEvent *event) {
  ws_item_obj_insp_widget_t::resizeEvent(event);
}

geom_view_obj_insp_widget_t::geom_view_obj_insp_widget_t() : ws_item_obj_insp_widget_t() {

  m_tab_disp = def_tab(tr("Display settings"),
                       "://images/monitor.svg");

  m_tab_anim = def_tab(tr("Animation"),
                       "://images/film.svg",
                       "://images/film_d.svg");

  m_tab_modify = def_tab(tr("Modify"),
                         "://images/outline-build-24px.svg",
                         "://images/outline-build-24px_d.svg");

  m_tab_measurement = def_tab(tr("Measurement"),
                              "://images/outline-straighten-24px.svg");

  m_tab_select = def_tab(tr("Atom selection groups"),
                         "://images/outline-select_all-24px.svg",
                         "://images/outline-select_all-24px_d.svg");

  m_tab_xgeom = def_tab(tr("XGeometry fields"),
                        "://images/format_list_numbered-24px.svg",
                        "://images/format_list_numbered-24px_d.svg");

  construct_general_tab();
  construct_disp_tab();
  construct_anim_tab();
  construct_msr_tab();
  construct_mod_tab();
  construct_sel_tab();
  construct_xgeom_tab();

  m_tab_disp_switch->button(0)->setChecked(true);
  disp_switch_cur_changed(0);

  tms_switch->button(0)->setChecked(true);
  msr_switch_current_changed(0);

  m_txg_switch->button(0)->setChecked(true);
  xgeom_switch_current_changed(0);

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
          &geom_view_obj_insp_widget_t::cur_it_list_sel_changed);

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
          &geom_view_obj_insp_widget_t::update_msr_tab);

}

void geom_view_obj_insp_widget_t::cur_anim_index_changed(int index) {
  if (b_al) {
    if (index < int(b_al->m_anim->get_total_anims())) {
      if (b_al->m_anim->m_cur_anim != index) {
        b_al->m_anim->m_cur_anim = index;
        b_al->m_anim->m_cur_anim_time = 0.0f;
        b_al->m_anim->update_geom_to_anim();
      }
      auto cur_anim = b_al->m_anim->get_current_anim();
      if (cur_anim && cur_anim->m_anim_type != geom_anim_e::anim_static) {
        m_anim_play->blockSignals(true);
        m_anim_play->setChecked(b_al->m_anim->m_play_anim.get_value());
        m_anim_play->blockSignals(false);
      } else {
        m_anim_play->blockSignals(true);
        m_anim_play->setChecked(false);
        m_anim_play->blockSignals(false);
      }
      //m_anim_total_frames_in_anim->setText(tr("%1").arg(cur_anim->frames.size()));
      m_anim_timeline_slider->setMinimum(0);
      m_anim_timeline_slider->setMaximum(cur_anim->frames.size()-1);
      anim_updated_external();
    }
    update_anim_tab_visibility();
  }
}

void geom_view_obj_insp_widget_t::play_anim_button_toggle(bool value) {
  if (b_al) {
    b_al->m_anim->m_play_anim.set_value(std::move(value));
  }
}

void geom_view_obj_insp_widget_t::anim_updated_external() {
  if (b_al) {
    int current_frame_truncated = b_al->m_anim->current_frame_truncated();
    // remap frame id from 0..size-1 1..size
    m_anim_cur_frame->setText(tr("[Frm:%1/%2][Tot. Anims.:%3]")
                                  .arg(current_frame_truncated+1)
                                  .arg(b_al->m_anim->current_frame_count())
                                  .arg(b_al->m_anim->m_anim_data.size()));
    m_anim_timeline_slider->blockSignals(true);
    m_anim_timeline_slider->setValue(current_frame_truncated);
    m_anim_timeline_slider->blockSignals(false);

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
  if (b_al)
    b_al->m_anim->update_current_frame_to_begin();
}

void geom_view_obj_insp_widget_t::anim_button_end_clicked() {
  if (b_al)
    b_al->m_anim->update_current_frame_to_end();
}

void geom_view_obj_insp_widget_t::anim_button_frame_move_forward_clicked() {
  if (b_al)
    b_al->m_anim->manual_frame_manipulate(1);
}

void geom_view_obj_insp_widget_t::anim_button_frame_move_backward_clicked() {
  if (b_al)
    b_al->m_anim->manual_frame_manipulate(-1);
}

void geom_view_obj_insp_widget_t::anim_act_ren_clicked() {
  if (!b_al || !b_al->m_anim->animable())
    return;
  auto cur_anim = b_al->m_anim->get_current_anim();
  if (!cur_anim)
    return;
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
  if (!b_al || !b_al->m_anim->animable())
    return;
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
  int new_anim_idx = std::clamp<int>(cur_anim_idx - 1, 0, b_al->m_anim->m_anim_data.size());
  b_al->m_anim->m_cur_anim = new_anim_idx;
  update_from_ws_item();
}

void geom_view_obj_insp_widget_t::disp_switch_cur_changed(int index) {
  m_gb_disp_s->setVisible(index == 0);
  m_gb_periodic_related_render->setVisible(index == 0 && b_al && b_al->m_geom->get_DIM() > 0);
  m_gb_disp_shading->setVisible(index == 0);
  m_gb_disp_labels->setVisible(index == 1);
  m_gb_disp_bt->setVisible(index == 2);
  m_gb_disp_type_spec_rend->setVisible(index == 3);
}

void geom_view_obj_insp_widget_t::cell_changed() {
  if (b_al) {
    if (b_al->m_geom->get_DIM() > 0) {
      m_tg_gb_cell_tbl->setRowCount(b_al->m_geom->get_DIM());
      QStringList table_hdr_cell_v;
      table_hdr_cell_v.push_back("a");
      table_hdr_cell_v.push_back("b");
      table_hdr_cell_v.push_back("c");
      m_tg_gb_cell_tbl->setVerticalHeaderLabels(table_hdr_cell_v);
      for (int c = 0; c < b_al->m_geom->get_DIM(); c++)
        for (int i = 0; i < 3; i++) {
          float cell_amp = b_al->m_geom->cell.v[c][i];
          QTableWidgetItem *n_ax = new QTableWidgetItem(tr("%1").arg(double(cell_amp)));
          m_tg_gb_cell_tbl->setItem(c, i, n_ax);
        }
      m_tg_gb_cell_tbl->resizeRowsToContents();
      m_tg_gb_cell->show();
    } else {
      m_tg_gb_cell->hide();
    }
  }
}

void geom_view_obj_insp_widget_t::draw_subcells_changed(int state) {

  qt_hlp::form_lt_ctrl_visibility(state == Qt::Checked,
                                  m_gb_periodic_related_render_lt,
                                  m_periodic_draw_subcells,
                                  m_periodic_subcells_idx_lbl,
                                  m_periodic_subcells_idx);

  qt_hlp::form_lt_ctrl_visibility(state == Qt::Checked,
                                  m_gb_periodic_related_render_lt,
                                  m_periodic_subcells_idx,
                                  m_periodic_subcells_color_lbl,
                                  m_periodic_subcells_clr);

}

void geom_view_obj_insp_widget_t::update_anim_section_status() {
  set_tab_enabled(m_tab_anim, b_al->m_anim->animable());
}

void geom_view_obj_insp_widget_t::cur_it_list_sel_changed() {
  if (b_al && b_al->is_selected()) {
    update_mod_tab();
  }
}

void geom_view_obj_insp_widget_t::mod_add_atom_button_clicked() {
  app_state_t *astate = app_state_t::get_inst();
  if (b_al) {
    vector3<float> new_atom_pos{
        float(m_tm_add_atom_vec3->sb_x->value()),
        float(m_tm_add_atom_vec3->sb_y->value()),
        float(m_tm_add_atom_vec3->sb_z->value())
    };
    std::string new_atom_name = m_tm_add_atom_combo->currentText().toStdString();
    b_al->ins_atom(new_atom_name, new_atom_pos);
    update_anim_section_status();
    astate->make_viewport_dirty();
  }
}

void geom_view_obj_insp_widget_t::mod_single_atom_button_clicked() {
  if (b_al && b_al->m_geom->num_selected() == 1) {
    auto oval = b_al->m_geom->nth_selected(0);
    if (oval) {
      auto val = *oval;
      b_al->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
      b_al->upd_atom(val.m_atm, m_tm_single_atom_combo->currentText().toStdString(),
                     vector3<float>(float(m_tm_single_atom_v3->sb_x->value()),
                                    float(m_tm_single_atom_v3->sb_y->value()),
                                    float(m_tm_single_atom_v3->sb_z->value())),
                     false /* off hs rec inside upd_atom procedure*/);
      b_al->end_recording();
      update_anim_section_status();
    }
  }
}

void geom_view_obj_insp_widget_t::mod_single_atom_delete_button_clicked() {
  app_state_t *astate = app_state_t::get_inst();
  if (b_al) {
    b_al->delete_selected_atoms();
    astate->make_viewport_dirty();
    update_anim_section_status();
  }
}

void geom_view_obj_insp_widget_t::mod_pair_dist_spinbox_value_changed(double newval) {
  app_state_t *astate = app_state_t::get_inst();
  if (b_al && b_al->m_geom->num_selected() == 2) {
    auto at1 = b_al->m_geom->nth_selected(0);
    auto at2 = b_al->m_geom->nth_selected(1);
    if (at1 && at2) {
      pair_dist_mode_e mode;
      mode = static_cast<pair_dist_mode_e>(m_tm_pair_dist_t_mode->currentIndex());
      b_al->update_inter_atomic_dist_ex(float(newval), (*at1).m_atm, (*at2).m_atm, mode);
      astate->make_viewport_dirty();
    }
  }
}

void geom_view_obj_insp_widget_t::mod_pair_dist_swap_button_clicked() {
  if (!(b_al && b_al->m_geom->num_aselected() != 2))
    return;
  auto oval1 = b_al->m_geom->nth_aselected(0);
  auto oval2 = b_al->m_geom->nth_aselected(1);
  if (oval1 && oval2) {
    auto val1 = *oval1;
    auto val2 = *oval2;
    if (val1.m_idx.is_zero() && val2.m_idx.is_zero())
      b_al->swap_atoms(val1.m_atm, val2.m_atm);
  }
}

void geom_view_obj_insp_widget_t::mod_add_atom_between_pair() {
  app_state_t *astate = app_state_t::get_inst();
  if (!b_al || b_al->m_geom->num_selected() != 2)
    return;
  auto oval1 = b_al->m_geom->nth_selected(0);
  auto oval2 = b_al->m_geom->nth_selected(1);
  if (oval1
      && oval2
      && (*oval1).m_idx.is_zero()
      && (*oval2).m_idx.is_zero()) {
    auto val1 = *oval1;
    auto val2 = *oval2;
    vector3<float> r_btw{0.0, 0.0, 0.0};
    r_btw = (b_al->m_geom->pos(val1.m_atm, val1.m_idx) +
             b_al->m_geom->pos(val2.m_atm, val2.m_idx))*0.5f;
    std::string new_atom_name = m_tm_pair_creation_combo->currentText().toStdString();
    b_al->ins_atom(new_atom_name, r_btw);
    update_anim_section_status();
    astate->make_viewport_dirty();
  }
}

void geom_view_obj_insp_widget_t::mod_barycentric_scale_button_clicked() {
  app_state_t *astate = app_state_t::get_inst();
  if (b_al && b_al->m_geom->num_selected() > 0) {
    b_al->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);
    vector3<float> center{0.0f, 0.0f, 0.0f};
    for (auto i = 0; i < b_al->m_geom->num_selected(); i++) {
      auto rec = b_al->m_geom->nth_selected(i);
      if (rec)
        center += b_al->m_geom->pos((*rec).m_atm);;
    }
    center /= b_al->m_geom->num_selected();
    for (auto i = 0; i < b_al->m_geom->num_selected(); i++) {
      auto rec =
          b_al->m_geom->nth_selected(i);
      if (!rec) continue;
      vector3<float> new_pos_dist = center - b_al->m_geom->pos((*rec).m_atm);
      vector3<float> new_pos = b_al->m_geom->pos((*rec).m_atm);
      auto scaleval = m_tm_u_scale_v3_val.get_value();
      new_pos[0] +=  (1 - scaleval[0]) * new_pos_dist[0];
      new_pos[1] +=  (1 - scaleval[1]) * new_pos_dist[1];
      new_pos[2] +=  (1 - scaleval[2]) * new_pos_dist[2] ;
      b_al->upd_atom((*rec).m_atm, new_pos, false);
    }
    b_al->end_recording();
    update_anim_section_status();
    astate->make_viewport_dirty();
  }
}

void geom_view_obj_insp_widget_t::mod_translate_selected_atoms_clicked() {
  app_state_t *astate = app_state_t::get_inst();
  if (b_al) {
    vector3<float> tr_vec(float(m_tm_translate_v3->sb_x->value()),
                          float(m_tm_translate_v3->sb_y->value()),
                          float(m_tm_translate_v3->sb_z->value()));
    if (m_tm_translate_coord_type->currentIndex() == 1 && b_al->m_geom->get_DIM() == 3) {
      vector3<float> tr_vec_c = tr_vec;
      tr_vec = b_al->m_geom->cell.frac2cart(tr_vec_c);
    }
    b_al->translate_selected(tr_vec);
  }
  astate->make_viewport_dirty();
}

void geom_view_obj_insp_widget_t::mod_translate_coord_type_changed(int coord_type) {
  if (coord_type == 0)
    m_tm_translate_v3->set_min_max_step_dec(-100, 100, 0.01);
  else
    m_tm_translate_v3->set_min_max_step_dec(-1.0, 1.0, 0.01);
}

void geom_view_obj_insp_widget_t::mod_bc_rot_angle_type_change(int new_angle_type) {
  if (new_angle_type == 0)
    m_tm_bc_rot_angle->setSingleStep(0.5);
  else
    m_tm_bc_rot_angle->setSingleStep(0.01);
  m_tm_bc_rot_angle->setValue(0.0);
}

void geom_view_obj_insp_widget_t::mod_bc_rot_apply() {
  if (b_al
      && b_al->m_parent_ws
      && b_al->m_parent_ws->get_edit_type() == ws_edit_type_e::edit_content) {
    float angle;
    if (m_tm_bc_rot_angle_type->currentIndex() == 0)
      angle = float(m_tm_bc_rot_angle->value() * qpp::pi) / 180.0f;
    else //bypass input value
      angle = float(m_tm_bc_rot_angle->value());
    vector3<float> rot_center = b_al->get_gizmo_content_barycenter();
    vector3<float> rot_axis{vector3<float>::Zero()};
    switch (m_tm_bc_rot_axis->currentIndex()) {
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

void geom_view_obj_insp_widget_t::mod_group_op_sv_show() {
  if (b_al) {
    b_al->sv_modify_selected(false);
    update_from_ws_item();
  }
}

void geom_view_obj_insp_widget_t::mod_group_op_sv_hide() {
  if (b_al) {
    b_al->sv_modify_selected(true);
    update_from_ws_item();
  }
}

void geom_view_obj_insp_widget_t::mod_group_op_sv_hide_invert() {
  if (b_al) {
    b_al->sv_hide_invert_selected();
    update_from_ws_item();
  }
}

void geom_view_obj_insp_widget_t::mod_group_op_sel_ngbs() {
  app_state_t *astate = app_state_t::get_inst();
  if (b_al)
    b_al->sel_selected_atoms_ngbs();
  astate->make_viewport_dirty();
}

void geom_view_obj_insp_widget_t::mod_group_op_del_sel() {
  app_state_t *astate = app_state_t::get_inst();
  if (b_al)
    b_al->delete_selected_atoms();
  astate->make_viewport_dirty();
}

void geom_view_obj_insp_widget_t::mod_group_op_make_static_anim() {
  if (!b_al)
    return;
  b_al->m_anim->make_animable();
  std::string _anim_name = fmt::format("static_{}", b_al->m_anim->get_total_anims());
  b_al->m_anim->make_anim(_anim_name, geom_anim_e::anim_static, 1);
}

void geom_view_obj_insp_widget_t::mod_group_op_rebond_all() {
  if (!b_al)
    return;
  b_al->rebond();
}

void geom_view_obj_insp_widget_t::msr_switch_current_changed(int index) {
  m_tms_common_settings_gb->setVisible(index == 0);
  m_tms_pair_dist_gb->setVisible(index == 1);
  m_tms_angle_gb->setVisible(index == 2);
}

void geom_view_obj_insp_widget_t::msr_pair_cur_idx_changed(int index) {
  app_state_t *astate = app_state_t::get_inst();
  if (b_al) {
    b_al->m_measure->m_cur_dist_rec_ui = index;
    update_dist_msr_tab_info();
  }
  astate->make_viewport_dirty();
}

void geom_view_obj_insp_widget_t::msr_angle_cur_idx_changed(int index) {
  app_state_t *astate = app_state_t::get_inst();
  if (b_al) {
    b_al->m_measure->m_cur_angle_rec_ui = index;
    update_angle_msr_tab_info();
  }
  astate->make_viewport_dirty();
}

void geom_view_obj_insp_widget_t::msr_pair_select_clicked() {
  if (b_al)
    b_al->m_measure->dist_select_selected_atoms();
}

void geom_view_obj_insp_widget_t::msr_pair_delete_clicked() {
  if (b_al)
    b_al->m_measure->dist_delete_selected();
}

void geom_view_obj_insp_widget_t::msr_pair_copy_clicked() {
  if (b_al)
    b_al->m_measure->dist_copy_selected();
}

void geom_view_obj_insp_widget_t::xgeom_switch_current_changed(int index) {
  m_txg_gb_info->setVisible(index == 0);
  m_txg_gb_comp_prop->setVisible(index == 1);
}

void geom_view_obj_insp_widget_t::cur_ws_edit_mode_changed() {
  update_mod_tab();
  update_msr_tab();
  update_sel_tab();
}

void geom_view_obj_insp_widget_t::cur_it_selected_content_changed() {
  if (b_al) {
    update_mod_tab();
    update_msr_tab();
    update_sel_tab();
  }
}

void geom_view_obj_insp_widget_t::type_summary_clicked(const QModelIndex &index) {
  app_state_t *astate = app_state_t::get_inst();
  if (!b_al)
    return;
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

void geom_view_obj_insp_widget_t::rebond_button_clicked() {
  if (!b_al)
    return;
  b_al->rebond();
}

void geom_view_obj_insp_widget_t::clear_color_override_button_clicked() {
  app_state_t *astate = app_state_t::get_inst();
  if (!b_al)
    return;
  b_al->m_type_color_override.clear();
  update_from_ws_item();
  astate->make_viewport_dirty();
}

