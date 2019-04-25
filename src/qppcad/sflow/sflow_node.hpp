#ifndef QPPCAD_SFLOW_NODE
#define QPPCAD_SFLOW_NODE

#include <qppcad/qppcad.hpp>
#include <qppcad/sflow/sflow_parameter.hpp>
#include <qppcad/qpp_object.hpp>

namespace qpp {

  namespace cad {

    class sflow_node_t;
    class sflow_connectivity_data_t;

    struct sflow_socket_info_t {

        sflow_parameter_e m_type{sflow_parameter_e::sfpar_none};
        size_t m_total_con{0};
        std::string m_socket_name{"Unknown"};

    };

    struct sflow_inplace_parameter_t {

        sflow_parameter_e m_type{sflow_parameter_e::sfpar_none};
        std::string m_name{"Unknown"};
        bool m_editable{false};

    };

    struct sflow_connectivity_data_t {

        std::shared_ptr<sflow_node_t> m_inp_node{nullptr};
        std::shared_ptr<sflow_node_t> m_out_node{nullptr};
        std::optional<size_t> m_inp_socket;
        std::optional<size_t> m_out_socket;

    };

    /**
     * @brief The sflow_node_t class
     */
    class sflow_node_t : public qpp_object_t {

        QPP_OBJECT(sflow_node_t, qpp_object_t)

      public:

        std::string m_node_name;
        bool m_is_outer{true};
        size_t m_idx_lookup{0};
        size_t m_total_gain{0};

        std::vector<sflow_socket_info_t> m_inp_types;
        std::vector<sflow_socket_info_t> m_out_types;

        std::vector<std::shared_ptr<sflow_parameter_t>> m_inps;
        std::vector<std::shared_ptr<sflow_parameter_t>> m_outs;

        std::vector<sflow_inplace_parameter_t> m_inplace_types;
        std::vector<std::shared_ptr<sflow_parameter_t>> m_inplace_parameters;

        bool validate_inputs();
        void validate_outputs();
        bool validate_inplace_parameters();
        bool has_inplace_parameters();
        void create_inplace_parameters();

        //if false flow stops
        bool execute();
        virtual bool execute_ex();
        virtual bool is_single_node();

        sflow_node_t();

    };

  }

}

#endif
