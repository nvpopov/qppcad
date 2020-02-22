#include <qppcad/sflow/sflow_base_nodes_xgeom.hpp>
#include <geom/xgeom.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_xgeom_nat_node_t::sf_xgeom_nat_node_t() {

  m_node_name = "number of atoms";

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_xgeom, "xgeom")
  };

  m_out_schema = {
    make_outs(sf_parameter_e::sfpar_int, "nat")
  };

  m_front_end_width = opt<int>(140);

}

bool sf_xgeom_nat_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_xgeom_t>(0, m_inps);

  if (out0 && inp0) {
      out0->m_value = inp0->m_value.nat();
    } else {
      return false;
    }

  return true;

}

sf_xgeom_ntypes_node_t::sf_xgeom_ntypes_node_t() {

  m_node_name = "number of types";

  m_inp_schema = {
    make_inps(sf_parameter_e::sfpar_xgeom, "xgeom")
  };

  m_out_schema = {
    make_outs(sf_parameter_e::sfpar_int, "ntypes")
  };

  m_front_end_width = opt<int>(140);

}

bool sf_xgeom_ntypes_node_t::execute_ex() {

  auto out0 = get_pars_as<sf_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sf_parameter_xgeom_t>(0, m_inps);

  if (out0 && inp0) {
      out0->m_value = inp0->m_value.n_atom_types();
    } else {
      return false;
    }

  return true;

}
