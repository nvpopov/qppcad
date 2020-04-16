#include <qppcad/ui/add_new_ws_item_widget.hpp>
#include <qppcad/core/app_state.hpp>

#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>
#include <qppcad/ws_item/pgf_producer/pgf_producer.hpp>
#include <qppcad/ws_item/cube_primitive/cube_primitive.hpp>
#include <qppcad/ws_item/arrow_primitive/arrow_primitive.hpp>
#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/ws_item/arrow_array/arrow_array.hpp>
#include <qppcad/ws_item/py_note_book/py_note_book.hpp>
#include <qppcad/ws_item/traj_hl/traj_hl.hpp>
#include <qppcad/ui/qt_helpers.hpp>

#include <symm/shoenflis.hpp>

using namespace qpp;
using namespace qpp::cad;

const int label_width = 90;

add_new_ws_item_widget_t::add_new_ws_item_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  setWindowTitle(tr("Add new item to workspace"));
  setFixedWidth(670);
  setFixedHeight(500);

  descr_list.reserve(11);

  descr_list.push_back(tr("Description for geom_view_t [DIM=0D]"));
  descr_list.push_back(tr("Description for geom_view_t [DIM=1D]"));
  descr_list.push_back(tr("Description for geom_view_t [DIM=2D]"));
  descr_list.push_back(tr("Description for geom_view_t [DIM=3D]"));
  descr_list.push_back(tr("Description for psg_view_t"));
  descr_list.push_back(tr("Description for pgf_producer_t"));
  descr_list.push_back(tr("Description for cube_primitive_t"));
  descr_list.push_back(tr("Description for arrow_primitive_t"));
  descr_list.push_back(tr("Description for node_book_t"));
  descr_list.push_back(tr("Description for arrow_array_t"));
  descr_list.push_back(tr("Description for py_note_book_t"));
  descr_list.push_back(tr("Description for traj_hl_t"));

  m_main_lt = new QVBoxLayout;
  m_data_lt = new QHBoxLayout;
  m_btn_lt = new QHBoxLayout;
  m_sub_data_lt = new QVBoxLayout;

  m_ok_btn = new QPushButton(tr("Ok"));
  m_ok_btn->setFixedWidth(astate->size_guide.common_button_fixed_w());
  m_cancel_btn = new QPushButton(tr("Cancel"));
  m_cancel_btn->setFixedWidth(astate->size_guide.common_button_fixed_w());

  m_btn_lt->addStretch();
  m_btn_lt->addWidget(m_ok_btn, 0, Qt::AlignCenter);
  m_btn_lt->addWidget(m_cancel_btn, 0, Qt::AlignCenter);
  m_btn_lt->addStretch();

  m_gb_type_descr = new qspoiler_widget_t(tr("Type description"), nullptr, false);
  //gb_type_descr->setMinimumWidth(240);
  //gb_type_descr->setMaximumHeight(130);
  m_gb_type_descr_lt = new QVBoxLayout;
  m_gb_type_descr->add_content_layout(m_gb_type_descr_lt);

  m_type_descr_lbl = new QLabel;
  m_type_descr_lbl->setObjectName("add_new_ws_item_type_desc");
  m_type_descr_lbl->setWordWrap(true);
  m_gb_type_descr_lt->addWidget(m_type_descr_lbl);
  m_gb_type_descr_lt->addStretch();

  m_gb_ctor = new qspoiler_widget_t(tr("Constructible types"), nullptr, false, 6, 280);
  //gb_ctor->setMinimumWidth(240);
  m_gb_ctor_lt = new QVBoxLayout;
  m_gb_ctor->add_content_layout(m_gb_ctor_lt);

  m_rb_ctor_geom0d = new QRadioButton(tr("geometry [DIM=0D]"));
  connect(m_rb_ctor_geom0d,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_geom0d_checked);

  m_rb_ctor_geom1d = new QRadioButton(tr("geometry [DIM=1D]"));
  m_rb_ctor_geom1d->setEnabled(false);
  connect(m_rb_ctor_geom1d,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_geom1d_checked);

  m_rb_ctor_geom2d = new QRadioButton(tr("geometry [DIM=2D]"));
  m_rb_ctor_geom2d->setEnabled(false);
  connect(m_rb_ctor_geom2d,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_geom2d_checked);

  m_rb_ctor_geom3d = new QRadioButton(tr("geometry [DIM=3D]"));
  connect(m_rb_ctor_geom3d,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_geom3d_checked);

  m_rb_ctor_psg = new QRadioButton(tr("point symmetry group"));
  connect(m_rb_ctor_psg,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_psg_checked);

  m_rb_ctor_pgf_prod = new QRadioButton(tr("pgf producer"));
  connect(m_rb_ctor_pgf_prod,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_pgf_prod_checked);

  m_rb_ctor_cube = new QRadioButton(tr("cube(parallelepiped) primitive"));
  connect(m_rb_ctor_cube,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_cube_checked);

  m_rb_ctor_arrow = new QRadioButton(tr("arrow primitive"));
  connect(m_rb_ctor_arrow,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_arrow_checked);

  m_rb_ctor_node_book = new QRadioButton(tr("node book"));
  connect(m_rb_ctor_node_book,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_node_book_checked);

  m_rb_ctor_arrow_array = new QRadioButton(tr("arrow array"));
  connect(m_rb_ctor_arrow_array,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_arrow_array_checked);

  m_rb_ctor_py_note_book = new QRadioButton(tr("python note book"));
  connect(m_rb_ctor_py_note_book,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_py_note_book_checked);

  m_rb_ctor_traj_hl = new QRadioButton(tr("trajectory highlighter"));
  connect(m_rb_ctor_traj_hl,
          &QRadioButton::toggled,
          this,
          &add_new_ws_item_widget_t::react_gb_ctor_traj_hl_checked);

  m_rb_ctor_geom0d->setChecked(true);

  m_gb_ctor_lt->addWidget(m_rb_ctor_geom0d);
  m_gb_ctor_lt->addWidget(m_rb_ctor_geom1d);
  m_gb_ctor_lt->addWidget(m_rb_ctor_geom2d);
  m_gb_ctor_lt->addWidget(m_rb_ctor_geom3d);
  m_gb_ctor_lt->addWidget(m_rb_ctor_psg);
  m_gb_ctor_lt->addWidget(m_rb_ctor_pgf_prod);
  m_gb_ctor_lt->addWidget(m_rb_ctor_cube);
  m_gb_ctor_lt->addWidget(m_rb_ctor_arrow);
  m_gb_ctor_lt->addWidget(m_rb_ctor_node_book);
  m_gb_ctor_lt->addWidget(m_rb_ctor_arrow_array);
  m_gb_ctor_lt->addWidget(m_rb_ctor_py_note_book);
  m_gb_ctor_lt->addWidget(m_rb_ctor_traj_hl);

  auto label_setup = [](QLabel *label, bool visible = false) {
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label->setMinimumWidth(label_width);
    label->setMinimumHeight(28);
    label->setVisible(visible);
  };

  m_type_param_ag = new QComboBox(this);
  m_type_param_ag->setFixedWidth(astate->size_guide.obj_insp_combo_max_w());
  m_type_param_ag->setVisible(false);
  m_type_param_ag_lbl = new QLabel(tr("Point group"));
  m_type_param_ag_lbl->setVisible(false);
  label_setup(m_type_param_ag_lbl);

  m_type_cell_ctor_mode = new QComboBox(this);
  m_type_cell_ctor_mode->setFixedWidth(astate->size_guide.obj_insp_combo_max_w());
  m_type_cell_ctor_mode->addItem(tr("Cubic cell"));
  m_type_cell_ctor_mode->addItem(tr("Ortho[3 len]"));
  m_type_cell_ctor_mode->addItem(tr("3 angles & 3 len"));
  m_type_cell_ctor_mode->addItem(tr("3 vecs"));
  m_type_cell_ctor_mode->setVisible(false);
  m_type_cell_ctor_mode_lbl = new QLabel(tr("Cell mode"));
  label_setup(m_type_cell_ctor_mode_lbl);

  m_type_cell_ctor_cubic_size = new QDoubleSpinBox(this);
  m_type_cell_ctor_cubic_size->setFixedWidth(astate->size_guide.obj_insp_combo_max_w());
  m_type_cell_ctor_cubic_size->setRange(0.00001, 100);
  m_type_cell_ctor_cubic_size->setValue(5);
  m_type_cell_ctor_cubic_size->setSuffix(astate->m_spatial_suffix);
  m_type_cell_ctor_cubic_size->setVisible(false);
  m_type_cell_ctor_cubic_size->setAlignment(Qt::AlignCenter);
  m_type_cell_ctor_cubic_size->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_type_cell_ctor_cubic_size->setFixedWidth(astate->size_guide.obj_insp_ctrl_max_w());
  m_type_cell_ctor_cubic_size->setLocale(QLocale::C);
  m_type_cell_ctor_cubic_size_lbl = new QLabel(tr("Cube size"));
  label_setup(m_type_cell_ctor_cubic_size_lbl);

  m_type_cell_ctor_ortho = new qbinded_float3_input_t(this);
  m_type_cell_ctor_ortho->set_min_max_step_dec(0.000, 50.0, 0.01);
  m_type_cell_ctor_ortho->set_default_suffix();
  m_type_cell_ctor_ortho->bind_value(&m_ortho_g3d_cell);
  m_type_cell_ctor_ortho->setVisible(false);
  m_type_cell_ctor_ortho_lbl = new QLabel(tr("Ortho vecs."));
  label_setup(m_type_cell_ctor_ortho_lbl);

  m_type_cell_ctor_a3 = new qbinded_float3_input_t(this);
  m_type_cell_ctor_a3->set_min_max_step_dec(0.000, 180.0, 0.01);
  m_type_cell_ctor_a3->set_suffix(astate->m_degree_suffix);
  m_type_cell_ctor_a3->bind_value(&m_a3_g3d_cell);
  m_type_cell_ctor_a3->setVisible(false);
  m_type_cell_ctor_a3_lbl = new QLabel(tr("Angles"));
  label_setup(m_type_cell_ctor_a3_lbl);

  m_type_cell_ctor_l3 = new qbinded_float3_input_t(this);
  m_type_cell_ctor_l3->set_min_max_step_dec(0.000, 180.0, 0.01);
  m_type_cell_ctor_l3->set_default_suffix();
  m_type_cell_ctor_l3->bind_value(&m_l3_g3d_cell);
  m_type_cell_ctor_l3->setVisible(false);

  m_type_cell_ctor_l3_lbl = new QLabel(tr("Lengths"));
  label_setup(m_type_cell_ctor_l3_lbl);

  m_type_cell_ctor_a = new qbinded_float3_input_t(this);
  m_type_cell_ctor_a->set_min_max_step_dec(-50.00, 50.0, 0.01);
  m_type_cell_ctor_a->set_default_suffix();
  m_type_cell_ctor_a->bind_value(&m_a_g3d_cell);
  m_type_cell_ctor_a->setVisible(false);
  m_type_cell_ctor_a_lbl = new QLabel(tr("a"));
  label_setup(m_type_cell_ctor_a_lbl);

  m_type_cell_ctor_b = new qbinded_float3_input_t(this);
  m_type_cell_ctor_b->set_min_max_step_dec(-50.00, 50.0, 0.01);
  m_type_cell_ctor_b->set_default_suffix();
  m_type_cell_ctor_b->bind_value(&m_b_g3d_cell);
  m_type_cell_ctor_b->setVisible(false);
  m_type_cell_ctor_b_lbl = new QLabel(tr("b"));
  label_setup(m_type_cell_ctor_b_lbl);

  m_type_cell_ctor_c = new qbinded_float3_input_t(this);
  m_type_cell_ctor_c->set_min_max_step_dec(-50.00, 50.0, 0.01);
  m_type_cell_ctor_c->set_default_suffix();
  m_type_cell_ctor_c->bind_value(&m_c_g3d_cell);
  m_type_cell_ctor_c->setVisible(false);
  m_type_cell_ctor_c_lbl = new QLabel(tr("c"));
  label_setup(m_type_cell_ctor_c_lbl);

  connect(m_type_cell_ctor_mode,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &add_new_ws_item_widget_t::geom3d_ctor_changed);

  m_type_cell_ctor_mode->setCurrentIndex(0);

  //construct ag_labels
  for (int n = 1; n < 11; n++) {
      std::vector<std::string> nl = shnfl<float>::groups_by_order(n);
      for (auto &elem : nl) m_type_param_ag->addItem(QString::fromStdString(elem));
    }
  //end of construct ag_labels

  m_gb_type_param = new qspoiler_widget_t(tr("Type parameters"), nullptr, false);
  //gb_type_param->setMinimumWidth(360);
  m_gb_type_param_lt = new QFormLayout;
  m_gb_type_param->add_content_layout(m_gb_type_param_lt);

  m_type_param_name = new QLineEdit(this);
  m_type_param_name->setFixedWidth(astate->size_guide.obj_insp_combo_max_w());
  m_type_param_name_lbl = new QLabel("Name", this);
  label_setup(m_type_param_name_lbl, true);

  m_gb_type_param_lt->addRow(m_type_param_name_lbl, m_type_param_name);

  auto cur_ws = astate->ws_mgr->get_cur_ws();
  if (cur_ws) m_type_param_name->setText(tr("new_item_%1").arg(cur_ws->m_ws_items.size()));

  m_main_lt->addLayout(m_data_lt);
  m_data_lt->addWidget(m_gb_ctor);
  m_data_lt->addLayout(m_sub_data_lt);
  m_sub_data_lt->addWidget(m_gb_type_descr);
  m_sub_data_lt->addWidget(m_gb_type_param);
  m_main_lt->addStretch();
  m_main_lt->addLayout(m_btn_lt);

  setLayout(m_main_lt);

  connect(m_ok_btn, &QPushButton::clicked,
          this, &add_new_ws_item_widget_t::ok_button_clicked);

  connect(m_cancel_btn, &QPushButton::clicked,
          this, &add_new_ws_item_widget_t::cancel_button_clicked);

}

void add_new_ws_item_widget_t::set_cell_ctors_visibility(bool show) {

  qt_hlp::form_lt_ctrl_visibility(show, m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_cubic_size_lbl, m_type_cell_ctor_cubic_size);

  qt_hlp::form_lt_ctrl_visibility(show, m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_ortho_lbl, m_type_cell_ctor_ortho);

  qt_hlp::form_lt_ctrl_visibility(show, m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_l3_lbl, m_type_cell_ctor_l3);

  qt_hlp::form_lt_ctrl_visibility(show, m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_a3_lbl, m_type_cell_ctor_a3);

  qt_hlp::form_lt_ctrl_visibility(show, m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_a_lbl, m_type_cell_ctor_a);

  qt_hlp::form_lt_ctrl_visibility(show, m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_b_lbl, m_type_cell_ctor_b);

  qt_hlp::form_lt_ctrl_visibility(show, m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_c_lbl, m_type_cell_ctor_c);

}

void add_new_ws_item_widget_t::control_top_type_parameters_visibility() {

  //hide all
  set_cell_ctors_visibility(false);
  qt_hlp::form_lt_ctrl_visibility(false,
                                      m_gb_type_param_lt,
                                      m_type_param_name,
                                      m_type_param_ag_lbl, m_type_param_ag);

  qt_hlp::form_lt_ctrl_visibility(m_rb_ctor_geom3d->isChecked(),
                                      m_gb_type_param_lt,
                                      m_type_param_name,
                                      m_type_cell_ctor_mode_lbl, m_type_cell_ctor_mode);

  qt_hlp::form_lt_ctrl_visibility(m_rb_ctor_psg->isChecked(),
                                      m_gb_type_param_lt,
                                      m_type_param_name,
                                      m_type_param_ag_lbl, m_type_param_ag);

}

void add_new_ws_item_widget_t::ok_button_clicked() {

  if (m_type_param_name->text() == "") {
      int ret = QMessageBox::warning(this,
                                     tr("Missing name!"),
                                     tr("Name is empty! Do something!"));
    } else {

      app_state_t *astate = app_state_t::get_inst();

      if (m_rb_ctor_geom0d->isChecked()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(geom_view_t::get_type_static());
          auto nt_gv = nt->cast_as<geom_view_t>();
          if (!nt_gv) return;
          nt_gv->m_name = m_type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }

      if (m_rb_ctor_geom3d->isChecked()) {

          cell_construct_mode ccm =
              static_cast<cell_construct_mode>(m_type_cell_ctor_mode->currentIndex());

          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(geom_view_t::get_type_static());
          auto nt_gv = nt->cast_as<geom_view_t>();
          if (!nt_gv) return;
          nt_gv->m_tws_tr->do_action(act_lock | act_clear_all);
          nt_gv->m_geom->DIM = 3;
          nt_gv->m_geom->cell.DIM = 3;

          switch (ccm) {

            case cell_construct_mode::construct_cubic : {
                vector3<float> cubic_a{float(m_type_cell_ctor_cubic_size->value()), 0, 0};
                vector3<float> cubic_b{0, float(m_type_cell_ctor_cubic_size->value()), 0};
                vector3<float> cubic_c{0, 0, float(m_type_cell_ctor_cubic_size->value())};
                nt_gv->m_geom->cell = periodic_cell<float>(cubic_a,
                                                           cubic_b,
                                                           cubic_c);
                break;
              }

            case cell_construct_mode::construct_ortho : {
                vector3<float> ortho_a{m_ortho_g3d_cell[0], 0, 0};
                vector3<float> ortho_b{0, m_ortho_g3d_cell[1], 0};
                vector3<float> ortho_c{0, 0, m_ortho_g3d_cell[2]};

                nt_gv->m_geom->cell = periodic_cell<float>(ortho_a,
                                                           ortho_b,
                                                           ortho_c);
                break;
              }

            case cell_construct_mode::construct_3angles_3len : {
                nt_gv->m_geom->cell = periodic_cell<float>(m_l3_g3d_cell[0],
                                                           m_l3_g3d_cell[1],
                                                           m_l3_g3d_cell[2],
                                                           m_a3_g3d_cell[0],
                                                           m_a3_g3d_cell[1],
                                                           m_a3_g3d_cell[2]);
                break;
              }

            case cell_construct_mode::construct_9el : {
                nt_gv->m_geom->cell = periodic_cell<float>(m_a_g3d_cell,
                                                           m_b_g3d_cell,
                                                           m_c_g3d_cell);
                break;
              }

            }

          nt_gv->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
          nt_gv->m_tws_tr->do_action(act_rebuild_ntable);
          nt_gv->m_name = m_type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);

        }

      if (m_rb_ctor_psg->isChecked()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(psg_view_t::get_type_static());
          auto nt_psg = nt->cast_as<psg_view_t>();
          if (!nt_psg) return;
          auto ag = shnfl<float>::group(m_type_param_ag->currentText().toStdString());
          nt_psg->m_ag =
              std::make_shared<array_group<matrix3<float>>>(ag);
          nt_psg->m_name = m_type_param_name->text().toStdString();
          nt_psg->update_view();
          cur_ws->add_item_to_ws(nt);
        }

      if (m_rb_ctor_pgf_prod->isChecked()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(pgf_producer_t::get_type_static());
          auto nt_pgfp = nt->cast_as<pgf_producer_t>();
          if (!nt_pgfp) return;
          nt_pgfp->m_name = m_type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }

      if (m_rb_ctor_cube->isChecked()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(cube_primitive_t::get_type_static());
          auto nt_cp = nt->cast_as<cube_primitive_t>();
          if (!nt_cp) return;
          nt_cp->m_name = m_type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }

      if (m_rb_ctor_arrow->isChecked()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(arrow_primitive_t::get_type_static());
          auto nt_ap = nt->cast_as<arrow_primitive_t>();
          if (!nt_ap) return;
          nt_ap->m_name = m_type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }

      if (m_rb_ctor_node_book->isChecked()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(node_book_t::get_type_static());
          auto nb_ap = nt->cast_as<node_book_t>();
          if (!nb_ap) return;
          nb_ap->m_name = m_type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);
        }

      if (m_rb_ctor_arrow_array->isChecked()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(arrow_array_t::get_type_static());
          auto aa_ap = nt->cast_as<arrow_array_t>();
          if (!aa_ap) return;
          aa_ap->m_name = m_type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);

          auto cur_it = cur_ws->get_sel_sp();
          if (cur_it && cur_it->get_type() == geom_view_t::get_type_static()) {
              aa_ap->m_src = cur_it;
              aa_ap->updated_externally(ws_item_updf_generic);
            }
        }

      if (m_rb_ctor_py_note_book->isChecked()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto nt =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(py_note_book_t::get_type_static());
          auto aa_ap = nt->cast_as<py_note_book_t>();
          if (!aa_ap) return;
          aa_ap->m_name = m_type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(nt);

        }

      if (m_rb_ctor_traj_hl->isChecked()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          if (!cur_ws) return;
          auto th =
              astate->ws_mgr->m_bhv_mgr->fbr_ws_item_by_type(traj_hl_t::get_type_static());
          auto aa_ap = th->cast_as<traj_hl_t>();
          if (!aa_ap) return;
          aa_ap->m_name = m_type_param_name->text().toStdString();
          cur_ws->add_item_to_ws(th);

        }

      accept();

    }

}

void add_new_ws_item_widget_t::cancel_button_clicked() {

  reject();

}

void add_new_ws_item_widget_t::react_gb_ctor_geom0d_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      m_type_descr_lbl->setText(descr_list[0]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_geom1d_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[1]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_geom2d_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[2]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_geom3d_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      m_type_cell_ctor_mode->setCurrentIndex(0);
      geom3d_ctor_changed(0);
      m_type_descr_lbl->setText(descr_list[3]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_psg_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[4]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_pgf_prod_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[5]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_cube_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[6]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_arrow_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[7]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_node_book_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[8]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_arrow_array_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[9]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_py_note_book_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[10]);
    }

}

void add_new_ws_item_widget_t::react_gb_ctor_traj_hl_checked(bool checked) {

  control_top_type_parameters_visibility();

  if (checked) {
      set_cell_ctors_visibility(false);
      m_type_descr_lbl->setText(descr_list[11]);
    }

}

void add_new_ws_item_widget_t::geom3d_ctor_changed(int index) {

  cell_construct_mode ccm = static_cast<cell_construct_mode>(index);

  qt_hlp::form_lt_ctrl_visibility(ccm == cell_construct_mode::construct_cubic,
                                      m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_cubic_size_lbl, m_type_cell_ctor_cubic_size);

  qt_hlp::form_lt_ctrl_visibility(ccm == cell_construct_mode::construct_ortho,
                                      m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_ortho_lbl, m_type_cell_ctor_ortho);

  qt_hlp::form_lt_ctrl_visibility(ccm == cell_construct_mode::construct_3angles_3len,
                                      m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_a3_lbl, m_type_cell_ctor_a3);

  qt_hlp::form_lt_ctrl_visibility(ccm == cell_construct_mode::construct_3angles_3len,
                                      m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_l3_lbl, m_type_cell_ctor_l3);

  qt_hlp::form_lt_ctrl_visibility(ccm == cell_construct_mode::construct_9el,
                                      m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_c_lbl, m_type_cell_ctor_c);

  qt_hlp::form_lt_ctrl_visibility(ccm == cell_construct_mode::construct_9el,
                                      m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_b_lbl, m_type_cell_ctor_b);

  qt_hlp::form_lt_ctrl_visibility(ccm == cell_construct_mode::construct_9el,
                                      m_gb_type_param_lt,
                                      m_type_cell_ctor_mode,
                                      m_type_cell_ctor_a_lbl, m_type_cell_ctor_a);

}
