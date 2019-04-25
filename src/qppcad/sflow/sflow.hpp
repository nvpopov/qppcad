#ifndef QPPCAD_SINGLE_FLOW
#define QPPCAD_SINGLE_FLOW

#include <qppcad/qppcad.hpp>
#include <qppcad/sflow/sflow_node.hpp>
#include <qppcad/sflow/sflow_parameter.hpp>
#include <qppcad/sflow/sflow_status.hpp>

namespace qpp {

  namespace cad {

    /**
     * @brief The sflow_context_t class
     */
    class sflow_context_t {

      public:

        std::vector<std::shared_ptr<sflow_node_t> > m_nodes;
        std::vector<sflow_connectivity_data_t> m_connectivity;

        sflow_context_t();

        void add_node(std::shared_ptr<sflow_node_t> node);
        sflow_status_e connect_node(std::shared_ptr<sflow_node_t> out_node,
                                    std::shared_ptr<sflow_node_t> inp_node,
                                    size_t out_socket,
                                    size_t inp_socket);

        void clear_context();
        void clear_outer_nodes();
        void clear_connectivity();

        void compile_flow();
        void execute();
        void execute_traverse(sflow_node_t *cur_node,
                              sflow_node_t *prev_node,
                              bool debug_print = false);

        sflow_status_e propagate_data(sflow_connectivity_data_t *cd, bool copy_par = true);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
