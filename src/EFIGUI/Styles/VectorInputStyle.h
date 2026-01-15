// src/EFIGUI/Styles/VectorInputStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for VectorInput components (InputFloat2/3/4, InputInt2/3/4)
struct VectorInputStyle {
    // Frame colors
    ImU32 frameColor = CyberpunkTheme::Colors::Surface;
    ImU32 frameHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 frameActiveColor = CyberpunkTheme::Colors::SurfaceActive;
    ImU32 borderColor = CyberpunkTheme::Colors::Border;
    ImU32 borderHoverColor = CyberpunkTheme::Colors::AccentCyan;

    // Component label colors (X, Y, Z, W)
    ImU32 xLabelColor = IM_COL32(255, 80, 80, 255);
    ImU32 yLabelColor = IM_COL32(80, 255, 80, 255);
    ImU32 zLabelColor = IM_COL32(80, 80, 255, 255);
    ImU32 wLabelColor = IM_COL32(255, 200, 80, 255);

    // Component accent bars
    ImU32 xAccentColor = IM_COL32(255, 80, 80, 200);
    ImU32 yAccentColor = IM_COL32(80, 255, 80, 200);
    ImU32 zAccentColor = IM_COL32(80, 80, 255, 200);
    ImU32 wAccentColor = IM_COL32(255, 200, 80, 200);
    float accentWidth = 3.0f;

    // Text
    ImU32 textColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 labelColor = CyberpunkTheme::Colors::TextSecondary;

    // Glow
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float padding = 4.0f;
    float componentSpacing = 4.0f;
    float labelWidth = 14.0f;

    // Animation
    float animSpeed = 0.15f;
    int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};

} // namespace EFIGUI
