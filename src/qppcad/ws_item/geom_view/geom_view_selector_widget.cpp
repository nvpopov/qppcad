#include <qppcad/ws_item/geom_view/geom_view_selector_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
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

            //astate->tlog("@DEBUG: purify_boundary_atoms_widget_t::rebuild_sub_gvs() -> in cycle");

            QListWidgetItem *list_item = new QListWidgetItem(list_gv);

            std::string list_item_name = fmt::format("{}/{}",
                                                     as_gv->m_parent_ws->m_ws_name,
                                                     as_gv->m_name);

            list_item->setText(QString::fromStdString(list_item_name));

            //m_sub_gvs.push_back(as_gv);

          }

}

qgeom_view_selector_widget_t::qgeom_view_selector_widget_t(QWidget *parent) : QWidget(parent) {

  app_state_t *astate = app_state_t::get_inst();

  setMinimumHeight(250);
  setMinimumWidth(400);

  main_lt = new QHBoxLayout;
  setLayout(main_lt);

  list_gv = new QListWidget;

  add_btn = new QPushButton(">>");
  add_btn_lt = new QVBoxLayout;
  add_btn_lt->addSpacing(1);
  add_btn_lt->addWidget(add_btn);
  add_btn_lt->addSpacing(1);

  list_gv_res = new QListWidget;

  main_lt->addWidget(list_gv);
  main_lt->addLayout(add_btn_lt);
  main_lt->addWidget(list_gv_res);

  generate_list_gv_items();

}
