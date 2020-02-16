#ifndef QPPCAD_SFLOW_NODE
#define QPPCAD_SFLOW_NODE

#include <qppcad/core/qppcad.hpp>
#include <qppcad/sflow/sflow_parameter.hpp>
#include <qppcad/core/qpp_object.hpp>
#include <qppcad/sflow/sflow_calc_meta.hpp>

#ifdef EXTENDED_SFLOW
#include <qppcad/sflow/sflow_parameter_ws_item.hpp>
#endif

namespace qpp {

  namespace cad {

    class sflow_node_t;
    struct sflow_connectivity_data_t;

    struct sflow_socket_info_t {

        sflow_parameter_e m_type{sflow_parameter_e::sfpar_none};
        size_t m_tot_con{0}; // total connections
        std::string m_sck_name{"Unknown"};

    };

    struct sflow_inplace_parameter_t {

        sflow_parameter_e m_type{sflow_parameter_e::sfpar_none};
        std::string m_name{"Unknown"};
        bool m_editable{false};

    };

    struct sflow_connectivity_data_t {

        std::shared_ptr<sflow_node_t> m_inp_node{nullptr};
        std::shared_ptr<sflow_node_t> m_out_node{nullptr};
        std::optional<size_t> m_inp_sck;
        std::optional<size_t> m_out_sck;

    };

    /**
     * @brief The sflow_node_t class
     */
    class sflow_node_t {

      public:

        size_t m_node_type_hash;

        /* The name that will be displayed in the node widget */
        std::string m_node_name;

        /* Means that this node has no input links and is external to the calculation flow. */
        bool m_is_outer{true};

        /* This value is interpreted as the number of transitions to this node from other nodes
         * during the execution of the calculation flow */
        size_t m_total_gain{0};

        size_t m_idx_lookup{0};

        opt<int> m_front_end_width{std::nullopt};

        sflow_calc_meta_t m_calc_meta;

        std::vector<sflow_socket_info_t> m_inp_schema;
        std::vector<std::shared_ptr<sflow_parameter_t>> m_inps;

        std::vector<sflow_socket_info_t> m_out_schema;
        std::vector<std::shared_ptr<sflow_parameter_t>> m_outs;

        std::vector<sflow_inplace_parameter_t> m_ipl_schema;
        std::vector<std::shared_ptr<sflow_parameter_t>> m_ipl;

        template<typename TYPE_CLASS>
        bool validate_data_vec(std::vector<TYPE_CLASS> &data_types,
                               std::vector<std::shared_ptr<sflow_parameter_t> > &data) {

          if (data_types.size() != data.size()) return false;
          for (size_t i = 0; i < data_types.size(); i++)
            if (data[i] && data[i]->get_param_meta() != data_types[i].m_type)
              return false;

          return true;

        }

        template<typename TYPE_CLASS>
        void fill_data_vec(std::vector<TYPE_CLASS> &data_types,
                           std::vector<std::shared_ptr<sflow_parameter_t> > &data) {

          if (data_types.size() != data.size()) data.resize(data_types.size(), nullptr);

          for (size_t i = 0; i < data_types.size(); i++)
            if (!data[i]) {

                switch (data_types[i].m_type) {

                  case sflow_parameter_e::sfpar_int : {
                      data[i] = std::make_shared<sflow_parameter_int_t>();
                      break;
                    }

                  case sflow_parameter_e::sfpar_float : {
                      data[i] = std::make_shared<sflow_parameter_float_t>();
                      break;
                    }

                  case sflow_parameter_e::sfpar_v3f : {
                      data[i] = std::make_shared<sflow_parameter_v3f_t>();
                      break;
                    }

                  #ifdef EXTENDED_SFLOW
                  case sflow_parameter_e::sfpar_ws_item : {
                      data[i] = std::make_shared<sflow_parameter_ws_item_t>();
                      break;
                    }
                  #endif

                  case sflow_parameter_e::sfpar_xgeom : {
                      data[i] = std::make_shared<sflow_parameter_xgeom_t>();
                      break;
                    }

                  case sflow_parameter_e::sfpar_bool : {
                      data[i] = std::make_shared<sflow_parameter_bool_t>();
                      break;
                    }

                  default :  {
                      break;
                    }

                  }

              } // end for

        }

        /**
         * @brief has_ipls
         * @return Returns true, if there are inplace parameters. Helper for frontend calls
         */
        bool has_ipls();

        /**
         * @brief Explicitly creates a vector of inplace parameters. Helper for frontend calls
         */
        void explicit_create_ipl();

        template<typename T>
        T* get_pars_as(size_t inp_idx, std::vector<std::shared_ptr<sflow_parameter_t>> &data_vec) {

          if (inp_idx >= data_vec.size()) return nullptr;
          if (!data_vec[inp_idx]) return nullptr;
          return data_vec[inp_idx]->cast_as<T>();

        }

        //if false flow stops
        bool execute();
        virtual bool execute_ex();

        /**
         * @brief A single node is such a node with one parameter that does not have a name,
         * but has the ability to edit the value of the parameter visually
         */
        virtual bool is_single_node();

        sflow_node_t();

    };

    template<class T>
    constexpr sflow_parameter_e type_to_sflow_parameter{sflow_parameter_e::sfpar_none};

    template<>
    constexpr sflow_parameter_e type_to_sflow_parameter<float>{sflow_parameter_e::sfpar_float};

    template<>
    constexpr sflow_parameter_e type_to_sflow_parameter<double>{sflow_parameter_e::sfpar_float};

    template<>
    constexpr sflow_parameter_e type_to_sflow_parameter<int>{sflow_parameter_e::sfpar_int};

    template<>
    constexpr sflow_parameter_e type_to_sflow_parameter<bool>{sflow_parameter_e::sfpar_bool};

#ifdef EXTENDED_SFLOW

    template<>
    constexpr sflow_parameter_e
    type_to_sflow_parameter<ws_item_t> {
      sflow_parameter_e::sfpar_ws_item
    };

    template<>
    constexpr sflow_parameter_e
    type_to_sflow_parameter<xgeometry<float, periodic_cell<float> > > {
      sflow_parameter_e::sfpar_xgeom
    };

#endif

    template<auto FUNC, typename ... args>
    class sf_func_node_t final : public sflow_node_t {

      public:

        sf_func_node_t() {

        };

        bool execute_ex() override {
          return true;
        };

        bool is_single_node() override {
          return true;
        };

    };

  } // namespace cad

} // namespace qpp

#endif
