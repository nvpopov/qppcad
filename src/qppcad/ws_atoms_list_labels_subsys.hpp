#ifndef QPP_CAD_WS_ATOMS_LIST_LABELS_S
#define QPP_CAD_WS_ATOMS_LIST_LABELS_S

#include <geom/geom.hpp>
#include <qppcad/app.hpp>
#include <vector>
#include <algorithm>

namespace qpp {

  namespace cad {

    enum ws_atoms_list_labels_style {
      show_none,
      show_id,
      show_type,
      show_id_type
    };

    template <typename DATA, typename REAL = float, typename AINT  = uint32_t>

    class ws_atoms_list_labels_subsys_t {

      private:

        DATA* p_owner;

      public:

        ws_atoms_list_labels_style m_style{ws_atoms_list_labels_style::show_none};

        ws_atoms_list_labels_subsys_t (DATA &_p_owner) {
          p_owner = &_p_owner;
        }

        void render_ui () {
          const char* items[] = { "None", "Show ID", "Show Type", "Show ID and Type"};
          ImGui::Combo("Labels style", reinterpret_cast<int*>(&m_style), items, 4);
        }

        void render_overlay () {

          if (m_style == show_none) return;

          app_state_t* astate = &(c_app::get_state());
          ImDrawList* imdrw = ImGui::GetOverlayDrawList();
          imdrw->PushClipRect(ImVec2(astate->viewport_xy_c[0], astate->viewport_xy_c[1]),
              ImVec2(astate->viewport_xy_c[0] + astate->viewport_size_c[0],
              astate->viewport_xy_c[1] + astate->viewport_size_c[1]));

          std::optional<vector2<REAL> > proj_pos;
          for (auto i = 0; i < p_owner->m_geom->nat(); i++) {
              proj_pos = astate->camera->project (p_owner->m_geom->pos(i));

              std::string label;/* = fmt::format("{}", i);*/

              switch (m_style) {

                case ws_atoms_list_labels_style::show_type : {
                    label = p_owner->m_geom->atom(i);
                    break;
                  }

                case ws_atoms_list_labels_style::show_id : {
                    label = fmt::format("{}", i);
                    break;
                  }

                case ws_atoms_list_labels_style::show_id_type : {
                    label = fmt::format("{}{}", p_owner->m_geom->atom(i), i);
                    break;
                  }

                default:
                  break;

                }

              //ImGui::SetWindowFontScale(2.0f);
              imdrw->AddText(
                  ImVec2((*proj_pos)[0]-label.length()*5, (*proj_pos)[1]-label.length()*5),
                  ImColor(0.0f, 0.0f, 0.0f, 1.0f),
                  label.c_str());
            }
        }


    };
  }

}

#endif
