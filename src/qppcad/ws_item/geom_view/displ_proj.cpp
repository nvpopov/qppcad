#include <qppcad/ws_item/geom_view/displ_proj.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_displ_proj_reg_helper_t::reg(py::module &module) {

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

  py::class_<displ_proj_package_t, std::shared_ptr<displ_proj_package_t> >
  py_displ_proj_package_t(module, "displ_proj_package_t");
  py_displ_proj_package_t.def(py::init<std::shared_ptr<geom_view_t>, std::shared_ptr<geom_view_t>,
                              std::vector<size_t>, std::optional<vector3<float> > >(),
                              py::arg("gs"), py::arg("ge"), py::arg("atlist"),
                              py::arg("exp_cnt") = std::nullopt);
  py_displ_proj_package_t.def_readwrite("cnt", &displ_proj_package_t::m_cnt);
  py_displ_proj_package_t.def("apply", &displ_proj_package_t::apply,
                              py::arg("gv"), py::arg("apply_point"),
                              py::arg("eps_sr") = 1.0f, py::arg("dry_run") = true);

  py_displ_proj_package_t.def("__len__", [](displ_proj_package_t &sels) {
    return sels.m_recs.size();
  });
  py_displ_proj_package_t.def("__getitem__", [](displ_proj_package_t &sels, size_t i) {
    if (i >= sels.m_recs.size()) throw py::index_error();
    return sels.m_recs[i];
  }, py::return_value_policy::reference_internal);

}

void displ_proj_package_t::apply(std::shared_ptr<geom_view_t> gv,
                             vector3<float> apply_point,
                             float eps_sr,
                             bool dry_run) {

  if (!gv) return;

  if (dry_run) py::print("Note: this is a dry run");

  //build lookup info
  std::vector<std::tuple<size_t, bool> > lkp_vec;
  lkp_vec.resize(m_recs.size());

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

      if (candidates.empty()) {
          lkp_vec[i] = {0, false};
        } else {
          lkp_vec[i] = {candidates.front(), true};
        }

    }

  //debug print lookup table
  py::print("Pattern matching stats:");
  for (auto &elem : lkp_vec)
    py::print(fmt::format("idx = {}, hit = {}", std::get<0>(elem), std::get<1>(elem)));

  //apply transformation
  for (size_t i = 0; i < m_recs.size(); i++)
    if (std::get<1>(lkp_vec[i])) {

      auto atom_id = std::get<0>(lkp_vec[i]);
      auto pos = gv->m_geom->pos(atom_id);
      pos += m_recs[i].m_end_pos - m_recs[i].m_start_pos;
      if (!dry_run) gv->m_geom->change_pos(atom_id, pos);

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
