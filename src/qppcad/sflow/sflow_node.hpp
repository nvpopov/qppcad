#ifndef QPPCAD_SFLOW_NODE
#define QPPCAD_SFLOW_NODE

#include <qppcad/qppcad.hpp>
#include <qppcad/sflow/sflow_parameter.hpp>

namespace qpp {

  namespace cad {

    class sflow_node_t;
    class sflow_connectivity_data_t;

    class sflow_pin_info_t {
      public:
        sflow_parameter_e m_type{sflow_parameter_e::sfpar_none};
        size_t m_total_con{0};
        std::string m_pin_name{"Unknown"};
    };

    class sflow_connectivity_data_t {
      public:
        std::shared_ptr<sflow_node_t> m_inp_node{nullptr};
        std::shared_ptr<sflow_node_t> m_out_node{nullptr};
        std::optional<size_t> m_inp_socket;
        std::optional<size_t> m_out_socket;
    };

    /**
     * @brief The sflow_node_t class
     */
    class sflow_node_t {

    public:

      std::string m_node_name;
      bool m_is_outer{true};
      size_t m_idx_lookup{0};

      std::vector<sflow_pin_info_t> m_inp_types;
      std::vector<sflow_pin_info_t> m_out_types;
      std::vector<std::shared_ptr<sflow_parameter_t>> m_inps;
      std::vector<std::shared_ptr<sflow_parameter_t>> m_outs;

      size_t m_total_gain{0};
      bool validate_inputs();

      //if false flow stops
      virtual bool execute();

      sflow_node_t();

    };

  }

}

#endif
