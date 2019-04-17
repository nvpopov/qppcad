#include <qppcad/sflow/sflow_node.hpp>

using namespace qpp;
using namespace qpp::cad;


bool sflow_node_t::execute() {

  if (!validate_inputs()) {
      fmt::print(std::cout, "!validate_inputs() for {}\n", m_node_name);
      return false;
    }

  m_outs.resize(m_out_types.size(), nullptr);
  return true;

}

sflow_node_t::sflow_node_t() {

}

bool sflow_node_t::validate_inputs() {

  if (m_inp_types.size() != m_inps.size()) return false;

  for (size_t i = 0; i < m_inp_types.size(); i++)
    if (m_inps[i] && m_inps[i]->get_param_meta() != m_inp_types[i].m_type)
      return false;

  return true;

}
