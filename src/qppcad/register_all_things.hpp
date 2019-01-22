#ifndef QPP_CAD_REGISTER_ALL_THINGS_THING
#define QPP_CAD_REGISTER_ALL_THINGS_THING

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class registration_helper_t {
      public:
        template<typename T>
        static void register_ws_item_fabric_t(ws_item_behaviour_manager_t *bhv_mgr){
          bhv_mgr->reg_item_fbr(T::get_type_static(),
                                   [](){return std::make_shared<T>();});
        }

        static void register_ws_item_fabric(ws_item_behaviour_manager_t *bhv_mgr);
        static void register_ws_item_io_bhv(ws_item_behaviour_manager_t *bhv_mgr);
    };

  }

}

#endif
