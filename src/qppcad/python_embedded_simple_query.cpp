#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/python_simple_query.hpp>

using namespace qpp;
using namespace qpp::cad;

PYBIND11_EMBEDDED_MODULE(sq, m) {

  py::module sel = m.def_submodule("sel", "Selection routines");

  sel.def("ws",  &simple_query::select_ws);
  sel.def("it",  &simple_query::select_itm);
  sel.def("c", &simple_query::sel_cnt);
  sel.def("c", &simple_query::sel_cnt_list);
  sel.def("c", &simple_query::sel_cnt_type);
  sel.def("c", &simple_query::sel_cnt_all);
  sel.def("uc", &simple_query::unsel_cnt_all);
  sel.def("uc", &simple_query::unsel_cnt);
  sel.def("uc", &simple_query::unsel_cnt_list);
  sel.def("uc", &simple_query::unsel_cnt_type);
  sel.def("c", &simple_query::get_sel);
  sel.def("sph", &simple_query::sel_cnt_sphere);
  sel.def("inv", &simple_query::sel_invert);

  m.def("mode", &simple_query::edit_mode);
  m.def("rebond", &simple_query::rebond);
  m.def("t", &simple_query::translate_selected);
  m.def("bg", &simple_query::set_ws_bg);
  m.def("bg", [](float r, float g, float b)
  {simple_query::set_ws_bg(vector3<float>(r,g,b));});

  py::module pt = m.def_submodule("pt", "Periodic table manipulations");
  pt.def("c", &simple_query::ptable_set_color_by_number);
  pt.def("c", &simple_query::ptable_set_color_by_name);
  pt.def("r", &simple_query::ptable_set_radius_by_name);
  pt.def("r", &simple_query::ptable_set_radius_by_number);
  pt.def("c", &simple_query::ptable_get_color_by_number);
  pt.def("c", &simple_query::ptable_get_color_by_name);
  pt.def("r", &simple_query::ptable_get_radius_by_name);
  pt.def("r", &simple_query::ptable_get_radius_by_number);

  py::module cam = m.def_submodule("cam", "Camera manipulation");
  cam.def("t", &simple_query::camera_move);
  cam.def("ry", &simple_query::camera_rotate_yaw);
  cam.def("rp", &simple_query::camera_rotate_pitch);
  cam.def("zoom", &simple_query::camera_zoom);
  cam.def("proj", &simple_query::camera_mode);

  py::module sv = m.def_submodule("sv", "Selective visibility manipulation");
  sv.def("state", &simple_query::sv_get);
  sv.def("set", &simple_query::sv_edit);
  sv.def("set", &simple_query::sv_edit_list);
  sv.def("set", &simple_query::sv_edit_all);

  py::module cl = m.def_submodule("cl", "Custom labels");
  cl.def("state", &simple_query::cl_get);
  cl.def("set_st", &simple_query::set_cl_state);
  cl.def("set_t", &simple_query::set_cl_text);
  //cam.def("tu", )
}
