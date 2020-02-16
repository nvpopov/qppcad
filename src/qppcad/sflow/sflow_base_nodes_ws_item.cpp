#include <qppcad/sflow/sflow_base_nodes_ws_item.hpp>
#include <qppcad/sflow/sflow_parameter_ws_item.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_claim_xgeom_node_t::sf_claim_xgeom_node_t() {

  m_node_name = "claim xgeom";

  m_out_schema = {
    {sflow_parameter_e::sfpar_xgeom, 0, "dst"}
  };

  m_ipl_schema = {
    {sflow_parameter_e::sfpar_ws_item, "", true}
  };

}

bool sf_claim_xgeom_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_xgeom_t>(0, m_outs);
  auto ipl0 = get_pars_as<sflow_parameter_ws_item_t>(0, m_ipl);

  geom_view_t *casted{nullptr};

  if (ipl0 && ipl0->m_value) casted = ipl0->m_value->cast_as<geom_view_t>();

  if (out0 && casted) {
      casted->m_geom->clone(out0->m_value);
    } else {
      return false;
    }

  return true;

}

bool sf_claim_xgeom_node_t::is_single_node() {
  return true;
}
