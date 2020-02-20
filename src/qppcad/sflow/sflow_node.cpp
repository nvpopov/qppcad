#include <qppcad/sflow/sflow_node.hpp>

using namespace qpp;
using namespace qpp::cad;

sflow_node_t::sflow_node_t() {

}

bool sflow_node_t::has_ipls() {

  return !m_ipl_schema.empty();

}

void sflow_node_t::explicit_create_ipl() {

  fill_data_vec(m_ipl_schema, m_ipl);

}

opt<size_t> sflow_node_t::get_data_by_name(sflow_data_group_e group, const std::string &iplname) {

}

opt<size_t> sflow_node_t::get_data_by_name(sflow_sck_info_group_t &sckinf,
                                           const std::string &iplname) {

  //  auto find_fn = [&iplname](sflow_data_group_t::value_type data_scheme) {
  //        return data_scheme->m_name == iplname;
  //  };

}

bool sflow_node_t::execute() {

  fill_data_vec(m_inp_schema, m_inps);
  fill_data_vec(m_out_schema, m_outs);
  fill_data_vec(m_ipl_schema, m_ipl);

  validate_data_vec(m_inp_schema, m_inps);
  validate_data_vec(m_out_schema, m_outs);
  validate_data_vec(m_ipl_schema, m_ipl);

  m_outs.resize(m_out_schema.size(), nullptr);
  return execute_ex();

}

bool sflow_node_t::execute_ex() {

  return true;

}

bool sflow_node_t::is_single_node() {

  return false;

}

sflow_socket_info_t qpp::cad::make_default_sck(sflow_data_group_e dg,
                                               sflow_parameter_e type,
                                               const std::string &sck_name) {

  sflow_socket_info_t tmp_sck_info;
  tmp_sck_info.m_data_group = dg;
  tmp_sck_info.m_type = type;
  tmp_sck_info.m_sck_name = sck_name;
  tmp_sck_info.m_tot_con = 0;
  tmp_sck_info.m_is_inplace = false;

  return tmp_sck_info;

}

sflow_socket_info_t qpp::cad::make_inps(sflow_parameter_e type, const std::string &sck_name) {

  sflow_socket_info_t tmp_sck_info;
  tmp_sck_info.m_data_group = sflow_data_group_e::inp_d;
  tmp_sck_info.m_type = type;
  tmp_sck_info.m_sck_name = sck_name;
  tmp_sck_info.m_tot_con = 0;
  tmp_sck_info.m_editable = true;
  tmp_sck_info.m_is_inplace = false;

  return tmp_sck_info;

}

sflow_socket_info_t qpp::cad::make_outs(sflow_parameter_e type, const std::string &sck_name) {

  sflow_socket_info_t tmp_sck_info;
  tmp_sck_info.m_data_group = sflow_data_group_e::out_d;
  tmp_sck_info.m_type = type;
  tmp_sck_info.m_sck_name = sck_name;
  tmp_sck_info.m_tot_con = 0;
  tmp_sck_info.m_editable = true;
  tmp_sck_info.m_is_inplace = false;

  return tmp_sck_info;

}

sflow_socket_info_t qpp::cad::make_ipls(sflow_parameter_e type, const std::string &sck_name) {

  sflow_socket_info_t tmp_sck_info;
  tmp_sck_info.m_data_group = sflow_data_group_e::ipl_d;
  tmp_sck_info.m_type = type;
  tmp_sck_info.m_sck_name = sck_name;
  tmp_sck_info.m_tot_con = 0;
  tmp_sck_info.m_editable = true;
  tmp_sck_info.m_is_inplace = true;

  return tmp_sck_info;

}

sflow_socket_info_t &sflow_socket_info_t::set_editable(const bool value) {

  m_editable = value;
  return *this;

}
