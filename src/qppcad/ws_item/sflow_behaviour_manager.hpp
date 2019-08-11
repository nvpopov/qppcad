#ifndef QPPCAD_SFLOW_BEHAVIOUR_MANAGER
#define QPPCAD_SFLOW_BEHAVIOUR_MANAGER

#include <qppcad/core/qppcad.hpp>
#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    struct sflow_node_group_info_t {
        std::string m_group_name;
        size_t m_group_hash;
    };


    struct sflow_node_info_t {
        std::string m_full_name;
        size_t m_group_hash;
        std::function<std::shared_ptr<sflow_node_t>() > m_fabric;
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
