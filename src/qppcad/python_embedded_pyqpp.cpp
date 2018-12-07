#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/app_state.hpp>
#include <qppcad/ws_atoms_list.hpp>

using namespace qpp;
using namespace qpp::cad;

void pyqpp_linalg_export (py::module m);
void pyqpp_cell_export   (py::module m);
void pyqpp_geom_export   (py::module m);
void pyqpp_xgeom_export  (py::module m);

qpp::xgeometry<float, qpp::periodic_cell<float>  >* claim_xgeom() {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());
          if (cur_it_al) {
              return cur_it_al->m_geom.get();
            }
        }

    }

  return nullptr;
}

PYBIND11_EMBEDDED_MODULE(pyqpp, m) {
  pyqpp_linalg_export(m);
  pyqpp_cell_export(m);
  pyqpp_geom_export(m);
  pyqpp_xgeom_export(m);

  m.def("claim_xgeom", &claim_xgeom);
}
