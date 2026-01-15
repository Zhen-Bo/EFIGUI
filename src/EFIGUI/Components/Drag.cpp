// src/EFIGUI/Components/Drag.cpp
#include "Drag.h"
#include "../Core/Animation.h"
#include "../Core/Draw.h"
#include "../Core/Layer.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <algorithm>
#include <unordered_map>

namespace EFIGUI {

// =============================================
// Internal: Value change tracking for glow feedback
// =============================================

struct DragValueState {
    float lastValue = 0.0f;
    float changeTime = 0.0f;  // Time since last value change
    bool wasActive = false;
    float lastTimeUsed = 0.0f;  // Track last usage time
};

// Type alias for per-context drag state storage
using DragValueStateMap = std::unordered_map<ImGuiID, DragValueState>;

static DragValueStateMap& GetDragValueStates() {
    // Use StyleSystem's per-context storage instead of global static
    // This ensures proper isolation in multi-context scenarios
    return StyleSystem::GetComponentState<DragValueStateMap>();
}

void PruneDragValueStates(float maxIdleSeconds) {
    float currentTime = static_cast<float>(ImGui::GetTime());
    auto& states = GetDragValueStates();

    for (auto it = states.begin(); it != states.end(); ) {
        if (currentTime - it->second.lastTimeUsed > maxIdleSeconds) {
            it = states.erase(it);
        } else {
            ++it;
        }
    }
}

static float GetChangeFeedbackAnim(ImGuiID id, float currentValue, float feedbackDuration) {
    auto& states = GetDragValueStates();
    auto& state = states[id];

    // Track when this state was last used for GC purposes
    state.lastTimeUsed = static_cast<float>(ImGui::GetTime());

    float dt = ImGui::GetIO().DeltaTime;

    // Check if value changed
    if (currentValue != state.lastValue) {
        state.changeTime = feedbackDuration;
        state.lastValue = currentValue;
    }

    // Decay the change feedback
    if (state.changeTime > 0.0f) {
        state.changeTime = std::max(0.0f, state.changeTime - dt);
    }

    if (feedbackDuration <= 0.0f)
        return 0.0f;
    return state.changeTime / feedbackDuration;
}

// =============================================
// Internal: Draw drag indicator
// =============================================

static void DrawDragIndicator(ImDrawList* draw, const ImRect& bb, const DragStyle& style,
                               float hoverAnim, bool isActive) {
    if (hoverAnim < 0.01f && !isActive)
        return;

    float alpha = isActive ? 1.0f : hoverAnim;

    // Draw small vertical bars on left side as drag indicator
    float barHeight = bb.GetHeight() - style.dragIndicatorPadding * 2;
    float barY = bb.Min.y + style.dragIndicatorPadding;
    float barX = bb.Min.x + style.dragIndicatorPadding;

    ImU32 indicatorColor = (style.dragIndicatorColor & 0x00FFFFFF) |
                           (static_cast<ImU32>(alpha * 255) << 24);

    // Draw two small vertical bars
    draw->AddRectFilled(
        ImVec2(barX, barY),
        ImVec2(barX + style.dragIndicatorWidth, barY + barHeight),
        indicatorColor, 1.0f);
    draw->AddRectFilled(
        ImVec2(barX + style.dragIndicatorWidth + 2, barY),
        ImVec2(barX + style.dragIndicatorWidth * 2 + 2, barY + barHeight),
        indicatorColor, 1.0f);
}

// =============================================
// DragFloat Implementation
// =============================================

bool DragFloatEx(const char* label, float* v, float v_speed,
                 float v_min, float v_max, const char* format,
                 ImGuiSliderFlags flags, const DragStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);

    // Get animation states
    Animation::WidgetState& animState = Animation::GetState(id);

    // Push style colors
    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    // Get item rect for custom rendering
    ImRect itemRect = g.LastItemData.Rect;
    bool hovered = ImGui::IsItemHovered();
    bool active = ImGui::IsItemActive();

    Animation::UpdateWidgetState(animState, hovered, active, false);

    ImDrawList* draw = window->DrawList;

    // Draw drag indicator on hover
    DrawDragIndicator(draw, itemRect, style, animState.hoverAnim, active);

    // Draw border glow
    ImU32 borderGlowColor = active ? style.borderActiveColor : style.borderHoverColor;
    if (animState.hoverAnim > 0.01f || active) {
        float glowAlpha = active ? 1.0f : animState.hoverAnim;
        Draw::RectGlow(itemRect.Min, itemRect.Max,
                       (borderGlowColor & 0x00FFFFFF) | IM_COL32(0, 0, 0, static_cast<int>(100 * glowAlpha)),
                       style.glowIntensity * glowAlpha, 3.0f);
    }

    // Draw value change feedback glow
    float changeFeedback = GetChangeFeedbackAnim(id, *v, style.changeFeedbackDuration);
    if (changeFeedback > 0.01f && changed) {
        Draw::RectGlow(itemRect.Min, itemRect.Max, style.changeGlowColor,
                       style.glowIntensity * changeFeedback, 5.0f);
    }

    return changed;
}

bool DragFloat(const char* label, float* v, float v_speed,
               float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
    return DragFloatEx(label, v, v_speed, v_min, v_max, format, flags,
                       StyleSystem::GetCurrentStyle<DragStyle>());
}

// =============================================
// DragFloat2/3/4 Implementation
// =============================================

bool DragFloat2(const char* label, float v[2], float v_speed,
                float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
    DragStyle style = StyleSystem::GetCurrentStyle<DragStyle>();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

bool DragFloat3(const char* label, float v[3], float v_speed,
                float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
    DragStyle style = StyleSystem::GetCurrentStyle<DragStyle>();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

bool DragFloat4(const char* label, float v[4], float v_speed,
                float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
    DragStyle style = StyleSystem::GetCurrentStyle<DragStyle>();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

bool DragFloatRange2(const char* label, float* v_current_min, float* v_current_max,
                     float v_speed, float v_min, float v_max,
                     const char* format, const char* format_max, ImGuiSliderFlags flags) {
    DragStyle style = StyleSystem::GetCurrentStyle<DragStyle>();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragFloatRange2(label, v_current_min, v_current_max,
                                           v_speed, v_min, v_max, format, format_max, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

// =============================================
// DragInt Implementation
// =============================================

bool DragIntEx(const char* label, int* v, float v_speed,
               int v_min, int v_max, const char* format,
               ImGuiSliderFlags flags, const DragStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);

    // Get animation states
    Animation::WidgetState& animState = Animation::GetState(id);

    // Push style colors
    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragInt(label, v, v_speed, v_min, v_max, format, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    // Get item rect for custom rendering
    ImRect itemRect = g.LastItemData.Rect;
    bool hovered = ImGui::IsItemHovered();
    bool active = ImGui::IsItemActive();

    Animation::UpdateWidgetState(animState, hovered, active, false);

    ImDrawList* draw = window->DrawList;

    // Draw drag indicator on hover
    DrawDragIndicator(draw, itemRect, style, animState.hoverAnim, active);

    // Draw border glow
    ImU32 borderGlowColor = active ? style.borderActiveColor : style.borderHoverColor;
    if (animState.hoverAnim > 0.01f || active) {
        float glowAlpha = active ? 1.0f : animState.hoverAnim;
        Draw::RectGlow(itemRect.Min, itemRect.Max,
                       (borderGlowColor & 0x00FFFFFF) | IM_COL32(0, 0, 0, static_cast<int>(100 * glowAlpha)),
                       style.glowIntensity * glowAlpha, 3.0f);
    }

    // Draw value change feedback glow
    float changeFeedback = GetChangeFeedbackAnim(id, static_cast<float>(*v), style.changeFeedbackDuration);
    if (changeFeedback > 0.01f && changed) {
        Draw::RectGlow(itemRect.Min, itemRect.Max, style.changeGlowColor,
                       style.glowIntensity * changeFeedback, 5.0f);
    }

    return changed;
}

bool DragInt(const char* label, int* v, float v_speed,
             int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
    return DragIntEx(label, v, v_speed, v_min, v_max, format, flags,
                     StyleSystem::GetCurrentStyle<DragStyle>());
}

// =============================================
// DragInt2/3/4 Implementation
// =============================================

bool DragInt2(const char* label, int v[2], float v_speed,
              int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
    DragStyle style = StyleSystem::GetCurrentStyle<DragStyle>();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragInt2(label, v, v_speed, v_min, v_max, format, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

bool DragInt3(const char* label, int v[3], float v_speed,
              int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
    DragStyle style = StyleSystem::GetCurrentStyle<DragStyle>();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragInt3(label, v, v_speed, v_min, v_max, format, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

bool DragInt4(const char* label, int v[4], float v_speed,
              int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
    DragStyle style = StyleSystem::GetCurrentStyle<DragStyle>();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragInt4(label, v, v_speed, v_min, v_max, format, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

bool DragIntRange2(const char* label, int* v_current_min, int* v_current_max,
                   float v_speed, int v_min, int v_max,
                   const char* format, const char* format_max, ImGuiSliderFlags flags) {
    DragStyle style = StyleSystem::GetCurrentStyle<DragStyle>();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    bool changed = ImGui::DragIntRange2(label, v_current_min, v_current_max,
                                         v_speed, v_min, v_max, format, format_max, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

} // namespace EFIGUI
