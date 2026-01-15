// src/EFIGUI/Styles/MenuStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for Menu components
struct MenuStyle {
    // Popup/menu background
    ImU32 popupBgColor = CyberpunkTheme::Colors::Surface;
    ImU32 popupBorderColor = CyberpunkTheme::Colors::Border;

    // Menu item colors
    ImU32 itemBgColor = IM_COL32(0, 0, 0, 0);
    ImU32 itemBgHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 itemTextColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 itemTextHoverColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 itemTextDisabledColor = CyberpunkTheme::Colors::TextMuted;

    // Keyboard shortcut text
    ImU32 shortcutTextColor = CyberpunkTheme::Colors::TextMuted;

    // Submenu arrow
    ImU32 arrowColor = CyberpunkTheme::Colors::TextSecondary;
    ImU32 arrowHoverColor = CyberpunkTheme::Colors::AccentCyan;

    // Check mark
    ImU32 checkmarkColor = CyberpunkTheme::Colors::AccentCyan;

    // Separator
    ImU32 separatorColor = CyberpunkTheme::Colors::Border;

    // Header styling
    ImU32 headerBgColor = IM_COL32(0, 245, 255, 20);
    ImU32 headerTextColor = CyberpunkTheme::Colors::AccentCyan;

    // Glow effects
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    float glowIntensity = 0.3f;

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float itemPadding = 8.0f;
    float minWidth = 150.0f;
};

} // namespace EFIGUI
