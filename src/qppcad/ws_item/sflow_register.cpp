#include <qppcad/ws_item/sflow_register.hpp>
#include <qppcad/sflow/sflow_base_nodes_int.hpp>
#include <qppcad/sflow/sflow_base_nodes_float.hpp>

using namespace qpp;
using namespace qpp::cad;


void sflow_node_reg_helper_t::reg_sflow_info(ws_item_behaviour_manager_t *bhv_mgr) {

  auto int_grp = bhv_mgr->reg_sflow_grp("int nodes");
  auto float_grp = bhv_mgr->reg_sflow_grp("float nodes");

  sflow_node_reg_helper_t::reg_sflow_info_fbr<sf_int_prop_node_t>(
        bhv_mgr, "int", int_grp);

  sflow_node_reg_helper_t::reg_sflow_info_fbr<sf_int_final_node_t>(
        bhv_mgr, "show int", int_grp);

  sflow_node_reg_helper_t::reg_sflow_info_fbr<sf_int_p_const_node_t>(
        bhv_mgr, "int + const", int_grp);

  sflow_node_reg_helper_t::reg_sflow_info_fbr<sf_int_sum_i_node_t>(
        bhv_mgr, "int + int", int_grp);

  sflow_node_reg_helper_t::reg_sflow_info_fbr<sf_int_patb_const_node_t>(
        bhv_mgr, "(int + a)*b", int_grp);

  sflow_node_reg_helper_t::reg_sflow_info_fbr<sf_float_prop_node_t>(
        bhv_mgr, "float", float_grp);

  sflow_node_reg_helper_t::reg_sflow_info_fbr<sf_float_p_const_node_t>(
        bhv_mgr, "float + const", float_grp);

  sflow_node_reg_helper_t::reg_sflow_info_fbr<sf_float_final_node_t>(
        bhv_mgr, "show float", float_grp);

}
