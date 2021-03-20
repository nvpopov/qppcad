#ifndef QPPCAD_TOOL_STRUCTURE_SIMILARITY
#define QPPCAD_TOOL_STRUCTURE_SIMILARITY
#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>
#include <QTextEdit>
#include <QTableWidget>
#include <QHeaderView>

namespace qpp {

namespace cad {

class str_sim_ws_item_rec_t : public QWidget {

  Q_OBJECT

public:

  QVBoxLayout *m_main_lt;

  qspoiler_widget_t *m_gb_ws_ws_item;
  QFormLayout *m_gb_ws_ws_item_lt;
  QComboBox *m_cmb_ws;
  QComboBox *m_cmb_it;

  qspoiler_widget_t *m_gb_gv_item;
  QFormLayout *m_gb_gv_item_lt;
  QComboBox *m_cmb_anim_name;
  QComboBox *m_cmb_anim_frame;

  geom_view_t *m_binded_gv{nullptr};

  void set_visible(bool visible);
  explicit str_sim_ws_item_rec_t(int index, QWidget *parent = nullptr);

  void rebuild_wss_list();
  void rebuild_anim_list();

public slots:

  void cmb_ws_changed(int idx);
  void cmb_it_changed(int idx);
  void cmb_anim_changed(int idx);

};

class structure_similarity_widget_t : public QDialog {

  Q_OBJECT

public:

  qspoiler_widget_t *m_gb_str_sim_main;
  QFormLayout *m_gb_str_sim_main_lt;
  QHBoxLayout *m_wdgt_top_lt;
  QVBoxLayout *m_wdgt_lt;

  std::array<str_sim_ws_item_rec_t*, 2> m_anim_info;

  QComboBox *m_cmb_method;
  QCheckBox *m_chck_only_selected;
  QPushButton *m_btn_compute;
  QPushButton *m_btn_copy_to_clipboard;

  qspoiler_widget_t *m_gb_str_sim_out;
  QVBoxLayout *m_gb_str_sim_out_lt;
  QTextEdit *m_str_sim_out;
  QTableWidget *m_str_sim_table;

  structure_similarity_widget_t();
  void compute_structure_similarity(ws_item_t *g1, ws_item_t *g2);
  void compute_structure_similarity_naive(geom_view_t *g1, geom_view_t *g2);
  void compute_structure_similarity_tws_tree(geom_view_t *g1, geom_view_t *g2);
  void set_out_table_data(geom_view_t *g1, geom_view_t *g2,
                          size_t table_idx, size_t atom_idx,
                          vector3<float> _dp);

public slots:

  void compute_btn_clck();
  void copy_to_cb_btn_clck();

};

class structure_similarity_tool_t : public ws_item_tool_t {

public:

  void exec(ws_item_t *item, uint32_t _error_ctx) override;

};

} // namespace qpp::cad

} // namespace qpp

#endif
