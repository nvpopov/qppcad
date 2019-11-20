#ifndef QPPCAD_WS_ITEM_SUBSYSTEM
#define QPPCAD_WS_ITEM_SUBSYSTEM

#include <qppcad/core/qppcad.hpp>
#include <qppcad/core/qpp_object.hpp>
#include <qppcad/core/iupdatable.hpp>
#include <qppcad/core/serializable.hpp>

namespace qpp {

  namespace cad {

    class ws_item_subsystem_t :
        public qpp_object_t,
        public serializable_t,
        public iupdatable_t{

      QPP_OBJECT(ws_item_subsystem_t, qpp_object_t)

    };

  }

}

#endif
