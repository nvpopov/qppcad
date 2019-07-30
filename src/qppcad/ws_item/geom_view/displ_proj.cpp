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

  py::class_<displ_proj_set_t, std::shared_ptr<displ_proj_set_t> >
  py_displ_proj_set_t(module, "displ_proj_set_t");
  py_displ_proj_set_t.def(py::init<std::shared_ptr<geom_view_t>, std::shared_ptr<geom_view_t>,
                          std::vector<size_t>>());
  py_displ_proj_set_t.def_readwrite("cnt", &displ_proj_set_t::m_cnt);
  py_displ_proj_set_t.def("__len__", [](displ_proj_set_t &sels) {
    return sels.m_recs.size();
  });
  py_displ_proj_set_t.def("__getitem__", [](displ_proj_set_t &sels, size_t i) {
    if (i >= sels.m_recs.size()) throw py::index_error();
    return sels.m_recs[i];
  }, py::return_value_policy::reference_internal);

}

displ_proj_set_t::displ_proj_set_t(std::shared_ptr<geom_view_t> gs,
                                   std::shared_ptr<geom_view_t> ge,
                                   std::vector<size_t> atlist) {

  if (atlist.empty()) return;
  if (gs->m_geom->nat() != ge->m_geom->nat()) return;

  for (auto &elem : atlist) {

      displ_proj_record_t rec(gs->m_geom->pos(elem),
                              ge->m_geom->pos(elem),
                              gs->m_geom->atom_name(elem));

      m_recs.push_back(std::move(rec));

    }

}

vector3<float> displ_proj_record_t::get_displ() {

  return m_end_pos - m_start_pos;

}
