// src/EFIGUI/Styles/RadioStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for RadioButton component
struct RadioStyle {
    // Colors
    ImU32 dotColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 textColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 circleColor = CyberpunkTheme::Colors::Surface;
    ImU32 circleHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 circleActiveColor = CyberpunkTheme::Colors::SurfaceActive;
    ImU32 borderColor = CyberpunkTheme::Colors::Border;
    ImU32 borderHoverColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;

    // Dimensions
    float radius = 9.0f;
    float dotRadius = 4.0f;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float labelGap = 6.0f;

    // Animation
    float animSpeed = 0.15f;
    int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};

} // namespace EFIGUI
