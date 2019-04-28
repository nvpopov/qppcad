#ifndef QPPCAD_SFLOW_PARAMETER_WS_ITEM
#define QPPCAD_SFLOW_PARAMETER_WS_ITEM

#include <qppcad/qppcad.hpp>
#include <qppcad/sflow/sflow_parameter.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

  namespace cad {

    /*
     This type of parameter is separated from the main parameters(sflow_parameter.hpp)
     for a simpler compilation of the test program and the main program
     */
    class sflow_parameter_ws_item_t : public sflow_parameter_t {

        QPP_OBJECT(sflow_parameter_ws_item_t, sflow_parameter_t)

      public:

        std::shared_ptr<ws_item_t> m_value{nullptr};
        sflow_parameter_e get_param_meta() override ;
        std::shared_ptr<sflow_parameter_t> clone() override;

    };

  } // namespace cad

} // namespace qpp

#endif
