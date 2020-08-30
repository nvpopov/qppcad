#include <qppcad/tools/supercell/supercell.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void supercell_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  astate->tlog("Supercell tools::exec()");

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

  if (al->m_geom->get_DIM() != 3) {
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
                                       const int a_n, const int b_n, const int c_n) {

  if (al->m_geom->get_DIM() != 3) {
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

  al->m_parent_ws->add_item_to_ws(sc_al);
  sc_al->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);

  sc_al->m_geom->set_DIM(3);
  //sc_al->m_geom->cell.DIM = 3;

  //sc_al->set_parent_workspace(parent_ws);
  sc_al->begin_structure_change();

  if (al->m_role.get_value() == geom_view_role_e::r_uc) {
    sc_al->m_draw_img_atoms.set_value(false);
    sc_al->m_draw_img_bonds.set_value(false);
  }

  index sc_dim{a_n - 1 , b_n - 1 , c_n - 1};

  geom_view_tools_t::gen_supercell(al->m_geom.get(), sc_al->m_geom.get(),
                                   sc_dim, geom_view_role_e(al->m_role.get_value()));

  sc_al->m_pos.set_value(al->m_pos.get_value() + al->m_geom->cell.v[0] * 1.4f);
  sc_al->m_name.set_value(fmt::format("{}_sc_{}_{}_{}", al->m_name.get_value(), a_n, b_n, c_n));

  if (sc_al->m_geom->nat() > 200) {
    sc_al->m_draw_img_atoms.set_value(false);
    sc_al->m_draw_img_bonds.set_value(false);
  }

  sc_al->end_structure_change();

  //perform purification
  if (al->m_role.get_value() == geom_view_role_e::r_uc) {

    sc_al->m_tws_tr->do_action(act_lock);
    xgeometry<float, periodic_cell<float> > g(3); //intermediate xgeom
    g.set_format({"charge"},{basic_types::type_real});
    g.set_DIM(3);
    //g.cell.DIM = 3;
    g.cell.v[0] = sc_al->m_geom->cell.v[0];
    g.cell.v[1] = sc_al->m_geom->cell.v[1];
    g.cell.v[2] = sc_al->m_geom->cell.v[2];

    const float equality_dist = 0.01f;

    for (int i = 0; i < sc_al->m_geom->nat(); i++) {

      std::vector<tws_node_cnt_t<float>> res;
      sc_al->m_tws_tr->query_sphere(equality_dist, sc_al->m_geom->pos(i), res);
      float accum_chg = 0;

      bool need_to_add{true};
      for (auto &elem : res)
        if (elem.m_idx == index::D(sc_al->m_geom->get_DIM()).all(0)) {
          accum_chg += sc_al->m_geom->xfield<float>(xgeom_charge, elem.m_atm);
          if (i > elem.m_atm)
            need_to_add = false;
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

  sc_al->end_recording();

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

super_cell_widget_t::super_cell_widget_t (QWidget *parent) : ws_item_inline_tool_widget_t(parent) {

  app_state_t *astate = app_state_t::get_inst();
  m_sc_dim.set_value({1,1,1});
  m_sc_tool_mode.set_value(sc_tool_mode_default);

  //setFixedWidth(250);
  setWindowTitle(tr("Supercell generation"));

  m_dialog_lt = new QVBoxLayout;
  m_dialog_lt->setContentsMargins(0, 1, 0, 1);
  setLayout(m_dialog_lt);

  m_gb_rep_par = new qspoiler_widget_t(tr("Parameters"), nullptr, false, 6,
                                       astate->size_guide.obj_insp_table_w() + 2);
  m_gb_rep_par->set_top_border(false);
  m_gb_rep_par_lt = new QFormLayout;
  m_gb_rep_par->add_content_layout(m_gb_rep_par_lt);

  m_sp_rep = new qbinded_int3_input_t;
  m_sp_rep->set_min_max_step(1, 20, 1);
  m_sp_rep->bind_value(&m_sc_dim, this);
  m_sp_rep->m_updated_externally_event = true;
  m_sp_rep_label = new QLabel;
  m_sp_rep_label->setText(tr("Cell ratio"));

  m_tmode_inp = new qbinded_combobox_t;
  m_tmode_inp->addItem(tr("Supercell"));
  m_tmode_inp->addItem(tr("By Indices"));
  m_tmode_inp->bind_value(&m_sc_tool_mode, this);
  m_tmode_inp->m_updated_externally_event = true;

  m_gb_rep_par_lt->addRow(tr("Mode"), m_tmode_inp);
  m_gb_rep_par_lt->addRow(m_sp_rep_label, m_sp_rep);

  for (size_t i = 0; i < 3; i++) {

    m_boundaries_values[i].set_value({-1, 0, -1});
    m_boundaries[i] = new qbinded_int2b_input_t;
    m_boundaries[i]->set_min_max_step(-10, 10, 1);
    m_boundaries[i]->bind_value(&m_boundaries_values[i], this);
    m_boundaries[i]->m_updated_externally_event = true;
    m_boundaries_label[i] = new QLabel;
    m_boundaries_label[i]->setText(tr("Dim %1").arg(i));
    m_gb_rep_par_lt->addRow(m_boundaries_label[i], m_boundaries[i]);

  }

  qt_hlp::resize_form_lt_lbls(m_gb_rep_par_lt, astate->size_guide.obj_insp_lbl_w());

  m_dialog_lt->addWidget(m_gb_rep_par);
  mode_changed();

}

void super_cell_widget_t::make_super_cell(bool target_cam) {

  if (!m_src_gv) {
    return;
  }

  if (m_src_gv->m_geom->get_DIM() != 3) {
    return;
  }

  auto sc_dim = m_sc_dim.get_value();

  if (!m_dst_gv) {
    m_dst_gv = std::make_shared<geom_view_t>();
    m_dst_gv->m_name.set_value(m_src_gv->m_name.get_value() +
                               fmt::format("_sc_{}_{}_{}", sc_dim[0], sc_dim[1], sc_dim[2]));
    m_src_gv->m_parent_ws->add_item_to_ws(m_dst_gv);
  }

  auto diml = m_sc_tool_mode.get_value() == supercell_tool_mode_e::sc_tool_mode_default ? 3 : 0;

  m_dst_gv->m_parent_ws->begin_recording(hs_doc_rec_type_e::hs_doc_rec_as_new_epoch);

  m_dst_gv->m_geom->set_DIM(diml);
  m_dst_gv->begin_structure_change();
  m_dst_gv->m_geom->clear();

  switch (m_sc_tool_mode.get_value()) {

  case supercell_tool_mode_e::sc_tool_mode_default : {

    index sc_idx{sc_dim[0] - 1, sc_dim[1] - 1, sc_dim[2] - 1};
    geom_view_tools_t::gen_supercell(m_src_gv->m_geom.get(),
                                     m_dst_gv->m_geom.get(), sc_idx);
    m_dst_gv->m_pos.set_value(m_src_gv->m_pos.get_value() + m_src_gv->m_geom->cell.v[0] * 1.4f);

    break;

  }

  case supercell_tool_mode_e::sc_tool_mode_by_idx : {

    bool can_apply{true};

    for (auto &elem : m_boundaries_values) {
      auto elem_val = elem.get_value();
      if (elem_val[0] > elem_val[1])
        can_apply = true;
    }

    auto b0 = m_boundaries_values[0].get_value();
    auto b1 = m_boundaries_values[0].get_value();
    auto b2 = m_boundaries_values[0].get_value();

    if (can_apply)
      geom_view_tools_t::gen_ncells_ex(m_src_gv->m_geom.get(), m_dst_gv->m_geom.get(),
                                       b0[0], b0[1], b1[0], b1[1], b2[0], b1[1]);
    break;

  }

  }

  // apply naive heuristics that depends on number of atoms
  if (m_dst_gv->m_geom->nat() < 800) {
    m_dst_gv->m_draw_img_atoms.set_value(true);
    m_dst_gv->m_draw_img_bonds.set_value(true);
    m_dst_gv->m_render_style.set_value(geom_view_render_style_e::ball_and_stick);
  } else {
    m_dst_gv->m_draw_img_atoms.set_value(false);
    m_dst_gv->m_draw_img_bonds.set_value(false);
    m_dst_gv->m_render_style.set_value(geom_view_render_style_e::billboards);
  }

  if (target_cam)
    m_dst_gv->apply_target_view(cam_tv_e::tv_b);

  m_dst_gv->end_structure_change();
  m_dst_gv->m_parent_ws->end_recording();

}

bool super_cell_widget_t::restore_cam_on_cancel() {
  return true;
}

void super_cell_widget_t::on_apply() {

}

void super_cell_widget_t::on_cancel() {

  if (m_dst_gv) {
    //m_dst_gv->hs_delete(true, false);

    auto dst_ws = m_dst_gv->m_parent_ws;
    if (dst_ws) {
      dst_ws->m_ws_items.hs_remove_child_force_rawptr(m_dst_gv.get());
    }

    m_dst_gv = nullptr;
    m_src_gv = nullptr;

  }

}

void super_cell_widget_t::bind_item(ws_item_t *item) {

  ws_item_inline_tool_widget_t::bind_item(item);

  if (m_src->get_type() == geom_view_t::get_type_static()) {

    m_src_gv = m_src->cast_as<geom_view_t>();
    m_dst_gv = nullptr;

  } else {

    m_src_gv = nullptr;
    m_dst_gv = nullptr;

  }

  //setup default cell dim
  m_sc_dim.set_value({1, 1, 1});

  for (int i = 0; i < 3; i++) {
    //  m_sc_dim[i] = 1;
    m_boundaries_values[i].set_value({-1, 0, 0});
  }

  m_sp_rep->unbind_value();
  m_sp_rep->bind_value(&m_sc_dim, this);

  make_super_cell();

}

void super_cell_widget_t::updated_externally(uint32_t update_reason) {
  mode_changed();
  make_super_cell(false);
}

void super_cell_widget_t::mode_changed() {

  qt_hlp::form_lt_ctrl_visibility(m_sc_tool_mode.get_value() == sc_tool_mode_default,
                                  m_gb_rep_par_lt, m_tmode_inp, m_sp_rep_label, m_sp_rep);

  for (size_t i = 0; i < 3; i++)
    qt_hlp::form_lt_ctrl_visibility(m_sc_tool_mode.get_value() == sc_tool_mode_by_idx,
                                    m_gb_rep_par_lt, m_tmode_inp,
                                    m_boundaries_label[i], m_boundaries[i]);

}


bool super_cell_can_apply_helper_t::can_apply(ws_item_t *item) {

  if (!item) return false;
  auto as_gv = item->cast_as<geom_view_t>();
  if (!as_gv) return false;
  return as_gv->m_geom->get_DIM() == 3;

}
