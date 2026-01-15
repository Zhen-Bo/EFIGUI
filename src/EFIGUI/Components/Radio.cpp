// src/EFIGUI/Components/Radio.cpp
#include "Radio.h"
#include "../Core/Animation.h"
#include "../Core/Draw.h"
#include "imgui_internal.h"
#include <algorithm>

namespace EFIGUI {

bool RadioButtonEx(const char* label, bool active, const RadioStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);

    const float diameter = style.radius * 2.0f;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + diameter + (label_size.x > 0.0f ? style.labelGap + label_size.x : 0.0f), pos.y + std::max(diameter, label_size.y)));

    ImGui::ItemSize(total_bb, 0.0f);
    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);

    // Get animation state
    Animation::WidgetState& state = Animation::GetState(id);
    Animation::UpdateWidgetState(state, hovered, held, active);

    ImDrawList* draw = window->DrawList;
    const ImVec2 center(pos.x + style.radius, pos.y + style.radius);

    // Draw circle background
    ImU32 circleColor = held ? style.circleActiveColor :
                        hovered ? style.circleHoverColor : style.circleColor;
    draw->AddCircleFilled(center, style.radius, circleColor);

    // Draw border
    ImU32 borderColor = (hovered || active) ? style.borderHoverColor : style.borderColor;
    draw->AddCircle(center, style.radius, borderColor, 0, style.borderWidth);

    // Draw glow when active
    if (active) {
        Draw::GlowLayersCircle(center, style.radius, style.glowColor,
                               style.glowIntensity, style.glowLayers, 2.0f);
    }

    // Draw dot when active
    if (active) {
        draw->AddCircleFilled(center, style.dotRadius, style.dotColor);
    }

    // Draw label
    if (label_size.x > 0.0f) {
        ImVec2 label_pos(pos.x + diameter + style.labelGap, pos.y + (diameter - label_size.y) * 0.5f);
        draw->AddText(label_pos, CyberpunkTheme::Colors::TextPrimary, label);
    }

    return pressed;
}

bool RadioButton(const char* label, bool active) {
    return RadioButtonEx(label, active, RadioStyle{});
}

bool RadioButtonEx(const char* label, int* v, int v_button, const RadioStyle& style) {
    const bool pressed = RadioButtonEx(label, *v == v_button, style);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool RadioButton(const char* label, int* v, int v_button) {
    return RadioButtonEx(label, v, v_button, RadioStyle{});
}

} // namespace EFIGUI
