// src/EFIGUI/Components/ColorEdit.cpp
#include "ColorEdit.h"
#include "../Core/Animation.h"
#include "../Core/Draw.h"
#include "../Core/Layer.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <algorithm>

namespace EFIGUI {

// =============================================
// Internal: Draw alpha checkerboard pattern
// =============================================

static void DrawAlphaCheckerboard(ImDrawList* draw, ImVec2 p_min, ImVec2 p_max,
                                   ImU32 light, ImU32 dark, float size, float rounding) {
    // Draw dark background first
    draw->AddRectFilled(p_min, p_max, dark, rounding);

    // Draw light squares in checkerboard pattern
    int cols = static_cast<int>((p_max.x - p_min.x) / size) + 1;
    int rows = static_cast<int>((p_max.y - p_min.y) / size) + 1;

    draw->PushClipRect(p_min, p_max, true);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if ((x + y) % 2 == 0) {
                ImVec2 sq_min(p_min.x + x * size, p_min.y + y * size);
                ImVec2 sq_max(sq_min.x + size, sq_min.y + size);
                draw->AddRectFilled(sq_min, sq_max, light, 0.0f);
            }
        }
    }
    draw->PopClipRect();
}

// =============================================
// ColorButton Implementation
// =============================================

bool ColorButtonEx(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags,
                   const ImVec2& size, const ColorEditStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(desc_id);

    float default_size = ImGui::GetFrameHeight();
    ImVec2 sz(size.x == 0 ? default_size : size.x, size.y == 0 ? default_size : size.y);

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, ImVec2(pos.x + sz.x, pos.y + sz.y));

    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Get animation state
    Animation::WidgetState& state = Animation::GetState(id);
    Animation::UpdateWidgetState(state, hovered, held, false);

    ImDrawList* draw = window->DrawList;

    // Draw alpha checkerboard if color has alpha
    bool has_alpha = !(flags & ImGuiColorEditFlags_NoAlpha) && col.w < 1.0f;
    if (has_alpha) {
        DrawAlphaCheckerboard(draw, bb.Min, bb.Max,
                               style.alphaCheckerLight, style.alphaCheckerDark,
                               style.alphaCheckerSize, style.swatchRounding);
    }

    // Draw color swatch
    ImU32 col32 = ImGui::ColorConvertFloat4ToU32(col);
    draw->AddRectFilled(bb.Min, bb.Max, col32, style.swatchRounding);

    // Draw border
    ImU32 borderColor = (hovered || state.hoverAnim > 0.1f)
                            ? style.swatchBorderHoverColor
                            : style.swatchBorderColor;
    draw->AddRect(bb.Min, bb.Max, borderColor, style.swatchRounding, 0, style.swatchBorderWidth);

    // Draw glow on hover
    if (state.hoverAnim > 0.01f) {
        Draw::RectGlow(bb.Min, bb.Max, style.swatchGlowColor,
                       style.glowIntensity * state.hoverAnim, 4.0f);
    }

    return pressed;
}

bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, const ImVec2& size) {
    return ColorButtonEx(desc_id, col, flags, size, StyleSystem::GetCurrentStyle<ColorEditStyle>());
}

// =============================================
// ColorEdit3/4 Implementation
// =============================================

bool ColorEdit3Ex(const char* label, float col[3], ImGuiColorEditFlags flags, const ColorEditStyle& style) {
    // Push style colors for ImGui's ColorEdit
    ImGui::PushStyleColor(ImGuiCol_FrameBg, CyberpunkTheme::Colors::Surface);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, CyberpunkTheme::Colors::SurfaceHover);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, CyberpunkTheme::Colors::SurfaceActive);
    ImGui::PushStyleColor(ImGuiCol_Border, style.swatchBorderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.swatchRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.swatchBorderWidth);

    // Force NoAlpha for 3-component version
    flags |= ImGuiColorEditFlags_NoAlpha;

    bool changed = ImGui::ColorEdit3(label, col, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

bool ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags) {
    return ColorEdit3Ex(label, col, flags, StyleSystem::GetCurrentStyle<ColorEditStyle>());
}

bool ColorEdit4Ex(const char* label, float col[4], ImGuiColorEditFlags flags, const ColorEditStyle& style) {
    // Push style colors for ImGui's ColorEdit
    ImGui::PushStyleColor(ImGuiCol_FrameBg, CyberpunkTheme::Colors::Surface);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, CyberpunkTheme::Colors::SurfaceHover);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, CyberpunkTheme::Colors::SurfaceActive);
    ImGui::PushStyleColor(ImGuiCol_Border, style.swatchBorderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.swatchRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.swatchBorderWidth);

    bool changed = ImGui::ColorEdit4(label, col, flags);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    return changed;
}

bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags) {
    return ColorEdit4Ex(label, col, flags, StyleSystem::GetCurrentStyle<ColorEditStyle>());
}

// =============================================
// ColorPicker3/4 Implementation
// =============================================

bool ColorPicker3Ex(const char* label, float col[3], ImGuiColorEditFlags flags, const ColorEditStyle& style) {
    // Push style colors for picker
    ImGui::PushStyleColor(ImGuiCol_FrameBg, CyberpunkTheme::Colors::Surface);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, CyberpunkTheme::Colors::SurfaceHover);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, CyberpunkTheme::Colors::SurfaceActive);
    ImGui::PushStyleColor(ImGuiCol_Border, style.popupBorderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.popupRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.swatchBorderWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, style.popupRounding);

    // Force NoAlpha for 3-component version
    flags |= ImGuiColorEditFlags_NoAlpha;

    bool changed = ImGui::ColorPicker3(label, col, flags);

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(5);

    return changed;
}

bool ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags) {
    return ColorPicker3Ex(label, col, flags, StyleSystem::GetCurrentStyle<ColorEditStyle>());
}

bool ColorPicker4Ex(const char* label, float col[4], ImGuiColorEditFlags flags,
                    const float* ref_col, const ColorEditStyle& style) {
    // Push style colors for picker
    ImGui::PushStyleColor(ImGuiCol_FrameBg, CyberpunkTheme::Colors::Surface);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, CyberpunkTheme::Colors::SurfaceHover);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, CyberpunkTheme::Colors::SurfaceActive);
    ImGui::PushStyleColor(ImGuiCol_Border, style.popupBorderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.popupRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.swatchBorderWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, style.popupRounding);

    bool changed = ImGui::ColorPicker4(label, col, flags, ref_col);

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(5);

    return changed;
}

bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col) {
    return ColorPicker4Ex(label, col, flags, ref_col, StyleSystem::GetCurrentStyle<ColorEditStyle>());
}

} // namespace EFIGUI
