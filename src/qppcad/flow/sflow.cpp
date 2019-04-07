#include <qppcad/flow/sflow.hpp>

using namespace qpp;
using namespace qpp::cad;

void sflow_context_t::add_node(std::shared_ptr<sflow_node_t> node) {

  if (node) {
      node->m_idx_lookup = m_nodes.size();
      m_nodes.push_back(node);
    }

}

sflow_error_e sflow_context_t::connect_node(std::shared_ptr<sflow_node_t> node1,
                                            std::shared_ptr<sflow_node_t> node2,
                                            size_t pin1,
                                            size_t pin2) {

  //check output pin
  bool pin1_valid = pin1 < node1->m_out_types.size();
  if (!pin1_valid) return sflow_error_e::pin1_invalid;

  //check input pin
  bool pin2_valid = pin2 < node2->m_inp_types.size();
  if (!pin2_valid) return sflow_error_e::pin2_invalid;

  //check pin types
  bool pin_types_eq = node1->m_out_types[pin1].m_type == node2->m_inp_types[pin2].m_type;
  if (!pin_types_eq) return sflow_error_e::types_pin1_not_eq_pin2;

  //check multipin output
  bool node2_is_connected = node2->m_inp_types[pin2].m_total_con == 0;
  if (!node2_is_connected) return sflow_error_e::too_many_inputs;
  node2->m_inp_types[pin2].m_total_con +=1;
  node2->m_is_outer = false;

  if (pin1_valid && pin2_valid && pin_types_eq) {
      sflow_connectivity_data_t _con_data;
      _con_data.m_inp_node = node2;
      _con_data.m_out_node = node1;
      _con_data.m_inp_socket = pin1;
      _con_data.m_out_socket = pin2;
      m_connectivity.push_back(std::move(_con_data));
    }

  return sflow_error_e::no_error;

}

void sflow_context_t::clear_context() {

  clear_outer_nodes();
  clear_connectivity();

}

void sflow_context_t::clear_outer_nodes() {

  for (auto &elem : m_nodes) {
      elem->m_is_outer = true;
      elem->m_total_gain = 0;
    }

}

void sflow_context_t::clear_connectivity() {

  m_connectivity.clear();

}

void sflow_context_t::compile_flow() {

}

void sflow_context_t::execute() {

  fmt::print(std::cout, "start executing \n");

  for (auto &node : m_nodes)
    if (node->m_is_outer) {
        execute_traverse(node.get());
      }

  fmt::print(std::cout, "end executing \n");

}

void sflow_context_t::execute_traverse(sflow_node_t *cur_node) {

  fmt::print(std::cout, "exec_traverse : {}\n", cur_node->m_node_name);

  for (auto &cd : m_connectivity) {
      if (cd.m_out_node.get() == cur_node) {
          propagate_data(&cd);
          execute_traverse(cd.m_inp_node.get());
        }
    }

}

sflow_error_e sflow_context_t::propagate_data(sflow_connectivity_data_t *cd) {

  return sflow_error_e::propagate_succes;

}

sflow_context_t::sflow_context_t() {

}

sflow_node_t::sflow_node_t() {

}

sflow_parameter_e sflow_parameter_t::get_param_meta() {

  return sflow_parameter_e::sfpar_none;

}

sflow_soi_node_t::sflow_soi_node_t() : sflow_node_t () {

  m_out_types.resize(1);
  m_out_types[0] = {sflow_parameter_e::sfpar_int, 0};

}

sflow_sgi_node_t::sflow_sgi_node_t() : sflow_node_t () {

  m_out_types.resize(1);
  m_out_types[0] = {sflow_parameter_e::sfpar_int, 0};

  m_inp_types.resize(1);
  m_inp_types[0] = {sflow_parameter_e::sfpar_int, 0};

}

sflow_sii_node_t::sflow_sii_node_t() : sflow_node_t () {

  m_inp_types.resize(1);
  m_inp_types[0] = {sflow_parameter_e::sfpar_int, 0};

}
