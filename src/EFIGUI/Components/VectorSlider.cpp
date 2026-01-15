// src/EFIGUI/Components/VectorSlider.cpp
#include "VectorSlider.h"
#include "../Core/Animation.h"
#include "../Core/Draw.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <algorithm>

namespace EFIGUI {

// =============================================
// Internal: Get accent color for component index
// =============================================

static ImU32 GetSliderAccentColor(int idx, const VectorSliderStyle& style) {
    switch (idx) {
        case 0: return style.xAccentColor;
        case 1: return style.yAccentColor;
        case 2: return style.zAccentColor;
        case 3: return style.wAccentColor;
        default: return style.xAccentColor;
    }
}

static ImU32 GetSliderLabelColor(int idx, const VectorSliderStyle& style) {
    switch (idx) {
        case 0: return style.xLabelColor;
        case 1: return style.yLabelColor;
        case 2: return style.zLabelColor;
        case 3: return style.wLabelColor;
        default: return style.xLabelColor;
    }
}

static const char* GetSliderLabel(int idx) {
    switch (idx) {
        case 0: return "X";
        case 1: return "Y";
        case 2: return "Z";
        case 3: return "W";
        default: return "?";
    }
}

// =============================================
// Internal: Draw single slider component
// =============================================

static bool DrawVectorSliderComponent(const char* id, float* v, float v_min, float v_max,
                                       const char* format, ImGuiSliderFlags flags,
                                       int componentIdx, float sliderWidth,
                                       const VectorSliderStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImDrawList* draw = window->DrawList;

    ImVec2 pos = ImGui::GetCursorScreenPos();

    // Push frame colors
    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, style.knobColor);
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, style.knobActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, style.knobRadius);

    // Draw component label
    ImU32 labelColor = GetSliderLabelColor(componentIdx, style);
    const char* label = GetSliderLabel(componentIdx);
    ImVec2 labelSize = ImGui::CalcTextSize(label);

    float labelX = pos.x + 4.0f;
    float labelY = pos.y + (ImGui::GetFrameHeight() - labelSize.y) * 0.5f;
    draw->AddText(ImVec2(labelX, labelY), labelColor, label);

    // Advance cursor past label
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + style.labelWidth);

    // Slider
    ImGui::PushItemWidth(sliderWidth - style.labelWidth);
    bool changed = ImGui::SliderFloat(id, v, v_min, v_max, format, flags);
    ImGui::PopItemWidth();

    // Get item rect for accent bar
    ImRect itemRect = GImGui->LastItemData.Rect;

    // Draw accent bar on left side
    ImU32 accentColor = GetSliderAccentColor(componentIdx, style);
    ImVec2 accentMin(itemRect.Min.x - style.labelWidth - 2, itemRect.Min.y);
    ImVec2 accentMax(accentMin.x + style.accentWidth, itemRect.Max.y);
    draw->AddRectFilled(accentMin, accentMax, accentColor, 1.0f);

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(7);

    return changed;
}

static bool DrawVectorSliderComponentInt(const char* id, int* v, int v_min, int v_max,
                                          const char* format, ImGuiSliderFlags flags,
                                          int componentIdx, float sliderWidth,
                                          const VectorSliderStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImDrawList* draw = window->DrawList;

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, style.knobColor);
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, style.knobActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, style.knobRadius);

    ImU32 labelColor = GetSliderLabelColor(componentIdx, style);
    const char* label = GetSliderLabel(componentIdx);
    ImVec2 labelSize = ImGui::CalcTextSize(label);

    float labelX = pos.x + 4.0f;
    float labelY = pos.y + (ImGui::GetFrameHeight() - labelSize.y) * 0.5f;
    draw->AddText(ImVec2(labelX, labelY), labelColor, label);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + style.labelWidth);

    ImGui::PushItemWidth(sliderWidth - style.labelWidth);
    bool changed = ImGui::SliderInt(id, v, v_min, v_max, format, flags);
    ImGui::PopItemWidth();

    ImRect itemRect = GImGui->LastItemData.Rect;

    ImU32 accentColor = GetSliderAccentColor(componentIdx, style);
    ImVec2 accentMin(itemRect.Min.x - style.labelWidth - 2, itemRect.Min.y);
    ImVec2 accentMax(accentMin.x + style.accentWidth, itemRect.Max.y);
    draw->AddRectFilled(accentMin, accentMax, accentColor, 1.0f);

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(7);

    return changed;
}

// =============================================
// SliderFloat2/3/4 Implementation
// =============================================

bool SliderFloat2Ex(const char* label, float v[2], float v_min, float v_max,
                    const char* format, ImGuiSliderFlags flags,
                    const VectorSliderStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    bool changed = false;

    ImGui::BeginGroup();
    ImGui::PushID(label);

    bool labelHidden = (label[0] == '#' && label[1] == '#');
    ImVec2 labelSize = labelHidden ? ImVec2(0, 0) : ImGui::CalcTextSize(label);
    float fullWidth = ImGui::GetContentRegionAvail().x;
    float labelWidth = labelHidden ? 0 : (labelSize.x + ImGui::GetStyle().ItemSpacing.x);
    float availWidth = fullWidth - labelWidth;
    float sliderWidth = (availWidth - style.componentSpacing) / 2.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorSliderComponent("##X", &v[0], v_min, v_max, format, flags, 0, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponent("##Y", &v[1], v_min, v_max, format, flags, 1, sliderWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool SliderFloat2(const char* label, float v[2], float v_min, float v_max,
                  const char* format, ImGuiSliderFlags flags) {
    return SliderFloat2Ex(label, v, v_min, v_max, format, flags,
                          StyleSystem::GetCurrentStyle<VectorSliderStyle>());
}

bool SliderFloat3Ex(const char* label, float v[3], float v_min, float v_max,
                    const char* format, ImGuiSliderFlags flags,
                    const VectorSliderStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    bool changed = false;

    ImGui::BeginGroup();
    ImGui::PushID(label);

    bool labelHidden = (label[0] == '#' && label[1] == '#');
    ImVec2 labelSize = labelHidden ? ImVec2(0, 0) : ImGui::CalcTextSize(label);
    float fullWidth = ImGui::GetContentRegionAvail().x;
    float labelWidth = labelHidden ? 0 : (labelSize.x + ImGui::GetStyle().ItemSpacing.x);
    float availWidth = fullWidth - labelWidth;
    float sliderWidth = (availWidth - style.componentSpacing * 2) / 3.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorSliderComponent("##X", &v[0], v_min, v_max, format, flags, 0, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponent("##Y", &v[1], v_min, v_max, format, flags, 1, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponent("##Z", &v[2], v_min, v_max, format, flags, 2, sliderWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool SliderFloat3(const char* label, float v[3], float v_min, float v_max,
                  const char* format, ImGuiSliderFlags flags) {
    return SliderFloat3Ex(label, v, v_min, v_max, format, flags,
                          StyleSystem::GetCurrentStyle<VectorSliderStyle>());
}

bool SliderFloat4Ex(const char* label, float v[4], float v_min, float v_max,
                    const char* format, ImGuiSliderFlags flags,
                    const VectorSliderStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    bool changed = false;

    ImGui::BeginGroup();
    ImGui::PushID(label);

    bool labelHidden = (label[0] == '#' && label[1] == '#');
    ImVec2 labelSize = labelHidden ? ImVec2(0, 0) : ImGui::CalcTextSize(label);
    float fullWidth = ImGui::GetContentRegionAvail().x;
    float labelWidth = labelHidden ? 0 : (labelSize.x + ImGui::GetStyle().ItemSpacing.x);
    float availWidth = fullWidth - labelWidth;
    float sliderWidth = (availWidth - style.componentSpacing * 3) / 4.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorSliderComponent("##X", &v[0], v_min, v_max, format, flags, 0, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponent("##Y", &v[1], v_min, v_max, format, flags, 1, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponent("##Z", &v[2], v_min, v_max, format, flags, 2, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponent("##W", &v[3], v_min, v_max, format, flags, 3, sliderWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool SliderFloat4(const char* label, float v[4], float v_min, float v_max,
                  const char* format, ImGuiSliderFlags flags) {
    return SliderFloat4Ex(label, v, v_min, v_max, format, flags,
                          StyleSystem::GetCurrentStyle<VectorSliderStyle>());
}

// =============================================
// SliderInt2/3/4 Implementation
// =============================================

bool SliderInt2Ex(const char* label, int v[2], int v_min, int v_max,
                  const char* format, ImGuiSliderFlags flags,
                  const VectorSliderStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    bool changed = false;

    ImGui::BeginGroup();
    ImGui::PushID(label);

    bool labelHidden = (label[0] == '#' && label[1] == '#');
    ImVec2 labelSize = labelHidden ? ImVec2(0, 0) : ImGui::CalcTextSize(label);
    float fullWidth = ImGui::GetContentRegionAvail().x;
    float labelWidth = labelHidden ? 0 : (labelSize.x + ImGui::GetStyle().ItemSpacing.x);
    float availWidth = fullWidth - labelWidth;
    float sliderWidth = (availWidth - style.componentSpacing) / 2.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorSliderComponentInt("##X", &v[0], v_min, v_max, format, flags, 0, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponentInt("##Y", &v[1], v_min, v_max, format, flags, 1, sliderWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool SliderInt2(const char* label, int v[2], int v_min, int v_max,
                const char* format, ImGuiSliderFlags flags) {
    return SliderInt2Ex(label, v, v_min, v_max, format, flags,
                        StyleSystem::GetCurrentStyle<VectorSliderStyle>());
}

bool SliderInt3Ex(const char* label, int v[3], int v_min, int v_max,
                  const char* format, ImGuiSliderFlags flags,
                  const VectorSliderStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    bool changed = false;

    ImGui::BeginGroup();
    ImGui::PushID(label);

    bool labelHidden = (label[0] == '#' && label[1] == '#');
    ImVec2 labelSize = labelHidden ? ImVec2(0, 0) : ImGui::CalcTextSize(label);
    float fullWidth = ImGui::GetContentRegionAvail().x;
    float labelWidth = labelHidden ? 0 : (labelSize.x + ImGui::GetStyle().ItemSpacing.x);
    float availWidth = fullWidth - labelWidth;
    float sliderWidth = (availWidth - style.componentSpacing * 2) / 3.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorSliderComponentInt("##X", &v[0], v_min, v_max, format, flags, 0, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponentInt("##Y", &v[1], v_min, v_max, format, flags, 1, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponentInt("##Z", &v[2], v_min, v_max, format, flags, 2, sliderWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool SliderInt3(const char* label, int v[3], int v_min, int v_max,
                const char* format, ImGuiSliderFlags flags) {
    return SliderInt3Ex(label, v, v_min, v_max, format, flags,
                        StyleSystem::GetCurrentStyle<VectorSliderStyle>());
}

bool SliderInt4Ex(const char* label, int v[4], int v_min, int v_max,
                  const char* format, ImGuiSliderFlags flags,
                  const VectorSliderStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    bool changed = false;

    ImGui::BeginGroup();
    ImGui::PushID(label);

    bool labelHidden = (label[0] == '#' && label[1] == '#');
    ImVec2 labelSize = labelHidden ? ImVec2(0, 0) : ImGui::CalcTextSize(label);
    float fullWidth = ImGui::GetContentRegionAvail().x;
    float labelWidth = labelHidden ? 0 : (labelSize.x + ImGui::GetStyle().ItemSpacing.x);
    float availWidth = fullWidth - labelWidth;
    float sliderWidth = (availWidth - style.componentSpacing * 3) / 4.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorSliderComponentInt("##X", &v[0], v_min, v_max, format, flags, 0, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponentInt("##Y", &v[1], v_min, v_max, format, flags, 1, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponentInt("##Z", &v[2], v_min, v_max, format, flags, 2, sliderWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorSliderComponentInt("##W", &v[3], v_min, v_max, format, flags, 3, sliderWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool SliderInt4(const char* label, int v[4], int v_min, int v_max,
                const char* format, ImGuiSliderFlags flags) {
    return SliderInt4Ex(label, v, v_min, v_max, format, flags,
                        StyleSystem::GetCurrentStyle<VectorSliderStyle>());
}

} // namespace EFIGUI
