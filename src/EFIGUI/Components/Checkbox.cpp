// src/EFIGUI/Components/Checkbox.cpp
#include "Checkbox.h"
#include "../Core/Animation.h"
#include "../Core/Draw.h"
#include "../Core/Layer.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <algorithm>

namespace EFIGUI {

bool CheckboxEx(const char* label, bool* v, const CheckboxStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);

    const float square_sz = style.size;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 0.0f ? style.checkPadding + label_size.x : 0.0f), pos.y + std::max(square_sz, label_size.y)));

    ImGui::ItemSize(total_bb, 0.0f);
    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    // Get animation state
    Animation::WidgetState& state = Animation::GetState(id);
    Animation::UpdateWidgetState(state, hovered, held, *v);

    ImDrawList* draw = window->DrawList;
    const ImRect check_bb(pos, ImVec2(pos.x + square_sz, pos.y + square_sz));

    // Draw box background
    ImU32 boxColor = held ? style.boxActiveColor :
                     hovered ? style.boxHoverColor : style.boxColor;
    draw->AddRectFilled(check_bb.Min, check_bb.Max, boxColor, style.rounding);

    // Draw border
    ImU32 borderColor = (hovered || *v) ? style.borderHoverColor : style.borderColor;
    draw->AddRect(check_bb.Min, check_bb.Max, borderColor, style.rounding, 0, style.borderWidth);

    // Draw glow (animate smoothly on both check and uncheck)
    if (state.selectedAnim > 0.01f) {
        Draw::RectGlow(check_bb.Min, check_bb.Max, style.glowColor,
                       style.glowIntensity * state.selectedAnim, 4.0f);
    }

    // Draw checkmark
    if (*v) {
        const float pad = style.checkPadding;
        const ImVec2 p1(check_bb.Min.x + pad, check_bb.Min.y + square_sz * 0.5f);
        const ImVec2 p2(check_bb.Min.x + square_sz * 0.4f, check_bb.Max.y - pad);
        const ImVec2 p3(check_bb.Max.x - pad, check_bb.Min.y + pad);

        draw->AddLine(p1, p2, style.checkColor, 2.0f);
        draw->AddLine(p2, p3, style.checkColor, 2.0f);
    }

    // Draw label
    if (label_size.x > 0.0f) {
        ImVec2 label_pos(check_bb.Max.x + style.checkPadding, pos.y + (square_sz - label_size.y) * 0.5f);
        draw->AddText(label_pos, style.textColor, label);
    }

    return pressed;
}

bool Checkbox(const char* label, bool* v) {
    return CheckboxEx(label, v, StyleSystem::GetCurrentStyle<CheckboxStyle>());
}

} // namespace EFIGUI
