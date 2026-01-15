// src/EFIGUI/Styles/TabBarStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for TabBar and TabItem components
struct TabBarStyle {
    // Tab bar background
    ImU32 barBgColor = CyberpunkTheme::Colors::Surface;
    ImU32 barBorderColor = CyberpunkTheme::Colors::Border;

    // Tab item colors
    ImU32 tabBgColor = IM_COL32(0, 0, 0, 0);  // Transparent
    ImU32 tabBgHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 tabBgActiveColor = IM_COL32(0, 245, 255, 30);
    ImU32 tabTextColor = CyberpunkTheme::Colors::TextSecondary;
    ImU32 tabTextActiveColor = CyberpunkTheme::Colors::AccentCyan;

    // Indicator (animated underline)
    ImU32 indicatorColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 indicatorGlowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    float indicatorHeight = 2.0f;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float tabPadding = 12.0f;
    float spacing = 4.0f;
};

} // namespace EFIGUI
