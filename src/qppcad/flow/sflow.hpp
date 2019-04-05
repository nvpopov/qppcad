#ifndef QPP_CAD_SINGLE_FLOW
#define QPP_CAD_SINGLE_FLOW

#include <qppcad/qppcad.hpp>

namespace qpp {

  namespace cad {

    class sflow_node_t;

    class sflow_context_t {

    public:

      std::vector<sflow_node_t*> m_nodes;
      std::vector<std::tuple<int,int>> m_con_data;
      sflow_context_t();

    };

    class sflow_node_t {

    public:

      std::string m_node_name;

      sflow_node_t();
      virtual bool can_be_as_input();
      virtual bool can_be_as_output();
      void pre_execute(sflow_context_t *fl_ctx);
      virtual void execute(sflow_context_t *fl_ctx);
      void post_execute(sflow_context_t *fl_ctx);

    };

  }

}

#endif
