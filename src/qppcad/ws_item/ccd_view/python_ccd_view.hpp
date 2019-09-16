#ifndef QPPCAD_PYTHON_CCD_VIEW
#define QPPCAD_PYTHON_CCD_VIEW

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#pragma pop_macro("slots")

#include <qppcad/ws_item/ccd_view/ccd_view.hpp>
#include <qppcad/ws_item/ws_item_trampoline.hpp>
#include <io/ccd_programs.hpp>

namespace py = pybind11;

namespace qpp {

  namespace cad {

    struct py_ccd_view_reg_helper_t {

        static void reg(
            py::module &module,
            py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base);

    };

  } // namespace cad

} // namespace qpp

#endif
