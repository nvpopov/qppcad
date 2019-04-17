#include <qppcad/sflow/sflow_base_nodes.hpp>

using namespace qpp;
using namespace qpp::cad;

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
