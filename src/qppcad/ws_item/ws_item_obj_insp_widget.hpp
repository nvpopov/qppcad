#ifndef QPPCAD_WS_ITEM_OBJ_INSP_H
#define QPPCAD_WS_ITEM_OBJ_INSP_H

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/ui/qbinded_inputs.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>

namespace qpp {

namespace cad {

struct ws_item_tab_widget_t {

  QWidget *tab_top_wdgt;
  QVBoxLayout *tab_top_wdgt_lt;
  QScrollArea *tab_scroll;
  QFrame *tab_inner_widget;
  QVBoxLayout *tab_inner_widget_lt;
  QIcon *icon_enabled;
  QIcon *icon_disabled;
  int tab_id;

};

class ws_item_obj_insp_widget_t : public QTabWidget {

  Q_OBJECT

public:

  ws_item_t *m_binded_item{nullptr};

  ws_item_tab_widget_t *m_tab_general;
  qspoiler_widget_t *m_sp_info_wdgt;

  QFormLayout *m_tg_form_lt;
  QLabel *m_ws_item_name;
  QLabel *m_ws_item_type;
  QLabel *m_ws_item_pos_label;
  qbinded_float3_input_t *m_ws_item_pos;

  qspoiler_widget_t *m_tg_acts;
  QGridLayout *m_tg_acts_layout;
  QPushButton *m_tg_acts_delete;
  QPushButton *m_tg_acts_rename;
  QPushButton *m_tg_acts_clone;

  qbinded_bool_named_vector_t *m_ws_item_show_item_bb;

  ws_item_tab_widget_t* def_tab(QString tab_name,
                                QString icon_name_enabled,
                                QString icon_name_disabled = "");

  virtual void bind_to_item(ws_item_t *_binding_item);
  virtual void unbind_item();
  virtual void update_from_ws_item();
  void set_tab_enabled(ws_item_tab_widget_t *tab,
                       bool v_enabled);
  void pre_init_gb(QGroupBox *gb, QFormLayout *gb_lt);
  void post_init_gb(QGroupBox *gb, QFormLayout *gb_lt);
  void init_form_lt(QFormLayout *frm_lt);
  void init_form_lt_lbl(QLabel *_label);

  void resizeEvent(QResizeEvent *event) override;

  ws_item_obj_insp_widget_t();

public slots:

  void cur_ws_selected_item_position_changed();
  void rename_current_item();
  void delete_current_item();
  void cur_tab_changed(int index);

};

} // namespace qpp::cad

} // namespace qpp

#endif
