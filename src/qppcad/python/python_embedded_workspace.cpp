#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/ws_item/ws_item_trampoline.hpp>
#include <qppcad/workspace.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>

using namespace qpp;
using namespace qpp::cad;

namespace py = pybind11;

std::shared_ptr<ws_item_t> construct_from_geom(
    workspace_t &ws,
    std::shared_ptr<xgeometry<float, periodic_cell<float> > > geom,
    std::string name) {

    auto new_item = ws.m_owner->m_bhv_mgr->fabric_by_type(geom_view_t::get_type_static());
    if (!new_item) return nullptr;

    auto as_gv = new_item->cast_as<geom_view_t>();
    if(!as_gv) return nullptr;

    if (as_gv->m_geom) {
        as_gv->m_geom->remove_observer(*as_gv->m_ext_obs);
        as_gv->m_geom->remove_observer(*as_gv->m_tws_tr);
      }

    as_gv->m_geom = geom;
    as_gv->m_ext_obs->geom = as_gv->m_geom.get();
    as_gv->m_tws_tr->geom = as_gv->m_geom.get();

    as_gv->m_tws_tr->do_action(act_lock | act_clear_all);
    as_gv->m_geom->add_observer(*as_gv->m_ext_obs);
    as_gv->m_geom->add_observer(*as_gv->m_tws_tr);
    as_gv->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
    as_gv->m_tws_tr->do_action(act_rebuild_ntable);
    as_gv->m_name = name;
    ws.add_item_to_ws(new_item);

    return new_item;

}

std::shared_ptr<ws_item_t> construct_from_array_group(
    workspace_t &ws,
    std::shared_ptr<array_group<matrix3<float> > > ag,
    std::string name) {

    auto new_item = ws.m_owner->m_bhv_mgr->fabric_by_type(psg_view_t::get_type_static());
    if (!new_item) return nullptr;

    auto as_psg = new_item->cast_as<psg_view_t>();
    if(!as_psg) return nullptr;

    as_psg->m_ag = ag;
    as_psg->m_name = name;
    ws.add_item_to_ws(new_item);
    as_psg->update_view();

    return new_item;

}

PYBIND11_EMBEDDED_MODULE(workspace_stuff, m) {

  py::class_<workspace_manager_t,  std::shared_ptr<workspace_manager_t> >(m, "workspace_manager_t")
      .def("__len__", [](const workspace_manager_t &wsm){ return wsm.m_ws.size();})
      .def("__getitem__", [](const workspace_manager_t &wsm, size_t i) {
        if (i >= wsm.m_ws.size()) throw py::index_error();
        return wsm.m_ws[i];
      }, py::return_value_policy::reference_internal)
      .def("__getitem__", [](workspace_manager_t &wsm, std::string ws_name) {
        auto retv = wsm.get_by_name(ws_name);
        if (!retv) throw py::key_error();
        return retv;
       }, py::return_value_policy::reference_internal)
      .def("has_wss", &workspace_manager_t::has_wss)
      .def("set_cur", &workspace_manager_t::set_cur_id);

  py::class_<workspace_t,  std::shared_ptr<workspace_t> >(m, "workspace_t")
      .def_readwrite("name", &workspace_t::m_ws_name)
      .def("__len__", [](workspace_t &ws) {return ws.m_ws_items.size();})
      .def("__getitem__", [](workspace_t &ws, size_t i) {
        if (i >= ws.m_ws_items.size()) throw py::index_error();
        return ws.m_ws_items[i];
      }, py::return_value_policy::reference_internal, py::keep_alive<0,2>())
      .def("__getitem__", [](workspace_t &ws, std::string item_name) {
        auto retv = ws.get_by_name(item_name);
        if (!retv) throw py::key_error();
        return retv;
       }, py::return_value_policy::reference_internal, py::keep_alive<0,2>())
      .def("construct", &workspace_t::py_construct_item)
      .def("construct", &construct_from_geom)
      .def("construct", &construct_from_array_group)
      .def("__repr__", &workspace_t::py_get_repr)
      .def("__str__", &workspace_t::py_get_repr);

  py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> py_ws_item_t(m, "ws_item_t");
  py_ws_item_t
          .def_readwrite("name", &ws_item_t::m_name)
          .def("get_cnt_count", &ws_item_t::get_content_count)
          .def("get_parent_ws", [](ws_item_t &wsi){return wsi.m_parent_ws;})
         // .def_readwrite("m_pos", &ws_item_t::get_pos, &ws_item_t::set_pos)
          .def("__repr__", &ws_item_t::py_get_repr)
          .def("__str__", &ws_item_t::py_get_repr);

  py::class_<geom_view_t, std::shared_ptr<geom_view_t> >
  py_atoms_list_t(m, "geom_view_t", py_ws_item_t);
  py_atoms_list_t.def_readwrite("geom", &geom_view_t::m_geom);

  py::class_<psg_view_t, std::shared_ptr<psg_view_t> >
  py_point_sym_group_t(m, "psg_view_t", py_ws_item_t);
  py_point_sym_group_t.def_readwrite("ag", &psg_view_t::m_ag, py::return_value_policy::reference);
  py_point_sym_group_t.def_readwrite("pg_axes", &psg_view_t::m_pg_axes);
  py_point_sym_group_t.def("update_view", &psg_view_t::update_view);
  py_point_sym_group_t.def_readwrite("center", &psg_view_t::m_new_centre);

}
