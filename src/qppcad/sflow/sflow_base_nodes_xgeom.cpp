#include <qppcad/sflow/sflow_base_nodes_xgeom.hpp>
#include <geom/xgeom.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_xgeom_nat_node_t::sf_xgeom_nat_node_t() {

  m_node_name = "number of atoms";

  m_inp_types = {
    {sflow_parameter_e::sfpar_xgeom, 0, "xgeom"}
  };

  m_out_types = {
    {sflow_parameter_e::sfpar_int, 0, "nat"}
  };

}

bool sf_xgeom_nat_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sflow_parameter_xgeom_t>(0, m_inps);

  if (out0 && inp0) {
      out0->m_value = inp0->m_value.nat();
    } else {
      return false;
    }

  return true;

}
