#ifndef QPPCAD_SFLOW_BASE_NODES_FLOAT
#define QPPCAD_SFLOW_BASE_NODES_FLOAT

#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    class sf_float_prop_node_t : public sflow_node_t {
        QPP_OBJECT(sf_float_prop_node_t, sflow_node_t)
      public:
        sf_float_prop_node_t();
        bool execute_ex() override;
        bool is_single_node() override;
    };

    class sf_float_p_const_node_t : public sflow_node_t {
        QPP_OBJECT(sf_float_p_const_node_t, sflow_node_t)
      public:
        sf_float_p_const_node_t();
        bool execute_ex() override;
    };

    class sf_float_sum_float_node_t : public sflow_node_t {
        QPP_OBJECT(sf_float_sum_float_node_t, sflow_node_t)
      public:
        sf_float_sum_float_node_t();
        bool execute_ex() override;
    };

    class sf_float_final_node_t : public sflow_node_t {
        QPP_OBJECT(sf_float_final_node_t, sflow_node_t)
      public:
        sf_float_final_node_t();
        bool execute_ex() override;
        bool is_single_node() override;
    };

  } // namespace cad

} // namespace qpp

#endif
