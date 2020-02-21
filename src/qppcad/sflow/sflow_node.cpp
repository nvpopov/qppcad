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

opt<size_t> sflow_node_t::get_data_by_name(sflow_data_group_e group, const std::string &parname) {

  switch (group) {

    case sflow_data_group_e::inp_d : {
        return get_data_by_name(m_inp_schema, parname);
        break;
      }

    case sflow_data_group_e::out_d : {
        return get_data_by_name(m_out_schema, parname);
        break;
      }

    case sflow_data_group_e::ipl_d : {
        return get_data_by_name(m_ipl_schema, parname);
        break;
      }

    }

  return std::nullopt;

}

opt<size_t> sflow_node_t::get_data_by_name(sflow_sck_info_group_t &sckinf,
                                           const std::string &parname) {

  auto find_fn = [&parname] (sflow_sck_info_group_t::value_type data_scheme) {
    return data_scheme.m_sck_name == parname;
  };

  auto find_itr = std::find_if(std::begin(sckinf), std::end(sckinf), find_fn);

  return find_itr != sckinf.end() ?
        opt<size_t>{std::distance(std::begin(sckinf), find_itr)} : std::nullopt;

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

sflow_socket_info_t &sflow_socket_info_t::editable(const bool value) {
  m_editable = value;
  return *this;
}

sflow_socket_info_t &sflow_socket_info_t::serializable(const bool value) {
  m_is_serializable = value;
  return *this;
}

sflow_socket_info_t &sflow_socket_info_t::hide_label(const bool value) {
  m_hide_label = value;
  return *this;
}
