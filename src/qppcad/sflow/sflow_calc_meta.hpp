#ifndef QPPCAD_SFLOW_CALC_META
#define QPPCAD_SFLOW_CALC_META

#include <qppcad/qppcad.hpp>
#ifdef EXTENDED_SFLOW
#include <qppcad/sflow/sflow_parameter_ws_item.hpp>
#endif

namespace qpp {

  namespace cad {

    struct sflow_calc_meta_t {
        std::set<size_t> m_involved_nodes;
        void clear() {
          m_involved_nodes.clear();
        }
    };

    struct sflow_calc_meta_global_t {

    #ifdef EXTENDED_SFLOW
      std::vector<std::shared_ptr<ws_item_t>> m_involved_ws_items;
    #endif

      void clear() {
         #ifdef EXTENDED_SFLOW
          m_involved_ws_items.clear();
         #endif
      }

    };

  } // namespace cad

} // namespace qpp

#endif
