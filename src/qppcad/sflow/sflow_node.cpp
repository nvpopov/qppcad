#include <qppcad/sflow/sflow_node.hpp>

using namespace qpp;
using namespace qpp::cad;

sflow_node_t::sflow_node_t() {

}

bool sflow_node_t::has_ipls() {

  return !m_ipl_schema.empty();

}

void sflow_node_t::explicit_create_ipl() {

  fill_data_vec(m_ipl_schema, m_ipl);

}

bool sflow_node_t::execute() {

  fill_data_vec(m_inp_schema, m_inps);
  fill_data_vec(m_out_schema, m_outs);
  fill_data_vec(m_ipl_schema, m_ipl);

  validate_data_vec(m_inp_schema, m_inps);
  validate_data_vec(m_out_schema, m_outs);
  validate_data_vec(m_ipl_schema, m_ipl);

  m_outs.resize(m_out_schema.size(), nullptr);
  return execute_ex();

}

bool sflow_node_t::execute_ex() {

  return true;

}

bool sflow_node_t::is_single_node() {

  return false;

}
