#include <qppcad/sflow/sflow.hpp>

using namespace qpp;
using namespace qpp::cad;

void sflow_context_t::add_node(std::shared_ptr<sflow_node_t> node) {

  if (node) {

      node->m_idx_lookup = m_nodes.size();
      m_nodes.push_back(node);

      //clear pin connections info
      for (auto &con_type : node->m_inp_types)
        con_type.m_tot_con = 0;
      for (auto &con_type : node->m_out_types)
        con_type.m_tot_con = 0;

    }

}

sflow_status_e sflow_context_t::connect_node(std::shared_ptr<sflow_node_t> out_node,
                                             std::shared_ptr<sflow_node_t> inp_node,
                                             size_t out_socket,
                                             size_t inp_socket) {

  fmt::print(std::cout, "connect_node on=\"{}\" in=\"{}\" os={} is={} ",
             out_node->m_node_name, inp_node->m_node_name, out_socket, inp_socket);

  if (out_node == inp_node) {
      fmt::print(std::cout,"sflow_status_e::error_nodes_are_equal");
      return sflow_status_e::error_nodes_are_equal;
    }

  //check output pin
  bool socket1_valid = out_socket < out_node->m_out_types.size();
  if (!socket1_valid) {
      fmt::print(std::cout,"sflow_status_e::pin1_invalid");
      return sflow_status_e::pin1_invalid;
    }

  //check input pin
  bool socket2_valid = inp_socket < inp_node->m_inp_types.size();
  if (!socket2_valid) {
      fmt::print(std::cout,"sflow_status_e::pin2_invalid");
      return sflow_status_e::pin2_invalid;
    }

  //check pin types
  bool socket_types_eq =
      out_node->m_out_types[out_socket].m_type == inp_node->m_inp_types[inp_socket].m_type;
  if (!socket_types_eq) {
      fmt::print(std::cout,"sflow_status_e::types_pin1_not_eq_pin2");
      return sflow_status_e::types_pin1_not_eq_pin2;
    }

  //check multipin output
  bool inp_node_is_connected = inp_node->m_inp_types[inp_socket].m_tot_con == 0;
  if (!inp_node_is_connected) {
      fmt::print(std::cout,"sflow_status_e::too_many_inputs {} ",
                 inp_node->m_inp_types[inp_socket].m_tot_con);
      return sflow_status_e::too_many_inputs;
    }

  inp_node->m_inp_types[inp_socket].m_tot_con +=1;
  inp_node->m_is_outer = false;

  if (socket1_valid && socket2_valid && socket_types_eq) {
      sflow_connectivity_data_t _con_data;
      _con_data.m_inp_node = inp_node;
      _con_data.m_out_node = out_node;
      _con_data.m_inp_sck = inp_socket;
      _con_data.m_out_sck = out_socket;
      m_connectivity.push_back(std::move(_con_data));
    }

  fmt::print(std::cout, "\n");
  return sflow_status_e::no_error;

}

void sflow_context_t::clear_context() {

  m_calc_meta_global.clear();
  clear_outer_nodes();
  clear_connectivity();
  m_nodes.clear();

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

void sflow_context_t::force_execute() {
  m_force_execute = true;
}

bool sflow_context_t::is_force_execute() {
  return m_force_execute;
}

void sflow_context_t::execute_threaded(bool debug_print) {

  // tasks already executed
  if (m_task_is_being_executed.load()) {
      return;
    }

  // start executing
  m_task_is_being_executed.store(true);
  m_thread = std::thread(&sflow_context_t::execute_threaded_fn, this);
  m_force_execute = false;

}

void sflow_context_t::execute_threaded_fn() {

  for (auto &node : m_nodes)
    if (node->m_is_outer) execute_traverse(node.get(), nullptr, false);

  m_task_has_been_finished.store(true);

}

bool sflow_context_t::is_finished() {

  if (m_task_has_been_finished.load()) {

      m_task_is_being_executed.store(false);
      m_task_has_been_finished.store(false);
      if (m_thread.joinable()) m_thread.join();
      return true;

    }

  return false;

}

bool sflow_context_t::is_running() {

  return m_task_is_being_executed.load();

}

void sflow_context_t::execute(bool debug_print) {

  if (debug_print) fmt::print(std::cout, "\nstart executing \n");

  for (auto &node : m_nodes)
    if (node->m_is_outer) execute_traverse(node.get(), nullptr, debug_print);

  if (debug_print) fmt::print(std::cout, "end executing \n");

}

void sflow_context_t::execute_traverse(sflow_node_t *cur_node,
                                       sflow_node_t *prev_node,
                                       bool debug_print) {

  if (debug_print) fmt::print(std::cout, "exec_traverse : [{}] -> [{}]\n",
                              prev_node ? prev_node->m_node_name : "None",
                              cur_node->m_node_name);

  if (!cur_node) return;

  bool node_exec_res = cur_node->execute();

  if (debug_print) fmt::print(std::cout, "node [{}] exec result {}\n",
                              cur_node->m_node_name, node_exec_res);

  if (!node_exec_res) return;

  for (size_t i = 0; i < m_connectivity.size(); i++) {

      if (m_connectivity[i].m_out_node.get() == cur_node) {

          //last parameter - by reference, otherwise - copy
          bool last_element = i + 1 == m_connectivity.size();
          auto pg_res = propagate_data(&m_connectivity[i], !last_element);
          auto pg_meta_info_res = propagate_meta_info(&m_connectivity[i]);

          if (pg_res == sflow_status_e::propagate_data_succes &&
              pg_meta_info_res == sflow_status_e::propagate_meta_succes)
            execute_traverse(m_connectivity[i].m_inp_node.get(), cur_node);

        }

    }

}

sflow_status_e sflow_context_t::propagate_data(sflow_connectivity_data_t *cd,
                                               bool copy_par,
                                               bool debug_print) {

  if (cd && cd->m_inp_node && cd->m_out_node) {

      if (debug_print) fmt::print(std::cout, "propagate_data from=[{}] to=[{}] copy_data?={} "
                                             "out_sck={} in_sck={} out_t={} in_t={} ",
                                  cd->m_out_node->m_node_name,
                                  cd->m_inp_node->m_node_name,
                                  copy_par,
                                  *(cd->m_out_sck),
                                  *(cd->m_inp_sck),
                                  cd->m_out_node->m_out_types[*(cd->m_out_sck)].m_type,
                                  cd->m_inp_node->m_inp_types[*(cd->m_inp_sck)].m_type
                                 );

      //check that inputs have space for fun
      if (cd->m_inp_node->m_inps.size() != cd->m_inp_node->m_inp_types.size())
        cd->m_inp_node->m_inps.resize(cd->m_inp_node->m_inp_types.size(), nullptr);

      //check that out is valid
      if (*cd->m_out_sck < cd->m_out_node->m_outs.size() &&
          cd->m_out_node->m_outs[*cd->m_out_sck]) {
          cd->m_inp_node->m_inps[*cd->m_inp_sck] =
              cd->m_out_node->m_outs[*cd->m_out_sck]->clone();

          if (debug_print) fmt::print(std::cout, "!succes!\n");

          return sflow_status_e::propagate_data_succes;

        } else {

          if (debug_print) fmt::print(std::cout, "error\n");

          return sflow_status_e::propagate_data_error;

        }

    } else return sflow_status_e::propagate_data_error;

  return sflow_status_e::propagate_data_succes;

}

sflow_status_e sflow_context_t::propagate_meta_info(sflow_connectivity_data_t *cd,
                                                    bool debug_print) {

  return sflow_status_e::propagate_meta_succes;

}

sflow_context_t::sflow_context_t() {

}

std::vector<std::shared_ptr<sflow_node_t> > &sflow_context_t::get_nodes() {
  return m_nodes;
}



