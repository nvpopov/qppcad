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
        std::vector<std::shared_ptr<sflow_parameter_t>> m_inps;

        std::vector<sflow_socket_info_t> m_out_types;
        std::vector<std::shared_ptr<sflow_parameter_t>> m_outs;

        std::vector<sflow_inplace_parameter_t> m_ipl_types;
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

                  case sflow_parameter_e::sfpar_bool : {
                      data[i] = std::make_shared<sflow_parameter_bool_t>();
                      break;
                    }

                  default :  {
                      break;
                    }

                  }

              }

        }

        /**
         * @brief has_ipls
         * @return Returns the true, if there are inplace parameters. Helper for frontend calls
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

  } // namespace cad

} // namespace qpp

#endif
