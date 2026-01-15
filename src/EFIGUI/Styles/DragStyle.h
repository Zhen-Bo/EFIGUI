// src/EFIGUI/Styles/DragStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for Drag components (DragFloat, DragInt, etc.)
struct DragStyle {
    // Frame colors
    ImU32 frameColor = CyberpunkTheme::Colors::Surface;
    ImU32 frameHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 frameActiveColor = CyberpunkTheme::Colors::SurfaceActive;
    ImU32 borderColor = CyberpunkTheme::Colors::Border;
    ImU32 borderHoverColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 borderActiveColor = CyberpunkTheme::Colors::AccentPurple;

    // Glow effects
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    ImU32 changeGlowColor = CyberpunkTheme::Colors::AccentPurpleGlow;  // Glow when value changes

    // Text
    ImU32 textColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 labelColor = CyberpunkTheme::Colors::TextSecondary;

    // Drag indicator (shows when draggable)
    ImU32 dragIndicatorColor = CyberpunkTheme::Colors::AccentCyan;
    float dragIndicatorWidth = 2.0f;
    float dragIndicatorPadding = 3.0f;

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;

    // Animation
    float animSpeed = 0.15f;
    float changeFeedbackDuration = 0.3f;  // Duration of value change glow
    int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};

} // namespace EFIGUI
