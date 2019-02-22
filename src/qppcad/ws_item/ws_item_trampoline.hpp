#ifndef QPP_CAD_WS_ITEM_TRAMPOLINE
#define QPP_CAD_WS_ITEM_TRAMPOLINE

#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/qppcad.hpp>

namespace qpp {

  namespace cad {

    struct py_ws_item_t : ws_item_t, std::enable_shared_from_this<py_ws_item_t> {

        using ws_item_t::ws_item_t;

        size_t get_content_count() override {
          PYBIND11_OVERLOAD_PURE(
                size_t,
                ws_item_t,
                get_content_count);
        }

    };

  }

}


#endif
