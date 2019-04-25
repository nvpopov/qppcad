#include <qppcad/sflow/sflow_base_nodes.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_i_prop_node_t::sf_i_prop_node_t() : sflow_node_t () {

  m_node_name = "integer";

  m_out_types = {
    {sflow_parameter_e::sfpar_int, 0, ""}
  };

  m_inplace_types = {
    {sflow_parameter_e::sfpar_int, "value", true}
  };

}

bool sf_i_prop_node_t::execute_ex() {

  auto out0 = std::make_shared<sflow_parameter_int_t>();

  auto inp0 = m_inplace_parameters[0]->cast_as<sflow_parameter_int_t>();
  if (!inp0) return false;

  out0->m_value = inp0->m_value;
  m_outs[0] = out0;

  return true;

}

bool sf_i_prop_node_t::is_single_node() {
  return true;
}

sf_i_final_node_t::sf_i_final_node_t() : sflow_node_t () {

  m_node_name = "integer show";

  m_inp_types = {
    {sflow_parameter_e::sfpar_int, 0, ""}
  };

  m_inplace_types = {
    {sflow_parameter_e::sfpar_int, "value", false}
  };

}

bool sf_i_final_node_t::execute_ex() {

  auto inp0 = static_cast<sflow_parameter_int_t*>(m_inps[0].get());
  auto inp_p0 = m_inplace_parameters[0]->cast_as<sflow_parameter_int_t>();

  if (inp0 && inp_p0) {
      inp_p0->m_value = inp0->m_value;
      fmt::print(std::cout, "NODE_MESSAGE {}\n", inp_p0->m_value);
      return true;
    } else {
      return false;
    }

  return true;

}

bool sf_i_final_node_t::is_single_node() {
  return true;
}

sf_i_p_const_node_t::sf_i_p_const_node_t() : sflow_node_t () {

  m_node_name = "integer + c";

  m_out_types = {
   {sflow_parameter_e::sfpar_int, 0, "a"}
  };

  m_inp_types = {
    {sflow_parameter_e::sfpar_int, 0, "b"}
  };

}

bool sf_i_p_const_node_t::execute_ex() {

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

  m_out_types = {
   {sflow_parameter_e::sfpar_int, 0, "dst"}
  };

  m_inp_types = {
    {sflow_parameter_e::sfpar_int, 0, "src1"},
    {sflow_parameter_e::sfpar_int, 0, "src2"}
  };

}

bool sf_i_sum_i_node_t::execute_ex() {

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

  m_node_name = "float";

  m_out_types = {
    {sflow_parameter_e::sfpar_float, 0, "dst"}
  };

}

bool sf_f_prop_node_t::execute_ex() {

  auto out0 = std::make_shared<sflow_parameter_float_t>();
  out0->m_value = 0.5f;
  m_outs[0] = out0;

  return true;

}

sf_f_p_const_node_t::sf_f_p_const_node_t() {

  m_node_name = "float + c";

  m_out_types = {
    {sflow_parameter_e::sfpar_float, 0, "dst"}
  };

  m_inp_types = {
   {sflow_parameter_e::sfpar_float, 0, "src"}
  };

}

bool sf_f_p_const_node_t::execute_ex() {

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

sf_ipatb_const_node_t::sf_ipatb_const_node_t() {

  m_node_name = "(int + a) * b";

  m_out_types = {
   {sflow_parameter_e::sfpar_int, 0, "src"}
  };

  m_inp_types = {
    {sflow_parameter_e::sfpar_int, 0, "dst"}
  };

  m_inplace_types = {
    {sflow_parameter_e::sfpar_int, "a", true},
    {sflow_parameter_e::sfpar_int, "b", true}
  };

}

bool sf_ipatb_const_node_t::execute_ex() {

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
