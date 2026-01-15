// src/EFIGUI/Components/Selectable.cpp
#include "Selectable.h"
#include "../Core/Animation.h"
#include "../Core/Style.h"
#include "imgui_internal.h"

namespace EFIGUI {

bool SelectableEx(const char* label, bool selected,
                  ImGuiSelectableFlags flags, ImVec2 size,
                  const SelectableStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 actual_size = ImGui::CalcItemSize(size,
        ImGui::CalcItemWidth(), label_size.y + style.padding * 2);

    const ImRect bb(pos, ImVec2(pos.x + actual_size.x, pos.y + actual_size.y));
    ImGui::ItemSize(actual_size, 0.0f);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

    // Get animation state
    Animation::WidgetState& state = Animation::GetState(id);
    Animation::UpdateWidgetState(state, hovered, held, selected);

    ImDrawList* draw = window->DrawList;

    // Draw background
    ImU32 bgColor = selected ? style.bgSelectedColor :
                    hovered ? style.bgHoverColor : style.bgColor;
    if ((bgColor & IM_COL32_A_MASK) != 0)
        draw->AddRectFilled(bb.Min, bb.Max, bgColor, style.rounding);

    // Draw accent bar when selected
    if (selected) {
        ImVec2 barMin(bb.Min.x, bb.Min.y + style.padding);
        ImVec2 barMax(bb.Min.x + style.accentBarWidth, bb.Max.y - style.padding);
        draw->AddRectFilled(barMin, barMax, style.accentColor, style.accentBarWidth * 0.5f);
    }

    // Draw text
    ImU32 textColor = selected ? style.textSelectedColor : style.textColor;
    ImVec2 text_pos(bb.Min.x + style.padding + (selected ? style.accentBarWidth + 4.0f : 0.0f),
                    bb.Min.y + (actual_size.y - label_size.y) * 0.5f);
    draw->AddText(text_pos, textColor, label);

    return pressed;
}

bool Selectable(const char* label, bool selected,
                ImGuiSelectableFlags flags, ImVec2 size) {
    return SelectableEx(label, selected, flags, size, StyleSystem::GetCurrentStyle<SelectableStyle>());
}

bool SelectableEx(const char* label, bool* p_selected,
                  ImGuiSelectableFlags flags, ImVec2 size,
                  const SelectableStyle& style) {
    if (SelectableEx(label, *p_selected, flags, size, style)) {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

bool Selectable(const char* label, bool* p_selected,
                ImGuiSelectableFlags flags, ImVec2 size) {
    return SelectableEx(label, p_selected, flags, size, StyleSystem::GetCurrentStyle<SelectableStyle>());
}

} // namespace EFIGUI
