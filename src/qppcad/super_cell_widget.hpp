#ifndef QPP_CAD_SUPER_CELL_WIDGET
#define QPP_CAD_SUPER_CELL_WIDGET

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>

namespace qpp {

  namespace cad {

    class super_cell_widget_t : public QDialog {

        Q_OBJECT

      private:

      public:
        QSpinBox *sp_rep_a;
        QSpinBox *sp_rep_b;
        QSpinBox *sp_rep_c;

        QGroupBox *gb_rep_par;
        QFormLayout *gb_rep_par_layout;

        QVBoxLayout *dialog_layout;

        QDialogButtonBox *dialog_bb;

        int get_replication_coeff(int dim_num);
        super_cell_widget_t();
    };

  }

}

#endif
