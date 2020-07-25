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

    enum class cell_construct_mode : int {
      construct_cubic = 0,
      construct_ortho = 1,
      construct_3angles_3len = 2,
      construct_9el = 3
    };

    class add_new_ws_item_widget_t : public QDialog {

      public:

        hs_prop_v3f_t m_ortho_g3d_cell;
        hs_prop_v3f_t m_a3_g3d_cell;
        hs_prop_v3f_t m_l3_g3d_cell;

        hs_prop_v3f_t m_a_g3d_cell;
        hs_prop_v3f_t m_b_g3d_cell;
        hs_prop_v3f_t m_c_g3d_cell;

        QVBoxLayout *m_main_lt;
        QHBoxLayout *m_data_lt;
        QVBoxLayout *m_sub_data_lt;
        QHBoxLayout *m_btn_lt;
        QPushButton *m_ok_btn;
        QPushButton *m_cancel_btn;
        qspoiler_widget_t *m_gb_ctor{};

        QVBoxLayout  *m_gb_ctor_lt{};
        QRadioButton *m_rb_ctor_geom0d{};
        QRadioButton *m_rb_ctor_geom1d{};
        QRadioButton *m_rb_ctor_geom2d{};
        QRadioButton *m_rb_ctor_geom3d{};
        QRadioButton *m_rb_ctor_psg{};
        QRadioButton *m_rb_ctor_pgf_prod{};
        QRadioButton *m_rb_ctor_cube{};
        QRadioButton *m_rb_ctor_arrow{};
        QRadioButton *m_rb_ctor_node_book{};
        QRadioButton *m_rb_ctor_arrow_array{};
        QRadioButton *m_rb_ctor_py_note_book{};
        QRadioButton *m_rb_ctor_traj_hl{};

        qspoiler_widget_t *m_gb_type_descr{};
        QVBoxLayout *m_gb_type_descr_lt{};
        QLabel *m_type_descr_lbl{};

        QComboBox *m_type_cell_ctor_mode{};
        QLabel *m_type_cell_ctor_mode_lbl{};

        QDoubleSpinBox *m_type_cell_ctor_cubic_size{};
        QLabel *m_type_cell_ctor_cubic_size_lbl{};

        qbinded_float3_input_t *m_type_cell_ctor_ortho{};
        QLabel *m_type_cell_ctor_ortho_lbl{};

        qbinded_float3_input_t *m_type_cell_ctor_a3{};
        QLabel *m_type_cell_ctor_a3_lbl{};
        qbinded_float3_input_t *m_type_cell_ctor_l3{};
        QLabel *m_type_cell_ctor_l3_lbl{};

        qbinded_float3_input_t *m_type_cell_ctor_a{};
        QLabel *m_type_cell_ctor_a_lbl{};
        qbinded_float3_input_t *m_type_cell_ctor_b{};
        QLabel *m_type_cell_ctor_b_lbl{};
        qbinded_float3_input_t *m_type_cell_ctor_c{};
        QLabel *m_type_cell_ctor_c_lbl{};

        qspoiler_widget_t *m_gb_type_param{};
        QFormLayout *m_gb_type_param_lt{};

        QLabel *m_type_param_name_lbl{};
        QLineEdit *m_type_param_name{};
        QLabel *m_type_param_ag_lbl{};
        QComboBox *m_type_param_ag{};

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
