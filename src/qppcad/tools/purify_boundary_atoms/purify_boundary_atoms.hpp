#ifndef QPPCAD_TOOL_PURIFY_BOUNDARY_ATOMS
#define QPPCAD_TOOL_PURIFY_BOUNDARY_ATOMS
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QListWidget>

namespace qpp {

  namespace cad {

    class geom_view_t;

    class purify_boundary_atoms_widget_t : public QDialog {

      Q_OBJECT

    private:

    public:

      geom_view_t *master_gv{nullptr};
      std::vector<geom_view_t*> m_sub_gvs;

      QVBoxLayout *main_lt;
      QDialogButtonBox *dialog_bb;
      QHBoxLayout *actions_lt;
      QPushButton *action_select_all;

      QListWidget *sub_gvs_wdgt;

      purify_boundary_atoms_widget_t();
      void rebuild_sub_gvs();
      void select_all_clicked();

    };

    class purify_boundary_atoms_tool_t : public ws_item_tool_t {

    public:

      void exec(ws_item_t *item, uint32_t _error_ctx) override;

    };

  } // namespace cad

} // namespace qpp

#endif
