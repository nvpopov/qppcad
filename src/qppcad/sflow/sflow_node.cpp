#include <qppcad/sflow/sflow_node.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_node_t::sf_node_t() {

}

bool sf_node_t::has_ipls() {

  return !m_ipl_schema.empty();

}

void sf_node_t::explicit_create_ipl() {

  fill_data_vec(m_ipl_schema, m_ipl);

}

std::optional<size_t> sf_node_t::get_data_by_name(sf_data_group_e group, const std::string &parname) {

  switch (group) {

    case sf_data_group_e::inp_d : {
        return get_data_by_name(m_inp_schema, parname);
        break;
      }

    case sf_data_group_e::out_d : {
        return get_data_by_name(m_out_schema, parname);
        break;
      }

    case sf_data_group_e::ipl_d : {
        return get_data_by_name(m_ipl_schema, parname);
        break;
      }

    }

  return std::nullopt;

}

std::optional<size_t> sf_node_t::get_data_by_name(sf_sck_info_group_t &sckinf,
                                                  const std::string &parname) {

  auto find_fn = [&parname] (sf_sck_info_group_t::value_type data_scheme) {
    return data_scheme.m_sck_name == parname;
  };

  auto find_itr = std::find_if(std::begin(sckinf), std::end(sckinf), find_fn);

  return find_itr != sckinf.end() ?
        std::optional<size_t>{std::distance(std::begin(sckinf), find_itr)} : std::nullopt;

}

bool sf_node_t::execute() {

  fill_data_vec(m_inp_schema, m_inps);
  fill_data_vec(m_out_schema, m_outs);
  fill_data_vec(m_ipl_schema, m_ipl);

  validate_data_vec(m_inp_schema, m_inps);
  validate_data_vec(m_out_schema, m_outs);
  validate_data_vec(m_ipl_schema, m_ipl);

  m_outs.resize(m_out_schema.size(), nullptr);
  return execute_ex();

}

bool sf_node_t::execute_ex() {

  return true;

}

bool sf_node_t::is_single_node() {

  return false;

}

sf_socket_info_t qpp::cad::make_default_sck(sf_data_group_e dg,
                                               sf_parameter_e type,
                                               const std::string &sck_name) {

  sf_socket_info_t tmp_sck_info;
  tmp_sck_info.m_data_group = dg;
  tmp_sck_info.m_type = type;
  tmp_sck_info.m_sck_name = sck_name;
  tmp_sck_info.m_tot_con = 0;
  tmp_sck_info.m_is_inplace = false;

  return tmp_sck_info;

}

sf_socket_info_t qpp::cad::make_inps(sf_parameter_e type, const std::string &sck_name) {

  sf_socket_info_t tmp_sck_info;
  tmp_sck_info.m_data_group = sf_data_group_e::inp_d;
  tmp_sck_info.m_type = type;
  tmp_sck_info.m_sck_name = sck_name;
  tmp_sck_info.m_tot_con = 0;
  tmp_sck_info.m_editable = true;
  tmp_sck_info.m_is_inplace = false;

  return tmp_sck_info;

}

sf_socket_info_t qpp::cad::make_outs(sf_parameter_e type, const std::string &sck_name) {

  sf_socket_info_t tmp_sck_info;
  tmp_sck_info.m_data_group = sf_data_group_e::out_d;
  tmp_sck_info.m_type = type;
  tmp_sck_info.m_sck_name = sck_name;
  tmp_sck_info.m_tot_con = 0;
  tmp_sck_info.m_editable = true;
  tmp_sck_info.m_is_inplace = false;

  return tmp_sck_info;

}

sf_socket_info_t qpp::cad::make_ipls(sf_parameter_e type, const std::string &sck_name) {

  sf_socket_info_t tmp_sck_info;
  tmp_sck_info.m_data_group = sf_data_group_e::ipl_d;
  tmp_sck_info.m_type = type;
  tmp_sck_info.m_sck_name = sck_name;
  tmp_sck_info.m_tot_con = 0;
  tmp_sck_info.m_editable = true;
  tmp_sck_info.m_is_inplace = true;

  return tmp_sck_info;

}

sf_socket_info_t &sf_socket_info_t::editable(const bool value) {
  m_editable = value;
  return *this;
}

sf_socket_info_t &sf_socket_info_t::serializable(const bool value) {
  m_is_serializable = value;
  return *this;
}

sf_socket_info_t &sf_socket_info_t::hide_label(const bool value) {
  m_hide_label = value;
  return *this;
}
