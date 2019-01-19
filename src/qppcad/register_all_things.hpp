#ifndef QPP_CAD_REGISTER_ALL_THINGS_THING
#define QPP_CAD_REGISTER_ALL_THINGS_THING

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class registration_helper_t {
      public:
        static void register_ws_item_io_bhv(ws_item_behaviour_manager_t *bhv_mgr);
    };

  }

}

#endif
