#include <qppcad/tools/structure_similarity/structure_similarity.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>
#include <QApplication>
#include <QClipboard>

using namespace qpp;
using namespace qpp::cad;

void structure_similarity_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_mgr->has_wss()) {

    structure_similarity_widget_t scw;
    scw.exec();

  }

}

structure_similarity_widget_t::structure_similarity_widget_t() : QDialog () {

  app_state_t *astate = app_state_t::get_inst();

  Qt::WindowFlags flags = 0;
  flags |= Qt::WindowMaximizeButtonHint;
  flags |= Qt::WindowCloseButtonHint;
  setWindowFlags(flags);

  setWindowTitle(tr("Structure similarity"));
  m_wdgt_lt = new QVBoxLayout;
  m_wdgt_top_lt = new QHBoxLayout;
  setLayout(m_wdgt_top_lt);
  m_wdgt_top_lt->addLayout(m_wdgt_lt);

  m_gb_str_sim_main = new qspoiler_widget_t(tr("Actions"), nullptr, false, 6,
                                            astate->size_guide.tool_left_part_w());
  m_gb_str_sim_main_lt = new QFormLayout;
  m_gb_str_sim_main->add_content_layout(m_gb_str_sim_main_lt);

  m_cmb_method = new QComboBox;

  m_cmb_method->addItem(tr("compare-naive"));
  m_cmb_method->addItem(tr("compare-tws_tree"));
  m_cmb_method->setFixedWidth(astate->size_guide.obj_insp_combo_max_w_v2());

  m_chck_only_selected = new QCheckBox;

  m_btn_compute = new QPushButton(tr("Compute"));
  m_btn_compute->setFixedWidth(astate->size_guide.obj_insp_combo_max_w_v2());
  connect(m_btn_compute,
          &QPushButton::clicked,
          this,
          &structure_similarity_widget_t::compute_btn_clck);

  m_btn_copy_to_clipboard = new QPushButton(tr("Copy to clipboard"));
  m_btn_copy_to_clipboard->setFixedWidth(astate->size_guide.obj_insp_combo_max_w_v2());
  connect(m_btn_copy_to_clipboard,
          &QPushButton::clicked,
          this,
          &structure_similarity_widget_t::copy_to_cb_btn_clck);

  m_gb_str_sim_main_lt->addRow(tr("Method"), m_cmb_method);
  m_gb_str_sim_main_lt->addRow(tr("Only selected"), m_chck_only_selected);
  m_gb_str_sim_main_lt->addRow(tr(""), m_btn_compute);
  m_gb_str_sim_main_lt->addRow(tr(""), m_btn_copy_to_clipboard);

  qt_hlp::resize_form_lt_lbls(m_gb_str_sim_main_lt,
                              astate->size_guide.common_tools_panel_label_w_big());
  m_wdgt_lt->addWidget(m_gb_str_sim_main);

  for (size_t i = 0; i < 2; i++) {
    m_anim_info[i] = new str_sim_ws_item_rec_t(i + 1, nullptr);
    m_wdgt_lt->addWidget(m_anim_info[i]);
  }

  m_gb_str_sim_out = new qspoiler_widget_t(tr("Results"));
  m_gb_str_sim_out->setFixedWidth(QWIDGETSIZE_MAX);
  m_gb_str_sim_out_lt = new QVBoxLayout;
  m_gb_str_sim_out->add_content_layout(m_gb_str_sim_out_lt);

  m_str_sim_table = new QTableWidget;
  QStringList table_hdr;
  table_hdr.push_back("Atom");
  table_hdr.push_back("Type1");
  table_hdr.push_back("Type2");
  table_hdr.push_back("|D|");
  table_hdr.push_back("Dx");
  table_hdr.push_back("Dy");
  table_hdr.push_back("Dz");

  m_str_sim_table->setColumnCount(7);
  m_str_sim_table->setHorizontalHeaderLabels(table_hdr);
  m_str_sim_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_str_sim_table->verticalHeader()->setVisible(false);

  m_str_sim_table->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  m_str_sim_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_str_sim_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_str_sim_table->setMinimumWidth(550);
  m_str_sim_table->setMinimumHeight(350);

  m_str_sim_out = new QTextEdit;
  m_str_sim_out->setReadOnly(true);
  m_str_sim_out->setMaximumHeight(150);

  m_gb_str_sim_out_lt->addWidget(m_str_sim_table);
  m_gb_str_sim_out_lt->addWidget(m_str_sim_out);

  m_wdgt_top_lt->addWidget(m_gb_str_sim_out);

  //qt_hlp::resize_form_lt_lbls(gb_str_sim_main_lt, astate->size_guide.common_tools_panel_label_w());

  m_wdgt_lt->addStretch(1);

}

void structure_similarity_widget_t::compute_structure_similarity(ws_item_t *g1, ws_item_t *g2) {

  m_str_sim_out->clear();
  m_str_sim_table->clearContents();
  m_str_sim_table->setRowCount(0);

  m_str_sim_out->insertPlainText("Computing structure similarity...\n");

  if (!g1) {
    m_str_sim_out->insertPlainText("Item №1 is invalid\n");
    return;
  }

  if (!g2) {
    m_str_sim_out->insertPlainText("Item №2 is invalid\n");
    return;
  }

  m_str_sim_out->insertPlainText("Is Item №1 has type of geom_view_t ?");
  auto gv1 = g1->cast_as<geom_view_t>();
  if (gv1) m_str_sim_out->insertPlainText(" Yes\n");
  else m_str_sim_out->insertPlainText(" No\n");

  m_str_sim_out->insertPlainText("Is Item №2 has type of geom_view_t ?");
  auto gv2 = g2->cast_as<geom_view_t>();
  if (gv2) m_str_sim_out->insertPlainText(" Yes\n");
  else m_str_sim_out->insertPlainText(" No\n");

  size_t compare_method = m_cmb_method->currentIndex();

  if (compare_method == 0) {
    compute_structure_similarity_naive(gv1, gv2);
  }

  if (compare_method == 1) {
    m_str_sim_out->insertPlainText("Compare method = unimplemented\n");
  }

}

void structure_similarity_widget_t::compute_structure_similarity_naive(geom_view_t *g1,
                                                                       geom_view_t *g2) {

  //Checking dimensions
  if (g1->m_geom->get_DIM() != g2->m_geom->get_DIM()) {
    m_str_sim_out->insertPlainText(tr("Dim1(%1) != Dim2(%2). Aborting\n")
                                      .arg(g1->m_geom->get_DIM())
                                      .arg(g2->m_geom->get_DIM()));
    return;
  }

  //Checking nat()
  if (g1->m_geom->nat() != g2->m_geom->nat()) {
    m_str_sim_out->insertPlainText(tr("nat1(%1) != nat2(%2). Aborting\n")
                                      .arg(g1->m_geom->nat())
                                      .arg(g2->m_geom->nat()));
    return;
  }

  //Checking n_types
  if (g1->m_geom->n_types() != g2->m_geom->n_types()) {
    m_str_sim_out->insertPlainText(tr("Warning! ntypes1(%1) != ntypes2(%2).\n")
                                      .arg(g1->m_geom->n_types())
                                      .arg(g2->m_geom->n_types()));
  }

  //All checks passed
  m_str_sim_out->insertPlainText("\nAll checks passed!\n");

  bool op_on_sel_atoms = m_chck_only_selected->checkState() == Qt::Checked;
  std::set<size_t> sel_atoms;

  for (auto &gs : {g1, g2})
    for (auto &rec : gs->m_atom_idx_sel) sel_atoms.insert(rec.m_atm);

  m_str_sim_table->setRowCount(op_on_sel_atoms ? sel_atoms.size() : g1->m_geom->nat());

  size_t table_data_idx{0};

  for (size_t i = 0; i < g1->m_geom->nat(); i++)
    if (!op_on_sel_atoms || (sel_atoms.find(i) != sel_atoms.end())) {

      vector3<float> p1 = g1->m_geom->pos(i);
      vector3<float> p2 = g2->m_geom->pos(i);

      if (m_anim_info[0]->m_cmb_anim_name->isVisible()) {
        size_t anim_id = m_anim_info[0]->m_cmb_anim_name->currentIndex();
        size_t frame_id = m_anim_info[0]->m_cmb_anim_frame->currentIndex();
        p1 = g1->m_anim->m_anim_data[anim_id].frames[frame_id].atom_pos[i];
      }

      if (m_anim_info[1]->m_cmb_anim_name->isVisible()) {
        size_t anim_id = m_anim_info[1]->m_cmb_anim_name->currentIndex();
        size_t frame_id = m_anim_info[1]->m_cmb_anim_frame->currentIndex();
        p2 = g2->m_anim->m_anim_data[anim_id].frames[frame_id].atom_pos[i];
      }

      vector3<float> dp = p2 - p1;
      set_out_table_data(g1, g2, table_data_idx, i, dp);
      table_data_idx++;

    }

}

void structure_similarity_widget_t::compute_structure_similarity_tws_tree(geom_view_t *g1,
                                                                          geom_view_t *g2) {

}

void structure_similarity_widget_t::set_out_table_data(geom_view_t *g1,
                                                       geom_view_t *g2,
                                                       size_t table_idx,
                                                       size_t atom_idx,
                                                       vector3<float> _dp) {

  float _dp_l = _dp.norm();

  QTableWidgetItem *atom_id = new QTableWidgetItem(QString("%1").arg(atom_idx));
  atom_id->setTextAlignment(Qt::AlignCenter);
  m_str_sim_table->setItem(table_idx, 0, atom_id);

  bool types_are_different = g1->m_geom->type_table(atom_idx) != g2->m_geom->type_table(atom_idx);
  QTableWidgetItem *atype_g1 =
      new QTableWidgetItem(QString::fromStdString(g1->m_geom->atom_name(atom_idx)));
  atype_g1->setTextAlignment(Qt::AlignCenter);
  if (types_are_different) atype_g1->setBackground(Qt::red);
  m_str_sim_table->setItem(table_idx, 1, atype_g1);

  QTableWidgetItem *atype_g2 =
      new QTableWidgetItem(QString::fromStdString(g2->m_geom->atom_name(atom_idx)));
  atype_g2->setTextAlignment(Qt::AlignCenter);
  if (types_are_different) atype_g2->setBackground(Qt::red);
  m_str_sim_table->setItem(table_idx, 2, atype_g2);

  QTableWidgetItem *dp_lr = new QTableWidgetItem(QString("%1").arg(_dp_l));
  dp_lr->setTextAlignment(Qt::AlignCenter);
  if (_dp_l > 0.001f) dp_lr->setBackgroundColor(Qt::red);
  m_str_sim_table->setItem(table_idx, 3, dp_lr);

  QTableWidgetItem *dp_x = new QTableWidgetItem(QString("%1").arg(_dp[0]));
  dp_x->setTextAlignment(Qt::AlignCenter);
  if (abs(_dp[0]) > 0.001f) dp_x->setBackgroundColor(Qt::red);
  m_str_sim_table->setItem(table_idx, 4, dp_x);

  QTableWidgetItem *dp_y = new QTableWidgetItem(QString("%1").arg(_dp[1]));
  dp_y->setTextAlignment(Qt::AlignCenter);
  if (abs(_dp[1]) > 0.001f) dp_y->setBackgroundColor(Qt::red);
  m_str_sim_table->setItem(table_idx, 5, dp_y);

  QTableWidgetItem *dp_z = new QTableWidgetItem(QString("%1").arg(_dp[2]));
  dp_z->setTextAlignment(Qt::AlignCenter);
  if (abs(_dp[2]) > 0.001f) dp_z->setBackgroundColor(Qt::red);
  m_str_sim_table->setItem(table_idx, 6, dp_z);

}

void structure_similarity_widget_t::compute_btn_clck() {

  compute_structure_similarity(m_anim_info[0]->m_binded_gv, m_anim_info[1]->m_binded_gv);

}

void structure_similarity_widget_t::copy_to_cb_btn_clck() {

  auto g1 = m_anim_info[0]->m_binded_gv;
  auto g2 = m_anim_info[1]->m_binded_gv;

  if (!g1 || !g2) return;
  if (g1->m_geom->nat() != g2->m_geom->nat()) return;

  std::string out_str = "name1, name2, id, x1, y1, z1, x2, y2, z2, dt, dx, dy, dz\n";
  //                       0      1    2   3   4   5   6   7   8   9   10  11  12
  bool op_on_sel_atoms = m_chck_only_selected->checkState() == Qt::Checked;
  std::set<size_t> sel_atoms;

  for (auto &gs : {g1, g2})
    for (auto &rec : gs->m_atom_idx_sel) sel_atoms.insert(rec.m_atm);

  m_str_sim_table->setRowCount(op_on_sel_atoms ? sel_atoms.size() : g1->m_geom->nat());

  for (size_t i = 0; i < g1->m_geom->nat(); i++)
    if (!op_on_sel_atoms || (sel_atoms.find(i) != sel_atoms.end())) {

      vector3<float> p1 = g1->m_geom->pos(i);
      vector3<float> p2 = g2->m_geom->pos(i);

      if (m_anim_info[0]->m_cmb_anim_name->isVisible()) {

        size_t anim_id = m_anim_info[0]->m_cmb_anim_name->currentIndex();
        size_t frame_id = m_anim_info[0]->m_cmb_anim_frame->currentIndex();
        p1 = g1->m_anim->m_anim_data[anim_id].frames[frame_id].atom_pos[i];

      }

      if (m_anim_info[1]->m_cmb_anim_name->isVisible()) {

        size_t anim_id = m_anim_info[1]->m_cmb_anim_name->currentIndex();
        size_t frame_id = m_anim_info[1]->m_cmb_anim_frame->currentIndex();
        p2 = g2->m_anim->m_anim_data[anim_id].frames[frame_id].atom_pos[i];

      }

      vector3<float> dp = p2 - p1;
      //                     0   1   2   3   4   5   6   7   8   9   10  11  12
      out_str += fmt::format("{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}\n",
                             g1->m_geom->atom_name(i),
                             g2->m_geom->atom_name(i),
                             i,
                             p1[0], p1[1], p1[2],
                             p2[0], p2[1], p2[2],
                             dp.norm(), dp[0], dp[1], dp[2],
                             g1->m_geom->xfield<std::string>(xgeom_label_text,i)
                             );

    }

  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(QString::fromStdString(out_str));

}

str_sim_ws_item_rec_t::str_sim_ws_item_rec_t(int index, QWidget *parent) : QWidget(parent) {

  app_state_t *astate = app_state_t::get_inst();

  m_main_lt = new QVBoxLayout;
  setLayout(m_main_lt);
  m_main_lt->setContentsMargins(0, 0, 0, 0);

  m_gb_ws_ws_item = new qspoiler_widget_t(tr("Workspace item №%1:").arg(index),
                                          nullptr, false, 6,
                                          astate->size_guide.tool_left_part_w());
  m_gb_ws_ws_item_lt = new QFormLayout;
  m_gb_ws_ws_item->add_content_layout(m_gb_ws_ws_item_lt);
  //gb_ws_ws_item->setFixedWidth(astate->size_guide.common_tools_panel_w());

  m_cmb_ws = new QComboBox;
  m_cmb_ws->setFixedWidth(astate->size_guide.obj_insp_combo_max_w_v2());
  m_cmb_it = new QComboBox;
  m_cmb_it->setFixedWidth(astate->size_guide.obj_insp_combo_max_w_v2());
  m_gb_ws_ws_item_lt->addRow("Workspace", m_cmb_ws);
  m_gb_ws_ws_item_lt->addRow("Item", m_cmb_it);
  qt_hlp::resize_form_lt_lbls(m_gb_ws_ws_item_lt,
                              astate->size_guide.common_tools_panel_label_w_big());

  m_gb_gv_item = new qspoiler_widget_t(tr("Anim info for item №%1:").arg(index),
                                       nullptr, false, 6,
                                       astate->size_guide.tool_left_part_w());
  m_gb_gv_item_lt = new QFormLayout;
  m_gb_gv_item->add_content_layout(m_gb_gv_item_lt);

  m_cmb_anim_name = new QComboBox;
  m_cmb_anim_name->setFixedWidth(astate->size_guide.obj_insp_combo_max_w_v2());
  m_cmb_anim_frame = new QComboBox;
  m_cmb_anim_frame->setFixedWidth(astate->size_guide.obj_insp_combo_max_w_v2());
  m_gb_gv_item_lt->addRow("Animation", m_cmb_anim_name);
  m_gb_gv_item_lt->addRow("Frame", m_cmb_anim_frame);
  qt_hlp::resize_form_lt_lbls(m_gb_gv_item_lt,
                              astate->size_guide.common_tools_panel_label_w_big());

  m_main_lt->addWidget(m_gb_ws_ws_item);
  m_main_lt->addWidget(m_gb_gv_item);

  setFixedWidth(astate->size_guide.tool_left_part_w());

  connect(m_cmb_ws,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &str_sim_ws_item_rec_t::cmb_ws_changed);

  connect(m_cmb_it,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &str_sim_ws_item_rec_t::cmb_it_changed);

  connect(m_cmb_anim_name,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &str_sim_ws_item_rec_t::cmb_anim_changed);

  rebuild_wss_list();

}

void str_sim_ws_item_rec_t::rebuild_wss_list() {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_mgr->has_wss()) {

    m_cmb_ws->clear();
    m_cmb_ws->setEnabled(true);
    m_cmb_ws->blockSignals(true);
    for (size_t i = 0; i < astate->ws_mgr->m_ws.size(); i++)
      m_cmb_ws->addItem(QString::fromStdString(astate->ws_mgr->m_ws[i]->m_ws_name));
    m_cmb_ws->blockSignals(false);
    cmb_ws_changed(0);

  } else {

    m_cmb_ws->clear();
    m_cmb_ws->setEnabled(false);

  }

}

void str_sim_ws_item_rec_t::rebuild_anim_list() {

  m_cmb_anim_frame->blockSignals(true);
  m_cmb_anim_name->clear();

  if (m_binded_gv && m_binded_gv->m_anim->animable()) {
    for (size_t i = 0 ; i < m_binded_gv->m_anim->m_anim_data.size(); i++)
      m_cmb_anim_name->addItem(
          QString::fromStdString(m_binded_gv->m_anim->m_anim_data[i].m_anim_name)
          );
  }

  m_cmb_anim_frame->blockSignals(false);

}

void str_sim_ws_item_rec_t::cmb_ws_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();

  if (index >=0
      && index < static_cast<int>(astate->ws_mgr->m_ws.size())
      && astate->ws_mgr->m_ws[index]->num_items() != 0) {

    m_cmb_it->clear();
    m_cmb_it->setEnabled(true);
    m_cmb_it->blockSignals(true);

    for (size_t i = 0; i < astate->ws_mgr->m_ws[index]->num_items(); i++) {

      auto ws_item = astate->ws_mgr->m_ws[index]->m_ws_items.get_hs_child_as_array(i);
      if (!ws_item) continue;

      m_cmb_it->addItem(tr("[%1]%2")
                            .arg(i)
                            .arg(QString::fromStdString(ws_item->m_name.get_value()))
                        );
    }

    m_cmb_it->blockSignals(false);
    m_cmb_it->setCurrentIndex(0);
    cmb_it_changed(0);

  } else {

    m_cmb_it->clear();
    m_cmb_it->setEnabled(false);
    m_gb_gv_item->hide();

  }

}

void str_sim_ws_item_rec_t::cmb_it_changed(int idx) {

  app_state_t *astate = app_state_t::get_inst();
  auto wsm = astate->ws_mgr.get();

  auto ws_idx = m_cmb_ws->currentIndex();

  if (ws_idx >= 0
      && ws_idx < static_cast<int>(astate->ws_mgr->m_ws.size())
      && wsm->m_ws[ws_idx]->num_items() !=0
      && idx >= 0
      && idx < static_cast<int>(wsm->m_ws[ws_idx]->num_items())
      /*      && wsm->m_ws[ws_idx]->m_ws_items[idx]
      && wsm->m_ws[ws_idx]->m_ws_items[idx]->get_type() == geom_view_t::get_type_static()*/) {

    auto ws_item = wsm->m_ws[ws_idx]->m_ws_items.get_hs_child_as_array(ws_idx);
    if (ws_item) {

      auto as_gv = ws_item->cast_as<geom_view_t>();
      m_binded_gv = as_gv;

      if (as_gv->m_anim->animable()) {
        m_gb_gv_item->show();
        rebuild_anim_list();
      } else {
        m_gb_gv_item->hide();
      }

      return;

    }

  }

  m_binded_gv = nullptr;
  m_gb_gv_item->hide();

}

void str_sim_ws_item_rec_t::cmb_anim_changed(int idx) {

  //app_state_t *astate = app_state_t::get_inst();
  m_cmb_anim_frame->blockSignals(true);
  m_cmb_anim_frame->clear();

  if (m_binded_gv && m_binded_gv->m_anim->animable()) {

    size_t anim_id = m_cmb_anim_name->currentIndex();

    if (anim_id < m_binded_gv->m_anim->get_total_anims())
      for (size_t i = 0 ; i < m_binded_gv->m_anim->m_anim_data[anim_id].frames.size(); i++)
        m_cmb_anim_frame->addItem(QString("%1").arg(i));

  }

  m_cmb_anim_frame->blockSignals(false);

}
