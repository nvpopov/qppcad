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

  app_state_t* astate = app_state_t::get_inst();
  //astate->log("qbinded_checkbox::checkStateSet()");

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = state == Qt::Checked;
      astate->make_viewport_dirty();
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

void qbinded_float_spinbox::set_min_max_step(double new_min, double new_max,
                                             double new_step) {
  setMinimum(new_min);
  setMaximum(new_max);
  setSingleStep(new_step);
}

qbinded_float_spinbox::qbinded_float_spinbox(QWidget *parent) : QDoubleSpinBox (parent) {
   connect(this, SIGNAL(valueChanged(double)), this, SLOT(value_changed(double)));
}

void qbinded_float_spinbox::value_changed(double d) {

  app_state_t* astate = app_state_t::get_inst();
  //astate->log("qbinded_checkbox::checkStateSet()");

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = float(d);
      astate->make_viewport_dirty();
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
  app_state_t* astate = app_state_t::get_inst();
  //astate->log("qbinded_checkbox::checkStateSet()");

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = i;
      astate->make_viewport_dirty();
    }
}
