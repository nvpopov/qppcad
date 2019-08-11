#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/python/python_console_output_redirectors.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/core/hotkey_manager.hpp>
#include <pybind11/functional.h>

using namespace qpp;
using namespace qpp::cad;

PYBIND11_EMBEDDED_MODULE(core, m) {

  m.def("clear", []() {
      app_state_t::get_inst()->astate_evd->python_console_clear_requested();
    });

  py::class_<python_std_output_redirector_t> py_redirector(m, "output_redirector");
  py_redirector.def(py::init<>());
  py_redirector.def("write", &python_std_output_redirector_t::write);

  /* hotkeys module */
  auto hk_module = m.def_submodule("hk", "hotkeys module");

  hk_module.def("mgr", [](){return app_state_t::get_inst()->hotkey_mgr;});

  py::class_<hotkey_entry_t, std::shared_ptr<hotkey_entry_t> >
      py_hotkey_entry_t(hk_module, "hotkey_entry_t");

  py_hotkey_entry_t.def("get_key_seq", &hotkey_entry_t::get_key_sequence)
                   .def("__str__", &hotkey_entry_t::py_print);

  py::class_<hotkey_manager_t, std::shared_ptr<hotkey_manager_t> >
      py_hotkey_manager_t(hk_module, "hotkey_manager_t");

  py_hotkey_manager_t.def("reg_hk", &hotkey_manager_t::reg_hotkey)
                     .def("unreg_hk", &hotkey_manager_t::unreg_hotkey_by_index)
                     .def("unreg_hk", &hotkey_manager_t::unreg_hotkey_by_key_sequence)
                     .def("__len__", [](hotkey_manager_t &hkm) {return hkm.m_hotkeys.size();})
                     .def("__getitem__", [](hotkey_manager_t &hkm, size_t i) {
                       if (i >= hkm.m_hotkeys.size()) throw py::index_error();
                       return hkm.m_hotkeys[i];
                     }, py::return_value_policy::reference_internal, py::keep_alive<0,2>());

  /* bhv helpers */
  auto bhv_module = m.def_submodule("bhv", "ws_item bhv manager");

  /* fixture info */
  py::class_<fixture_info_t> py_fixture_info_t(bhv_module, "fixture_info_t");
  py_fixture_info_t.def_readwrite("fixture_name", &fixture_info_t::m_fixture_name)
                   .def_readwrite("fixture_path", &fixture_info_t::m_fixure_path)
                   .def_readwrite("tool_group", &fixture_info_t::m_tool_group);

  /* bhv manager */
  bhv_module.def("mgr", [](){return app_state_t::get_inst()->ws_mgr->m_bhv_mgr;});
  py::class_<ws_item_behaviour_manager_t, std::shared_ptr<ws_item_behaviour_manager_t> >
      py_ws_item_bhv_mgr_t(bhv_module, "ws_item_behaviour_manager_t");
  py_ws_item_bhv_mgr_t.def_readonly("fxt_nfo", &ws_item_behaviour_manager_t::m_fixtures_info);

}
