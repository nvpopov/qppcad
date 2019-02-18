#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/workspace.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/ws_point_sym_group/ws_point_sym_group.hpp>

using namespace qpp;
using namespace qpp::cad;

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(workspace_stuff, m) {

  py::class_<workspace_manager_t,  std::shared_ptr<workspace_manager_t> >(m, "workspace_manager_t")
      .def("num_ws", [](const workspace_manager_t &wsm){ return wsm.m_ws.size();})
      .def("__getitem__", [](const workspace_manager_t &wsm, size_t i) {
                  if (i >= wsm.m_ws.size()) throw py::index_error();
                  return wsm.m_ws[i];
              })
      .def("__getitem__", [](workspace_manager_t &wsm, std::string ws_name) {
              auto retv = wsm.get_by_name(ws_name);
              if (!retv) throw py::key_error();
              return retv;
          })
      .def("has_wss", &workspace_manager_t::has_wss)
      .def("set_cur", &workspace_manager_t::set_cur_id);

  py::class_<workspace_t,  std::shared_ptr<workspace_t> >(m, "workspace_t")
      .def_readwrite("m_name", &workspace_t::m_ws_name);

}
