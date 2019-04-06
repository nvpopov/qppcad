#include <qppcad/flow/sflow.hpp>

using namespace qpp;
using namespace qpp::cad;

void sflow_context_t::add_node(std::shared_ptr<sflow_node_t> node) {

  if (node) {
      node->m_idx_lookup = m_nodes.size();
      m_nodes.push_back(node);
    }

}

void sflow_context_t::connect_node(std::shared_ptr<sflow_node_t> node1,
                                   std::shared_ptr<sflow_node_t> node2) {

  auto it1 = std::find(m_nodes.begin(), m_nodes.end(), node1);
  auto it2 = std::find(m_nodes.begin(), m_nodes.end(), node2);

  if (it1 != m_nodes.end() && it2 != m_nodes.end()) {
      auto idx1 = std::distance(m_nodes.begin(), it1);
      auto idx2 = std::distance(m_nodes.begin(), it2);
      node1->m_outputs.push_back(node2);
      node2->m_inputs.push_back(node1);
      m_con_data.push_back({idx1, idx2});
    }

}

void sflow_context_t::mark_outer_nodes() {

  for (auto &node : m_nodes)
    node->m_is_outer = node->m_inputs.empty();

}

void sflow_context_t::make_raw_calc_flows() {

  for (auto &node : m_nodes)
    if (node->m_is_outer) {
        std::vector<size_t> _raw_flow;
        traverse_calc_flow(node.get(), _raw_flow);
        m_raw_calc_flow.emplace_back(std::move(_raw_flow));
      }

}

void sflow_context_t::make_calc_flows() {

  m_main_flow.resize(m_raw_calc_flow.size());

  std::vector<size_t> len_vec;
  len_vec.resize(m_raw_calc_flow.size());

  for (size_t i = 0 ; i < m_raw_calc_flow.size(); i++)
    len_vec[i] = m_raw_calc_flow[i].size();

  auto max_el = std::max_element(len_vec.begin(), len_vec.end());
  auto max_idx = std::distance(len_vec.begin(), max_el);
  auto max_val = *max_el;

  for (size_t i = 0; i < m_raw_calc_flow.size(); i++) m_main_flow[i] = i == max_idx;

}

void sflow_context_t::traverse_calc_flow(sflow_node_t *cur_node, std::vector<size_t> &node_idx) {

  node_idx.push_back(cur_node->m_idx_lookup);

  for (auto &node : cur_node->m_outputs)
    traverse_calc_flow(node.get(), node_idx);

}

void sflow_context_t::compile_flow() {

  mark_outer_nodes();
  make_raw_calc_flows();
  make_calc_flows();

  for (auto &node : m_nodes)
    if (node->m_is_outer) fmt::print(std::cout, "outer node {}\n", node->m_idx_lookup);

  fmt::print(std::cout, "total flows {}\n", m_raw_calc_flow.size());

  for (size_t i = 0; i < m_raw_calc_flow.size(); i++) {
      auto &flow = m_raw_calc_flow[i];
      fmt::print(std::cout, "flow N{} [", i);
      for (size_t q = 0; q < flow.size(); q++) fmt::print(std::cout, "{},", flow[q]);
      fmt::print(std::cout, "]\n");
    }

  for (size_t i = 0; i < m_raw_calc_flow.size(); i++) {
      auto &flow = m_raw_calc_flow[i];
      fmt::print(std::cout, "flow N{} ({})[", i, m_main_flow[i]);
      for (size_t q = 0; q < flow.size(); q++) fmt::print(std::cout, "{},", flow[q]);
      fmt::print(std::cout, "]\n");
    }

}

void sflow_context_t::execute() {

  for (auto &node : m_nodes)
    if (node->m_is_outer) {
        execute_traverse(node.get());
      }

}

void sflow_context_t::execute_traverse(sflow_node_t *cur_node) {

  fmt::print(std::cout, "exec_traverse : {}\n", cur_node->m_node_name);

  for (auto &node : cur_node->m_outputs) {
      node->m_total_gain += 1;
      if (node->m_total_gain >= node->m_inputs.size())
        execute_traverse(node.get());
    }

}

sflow_context_t::sflow_context_t() {

}

sflow_node_t::sflow_node_t() {

}

bool sflow_node_t::can_be_as_input() {
  return true;
}

bool sflow_node_t::can_be_as_output() {
  return true;
}

void sflow_node_t::execute(sflow_context_t *fl_ctx) {

}

void sflow_node_t::feed_inputs(size_t input_idx, sflow_parameters_t value) {

}
