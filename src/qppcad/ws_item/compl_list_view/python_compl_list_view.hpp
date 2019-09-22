#ifndef QPPCAD_PYTHON_CLV_VIEW
#define QPPCAD_PYTHON_CLV_VIEW

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#pragma pop_macro("slots")

#include <qppcad/ws_item/compl_list_view/compl_list_view.hpp>
#include <qppcad/ws_item/ws_item_trampoline.hpp>

namespace py = pybind11;

namespace qpp {

  namespace cad {

    struct py_compl_list_view_reg_helper_t {

        static void reg(
            py::module &module,
            py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base);

    };

  } // namespace cad

} // namespace qpp

#endif
