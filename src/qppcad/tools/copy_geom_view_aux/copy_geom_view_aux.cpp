#include <qppcad/tools/copy_geom_view_aux/copy_geom_view_aux.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void copy_geom_view_aux_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

}

ws_item_inline_tool_widget_t *copy_geom_view_aux_tool_t::construct_inline_tool() {

  return new copy_geom_view_aux_widget_t();

}

copy_geom_view_aux_widget_t::copy_geom_view_aux_widget_t(QWidget *parent)
  : ws_item_inline_tool_widget_t(parent) {

  app_state_t *astate = app_state_t::get_inst();

  setWindowTitle(tr("Copy geom. view aux data"));

  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(2, 2, 2, 2);
  setLayout(main_lt);

  sub_gv = new ws_item_list_widget_t;
  sub_gv->m_ws_item_class = geom_view_t::get_type_static();

  actions_lt = new QHBoxLayout;
  action_select_all = new QPushButton(tr(" Toggle selection "));
  connect(action_select_all,
          &QPushButton::clicked,
          sub_gv,
          &ws_item_list_widget_t::select_all_clicked);

  cb_copy_settings = new QCheckBox(nullptr);
  cb_copy_settings->setText(tr("Copy settings"));
  cb_copy_settings->setChecked(true);

  cb_copy_xgeom = new QCheckBox(nullptr);
  cb_copy_xgeom->setText(tr("Copy xgeometry"));
  cb_copy_xgeom->setChecked(true);

  cb_copy_msr = new QCheckBox(nullptr);
  cb_copy_msr->setText(tr("Copy measurements"));
  cb_copy_msr->setChecked(true);

  actions_lt->addWidget(action_select_all);
  actions_lt->addWidget(cb_copy_settings);
  actions_lt->addWidget(cb_copy_xgeom);
  actions_lt->addWidget(cb_copy_msr);
  actions_lt->addStretch(1);

  main_lt->addLayout(actions_lt);
  main_lt->addWidget(sub_gv);
  //main_lt->addWidget(dialog_bb);

}

void copy_geom_view_aux_widget_t::on_apply() {

  auto master_as_gv = sub_gv->m_master_item->cast_as<geom_view_t>();
  for (size_t i = 0; i < sub_gv->count(); i++)
    if (QListWidgetItem *item = sub_gv->item(i); item && item->checkState() == Qt::Checked)
      if (auto slave_as_gv = sub_gv->m_sub_items[i]->cast_as<geom_view_t>(); slave_as_gv) {
          if (cb_copy_settings->isChecked()) slave_as_gv->copy_settings(master_as_gv);
          if (cb_copy_xgeom->isChecked()) slave_as_gv->copy_xgeom_aux(master_as_gv);
          if (cb_copy_msr->isChecked()) slave_as_gv->copy_measurements(master_as_gv);
        }

}

void copy_geom_view_aux_widget_t::on_cancel() {

}

void copy_geom_view_aux_widget_t::bind_item(ws_item_t *item) {

  ws_item_inline_tool_widget_t::bind_item(item);

  if (m_src->get_type() == geom_view_t::get_type_static()) {
      m_src_gv = m_src->cast_as<geom_view_t>();
    } else {
      m_src_gv = nullptr;
      sub_gv->m_master_item = nullptr;
    }

  if (!m_src_gv) return;

  //std::cout << "@@@ MSRCGV SET " << m_src_gv->m_name << std::endl;

  sub_gv->m_master_item = m_src_gv;
  sub_gv->rebuild_sub_gvs([](ws_item_t *master, ws_item_t *slave) -> bool {

    if (!master || !slave) return false;

    auto master_as_gv = master->cast_as<geom_view_t>();
    auto slave_as_gv = slave->cast_as<geom_view_t>();
    if (!master_as_gv || !slave_as_gv) return false;

    return master_as_gv != slave_as_gv
           && master_as_gv->m_geom->get_DIM() == slave_as_gv->m_geom->get_DIM()
           && master_as_gv->m_geom->nat() == slave_as_gv->m_geom->nat();

  });

}

void copy_geom_view_aux_widget_t::updated_externally(uint32_t update_reason) {

}

