// src/EFIGUI/Styles/TableStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for Table component
struct TableStyle {
    // Header colors
    ImU32 headerBgColor = CyberpunkTheme::Colors::SurfaceActive;
    ImU32 headerTextColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 headerBorderColor = CyberpunkTheme::Colors::Border;

    // Row colors
    ImU32 rowBgColor = IM_COL32(0, 0, 0, 0);
    ImU32 rowBgAltColor = IM_COL32(30, 35, 50, 100);  // Alternating row color
    ImU32 rowHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 rowSelectedColor = IM_COL32(0, 245, 255, 40);

    // Cell colors
    ImU32 cellTextColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 cellBorderColor = CyberpunkTheme::Colors::Border;

    // Resize handle
    ImU32 resizeHandleColor = CyberpunkTheme::Colors::Border;
    ImU32 resizeHandleHoverColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 resizeHandleGlowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    float resizeGlowIntensity = 0.5f;

    // Borders
    ImU32 outerBorderColor = CyberpunkTheme::Colors::Border;
    ImU32 innerBorderColor = IM_COL32(60, 65, 80, 100);

    // Dimensions
    float headerPadding = 8.0f;
    float cellPadding = 6.0f;
    float rounding = CyberpunkTheme::Dimensions::Rounding;
};

} // namespace EFIGUI
