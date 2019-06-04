#ifndef QPPCAD_PYTHON_SQ_H
#define QPPCAD_PYTHON_SQ_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <qppcad/workspace.hpp>

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
//#include <pybind11/eigen.h>
#pragma pop_macro("slots")

namespace py = pybind11;

namespace qpp {

  namespace cad {

    struct cp2k_helper_t {
        static void save_neb_data(std::shared_ptr<ws_item_t> g_start,
                                  std::shared_ptr<ws_item_t> g_end,
                                  std::string path);
    };

  } // namespace cad


} // namespace qpp

#endif
