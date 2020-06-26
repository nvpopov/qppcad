#ifndef QPPCAD_SINGLE_FLOW
#define QPPCAD_SINGLE_FLOW

#include <thread>

#include <qppcad/core/qppcad.hpp>
#include <qppcad/sflow/sflow_node.hpp>
#include <qppcad/sflow/sflow_parameter.hpp>
#include <qppcad/sflow/sflow_status.hpp>
#include <atomic>

namespace qpp {

namespace cad {

/**
     * @brief The sflow_context_t class
     */
class sf_context_t {

private:

  std::vector<std::shared_ptr<sf_node_t> > m_nodes;
  std::vector<sf_connectivity_data_t> m_connectivity;
  sflow_calc_meta_global_t m_calc_meta_global;

  std::atomic_bool m_task_is_being_executed{false};
  std::atomic_bool m_task_has_been_finished{false};
  std::thread m_thread;
  bool m_force_execute{false};

public:

  sf_context_t();

  std::vector<std::shared_ptr<sf_node_t> > &get_nodes();
  void add_node(std::shared_ptr<sf_node_t> node);
  sf_status_e connect_node(std::shared_ptr<sf_node_t> out_node,
                           std::shared_ptr<sf_node_t> inp_node,
                           size_t out_socket,
                           size_t inp_socket);

  void clear_context();
  void clear_outer_nodes();
  void clear_connectivity();

  void force_execute();
  bool is_force_execute();

  void execute_threaded(bool debug_print = false);
  void execute_threaded_fn();
  bool is_finished();
  bool is_running();

  void execute(bool debug_print = false);
  void execute_traverse(sf_node_t *cur_node,
                        sf_node_t *prev_node,
                        bool debug_print = false);

  sf_status_e propagate_data(sf_connectivity_data_t *cd,
                             bool copy_par = true,
                             bool debug_print = true);
  sf_status_e propagate_meta_info(sf_connectivity_data_t *cd, bool debug_print = true);

}; // class sflow_context_t

} // namespace qpp::cad

} // namespace qpp

#endif
