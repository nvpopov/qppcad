#ifndef QPPCAD_SFLOW_BASE_NODES
#define QPPCAD_SFLOW_BASE_NODES

#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    /**
     * @brief The sflow_soi_node_t class
     */
    class sf_int_prop_node_t : public sflow_node_t {
        QPP_OBJECT(sf_int_prop_node_t, sflow_node_t)
      public:
        sf_int_prop_node_t();
        bool execute_ex() override;
        bool is_single_node() override;
    };

    /**
     * @brief The sflow_sgi_node_t class
     */
    class sf_int_final_node_t : public sflow_node_t {
        QPP_OBJECT(sf_int_final_node_t, sflow_node_t)
      public:
        sf_int_final_node_t();
        bool execute_ex() override;
        bool is_single_node() override;
    };

    /**
     * @brief The sflow_sii_node_t class
     */
    class sf_int_p_const_node_t : public sflow_node_t {
        QPP_OBJECT(sf_int_p_const_node_t, sflow_node_t)
      public:
        sf_int_p_const_node_t();
        bool execute_ex() override;
    };

    class sf_int_patb_const_node_t : public sflow_node_t {
        QPP_OBJECT(sf_int_patb_const_node_t, sflow_node_t)
      public:
        sf_int_patb_const_node_t();
        bool execute_ex() override;
    };

    /**
     * @brief The sflow_sum_i_node_t class
     */
    class sf_int_sum_i_node_t : public sflow_node_t {
        QPP_OBJECT(sf_int_sum_i_node_t, sflow_node_t)
      public:
        sf_int_sum_i_node_t();
        bool execute_ex() override;
    };

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
