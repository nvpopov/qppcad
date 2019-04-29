#include <qppcad/ws_item/sflow_register.hpp>
#include <qppcad/sflow/sflow_base_nodes_int.hpp>
#include <qppcad/sflow/sflow_base_nodes_float.hpp>
#include <qppcad/sflow/sflow_base_nodes_vec3.hpp>
#include <qppcad/sflow/sflow_base_nodes_ws_item.hpp>
#include <qppcad/sflow/sflow_base_nodes_xgeom.hpp>

using namespace qpp;
using namespace qpp::cad;


void sf_node_reg_hlp_t::reg_sf_info(ws_item_behaviour_manager_t *b_mgr) {

  auto int_g = b_mgr->reg_sflow_grp("int nodes");
  auto float_g = b_mgr->reg_sflow_grp("float nodes");
  auto v3f_g = b_mgr->reg_sflow_grp("v3f nodes");
  auto wsi_g = b_mgr->reg_sflow_grp("ws_item");
  auto xgeom_g = b_mgr->reg_sflow_grp("xgeom");

  /**********************************************************************
   * integer nodes
   **********************************************************************/
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_int_prop_node_t>(b_mgr, "int", int_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_int_final_node_t>(b_mgr, "show int", int_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_int_p_const_node_t>(b_mgr, "int + const", int_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_int_sum_int_node_t>(b_mgr, "int + int", int_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_int_mul_int_node_t>(b_mgr, "int * int", int_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_int_div_int_node_t>(b_mgr, "int / int", int_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_int_patb_const_node_t>(b_mgr, "(int + a)*b", int_g);

  /**********************************************************************
   * float nodes
   **********************************************************************/
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_float_prop_node_t>(b_mgr, "float", float_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_float_final_node_t>(b_mgr, "show float", float_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_float_p_const_node_t>(b_mgr, "float + const", float_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_float_sum_float_node_t>(b_mgr, "float + float", float_g);

  /**********************************************************************
   * vector3f nodes
   **********************************************************************/
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_v3f_prop_node_t>(b_mgr, "v3f", v3f_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_v3f_final_node_t>(b_mgr, "show v3f", v3f_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_v3f_sum_v3f_node_t>(b_mgr, "v3f + v3f", v3f_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_v3f_dot_node_t>(b_mgr, "v3f . v3f", v3f_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_v3f_normalized_node_t>(b_mgr, "v3f.normalized()", v3f_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_v3f_norm_node_t>(b_mgr, "v3f.norm()", v3f_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_v3f_cross_node_t>(b_mgr, "v3f x v3f", v3f_g);

  /**********************************************************************
   * ws_item nodes
   **********************************************************************/
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_claim_xgeom_node_t>(b_mgr, "claim xgeom", wsi_g);

  /**********************************************************************
   * xgeom nodes
   **********************************************************************/
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_xgeom_nat_node_t>(b_mgr, "xgeom.nat()", xgeom_g);
  sf_node_reg_hlp_t::reg_sf_info_fbr<sf_xgeom_ntypes_node_t>(b_mgr, "xgeom.ntypes()", xgeom_g);

}
