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
  astate->log("qbinded_checkbox::checkStateSet()");

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = state == Qt::Checked;
      astate->make_viewport_dirty();
    }

}
