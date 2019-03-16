#ifndef QPP_CAD_TOOL_COLORIZE_BY_XFIELD
#define QPP_CAD_TOOL_COLORIZE_BY_XFIELD

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ui/qbinded_inputs.hpp>
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

    class colorize_by_xfield_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item) override;

    };

    class colorize_by_xfield_widget_t : public QDialog {

        Q_OBJECT

      public:

        QVBoxLayout *main_lt;

        QGroupBox *main_gb;
        QFormLayout *main_gb_lt;

        QHBoxLayout *buttons_lt;
        QPushButton *button_apply;
        QPushButton *button_cancel;

        QComboBox *cmb_xfield_name;

        qbinded_color3_input *clr_low_input;
        qbinded_color3_input *clr_high_input;

        vector3<float> clr_low{0};
        vector3<float> clr_high{1};

        geom_view_t *b_gv{nullptr};
        std::vector<STRING_EX> b_fn;
        std::vector<basic_types> b_ft;

        void build_xfield();
        colorize_by_xfield_widget_t();

    };

  }

}

#endif
