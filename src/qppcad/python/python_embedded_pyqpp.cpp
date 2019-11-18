#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/core/app_state.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void pyqpp_linalg_export(py::module m);
void pyqpp_opaque_types_export(py::module m);
void pyqpp_cell_export(py::module m);
void pyqpp_geom_export(py::module m);
void pyqpp_xgeom_export(py::module m);
void pyqpp_builders_export(py::module m);
void pyqpp_autosymm_export(py::module m);
void pyqpp_ccd_export(py::module m);

qpp::xgeometry<float, qpp::periodic_cell<float>  >* claim_xgeom() {

  app_state_t *astate = app_state_t::get_inst();
  if (astate->ws_mgr->has_wss()) {
      auto cur_ws = astate->ws_mgr->get_cur_ws();
      if (cur_ws) {
          auto cur_it_al = dynamic_cast<geom_view_t*>(cur_ws->get_sel());
          if (cur_it_al) {
              return cur_it_al->m_geom.get();
            }
        }

    }

  return nullptr;
}

PYBIND11_EMBEDDED_MODULE(pyqpp, m) {

  pyqpp_linalg_export(m);

  pyqpp_opaque_types_export(m);

  pyqpp_cell_export(m);
  pyqpp_geom_export(m);
  pyqpp_xgeom_export(m);
  pyqpp_autosymm_export(m);
  pyqpp_builders_export(m);
  pyqpp_ccd_export(m);
  m.def("claim_xgeom", &claim_xgeom, py::return_value_policy::reference);

}
