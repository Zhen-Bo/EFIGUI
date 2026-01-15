// src/EFIGUI/Styles/VectorSliderStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for VectorSlider components (SliderFloat2/3/4, SliderInt2/3/4)
/// Follows the same pattern as VectorInputStyle with additional slider-specific fields.
struct VectorSliderStyle {
    // Frame colors (same as VectorInputStyle)
    ImU32 frameColor = CyberpunkTheme::Colors::Surface;
    ImU32 frameHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 frameActiveColor = CyberpunkTheme::Colors::SurfaceActive;
    ImU32 borderColor = CyberpunkTheme::Colors::Border;
    ImU32 borderHoverColor = CyberpunkTheme::Colors::AccentCyan;

    // Component label colors (X, Y, Z, W) - same as VectorInputStyle
    ImU32 xLabelColor = IM_COL32(255, 80, 80, 255);   // Red
    ImU32 yLabelColor = IM_COL32(80, 255, 80, 255);   // Green
    ImU32 zLabelColor = IM_COL32(80, 80, 255, 255);   // Blue
    ImU32 wLabelColor = IM_COL32(255, 200, 80, 255);  // Yellow

    // Component accent bars - same as VectorInputStyle
    ImU32 xAccentColor = IM_COL32(255, 80, 80, 200);
    ImU32 yAccentColor = IM_COL32(80, 255, 80, 200);
    ImU32 zAccentColor = IM_COL32(80, 80, 255, 200);
    ImU32 wAccentColor = IM_COL32(255, 200, 80, 200);
    float accentWidth = 3.0f;

    // Text - same as VectorInputStyle
    ImU32 textColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 labelColor = CyberpunkTheme::Colors::TextSecondary;

    // Glow - same as VectorInputStyle
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;

    // Slider-specific: Track colors
    ImU32 trackColor = CyberpunkTheme::Colors::Surface;
    ImU32 fillColorLeft = CyberpunkTheme::Colors::AccentPurple;
    ImU32 fillColorRight = CyberpunkTheme::Colors::AccentCyan;

    // Slider-specific: Knob colors
    ImU32 knobColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 knobActiveColor = CyberpunkTheme::Colors::AccentCyan;

    // Dimensions - base fields same as VectorInputStyle
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float padding = 4.0f;
    float componentSpacing = 4.0f;
    float labelWidth = 14.0f;

    // Slider-specific: Track dimensions
    float trackHeight = 4.0f;
    float knobRadius = 6.0f;

    // Animation - same as VectorInputStyle
    float animSpeed = 0.15f;
    int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};

} // namespace EFIGUI
