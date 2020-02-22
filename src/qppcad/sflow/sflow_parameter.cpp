#include <qppcad/sflow/sflow_parameter.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_parameter_t::sf_parameter_t() {

}

sf_parameter_e sf_parameter_t::get_param_meta() {
  return sf_parameter_e::sfpar_none;
}

sf_parameter_e sf_parameter_int_t::get_param_meta() {
  return sf_parameter_e::sfpar_int;
}

std::shared_ptr<sf_parameter_t> sf_parameter_int_t::clone() {

  auto _clone = std::make_shared<sf_parameter_int_t>();
  _clone->m_value = m_value;
  return std::move(_clone);

}

sf_parameter_e sf_parameter_float_t::get_param_meta() {
  return sf_parameter_e::sfpar_float;
}

std::shared_ptr<sf_parameter_t> sf_parameter_float_t::clone() {

  auto _clone = std::make_shared<sf_parameter_float_t>();
  _clone->m_value = m_value;
  return std::move(_clone);

}

sf_parameter_e sf_parameter_xgeom_t::get_param_meta() {
  return sf_parameter_e::sfpar_xgeom;
}

std::shared_ptr<sf_parameter_t> sf_parameter_xgeom_t::clone() {

  auto _clone = std::make_shared<sf_parameter_xgeom_t>();
  m_value.clone(_clone->m_value);
  return std::move(_clone);

}

sf_parameter_e sf_parameter_bool_t::get_param_meta() {
  return sf_parameter_e::sfpar_bool;
}

std::shared_ptr<sf_parameter_t> sf_parameter_bool_t::clone() {

  auto _clone = std::make_shared<sf_parameter_bool_t>();
  _clone->m_value = m_value;
  return std::move(_clone);

}

sf_parameter_e sf_parameter_v3f_t::get_param_meta() {
  return sf_parameter_e::sfpar_v3f;
}

std::shared_ptr<sf_parameter_t> sf_parameter_v3f_t::clone() {

  auto _clone = std::make_shared<sf_parameter_v3f_t>();
  _clone->m_value = m_value;
  return std::move(_clone);

}
