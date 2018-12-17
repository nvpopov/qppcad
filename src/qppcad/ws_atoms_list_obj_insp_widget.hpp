#ifndef QPP_CAD_WS_ATOMS_LIST_OBJ_INSP_H
#define QPP_CAD_WS_ATOMS_LIST_OBJ_INSP_H

#include <qppcad/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/qbonding_table_model.hpp>
#include <qppcad/qrealspinbox_delegate.hpp>
#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include <QSlider>

namespace qpp {

  namespace cad {

    class ws_atoms_list_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        ws_atoms_list_t *b_al{nullptr}; // binded atoms list

        QGroupBox *tg_geom_summary_widget;
        QFormLayout *tg_geom_summary_layout;

        QLabel *tg_geom_summary_total_atoms;
        QLabel *tg_geom_summary_total_types;

        QGroupBox *tg_type_summary_widget;
        QVBoxLayout *tg_type_summary_layout;
        QTableWidget *tg_type_summary_table;

        QGroupBox *tg_gb_cell;
        QVBoxLayout *tg_gb_cell_layout;
        QTableWidget *tg_gb_cell_table;

        ws_item_tab_widget_t *tab_display;
        QGroupBox *gb_disp_s;
        QFormLayout *gb_disp_s_layout;
        qbinded_checkbox *disp_s_draw_atoms;
        qbinded_checkbox *disp_s_draw_bonds;
        qbinded_checkbox *disp_s_draw_img_atoms;
        qbinded_checkbox *disp_s_draw_img_bonds;
        qbinded_float_spinbox *disp_s_atom_scale;
        qbinded_float_spinbox *disp_s_bond_scale;
        qbinded_combobox *disp_s_render_style;
        qbinded_combobox *disp_s_labels_style;
        qbinded_checkbox *disp_s_sel_vis;
        qbinded_checkbox *disp_s_sel_vis_affect_bonds;

        QLabel *disp_s_draw_subcells_label;
        QLabel *disp_s_subcells_idx_label;
        qbinded_checkbox *disp_s_draw_subcells;
        qbinded_int3_input *disp_s_subcells_idx;

        QGroupBox *gb_display_shading;
        QFormLayout *gb_display_shading_layout;
        qbinded_checkbox *display_shading_spec;
        qbinded_float_spinbox *display_shading_spec_value;

        QGroupBox *gb_display_bt;
        QVBoxLayout *display_bt_layout;
        QTableView *display_bt;
        qbonding_table_model_t *bt_model;
        qrealspinbox_delegate_t *bt_dist_delegate;

        ws_item_tab_widget_t *tab_animation;
        QGroupBox *gb_anim_summary;
        QLabel *gb_anim_total_anims;
        qbinded_checkbox *gb_rebuild_bonds;
        qbinded_checkbox *gb_play_cyclic;
        qbinded_float_spinbox *gb_anim_speed;
        QComboBox *gb_current_anim;
        //QLabel *gb_current_anim_type;
        QFormLayout *gb_anim_summary_layout;

        QGroupBox *gb_anim_timeline;
        QHBoxLayout *gb_anim_timeline_layout;
        QLabel *gb_anim_total_frames_in_anim;
        QLabel *gb_anim_cur_frame;
        QSlider *gb_anim_timeline_slider;
        QGroupBox *gb_anim_buttons;
        QHBoxLayout *gb_anim_buttons_layout;

        QPushButton *anim_play;
        QPushButton *anim_to_start;
        QPushButton *anim_to_end;
        QPushButton *anim_frame_forward;
        QPushButton *anim_frame_backward;

        ws_item_tab_widget_t *tab_measurement;
        QGroupBox *tmeasure_pair_dist_gb;
        QVBoxLayout *tmeasure_pair_dist_gb_layout;
        QTableView *tmeasure_pair_dist_table;

        ws_item_tab_widget_t *tab_modify;

        QGroupBox *tm_gb_add_atom;
        QFormLayout *tm_gb_add_atom_layout;
        QComboBox *tm_add_atom_combo;
        qbinded_float3_input *tm_add_atom_vec3;
        QPushButton *tm_add_atom_button;

        QGroupBox *tm_gb_single_atom;
        QFormLayout *tm_gb_single_atom_layout;
        QLabel *tm_single_atom_idx;
        QComboBox *tm_single_atom_combo;
        qbinded_float3_input *tm_single_atom_vec3;
        QPushButton *tm_single_atom_commit;
        QPushButton *tm_single_atom_delete;

        QGroupBox *tm_gb_pair_dist;
        QFormLayout *tm_gb_pair_dist_layout;
        QLabel *tm_pair_dist_atom1;
        QLabel *tm_pair_dist_atom2;
        QLabel *tm_pair_dist_atom1_idx;
        QLabel *tm_pair_dist_atom2_idx;
        QComboBox *tm_pair_dist_t_mode;
        QDoubleSpinBox *tm_pair_dist_spinbox;
        QLabel *tm_pair_dist_note_label;

        QGroupBox *tm_gb_pair_creation;
        QFormLayout *tm_gb_pair_creation_layout;
        QComboBox *tm_pair_creation_combo;
        QPushButton *tm_pair_creation_button;

        QGroupBox *tm_gb_u_scale;
        QFormLayout *tm_gb_u_scale_layout;
        QDoubleSpinBox *tm_u_scale_sb_x;
        QDoubleSpinBox *tm_u_scale_sb_y;
        QDoubleSpinBox *tm_u_scale_sb_z;
        QCheckBox *tm_u_scale_x_enabled;
        QCheckBox *tm_u_scale_y_enabled;
        QCheckBox *tm_u_scale_z_enabled;
        QPushButton *tm_u_apply_scale_button;

        QGroupBox *tm_gb_translate;
        QFormLayout *tm_gb_translate_layout;
        qbinded_float3_input *tm_translate_vec3;
        QLabel *tm_translate_coord_type_label;
        QComboBox *tm_translate_coord_type;
        QPushButton *tm_translate_apply_button;

        void construct_general_tab();
        void construct_display_tab();
        void construct_anim_tab();
        void construct_measure_tab();
        void construct_modify_tab();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
        void update_anim_tab_gb();
        void update_modify_tab();
        void fill_combo_with_atom_types(QComboBox *combo, ws_atoms_list_t *_al);
        ws_atoms_list_obj_insp_widget_t();

      public slots:
        void cur_anim_index_changed(int index);
        void play_anim_button_toggle(bool value);
        void anim_updated_external();
        void cur_ws_selected_item_frame_changed();
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
        void cur_ws_edit_mode_changed();
        void cur_it_selected_content_changed();

    };

  }

}
#endif
