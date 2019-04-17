#ifndef QPPCAD_SFLOW_BASE_NODES
#define QPPCAD_SFLOW_BASE_NODES

#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    /**
     * @brief The sflow_soi_node_t class
     */
    class sflow_soi_node_t : public sflow_node_t {
      public:
        sflow_soi_node_t();
        bool execute() override;
    };

    /**
     * @brief The sflow_sgi_node_t class
     */
    class sflow_sgi_node_t : public sflow_node_t {
      public:
        sflow_sgi_node_t();
        bool execute() override;
    };

    /**
     * @brief The sflow_sii_node_t class
     */
    class sflow_sii_node_t : public sflow_node_t {
      public:
        sflow_sii_node_t();
        bool execute() override;
    };

  } // namespace cad

} // namespace qpp

#endif
