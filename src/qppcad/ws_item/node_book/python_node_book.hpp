#ifndef QPPCAD_PYTHON_NODE_BOOK
#define QPPCAD_PYTHON_NODE_BOOK

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

namespace py = pybind11;

#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/ws_item/ws_item_trampoline.hpp>

namespace qpp {

  namespace cad {

    struct py_node_book_reg_helper_t {

        static void reg(
            py::module &module,
            py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base);

    };

  } // namespace cad

} // namespace qpp

#endif
