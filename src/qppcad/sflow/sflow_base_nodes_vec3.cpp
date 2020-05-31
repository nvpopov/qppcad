#include <qppcad/sflow/sflow_base_nodes_vec3.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_v3f_prop_node_t::sf_v3f_prop_node_t() {

  m_node_name = "v3f";

  m_out_schema = {
    make_outs(sf_parameter_e::sfpar_v3f, "")
  };

  m_ipl_schema = {
    make_ipls(sf_parameter_e::sfpar_v3f, "value").hide_label(true)
  };

  m_front_end_width = std::optional<int>(250);

}

bool sf_v3f_prop_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_v3f_t>(0, m_outs);
  auto ipl0 = get_pars_as<sf_parameter_v3f_t>(0, m_ipl);

  out0->m_value = ipl0->m_value;

  return true;

}

bool sf_v3f_prop_node_t::is_single_node() {
  return true;
}

sf_v3f_final_node_t::sf_v3f_final_node_t() {

  m_node_name = "show v3f";

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_v3f, "")
  };

  m_ipl_schema = {
    make_ipls(sf_parameter_e::sfpar_v3f, "vec").editable(false).hide_label(true)
  };

  m_front_end_width = std::optional<int>(250);

}

bool sf_v3f_final_node_t::execute_ex() {

  auto inp0 = get_pars_as<sf_parameter_v3f_t>(0, m_inps);
  auto ipl0 = get_pars_as<sf_parameter_v3f_t>(0, m_ipl);

  if (inp0 && ipl0) {
      ipl0->m_value = inp0->m_value;
      return true;
    } else {
      return false;
    }

}

bool sf_v3f_final_node_t::is_single_node() {
  return true;
}

sf_v3f_sum_v3f_node_t::sf_v3f_sum_v3f_node_t() {

  m_node_name = "v3f + v3f";

  m_out_schema = {
    make_outs(sf_parameter_e::sfpar_v3f, "dst")
  };

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_v3f, "src1"),
    make_inps(sf_parameter_e::sfpar_v3f, "src2")
  };

  m_front_end_width = std::optional<int>(120);

}

bool sf_v3f_sum_v3f_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_v3f_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_v3f_t>(0, m_inps);
  auto inp1 = get_pars_as<sf_parameter_v3f_t>(1, m_inps);

  if (out0 && inp0 && inp1) {
    out0->m_value.set_value(inp0->m_value.get_value() + inp1->m_value.get_value());
  } else {
    return false;
  }

  return true;

}

sf_v3f_dot_node_t::sf_v3f_dot_node_t() {

  m_node_name = "v3f . v3f";

  m_out_schema = {
   make_outs(sf_parameter_e::sfpar_float, "dst")
  };

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_v3f, "src1"),
    make_inps(sf_parameter_e::sfpar_v3f, "src2")
  };

  m_front_end_width = std::optional<int>(120);

}

bool sf_v3f_dot_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_float_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_v3f_t>(0, m_inps);
  auto inp1 = get_pars_as<sf_parameter_v3f_t>(1, m_inps);

  if (out0 && inp0 && inp1) {
    out0->m_value.set_value(inp0->m_value.get_value().dot(inp1->m_value.get_value()));
  } else {
    return false;
  }

  return true;

}

sf_v3f_normalized_node_t::sf_v3f_normalized_node_t() {

  m_node_name = "v3f.normalized()";

  m_out_schema = {
   make_outs(sf_parameter_e::sfpar_v3f, "dst")
  };

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_v3f, "src"),
  };

  m_front_end_width = std::optional<int>(120);

}

bool sf_v3f_normalized_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_v3f_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_v3f_t>(0, m_inps);

  if (out0 && inp0) {
    out0->m_value.set_value(inp0->m_value.get_value().normalized());
  } else {
    return false;
  }

  return true;

}

sf_v3f_norm_node_t::sf_v3f_norm_node_t() {

  m_node_name = "v3f.norm()";

  m_out_schema = {
   make_outs(sf_parameter_e::sfpar_float, "dst")
  };

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_v3f, "src"),
  };

  m_front_end_width = std::optional<int>(120);
}

bool sf_v3f_norm_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_float_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_v3f_t>(0, m_inps);

  if (out0 && inp0) {
    out0->m_value.set_value(inp0->m_value.get_value().norm());
  } else {
    return false;
  }

  return true;

}

sf_v3f_cross_node_t::sf_v3f_cross_node_t() {

  m_node_name = "v3f x v3f";

  m_out_schema = {
   make_outs(sf_parameter_e::sfpar_v3f, "dst")
  };

  m_inp_schema = {
   make_inps(sf_parameter_e::sfpar_v3f, "src1"),
   make_inps(sf_parameter_e::sfpar_v3f, "src2")
  };

  m_front_end_width = std::optional<int>(120);

}

bool sf_v3f_cross_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_v3f_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_v3f_t>(0, m_inps);
  auto inp1 = get_pars_as<sf_parameter_v3f_t>(1, m_inps);

  if (out0 && inp0) {
    out0->m_value.set_value(inp0->m_value.get_value().cross(inp1->m_value.get_value()));
  } else {
    return false;
  }

  return true;

}
