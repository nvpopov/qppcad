#include <qppcad/ui/add_new_ws_item_widget.hpp>

using namespace qpp;
using namespace qpp::cad;


add_new_ws_item_widget_t::add_new_ws_item_widget_t() {

  setWindowTitle(tr("Add new item to workspace"));

  descr_list.reserve(6);
  descr_list.push_back(tr("Description for geom_view_t [DIM=0D]"));
  descr_list.push_back(tr("Description for geom_view_t [DIM=1D]"));
  descr_list.push_back(tr("Description for geom_view_t [DIM=2D]"));
  descr_list.push_back(tr("Description for geom_view_t [DIM=3D]"));
  descr_list.push_back(tr("Description for psg_view_t"));
  descr_list.push_back(tr("Description for pgf_producer_t"));

  main_lt = new QVBoxLayout;
  data_lt = new QHBoxLayout;
  button_lt = new QHBoxLayout;

  ok_button = new QPushButton(tr("Ok"));
  cancel_button = new QPushButton(tr("Cancel"));

  button_lt->addStretch();
  button_lt->addWidget(ok_button, 0, Qt::AlignCenter);
  button_lt->addWidget(cancel_button, 0, Qt::AlignCenter);
  button_lt->addStretch();

  gb_ctor = new QGroupBox(tr("Constructible types"));
  gb_ctor_lt = new QVBoxLayout;
  gb_ctor->setLayout(gb_ctor_lt);

  gb_ctor_geom0d = new QRadioButton(tr("geom_view_t [DIM=0D]"));
  gb_ctor_geom0d->setChecked(true);

  gb_ctor_geom1d = new QRadioButton(tr("geom_view_t [DIM=1D]"));
  gb_ctor_geom1d->setEnabled(false);

  gb_ctor_geom2d = new QRadioButton(tr("geom_view_t [DIM=2D]"));
  gb_ctor_geom2d->setEnabled(false);

  gb_ctor_geom3d = new QRadioButton(tr("geom_view_t [DIM=3D]"));

  gb_ctor_psg = new QRadioButton(tr("psg_view_t"));
  gb_ctor_pgf_prod = new QRadioButton(tr("pgf_producer_t"));

  gb_ctor_lt->addWidget(gb_ctor_geom0d);
  gb_ctor_lt->addWidget(gb_ctor_geom1d);
  gb_ctor_lt->addWidget(gb_ctor_geom2d);
  gb_ctor_lt->addWidget(gb_ctor_geom3d);
  gb_ctor_lt->addWidget(gb_ctor_psg);
  gb_ctor_lt->addWidget(gb_ctor_pgf_prod);

  gb_type_descr = new QGroupBox(tr("Type description"));
  gb_type_descr->setMinimumWidth(190);
  gb_type_descr_lt = new QVBoxLayout;
  gb_type_descr->setLayout(gb_type_descr_lt);

  type_descr_lbl = new QLabel;
  type_descr_lbl->setWordWrap(true);
  gb_type_descr_lt->addWidget(type_descr_lbl);

  gb_type_param = new QGroupBox(tr("Type parameters"));
  gb_type_param->setMinimumWidth(240);
  gb_type_param_lt = new QFormLayout;
  gb_type_param->setLayout(gb_type_param_lt);

  type_param_name = new QLineEdit();
  gb_type_param_lt->addRow(tr("Name"), type_param_name);

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
    }
}

void add_new_ws_item_widget_t::cancel_button_clicked() {
  reject();
}
