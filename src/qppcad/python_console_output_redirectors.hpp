#ifndef QPP_CAD_PYTHON_OUTPUT_REDIRECTORS_H
#define QPP_CAD_PYTHON_OUTPUT_REDIRECTORS_H

#include <qppcad/qppcad.hpp>
#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

namespace py = pybind11;

namespace qpp {

  namespace cad {

    class python_std_output_redirector_t {
      public:
        void write(py::str data);
    };



  }

}

#endif
