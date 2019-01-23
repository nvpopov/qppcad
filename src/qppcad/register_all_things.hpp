#ifndef QPP_CAD_REGISTER_ALL_THINGS_THING
#define QPP_CAD_REGISTER_ALL_THINGS_THING

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class registration_helper_t {
      public:

        template<typename T>
        static void reg_ws_item_fbr_t(ws_item_behaviour_manager_t *bhv_mgr){
          bhv_mgr->reg_item_fbr(T::get_type_static(),
                                [](){return std::make_shared<T>();});
        }

        template<typename TYPE_FOR, typename TYPE_OI>
        static void reg_ws_item_obj_insp_t(ws_item_behaviour_manager_t *bhv_mgr){
          bhv_mgr->reg_obj_insp_fbr(TYPE_FOR::get_type_static(),
                                    [](){return std::make_shared<TYPE_OI>();});
        }

        static void reg_ws_item_fbr(ws_item_behaviour_manager_t *bhv_mgr);
        static void reg_ws_item_obj_insp(ws_item_behaviour_manager_t *bhv_mgr);
        static void reg_ws_item_io_bhv(ws_item_behaviour_manager_t *bhv_mgr);
    };

  }

}

#endif
