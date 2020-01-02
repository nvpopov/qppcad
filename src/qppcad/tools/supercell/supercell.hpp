#ifndef QPPCAD_TOOL_SUPERCELL
#define QPPCAD_TOOL_SUPERCELL
#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>
#include <qppcad/ui/ws_item_inline_tool_widget.hpp>
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

    class super_cell_widget_t : public ws_item_inline_tool_widget_t {

        Q_OBJECT

      private:

      public:

        QSpinBox *m_sp_rep_a;
        QSpinBox *m_sp_rep_b;
        QSpinBox *m_sp_rep_c;

        qspoiler_widget_t *m_gb_rep_par;
        QFormLayout *m_gb_rep_par_lt;

        QVBoxLayout *m_dialog_lt;

        int get_replication_coeff(int dim_num);
        super_cell_widget_t(QWidget *parent = nullptr);

        void on_apply() override;
        void on_cancel() override;

    };

    class supercell_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item, uint32_t _error_ctx) override;
        void make_super_cell(geom_view_t *al,
                             const int a_steps,
                             const int b_steps,
                             const int c_steps);

        ws_item_inline_tool_widget_t *construct_inline_tool() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
