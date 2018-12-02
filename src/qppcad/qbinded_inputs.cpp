#include <qppcad/qbinded_inputs.hpp>
#include <qppcad/app_state.hpp>
using namespace qpp;
using namespace qpp::cad;


void qbinded_checkbox::bind_value(bool *_binded_value) {
  m_binded_value = _binded_value;
  m_ignore_state_change = true;
  load_value();
  m_ignore_state_change = false;
}

void qbinded_checkbox::load_value() {
  if (m_binded_value) {
      if (*m_binded_value) setCheckState(Qt::Checked);
      else setCheckState(Qt::Unchecked);
    }
}

void qbinded_checkbox::unbind_value() {
  m_binded_value = nullptr;
}

qbinded_checkbox::qbinded_checkbox(QWidget *parent) : QCheckBox (parent){
  connect(this, SIGNAL(stateChanged(int)), this, SLOT(check_state_changed(int)));
}

void qbinded_checkbox::check_state_changed(int state) {
  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = state == Qt::Checked;
      app_state_t::get_inst()->make_viewport_dirty();
    }
}

qbinded_spinbox::qbinded_spinbox(QWidget *parent) : QSpinBox (parent) {

}

//float spinbox

void qbinded_float_spinbox::bind_value(float *_binded_value) {
  m_binded_value = _binded_value;
  m_ignore_state_change = true;
  load_value();
  m_ignore_state_change = false;
}

void qbinded_float_spinbox::load_value() {
  if (m_binded_value) {
      setValue(*m_binded_value);
    }
}

void qbinded_float_spinbox::unbind_value() {
  m_binded_value = nullptr;
}

void qbinded_float_spinbox::set_min_max_step(double new_min,
                                             double new_max,
                                             double new_step) {
  setMinimum(new_min);
  setMaximum(new_max);
  setSingleStep(new_step);
}

qbinded_float_spinbox::qbinded_float_spinbox(QWidget *parent) : QDoubleSpinBox (parent) {
  connect(this, SIGNAL(valueChanged(double)), this, SLOT(value_changed(double)));
}

void qbinded_float_spinbox::value_changed(double d) {
  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = float(d);
      app_state_t::get_inst()->make_viewport_dirty();
    }
}

void qbinded_combobox::bind_value(int *_binded_value) {
  m_binded_value = _binded_value;
  m_ignore_state_change = true;
  load_value();
  m_ignore_state_change = false;
}

void qbinded_combobox::load_value() {
  if (m_binded_value) {
      setCurrentIndex(*m_binded_value);
    }
}

void qbinded_combobox::unbind_value() {
  m_binded_value = nullptr;
}

qbinded_combobox::qbinded_combobox(QWidget *parent) : QComboBox (parent) {
  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(value_changed(int)));
}

void qbinded_combobox::value_changed(int i) {
  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = i;
      app_state_t::get_inst()->make_viewport_dirty();
    }
}

void qbinded_int3_input::bind_value(vector3<int> *_binded_value) {
  m_binded_value = _binded_value;
  m_ignore_state_change = true;
  load_value();
  m_ignore_state_change = false;
}

void qbinded_int3_input::load_value() {
  if (m_binded_value) {
      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);
      sb_z->setValue((*m_binded_value)[2]);
    }
}

void qbinded_int3_input::unbind_value() {
  m_binded_value = nullptr;
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
      app_state_t::get_inst()->make_viewport_dirty();
    }
}

void qbinded_float3_input::bind_value(vector3<float> *_binded_value) {
  m_binded_value = _binded_value;
  m_ignore_state_change = true;
  load_value();
  m_ignore_state_change = false;
}

void qbinded_float3_input::load_value() {

  if (m_binded_value) {
      m_ignore_state_change = true;
      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);
      sb_z->setValue((*m_binded_value)[2]);
      m_ignore_state_change = false;
    }

}

void qbinded_float3_input::unbind_value() {
  m_binded_value = nullptr;
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

qbinded_float3_input::qbinded_float3_input(QWidget *parent) : QWidget(parent) {

  widget_layout = new QHBoxLayout;
  setLayout(widget_layout);

  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);

  sb_x = new QDoubleSpinBox(this);
  sb_x->setMinimumWidth(30);

  sb_y = new QDoubleSpinBox(this);
  sb_y->setMinimumWidth(30);

  sb_z = new QDoubleSpinBox(this);
  sb_z->setMinimumWidth(30);

  widget_layout->addWidget(sb_x, 1);
  widget_layout->addWidget(sb_y, 1);
  widget_layout->addWidget(sb_z, 1);
  //widget_layout->addStretch(1);

  connect(sb_x, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this, &qbinded_float3_input::spinbox_value_changed);
  connect(sb_y, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this, &qbinded_float3_input::spinbox_value_changed);
  connect(sb_z, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this, &qbinded_float3_input::spinbox_value_changed);

}

void qbinded_float3_input::spinbox_value_changed(double newval) {
  if (m_binded_value && !m_ignore_state_change) {
      (*m_binded_value)[0] = sb_x->value();
      (*m_binded_value)[1] = sb_y->value();
      (*m_binded_value)[2] = sb_z->value();
      app_state_t::get_inst()->make_viewport_dirty();
    }
}
