#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/python_simple_query.hpp>

using namespace qpp;
using namespace qpp::cad;

PYBIND11_EMBEDDED_MODULE(sq, m) {

  py::enum_<qc_file_fmt>(m, "ffmt", py::arithmetic(), "qc file format")
      .value("std_xyz", qc_file_fmt::standart_xyz)
      .value("vasp_poscar", qc_file_fmt::vasp_poscar)
      .value("vasp_outcar_md", qc_file_fmt::vasp_outcar_md)
      .value("ff_out", qc_file_fmt::firefly_output)
      .value("ff_inp", qc_file_fmt::firefly_input)
      .value("cp2k_out", qc_file_fmt::cp2k_output)
      .value("cp2k_coord", qc_file_fmt::cp2k_coord_cell_section)
      .value("json", qc_file_fmt::standart_xyz)
      .value("uc", qc_file_fmt::qpp_uc)
      .value("unk", qc_file_fmt::unknown_fileformat)
      .export_values();

  m.def("qopen", &simple_query::open_file);
  m.def("qopen", &simple_query::open_file_query);

  m.def("mode", &simple_query::edit_mode);

  m.def("bg", &simple_query::set_ws_bg);
  m.def("bg", [](float r, float g, float b) {simple_query::set_ws_bg(vector3<float>(r,g,b));} );
  m.def("gpos", &simple_query::gizmo_pos);

  py::module tools = m.def_submodule("tools", "Generic tools");
  tools.def("rebond", &simple_query::rebond);
  tools.def("t", &simple_query::translate_selected);
  tools.def("sc", &simple_query::make_super_cell);
  tools.def("set_chg", &simple_query::set_charge);

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
  sel.def("get", &simple_query::get_sel);
  sel.def("sph", &simple_query::sel_cnt_sphere);
  sel.def("inv", &simple_query::sel_invert);
  sel.def("hemisph", &simple_query::sel_hemisphere);

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
  cam.def("copy", &simple_query::copy_camera_from_ws);

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
