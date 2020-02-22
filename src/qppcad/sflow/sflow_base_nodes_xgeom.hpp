#ifndef QPPCAD_SFLOW_BASE_NODES_XGEOM
#define QPPCAD_SFLOW_BASE_NODES_XGEOM

#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    class sf_xgeom_nat_node_t final : public sf_node_t {

      public:

        sf_xgeom_nat_node_t();
        bool execute_ex() override;

    };

    class sf_xgeom_ntypes_node_t final : public sf_node_t {

      public:

        sf_xgeom_ntypes_node_t();
        bool execute_ex() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
