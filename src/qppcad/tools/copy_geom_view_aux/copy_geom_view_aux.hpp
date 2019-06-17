#ifndef QPPCAD_TOOL_COPY_GEOM_VIEW_AUX
#define QPPCAD_TOOL_COPY_GEOM_VIEW_AUX
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class copy_geom_view_aux_tool_t : public ws_item_tool_t {
    
      public:
      
        void exec(ws_item_t *item, uint32_t _error_ctx) override;
        
    };
    
  } // namespace cad
 
} // namespace qpp

#endif
