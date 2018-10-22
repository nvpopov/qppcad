#ifndef QPPCAD_WS_ATOMS_LIST_OBJ_INSP
#define QPPCAD_WS_ATOMS_LIST_OBJ_INSP
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/imgui_addons.hpp>
#include <qppcad/app.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"

namespace qpp {

  namespace cad {

    struct ws_atoms_list_obj_insp_helper {
        static void render_ui (ws_atoms_list_t &al);
    };

  }

}
#pragma clang diagnostic pop
#endif
