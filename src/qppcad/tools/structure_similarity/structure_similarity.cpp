#include <qppcad/tools/structure_similarity/structure_similarity.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

void structure_similarity_tool_t::exec(ws_item_t *item) {

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

  gb_select_actors = new QGroupBox(tr("Actors"));
  gb_select_actors_lt = new QFormLayout;
  gb_select_actors->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  gb_select_actors->setFixedWidth(astate->size_guide.common_tools_panel_w());

  cmb_it1 = new QComboBox;
  cmb_ws1 = new QComboBox;
  cmb_it2 = new QComboBox;
  cmb_ws2 = new QComboBox;

  cmb_method = new QComboBox;
  cmb_method->addItem(tr("compare-naive"));
  cmb_method->addItem(tr("compare-tws_tree"));

  btn_compute = new QPushButton(tr("Compute"));
  connect(btn_compute,
          &QPushButton::clicked,
          this,
          &structure_similarity_widget_t::compute_button_clicked);

  gb_select_actors_lt->addRow(tr("Wrksp. №1"), cmb_ws1);
  gb_select_actors_lt->addRow(tr("Item №1"), cmb_it1);
  gb_select_actors_lt->addRow(tr("Wrksp. №2"), cmb_ws2);
  gb_select_actors_lt->addRow(tr("Item №2"), cmb_it2);
  gb_select_actors_lt->addRow(tr("Method"), cmb_method);
  gb_select_actors_lt->addRow(tr(""), btn_compute);

  gb_select_actors->setLayout(gb_select_actors_lt);
  widget_lt->addWidget(gb_select_actors);

  m_anim_info[0] = new structure_similarity_anim_info_t(widget_lt, "Actor №1 anim");
  m_anim_info[1] = new structure_similarity_anim_info_t(widget_lt, "Actor №2 anim");

  connect(cmb_ws1,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &structure_similarity_widget_t::cmb_ws1_changed);

  connect(cmb_ws2,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &structure_similarity_widget_t::cmb_ws2_changed);

  connect(cmb_it1,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &structure_similarity_widget_t::cmb_it1_changed);

  connect(cmb_it2,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &structure_similarity_widget_t::cmb_it2_changed);

  connect(m_anim_info[0]->cmb_anim_name,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &structure_similarity_widget_t::cmb_anim1_changed);

  connect(m_anim_info[1]->cmb_anim_name,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &structure_similarity_widget_t::cmb_anim2_changed);

  cmb_ws(cmb_ws1);
  cmb_ws(cmb_ws2);

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
  str_sim_output->setMaximumHeight(110);

  gb_str_sim_output_lt->addWidget(str_sim_table);
  gb_str_sim_output_lt->addWidget(str_sim_output);

  widget_top_lt->addWidget(gb_str_sim_output);

  qt_helpers::resize_form_lt_lbls(gb_select_actors_lt,
                                  astate->size_guide.common_tools_panel_label_w());
  qt_helpers::resize_form_lt_lbls(m_anim_info[0]->gb_itm_anim_lt,
                                  astate->size_guide.common_tools_panel_label_w());
  qt_helpers::resize_form_lt_lbls(m_anim_info[1]->gb_itm_anim_lt,
                                  astate->size_guide.common_tools_panel_label_w());
  widget_lt->addStretch(1);

}

void structure_similarity_widget_t::cmb_ws(QComboBox *_cmb) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_mgr->has_wss()) {

      _cmb->clear();
      _cmb->setEnabled(true);
      for (size_t i = 0; i < astate->ws_mgr->m_ws.size(); i++)
        _cmb->addItem(QString::fromStdString(astate->ws_mgr->m_ws[i]->m_ws_name));

    } else {

      _cmb->clear();
      _cmb->setEnabled(false);

    }

}

void structure_similarity_widget_t::cmb_it(QComboBox *_cmb,
                                           std::shared_ptr<workspace_t> _ws,
                                           size_t itm_idx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!_ws->m_ws_items.empty()) {

      _cmb->clear();
      _cmb->setEnabled(true);
      for (size_t i = 0; i < _ws->m_ws_items.size(); i++) {
          _cmb->addItem(tr("[%1]%2")
                        .arg(i)
                        .arg(QString::fromStdString(_ws->m_ws_items[i]->m_name)));
        }

    } else {

      _cmb->clear();
      _cmb->setEnabled(false);

    }

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

void structure_similarity_widget_t::cmb_it_changed_marshall(int index, size_t actor_id) {

  app_state_t *astate = app_state_t::get_inst();

  m_anim_info[actor_id]->set_visible(false);

  size_t ws_id = actor_id == 0 ? cmb_ws1->currentIndex() : cmb_ws2->currentIndex();

  if (ws_id >= astate->ws_mgr->m_ws.size()) return;
  if (index >= astate->ws_mgr->m_ws[ws_id]->m_ws_items.size()) return;

  auto ws_itm = astate->ws_mgr->m_ws[ws_id]->m_ws_items[index];

  if (!ws_itm) return;

  auto as_al = ws_itm->cast_as<geom_view_t>();
  if (!as_al) return;
  if (!as_al->m_anim->animable()) return;

  m_anim_info[actor_id]->cmb_anim_name->clear();

  for (size_t i = 0 ; i < as_al->m_anim->m_anim_data.size(); i++)
    m_anim_info[actor_id]->cmb_anim_name->addItem(QString::fromStdString(
                                                    as_al->m_anim->m_anim_data[i].m_anim_name));

  m_anim_info[actor_id]->set_visible(true);

  //std::cout << "cmb_it_changed_marshall " << std::endl;

}

void structure_similarity_widget_t::cmb_anim_changed_marshall(int index, size_t actor_id) {

  app_state_t *astate = app_state_t::get_inst();

  size_t ws_id = actor_id == 0 ? cmb_ws1->currentIndex() : cmb_ws2->currentIndex();
  size_t anim_id =  m_anim_info[actor_id]->cmb_anim_name->currentIndex();
  size_t itm_id = 0 ? cmb_it1->currentIndex() : cmb_it2->currentIndex();

  if (ws_id >= astate->ws_mgr->m_ws.size()) return;
  if (index >= astate->ws_mgr->m_ws[ws_id]->m_ws_items.size()) return;

  auto ws_itm = astate->ws_mgr->m_ws[ws_id]->m_ws_items[itm_id];

  if (!ws_itm) return;

  auto as_al = ws_itm->cast_as<geom_view_t>();
  if (!as_al) return;
  if (!as_al->m_anim->animable()) return;

  m_anim_info[actor_id]->cmb_anim_frame->clear();

  for (size_t i = 0 ; i < as_al->m_anim->m_anim_data[anim_id].frames.size(); i++)
    m_anim_info[actor_id]->cmb_anim_frame->addItem(QString("%1").arg(i));

  //std::cout << "cmb_anim_changed_marshall " << std::endl;

}

void structure_similarity_widget_t::compute_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  ws_item_t *it1{nullptr};
  ws_item_t *it2{nullptr};

  size_t ws_i1 = 0;
  size_t ws_i2 = 0;
  size_t it_i1 = 0;
  size_t it_i2 = 0;

  if (cmb_ws1->isEnabled() && cmb_it1->isEnabled()) {
      ws_i1 = cmb_ws1->currentIndex();
      it_i1 = cmb_it1->currentIndex();
      it1 = astate->ws_mgr->m_ws[ws_i1]->m_ws_items[it_i1].get();
    }

  if (cmb_ws2->isEnabled() && cmb_it2->isEnabled()) {
      ws_i2 = cmb_ws2->currentIndex();
      it_i2 = cmb_it2->currentIndex();
      it2 = astate->ws_mgr->m_ws[ws_i2]->m_ws_items[it_i2].get();
    }

  compute_structure_similarity(it1, it2);

}

void structure_similarity_widget_t::cmb_ws1_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();
  cmb_it(cmb_it1, astate->ws_mgr->m_ws[cmb_ws1->currentIndex()], 0);

}

void structure_similarity_widget_t::cmb_ws2_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();
  cmb_it(cmb_it2, astate->ws_mgr->m_ws[cmb_ws2->currentIndex()], 1);

}

void structure_similarity_widget_t::cmb_it1_changed(int index) {

  cmb_it_changed_marshall(index, 0);

}

void structure_similarity_widget_t::cmb_it2_changed(int index) {

  cmb_it_changed_marshall(index, 1);

}

void structure_similarity_widget_t::cmb_anim1_changed(int index) {

  cmb_anim_changed_marshall(index, 0);

}

void structure_similarity_widget_t::cmb_anim2_changed(int index) {

  cmb_anim_changed_marshall(index, 1);

}

void structure_similarity_anim_info_t::set_visible(bool visible) {

  gb_itm_anim->setVisible(visible);

}

structure_similarity_anim_info_t::structure_similarity_anim_info_t(QLayout *lt, QString gb_title) {

  app_state_t *astate = app_state_t::get_inst();

  gb_itm_anim = new QGroupBox(gb_title);
  gb_itm_anim_lt = new QFormLayout;
  gb_itm_anim->setLayout(gb_itm_anim_lt);

  cmb_anim_name = new QComboBox;
  cmb_anim_frame = new QComboBox;

  gb_itm_anim_lt->addRow(QObject::tr("Anim name"), cmb_anim_name);
  gb_itm_anim_lt->addRow(QObject::tr("Anim frame"), cmb_anim_frame);

  lt->addWidget(gb_itm_anim);

  gb_itm_anim->setFixedWidth(astate->size_guide.common_tools_panel_w());

}
