#include <qppcad/flow/sflow.hpp>

using namespace qpp;
using namespace qpp::cad;

sflow_context_t::sflow_context_t() {

}

sflow_node_t::sflow_node_t() {

}

bool sflow_node_t::can_be_as_input() {
  return true;
}

bool sflow_node_t::can_be_as_output() {
  return true;
}

void sflow_node_t::execute(sflow_context_t *fl_ctx) {

}
