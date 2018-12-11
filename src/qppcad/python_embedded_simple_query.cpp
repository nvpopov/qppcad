#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/python_simple_query.hpp>

using namespace qpp;
using namespace qpp::cad;

PYBIND11_EMBEDDED_MODULE(sq, m) {

  m.def("sel_ws",  &simple_query::select_ws);
  m.def("sel_it",  &simple_query::select_itm);
  m.def("sel", &simple_query::sel_cnt);
  m.def("sel", &simple_query::sel_cnt_list);
  m.def("sel", &simple_query::sel_cnt_type);
  m.def("unsel", &simple_query::unsel_cnt);
  m.def("unsel", &simple_query::unsel_cnt_list);
  m.def("unsel", &simple_query::unsel_cnt_type);
  m.def("mode", &simple_query::edit_mode);
  m.def("t", &simple_query::translate_selected);
  m.def("pt_color", &simple_query::ptable_set_color_by_number);
  m.def("pt_color", &simple_query::ptable_set_color_by_name);
  m.def("pt_r", &simple_query::ptable_set_radius_by_name);
  m.def("pt_r", &simple_query::ptable_set_radius_by_number);
  m.def("get_pt_color", &simple_query::ptable_get_color_by_number);
  m.def("get_pt_color", &simple_query::ptable_get_color_by_name);
  m.def("get_pt_r", &simple_query::ptable_get_radius_by_name);
  m.def("get_pt_r", &simple_query::ptable_get_radius_by_number);
}
