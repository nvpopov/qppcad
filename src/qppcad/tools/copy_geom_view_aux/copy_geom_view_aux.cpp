#include <qppcad/tools/copy_geom_view_aux/copy_geom_view_aux.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void copy_geom_view_aux_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  copy_geom_view_aux_widget_t cgv;
  cgv.sub_gv->m_ws_item_class = geom_view_t::get_type_static();

  if (!item) return;

  auto as_gv = item->cast_as<geom_view_t>();
  if (!as_gv) return;

  cgv.sub_gv->m_master_item = as_gv;
  cgv.sub_gv->rebuild_sub_gvs([](ws_item_t *master, ws_item_t *slave) -> bool {

    if (!master || !slave) return false;

    auto master_as_gv = master->cast_as<geom_view_t>();
    auto slave_as_gv = slave->cast_as<geom_view_t>();
    if (!master_as_gv || !slave_as_gv) return false;

    return master_as_gv != slave_as_gv &&
           master_as_gv->m_geom->DIM == slave_as_gv->m_geom->DIM &&
           master_as_gv->m_geom->nat() == slave_as_gv->m_geom->nat();

  });

  int ret_code = cgv.exec();

  auto master_as_gv = cgv.sub_gv->m_master_item->cast_as<geom_view_t>();

  if (ret_code == QDialog::Accepted)
    for (size_t i = 0; i < cgv.sub_gv->count(); i++) {

        QListWidgetItem *item = cgv.sub_gv->item(i);

        if (item->checkState() == Qt::Checked) {
            auto slave_as_gv = cgv.sub_gv->m_sub_items[i]->cast_as<geom_view_t>();
            if (slave_as_gv) {
                slave_as_gv->py_copy_settings(master_as_gv);
                slave_as_gv->py_copy_xgeom_aux(master_as_gv);
              }

          }

      }

}

copy_geom_view_aux_widget_t::copy_geom_view_aux_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  setWindowTitle(tr("Copy geom. view aux data"));

  main_lt = new QVBoxLayout;
  setLayout(main_lt);

  dialog_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  for (auto btn : dialog_bb->buttons())
    btn->setFixedWidth(astate->size_guide.common_button_fixed_w());

  connect(dialog_bb,
          &QDialogButtonBox::accepted,
          this,
          &copy_geom_view_aux_widget_t::accept);

  connect(dialog_bb,
          &QDialogButtonBox::rejected,
          this,
          &copy_geom_view_aux_widget_t::reject);

  sub_gv = new ws_item_list_widget_t;

  actions_lt = new QHBoxLayout;
  action_select_all = new QPushButton(tr(" Toggle selection "));
  connect(action_select_all,
          &QPushButton::clicked,
          sub_gv,
          &ws_item_list_widget_t::select_all_clicked);

  actions_lt->addWidget(action_select_all);
  actions_lt->addStretch(1);

  main_lt->addLayout(actions_lt);
  main_lt->addWidget(sub_gv);
  main_lt->addWidget(dialog_bb);

}
