#include <qppcad/ws_item/geom_view/displ_proj.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_displ_proj_reg_helper_t::reg(py::module &module) {

  /*
   * python bindings for displ_proj_record_t
   */
  py::class_<displ_proj_record_t, std::shared_ptr<displ_proj_record_t> >
  py_displ_proj_record_t(module, "displ_proj_record_t");
  py_displ_proj_record_t.def(py::init<vector3<float>, vector3<float>, const std::string&>());
  py_displ_proj_record_t.def_readwrite("spos", &displ_proj_record_t::m_start_pos);
  py_displ_proj_record_t.def_readwrite("epos", &displ_proj_record_t::m_end_pos);
  py_displ_proj_record_t.def_readwrite("aname", &displ_proj_record_t::m_atom_name);
  py_displ_proj_record_t.def("displ", &displ_proj_record_t::get_displ);
  py_displ_proj_record_t.def("__str__",
                             [](displ_proj_record_t &src) {
                               return fmt::format("aname = \"{}\", "
                                                  "spos = [{:>8.5f}, {:>8.5f}, {:>8.5f}],"
                                                  " epos = [{:>8.5f}, {:>8.5f}, {:>8.5f}]",
                                                  src.m_atom_name,
                                                  src.m_start_pos[0],
                                                  src.m_start_pos[1],
                                                  src.m_start_pos[2],
                                                  src.m_end_pos[0],
                                                  src.m_end_pos[1],
                                                  src.m_end_pos[2]);
                             });

  /*
   * python bindings for displ_proj_mapping_t
   */
  py::class_<displ_proj_mapping_t, std::shared_ptr<displ_proj_mapping_t> >
  py_displ_proj_mapping_t(module, "displ_proj_mapping_t");
  py_displ_proj_mapping_t.def_readonly("mapped", &displ_proj_mapping_t::m_mapped);
  py_displ_proj_mapping_t.def_readwrite("displ_id", &displ_proj_mapping_t::m_displ_id);
  py_displ_proj_mapping_t.def_readwrite("mapped_atom_id", &displ_proj_mapping_t::m_mapped_atom_id);

  /*
   * python bindings for displ_proj_package_t
   */
  py::class_<displ_proj_package_t, std::shared_ptr<displ_proj_package_t> >
  py_displ_proj_package_t(module, "displ_proj_package_t");
  py_displ_proj_package_t.def(py::init<std::shared_ptr<geom_view_t>, std::shared_ptr<geom_view_t>,
                              std::vector<size_t>, std::optional<vector3<float> > >(),
                              py::arg("gs"), py::arg("ge"), py::arg("atlist"),
                              py::arg("exp_cnt") = std::nullopt);
  py_displ_proj_package_t.def_readwrite("cnt", &displ_proj_package_t::m_cnt);
  py_displ_proj_package_t.def("gen_mapping", &displ_proj_package_t::gen_mapping);
  py_displ_proj_package_t.def("apply", &displ_proj_package_t::apply);

  py_displ_proj_package_t.def("__len__", [](displ_proj_package_t &sels) {
    return sels.m_recs.size();
  });
  py_displ_proj_package_t.def("__getitem__", [](displ_proj_package_t &sels, size_t i) {
    if (i >= sels.m_recs.size()) throw py::index_error();
    return sels.m_recs[i];
  }, py::return_value_policy::reference_internal);

}

std::vector<displ_proj_mapping_t> displ_proj_package_t::gen_mapping(std::shared_ptr<geom_view_t> gv,
                                                                    vector3<float> apply_point,
                                                                    float eps_sr) {

  std::vector<displ_proj_mapping_t> retv;

  if (!gv) return retv;

  for (size_t i = 0; i < m_recs.size(); i++) {

      auto &rec = m_recs[i];
      auto ps_infr = apply_point + rec.m_start_pos;

      std::vector<tws_node_content_t<float> > qs_res;
      gv->m_tws_tr->query_sphere(eps_sr, ps_infr, qs_res);

      py::print(fmt::format("rec_id = {}, an = \"{}\", qs_res.sz = {}, spc = [{}, {}, {}]",
                            i, rec.m_atom_name, qs_res.size(),
                            ps_infr[0], ps_infr[1], ps_infr[2]));

      std::vector<size_t> candidates;
      for (auto &elem : qs_res)
        if (gv->m_geom->atom_name(elem.m_atm).find(rec.m_atom_name) != std::string::npos) {

            py::print(fmt::format("   id = {}, an_m = \"{}\", pc = [{}, {}, {}]",
                      elem.m_atm, gv->m_geom->atom_name(elem.m_atm),
                      gv->m_geom->pos(elem.m_atm)[0],
                      gv->m_geom->pos(elem.m_atm)[1],
                      gv->m_geom->pos(elem.m_atm)[2]));
            candidates.push_back(elem.m_atm);

          }

      displ_proj_mapping_t tmp_dm;
      tmp_dm.m_displ_id = i;

      if (candidates.empty()) {
          tmp_dm.m_mapped = false;
        } else {
          tmp_dm.m_mapped = true;
          tmp_dm.m_mapped_atom_id = candidates.front();
        }

      retv.push_back(std::move(tmp_dm));

    }

  return retv;

}

void displ_proj_package_t::apply(std::shared_ptr<geom_view_t> gv,
                                 std::vector<displ_proj_mapping_t> &map_vec) {

  if (!gv) return;

  //apply transformation
  //old
//  for (size_t i = 0; i < m_recs.size(); i++)
//    if (std::get<1>(lkp_vec[i])) {

//      auto atom_id = std::get<0>(lkp_vec[i]);
//      auto pos = gv->m_geom->pos(atom_id);
//      pos += m_recs[i].m_end_pos - m_recs[i].m_start_pos;
//      if (!check_run) gv->m_geom->change_pos(atom_id, pos);

//    }

  //new
  for (size_t i = 0; i < map_vec.size(); i++) {

      auto &dmp = map_vec[i];

      if (dmp.m_mapped && dmp.m_displ_id < m_recs.size()
          && dmp.m_mapped_atom_id < gv->m_geom->nat()) {

          auto pos = gv->m_geom->pos(dmp.m_mapped_atom_id);
          pos += m_recs[dmp.m_displ_id].m_end_pos - m_recs[dmp.m_displ_id].m_start_pos;
          gv->m_geom->change_pos(dmp.m_mapped_atom_id, pos);

        }

    }

}

displ_proj_package_t::displ_proj_package_t(std::shared_ptr<geom_view_t> gs,
                                   std::shared_ptr<geom_view_t> ge,
                                   std::vector<size_t> atlist,
                                   std::optional<vector3<float> > exp_cnt) {

  m_cnt = exp_cnt ? *exp_cnt : vector3<float>{0};

  if (atlist.empty()) return;
  if (gs->m_geom->nat() != ge->m_geom->nat()) return;

  for (auto &elem : atlist) {

      auto d_s = exp_cnt ? gs->m_geom->pos(elem) - *exp_cnt : gs->m_geom->pos(elem);
      auto d_e = exp_cnt ? ge->m_geom->pos(elem) - *exp_cnt : ge->m_geom->pos(elem);

      displ_proj_record_t rec(d_s, d_e, gs->m_geom->atom_name(elem));

      m_recs.push_back(std::move(rec));

    }

}

vector3<float> displ_proj_record_t::get_displ() {

  return m_end_pos - m_start_pos;

}
