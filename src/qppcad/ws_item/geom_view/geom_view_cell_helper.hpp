#ifndef QPP_CAD_WS_ATOMS_LIST_T_CELL_HELPER
#define QPP_CAD_WS_ATOMS_LIST_T_CELL_HELPER
#include <qppcad/app.hpp>
#include <qppcad/geom_view.hpp>

namespace qpp {

  namespace cad {

    struct geom_view_cell_helper {

        static void render_ui_obj_insp (geom_view_t &al) {

          if (al.m_geom->DIM == 0) return;

          if (ImGui::CollapsingHeader("Cell")) {

              ImGui::Spacing();
              ImGui::Separator();

              ImGui::Spacing();
              ImGui::TextUnformatted("Lattice vectors:");
              ImGui::Spacing();

              if (al.m_geom->DIM >= 1)
                ImGui::TextUnformatted(fmt::format("a = {: 4.4f} {: 4.4f} {: 4.4f}",
                                                   al.m_geom->cell.v[0][0], al.m_geom->cell.v[0][1],
                    al.m_geom->cell.v[0][2]).c_str());

              if (al.m_geom->DIM >= 2)
                ImGui::TextUnformatted(fmt::format("b = {: 4.4f} {: 4.4f} {: 4.4f}",
                                                   al.m_geom->cell.v[1][0], al.m_geom->cell.v[1][1],
                    al.m_geom->cell.v[1][2]).c_str());

              if (al.m_geom->DIM >= 3)
                ImGui::TextUnformatted(fmt::format("c = {: 4.4f} {: 4.4f} {: 4.4f}",
                                                   al.m_geom->cell.v[2][0], al.m_geom->cell.v[2][1],
                    al.m_geom->cell.v[2][2]).c_str());

              ImGui::Separator();
              ImGui::Spacing();
            }

        }

    };

  }

}

#endif
