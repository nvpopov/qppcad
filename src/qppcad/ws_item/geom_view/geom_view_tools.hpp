#ifndef QPPCAD_GEOM_VIEW_TOOLS
#define QPPCAD_GEOM_VIEW_TOOLS

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

  namespace cad {

    struct geom_view_tools_t {

        static void shake_atoms(geom_view_t *gv, std::set<size_t> atoms_to_shake, float magn);
        static void py_shake_atoms(geom_view_t *gv, py::list atoms_to_shake, float magn);

    };

  }

}

#endif
