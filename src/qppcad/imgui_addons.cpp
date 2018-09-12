#include <qppcad/imgui_addons.hpp>

bool ImGui::ToggleButton(const char* label, bool* v, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if(window->SkipItems)
        return false;

    int flags = 0;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = ImGui::GetStyle();
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg,
                               label_size.x + style.FramePadding.x * 2.0f,
                               label_size.y + style.FramePadding.y * 2.0f);

    ImVec2 pos_p_size(pos.x + size.x, pos.y + size.y);
    const ImRect bb(pos, pos_p_size);
    ItemSize(bb, style.FramePadding.y);
    if(!ItemAdd(bb, id))
        return false;

    //if(window->DC.ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const ImU32 col = GetColorU32(((held && hovered) || *v) ? ImGuiCol_ButtonActive : (hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button));

    RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
    ImVec2 bb_min_s_fp(bb.Min.x + style.FramePadding.x, bb.Min.y + style.FramePadding.y);
    ImVec2 bb_max_s_fp(bb.Max.x - style.FramePadding.y, bb.Max.y - style.FramePadding.y);
    ImGui::RenderTextClipped(bb_min_s_fp, bb_max_s_fp, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    if(pressed)
        *v = !*v;

    return pressed;
}
