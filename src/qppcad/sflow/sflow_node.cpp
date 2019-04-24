#include <qppcad/sflow/sflow_node.hpp>

using namespace qpp;
using namespace qpp::cad;


sflow_node_t::sflow_node_t() {

}

bool sflow_node_t::validate_inputs() {

  if (m_inp_types.size() != m_inps.size()) return false;

  for (size_t i = 0; i < m_inp_types.size(); i++)
    if (m_inps[i] && m_inps[i]->get_param_meta() != m_inp_types[i].m_type)
      return false;

  return true;

}

bool sflow_node_t::validate_inplace_parameters() {

  if (m_inplace_types.empty()) return false;

  m_inplace_parameters.resize(m_inplace_types.size());

  for (size_t i = 0; i < m_inplace_types.size(); i++) {

      switch (m_inplace_types[i].m_type) {

        case sflow_parameter_e::sfpar_int : {
            m_inplace_parameters[i] = std::make_shared<sflow_parameter_int_t>();
            break;
          }

        case sflow_parameter_e::sfpar_float : {
            m_inplace_parameters[i] = std::make_shared<sflow_parameter_float_t>();
            break;
          }

        case sflow_parameter_e::sfpar_bool : {
            m_inplace_parameters[i] = std::make_shared<sflow_parameter_bool_t>();
            break;
          }

        default :  {
            return false;
            break;
          }

        }

    }

  return true;

}

bool sflow_node_t::execute() {

  if (!validate_inputs()) {
      fmt::print(std::cout, "!validate_inputs() for {}\n", m_node_name);
      return false;
    }

  m_outs.resize(m_out_types.size(), nullptr);

  return execute_ex();

}

bool sflow_node_t::execute_ex() {
  return true;
}

bool sflow_node_t::is_single_node() {
  return false;
}
