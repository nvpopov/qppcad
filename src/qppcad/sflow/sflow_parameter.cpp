#include <qppcad/sflow/sflow_parameter.hpp>

using namespace qpp;
using namespace qpp::cad;

sflow_parameter_t::sflow_parameter_t() {

}

sflow_parameter_e sflow_parameter_t::get_param_meta() {
  return sflow_parameter_e::sfpar_none;
}

sflow_parameter_e sflow_parameter_int_t::get_param_meta() {
  return sflow_parameter_e::sfpar_int;
}

std::shared_ptr<sflow_parameter_t> sflow_parameter_int_t::clone() {
  auto _clone = std::make_shared<sflow_parameter_int_t>();
  _clone->m_value = m_value;
  return _clone;
}

sflow_parameter_e sflow_parameter_float_t::get_param_meta() {
  return sflow_parameter_e::sfpar_float;
}

std::shared_ptr<sflow_parameter_t> sflow_parameter_float_t::clone() {
  auto _clone = std::make_shared<sflow_parameter_float_t>();
  _clone->m_value = m_value;
  return _clone;
}

sflow_parameter_e sflow_parameter_xgeom_t::get_param_meta() {
  return sflow_parameter_e::sfpar_xgeom;
}

std::shared_ptr<sflow_parameter_t> sflow_parameter_xgeom_t::clone() {
  auto _clone = std::make_shared<sflow_parameter_xgeom_t>();
  return _clone;
}

sflow_parameter_e sflow_parameter_bool_t::get_param_meta() {
  return sflow_parameter_e::sfpar_bool;
}

std::shared_ptr<sflow_parameter_t> sflow_parameter_bool_t::clone() {
  auto _clone = std::make_shared<sflow_parameter_bool_t>();
  _clone->m_value = m_value;
  return _clone;
}
