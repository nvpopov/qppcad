#ifndef QPP_CAD_SINGLE_FLOW
#define QPP_CAD_SINGLE_FLOW

#include <qppcad/qppcad.hpp>
#include <variant>

namespace qpp {

  namespace cad {

    class sflow_node_t;

    using sflow_parameters_t = std::variant<
    int,
    bool,
    std::string,
    float, vector3<float>,
    std::monostate
    >;

    /**
     * @brief The sflow_context_t class
     */
    class sflow_context_t {

    public:

      std::vector<std::shared_ptr<sflow_node_t> > m_nodes;
      std::vector<std::tuple<size_t, size_t> > m_con_data;
      std::vector<std::vector<size_t> > m_raw_calc_flow;
      std::vector<bool> m_main_flow;

      sflow_context_t();

      void add_node(std::shared_ptr<sflow_node_t> node);
      void connect_node(std::shared_ptr<sflow_node_t> node1,
                        std::shared_ptr<sflow_node_t> node2);
      void mark_outer_nodes();
      void make_raw_calc_flows();
      void make_calc_flows();
      void traverse_calc_flow(sflow_node_t *cur_node, std::vector<size_t> &node_idx);
      void compile_flow();
      void execute();
      void execute_traverse(sflow_node_t *cur_node);

    };

    /**
     * @brief The sflow_node_t class
     */
    class sflow_node_t {

    public:

      std::string m_node_name;
      bool m_is_outer{false};
      size_t m_idx_lookup{0};

      std::vector<std::shared_ptr<sflow_node_t>> m_inputs;
      std::vector<std::shared_ptr<sflow_node_t>> m_outputs;

      size_t m_total_gain{0};

      sflow_node_t();
      virtual bool can_be_as_input();
      virtual bool can_be_as_output();
      void pre_execute(sflow_context_t *fl_ctx);
      virtual void execute(sflow_context_t *fl_ctx);
      void post_execute(sflow_context_t *fl_ctx);

      void feed_inputs(size_t input_idx, sflow_parameters_t value);

    };

  }

}

#endif
