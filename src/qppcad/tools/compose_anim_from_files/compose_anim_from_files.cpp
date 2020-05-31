#include <qppcad/tools/compose_anim_from_files/compose_anim_from_files.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void compose_anim_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  astate->log("compose_anim_from_files_tool_t::exec()");

  auto [ok, cur_ws] = astate->ws_mgr->get_sel_tuple_ws();

  if (!ok) {
      QMessageBox::warning(nullptr, QObject::tr("compose_anim_from_files_tool_t"),
                           QObject::tr("cur_ws == nullptr"));
      return;
    }

  compose_anim_widget_t caw;
  int ret_code = caw.exec();

  std::vector<geom_view_selection_query_t> sel_query;
  caw.gv_selector->compose_selection_query(sel_query);

  if (ret_code == QDialog::Accepted && !sel_query.empty()) {

      //perform nat() check
      std::set<size_t> nats;
      for (auto &rec : sel_query) nats.insert(rec.gv->m_geom->nat());
      if (nats.size() != 1) {
          //throw error
          return;
        }

      //fine, do the job
      auto new_it = astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(geom_view_t::get_type_static());
      auto new_gv = new_it->cast_as<geom_view_t>();
      new_gv->m_name.set_value(fmt::format("composed_gv{}", cur_ws->num_items()));
      cur_ws->add_item_to_ws(new_it);

      geom_anim_record_t<float> new_anim;
      new_anim.frames.resize(sel_query.size());
      new_anim.m_anim_name = "composed0";
      new_anim.m_anim_type = geom_anim_t::anim_geo_opt;
      int img_idx = 0;

      for (auto &rec : sel_query) {

          //init xgeom
          if (img_idx == 0) {
              new_gv->begin_structure_change();
              rec.gv->copy_to_xgeom(*new_gv->m_geom);
              new_gv->end_structure_change();
            }

          //copy frames
          new_anim.frames[img_idx].atom_pos.resize(new_gv->m_geom->nat());

          for (size_t i = 0; i < new_gv->m_geom->nat(); i++)
            new_anim.frames[img_idx].atom_pos[i] = rec.gv->m_geom->pos(i);

          img_idx++;

        }

      new_gv->m_anim->m_anim_data.emplace_back(std::move(new_anim));

    }

  astate->make_viewport_dirty();

}

compose_anim_widget_t::compose_anim_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  setWindowTitle(tr("Compose animation from files"));
  setMaximumWidth(480);

  main_lt = new QVBoxLayout;
  setLayout(main_lt);

  gv_selector = new qgeom_view_selector_widget_t;
  dialog_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  for (auto btn : dialog_bb->buttons())
    btn->setFixedWidth(astate->size_guide.common_button_fixed_w());

  connect(dialog_bb,
          &QDialogButtonBox::accepted,
          this,
          &compose_anim_widget_t::accept);

  connect(dialog_bb,
          &QDialogButtonBox::rejected,
          this,
          &compose_anim_widget_t::reject);

  main_lt->addWidget(gv_selector);
  main_lt->addWidget(dialog_bb);

}
