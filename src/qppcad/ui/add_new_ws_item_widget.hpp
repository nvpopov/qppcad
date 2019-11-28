#ifndef QPPCAD_ADD_NEW_WS_ITEM_WIDGET
#define QPPCAD_ADD_NEW_WS_ITEM_WIDGET

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ui/qbinded_inputs.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>

#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QDoubleSpinBox>

namespace qpp {

  namespace cad {

    enum cell_construct_mode : int {
      construct_cubic = 0,
      construct_ortho = 1,
      construct_3angles_3len = 2,
      construct_9el = 3
    };

    class add_new_ws_item_widget_t : public QDialog {

      public:

        vector3<float> m_ortho_g3d_cell{5, 5, 5};
        vector3<float> m_a3_g3d_cell{90, 90, 90};
        vector3<float> m_l3_g3d_cell{5, 5, 5};

        vector3<float> m_a_g3d_cell{5, 0, 0};
        vector3<float> m_b_g3d_cell{0, 5, 0};
        vector3<float> m_c_g3d_cell{0, 0, 5};

        QVBoxLayout *main_lt;
        QHBoxLayout *data_lt;
        QVBoxLayout *sub_data_lt;
        QHBoxLayout *button_lt;
        QPushButton *ok_button;
        QPushButton *cancel_button;

        qspoiler_widget_t *gb_ctor{};
        QVBoxLayout *gb_ctor_lt{};
        QRadioButton *rb_ctor_geom0d{};
        QRadioButton *rb_ctor_geom1d{};
        QRadioButton *rb_ctor_geom2d{};
        QRadioButton *rb_ctor_geom3d{};
        QRadioButton *rb_ctor_psg{};
        QRadioButton *rb_ctor_pgf_prod{};
        QRadioButton *rb_ctor_cube{};
        QRadioButton *rb_ctor_arrow{};
        QRadioButton *rb_ctor_node_book{};
        QRadioButton *rb_ctor_arrow_array{};
        QRadioButton *rb_ctor_py_note_book{};
        QRadioButton *rb_ctor_traj_hl{};

        qspoiler_widget_t *gb_type_descr{};
        QVBoxLayout *gb_type_descr_lt{};
        QLabel *type_descr_lbl{};

        QComboBox *type_cell_ctor_mode{};
        QLabel *type_cell_ctor_mode_lbl{};

        QDoubleSpinBox *type_cell_ctor_cubic_size{};
        QLabel *type_cell_ctor_cubic_size_lbl{};

        qbinded_float3_input_t *type_cell_ctor_ortho{};
        QLabel *type_cell_ctor_ortho_lbl{};

        qbinded_float3_input_t *type_cell_ctor_a3{};
        QLabel *type_cell_ctor_a3_lbl{};
        qbinded_float3_input_t *type_cell_ctor_l3{};
        QLabel *type_cell_ctor_l3_lbl{};

        qbinded_float3_input_t *type_cell_ctor_a{};
        QLabel *type_cell_ctor_a_lbl{};
        qbinded_float3_input_t *type_cell_ctor_b{};
        QLabel *type_cell_ctor_b_lbl{};
        qbinded_float3_input_t *type_cell_ctor_c{};
        QLabel *type_cell_ctor_c_lbl{};

        qspoiler_widget_t *gb_type_param{};
        QFormLayout *gb_type_param_lt{};

        QLabel *type_param_name_lbl{};
        QLineEdit *type_param_name{};
        QLabel *type_param_ag_lbl{};
        QComboBox *type_param_ag{};

        QStringList descr_list;

        add_new_ws_item_widget_t();

        void set_cell_ctors_visibility(bool show);
        void control_top_type_parameters_visibility();

      public slots:

        void ok_button_clicked();
        void cancel_button_clicked();

        void react_gb_ctor_geom0d_checked(bool checked);
        void react_gb_ctor_geom1d_checked(bool checked);
        void react_gb_ctor_geom2d_checked(bool checked);
        void react_gb_ctor_geom3d_checked(bool checked);
        void react_gb_ctor_psg_checked(bool checked);
        void react_gb_ctor_pgf_prod_checked(bool checked);
        void react_gb_ctor_cube_checked(bool checked);
        void react_gb_ctor_arrow_checked(bool checked);
        void react_gb_ctor_node_book_checked(bool checked);
        void react_gb_ctor_arrow_array_checked(bool checked);
        void react_gb_ctor_py_note_book_checked(bool checked);
        void react_gb_ctor_traj_hl_checked(bool checked);
        void geom3d_ctor_changed(int index);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
