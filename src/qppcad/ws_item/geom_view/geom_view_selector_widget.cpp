#include <qppcad/ws_item/geom_view/geom_view_selector_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void qgeom_view_selector_widget_t::generate_list_gv_items() {

  app_state_t *astate = app_state_t::get_inst();

  if (!list_gv) return;

  list_gv->clear();

  for (auto ws : astate->ws_mgr->m_ws)
    for (auto ws_item : ws->m_ws_items)
      if (ws_item->get_type() == geom_view_t::get_type_static())
        if (auto as_gv = ws_item->cast_as<geom_view_t>(); as_gv) {

            QListWidgetItem *list_item = new QListWidgetItem(list_gv);

            std::string list_item_name = fmt::format("{}/{}",
                                                     as_gv->m_parent_ws->m_ws_name,
                                                     as_gv->m_name);

            list_item->setText(QString::fromStdString(list_item_name));

            m_sub_gvs.push_back(as_gv);

          }

}

void qgeom_view_selector_widget_t::compose_selection_query(
    std::vector<geom_view_selection_query_t> &sel_query) {

  QList<qgeom_view_selector_entry_t*> list_wdgt = findChildren<qgeom_view_selector_entry_t*>();

  for (auto &list_rec : list_wdgt) {

      geom_view_selection_query_t sel_rec;
      sel_rec.gv = list_rec->binded_gv;

      if (list_rec->gv_anim_name->isEnabled())
        sel_rec.anim_id = list_rec->gv_anim_name->currentIndex();

      if (list_rec->gv_frame_id->isEnabled())
        sel_rec.frame_id = list_rec->gv_frame_id->currentIndex();

      sel_query.push_back(std::move(sel_rec));

    }

}

qgeom_view_selector_widget_t::qgeom_view_selector_widget_t(QWidget *parent) : QWidget(parent) {

  app_state_t *astate = app_state_t::get_inst();

  setMinimumHeight(350);
  setMinimumWidth(450);

  main_lt = new QHBoxLayout;
  setLayout(main_lt);

  list_gv = new QListWidget;
  list_gv->setMinimumWidth(400);

  add_btn = new QPushButton(">>");
  add_btn_lt = new QVBoxLayout;
  add_btn_lt->addSpacing(1);
  add_btn_lt->addWidget(add_btn);
  add_btn_lt->addSpacing(1);

  connect(add_btn,
          &QPushButton::clicked,
          this,
          &qgeom_view_selector_widget_t::add_btn_clicked);

  list_gv_res = new qgeom_view_result_widget_t;
  list_gv_res->setMinimumWidth(400);

  list_gv_res_scroll_area = new QScrollArea;
  list_gv_res_scroll_area->setMinimumWidth(450);
  list_gv_res_scroll_area->setWidget(list_gv_res);
  list_gv_res_scroll_area->setWidgetResizable(true);
  list_gv_res_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  list_gv_res_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  main_lt->addWidget(list_gv);
  main_lt->addLayout(add_btn_lt);
  main_lt->addWidget(list_gv_res_scroll_area);

  generate_list_gv_items();

}

void qgeom_view_selector_widget_t::add_btn_clicked() {

  auto sel_id = list_gv->currentRow();
  if (sel_id < 0 || sel_id >= list_gv->count()) return;

  qgeom_view_selector_entry_t *new_entry = new qgeom_view_selector_entry_t;
  new_entry->binded_gv = m_sub_gvs[sel_id];
  new_entry->rebuild();
  list_gv_res->main_lt->addWidget(new_entry);

}

qgeom_view_selector_entry_t::qgeom_view_selector_entry_t(QWidget *parent): QFrame(parent) {

  app_state_t *astate = app_state_t::get_inst();

  main_lt = new QVBoxLayout;
  setLayout(main_lt);

  setFrameStyle(QFrame::StyledPanel);
  ctrls_lt = new QHBoxLayout;

  gv_name = new QLabel("entry_name1");
  gv_anim_name = new QComboBox;
  gv_frame_id = new QComboBox;
  close_button = new QPushButton(tr("Delete"));

  main_lt->addWidget(gv_name);
  ctrls_lt->addWidget(gv_anim_name);
  ctrls_lt->addWidget(gv_frame_id);
  ctrls_lt->addWidget(close_button);
  ctrls_lt->addSpacing(1);
  main_lt->addLayout(ctrls_lt);

  setFixedHeight(80);

}

void qgeom_view_selector_entry_t::rebuild() {

  if (!binded_gv) return;

  std::string list_item_name = fmt::format("{}/{}",
                                           binded_gv->m_parent_ws->m_ws_name,
                                           binded_gv->m_name);

  gv_name->setText(QString::fromStdString(list_item_name));

  if (binded_gv->m_anim->animable()) {

      gv_anim_name->setEnabled(true);
      gv_frame_id->setEnabled(true);
      gv_anim_name->blockSignals(true);
      gv_frame_id->blockSignals(true);

      gv_anim_name->clear();
      for (size_t i = 0; i < binded_gv->m_anim->get_total_anims(); i++)
        gv_anim_name->addItem(
              QString::fromStdString(binded_gv->m_anim->m_anim_data[i].m_anim_name));

      gv_anim_name->blockSignals(false);
      gv_frame_id->blockSignals(false);

    } else {
      gv_anim_name->setEnabled(false);
      gv_frame_id->setEnabled(false);
    }

  //gv_frame_id->

}

void qgeom_view_selector_entry_t::cmb_anim_name_changed(int idx) {

  if (!binded_gv || !binded_gv->m_anim->animable()) return;

  auto cur_anim_idx = gv_anim_name->currentIndex();

  gv_frame_id->clear();

  for (size_t i = 0; i < binded_gv->m_anim->m_anim_data[cur_anim_idx].frames.size(); i++)
    gv_frame_id->addItem(QString("%1").arg(i));

}

qgeom_view_result_widget_t::qgeom_view_result_widget_t(QWidget *parent) : QWidget(parent) {

  main_lt = new QVBoxLayout;
  main_lt->addSpacing(1);
  setLayout(main_lt);

}
