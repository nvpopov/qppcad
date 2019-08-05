#include <qppcad/hotkey_manager.hpp>
#include <pybind11/functional.h>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")
namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(hk, m) {

  m.def("hk", [](){return app_state_t::get_inst()->hotkey_mgr;});

  py::class_<hotkey_entry_t, std::shared_ptr<hotkey_entry_t> >
      py_hotkey_entry_t(m, "hotkey_entry_t");
  py_hotkey_entry_t.def("get_key_seq", &hotkey_entry_t::get_key_sequence);

  py::class_<hotkey_manager_t, std::shared_ptr<hotkey_manager_t> >
      hotkey_manager_t(m, "hotkey_manager_t");
  hotkey_manager_t.def("reg_hotkey", &hotkey_manager_t::reg_hotkey);

}
