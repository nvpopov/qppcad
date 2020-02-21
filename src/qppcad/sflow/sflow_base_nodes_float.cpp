#include <qppcad/sflow/sflow_base_nodes_float.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_float_prop_node_t::sf_float_prop_node_t() {

  m_node_name = "float";

  m_out_schema = {
    make_outs(sflow_parameter_e::sfpar_float, "dst")
  };

  m_ipl_schema = {
    make_ipls(sflow_parameter_e::sfpar_float, "value")
  };

  m_front_end_width = opt<int>(130);

}

bool sf_float_prop_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_float_t>(0, m_outs);
  auto ipl0 = get_pars_as<sflow_parameter_float_t>(0, m_ipl);

  if (out0 && ipl0) out0->m_value = ipl0->m_value;
  else return false;

  return true;

}

bool sf_float_prop_node_t::is_single_node() {
  return true;
}

sf_float_p_const_node_t::sf_float_p_const_node_t() {

  m_node_name = "float + const";

  m_out_schema = {
    make_outs(sflow_parameter_e::sfpar_float, "dst")
  };

  m_inp_schema = {
   make_inps(sflow_parameter_e::sfpar_float, "src")
  };

  m_ipl_schema = {
    make_ipls(sflow_parameter_e::sfpar_float, "const")
  };

}

bool sf_float_p_const_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_float_t>(0, m_outs);
  auto inp0 = get_pars_as<sflow_parameter_float_t>(0, m_inps);
  auto ipl0 = get_pars_as<sflow_parameter_float_t>(0, m_ipl);

  if (out0 && inp0 && ipl0) {
      out0->m_value = inp0->m_value + ipl0->m_value;
    } else {
      return false;
    }

  return true;

}

sf_float_final_node_t::sf_float_final_node_t() {

  m_node_name = "show float";

  m_inp_schema = {
    make_inps(sflow_parameter_e::sfpar_float, "")
  };

  m_ipl_schema = {
    make_ipls(sflow_parameter_e::sfpar_float, "").editable(false)
  };

  m_front_end_width = opt<int>(130);

}

bool sf_float_final_node_t::execute_ex() {

  auto inp0   = get_pars_as<sflow_parameter_float_t>(0, m_inps);
  auto inp_p0 = get_pars_as<sflow_parameter_float_t>(0, m_ipl);

  if (inp0 && inp_p0) {
      inp_p0->m_value = inp0->m_value;
      return true;
    } else {
      return false;
    }

}

bool sf_float_final_node_t::is_single_node() {
  return true;
}

sf_float_sum_float_node_t::sf_float_sum_float_node_t() {

  m_node_name = "float + float";

  m_out_schema = {
    make_outs(sflow_parameter_e::sfpar_float, "dst")
  };

  m_inp_schema = {
    make_inps(sflow_parameter_e::sfpar_float, "src1"),
    make_inps(sflow_parameter_e::sfpar_float, "src2")
  };

  m_front_end_width = opt<int>(120);

}

bool sf_float_sum_float_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_float_t>(0, m_outs);
  auto inp0 = get_pars_as<sflow_parameter_float_t>(0, m_inps);
  auto inp1 = get_pars_as<sflow_parameter_float_t>(1, m_inps);

  if (out0 && inp0 && inp1) {
      out0->m_value = inp0->m_value + inp1->m_value;
    } else {
      return false;
    }

  return true;

}
