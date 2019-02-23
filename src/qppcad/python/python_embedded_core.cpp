#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/python/python_console_output_redirectors.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

PYBIND11_EMBEDDED_MODULE(core, m) {

  m.def("clear", []() {
      app_state_t::get_inst()->astate_evd->python_console_clear_requested();
    });

  py::class_<python_std_output_redirector_t> py_redirector(m, "output_redirector");
  py_redirector.def(py::init<>());
  py_redirector.def("write", &python_std_output_redirector_t::write);

}
