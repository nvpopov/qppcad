#include <qppcad/ui/qbinded_inputs.hpp>
#include <qppcad/app_state.hpp>
#include <QMouseEvent>
#include <QColorDialog>

using namespace qpp;
using namespace qpp::cad;

void qbinded_checkbox::load_value_ex() {

  if (m_binded_value) {
      if (*m_binded_value) setCheckState(Qt::Checked);
      else setCheckState(Qt::Unchecked);
    }

}

qbinded_checkbox::qbinded_checkbox(QWidget *parent) : QCheckBox (parent){
  connect(this, SIGNAL(stateChanged(int)),
          this, SLOT(check_state_changed(int)));
}

void qbinded_checkbox::check_state_changed(int state) {
  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = state == Qt::Checked;
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }
}

void qbinded_float_spinbox::load_value_ex() {
  if (m_binded_value) {
      setValue(*m_binded_value);
    }
}

void qbinded_float_spinbox::set_min_max_step(double new_min,
                                             double new_max,
                                             double new_step,
                                             int decimals) {
  setRange(new_min, new_max);
  setSingleStep(new_step);
  setDecimals(decimals);
}

qbinded_float_spinbox::qbinded_float_spinbox(QWidget *parent) : QDoubleSpinBox (parent) {

  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  connect(this, SIGNAL(valueChanged(double)),
          this, SLOT(value_changed(double)));
  setMaximumWidth(180);

}

void qbinded_float_spinbox::set_suffix(QString &new_suffix) {
  setSuffix(new_suffix);
}

void qbinded_float_spinbox::set_default_suffix() {
  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);
}

void qbinded_float_spinbox::value_changed(double d) {
  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = float(d);
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }
}

void qbinded_combobox::load_value_ex() {
  if (m_binded_value) {
      setCurrentIndex(*m_binded_value);
    }
}

qbinded_combobox::qbinded_combobox(QWidget *parent) : QComboBox (parent) {

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(value_changed(int)));
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(180);

}

void qbinded_combobox::value_changed(int i) {
  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = i;
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }
}

void qbinded_int3_input::load_value_ex() {
  if (m_binded_value) {
      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);
      sb_z->setValue((*m_binded_value)[2]);
    }
}

void qbinded_int3_input::set_min_max_step(int min, int max, int step) {
  sb_x->setMinimum(min);
  sb_x->setMaximum(max);
  sb_x->setSingleStep(step);

  sb_y->setMinimum(min);
  sb_y->setMaximum(max);
  sb_y->setSingleStep(step);

  sb_z->setMinimum(min);
  sb_z->setMaximum(max);
  sb_z->setSingleStep(step);
}

qbinded_int3_input::qbinded_int3_input(QWidget *parent) : QWidget (parent) {

  widget_layout = new QHBoxLayout;
  setLayout(widget_layout);
  sb_x = new QSpinBox(this);
  sb_y = new QSpinBox(this);
  sb_z = new QSpinBox(this);
  widget_layout->addWidget(sb_x);
  widget_layout->addWidget(sb_y);
  widget_layout->addWidget(sb_z);

  connect(sb_x, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this, &qbinded_int3_input::spinbox_value_changed);
  connect(sb_y, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this, &qbinded_int3_input::spinbox_value_changed);
  connect(sb_z, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this, &qbinded_int3_input::spinbox_value_changed);

}

void qbinded_int3_input::spinbox_value_changed(int newval) {
  //we ignore newval here and make batch update based on inputs
  if (m_binded_value && !m_ignore_state_change) {
      (*m_binded_value)[0] = sb_x->value();
      (*m_binded_value)[1] = sb_y->value();
      (*m_binded_value)[2] = sb_z->value();
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }
}

void qbinded_float3_input::load_value_ex() {

  if (m_binded_value) {
      m_ignore_state_change = true;
      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);
      sb_z->setValue((*m_binded_value)[2]);
      m_ignore_state_change = false;
    }

}

void qbinded_float3_input::set_min_max_step(double min, double max, double step) {
  sb_x->setMinimum(min);
  sb_x->setMaximum(max);
  sb_x->setSingleStep(step);

  sb_y->setMinimum(min);
  sb_y->setMaximum(max);
  sb_y->setSingleStep(step);

  sb_z->setMinimum(min);
  sb_z->setMaximum(max);
  sb_z->setSingleStep(step);
}

void qbinded_float3_input::set_suffix(QString &new_suffix) {
  sb_x->setSuffix(new_suffix);
  sb_y->setSuffix(new_suffix);
  sb_z->setSuffix(new_suffix);
}

void qbinded_float3_input::set_empty_suffix() {
  sb_x->setSuffix("");
  sb_y->setSuffix("");
  sb_z->setSuffix("");
}

void qbinded_float3_input::set_default_suffix() {
  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);
}

void qbinded_float3_input::set_sb_lbls(const QString _lb_x,
                                       const QString _lb_y,
                                       const QString _lb_z) {

  sb_x_lbl->setText(_lb_x);
  sb_y_lbl->setText(_lb_y);
  sb_z_lbl->setText(_lb_z);

}

void qbinded_float3_input::set_sb_lbls_def() {
  set_sb_lbls("X", "Y", "Z");
}

qbinded_float3_input::qbinded_float3_input(QWidget *parent) : QWidget(parent) {

  widget_layout = new QVBoxLayout;
  widget_layout->setContentsMargins(2,2,2,2);
  setLayout(widget_layout);

  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);

  sb_x = new QDoubleSpinBox(this);
  sb_x->setMaximumWidth(100);

  sb_y = new QDoubleSpinBox(this);
  sb_y->setMaximumWidth(100);

  sb_z = new QDoubleSpinBox(this);
  sb_z->setMaximumWidth(100);

  sb_x_lt = new QHBoxLayout;
  sb_y_lt = new QHBoxLayout;
  sb_z_lt = new QHBoxLayout;

  sb_x_lbl = new QLabel;
  sb_x_lbl->setMinimumWidth(20);
  sb_x_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  sb_y_lbl = new QLabel;
  sb_y_lbl->setMinimumWidth(20);
  sb_y_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  sb_z_lbl = new QLabel;
  sb_z_lbl->setMinimumWidth(20);
  sb_z_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  sb_x_lt->addWidget(sb_x_lbl);
  sb_x_lt->addWidget(sb_x);
  sb_x_lt->addStretch(1);

  sb_y_lt->addWidget(sb_y_lbl);
  sb_y_lt->addWidget(sb_y);
  sb_y_lt->addStretch(1);

  sb_z_lt->addWidget(sb_z_lbl);
  sb_z_lt->addWidget(sb_z);
  sb_z_lt->addStretch(1);

  widget_layout->addLayout(sb_x_lt);
  widget_layout->addLayout(sb_y_lt);
  widget_layout->addLayout(sb_z_lt);

  set_sb_lbls_def();
  //widget_layout->addStretch(1);

  connect(sb_x,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float3_input::spinbox_value_changed);

  connect(sb_y,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float3_input::spinbox_value_changed);

  connect(sb_z,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float3_input::spinbox_value_changed);

}

void qbinded_float3_input::spinbox_value_changed(double newval) {

  if (m_binded_value && !m_ignore_state_change) {
      (*m_binded_value)[0] = sb_x->value();
      (*m_binded_value)[1] = sb_y->value();
      (*m_binded_value)[2] = sb_z->value();
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_color3_input::load_value_ex() {

  if (m_binded_value) {
      m_stored_color.setRgbF((*m_binded_value)[0],(*m_binded_value)[1], (*m_binded_value)[2]);
      QPalette pal = palette();
      pal.setColor(QPalette::Background, m_stored_color);
      setAutoFillBackground(true);
      setPalette(pal);
    }

}

void qbinded_color3_input::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
      const QColor color = QColorDialog::getColor(m_stored_color, this, "Select Color");
      if (color.isValid() && m_binded_value) {
          (*m_binded_value)[0] = color.redF();
          (*m_binded_value)[1] = color.greenF();
          (*m_binded_value)[2] = color.blueF();
          load_value();
          on_value_changed();
          app_state_t::get_inst()->make_viewport_dirty();
        }
    }

}

qbinded_color3_input::qbinded_color3_input(QWidget *parent) : QWidget (parent) {

  setMaximumWidth(21);
  setMaximumHeight(21);

}

void qbinded_int_spinbox::load_value_ex() {
  if (m_binded_value) {
      setValue(*m_binded_value);
    }
}

void qbinded_int_spinbox::set_min_max_step(int new_min,
                                           int new_max,
                                           int new_step) {
  setRange(new_min, new_max);
}

qbinded_int_spinbox::qbinded_int_spinbox(QWidget *parent) : QSpinBox(parent) {

  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  connect(this, SIGNAL(valueChanged(int)),
          this, SLOT(value_changed(int)));
  setMaximumWidth(180);

}

void qbinded_int_spinbox::set_suffix(QString &new_suffix) {
  setSuffix(new_suffix);
}

void qbinded_int_spinbox::set_default_suffix() {
  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);
}

void qbinded_int_spinbox::value_changed(int value) {
  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = value;
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }
}

qbinded_ws_item_combobox::qbinded_ws_item_combobox(QWidget *parent) {

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(value_changed(int)));

}

void qbinded_ws_item_combobox::bind_value(std::shared_ptr<ws_item_t> *_binded_value,
                                          ws_item_t *item_to_bind) {
  m_binded_ws_item_p = _binded_value;
  m_binded_ws_item = item_to_bind;
  m_binded_ws = item_to_bind->m_parent_ws;
  load_value();

}

void qbinded_ws_item_combobox::load_value() {

  rebuild_variants();

  if (!m_binded_ws_item_p->get()) {
      setCurrentIndex(0);
      return;
    }

  if (!m_binded_ws) {
      setCurrentIndex(0);
      return;
    }

  for (size_t i = 0; i < count(); i++) {
      QString name_in_checkbox = itemText(i);
      auto itm = m_binded_ws->get_by_name(name_in_checkbox.toStdString());
      if (itm && itm->get_type() == m_type_id) {
          setCurrentIndex(i);
          return;
        }
    }

}

void qbinded_ws_item_combobox::unbind_value() {

  m_binded_ws_item_p = nullptr;
  m_binded_ws_item = nullptr;
  m_binded_ws = nullptr;

}

void qbinded_ws_item_combobox::rebuild_variants() {

  blockSignals(true);
  clear();
  addItem(tr("None"));

  if (m_binded_ws) {
      for (auto item : m_binded_ws->m_ws_items)
        if (item->get_type() == m_type_id) {
            addItem(QString::fromStdString(item->m_name));
          }
    }

  blockSignals(false);

}

void qbinded_ws_item_combobox::value_changed(int i) {

  if (!m_binded_ws) {
      setCurrentIndex(0);
      return;
    }

  if (i == 0) {
      *m_binded_ws_item_p = nullptr;
      return;
    }

  std::string name_to_find = currentText().toStdString();
  auto itm = m_binded_ws->get_by_name(name_to_find);

  if (itm && itm->get_type() == m_type_id) {
      *m_binded_ws_item_p = itm;
    }

}
