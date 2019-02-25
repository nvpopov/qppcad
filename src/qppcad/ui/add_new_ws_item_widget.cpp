#include <qppcad/ui/add_new_ws_item_widget.hpp>
#include <qppcad/app_state.hpp>

#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>
#include <qppcad/ws_item/pgf_producer/pgf_producer.hpp>
#include <qppcad/ws_item/cube_primitive/cube_primitive.hpp>
#include <qppcad/ws_item/arrow_primitive/arrow_primitive.hpp>

#include <symm/shoenflis.hpp>

using namespace qpp;
using namespace qpp::cad;

const int label_width = 100;

add_new_ws_item_widget_t::add_new_ws_item_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  setWindowTitle(tr("Add new item to workspace"));

  descr_list.reserve(8);
  descr_list.push_back(tr("Description for geom_view_t [DIM=0D]"));
  descr_list.push_back(tr("Description for geom_view_t [DIM=1D]"));
  descr_list.push_back(tr("Description for geom_view_t [DIM=2D]"));
  descr_list.push_back(tr("Description for geom_view_t [DIM=3D]"));
  descr_list.push_back(tr("Description for psg_view_t"));
  descr_list.push_back(tr("Description for pgf_producer_t"));
  descr_list.push_back(tr("Description for cube_primitive_t"));
  descr_list.push_back(tr("Description for arrow_primitive_t"));

  main_lt = new QVBoxLayout;
  data_lt = new QHBoxLayout;
  button_lt = new QHBoxLayout;

  ok_button = new QPushButton(tr("Ok"));
  cancel_button = new QPushButton(tr("Cancel"));

  button_lt->addStretch();
  button_lt->addWidget(ok_button, 0, Qt::AlignCenter);
  button_lt->addWidget(cancel_button, 0, Qt::AlignCenter);
  button_lt->addStretch();

  gb_type_descr = new QGroupBox(tr("Type description"));
  gb_type_descr->setMinimumWidth(220);
  gb_type_descr_lt = new QVBoxLayout;
  gb_type_descr->setLayout(gb_type_descr_lt);

  type_descr_lbl = new QLabel;
  type_descr_lbl->setWordWrap(true);
  gb_type_descr_lt->addWidget(type_descr_lbl);
  gb_type_descr_lt->addStretch();

  gb_ctor = new QGroupBox(tr("Constructible types"));
  gb_ctor_lt = new QVBoxLayout;
  gb_ctor->setLayout(gb_ctor_lt);

  gb_ctor_geom0d = new QRadioButton(tr("geometry [DIM=0D]"));
  connect(gb_ctor_geom0d, &QRadioButton::toggled,
          this, &add_new_ws_item_widget_t::react_gb_ctor_geom0d_checked);

  gb_ctor_geom1d = new QRadioButton(tr("geometry [DIM=1D]"));
  gb_ctor_geom1d->setEnabled(false);
  connect(gb_ctor_geom1d, &QRadioButton::toggled,
          this, &add_new_ws_item_widget_t::react_gb_ctor_geom1d_checked);

  gb_ctor_geom2d = new QRadioButton(tr("geometry [DIM=2D]"));
  gb_ctor_geom2d->setEnabled(false);
  connect(gb_ctor_geom2d, &QRadioButton::toggled,
          this, &add_new_ws_item_widget_t::react_gb_ctor_geom2d_checked);

  gb_ctor_geom3d = new QRadioButton(tr("geometry [DIM=3D]"));
  connect(gb_ctor_geom3d, &QRadioButton::toggled,
          this, &add_new_ws_item_widget_t::react_gb_ctor_geom3d_checked);

  gb_ctor_psg = new QRadioButton(tr("point symmetry group"));
  connect(gb_ctor_psg, &QRadioButton::toggled,
          this, &add_new_ws_item_widget_t::react_gb_ctor_psg_checked);

  gb_ctor_pgf_prod = new QRadioButton(tr("pgf producer"));
  connect(gb_ctor_pgf_prod, &QRadioButton::toggled,
          this, &add_new_ws_item_widget_t::react_gb_ctor_pgf_prod_checked);

  gb_ctor_cube = new QRadioButton(tr("cube(parallelepiped) primitive"));
  connect(gb_ctor_cube, &QRadioButton::toggled,
          this, &add_new_ws_item_widget_t::react_gb_ctor_cube_checked);

  gb_ctor_arrow = new QRadioButton(tr("arrow primitive"));
  connect(gb_ctor_arrow, &QRadioButton::toggled,
          this, &add_new_ws_item_widget_t::react_gb_ctor_arrow_checked);

  gb_ctor_geom0d->setChecked(true);

  gb_ctor_lt->addWidget(gb_ctor_geom0d);
  gb_ctor_lt->addWidget(gb_ctor_geom1d);
  gb_ctor_lt->addWidget(gb_ctor_geom2d);
  gb_ctor_lt->addWidget(gb_ctor_geom3d);
  gb_ctor_lt->addWidget(gb_ctor_psg);
  gb_ctor_lt->addWidget(gb_ctor_pgf_prod);
  gb_ctor_lt->addWidget(gb_ctor_cube);
  gb_ctor_lt->addWidget(gb_ctor_arrow);

  type_param_ag_lbl = new QLabel(tr("Point group"));
  type_param_ag_lbl->setMinimumWidth(label_width);
  type_param_ag = new QComboBox;
  type_param_ag_lbl->setVisible(false);
  type_param_ag->setVisible(false);

  //construct ag_labels
  for (int n = 1; n < 11; n++) {
      std::vector<std::string> nl = shnfl<float>::groups_by_order(n);
      for (auto &elem : nl) type_param_ag->addItem(QString::fromStdString(elem));
    }
  //end of construct ag_labels

  gb_type_param = new QGroupBox(tr("Type parameters"));
  gb_type_param->setMinimumWidth(300);
  gb_type_param_lt = new QFormLayout;
  gb_type_param->setLayout(gb_type_param_lt);

  type_param_name = new QLineEdit();
  type_param_name_lbl = new QLabel("Name");
  type_param_name_lbl->setMinimumWidth(label_width);

  gb_type_param_lt->addRow(type_param_name_lbl, type_param_name);
  gb_type_param_lt->addRow(type_param_ag_lbl, type_param_ag);

  auto cur_ws = astate->ws_manager->get_cur_ws();
  if (cur_ws) {
      type_param_name->setText(tr("new_item_%1").arg(cur_ws->m_ws_items.size()));
    }

  main_lt->addLayout(data_lt);
  data_lt->addWidget(gb_ctor);
  data_lt->addWidget(gb_type_descr);
  data_lt->addWidget(gb_type_param);
  main_lt->addStretch();
  main_lt->addLayout(button_lt);

  setLayout(main_lt);
  setMinimumWidth(300);
  setMinimumHeight(300);

  connect(ok_button, &QPushButton::clicked,
          this, &add_new_ws_item_widget_t::ok_button_clicked);

  connect(cancel_button, &QPushButton::clicked,
          this, &add_new_ws_item_widget_t::cancel_button_clicked);

}

void add_new_ws_item_widget_t::ok_button_clicked() {

  if (type_param_name->text() == "") {
      int ret = QMessageBox::warning(this,
                                     tr("Missing name!"),
                                     tr("Name is empty! Do something!"));
    } else {

      accept();

      app_state_t *astate = app_state_t::get_inst();

      //

      if (gb_ctor_geom0d->isChecked()) {
          auto cur_ws = astate->ws_manager->get_cur_ws();
          if (!cur_ws) return;
          auto nt = astate->ws_manager->m_bhv_mgr->fabric_by_type(geom_view_t::get_type_static());
          auto nt_gv = nt->cast_as<geom_view_t>();
          if (!nt_gv) return;
          nt_gv->m_name = type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }

      if (gb_ctor_geom3d->isChecked()) {
          auto cur_ws = astate->ws_manager->get_cur_ws();
          if (!cur_ws) return;
          auto nt = astate->ws_manager->m_bhv_mgr->fabric_by_type(geom_view_t::get_type_static());
          auto nt_gv = nt->cast_as<geom_view_t>();
          if (!nt_gv) return;
          nt_gv->m_tws_tr->do_action(act_lock | act_clear_all);
          nt_gv->m_geom->DIM = 3;
          nt_gv->m_geom->cell.DIM = 3;
          nt_gv->m_geom->cell.v[0] = vector3<float>(5, 0, 0);
          nt_gv->m_geom->cell.v[1] = vector3<float>(0, 5, 0);
          nt_gv->m_geom->cell.v[2] = vector3<float>(0, 0, 5);
          nt_gv->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
          nt_gv->m_tws_tr->do_action(act_rebuild_ntable);
          nt_gv->m_name = type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }

      if (gb_ctor_psg->isChecked()) {
          auto cur_ws = astate->ws_manager->get_cur_ws();
          if (!cur_ws) return;
          auto nt = astate->ws_manager->m_bhv_mgr->fabric_by_type(psg_view_t::get_type_static());
          auto nt_psg = nt->cast_as<psg_view_t>();
          if (!nt_psg) return;
          auto ag = shnfl<float>::group(type_param_ag->currentText().toStdString());
          nt_psg->m_ag =
              std::make_shared<array_group<matrix3<float>>>(ag);
          nt_psg->m_name = type_param_name->text().toStdString();
          nt_psg->update_view();
          cur_ws->add_item_to_ws(nt);
        }

      if (gb_ctor_pgf_prod->isChecked()) {
          auto cur_ws = astate->ws_manager->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_manager->m_bhv_mgr->fabric_by_type(pgf_producer_t::get_type_static());
          auto nt_pgfp = nt->cast_as<pgf_producer_t>();
          if (!nt_pgfp) return;
          nt_pgfp->m_name = type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }

      if (gb_ctor_cube->isChecked()) {
          auto cur_ws = astate->ws_manager->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_manager->m_bhv_mgr->fabric_by_type(cube_primitive_t::get_type_static());
          auto nt_cp = nt->cast_as<cube_primitive_t>();
          if (!nt_cp) return;
          nt_cp->m_name = type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }

      if (gb_ctor_arrow->isChecked()) {
          auto cur_ws = astate->ws_manager->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_manager->m_bhv_mgr->fabric_by_type(arrow_primitive_t::get_type_static());
          auto nt_ap = nt->cast_as<arrow_primitive_t>();
          if (!nt_ap) return;
          nt_ap->m_name = type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }


    }
}

void add_new_ws_item_widget_t::cancel_button_clicked() {

  reject();

}

void add_new_ws_item_widget_t::react_gb_ctor_geom0d_checked(bool checked) {

  if (checked) {
      type_descr_lbl->setText(descr_list[0]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_geom1d_checked(bool checked) {

  if (checked) {
      type_descr_lbl->setText(descr_list[1]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_geom2d_checked(bool checked) {

  if (checked) {
      type_descr_lbl->setText(descr_list[2]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_geom3d_checked(bool checked) {

  if (checked) {
      type_descr_lbl->setText(descr_list[3]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_psg_checked(bool checked) {

  type_param_ag_lbl->setVisible(checked);
  type_param_ag->setVisible(checked);

  if (checked) {
      type_descr_lbl->setText(descr_list[4]);
    } else {

    }
}

void add_new_ws_item_widget_t::react_gb_ctor_pgf_prod_checked(bool checked) {

  if (checked) {
      type_descr_lbl->setText(descr_list[5]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_cube_checked(bool checked) {

  if (checked) {
      type_descr_lbl->setText(descr_list[6]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_arrow_checked(bool checked) {

  if (checked) {
      type_descr_lbl->setText(descr_list[7]);
    }

}
