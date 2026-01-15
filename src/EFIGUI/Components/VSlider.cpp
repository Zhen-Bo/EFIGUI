// src/EFIGUI/Components/VSlider.cpp
#include "VSlider.h"
#include "../Core/Animation.h"
#include "../Core/Draw.h"
#include "../Core/Layer.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <algorithm>
#include <cstdio>

namespace EFIGUI {

bool VSliderFloatEx(const char* label, const ImVec2& size, float* v,
                    float v_min, float v_max, const char* format,
                    ImGuiSliderFlags flags, const VSliderStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    const ImVec2 pos = window->DC.CursorPos;

    // Calculate actual size
    ImVec2 actualSize = size;
    if (actualSize.x <= 0.0f)
        actualSize.x = style.trackWidth;
    if (actualSize.y <= 0.0f)
        actualSize.y = 100.0f;

    const ImRect bb(pos, ImVec2(pos.x + actualSize.x, pos.y + actualSize.y));

    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    // Get animation state
    Animation::WidgetState& animState = Animation::GetState(id);

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    (void)pressed;  // Suppress unused warning

    Animation::UpdateWidgetState(animState, hovered, held, false);

    // Clamp value
    *v = std::clamp(*v, v_min, v_max);

    bool changed = false;

    // Handle drag interaction
    if (held) {
        float mouseY = ImGui::GetMousePos().y;
        float sliderTop = bb.Min.y + style.knobRadius;
        float sliderBottom = bb.Max.y - style.knobRadius;
        float clampedMouseY = std::clamp(mouseY, sliderTop, sliderBottom);

        // Invert Y axis (bottom = min, top = max)
        float range = sliderBottom - sliderTop;

        // Protect against division by zero when size.y is too small
        if (range < 1.0f) {
            range = 1.0f;
        }

        float normalizedValue = 1.0f - (clampedMouseY - sliderTop) / range;
        float newValue = v_min + normalizedValue * (v_max - v_min);

        if (newValue != *v) {
            *v = newValue;
            changed = true;
            ImGui::MarkItemEdited(id);
        }
    }

    ImDrawList* draw = window->DrawList;

    // Calculate track area (centered in bounding box)
    float trackX = pos.x + (actualSize.x - style.trackWidth) * 0.5f;
    ImVec2 trackMin(trackX, pos.y);
    ImVec2 trackMax(trackX + style.trackWidth, pos.y + actualSize.y);

    // Draw track background
    draw->AddRectFilled(trackMin, trackMax, style.trackColor, style.trackRounding);

    // Draw track border
    ImU32 borderColor = (hovered || held) ? style.trackBorderHoverColor : style.trackBorderColor;
    draw->AddRect(trackMin, trackMax, borderColor, style.trackRounding, 0, style.borderWidth);

    // Calculate fill height
    float valueRange = v_max - v_min;
    float normalizedCurrent = (valueRange != 0.0f)
        ? std::clamp((*v - v_min) / valueRange, 0.0f, 1.0f)
        : 0.0f;
    float fillHeight = (actualSize.y - style.knobRadius * 2) * normalizedCurrent;
    float fillTop = trackMax.y - style.knobRadius - fillHeight;

    // Draw filled portion with gradient
    if (fillHeight > 0) {
        ImVec2 fillMin(trackMin.x, fillTop);
        ImVec2 fillMax(trackMax.x, trackMax.y - style.knobRadius);

        // Gradient from bottom to top
        Draw::RectGradientV(fillMin, fillMax,
                            style.fillColorTop,
                            style.fillColorBottom,
                            style.trackRounding);

        // Glow effect on hover/active
        if (animState.hoverAnim > 0.1f || held) {
            float glowIntensity = held ? 0.8f : animState.hoverAnim * 0.5f;
            Draw::RectGlow(fillMin, fillMax, style.fillGlowColor,
                          glowIntensity, 4.0f);
        }
    }

    // Draw knob
    float knobY = trackMax.y - style.knobRadius - fillHeight;
    float knobX = trackX + style.trackWidth * 0.5f;
    ImVec2 knobCenter(knobX, knobY);

    // Knob glow effect
    if (animState.hoverAnim > 0.1f || held) {
        float glowIntensity = held ? 0.8f : animState.hoverAnim * 0.5f;
        Draw::GlowLayersCircle(knobCenter, style.knobRadius + 2.0f, style.glowColor,
                               glowIntensity, style.glowLayers, style.knobGlowExpand);
    }

    ImU32 knobColor = held ? style.knobActiveColor : style.knobColor;
    draw->AddCircleFilled(knobCenter, style.knobRadius, knobColor);
    draw->AddCircle(knobCenter, style.knobRadius, style.knobBorderColor, 0, style.borderWidth);

    // Draw value tooltip on hover
    if (hovered || held) {
        char buf[64];
        snprintf(buf, sizeof(buf), format, *v);
        ImGui::SetTooltip("%s", buf);
    }

    // Draw label below slider
    bool labelHidden = (label[0] == '#' && label[1] == '#');
    if (!labelHidden) {
        ImVec2 textSize = ImGui::CalcTextSize(label);
        float textX = pos.x + (actualSize.x - textSize.x) * 0.5f;
        float textY = pos.y + actualSize.y + ImGui::GetStyle().ItemSpacing.y;
        draw->AddText(ImVec2(textX, textY), style.labelColor, label);
    }

    return changed;
}

bool VSliderFloat(const char* label, const ImVec2& size, float* v,
                  float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
    return VSliderFloatEx(label, size, v, v_min, v_max, format, flags,
                          StyleSystem::GetCurrentStyle<VSliderStyle>());
}

bool VSliderIntEx(const char* label, const ImVec2& size, int* v,
                  int v_min, int v_max, const char* format,
                  ImGuiSliderFlags flags, const VSliderStyle& style) {
    float floatValue = static_cast<float>(*v);
    bool changed = VSliderFloatEx(label, size, &floatValue,
                                   static_cast<float>(v_min),
                                   static_cast<float>(v_max),
                                   format, flags, style);

    int newValue = static_cast<int>(floatValue + 0.5f);
    newValue = std::clamp(newValue, v_min, v_max);

    if (newValue != *v) {
        *v = newValue;
        return true;
    }
    return changed;
}

bool VSliderInt(const char* label, const ImVec2& size, int* v,
                int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
    return VSliderIntEx(label, size, v, v_min, v_max, format, flags,
                        StyleSystem::GetCurrentStyle<VSliderStyle>());
}

} // namespace EFIGUI
