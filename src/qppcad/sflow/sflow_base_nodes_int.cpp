#include <qppcad/sflow/sflow_base_nodes_int.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_int_prop_node_t::sf_int_prop_node_t() : sf_node_t () {

  m_node_name = "int";

  m_out_schema = {
    make_outs(sf_parameter_e::sfpar_int, "")
  };

  m_ipl_schema = {
    make_ipls(sf_parameter_e::sfpar_int, "value").hide_label(false)
  };

  m_front_end_width = std::optional<int>(130);

}

bool sf_int_prop_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_int_t>(0, m_outs);
  auto ipl0 = get_pars_as<sf_parameter_int_t>(0, m_ipl);

  out0->m_value.set_value(ipl0->m_value.get_value());

  return true;

}

bool sf_int_prop_node_t::is_single_node() {
  return true;
}

sf_int_final_node_t::sf_int_final_node_t() : sf_node_t () {

  m_node_name = "show int";

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_int, "")
  };

  m_ipl_schema = {
    make_ipls(sf_parameter_e::sfpar_int, "value").editable(false)
  };

  m_front_end_width = std::optional<int>(130);

}

bool sf_int_final_node_t::execute_ex() {

  auto inp0 = get_pars_as<sf_parameter_int_t>(0, m_inps);
  auto ipl0 = get_pars_as<sf_parameter_int_t>(0, m_ipl);

  if (inp0 && ipl0) {
    ipl0->m_value.set_value(inp0->m_value.get_value());
    return true;
  } else {
    return false;
  }

}

bool sf_int_final_node_t::is_single_node() {
  return true;
}

sf_int_p_const_node_t::sf_int_p_const_node_t() : sf_node_t () {

  m_node_name = "int + const";

  m_out_schema = {
    make_outs(sf_parameter_e::sfpar_int, "a")
  };

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_int, "b")
  };

  m_ipl_schema = {
    make_ipls(sf_parameter_e::sfpar_int, "const")
  };

}

bool sf_int_p_const_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_int_t>(0, m_inps);
  auto ipl0 = get_pars_as<sf_parameter_int_t>(0, m_ipl);

  if (out0 && inp0 && ipl0) {
    out0->m_value.set_value(inp0->m_value.get_value() + ipl0->m_value.get_value());
  } else {
      return false;
  }

  return true;

}

sf_int_sum_int_node_t::sf_int_sum_int_node_t() {

  m_node_name = "int + int";

  m_out_schema = {
    make_outs(sf_parameter_e::sfpar_int, "dst")
  };

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_int, "src1"),
    make_inps(sf_parameter_e::sfpar_int, "src2")
  };

  m_front_end_width = std::optional<int>(130);

}

bool sf_int_sum_int_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_int_t>(0, m_inps);
  auto inp1 = get_pars_as<sf_parameter_int_t>(1, m_inps);

  if (out0 && inp0 && inp1) {
    out0->m_value.set_value(inp0->m_value.get_value() + inp1->m_value.get_value());
  } else {
    return false;
  }

  return true;

}

sf_int_patb_const_node_t::sf_int_patb_const_node_t() {

  m_node_name = "(int + a) * b";

  m_out_schema = {
   make_outs(sf_parameter_e::sfpar_int, "src")
  };

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_int, "dst")
  };

  m_ipl_schema = {
    make_ipls(sf_parameter_e::sfpar_int, "a"),
    make_ipls(sf_parameter_e::sfpar_int, "b")
  };

  m_front_end_width = std::optional<int>(130);

}

bool sf_int_patb_const_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_int_t>(0, m_inps);
  auto ipl0 = get_pars_as<sf_parameter_int_t>(0, m_ipl);
  auto ipl1 = get_pars_as<sf_parameter_int_t>(1, m_ipl);

  if (out0 && inp0 && ipl0 && ipl1) {
    out0->m_value.set_value(
        (inp0->m_value.get_value() + ipl0->m_value.get_value()) * ipl1->m_value.get_value());
  } else {
    return false;
  }

  return true;

}

sf_int_mul_int_node_t::sf_int_mul_int_node_t() {

  m_node_name = "int * int";

  m_out_schema = {
    make_outs(sf_parameter_e::sfpar_int, "dst")
  };

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_int, "src1"),
    make_inps(sf_parameter_e::sfpar_int, "src2")
  };

  m_front_end_width = std::optional<int>(130);

}

bool sf_int_mul_int_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_int_t>(0, m_inps);
  auto inp1 = get_pars_as<sf_parameter_int_t>(1, m_inps);

  if (out0 && inp0 && inp1) {
    out0->m_value.set_value(inp0->m_value.get_value() * inp1->m_value.get_value());
  } else {
    return false;
  }

  return true;

}

sf_int_div_int_node_t::sf_int_div_int_node_t() {

  m_node_name = "int / int";

  m_out_schema = {
   make_outs(sf_parameter_e::sfpar_int, "dst")
  };

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_int, "src1"),
    make_inps(sf_parameter_e::sfpar_int, "src2")
  };

}

bool sf_int_div_int_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_int_t>(0, m_inps);
  auto inp1 = get_pars_as<sf_parameter_int_t>(1, m_inps);

  if (out0 && inp0 && inp1) {
    out0->m_value.set_value(
        inp1->m_value.get_value() == 0 ? 0 : inp0->m_value.get_value() / inp1->m_value.get_value());
  } else {
    return false;
  }

  return true;

}
