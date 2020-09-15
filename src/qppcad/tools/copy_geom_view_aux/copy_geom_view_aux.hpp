#ifndef QPPCAD_TOOL_COPY_GEOM_VIEW_AUX
#define QPPCAD_TOOL_COPY_GEOM_VIEW_AUX

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <qppcad/core/iupdatable.hpp>
#include <qppcad/core/qppcad.hpp>
#include <qppcad/ui/ws_item_inline_tool_widget.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/ws_item_list_widget.hpp>

namespace qpp {

namespace cad {

class geom_view_t;

class copy_geom_view_aux_widget_t : public ws_item_inline_tool_widget_t,
                                    public iupdatable_externally_t {
  Q_OBJECT

 private:
 public:

  QVBoxLayout* main_lt;
  QHBoxLayout* actions_lt;
  QPushButton* action_select_all;

  QCheckBox* cb_copy_settings;
  QCheckBox* cb_copy_xgeom;
  QCheckBox* cb_copy_msr;

  ws_item_list_widget_t* sub_gv;
  geom_view_t* m_src_gv{nullptr};

  explicit copy_geom_view_aux_widget_t(QWidget* parent = nullptr);

  void on_apply() override;
  void on_cancel() override;
  void bind_item(ws_item_t* item) override;
  void updated_externally(uint32_t update_reason) override;

};

class copy_geom_view_aux_tool_t : public ws_item_tool_t {

public:

  void exec(ws_item_t* item, uint32_t _error_ctx) override;
  ws_item_inline_tool_widget_t* construct_inline_tool() override;

};

}  // namespace cad

}  // namespace qpp

#endif
