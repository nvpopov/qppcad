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
#include <qppcad/core/iupdatable.hpp>

namespace qpp {

  namespace cad {

    struct super_cell_can_apply_helper_t {

      static bool can_apply(ws_item_t *item);

    };

    enum supercell_tool_mode_e {

      sc_tool_mode_default,
      sc_tool_mode_by_idx

    };

    class super_cell_widget_t : public ws_item_inline_tool_widget_t,
                                public iupdatable_externally_t {

        Q_OBJECT

      private:

      public:

        qbinded_int3_input_t *m_sp_rep;
        QLabel *m_sp_rep_label;

        qbinded_combobox_t *m_tmode_inp;

        std::array<QLabel*, 3> m_boundaries_label;
        std::array<qbinded_int2b_input_t*,3> m_boundaries;

        qspoiler_widget_t *m_gb_rep_par;
        QFormLayout *m_gb_rep_par_lt;

        QVBoxLayout *m_dialog_lt;

        std::shared_ptr<geom_view_t> m_dst{nullptr};
        geom_view_t *m_src_gv{nullptr};
        vector3<int> m_sc_dim{1,1,1};
        supercell_tool_mode_e m_sc_tool_mode{supercell_tool_mode_e::sc_tool_mode_default};

        int get_replication_coeff(int dim_num);
        super_cell_widget_t(QWidget *parent = nullptr);

        void make_super_cell(bool target_cam = true);

        bool restore_cam_on_cancel() override;
        void on_apply() override;
        void on_cancel() override;
        void bind_item(ws_item_t *item) override;
        void updated_externally(uint32_t update_reason) override;
        void mode_changed();

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
