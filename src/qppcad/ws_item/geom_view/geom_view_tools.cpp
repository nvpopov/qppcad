#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/app_state.hpp>
#include <random>

using namespace qpp;
using namespace qpp::cad;

void geom_view_tools_t::shake_atoms(geom_view_t *gv, std::set<size_t> atoms_to_shake, float magn) {

  if (!gv) return;

  app_state_t* astate = app_state_t::get_inst();

  static std::default_random_engine e;
  static std::uniform_real_distribution<> dis(0, magn);

  for (auto &atom : atoms_to_shake)
    if (atom < gv->m_geom->nat()) {

        vector3<float> new_pos = gv->m_geom->coord(atom) +
                                 vector3<float>(magn / 2) -
                                 vector3<float>(dis(e), dis(e), dis(e));

        gv->m_geom->coord(atom) = new_pos;

      }

  astate->make_viewport_dirty();

}

void geom_view_tools_t::py_shake_atoms(geom_view_t *gv, py::list atoms_to_shake, float magn) {

  if (!gv) return;

  std::set<size_t> atoms;
  for (auto elem : atoms_to_shake)
    if (py::isinstance<py::int_>(elem)) atoms.insert(py::cast<int>(elem));

  geom_view_tools_t::shake_atoms(gv, atoms, magn);

}
