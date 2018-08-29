#ifndef QPP_IMGUI_ADDON_
#define QPP_IMGUI_ADDON_
#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <string>
#include <stdio.h>

namespace ImGui {
  struct TabsDesc {
      int	lableCount;
      float lableWidth;
      int currentidx;

  };

  struct Tabs {
      TabsDesc* tbd;
      ImGuiID ID;
      int *selectedIdx;
  };

  static   ImVector<Tabs*> CacheTabs;
  static   Tabs* CurTabs;

  inline bool ListBox_stl( const char* label, int* current_item,
                           const std::vector<std::string>& items,
                           int height_in_items = -1){
          // copy names to a vector
          std::vector<const char*> names;
          for( auto item : items ) {
                  char *cname = new char[ item.size() + 1 ];
                  strcpy( cname, item.c_str() );
                  names.push_back( cname );
          }

          bool result = ListBox( label, current_item, names.data(), names.size(), height_in_items );
          // cleanup
          for( auto &name : names ) delete [] name;
          return result;
  }


  inline void BeginTabs(const char* name, int lablesCount,
                        int &selIdx, float tabwidth = 0) {
    //Find exists Tabs
    Tabs* exsisttbs = nullptr;
    ImGuiID id = ImHash(name, 0);
    for (int i = 0; i < CacheTabs.Size; i++) {
        if (CacheTabs[i]->ID == id) {
            exsisttbs = CacheTabs[i];
          }
      }

    if (exsisttbs == nullptr) {
        Tabs* tbs = static_cast<Tabs*>(ImGui::MemAlloc(sizeof(Tabs)));
        tbs->selectedIdx = &selIdx;
        tbs->ID = id;
        CacheTabs.insert(CacheTabs.begin(), tbs);
        CurTabs = tbs;
      }
    else{
        CurTabs = exsisttbs;
      }

    TabsDesc* tbd =
        static_cast<TabsDesc*>(ImGui::MemAlloc(sizeof(TabsDesc)));
    tbd->lableCount = lablesCount;
    tbd->currentidx = 0;

    //ImVec2 windowSize = ImGui::GetWindowSize();
    tbd->lableWidth = tabwidth;
    CurTabs->tbd = tbd;
  }

  inline void EndTabs() {
    MemFree(CurTabs->tbd);
    CurTabs = nullptr;

  }

  inline bool AddTab(const char* label){
    TabsDesc* tbs = CurTabs->tbd;
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 itemSpacing = style.ItemSpacing;
    ImVec4 color = style.Colors[ImGuiCol_Button];
    ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
    ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
    style.ItemSpacing.x = 0;

    if (tbs->currentidx > 0)
      ImGui::SameLine();

    // push the style
    if (tbs->currentidx == *(CurTabs->selectedIdx)){
        style.Colors[ImGuiCol_Button] = colorActive;
        style.Colors[ImGuiCol_ButtonActive] = colorActive;
        style.Colors[ImGuiCol_ButtonHovered] = colorActive;
      }
    else {
        style.Colors[ImGuiCol_Button] = color;
        style.Colors[ImGuiCol_ButtonActive] = colorActive;
        style.Colors[ImGuiCol_ButtonHovered] = colorHover;
      }

    // Draw the button
    if (ImGui::Button(label, ImVec2(tbs->lableWidth, 0))) {
        *(CurTabs->selectedIdx) = tbs->currentidx;
      }

    //                if (ImGui::IsItemHovered()){
    //                        ImGui::BeginTooltip();
    //                        ImGui::Text("%s", tooltip);
    //                        ImGui::EndTooltip();
    //                }

    // Restore the style
    style.Colors[ImGuiCol_Button] = color;
    style.Colors[ImGuiCol_ButtonActive] = colorActive;
    style.Colors[ImGuiCol_ButtonHovered] = colorHover;
    style.ItemSpacing = itemSpacing;

    tbs->currentidx++;
    return *(CurTabs->selectedIdx) == tbs->currentidx;
  }
}

#endif
