#ifndef QPPCAD_PTABLE_RICH_WIDGET_H
#define QPPCAD_PTABLE_RICH_WIDGET_H

#include <qppcad/core/qppcad.hpp>
#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <qppcad/ui/qspoiler_widget.hpp>
#include <qppcad/ui/qbinded_inputs.hpp>

namespace qpp {

  namespace cad {

    class ptable_rich_widget_t : public QDialog {

        Q_OBJECT

      public:

        QHBoxLayout *m_main_lt;
        QGridLayout *m_elem_lt;
        QButtonGroup *m_btn_grp;

        explicit ptable_rich_widget_t(QWidget *parent = nullptr);
        void construct_widgets();

        void colorize_element_btn(int btn_id);
        std::optional<std::tuple<int, int> > get_ptable_x_y(size_t elem_id);


      public slots:

        void btn_grp_clicked(int id);

    };

    class ptable_element_editor_t : public QDialog {

        Q_OBJECT

      public:

        QVBoxLayout *m_main_lt;
        qspoiler_widget_t *m_main_splr;
        QDialogButtonBox *m_dialog_bb;

        QFormLayout *m_splr_lt;
        qbinded_float_spinbox_t *m_radius;
        qbinded_float_spinbox_t *m_cov_rad;
        qbinded_float_spinbox_t *m_ionic_rad;
        qbinded_color3_input_t *m_color;

        float m_binded_radius;
        float m_binded_cov_rad;
        float m_binded_ionic_rad;
        vector3<float> m_binded_color;
        int m_elem_id{-1};

        explicit ptable_element_editor_t(int elem_id, QWidget *parent = nullptr);

        void update_element_data();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
