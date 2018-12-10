#ifndef QPP_CAD_RENDER_BS
#define QPP_CAD_RENDER_BS
#include <qppcad/ws_atoms_list.hpp>

namespace qpp {

  namespace cad {

    struct ws_atoms_list_render_bs {

        static void render(ws_atoms_list_t &al);

        static void render_atom(ws_atoms_list_t &al, const uint32_t at_num,
                                const index &at_index);

        static void render_bond(ws_atoms_list_t &al,
                                const uint32_t at_num1, const index &at_index1,
                                const uint32_t at_num2, const index &at_index2);
    };

  }

}

#endif
