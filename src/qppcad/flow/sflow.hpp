#ifndef QPPCAD_SINGLE_FLOW
#define QPPCAD_SINGLE_FLOW

#include <qppcad/qppcad.hpp>
#include <geom/xgeom.hpp>

namespace qpp {

  namespace cad {

    class sflow_node_t;
    class sflow_connectivity_data_t;

    enum sflow_parameter_e : int {
      sfpar_none,
      sfpar_int,
      sfpar_bool,
      sfpar_float,
      sfpar_v3f,
      sfpar_string,
      sfpar_xgeom
    };

    enum sflow_status_e : int {
      no_error,
      pin1_invalid,
      pin2_invalid,
      types_pin1_not_eq_pin2,
      too_many_inputs,
      propagate_succes,
      propagate_error
    };

    /**
     * @brief The sflow_context_t class
     */
    class sflow_context_t {

    public:

      std::vector<std::shared_ptr<sflow_node_t> > m_nodes;
      std::vector<sflow_connectivity_data_t> m_connectivity;

      sflow_context_t();

      void add_node(std::shared_ptr<sflow_node_t> node);
      sflow_status_e connect_node(std::shared_ptr<sflow_node_t> node1,
                                 std::shared_ptr<sflow_node_t> node2,
                                 size_t pin1,
                                 size_t pin2);

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

    class sflow_parameter_t {
      public:
        virtual sflow_parameter_e get_param_meta();
        virtual std::shared_ptr<sflow_parameter_t> clone() = 0;
        explicit sflow_parameter_t();
    };

    class sflow_parameter_int_t : public sflow_parameter_t {
      public:
        int m_value;
        sflow_parameter_e get_param_meta() override ;
        std::shared_ptr<sflow_parameter_t> clone() override;
    };

    class sflow_parameter_float_t : public sflow_parameter_t {
      public:
        float m_value;
        sflow_parameter_e get_param_meta() override ;
        std::shared_ptr<sflow_parameter_t> clone() override;
    };

    class sflow_parameter_xgeom_t : public sflow_parameter_t {
      public:
        xgeometry<float, periodic_cell<float> > m_value{3};
        sflow_parameter_e get_param_meta() override ;
        std::shared_ptr<sflow_parameter_t> clone() override;
    };

    class sflow_pin_info_t {
      public:
        sflow_parameter_e m_type{sflow_parameter_e::sfpar_none};
        size_t m_total_con{0};
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

    class sflow_soi_node_t : public sflow_node_t {
      public:
       sflow_soi_node_t();
       bool execute() override;
    };

    class sflow_sgi_node_t : public sflow_node_t {
      public:
        sflow_sgi_node_t();
        bool execute() override;
    };

    class sflow_sii_node_t : public sflow_node_t {
      public:
        sflow_sii_node_t();
        bool execute() override;
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
