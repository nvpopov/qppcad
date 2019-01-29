
#ifndef QPP_CAD_TOOL_AXIAL_SCALE
#define QPP_CAD_TOOL_AXIAL_SCALE
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>
#include <QWidget>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>

namespace qpp {

  namespace cad {

    class axial_scale_tool_t : public ws_item_tool_t {
      public:
        void exec(ws_item_t *item) override;
    };

    class axial_scale_widget_t : public QDialog {

        Q_OBJECT

      public:
        QDoubleSpinBox *sb_sc_a;
        QDoubleSpinBox *sb_sc_b;
        QDoubleSpinBox *sb_sc_c;
        QGroupBox *gb_sc_par;
        QFormLayout *gb_sc_par_layout;
        QVBoxLayout *dialog_layout;
        QDialogButtonBox *dialog_bb;
        double get_scale_value(int dim_id);
        axial_scale_widget_t();

    };

  }

}

#endif
