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
        ws_atoms_list_t *b_al; // binded atoms list

        QGroupBox *tg_geom_summary_widget;
        QFormLayout *tg_geom_summary_layout;

        QLabel *tg_geom_summary_total_atoms;
        QLabel *tg_geom_summary_total_types;

        QGroupBox *tg_type_summary_widget;
        QVBoxLayout *tg_type_summary_layout;
        QTableWidget *tg_type_summary_table;

        ws_item_tab_widget_t *tab_display;
        QGroupBox *gb_display_settings;
        QFormLayout *gb_display_settings_layout;
        qbinded_checkbox *display_settings_draw_atoms;
        qbinded_checkbox *display_settings_draw_bonds;
        qbinded_checkbox *display_settings_draw_img_atoms;
        qbinded_checkbox *display_settings_draw_img_bonds;
        qbinded_float_spinbox *display_settings_atom_scale;
        qbinded_float_spinbox *display_settings_bond_scale;
        qbinded_combobox *display_settings_render_style;
        qbinded_combobox *display_settings_labels_style;

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

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
        void update_anim_tab_gb();
        ws_atoms_list_obj_insp_widget_t();

      public slots:
        void current_anim_index_changed(int index);
        void play_anim_button_toggle(bool value);
        void animation_updated_external();
        void current_workspace_selected_item_frame_changed();
        void anim_timeline_slider_value_changed(int value);
        void anim_button_begin_clicked();
        void anim_button_end_clicked();
        void anim_button_frame_move_forward_clicked();
        void anim_button_frame_move_backward_clicked();
    };

  }

}
#endif
