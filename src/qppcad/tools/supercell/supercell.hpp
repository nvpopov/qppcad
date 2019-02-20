#ifndef QPP_CAD_TOOL_SUPERCELL
#define QPP_CAD_TOOL_SUPERCELL
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>
#include <qppcad/geom_view/geom_view.hpp>
#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QApplication>

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

    class supercell_tool_t : public ws_item_tool_t {
      public:
        void exec(ws_item_t *item) override;
        void make_super_cell(geom_view_t *al,
                             const int a_steps,
                             const int b_steps,
                             const int c_steps);
    };
  }

}

#endif
