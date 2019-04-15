#include <qppcad/flow/sflow.hpp>

using namespace qpp;
using namespace qpp::cad;

void sflow_context_t::add_node(std::shared_ptr<sflow_node_t> node) {

  if (node) {
      node->m_idx_lookup = m_nodes.size();
      m_nodes.push_back(node);
    }

}

sflow_status_e sflow_context_t::connect_node(std::shared_ptr<sflow_node_t> node1,
                                            std::shared_ptr<sflow_node_t> node2,
                                            size_t pin1,
                                            size_t pin2) {

  //check output pin
  bool pin1_valid = pin1 < node1->m_out_types.size();
  if (!pin1_valid) return sflow_status_e::pin1_invalid;

  //check input pin
  bool pin2_valid = pin2 < node2->m_inp_types.size();
  if (!pin2_valid) return sflow_status_e::pin2_invalid;

  //check pin types
  bool pin_types_eq = node1->m_out_types[pin1].m_type == node2->m_inp_types[pin2].m_type;
  if (!pin_types_eq) return sflow_status_e::types_pin1_not_eq_pin2;

  //check multipin output
  bool node2_is_connected = node2->m_inp_types[pin2].m_total_con == 0;
  if (!node2_is_connected) return sflow_status_e::too_many_inputs;
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

  return sflow_status_e::no_error;

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

  fmt::print(std::cout, "\nstart executing \n");

  for (auto &node : m_nodes)
    if (node->m_is_outer) {
        execute_traverse(node.get(), nullptr);
      }

  fmt::print(std::cout, "end executing \n");

}

void sflow_context_t::execute_traverse(sflow_node_t *cur_node,
                                       sflow_node_t *prev_node,
                                       bool debug_print) {

  fmt::print(std::cout, "exec_traverse : {} -> {}\n",
             prev_node ? prev_node->m_node_name : "None",
             cur_node->m_node_name);

  if (!cur_node) return;

  bool node_exec_res = cur_node->execute();
  fmt::print(std::cout, "node {} exec result {}\n", cur_node->m_node_name, node_exec_res);
  if (!node_exec_res) return;

  for (size_t i = 0; i < m_connectivity.size(); i++) {
      if (m_connectivity[i].m_out_node.get() == cur_node) {
          //last parameter - by reference, otherwise - copy
          bool last_element = i + 1 == m_connectivity.size();
          auto pg_res = propagate_data(&m_connectivity[i], !last_element);
          if (pg_res == sflow_status_e::propagate_succes)
            execute_traverse(m_connectivity[i].m_inp_node.get(), cur_node);
        }
    }

}

sflow_status_e sflow_context_t::propagate_data(sflow_connectivity_data_t *cd,
                                              bool copy_par) {

  if (cd && cd->m_inp_node && cd->m_out_node) {

      fmt::print(std::cout, "propagate_data from={} to={} copy_data?={} "
                            "out_sck={} in_sck={} out_t={} in_t={} ",
                 cd->m_out_node->m_node_name,
                 cd->m_inp_node->m_node_name,
                 copy_par,
                 *(cd->m_out_socket),
                 *(cd->m_inp_socket),
                 cd->m_out_node->m_out_types[*(cd->m_out_socket)].m_type,
                 cd->m_inp_node->m_inp_types[*(cd->m_inp_socket)].m_type
          );

      //check that inputs have space for fun
      if (cd->m_inp_node->m_inps.size() != cd->m_inp_node->m_inp_types.size())
        cd->m_inp_node->m_inps.resize(cd->m_inp_node->m_inp_types.size(), nullptr);

      //check that out is valid
      if (*cd->m_out_socket < cd->m_out_node->m_outs.size() &&
          cd->m_out_node->m_outs[*cd->m_out_socket]) {
          cd->m_inp_node->m_inps[*cd->m_inp_socket] =
              cd->m_out_node->m_outs[*cd->m_out_socket]->clone();
          fmt::print(std::cout, "succes\n");
          return sflow_status_e::propagate_succes;
        } else {
          fmt::print(std::cout, "error\n");
          return sflow_status_e::propagate_error;
        }

    } else return sflow_status_e::propagate_error;

  return sflow_status_e::propagate_succes;

}

sflow_context_t::sflow_context_t() {

}

bool sflow_node_t::validate_inputs() {

  if (m_inp_types.size() != m_inps.size()) return false;

  for (size_t i = 0; i < m_inp_types.size(); i++)
    if (m_inps[i] && m_inps[i]->get_param_meta() != m_inp_types[i].m_type)
      return false;

  return true;

}

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

sflow_parameter_e sflow_parameter_t::get_param_meta() {

  return sflow_parameter_e::sfpar_none;

}

sflow_parameter_t::sflow_parameter_t() {

}

sflow_soi_node_t::sflow_soi_node_t() : sflow_node_t () {

  m_out_types.resize(1);
  m_out_types[0] = {sflow_parameter_e::sfpar_int, 0};

}

bool sflow_soi_node_t::execute() {

  bool pres = sflow_node_t::execute();
  if (!pres) return false;

  auto out0 = std::make_shared<sflow_parameter_int_t>();
  out0->m_value = 1;
  m_outs[0] = out0;

  return true;

}

sflow_sgi_node_t::sflow_sgi_node_t() : sflow_node_t () {

  m_out_types.resize(1);
  m_out_types[0] = {sflow_parameter_e::sfpar_int, 0};

  m_inp_types.resize(1);
  m_inp_types[0] = {sflow_parameter_e::sfpar_int, 0};

}

bool sflow_sgi_node_t::execute() {

  bool pres = sflow_node_t::execute();
  if (!pres) return false;

  auto inp0 = static_cast<sflow_parameter_int_t*>(m_inps[0].get());

  if (inp0) {
      auto out0 = std::make_shared<sflow_parameter_int_t>();
      out0->m_value = inp0->m_value + 1;
      m_outs[0] = out0;
    } else {
      return false;
    }

  return true;

}

sflow_sii_node_t::sflow_sii_node_t() : sflow_node_t () {

  m_inp_types.resize(1);
  m_inp_types[0] = {sflow_parameter_e::sfpar_int, 0};

}

bool sflow_sii_node_t::execute() {

  bool pres = sflow_node_t::execute();
  if (!pres) return false;

  auto inp0 = static_cast<sflow_parameter_int_t*>(m_inps[0].get());
  if (inp0) {
      fmt::print(std::cout, "NODE_MESSAGE {}\n", inp0->m_value);
      return true;
    } else {
      return false;
    }

  return true;

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
  //_clone->m_value = m_value;
  return _clone;
}
