// src/EFIGUI/Styles/SelectableStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for Selectable component
struct SelectableStyle {
    // Colors
    ImU32 bgColor = IM_COL32(0, 0, 0, 0);  // Transparent by default
    ImU32 bgHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 bgSelectedColor = IM_COL32(0, 245, 255, 40);
    ImU32 textColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 textSelectedColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 accentColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 borderColor = CyberpunkTheme::Colors::Border;
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float accentBarWidth = 3.0f;
    float padding = 4.0f;

    // Animation
    float animSpeed = 0.15f;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};

} // namespace EFIGUI
