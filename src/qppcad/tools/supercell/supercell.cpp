#include <qppcad/tools/supercell/supercell.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

void supercell_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  astate->log("Supercell tools::exec()");

  if (!item) {
      QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                           QObject::tr("ws_item == nullptr"));
      return;
    }

  auto al = item->cast_as<geom_view_t>();
  if (!al) {
      QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                           QObject::tr("ws_item.cast<geom_view_t>() == nullptr"));
      return;
    }

  if (al->m_geom->DIM != 3) {
      QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                           QObject::tr("al->m_geom->DIM != 3"));
      return;
    }

  if (!al->m_parent_ws) {
      QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                           QObject::tr("!al->m_parent_ws!al->m_parent_ws"));
      return;
    }

}

void supercell_tool_t::make_super_cell(geom_view_t *al,
                                       const int a_steps,
                                       const int b_steps,
                                       const int c_steps) {

  if (al->m_geom->DIM != 3) {
      QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                           QObject::tr("al->m_geom->DIM != 3"));
      return;
    }

  if (!al->m_parent_ws) {
      QMessageBox::warning(nullptr, QObject::tr("Supercell generation"),
                           QObject::tr("!al->m_parent_ws!al->m_parent_ws"));
      return;
    }

  std::shared_ptr<geom_view_t> sc_al = std::make_shared<geom_view_t>();
  sc_al->m_geom->DIM = 3;
  sc_al->m_geom->cell.DIM = 3;

  //sc_al->set_parent_workspace(parent_ws);
  sc_al->begin_structure_change();

  if (al->m_role == geom_view_role_e::r_uc) {
      sc_al->m_draw_img_atoms = false;
      sc_al->m_draw_img_bonds = false;
    }

  index sc_dim{a_steps - 1 , b_steps - 1 , c_steps - 1};

  geom_view_tools_t::generate_supercell(al->m_geom.get(), sc_al->m_geom.get(), sc_dim, al->m_role);

  sc_al->m_pos = al->m_pos + al->m_geom->cell.v[0] * 1.4f;
  sc_al->m_name = al->m_name + fmt::format("_sc_{}_{}_{}", a_steps, b_steps, c_steps);

  al->m_parent_ws->add_item_to_ws(sc_al);

  if (sc_al->m_geom->nat() > 200) {
      sc_al->m_draw_img_atoms = false;
      sc_al->m_draw_img_bonds = false;
    }

  sc_al->end_structure_change();

  //perform purification
  if (al->m_role == geom_view_role_e::r_uc) {

      sc_al->m_tws_tr->do_action(act_lock);
      xgeometry<float, periodic_cell<float> > g(3); //intermediate xgeom
      g.set_format({"charge"},{type_real});
      g.DIM = 3;
      g.cell.DIM = 3;
      g.cell.v[0] = sc_al->m_geom->cell.v[0];
      g.cell.v[1] = sc_al->m_geom->cell.v[1];
      g.cell.v[2] = sc_al->m_geom->cell.v[2];

      const float equality_dist = 0.01f;

      for (int i = 0; i < sc_al->m_geom->nat(); i++) {

          std::vector<tws_node_content_t<float> > res;
          sc_al->m_tws_tr->query_sphere(equality_dist, sc_al->m_geom->pos(i), res);
          float accum_chg = 0;

          bool need_to_add{true};
          for (auto &elem : res)
            if (elem.m_idx == index::D(sc_al->m_geom->DIM).all(0)) {
                accum_chg += sc_al->m_geom->xfield<float>(xgeom_charge, elem.m_atm);
                if (i > elem.m_atm) need_to_add = false;
              }

          if (need_to_add) {
              g.add(sc_al->m_geom->atom(i), sc_al->m_geom->pos(i));
              g.xfield<float>(xgeom_charge, g.nat()-1) = accum_chg;
            }

        }

      sc_al->m_geom->clear();
      sc_al->copy_from_xgeom(g);
      sc_al->m_tws_tr->do_action(act_unlock | act_rebuild_all);

    }

  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_changed();

}

ws_item_inline_tool_widget_t *supercell_tool_t::construct_inline_tool() {

  //TODO: test widget
  return new super_cell_widget_t();

}

int super_cell_widget_t::get_replication_coeff(int dim_num) {

  switch (dim_num) {

    case 0:
      return m_sp_rep->sb_x->value();

    case 1:
      return m_sp_rep->sb_y->value();

    case 2:
      return m_sp_rep->sb_z->value();

    default:
      return 0;

    }

}

super_cell_widget_t::super_cell_widget_t (QWidget *parent)
  : ws_item_inline_tool_widget_t(parent) {

  app_state_t *astate = app_state_t::get_inst();

  //setFixedWidth(250);
  setWindowTitle(tr("Supercell generation"));

  m_dialog_lt = new QVBoxLayout;
  m_dialog_lt->setContentsMargins(1, 1, 1, 1);
  setLayout(m_dialog_lt);

  m_gb_rep_par = new qspoiler_widget_t(tr("Parameters"), nullptr, false, 6,
                                       astate->size_guide.obj_insp_table_w());
  m_gb_rep_par_lt = new QFormLayout;
  m_gb_rep_par->add_content_layout(m_gb_rep_par_lt);

  m_sp_rep = new qbinded_int3_input_t;
  m_sp_rep->set_min_max_step(1, 20, 1);
  m_sp_rep->bind_value(&m_sc_dim, this);
  m_sp_rep->m_updated_externally_event = true;

  m_gb_rep_par_lt->addRow(tr("Cell ratio"), m_sp_rep);

  qt_hlp::resize_form_lt_lbls(m_gb_rep_par_lt, astate->size_guide.obj_insp_lbl_w());

  m_dialog_lt->addWidget(m_gb_rep_par);

}

void super_cell_widget_t::make_super_cell(const int a_max, const int b_max, const int c_max,
                                          bool target_cam) {

  if (!m_src_gv) {
      return;
    }

  if (m_src_gv->m_geom->DIM != 3) {
      return;
    }

  if (!m_dst) {
      m_dst = std::make_shared<geom_view_t>();
      m_dst->m_name = m_src->m_name + fmt::format("_sc_{}_{}_{}", a_max, b_max, c_max);
      m_src->m_parent_ws->add_item_to_ws(m_dst);
    }

  m_dst->m_geom->DIM = 3;
  m_dst->m_geom->cell.DIM = 3;

  m_dst->begin_structure_change();
  m_dst->m_geom->clear();
  geom_view_tools_t::generate_supercell(m_src_gv->m_geom.get(), m_dst->m_geom.get(),
                                        index{a_max -1 , b_max - 1 , c_max - 1});
  m_dst->m_pos = m_src->m_pos + m_src_gv->m_geom->cell.v[0] * 1.4f;
  if (target_cam) m_dst->apply_target_view(cam_tv_e::tv_b);
  m_dst->end_structure_change();

}

void super_cell_widget_t::on_apply() {

}

void super_cell_widget_t::on_cancel() {

  if (m_dst) {

      m_dst->m_marked_for_deletion = true;

    }

}

void super_cell_widget_t::bind_item(ws_item_t *item) {

  ws_item_inline_tool_widget_t::bind_item(item);

  if (m_src->get_type() == geom_view_t::get_type_static()) {

      m_src_gv = m_src->cast_as<geom_view_t>();
      m_dst = nullptr;

    } else {

      m_src = nullptr;
      m_src_gv = nullptr;

    }

  make_super_cell(m_sc_dim[0], m_sc_dim[1], m_sc_dim[2]);

}

void super_cell_widget_t::updated_externally(uint32_t update_reason) {

  make_super_cell(m_sc_dim[0], m_sc_dim[1], m_sc_dim[2], false);

}

