#include <qppcad/sflow/sflow_base_nodes.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_i_prop_node_t::sf_i_prop_node_t() : sflow_node_t () {

  m_node_name = "integer propagator";
  m_out_types.resize(1);
  m_out_types[0] = {sflow_parameter_e::sfpar_int, 0, "value"};

}

bool sf_i_prop_node_t::execute() {

  bool pres = sflow_node_t::execute();
  if (!pres) return false;

  auto out0 = std::make_shared<sflow_parameter_int_t>();
  out0->m_value = 1;
  m_outs[0] = out0;

  return true;

}

sf_i_final_node_t::sf_i_final_node_t() : sflow_node_t () {

  m_node_name = "integer finalize";

  m_inp_types.resize(1);
  m_inp_types[0] = {sflow_parameter_e::sfpar_int, 0, "a"};

}

bool sf_i_final_node_t::execute() {

  auto inp0 = static_cast<sflow_parameter_int_t*>(m_inps[0].get());
  if (inp0) {
      fmt::print(std::cout, "NODE_MESSAGE {}\n", inp0->m_value);
      return true;
    } else {
      return false;
    }

  return true;

}

sf_i_p_const_node_t::sf_i_p_const_node_t() : sflow_node_t () {

  m_node_name = "integer + c";

  m_out_types.resize(1);
  m_out_types[0] = {sflow_parameter_e::sfpar_int, 0, "a"};

  m_inp_types.resize(1);
  m_inp_types[0] = {sflow_parameter_e::sfpar_int, 0, "b"};

}

bool sf_i_p_const_node_t::execute() {

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

sf_i_sum_i_node_t::sf_i_sum_i_node_t() {

  m_node_name = "integer sum";

  m_out_types.resize(1);
  m_out_types[0] = {sflow_parameter_e::sfpar_int, 0, "dst"};

  m_inp_types.resize(2);
  m_inp_types[0] = {sflow_parameter_e::sfpar_int, 0, "src1"};
  m_inp_types[1] = {sflow_parameter_e::sfpar_int, 0, "src2"};

}

bool sf_i_sum_i_node_t::execute() {

  bool pres = sflow_node_t::execute();
  if (!pres) return false;

  auto inp0 = static_cast<sflow_parameter_int_t*>(m_inps[0].get());
  auto inp1 = static_cast<sflow_parameter_int_t*>(m_inps[1].get());

  if (inp0 && inp1) {
      auto out0 = std::make_shared<sflow_parameter_int_t>();
      out0->m_value = inp0->m_value + inp1->m_value;
      m_outs[0] = out0;
    } else {
      return false;
    }

  return true;

}

sf_f_prop_node_t::sf_f_prop_node_t() {

  m_node_name = "float propagator";

  m_out_types.resize(1);
  m_out_types[0] = {sflow_parameter_e::sfpar_float, 0, "dst"};

}

bool sf_f_prop_node_t::execute() {

  bool pres = sflow_node_t::execute();
  if (!pres) return false;

  auto out0 = std::make_shared<sflow_parameter_float_t>();
  out0->m_value = 0.5f;
  m_outs[0] = out0;

  return true;

}

sf_f_p_const_node_t::sf_f_p_const_node_t() {

  m_node_name = "float + c";

  m_out_types.resize(1);
  m_out_types[0] = {sflow_parameter_e::sfpar_float, 0, "dst"};

  m_inp_types.resize(1);
  m_inp_types[0] = {sflow_parameter_e::sfpar_float, 0, "src"};

}

bool sf_f_p_const_node_t::execute() {

  bool pres = sflow_node_t::execute();
  if (!pres) return false;

  auto inp0 = static_cast<sflow_parameter_float_t*>(m_inps[0].get());

  if (inp0) {
      auto out0 = std::make_shared<sflow_parameter_float_t>();
      out0->m_value = inp0->m_value + 0.5;
      m_outs[0] = out0;
    } else {
      return false;
    }

  return true;

}
