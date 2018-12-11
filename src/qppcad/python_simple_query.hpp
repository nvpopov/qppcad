#ifndef QPP_CAD_PYTHON_SQ_H
#define QPP_CAD_PYTHON_SQ_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>

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

        static void ptable_set_color_by_number(int num, float r, float g, float b);
        static void ptable_set_color_by_name(std::string name, float r, float g, float b);
        static void ptable_set_radius_by_number(int num, float r);
        static void ptable_set_radius_by_name(std::string name, float r);

        static vector3<float> ptable_get_color_by_number(int num);
        static vector3<float> ptable_get_color_by_name(std::string name);
        static float ptable_get_radius_by_number(int num);
        static float ptable_get_radius_by_name(std::string name);
    };

  }

}


#endif
