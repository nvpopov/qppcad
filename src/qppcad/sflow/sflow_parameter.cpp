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

  //copy dim
  _clone->m_value.DIM = m_value.DIM;

  //copy cell vectors
  for (size_t i = 0; i < m_value.DIM; i++)
    _clone->m_value.cell.v[i] = m_value.cell.v[i];

  //copy xgeom headers
  std::vector<std::string> fn;
  std::vector<basic_types> ft;
  m_value.get_format(fn, ft);
  _clone->m_value.set_format(fn, ft);

  //copy data
  for (size_t i = 0; i < m_value.nat(); i++) {
      _clone->m_value.add(m_value.atom_name(i), m_value.pos(i));
      std::vector<datum> v;
      m_value.get_fields(i, v);
      _clone->m_value.set_fields(i,v);
    }

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

sflow_parameter_e sflow_parameter_v3f_t::get_param_meta() {
  return sflow_parameter_e::sfpar_v3f;
}

std::shared_ptr<sflow_parameter_t> sflow_parameter_v3f_t::clone() {
  auto _clone = std::make_shared<sflow_parameter_v3f_t>();
  _clone->m_value = m_value;
  return _clone;
}
