#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <symm/index.hpp>
#include <geom/lace3d.hpp>

#include <qppcad/ws_item/ws_item_trampoline.hpp>
#include <qppcad/core/workspace.hpp>

#include <qppcad/ws_item/geom_view/python_geom_view.hpp>
#include <qppcad/ws_item/ccd_view/python_ccd_view.hpp>
#include <qppcad/ws_item/psg_view/python_psg_view.hpp>
#include <qppcad/ws_item/pdos_view/python_pdos_view.hpp>
#include <qppcad/ws_item/node_book/python_node_book.hpp>
#include <qppcad/ws_item/volume_view/python_volume_view.hpp>

#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

namespace py = pybind11;

std::shared_ptr<ws_item_t> construct_from_geom(
    workspace_t &ws,
    std::shared_ptr<xgeometry<float, periodic_cell<float> > > geom,
    const std::string &name) {

    auto new_item = ws.m_owner->m_bhv_mgr->fbr_ws_item_by_type(geom_view_t::get_type_static());
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
    const std::string &name) {

    auto new_item = ws.m_owner->m_bhv_mgr->fbr_ws_item_by_type(psg_view_t::get_type_static());
    if (!new_item) return nullptr;

    auto as_psg = new_item->cast_as<psg_view_t>();
    if(!as_psg) return nullptr;

    as_psg->m_ag = ag;
    as_psg->m_name = name;
    ws.add_item_to_ws(new_item);
    as_psg->update_view();

    return new_item;

}

//void upd_oi(ws_item_t *_item) {

//  if (_item && _item->m_selected)
//    app_state_t::get_inst()->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();

//}

void mvd() {

  app_state_t *astate = app_state_t::get_inst();
  astate->make_viewport_dirty();

}


void cws_changed() {

  app_state_t *astate = app_state_t::get_inst();
  astate->astate_evd->cur_ws_changed();

}

PYBIND11_EMBEDDED_MODULE(cad, m) {

  /* cam_target_view pybindings */
  py::enum_<cam_tv_e>(m, "cam_tv_e", py::arithmetic(), "")
          .value("tv_x", tv_x, "tv_x_axis")
          .value("tv_y", tv_y, "tv_y_axis")
          .value("tv_z", tv_z, "tv_z_axis")
          .value("tv_cc", tv_cc, "tv_cart_center")
          .value("tv_a", tv_a, "tv_a_axis")
          .value("tv_b", tv_b, "tv_b_axis")
          .value("tv_c", tv_c, "tv_c_axis")
          .value("tv_as", tv_as, "tv_a_star_axis")
          .value("tv_bs", tv_bs, "tv_b_star_axis")
          .value("tv_cs", tv_cs, "tv_c_star_axis")
          .value("tv_auto", tv_auto, "tv_auto")
          .export_values();

  /* workspace_manager_t pybindings */
  py::class_<workspace_manager_t,  std::shared_ptr<workspace_manager_t> >
      py_workspace_manager_t(m, "workspace_manager_t");
      py_workspace_manager_t.def("__len__",
                                 [](const workspace_manager_t &wsm){ return wsm.m_ws.size();})
                            .def("__getitem__", [](const workspace_manager_t &wsm, size_t i) {
                              if (i >= wsm.m_ws.size()) throw py::index_error();
                              return wsm.m_ws[i];
                            }, py::return_value_policy::reference_internal)
                            .def("__getitem__", [](workspace_manager_t &wsm, std::string ws_name) {
                              auto retv = wsm.get_by_name(ws_name);
                              if (!retv) throw py::key_error();
                              return retv;
                             }, py::return_value_policy::reference_internal)
                            .def("get_sel_itm", [](workspace_manager_t &wsm) {
                              auto [cur_ws, cur_it, ok] = wsm.get_sel_tpl_itm_nc();
                              return cur_it;
                             })
                            .def("next_ws", &workspace_manager_t::next_ws)
                            .def("prev_ws", &workspace_manager_t::prev_ws)
                            .def("next_item", &workspace_manager_t::cur_ws_next_item)
                            .def("prev_item", &workspace_manager_t::cur_ws_prev_item)
                            .def("has_wss", &workspace_manager_t::has_wss)
                            .def("set_cur", &workspace_manager_t::set_cur_id);

  /* workspace_t pybindings */
  py::class_<workspace_t,  std::shared_ptr<workspace_t> > py_workspace_t(m, "workspace_t");
      py_workspace_t.def_readwrite("name", &workspace_t::m_ws_name)
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
                    .def("next_item", &workspace_t::next_item)
                    .def("prev_item", &workspace_t::prev_item)
                    .def("construct", &workspace_t::py_construct_item)
                    .def("construct", &construct_from_geom)
                    .def("construct", &construct_from_array_group)
                    .def_readwrite("scenic_rot_magn", &workspace_t::m_scenic_rotation_speed)
                    .def_property("scenic_rot",
                                  [](workspace_t &src)
                                  {return src.m_scenic_rotation;},
                                  [](workspace_t &src, const bool value)
                                  {src.m_scenic_rotation = value; cws_changed();})
                    .def_property("bg",
                                  [](workspace_t &src)
                                  {return src.m_background_color;},
                                  [](workspace_t &src, const vector3<float> value)
                                  {src.m_background_color = value; mvd();})
                    .def("delete_item", &workspace_t::delete_item_by_index)
                    .def("gp", [](workspace_t &src){return src.m_gizmo->m_pos;})
                    .def("__repr__", &workspace_t::py_get_repr)
                    .def("__str__", &workspace_t::py_get_repr);

  /* aabb_3d_t pybindings */
  py::class_<aabb_3d_t<float>, std::shared_ptr<aabb_3d_t<float>>> py_aabb_3d_t(m, "aabb_3d_t");
  py_aabb_3d_t.def_readwrite("min", &aabb_3d_t<float>::min)
              .def_readwrite("max", &aabb_3d_t<float>::max)
              .def("cnt", &aabb_3d_t<float>::center)
              .def("tst_sph", &aabb_3d_t<float>::test_sphere)
              .def("vol", &aabb_3d_t<float>::volume);

  /* ws_item_t pybindings */
  py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> py_ws_item_t(m, "ws_item_t");
  py_ws_item_t.def_readwrite("name", &ws_item_t::m_name)
              .def("get_cnt_count", &ws_item_t::get_content_count)
              .def("get_parent_ws", [](ws_item_t &wsi){return wsi.m_parent_ws;})
             // .def_readwrite("m_pos", &ws_item_t::get_pos, &ws_item_t::set_pos)
              .def_property("is_visible",
                            [](ws_item_t &src)
                            {return src.m_is_visible;},
                            [](ws_item_t &src, const bool value)
                            {src.m_is_visible = value; src.update_oi();})
              .def_property("show_bb",
                            [](ws_item_t &src)
                            {return src.m_show_bb;},
                            [](ws_item_t &src, const bool value)
                            {src.m_show_bb = value; src.update_oi();})
              .def_property("pos",
                            [](ws_item_t &src)
                            {return src.m_pos;},
                            [](ws_item_t &src, const vector3<float> value)
                            {src.m_pos = value; src.update_oi();})
              .def("__repr__", &ws_item_t::py_get_repr)
              .def("__str__", &ws_item_t::py_get_repr)
              .def("apply_tv", &ws_item_t::apply_target_view)
              .def_readwrite("offset", &ws_item_t::m_leader_offset)
              .def("bb", [](ws_item_t &src){return src.m_aabb;});

  /* per ws_item_t types pybindings */
  py_geom_view_reg_helper_t::reg(m, py_ws_item_t);
  py_ccd_view_reg_helper_t::reg(m, py_ws_item_t);
  py_psg_view_reg_helper_t::reg(m, py_ws_item_t);
  py_pdos_view_reg_helper_t::reg(m, py_ws_item_t);
  py_node_book_reg_helper_t::reg(m, py_ws_item_t);
  py_volume_view_reg_helper_t::reg(m, py_ws_item_t);

}
