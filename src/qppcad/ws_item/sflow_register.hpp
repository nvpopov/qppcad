#ifndef QPPCAD_SFLOW_REGISTER_NODES_INFO
#define QPPCAD_SFLOW_REGISTER_NODES_INFO

#include <qppcad/core/qppcad.hpp>
#include <qppcad/sflow/sflow_node.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

namespace cad {

struct sf_node_reg_hlp_t {

  template<typename T>
  static void reg_sf_info_fbr(ws_item_behaviour_manager_t *bhv_mgr,
                              std::string full_name,
                              size_t group_hash) {
    bhv_mgr->reg_reg_sf_fbr(full_name, group_hash, [](){return std::make_shared<T>();});
  }
  static void reg_sf_info(ws_item_behaviour_manager_t *bhv_mgr);

}; // struct sflow_node_registration_helper_t

} // namespace qpp::cad

} // namespace qpp

#endif
