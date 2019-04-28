#ifndef QPPCAD_SFLOW_BASE_NODES_XGEOM
#define QPPCAD_SFLOW_BASE_NODES_XGEOM

#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    class sf_xgeom_nat_node_t : public sflow_node_t {
        QPP_OBJECT(sf_xgeom_nat_node_t, sflow_node_t)
      public:
        sf_xgeom_nat_node_t();
        bool execute_ex() override;
    };


  } // namespace cad

} // namespace qpp

#endif
