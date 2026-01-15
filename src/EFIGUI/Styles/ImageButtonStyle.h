// src/EFIGUI/Styles/ImageButtonStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for ImageButton components
struct ImageButtonStyle {
    // Frame colors
    ImU32 bgColor = CyberpunkTheme::Colors::Surface;
    ImU32 bgHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 bgActiveColor = CyberpunkTheme::Colors::SurfaceActive;
    ImU32 borderColor = CyberpunkTheme::Colors::Border;
    ImU32 borderHoverColor = CyberpunkTheme::Colors::AccentCyan;

    // Image tint
    ImU32 tintColor = IM_COL32(255, 255, 255, 255);  // Normal tint
    ImU32 tintHoverColor = IM_COL32(255, 255, 255, 255);  // Hover tint
    ImU32 tintActiveColor = IM_COL32(200, 200, 200, 255);  // Pressed tint
    ImU32 tintDisabledColor = IM_COL32(128, 128, 128, 128);  // Disabled tint

    // Glow effects
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    bool showGlow = true;

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float padding = 4.0f;  // Padding around image

    // Animation
    float animSpeed = 0.15f;
    int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};

} // namespace EFIGUI
