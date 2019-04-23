#ifndef QPPCAD_SFLOW_BASE_NODES
#define QPPCAD_SFLOW_BASE_NODES

#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    /**
     * @brief The sflow_soi_node_t class
     */
    class sf_i_prop_node_t : public sflow_node_t {

        QPP_OBJECT(sf_i_prop_node_t, sflow_node_t)

      public:

        sf_i_prop_node_t();
        bool execute_ex() override;

    };

    /**
     * @brief The sflow_sgi_node_t class
     */
    class sf_i_final_node_t : public sflow_node_t {

        QPP_OBJECT(sf_i_final_node_t, sflow_node_t)

      public:

        sf_i_final_node_t();
        bool execute_ex() override;

    };

    /**
     * @brief The sflow_sii_node_t class
     */
    class sf_i_p_const_node_t : public sflow_node_t {

        QPP_OBJECT(sf_i_p_const_node_t, sflow_node_t)

      public:

        sf_i_p_const_node_t();
        bool execute_ex() override;

    };

    /**
     * @brief The sflow_sum_i_node_t class
     */
    class sf_i_sum_i_node_t : public sflow_node_t {

        QPP_OBJECT(sf_i_sum_i_node_t, sflow_node_t)

      public:

        sf_i_sum_i_node_t();
        bool execute_ex() override;

    };

    class sf_f_prop_node_t : public sflow_node_t {

        QPP_OBJECT(sf_f_prop_node_t, sflow_node_t)

      public:

        sf_f_prop_node_t();
        bool execute_ex() override;

    };

    class sf_f_p_const_node_t : public sflow_node_t {

        QPP_OBJECT(sf_f_p_const_node_t, sflow_node_t)

      public:

        sf_f_p_const_node_t();
        bool execute_ex() override;

    };

  } // namespace cad

} // namespace qpp

#endif
