#include <qppcad/tools/structure_similarity/structure_similarity.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

void structure_similarity_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_mgr->has_wss()) {

      structure_similarity_widget_t scw;

      int ret_code = scw.exec();

    }

}

structure_similarity_widget_t::structure_similarity_widget_t() : QDialog () {

  app_state_t *astate = app_state_t::get_inst();

  Qt::WindowFlags flags = 0;
  flags |= Qt::WindowMaximizeButtonHint;
  flags |= Qt::WindowCloseButtonHint;
  setWindowFlags(flags);

  setWindowTitle(tr("Structure similarity"));
  widget_lt = new QVBoxLayout;
  widget_top_lt = new QHBoxLayout;
  setLayout(widget_top_lt);
  widget_top_lt->addLayout(widget_lt);

  gb_str_sim_main = new QGroupBox(tr("Actions"));
  gb_str_sim_main_lt = new QFormLayout;
  gb_str_sim_main->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  //gb_str_sim_main->setFixedWidth(astate->size_guide.common_tools_panel_w());

  cmb_method = new QComboBox;
  cmb_method->addItem(tr("compare-naive"));
  cmb_method->addItem(tr("compare-tws_tree"));

  btn_compute = new QPushButton(tr("Compute"));
  connect(btn_compute,
          &QPushButton::clicked,
          this,
          &structure_similarity_widget_t::compute_button_clicked);

  gb_str_sim_main_lt->addRow(tr("Method"), cmb_method);
  gb_str_sim_main_lt->addRow(tr(""), btn_compute);

  gb_str_sim_main->setLayout(gb_str_sim_main_lt);
  widget_lt->addWidget(gb_str_sim_main);

  for (size_t i = 0; i < 2; i++) {
      m_anim_info[i] = new str_sim_ws_item_rec_t(i + 1, nullptr);
      widget_lt->addWidget(m_anim_info[i]);
    }

  gb_str_sim_output = new QGroupBox(tr("Results"));
  gb_str_sim_output_lt = new QVBoxLayout;
  gb_str_sim_output->setLayout(gb_str_sim_output_lt);

  str_sim_table = new QTableWidget;
  QStringList table_hdr;
  table_hdr.push_back("Atom");
  table_hdr.push_back("Type1");
  table_hdr.push_back("Type2");
  table_hdr.push_back("|D|");
  table_hdr.push_back("Dx");
  table_hdr.push_back("Dy");
  table_hdr.push_back("Dz");

  str_sim_table->setColumnCount(7);
  str_sim_table->setHorizontalHeaderLabels(table_hdr);
  str_sim_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  str_sim_table->verticalHeader()->setVisible(false);

  str_sim_table->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  str_sim_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  str_sim_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  str_sim_table->setMinimumWidth(550);
  str_sim_table->setMinimumHeight(350);

  str_sim_output = new QTextEdit;
  str_sim_output->setReadOnly(true);
  str_sim_output->setMaximumHeight(150);

  gb_str_sim_output_lt->addWidget(str_sim_table);
  gb_str_sim_output_lt->addWidget(str_sim_output);

  widget_top_lt->addWidget(gb_str_sim_output);

  qt_hlp::resize_form_lt_lbls(gb_str_sim_main_lt, astate->size_guide.common_tools_panel_label_w());

  widget_lt->addStretch(1);

}

void structure_similarity_widget_t::compute_structure_similarity(ws_item_t *g1, ws_item_t *g2) {

  str_sim_output->clear();
  str_sim_table->clearContents();
  str_sim_table->setRowCount(0);

  str_sim_output->insertPlainText("Computing structure similarity...\n");

  if (!g1) {
      str_sim_output->insertPlainText("Item №1 is invalid\n");
      return;
    }

  if (!g2) {
      str_sim_output->insertPlainText("Item №2 is invalid\n");
      return;
    }

  str_sim_output->insertPlainText("Is Item №1 has type of geom_view_t ?");
  auto gv1 = g1->cast_as<geom_view_t>();
  if (gv1) str_sim_output->insertPlainText(" Yes\n");
  else str_sim_output->insertPlainText(" No\n");

  str_sim_output->insertPlainText("Is Item №2 has type of geom_view_t ?");
  auto gv2 = g2->cast_as<geom_view_t>();
  if (gv2) str_sim_output->insertPlainText(" Yes\n");
  else str_sim_output->insertPlainText(" No\n");

  size_t compare_method = cmb_method->currentIndex();

  if (compare_method == 0) {
      compute_structure_similarity_naive(gv1, gv2);
    }

  if (compare_method == 1) {
      str_sim_output->insertPlainText("Compare method = unimplemented\n");
    }

}

void structure_similarity_widget_t::compute_structure_similarity_naive(geom_view_t *g1,
                                                                       geom_view_t *g2) {

  //Checking dimensions
  if (g1->m_geom->DIM != g2->m_geom->DIM) {
      str_sim_output->insertPlainText(tr("Dim1(%1) != Dim2(%2). Aborting\n")
                                      .arg(g1->m_geom->DIM)
                                      .arg(g2->m_geom->DIM));
      return;
    }

  //Checking nat()
  if (g1->m_geom->nat() != g2->m_geom->nat()) {
      str_sim_output->insertPlainText(tr("nat1(%1) != nat2(%2). Aborting\n")
                                      .arg(g1->m_geom->nat())
                                      .arg(g2->m_geom->nat()));
      return;
    }

  //Checking n_types
  if (g1->m_geom->n_types() != g2->m_geom->n_types()) {
      str_sim_output->insertPlainText(tr("Warning! ntypes1(%1) != ntypes2(%2).\n")
                                      .arg(g1->m_geom->n_types())
                                      .arg(g2->m_geom->n_types()));
    }

  //All checks passed
  str_sim_output->insertPlainText("\nAll checks passed!\n");

  str_sim_table->setRowCount(g1->m_geom->nat());

  for (size_t i = 0; i < g1->m_geom->nat(); i++) {

      vector3<float> p1 = g1->m_geom->pos(i);
      vector3<float> p2 = g2->m_geom->pos(i);

      if (m_anim_info[0]->cmb_anim_name->isVisible()) {
          size_t anim_id = m_anim_info[0]->cmb_anim_name->currentIndex();
          size_t frame_id = m_anim_info[0]->cmb_anim_frame->currentIndex();
          p1 = g1->m_anim->m_anim_data[anim_id].frames[frame_id].atom_pos[i];
        }

      if (m_anim_info[1]->cmb_anim_name->isVisible()) {
          size_t anim_id = m_anim_info[1]->cmb_anim_name->currentIndex();
          size_t frame_id = m_anim_info[1]->cmb_anim_frame->currentIndex();
          p2 = g2->m_anim->m_anim_data[anim_id].frames[frame_id].atom_pos[i];
        }

      vector3<float> dp = p2 - p1;
      set_out_table_data(g1, g2, i, dp);

    }

}

void structure_similarity_widget_t::compute_structure_similarity_tws_tree(geom_view_t *g1,
                                                                          geom_view_t *g2) {

}

void structure_similarity_widget_t::set_out_table_data(geom_view_t *g1,
                                                       geom_view_t *g2,
                                                       size_t atom_idx,
                                                       vector3<float> _dp) {

  float _dp_l = _dp.norm();

  QTableWidgetItem *atom_id = new QTableWidgetItem(QString("%1").arg(atom_idx));
  atom_id->setTextAlignment(Qt::AlignCenter);
  str_sim_table->setItem(atom_idx, 0, atom_id);

  bool types_are_different = g1->m_geom->type_table(atom_idx) != g2->m_geom->type_table(atom_idx);
  QTableWidgetItem *atype_g1 =
      new QTableWidgetItem(QString::fromStdString(g1->m_geom->atom_name(atom_idx)));
  atype_g1->setTextAlignment(Qt::AlignCenter);
  if (types_are_different) atype_g1->setBackground(Qt::red);
  str_sim_table->setItem(atom_idx, 1, atype_g1);

  QTableWidgetItem *atype_g2 =
      new QTableWidgetItem(QString::fromStdString(g2->m_geom->atom_name(atom_idx)));
  atype_g2->setTextAlignment(Qt::AlignCenter);
  if (types_are_different) atype_g2->setBackground(Qt::red);
  str_sim_table->setItem(atom_idx, 2, atype_g2);

  QTableWidgetItem *dp_lr = new QTableWidgetItem(QString("%1").arg(_dp_l));
  dp_lr->setTextAlignment(Qt::AlignCenter);
  if (_dp_l > 0.001f) dp_lr->setBackgroundColor(Qt::red);
  str_sim_table->setItem(atom_idx, 3, dp_lr);

  QTableWidgetItem *dp_x = new QTableWidgetItem(QString("%1").arg(_dp[0]));
  dp_x->setTextAlignment(Qt::AlignCenter);
  if (abs(_dp[0]) > 0.001f) dp_x->setBackgroundColor(Qt::red);
  str_sim_table->setItem(atom_idx, 4, dp_x);

  QTableWidgetItem *dp_y = new QTableWidgetItem(QString("%1").arg(_dp[1]));
  dp_y->setTextAlignment(Qt::AlignCenter);
  if (abs(_dp[1]) > 0.001f) dp_y->setBackgroundColor(Qt::red);
  str_sim_table->setItem(atom_idx, 5, dp_y);

  QTableWidgetItem *dp_z = new QTableWidgetItem(QString("%1").arg(_dp[2]));
  dp_z->setTextAlignment(Qt::AlignCenter);
  if (abs(_dp[2]) > 0.001f) dp_z->setBackgroundColor(Qt::red);
  str_sim_table->setItem(atom_idx, 6, dp_z);

}

void structure_similarity_widget_t::compute_button_clicked() {

  compute_structure_similarity(m_anim_info[0]->m_binded_gv, m_anim_info[1]->m_binded_gv);

}

str_sim_ws_item_rec_t::str_sim_ws_item_rec_t(int index, QWidget *parent) : QWidget(parent) {

  app_state_t *astate = app_state_t::get_inst();

  main_lt = new QVBoxLayout;
  setLayout(main_lt);
  main_lt->setContentsMargins(0, 0, 0, 0);

  gb_ws_ws_item = new QGroupBox(tr("Workspace item №%1:").arg(index));
  gb_ws_ws_item_lt = new QFormLayout;
  gb_ws_ws_item->setLayout(gb_ws_ws_item_lt);
  //gb_ws_ws_item->setFixedWidth(astate->size_guide.common_tools_panel_w());

  cmb_ws = new QComboBox;
  cmb_it = new QComboBox;
  gb_ws_ws_item_lt->addRow("Workspace", cmb_ws);
  gb_ws_ws_item_lt->addRow("Item", cmb_it);
  qt_hlp::resize_form_lt_lbls(gb_ws_ws_item_lt, astate->size_guide.common_tools_panel_label_w());

  gb_gv_item = new QGroupBox(tr("Anim info for item №%1:").arg(index));
  gb_gv_item_lt = new QFormLayout;
  gb_gv_item->setLayout(gb_gv_item_lt);
  //gb_gv_item->setFixedWidth(astate->size_guide.common_tools_panel_w());

  cmb_anim_name = new QComboBox;
  cmb_anim_frame = new QComboBox;
  gb_gv_item_lt->addRow("Animation", cmb_anim_name);
  gb_gv_item_lt->addRow("Frame", cmb_anim_frame);
  qt_hlp::resize_form_lt_lbls(gb_gv_item_lt, astate->size_guide.common_tools_panel_label_w());

  main_lt->addWidget(gb_ws_ws_item);
  main_lt->addWidget(gb_gv_item);

  setFixedWidth(astate->size_guide.common_tools_panel_w());

  connect(cmb_ws,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &str_sim_ws_item_rec_t::cmb_ws_changed);

  connect(cmb_it,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &str_sim_ws_item_rec_t::cmb_it_changed);

  connect(cmb_anim_name,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &str_sim_ws_item_rec_t::cmb_anim_changed);

  rebuild_wss_list();

}

void str_sim_ws_item_rec_t::rebuild_wss_list() {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_mgr->has_wss()) {

      cmb_ws->clear();
      cmb_ws->setEnabled(true);
      cmb_ws->blockSignals(true);
      for (size_t i = 0; i < astate->ws_mgr->m_ws.size(); i++)
        cmb_ws->addItem(QString::fromStdString(astate->ws_mgr->m_ws[i]->m_ws_name));
      cmb_ws->blockSignals(false);
      cmb_ws_changed(0);

    } else {

      cmb_ws->clear();
      cmb_ws->setEnabled(false);

    }

}

void str_sim_ws_item_rec_t::rebuild_anim_list() {

  cmb_anim_frame->blockSignals(true);
  cmb_anim_name->clear();

  if (m_binded_gv && m_binded_gv->m_anim->animable()) {
      for (size_t i = 0 ; i < m_binded_gv->m_anim->m_anim_data.size(); i++)
         cmb_anim_name->addItem(
               QString::fromStdString(m_binded_gv->m_anim->m_anim_data[i].m_anim_name)
               );
    }

  cmb_anim_frame->blockSignals(false);

}

void str_sim_ws_item_rec_t::cmb_ws_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();

  if (index >=0 &&
      index < astate->ws_mgr->m_ws.size() &&
      !astate->ws_mgr->m_ws[index]->m_ws_items.empty()) {

      cmb_it->clear();
      cmb_it->setEnabled(true);
      cmb_it->blockSignals(true);

      for (size_t i = 0; i < astate->ws_mgr->m_ws[index]->m_ws_items.size(); i++) {
          cmb_it->addItem(tr("[%1]%2")
                        .arg(i)
                        .arg(QString::fromStdString(
                               astate->ws_mgr->m_ws[index]->m_ws_items[i]->m_name))
                          );
        }

      cmb_it->blockSignals(false);
      cmb_it->setCurrentIndex(0);
      cmb_it_changed(0);

    } else {

      cmb_it->clear();
      cmb_it->setEnabled(false);
      gb_gv_item->hide();

    }

}

void str_sim_ws_item_rec_t::cmb_it_changed(int idx) {

  app_state_t *astate = app_state_t::get_inst();
  auto wsm = astate->ws_mgr.get();

  auto ws_idx = cmb_ws->currentIndex();

  if (ws_idx >= 0 &&
      ws_idx < astate->ws_mgr->m_ws.size() &&
      !wsm->m_ws[ws_idx]->m_ws_items.empty() &&
      idx >= 0 &&
      idx < wsm->m_ws[ws_idx]->m_ws_items.size() &&
      wsm->m_ws[ws_idx]->m_ws_items[idx] &&
      wsm->m_ws[ws_idx]->m_ws_items[idx]->get_type() == geom_view_t::get_type_static()) {

      auto as_gv = wsm->m_ws[ws_idx]->m_ws_items[idx]->cast_as<geom_view_t>();
      m_binded_gv = as_gv;

      if (as_gv->m_anim->animable()) {
          gb_gv_item->show();
          rebuild_anim_list();
        } else {
          gb_gv_item->hide();
        }

    } else {

      m_binded_gv = nullptr;
      gb_gv_item->hide();

    }

}

void str_sim_ws_item_rec_t::cmb_anim_changed(int idx) {

  app_state_t *astate = app_state_t::get_inst();
  cmb_anim_frame->blockSignals(true);
  cmb_anim_frame->clear();

  if (m_binded_gv && m_binded_gv->m_anim->animable()) {

      size_t anim_id = cmb_anim_name->currentIndex();

      if (anim_id >= 0 && anim_id < m_binded_gv->m_anim->get_total_anims())
        for (size_t i = 0 ; i < m_binded_gv->m_anim->m_anim_data[anim_id].frames.size(); i++)
          cmb_anim_frame->addItem(QString("%1").arg(i));

    }

  cmb_anim_frame->blockSignals(false);

}
