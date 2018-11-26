#ifndef QPP_CAD_WS_ATOMS_LIST_OBJ_INSP_H
#define QPP_CAD_WS_ATOMS_LIST_OBJ_INSP_H

#include <qppcad/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>

namespace qpp {

  namespace cad {

    class ws_atoms_list_obj_insp_widget_t : public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        ws_atoms_list_t *m_binded_atoms_list;

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

        ws_item_tab_widget_t *tab_animation;
        ws_item_tab_widget_t *tab_measurement;

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
        ws_atoms_list_obj_insp_widget_t();
    };

  }

}
#endif
