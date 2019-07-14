#include <qppcad/sflow/sflow_parameter_ws_item.hpp>

using namespace qpp;
using namespace qpp::cad;

sflow_parameter_e sflow_parameter_ws_item_t::get_param_meta() {
  return sflow_parameter_e::sfpar_ws_item;
}

std::shared_ptr<sflow_parameter_t> sflow_parameter_ws_item_t::clone() {
  auto _clone = std::make_shared<sflow_parameter_ws_item_t>();
  _clone->m_value = m_value;
  return std::move(_clone);
}
