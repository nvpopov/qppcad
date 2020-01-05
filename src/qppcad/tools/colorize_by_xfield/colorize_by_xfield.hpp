#ifndef QPPCAD_TOOL_COLORIZE_BY_XFIELD
#define QPPCAD_TOOL_COLORIZE_BY_XFIELD

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ui/qbinded_inputs.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>
#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QApplication>

namespace qpp {

  namespace cad {

    class colorize_by_xfield_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item, uint32_t _error_ctx) override;

    };

    class colorize_by_xfield_widget_t : public QDialog {

        Q_OBJECT

      public:

        QVBoxLayout *m_main_lt;

        qspoiler_widget_t *m_main_gb;
        QFormLayout *m_main_gb_lt;

        QHBoxLayout *m_buttons_lt;
        QPushButton *m_button_apply;
        QPushButton *m_button_cancel;

        QComboBox *m_cmb_xfield_name;

        qbinded_color3_input_t *m_clr_low_input;
        qbinded_color3_input_t *m_clr_high_input;

        vector3<float> m_clr_low{0};
        vector3<float> m_clr_high{1};

        geom_view_t *b_gv{nullptr};
        std::vector<STRING_EX> b_fn;
        std::vector<basic_types> b_ft;

        void build_xfield();
        colorize_by_xfield_widget_t();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
