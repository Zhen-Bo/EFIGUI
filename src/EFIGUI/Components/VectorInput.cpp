// src/EFIGUI/Components/VectorInput.cpp
#include "VectorInput.h"
#include "../Core/Animation.h"
#include "../Core/Draw.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <algorithm>
#include <cstdio>

namespace EFIGUI {

static ImU32 GetComponentAccentColor(int idx, const VectorInputStyle& style) {
    switch (idx) {
        case 0: return style.xAccentColor;
        case 1: return style.yAccentColor;
        case 2: return style.zAccentColor;
        case 3: return style.wAccentColor;
        default: return style.xAccentColor;
    }
}

static ImU32 GetComponentLabelColor(int idx, const VectorInputStyle& style) {
    switch (idx) {
        case 0: return style.xLabelColor;
        case 1: return style.yLabelColor;
        case 2: return style.zLabelColor;
        case 3: return style.wLabelColor;
        default: return style.xLabelColor;
    }
}

static const char* GetComponentLabel(int idx) {
    switch (idx) {
        case 0: return "X";
        case 1: return "Y";
        case 2: return "Z";
        case 3: return "W";
        default: return "?";
    }
}

static bool DrawVectorInputComponent(const char* id, float* v, const char* format,
                                      ImGuiInputTextFlags flags, int componentIdx,
                                      float inputWidth, const VectorInputStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImDrawList* draw = window->DrawList;

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    // Draw component label
    ImU32 labelColor = GetComponentLabelColor(componentIdx, style);
    const char* label = GetComponentLabel(componentIdx);
    ImVec2 labelSize = ImGui::CalcTextSize(label);

    float labelX = pos.x + 4.0f;
    float labelY = pos.y + (ImGui::GetFrameHeight() - labelSize.y) * 0.5f;
    draw->AddText(ImVec2(labelX, labelY), labelColor, label);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + style.labelWidth);

    ImGui::PushItemWidth(inputWidth - style.labelWidth);
    bool changed = ImGui::InputFloat(id, v, 0, 0, format, flags);
    ImGui::PopItemWidth();

    ImRect itemRect = GImGui->LastItemData.Rect;

    // Draw accent bar
    ImU32 accentColor = GetComponentAccentColor(componentIdx, style);
    ImVec2 accentMin(itemRect.Min.x - style.labelWidth - 2, itemRect.Min.y);
    ImVec2 accentMax(accentMin.x + style.accentWidth, itemRect.Max.y);
    draw->AddRectFilled(accentMin, accentMax, accentColor, 1.0f);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

static bool DrawVectorInputComponentInt(const char* id, int* v,
                                         ImGuiInputTextFlags flags, int componentIdx,
                                         float inputWidth, const VectorInputStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImDrawList* draw = window->DrawList;

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.frameHoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, style.frameActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.borderWidth);

    ImU32 labelColor = GetComponentLabelColor(componentIdx, style);
    const char* label = GetComponentLabel(componentIdx);
    ImVec2 labelSize = ImGui::CalcTextSize(label);

    float labelX = pos.x + 4.0f;
    float labelY = pos.y + (ImGui::GetFrameHeight() - labelSize.y) * 0.5f;
    draw->AddText(ImVec2(labelX, labelY), labelColor, label);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + style.labelWidth);

    ImGui::PushItemWidth(inputWidth - style.labelWidth);
    bool changed = ImGui::InputInt(id, v, 0, 0, flags);
    ImGui::PopItemWidth();

    ImRect itemRect = GImGui->LastItemData.Rect;

    ImU32 accentColor = GetComponentAccentColor(componentIdx, style);
    ImVec2 accentMin(itemRect.Min.x - style.labelWidth - 2, itemRect.Min.y);
    ImVec2 accentMax(accentMin.x + style.accentWidth, itemRect.Max.y);
    draw->AddRectFilled(accentMin, accentMax, accentColor, 1.0f);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

// InputFloat2/3/4 Implementations

bool InputFloat2Ex(const char* label, float v[2], const char* format,
                   ImGuiInputTextFlags flags, const VectorInputStyle& style) {
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
    float inputWidth = (availWidth - style.componentSpacing) / 2.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorInputComponent("##X", &v[0], format, flags, 0, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponent("##Y", &v[1], format, flags, 1, inputWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool InputFloat2(const char* label, float v[2], const char* format, ImGuiInputTextFlags flags) {
    return InputFloat2Ex(label, v, format, flags, StyleSystem::GetCurrentStyle<VectorInputStyle>());
}

bool InputFloat3Ex(const char* label, float v[3], const char* format,
                   ImGuiInputTextFlags flags, const VectorInputStyle& style) {
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
    float inputWidth = (availWidth - style.componentSpacing * 2) / 3.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorInputComponent("##X", &v[0], format, flags, 0, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponent("##Y", &v[1], format, flags, 1, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponent("##Z", &v[2], format, flags, 2, inputWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool InputFloat3(const char* label, float v[3], const char* format, ImGuiInputTextFlags flags) {
    return InputFloat3Ex(label, v, format, flags, StyleSystem::GetCurrentStyle<VectorInputStyle>());
}

bool InputFloat4Ex(const char* label, float v[4], const char* format,
                   ImGuiInputTextFlags flags, const VectorInputStyle& style) {
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
    float inputWidth = (availWidth - style.componentSpacing * 3) / 4.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorInputComponent("##X", &v[0], format, flags, 0, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponent("##Y", &v[1], format, flags, 1, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponent("##Z", &v[2], format, flags, 2, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponent("##W", &v[3], format, flags, 3, inputWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool InputFloat4(const char* label, float v[4], const char* format, ImGuiInputTextFlags flags) {
    return InputFloat4Ex(label, v, format, flags, StyleSystem::GetCurrentStyle<VectorInputStyle>());
}

// InputInt2/3/4 Implementations

bool InputInt2Ex(const char* label, int v[2], ImGuiInputTextFlags flags,
                 const VectorInputStyle& style) {
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
    float inputWidth = (availWidth - style.componentSpacing) / 2.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorInputComponentInt("##X", &v[0], flags, 0, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponentInt("##Y", &v[1], flags, 1, inputWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool InputInt2(const char* label, int v[2], ImGuiInputTextFlags flags) {
    return InputInt2Ex(label, v, flags, StyleSystem::GetCurrentStyle<VectorInputStyle>());
}

bool InputInt3Ex(const char* label, int v[3], ImGuiInputTextFlags flags,
                 const VectorInputStyle& style) {
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
    float inputWidth = (availWidth - style.componentSpacing * 2) / 3.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorInputComponentInt("##X", &v[0], flags, 0, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponentInt("##Y", &v[1], flags, 1, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponentInt("##Z", &v[2], flags, 2, inputWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool InputInt3(const char* label, int v[3], ImGuiInputTextFlags flags) {
    return InputInt3Ex(label, v, flags, StyleSystem::GetCurrentStyle<VectorInputStyle>());
}

bool InputInt4Ex(const char* label, int v[4], ImGuiInputTextFlags flags,
                 const VectorInputStyle& style) {
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
    float inputWidth = (availWidth - style.componentSpacing * 3) / 4.0f;

    if (!labelHidden) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
    }

    if (DrawVectorInputComponentInt("##X", &v[0], flags, 0, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponentInt("##Y", &v[1], flags, 1, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponentInt("##Z", &v[2], flags, 2, inputWidth, style))
        changed = true;

    ImGui::SameLine(0, style.componentSpacing);

    if (DrawVectorInputComponentInt("##W", &v[3], flags, 3, inputWidth, style))
        changed = true;

    ImGui::PopID();
    ImGui::EndGroup();

    return changed;
}

bool InputInt4(const char* label, int v[4], ImGuiInputTextFlags flags) {
    return InputInt4Ex(label, v, flags, StyleSystem::GetCurrentStyle<VectorInputStyle>());
}

} // namespace EFIGUI
