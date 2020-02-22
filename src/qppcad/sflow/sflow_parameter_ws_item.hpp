#ifndef QPPCAD_SFLOW_PARAMETER_WS_ITEM
#define QPPCAD_SFLOW_PARAMETER_WS_ITEM

#include <qppcad/core/qppcad.hpp>
#include <qppcad/sflow/sflow_parameter.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

  namespace cad {

    /*
     This type of parameter is separated from the main parameters(sflow_parameter.hpp)
     for a simpler compilation of the test program and the main program
     */
    class sf_parameter_ws_item_t : public sf_parameter_t {

        QPP_OBJECT(sf_parameter_ws_item_t, sf_parameter_t)

      public:

        std::shared_ptr<ws_item_t> m_value{nullptr};
        sf_parameter_e get_param_meta() override ;
        std::shared_ptr<sf_parameter_t> clone() override;

    };

  } // namespace cad

} // namespace qpp

#endif
