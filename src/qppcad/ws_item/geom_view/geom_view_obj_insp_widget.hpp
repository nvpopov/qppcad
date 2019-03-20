#ifndef QPP_CAD_WS_ATOMS_LIST_OBJ_INSP_H
#define QPP_CAD_WS_ATOMS_LIST_OBJ_INSP_H

#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/qbonding_table_model.hpp>
#include <qppcad/ws_item/geom_view/qtype_specific_rendering_model.hpp>
#include <qppcad/ws_item/geom_view/qmeasurements_table_model.hpp>
#include <qppcad/ui/qrealspinbox_delegate.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>

#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include <QSlider>

namespace qpp {

  namespace cad {

    class geom_view_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        geom_view_t *b_al{nullptr}; // binded atoms list

        qspoiler_widget_t *tg_geom_summary_widget;
        QFormLayout *tg_geom_summary_lt;

        QLabel *tg_geom_summary_total_atoms;
        QLabel *tg_geom_summary_total_types;

        qspoiler_widget_t *tg_type_summary_widget;
        QVBoxLayout *tg_type_summary_lt;
        QTableWidget *tg_type_summary_tbl;

        qspoiler_widget_t *tg_gb_cell;
        QVBoxLayout *tg_gb_cell_lt;
        QTableWidget *tg_gb_cell_tbl;

        //START: Display tab
        ws_item_tab_widget_t *tab_disp;
        qspoiler_widget_t *gb_disp_s;
        QFormLayout *gb_disp_s_lt;
        qbinded_checkbox *disp_s_draw_atoms;
        qbinded_checkbox *disp_s_draw_bonds;
        qbinded_checkbox *disp_s_draw_img_atoms;
        qbinded_checkbox *disp_s_draw_img_bonds;
        qbinded_float_spinbox *disp_s_atom_scale;
        qbinded_float_spinbox *disp_s_bond_scale;
        qbinded_combobox *disp_s_render_style;
        qbinded_combobox *disp_s_color_mode;

        qspoiler_widget_t *gb_disp_labels;
        QFormLayout *gb_disp_labels_lt;
        qbinded_combobox *disp_labels_style;
        qbinded_checkbox *disp_inplace_labels;
        qbinded_int_spinbox *disp_labels_size;
        qbinded_checkbox *disp_sl_labels;
        qbinded_checkbox *disp_labels_screen_scale;
        qbinded_checkbox *disp_labels_draw_outline;

        qbinded_checkbox *disp_s_sel_vis;
        qbinded_checkbox *disp_s_sel_vis_affect_bonds;

        QLabel *disp_draw_cell_label;
        qbinded_checkbox *disp_draw_cell;
        QLabel *disp_cell_color_label;
        qbinded_color3_input *disp_cell_color;

        QLabel *disp_draw_subcells_lbl;
        qbinded_checkbox *disp_draw_subcells;
        QLabel *disp_subcells_idx_lbl;
        qbinded_int3_input *disp_subcells_idx;
        QLabel *disp_subcells_color_lbl;
        qbinded_color3_input *disp_subcells_clr;

        qspoiler_widget_t *gb_disp_shading;
        QFormLayout *gb_disp_shading_lt;
        qbinded_checkbox *disp_shading_spec;
        qbinded_float_spinbox *disp_shading_spec_value;

        qspoiler_widget_t *gb_disp_type_spec_rend;
        QTableView *disp_type_spec_tv;
        qtype_specific_rendering_model_t *disp_type_spec_mdl;
        QVBoxLayout *gb_disp_type_spec_rend_lt;

        qspoiler_widget_t *gb_disp_bt;
        QVBoxLayout *disp_bt_lt;
        QTableView *disp_bt;
        qbonding_table_model_t *bt_mdl;
        qrealspinbox_delegate_t *bt_dist_delegate;
        //END: Display tab

        ws_item_tab_widget_t *tab_anim;
        qspoiler_widget_t *gb_anim_summary;
        QLabel *gb_anim_total_anims;
        qbinded_checkbox *gb_rebuild_bonds;
        qbinded_checkbox *gb_play_cyclic;
        qbinded_float_spinbox *gb_anim_speed;
        QComboBox *gb_current_anim;
        QFormLayout *gb_anim_summary_lt;

        qspoiler_widget_t *gb_anim_timeline;
        QHBoxLayout *gb_anim_timeline_lt;
        QLabel *gb_anim_total_frames_in_anim;
        QLabel *gb_anim_cur_frame;
        QSlider *gb_anim_timeline_slider;
        qspoiler_widget_t *gb_anim_buttons;
        QHBoxLayout *gb_anim_buttons_lt;

        QPushButton *anim_play;
        QPushButton *anim_to_start;
        QPushButton *anim_to_end;
        QPushButton *anim_frame_forward;
        QPushButton *anim_frame_backward;

        //START TAB MEASUREMENT
        ws_item_tab_widget_t *tab_measurement;

        //common measurements settings
        qspoiler_widget_t *tms_common_settings_gb;
        QFormLayout *tms_common_settings_gb_lt;
        qbinded_checkbox *tms_render_dist;
        qbinded_checkbox *tms_render_angle;

        //pair dist
        qspoiler_widget_t *tms_pair_dist_gb;
        QFormLayout *tms_pair_dist_gb_lt;
        QComboBox *tms_pair_cur_msr;
        QLabel *tms_pair_at1_info;
        QLabel *tms_pair_at2_info;
        QLabel *tms_pair_dist;
        qbinded_color3_input *tms_pair_dist_color;
        qbinded_checkbox *tms_pair_enabled;
        qbinded_checkbox *tms_pair_label_enabled;
        qbinded_combobox *tms_pair_line_style;
        qbinded_int_spinbox *tms_pair_line_size;
        qbinded_int_spinbox *tms_font_screen_size;
        qbinded_combobox *tms_pair_label_style;

        //interatomic angle
        qspoiler_widget_t *tms_angle_gb;
        QFormLayout *tms_angle_gb_lt;
        QComboBox *tms_angle_cur_msr;
        QLabel *tms_angle_at1_info;
        QLabel *tms_angle_at2_info;
        QLabel *tms_angle_at3_info;
        qbinded_checkbox *tms_angle_enabled;
        qbinded_int_spinbox *tms_angle_order;
        //END TAB MEASUREMENT

        //START TAB MODIFY
        ws_item_tab_widget_t *tab_modify;

        qspoiler_widget_t *tm_gb_add_atom;
        QFormLayout *tm_gb_add_atom_lt;
        QComboBox *tm_add_atom_combo;
        qbinded_float3_input *tm_add_atom_vec3;
        QPushButton *tm_add_atom_button;

        qspoiler_widget_t *tm_gb_single_atom;
        QFormLayout *tm_gb_single_atom_lt;
        QLabel *tm_single_atom_idx;
        QLabel *tm_single_atom_num;
        QComboBox *tm_single_atom_combo;
        qbinded_float3_input *tm_single_atom_vec3;
        QPushButton *tm_single_atom_commit;
        QPushButton *tm_single_atom_delete;

        qspoiler_widget_t *tm_gb_pair_dist;
        QFormLayout *tm_gb_pair_dist_lt;
        QLabel *tm_pair_dist_atom1;
        QLabel *tm_pair_dist_atom2;
        QLabel *tm_pair_dist_atom1_idx;
        QLabel *tm_pair_dist_atom2_idx;
        QComboBox *tm_pair_dist_t_mode;
        QDoubleSpinBox *tm_pair_dist_spinbox;
        QLabel *tm_pair_dist_note_label;

        qspoiler_widget_t *tm_gb_pair_creation;
        QFormLayout *tm_gb_pair_creation_lt;
        QComboBox *tm_pair_creation_combo;
        QPushButton *tm_pair_creation_button;

        qspoiler_widget_t *tm_gb_u_scale;
        QFormLayout *tm_gb_u_scale_lt;
        QDoubleSpinBox *tm_u_scale_sb_x;
        QDoubleSpinBox *tm_u_scale_sb_y;
        QDoubleSpinBox *tm_u_scale_sb_z;
        QCheckBox *tm_u_scale_x_enabled;
        QCheckBox *tm_u_scale_y_enabled;
        QCheckBox *tm_u_scale_z_enabled;
        QPushButton *tm_u_apply_scale_button;

        qspoiler_widget_t *tm_gb_translate;
        QFormLayout *tm_gb_translate_lt;
        qbinded_float3_input *tm_translate_vec3;
        QLabel *tm_translate_coord_type_label;
        QComboBox *tm_translate_coord_type;
        QPushButton *tm_translate_apply_button;

        qspoiler_widget_t *tm_gb_bc_rot;
        QFormLayout *tm_gb_bc_rot_lt;
        QComboBox *tm_bc_rot_axis;
        QComboBox *tm_bc_rot_angle_type;
        QDoubleSpinBox *tm_bc_rot_angle;
        QPushButton *tm_bc_rot_apply;

        qspoiler_widget_t *tm_gb_group_op;
        QGridLayout *tm_group_op_lt;
        QPushButton *tm_group_op_sv_show;
        QPushButton *tm_group_op_sv_hide;
        QPushButton *tm_group_op_sv_show_all;
        QPushButton *tm_group_op_sel_ngbs;
        QPushButton *tm_group_op_del_sel;

        qspoiler_widget_t *tm_gb_override_atom;
        QFormLayout *tm_gb_override_atom_lt;
        QLabel *tm_override_atom_info;
        qbinded_xgeom_color3_input *tm_override_atom_color;
        qbinded_xgeom_float_spinbox *tm_override_atom_radii;
        //END TAB MODIFY

        //START TAB SELECTIONS
        ws_item_tab_widget_t *tab_select;
        //END TAB SELECTIONS

        void construct_general_tab();
        void construct_display_tab();
        void construct_anim_tab();
        void construct_measure_tab();
        void construct_modify_tab();
        void construct_select_tab();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;

        void unbind_item() override;

        void bind_dist_measure_tab();
        void unbind_dist_measure_tab();
        void bind_angle_measure_tab();
        void unbind_angle_measure_tab();

        void update_anim_tab();
        void update_anim_tab_visibility();
       //
        void update_modify_tab();
        void update_measurement_tab();
        void update_dist_measurement_tab_info();
        void update_angle_measurement_tab_info();
        void update_select_tab();
        void fill_combo_with_atom_types(QComboBox *combo, geom_view_t *_al);
        geom_view_obj_insp_widget_t();

      public slots:

        void cur_anim_index_changed(int index);
        void play_anim_button_toggle(bool value);
        void anim_updated_external();
        void cur_ws_sel_item_frame_changed();

        void anim_timeline_slider_value_changed(int value);
        void anim_button_begin_clicked();
        void anim_button_end_clicked();
        void anim_button_frame_move_forward_clicked();
        void anim_button_frame_move_backward_clicked();

        void cell_changed();
        void draw_subcells_changed(int state);
        void update_animate_section_status();
        void cur_it_list_selection_changed();

        void modify_add_atom_button_clicked();
        void modify_single_atom_button_clicked();
        void modify_single_atom_delete_button_clicked();
        void modify_pair_dist_spinbox_value_changed(double newval);
        void modify_add_atom_between_pair();
        void modify_barycentric_scale_button_clicked();
        void modify_translate_selected_atoms_clicked();
        void modify_translate_coord_type_changed(int coord_type);
        void modify_bc_rot_angle_type_change(int new_angle_type);
        void modify_bc_rot_apply();

        void modify_group_op_sv_show();
        void modify_group_op_sv_hide();
        void modify_group_op_sv_show_all();
        void modify_group_op_sel_ngbs();
        void modify_group_op_del_sel();

        void msr_pair_cur_idx_changed(int index);
        void msr_angle_cur_idx_changed(int index);

        void cur_ws_edit_mode_changed();
        void cur_it_selected_content_changed();

        void type_summary_clicked(const QModelIndex &index);

    };

  }

}
#endif
