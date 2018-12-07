#ifndef QPP_CAD_PYTHON_SQ_H
#define QPP_CAD_PYTHON_SQ_H

#include <qppcad/qppcad.hpp>

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

namespace py = pybind11;

namespace qpp {

  namespace cad {

    class simple_query {
       public:
        static void select_ws(int ws_idx);
        static void select_itm(int itm_idx);
        static void sel_cnt(int cnt_idx);
        static void sel_cnt_list(py::list sel_list);
        static void sel_cnt_type(py::str sel_type);
        static void edit_mode(int mode);
        static void unsel_cnt(int cnt_idx);
        static void unsel_cnt_list(py::list sel_list);
        static void unsel_cnt_type(py::str sel_type);
        static void translate_selected(float tx, float ty, float tz);

    };

  }

}


#endif
