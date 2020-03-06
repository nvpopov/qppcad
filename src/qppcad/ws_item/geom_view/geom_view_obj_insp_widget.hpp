#ifndef QPPCAD_WS_ATOMS_LIST_OBJ_INSP_H
#define QPPCAD_WS_ATOMS_LIST_OBJ_INSP_H

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ui/qrealspinbox_delegate.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>

#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include <QSlider>
#include <QButtonGroup>

namespace qpp {

  namespace cad {

    class qtype_specific_rendering_model_t;
    class qbonding_table_model_t;
    class qxgeom_fields_model_t;

    class geom_view_obj_insp_widget_t final: public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:

        geom_view_t *b_al{nullptr}; // binded atoms list

        // Summary group
        //qspoiler_widget_t *m_tg_geom_sum_wdgt;
        //QFormLayout *m_tg_geom_sum_lt;

        QLabel *m_tg_geom_sum_total_atoms;
        //QLabel *m_tg_geom_sum_total_types;

        qspoiler_widget_t *m_tg_type_sum_wdgt;
        QVBoxLayout *m_tg_type_sum_lt;
        QTableWidget *m_tg_type_sum_tbl;
        QPushButton *m_type_sum_clear_tclr_override;

        qspoiler_widget_t *m_tg_gb_cell;
        QVBoxLayout *m_tg_gb_cell_lt;
        QTableWidget *m_tg_gb_cell_tbl;

        //START: Display tab
        ws_item_tab_widget_t *m_tab_disp;

        QButtonGroup *m_tab_disp_switch;

        qspoiler_widget_t *m_gb_disp_s;
        QFormLayout *m_gb_disp_s_lt;
        qbinded_bool_named_vector_t *m_disp_s_draw_atoms_bonds;
        qbinded_bool_named_vector_t *m_disp_s_draw_img_atoms_bonds;
//        qbinded_float_spinbox_t *m_disp_s_atom_scale;
//        qbinded_float_spinbox_t *m_disp_s_bond_scale;
        qbinded_float_named_vector_t *m_disp_s_scale;
        qbinded_combobox_t *m_disp_s_render_style;
        qbinded_combobox_t *m_disp_s_color_mode;

        qspoiler_widget_t *m_gb_disp_labels;
        QFormLayout *m_gb_disp_labels_lt;
        qbinded_combobox_t *m_disp_labels_style;
        qbinded_checkbox_t *m_disp_inplace_labels;
        qbinded_float2_input_t *m_disp_inplace_offset;
        qbinded_int_spinbox_t *m_disp_labels_size;
        qbinded_checkbox_t *m_disp_sl_labels;
        qbinded_checkbox_t *m_disp_labels_screen_scale;
        qbinded_checkbox_t *m_disp_labels_draw_outline;
        qbinded_float_spinbox_t *m_disp_labels_outline_size;
        qbinded_checkbox_t *m_disp_labels_draw_axis_labels;

        qbinded_bool_named_vector_t *m_disp_s_sel_vis_p_affect_bonds;

        // Periodic related render
        qspoiler_widget_t *m_gb_periodic_related_render;
        QFormLayout *m_gb_periodic_related_render_lt;

        qbinded_bool_named_vector_t *m_periodic_draw_settings;

        //qbinded_checkbox_t *periodic_draw_cell_v;
        qbinded_float_spinbox_t *m_periodic_cell_v_ratio;
        qbinded_color3_input_t *m_periodic_cell_vectors_color;

        //qbinded_checkbox_t *periodic_draw_cell;
        qbinded_color3_input_t *m_periodic_cell_color;
        qbinded_float3_input_t *m_periodic_cell_offset;
        qbinded_checkbox_t *m_periodic_draw_subcells;

        QLabel *m_periodic_subcells_idx_lbl;
        qbinded_int3_input_t *m_periodic_subcells_idx;
        QLabel *m_periodic_subcells_color_lbl;
        qbinded_color3_input_t *m_periodic_subcells_clr;
        // End of Periodic related render

        qspoiler_widget_t *m_gb_disp_shading;
        QFormLayout *m_gb_disp_shading_lt;
        qbinded_checkbox_t *m_disp_shading_spec;
        qbinded_float_spinbox_t *m_disp_shading_spec_value;

        qspoiler_widget_t *m_gb_disp_type_spec_rend;
        QTableView *m_disp_type_spec_tv;
        qtype_specific_rendering_model_t *m_disp_type_spec_mdl;
        QVBoxLayout *m_gb_disp_type_spec_rend_lt;

        qspoiler_widget_t *m_gb_disp_bt;
        QPushButton *m_disp_bt_rebond_btn;
        QVBoxLayout *m_disp_bt_lt;
        QTableView *m_disp_bt;
        qbonding_table_model_t *m_bt_mdl;
        qrealspinbox_delegate_t *m_bt_dist_delegate;
        //END: Display tab

        ws_item_tab_widget_t *m_tab_anim;
        qspoiler_widget_t *m_gb_anim_sum;
//        QLabel *m_anim_total_anims;
        qbinded_bool_named_vector_t *m_anim_settings;
//        qbinded_checkbox_t *m_anim_rebuild_bonds;
//        qbinded_checkbox_t *m_anim_play_cyclic;
//        qbinded_checkbox_t *m_anim_interpolate_anim;
        qbinded_float_spinbox_t *m_anim_speed;
        QComboBox *m_anim_current_anim;
        QHBoxLayout *m_anim_act_lt;
        QPushButton *m_anim_act_ren;
        QPushButton *m_anim_act_del;
        QFormLayout *m_gb_anim_summary_lt;

        qspoiler_widget_t *m_gb_anim_timeline;
        QHBoxLayout *m_anim_timeline_lt;
        //QLabel *m_anim_total_frames_in_anim;
        QLabel *m_anim_cur_frame;
        QSlider *m_anim_timeline_slider;
        qspoiler_widget_t *m_anim_buttons;
        QHBoxLayout *m_anim_buttons_lt;

        QPushButton *m_anim_play;
        QPushButton *m_anim_to_start;
        QPushButton *m_anim_to_end;
        QPushButton *m_anim_frame_fwd;
        QPushButton *m_anim_frame_bck;

        //START TAB MEASUREMENT
        ws_item_tab_widget_t *m_tab_measurement;

        QButtonGroup *tms_switch;

        //common measurements settings
        qspoiler_widget_t *m_tms_common_settings_gb;
        QFormLayout *m_tms_common_settings_gb_lt;
        qbinded_bool_named_vector_t *m_tms_render_dist_angle;
        qbinded_bool_named_vector_t *m_tms_render_dist_angle_legend;

        //pair dist
        qspoiler_widget_t *m_tms_pair_dist_gb;
        QFormLayout *m_tms_pair_dist_gb_lt;
        QComboBox *m_tms_pair_cur_msr;
        QLabel *m_tms_pair_at1_info;
        QLabel *m_tms_pair_at2_info;
        QLabel *m_tms_pair_dist;

        qbinded_color3_input_t *m_tms_pair_dist_color;
        qbinded_checkbox_t *m_tms_pair_enabled;
        qbinded_checkbox_t *m_tms_pair_label_enabled;
        qbinded_combobox_t *m_tms_pair_line_style;
        qbinded_int_spinbox_t *m_tms_pair_line_size;
        qbinded_int_spinbox_t *m_tms_font_screen_size;
        qbinded_float_spinbox_t *m_tms_pair_delta_angle;
        qbinded_int2b_input_t *m_tms_pair_delta_offset;
        qbinded_combobox_t *m_tms_pair_label_style;
        qbinded_combobox_t *m_tms_pair_term_style;
        qbinded_int_spinbox_t *m_tms_pair_term_size;
        qbinded_line_edit_t *m_tms_pair_custom_text_edit;
        qbinded_checkbox_t *m_tms_pair_custom_text_enabled;

        QHBoxLayout *m_tms_pair_action_lt;
        QPushButton *m_tms_pair_action_sel;
        QPushButton *m_tms_pair_action_copy;
        QPushButton *m_tms_pair_action_del;

        //interatomic angle
        qspoiler_widget_t *m_tms_angle_gb;
        QFormLayout *m_tms_angle_gb_lt;
        QComboBox *m_tms_angle_cur_msr;
        QLabel *m_tms_angle_at1_info;
        QLabel *m_tms_angle_at2_info;
        QLabel *m_tms_angle_at3_info;
        qbinded_checkbox_t *m_tms_angle_enabled;
        qbinded_int_spinbox_t *m_tms_angle_order;
        //END TAB MEASUREMENT

        // start tab modify
        ws_item_tab_widget_t *m_tab_modify;

        qspoiler_widget_t *m_tm_gb_add_atom;
        QFormLayout *m_tm_gb_add_atom_lt;
        QComboBox *m_tm_add_atom_combo;
        qbinded_float3_input_t *m_tm_add_atom_vec3;
        QPushButton *m_tm_add_atom_button;

        qspoiler_widget_t *m_tm_gb_single_atom;
        QFormLayout *m_tm_gb_single_atom_lt;
        QLabel *m_tm_single_atom_info;
        QComboBox *m_tm_single_atom_combo;
        qbinded_float3_input_t *m_tm_single_atom_v3;
        QHBoxLayout *m_tm_single_atom_btn_lt;
        QPushButton *m_tm_single_atom_commit;
        QPushButton *m_tm_single_atom_delete;

        qspoiler_widget_t *m_tm_gb_pair_dist;
        QFormLayout *m_tm_gb_pair_dist_lt;
        QLabel *m_tm_pair_dist_atom1;
        QLabel *m_tm_pair_dist_atom2;
        QComboBox *m_tm_pair_dist_t_mode;
        QDoubleSpinBox *m_tm_pair_dist_spinbox;
        QLabel *m_tm_pair_dist_note_label;
        QHBoxLayout *m_tm_pair_dist_cmb_lt;
        QPushButton *m_tm_pair_dist_swap_atoms;

        qspoiler_widget_t *m_tm_gb_pair_creation;
        QFormLayout *m_tm_gb_pair_creation_lt;
        QComboBox *m_tm_pair_creation_combo;
        QHBoxLayout *m_tm_pair_creation_combo_btn_lt;
        QPushButton *m_tm_pair_creation_button;

        qspoiler_widget_t *m_tm_gb_u_scale;
        QFormLayout *m_tm_gb_u_scale_lt;
        qbinded_float3_input_t *m_tm_u_scale_v3;
        vector3<float> m_tm_u_scale_v3_val{1};
        QPushButton *m_tm_u_apply_scale_button;

        qspoiler_widget_t *m_tm_gb_translate;
        QFormLayout *m_tm_gb_translate_lt;
        qbinded_float3_input_t *m_tm_translate_v3;
        QComboBox *m_tm_translate_coord_type;
        QPushButton *m_tm_translate_apply_button;
        QHBoxLayout *m_tm_translate_combo_btn_lt;

        qspoiler_widget_t *m_tm_gb_bc_rot;
        QFormLayout *m_tm_gb_bc_rot_lt;
        QComboBox *m_tm_bc_rot_axis;
        QComboBox *m_tm_bc_rot_angle_type;
        QDoubleSpinBox *m_tm_bc_rot_angle;
        QPushButton *m_tm_bc_rot_apply;
        QHBoxLayout *m_tm_bc_rot_cmb_lt1;
        QHBoxLayout *m_tm_bc_rot_cmb_lt2;

        qspoiler_widget_t *m_tm_gb_grp_op;
        QGridLayout *m_tm_grp_op_lt;
        QPushButton *m_tm_grp_op_sv_show;
        QPushButton *m_tm_grp_op_sv_hide;
        QPushButton *m_tm_grp_op_sv_hide_invert;
        QPushButton *m_tm_grp_op_sel_ngbs;
        QPushButton *m_tm_grp_op_del_sel;
        QPushButton *m_tm_grp_make_animable;
        QPushButton *m_tm_grp_op_flip_a_p;
        QPushButton *m_tm_grp_op_flip_b_p;
        QPushButton *m_tm_grp_op_flip_c_p;
        QPushButton *m_tm_grp_op_flip_a_n;
        QPushButton *m_tm_grp_op_flip_b_n;
        QPushButton *m_tm_grp_op_flip_c_n;

        qspoiler_widget_t *m_tm_gb_override_atom;
        QFormLayout *m_tm_gb_override_atom_lt;
        qbinded_xgeom_color3_input_t *m_tm_override_atom_color;
        qbinded_xgeom_float_spinbox_t *m_tm_override_atom_radii;
        // end tab modify

        // start tab sels
        ws_item_tab_widget_t *m_tab_select;
        qspoiler_widget_t *m_ts_gb_general;
        QFormLayout *m_ts_gb_general_lt;
        QLabel *m_ts_total_grps;
        qbinded_checkbox_t *m_ts_auto_apply;
        QComboBox *m_ts_cur_sel_grp;

        qspoiler_widget_t *m_ts_gb_sel_grp_details;
        QFormLayout *m_ts_gb_sel_grp_details_lt;
        // end tab sels

        // start tab xgeom
        ws_item_tab_widget_t *m_tab_xgeom;
        QButtonGroup *m_txg_switch;
        qspoiler_widget_t *m_txg_gb_info;
        QVBoxLayout *m_txg_gb_info_lt;
        QTableView *m_txg_info_tv;
        qxgeom_fields_model_t *m_txg_info_tmdl;

        qspoiler_widget_t *m_txg_gb_comp_prop;
        QVBoxLayout *m_txg_gb_comp_prop_lt;

        // end tab xgeom

        void construct_general_tab();
        void construct_disp_tab();
        void construct_anim_tab();
        void construct_msr_tab();
        void construct_mod_tab();
        void construct_sel_tab();
        void construct_xgeom_tab();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;

        void unbind_item() override;

        void bind_dist_msr_tab();
        void unbind_dist_msr_tab();
        void bind_angle_msr_tab();
        void unbind_angle_msr_tab();

        void bind_sel_tab();
        void unbind_sel_tab();

        void update_anim_tab();
        void update_anim_tab_visibility();
        //
        void update_mod_tab();
        void update_msr_tab();
        void update_dist_msr_tab_info();
        void update_angle_msr_tab_info();
        void update_sel_tab();
        void update_xgeom_tab();
        void fill_combo_with_atom_types(QComboBox *combo, geom_view_t *_al);

        void tab_modify_flip_cell_clicked(size_t flip_dim, float flip_magn = 1);

        void resizeEvent(QResizeEvent *event) override;

        geom_view_obj_insp_widget_t();

      public slots:

        void cur_anim_index_changed(int index);
        void play_anim_button_toggle(bool value); //TODO: refc toggle -> toggled
        void anim_updated_external();
        void cur_ws_sel_item_frame_changed();

        void anim_timeline_slider_value_changed(int value);
        void anim_button_begin_clicked();
        void anim_button_end_clicked();
        void anim_button_frame_move_forward_clicked();
        void anim_button_frame_move_backward_clicked();
        void anim_act_ren_clicked();
        void anim_act_del_clicked();

        void disp_switch_cur_changed(int index);
        void cell_changed();
        void draw_subcells_changed(int state);
        void update_anim_section_status();
        void cur_it_list_sel_changed();

        void mod_add_atom_button_clicked();
        void mod_single_atom_button_clicked();
        void mod_single_atom_delete_button_clicked();
        void mod_pair_dist_spinbox_value_changed(double newval);
        void mod_pair_dist_swap_button_clicked();
        void mod_add_atom_between_pair();
        void mod_barycentric_scale_button_clicked();
        void mod_translate_selected_atoms_clicked();
        void mod_translate_coord_type_changed(int coord_type);
        void mod_bc_rot_angle_type_change(int new_angle_type);
        void mod_bc_rot_apply();

        void mod_group_op_sv_show();
        void mod_group_op_sv_hide();
        void mod_group_op_sv_hide_invert();
        void mod_group_op_sel_ngbs();
        void mod_group_op_del_sel();
        void mod_group_op_make_static_anim();

        void msr_switch_current_changed(int index);
        void msr_pair_cur_idx_changed(int index);
        void msr_angle_cur_idx_changed(int index);
        void msr_pair_select_clicked();
        void msr_pair_delete_clicked();
        void msr_pair_copy_clicked();

        void xgeom_switch_current_changed(int index);

        void cur_ws_edit_mode_changed();
        void cur_it_selected_content_changed();

        void type_summary_clicked(const QModelIndex &index);
        void rebond_button_clicked();
        void clear_color_override_button_clicked();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
