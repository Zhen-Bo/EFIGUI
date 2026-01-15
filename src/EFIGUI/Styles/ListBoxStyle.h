// src/EFIGUI/Styles/ListBoxStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for ListBox component
struct ListBoxStyle {
    // Frame colors
    ImU32 frameBgColor = CyberpunkTheme::Colors::Surface;
    ImU32 frameBorderColor = CyberpunkTheme::Colors::Border;

    // Item colors (inherits from Selectable pattern)
    ImU32 itemBgColor = IM_COL32(0, 0, 0, 0);
    ImU32 itemBgHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 itemBgSelectedColor = IM_COL32(0, 245, 255, 40);
    ImU32 itemTextColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 itemTextSelectedColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 accentColor = CyberpunkTheme::Colors::AccentCyan;

    // Scrollbar
    ImU32 scrollbarBgColor = IM_COL32(25, 28, 38, 100);
    ImU32 scrollbarGrabColor = CyberpunkTheme::Colors::Border;
    ImU32 scrollbarGrabHoverColor = CyberpunkTheme::Colors::AccentCyan;

    // Separator between items
    ImU32 separatorColor = IM_COL32(60, 65, 80, 80);
    bool showSeparators = false;

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float itemPadding = 4.0f;
    float accentBarWidth = 3.0f;
};

} // namespace EFIGUI
