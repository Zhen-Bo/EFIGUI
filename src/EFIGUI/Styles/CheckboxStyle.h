// src/EFIGUI/Styles/CheckboxStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for Checkbox component
struct CheckboxStyle {
    // Colors
    ImU32 checkColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 textColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 boxColor = CyberpunkTheme::Colors::Surface;
    ImU32 boxHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 boxActiveColor = CyberpunkTheme::Colors::SurfaceActive;
    ImU32 borderColor = CyberpunkTheme::Colors::Border;
    ImU32 borderHoverColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;

    // Dimensions
    float size = 18.0f;
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float checkPadding = 3.0f;

    // Animation
    float animSpeed = 0.15f;
    int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};

} // namespace EFIGUI
